/*
 * SuperH VEU color space conversion and stretching
 * Based on MPlayer Vidix driver by Magnus Damm
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <pthread.h>
#include <semaphore.h>

#include <inttypes.h>
#include <unistd.h>
#include <errno.h>

#include "config.h"

#define SH_VEU_RESERVE_TOP (512 << 10)
#define YUV_COLOR
#define CACHED_UV
//#define USE_THREADS

static int fgets_with_openclose(char *fname, char *buf, size_t maxlen)
{
	FILE *fp;

	if ((fp = fopen(fname, "r")) != NULL) {
		fgets(buf, maxlen, fp);
		fclose(fp);
		return strlen(buf);
	} else {
		return -1;
	}
}

struct sh_veu_uio_device {
	char *name;
	char *path;
	int fd;
};

#define MAXUIOIDS  100
#define MAXNAMELEN 256

static int locate_sh_veu_uio_device(char *name,
				    struct sh_veu_uio_device *udp)
{
	char fname[MAXNAMELEN], buf[MAXNAMELEN];
	int uio_id, i;

	for (uio_id = 0; uio_id < MAXUIOIDS; uio_id++) {
		sprintf(fname, "/sys/class/uio/uio%d/name", uio_id);
		if (fgets_with_openclose(fname, buf, MAXNAMELEN) < 0)
			continue;
		if (strncmp(name, buf, strlen(name)) == 0)
			break;
	}

	if (uio_id >= MAXUIOIDS)
		return -1;

	udp->name = strdup(buf);
	udp->path = strdup(fname);
	udp->path[strlen(udp->path) - 4] = '\0';

	sprintf(buf, "/dev/uio%d", uio_id);
	udp->fd = open(buf, O_RDWR | O_SYNC /*| O_NONBLOCK */ );

	if (udp->fd < 0) {
		perror("open");
		return -1;
	}

	return 0;
}

struct uio_map {
	unsigned long address;
	unsigned long size;
	void *iomem;
};

static int setup_uio_map(struct sh_veu_uio_device *udp, int nr,
			 struct uio_map *ump)
{
	char fname[MAXNAMELEN], buf[MAXNAMELEN];

	sprintf(fname, "%s/maps/map%d/addr", udp->path, nr);
	if (fgets_with_openclose(fname, buf, MAXNAMELEN) <= 0)
		return -1;

	ump->address = strtoul(buf, NULL, 0);

	sprintf(fname, "%s/maps/map%d/size", udp->path, nr);
	if (fgets_with_openclose(fname, buf, MAXNAMELEN) <= 0)
		return -1;

	ump->size = strtoul(buf, NULL, 0);

	ump->iomem = mmap(0, ump->size,
			  PROT_READ | PROT_WRITE, MAP_SHARED,
			  udp->fd, nr * getpagesize());

	if (ump->iomem == MAP_FAILED)
		return -1;

	return 0;
}

#define VESTR 0x00		/* start register */
#define VESWR 0x10		/* src: line length */
#define VESSR 0x14		/* src: image size */
#define VSAYR 0x18		/* src: y/rgb plane address */
#define VSACR 0x1c		/* src: c plane address */
#define VBSSR 0x20		/* bundle mode register */
#define VEDWR 0x30		/* dst: line length */
#define VDAYR 0x34		/* dst: y/rgb plane address */
#define VDACR 0x38		/* dst: c plane address */
#define VTRCR 0x50		/* transform control */
#define VRFCR 0x54		/* resize scale */
#define VRFSR 0x58		/* resize clip */
#define VENHR 0x5c		/* enhance */
#define VFMCR 0x70		/* filter mode */
#define VVTCR 0x74		/* lowpass vertical */
#define VHTCR 0x78		/* lowpass horizontal */
#define VAPCR 0x80		/* color match */
#define VECCR 0x84		/* color replace */
#define VAFXR 0x90		/* fixed mode */
#define VSWPR 0x94		/* swap */
#define VEIER 0xa0		/* interrupt mask */
#define VEVTR 0xa4		/* interrupt event */
#define VSTAR 0xb0		/* status */
#define VBSRR 0xb4		/* reset */

#define VMCR00 0x200		/* color conversion matrix coefficient 00 */
#define VMCR01 0x204		/* color conversion matrix coefficient 01 */
#define VMCR02 0x208		/* color conversion matrix coefficient 02 */
#define VMCR10 0x20c		/* color conversion matrix coefficient 10 */
#define VMCR11 0x210		/* color conversion matrix coefficient 11 */
#define VMCR12 0x214		/* color conversion matrix coefficient 12 */
#define VMCR20 0x218		/* color conversion matrix coefficient 20 */
#define VMCR21 0x21c		/* color conversion matrix coefficient 21 */
#define VMCR22 0x220		/* color conversion matrix coefficient 22 */
#define VCOFFR 0x224		/* color conversion offset */
#define VCBR   0x228		/* color conversion clip */

/* Helper functions for reading registers. */

static unsigned long read_reg(struct uio_map *ump, int reg_nr)
{
	volatile unsigned long *reg = ump->iomem + reg_nr;

	return *reg;
}

static void write_reg(struct uio_map *ump, unsigned long value, int reg_nr)
{
	volatile unsigned long *reg = ump->iomem + reg_nr;

	*reg = value;
}

static void set_scale(struct uio_map *ump, int vertical,
		      int size_in, int size_out)
{
	unsigned long fixpoint, mant, frac, value;

	/* calculate FRAC and MANT */

	fixpoint = (4096 * (size_in - 1)) / (size_out + 1);
	mant = fixpoint / 4096;
	frac = fixpoint - (mant * 4096);

	if (frac & 0x07) {
		frac &= ~0x07;

		if (size_out > size_in)
			frac -= 8;	/* round down if scaling up */
		else
			frac += 8;	/* round up if scaling down */
	}

	/* set scale */

	value = read_reg(ump, VRFCR);

	if (vertical) {
		value &= ~0xffff0000;
		value |= ((mant << 12) | frac) << 16;
	} else {
		value &= ~0xffff;
		value |= (mant << 12) | frac;
	}

	write_reg(ump, value, VRFCR);

	/* set clip */

	value = read_reg(ump, VRFSR);

	if (vertical) {
		value &= ~0xffff0000;
		value |= size_out << 16;
	} else {
		value &= ~0xffff;
		value |= size_out;
	}

	write_reg(ump, value, VRFSR);
}

/* global variables yuck */

struct sh_veu_uio_device sh_veu_uio_dev;
struct uio_map sh_veu_uio_mmio, sh_veu_uio_mem;

static int sh_veu_probe(int verbose, int force)
{
	unsigned long addr;
	int src_w, src_h, src_bpp;
	int dst_w, dst_h;
	int ret;

	ret = locate_sh_veu_uio_device("VEU", &sh_veu_uio_dev);
	if (ret < 0)
		return ret;

	printf("found matching UIO device at %s\n", sh_veu_uio_dev.path);

	ret = setup_uio_map(&sh_veu_uio_dev, 0, &sh_veu_uio_mmio);
	if (ret < 0)
		return ret;

	ret = setup_uio_map(&sh_veu_uio_dev, 1, &sh_veu_uio_mem);
	if (ret < 0)
		return ret;

	return ret;
}

//struct sh_veu_plane _src, _dst, _tmp;

#ifdef USE_THREADS
sem_t sh_veu_blocker;
pthread_mutex_t sh_veu_busy;
pthread_t sh_veu_thread;
int sh_veu_frame;
#endif

//void *cached_data;

//static int YOffs,UOffs,VOffs,UVOffs;


#if 0
static int sh_veu_do_copy(vidix_playback_t * info, int frame)
{
#if defined(YUV_COLOR)
	int k;
	unsigned char *u, *v;
	unsigned short *uv;

	/* hack: do software conversion of planar U and V
	 *       to single interleaved UV plane.
	 */

#ifdef CACHED_UV
	v = cached_data + info->offsets[frame] + VOffs;
	u = cached_data + info->offsets[frame] + UOffs;
#else
	v = sh_veu_uio_mem.iomem + info->offsets[frame] + VOffs;
	u = sh_veu_uio_mem.iomem + info->offsets[frame] + UOffs;
#endif
	uv = (unsigned char *) sh_veu_uio_mem.iomem +
	    info->offsets[frame] + UVOffs;

	for (k = 0; k < (UOffs / 2); k++)
		uv[k] = v[k] | (u[k] << 8);
#endif
}
#endif

#if 0
struct sh_veu_plane {
	unsigned long width;
	unsigned long height;
	unsigned long stride;
	unsigned long phys_addr_y;
	unsigned long phys_addr_c;
};

static int sh_veu_do_blit(vidix_playback_t * info, int do_rgb,
			  struct sh_veu_plane *src,
			  struct sh_veu_plane *dst)
{

	write_reg(&sh_veu_uio_mmio, src->stride, VESWR);
	write_reg(&sh_veu_uio_mmio, src->width | (src->height << 16),
		  VESSR);
	write_reg(&sh_veu_uio_mmio, 0, VBSSR);	/* not using bundle mode */

	write_reg(&sh_veu_uio_mmio, dst->stride, VEDWR);
	write_reg(&sh_veu_uio_mmio, dst->phys_addr_y, VDAYR);
	write_reg(&sh_veu_uio_mmio, dst->phys_addr_c, VDACR);	/* unused for RGB */

	if (do_rgb) {
		write_reg(&sh_veu_uio_mmio, 0x66, VSWPR);
		write_reg(&sh_veu_uio_mmio, (6 << 16) | (3 << 8) | 1,
			  VTRCR);
	} else {
		write_reg(&sh_veu_uio_mmio, 0x67, VSWPR);
		write_reg(&sh_veu_uio_mmio, (6 << 16) | (0 << 14) | 2 | 4,
			  VTRCR);

		if (sh_veu_uio_mmio.size > VBSRR) {	/* sh7723 */
			write_reg(&sh_veu_uio_mmio, 0x0cc5, VMCR00);
			write_reg(&sh_veu_uio_mmio, 0x0950, VMCR01);
			write_reg(&sh_veu_uio_mmio, 0x0000, VMCR02);

			write_reg(&sh_veu_uio_mmio, 0x397f, VMCR10);
			write_reg(&sh_veu_uio_mmio, 0x0950, VMCR11);
			write_reg(&sh_veu_uio_mmio, 0x3ccd, VMCR12);

			write_reg(&sh_veu_uio_mmio, 0x0000, VMCR20);
			write_reg(&sh_veu_uio_mmio, 0x0950, VMCR21);
			write_reg(&sh_veu_uio_mmio, 0x1023, VMCR22);

			write_reg(&sh_veu_uio_mmio, 0x00800010, VCOFFR);
		}
	}

	set_scale(&sh_veu_uio_mmio, 0, src->width, dst->width);
	set_scale(&sh_veu_uio_mmio, 1, src->height, dst->height);

	write_reg(&sh_veu_uio_mmio, src->phys_addr_y, VSAYR);
	write_reg(&sh_veu_uio_mmio, src->phys_addr_c, VSACR);	/* unused for RGB */

	write_reg(&sh_veu_uio_mmio, 1, VEIER);	/* enable interrupt in VEU */

	/* Enable interrupt in UIO driver */
	{
		unsigned long enable = 1;

		write(sh_veu_uio_dev.fd, &enable, sizeof(u_long));
	}

	write_reg(&sh_veu_uio_mmio, 1, VESTR);	/* start operation */

	/* Wait for an interrupt */
	{
		unsigned long n_pending;

		read(sh_veu_uio_dev.fd, &n_pending, sizeof(u_long));
	}

	write_reg(&sh_veu_uio_mmio, 0x100, VEVTR);	/* ack int, write 0 to bit 0 */
}
#endif

#if 0
void sh_veu_setup_planes(vidix_playback_t * info,
			 struct sh_veu_plane *src,
			 struct sh_veu_plane *dst,
			 struct sh_veu_plane *tmp)
{
	unsigned long addr;
	char *envstr;

	src->width = info->src.w;
	src->height = info->src.h;
	src->stride = (info->src.w + 15) & ~15;

	dst->width = fbi.width;
	dst->height = fbi.height;
	dst->stride = fbi.line_length;

	/* aspect ratio calculations hack - use mplayer info later */
	if (src->width == 320 && src->height == 240) {
		dst->width = 640;
		dst->height = 472;
	}

	if (src->width == 640 && src->height == 480) {
		dst->width = 640;
		dst->height = 472;
	}

	tmp->width = 0;

	envstr = getenv("sh_veu_mode");
	envstr = envstr ? envstr : "";

	while (dst->width == 800 && dst->height == 480) {
		if (!strcmp(envstr, "buf=220x140")) {
			tmp->width = 220;
			tmp->height = 140;
			break;
		}

		if (!strcmp(envstr, "buf=256x240")) {
			tmp->width = 256;
			tmp->height = 240;
			dst->width = 800;
			dst->height = 472;
			break;
		}

		if (!strcmp(envstr, "buf=440x280")) {
			tmp->width = 440;
			tmp->height = 280;
			break;
		}

		if (!strcmp(envstr, "buf=512x480")) {
			tmp->width = 512;
			tmp->height = 480;
			dst->width = 800;
			dst->height = 472;
			break;
		}
		break;
	}

	/* center on frame buffer */
	addr = fbi.address;
	addr += ((fbi.width - dst->width) / 2) * (fbi.bpp / 8);
	addr += dst->stride * ((fbi.height - dst->height) / 2);

	dst->phys_addr_y = addr;

	/* setup mid buffer */
	tmp->stride = tmp->width * 2;
	addr = sh_veu_uio_mem.address + sh_veu_uio_mem.size;
	addr -= SH_VEU_RESERVE_TOP;
	tmp->phys_addr_y = addr;

	printf("stretching from %ldx%ld to %ldx%ld\n",
	       src->width, src->height, dst->width, dst->height);

	if (tmp->width)
		printf("using %ldx%ld middle buffer\n", tmp->width,
		       tmp->height);
}
#endif

#if 0
void sh_veu_draw(vidix_playback_t * info,
		 int frame,
		 struct sh_veu_plane *src,
		 struct sh_veu_plane *dst, struct sh_veu_plane *tmp)
{
	unsigned long addr;

	sh_veu_do_copy(info, frame);

	addr = sh_veu_uio_mem.address + info->offsets[frame];
	src->phys_addr_y = addr + YOffs;
	src->phys_addr_c = addr + UVOffs;

	if (tmp->width == 0) {
		sh_veu_do_blit(info, 0, src, dst);
	} else {
		sh_veu_do_blit(info, 0, src, tmp);
		sh_veu_do_blit(info, 1, tmp, dst);
	}
}

#ifdef USE_THREADS

void *sh_veu_thread_fn(void *arg)
{
	while (1) {
		sem_wait(&sh_veu_blocker);

		pthread_mutex_lock(&sh_veu_busy);

		sh_veu_draw(&my_info, sh_veu_frame, &_src, &_dst, &_tmp);

		pthread_mutex_unlock(&sh_veu_busy);
	}

	return NULL;
}

#endif

#endif

static int sh_veu_init(void)
{
	/* reset VEU */
	write_reg(&sh_veu_uio_mmio, 0x100, VBSRR);
#ifdef USE_THREADS
	pthread_mutex_init(&sh_veu_busy, NULL);
	sem_init(&sh_veu_blocker, 0, 0);
#endif
	return 0;
}

static void sh_veu_destroy(void)
{
}


int sh_veu_open(void)
{
	sh_veu_probe(0, 0);
	sh_veu_init();
}

void sh_veu_close(void)
{
}

int
sh_veu_rgb565_to_nv12(unsigned char *rgb565_in,
		      unsigned char *y_out,
		      unsigned char *c_out,
		      unsigned long width, unsigned long height)
{
	unsigned long stride;

	printf("sh_veu_rgb565_to_nv12 IN\n");

	stride = (width + 15) & ~15;

	write_reg(&sh_veu_uio_mmio, stride * 2, VESWR);
	write_reg(&sh_veu_uio_mmio, width | (height << 16), VESSR);
	write_reg(&sh_veu_uio_mmio, 0, VBSSR);	/* not using bundle mode */

	write_reg(&sh_veu_uio_mmio, stride, VEDWR);

	write_reg(&sh_veu_uio_mmio, rgb565_in, VSAYR);

	write_reg(&sh_veu_uio_mmio, y_out, VDAYR);
	write_reg(&sh_veu_uio_mmio, c_out, VDACR);	/* unused for RGB */

	//write_reg(&sh_veu_uio_mmio, 0x66, VSWPR);
	write_reg(&sh_veu_uio_mmio, 0x60 | 0x07, VSWPR);
	//write_reg(&sh_veu_uio_mmio, (6 << 16) | (3 << 8) | 1, VTRCR);
	//write_reg(&sh_veu_uio_mmio, (5 << 8) | 3, VTRCR);
	write_reg(&sh_veu_uio_mmio, (6 << 16) | 2, VTRCR);

	set_scale(&sh_veu_uio_mmio, 0, width, width);
	set_scale(&sh_veu_uio_mmio, 1, height, height);

	write_reg(&sh_veu_uio_mmio, 1, VEIER);	/* enable interrupt in VEU */

	/* Enable interrupt in UIO driver */
	{
		unsigned long enable = 1;
		int ret;

		if ((ret =
		     write(sh_veu_uio_dev.fd, &enable,
			   sizeof(u_long))) != (sizeof(u_long))) {
			printf("veu csp: write error\n");
		}
	}

	write_reg(&sh_veu_uio_mmio, 1, VESTR);	/* start operation */

	/* Wait for an interrupt */
	{
		unsigned long n_pending;

		read(sh_veu_uio_dev.fd, &n_pending, sizeof(u_long));
	}

	write_reg(&sh_veu_uio_mmio, 0x100, VEVTR);	/* ack int, write 0 to bit 0 */

	printf("sh_veu_rgb565_to_nv12 OUT\n");
}

#if 0
static int sh_veu_get_caps(vidix_capability_t * to)
{
	memcpy(to, &sh_veu_cap, sizeof(vidix_capability_t));
	return 0;
}

static int is_supported_fourcc(uint32_t fourcc)
{
	switch (fourcc) {
	case IMGFMT_YV12:
		return 1;
	default:
		return 0;
	}
}

static int sh_veu_query_fourcc(vidix_fourcc_t * to)
{
	if (is_supported_fourcc(to->fourcc)) {
		to->depth = VID_DEPTH_ALL;
		to->flags = VID_CAP_EXPAND | VID_CAP_SHRINK;
		return 0;
	}
	to->depth = to->flags = 0;
	return ENOSYS;
}


static int sh_veu_config_playback(vidix_playback_t * info)
{
	int src_w, dst_w;
	int src_h, dst_h;
	int hscale, vscale;
	long base0;
	int y_pitch = 0, uv_pitch = 0;
	unsigned int i;
	unsigned long addr;
	unsigned long size;

	if (!is_supported_fourcc(info->fourcc))
		return -1;

	src_w = info->src.w;
	src_h = info->src.h;

	switch (info->fourcc) {
	case IMGFMT_YV12:
		y_pitch = (src_w + 15) & ~15;
		uv_pitch = ((src_w / 2) + 15) & ~15;

		YOffs = info->offset.y = 0;
		UOffs = info->offset.u = y_pitch * src_h;

		VOffs = info->offset.v =
		    info->offset.u + (uv_pitch * src_h / 2);
		UVOffs = info->offset.v + (uv_pitch * src_h / 2);
		info->frame_size = y_pitch * src_h * 3;
		break;
	default:
		exit(1);
	}

	size = sh_veu_uio_mem.size - SH_VEU_RESERVE_TOP;
	info->num_frames = size / info->frame_size;
	if (info->num_frames > VID_PLAY_MAXFRAMES)
		info->num_frames = VID_PLAY_MAXFRAMES;

	printf("using %d frames, size = %d, frame size = %d\n",
	       (int) info->num_frames, (int) size, (int) info->frame_size);

	info->dga_addr = sh_veu_uio_mem.iomem;

	info->dest.pitch.y = 16;
	info->dest.pitch.u = 16;
	info->dest.pitch.v = 16;

	for (i = 0; i < info->num_frames; i++)
		info->offsets[i] = info->frame_size * i;

#if defined(CACHED_UV)
	{
		cached_data = malloc(sh_veu_uio_mem.size);
		info->offset.u += (cached_data - (void *) info->dga_addr);
		info->offset.v += (cached_data - (void *) info->dga_addr);
	}
#endif
	my_info = *info;

	sh_veu_setup_planes(info, &_src, &_dst, &_tmp);
#ifdef USE_THREADS
	pthread_create(&sh_veu_thread, NULL, sh_veu_thread_fn, NULL);
#endif
	return 0;
}


static int sh_veu_playback_on(void)
{
	return 0;
}


static int sh_veu_playback_off(void)
{
	return 0;
}

int sh_veu_framedrop;

static int sh_veu_frame_sel(unsigned int frame)
{
#ifdef USE_THREADS
	if (pthread_mutex_trylock(&sh_veu_busy) == 0) {
		sh_veu_frame = frame;
		sem_post(&sh_veu_blocker);
		pthread_mutex_unlock(&sh_veu_busy);
	} else
		printf("sh_veu_framefrop = %d  -\n", ++sh_veu_framedrop);
#else
	sh_veu_draw(&my_info, frame, &_src, &_dst, &_tmp);
#endif
	return 0;
}
#endif
