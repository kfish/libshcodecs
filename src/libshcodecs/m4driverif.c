/*
 * libshcodecs: A library for controlling SH-Mobile hardware codecs
 * Copyright (C) 2009 Renesas Technology Corp.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA  02110-1301 USA
 */

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sh7722_vpu.h>
#include <m4iph_vpu4.h>
#include <avcbd.h>
#include "avcbd_optionaldata.h"
#include "avcbd_inner_typedef.h"

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

struct uio_device {
	char *name;
	char *path;
	int fd;
};

#define MAXUIOIDS  100
#define MAXNAMELEN 256

static int locate_uio_device(char *name, struct uio_device *udp)
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
		perror(buf);
		return -1;
	}

	return 0;
}

struct uio_map {
	unsigned long address;
	unsigned long size;
	void *iomem;
};

static int setup_uio_map(struct uio_device *udp, int nr,
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

struct uio_device uio_dev;
struct uio_map uio_mmio, uio_mem;

/* extern void *global_context; */
static int sleep_time;
static unsigned long sdr_base, sdr_start, sdr_end;
unsigned long _BM4VSD_BGN, _BM4VSD_END;
pthread_mutex_t vpu_mutex = PTHREAD_MUTEX_INITIALIZER;

static void usr_memcpy32(unsigned long *dst, unsigned long *src,
			 long size);
int avcbd_idr_adjust(void *context);

/* User defined functions as specified by the Encoder/Decoder middleware
 * documents.
 */
void m4iph_sleep_time_init(void)
{
	sleep_time = 0;
}

unsigned long m4iph_sleep_time_get(void)
{
	return sleep_time;
}

void m4iph_start(void)
{
}

long m4iph_sleep(void)
{
	struct timeval tv, tv1;
	struct timezone tz;
	long tm;

	gettimeofday(&tv, &tz);
#ifdef DISABLE_INT
	while (m4iph_vpu4_status() != 0);
	m4iph_vpu4_int_handler();
#else
	/* Enable interrupt in UIO driver */
	{
		unsigned long enable = 1;

		write(uio_dev.fd, &enable, sizeof(u_long));
	}

	/* Wait for an interrupt */
	{
		unsigned long n_pending;

		read(uio_dev.fd, &n_pending, sizeof(u_long));
	}

	m4iph_vpu4_int_handler();
#endif
	gettimeofday(&tv1, &tz);
	tm = (tv1.tv_usec - tv.tv_usec) / 1000;
	if (tm < 0) {
		tm = 1000 - (tv.tv_usec - tv1.tv_usec) / 1000;
	}
	sleep_time += tm;
	/* avcbd_idr_adjust(global_context); */
	return 0;
}

void m4iph_restart(void)
{
	/* Do nothing for Linux */
}

int m4iph_vpu_open(void)
{
	int ret;

	ret = locate_uio_device("VPU", &uio_dev);
	if (ret < 0)
		return ret;

	printf("found matching UIO device at %s\n", uio_dev.path);

	ret = setup_uio_map(&uio_dev, 0, &uio_mmio);
	if (ret < 0)
		return ret;

	ret = setup_uio_map(&uio_dev, 1, &uio_mem);
	if (ret < 0)
		return ret;

	return 0;
}

void m4iph_vpu_close(void)
{
}

unsigned long m4iph_reg_table_read(unsigned long *addr,
				   unsigned long *data, long nr_longs)
{
	unsigned long *reg_base = uio_mmio.iomem;
	int k;

	reg_base += ((unsigned long) addr - VP4_CTRL) / 4;

	for (k = 0; k < nr_longs; k++)
		data[k] = reg_base[k];

#if DEBUG
	for (offset = 0; offset < nr_longs; offset++)
		printf("%s: addr = %p, data = %08lx\n", __FUNCTION__,
		       addr++, *data++);
#endif
	return nr_longs;
}

void m4iph_reg_table_write(unsigned long *addr, unsigned long *data,
			   long nr_longs)
{
	unsigned long *reg_base = uio_mmio.iomem;
	int k;

	reg_base += ((unsigned long) addr - VP4_CTRL) / 4;

	for (k = 0; k < nr_longs; k++)
		reg_base[k] = data[k];

#if DEBUG
	for (offset = 0; offset < nr_longs; offset++) {
		printf("%s: addr = %p, data = %08lx\n", __FUNCTION__, addr,
		       *data);
		addr++;
		data++;
	}
#endif
}

int m4iph_sdr_open(void)
{
	sdr_base = sdr_start = uio_mem.address;
	sdr_end = sdr_base + uio_mem.size;
	return 0;
}

void m4iph_sdr_close(void)
{
	sdr_base = sdr_start = sdr_end = 0;
}

void *m4iph_map_sdr_mem(void *address, int size)
{
	return uio_mem.iomem + ((unsigned long) address - uio_mem.address);
}

int m4iph_unmap_sdr_mem(void *address, int size)
{
	return 0;
}

int m4iph_sync_sdr_mem(void *address, int size)
{
	return 0;
}

unsigned long m4iph_sdr_read(unsigned char *address, unsigned char *buffer,
			     unsigned long count)
{
	unsigned char *buf;
	unsigned long addr;
	int roundoff;
	int pagesize = getpagesize();

	if ((unsigned long) address + count >= sdr_end
	    || (unsigned long) address < sdr_start) {
		fprintf(stderr, "%s: Invalid read from SDR memory. ",
			__FUNCTION__);
		fprintf(stderr, "address = %p, count = %ld\n", address,
			count);
		return 0;
	}
	addr = (unsigned long) address & ~(pagesize - 1);
	roundoff = (unsigned long) address - addr;
	buf =
	    (unsigned char *) m4iph_map_sdr_mem((void *) addr,
						count + roundoff);
	if (buf == NULL) {
		printf("%s: Aborting since mmap() failed.\n",
		       __FUNCTION__);
		printf("%s: address = %p, buffer = %p, count = %ld\n",
		       __FUNCTION__, address, buffer, count);
		abort();
	}
	memcpy(buffer, buf + roundoff, count);
	m4iph_unmap_sdr_mem(buf, count + roundoff);
	return count;
}

/* Same arg order as memcpy; does alignment on dest */
void m4iph_sdr_write(unsigned char *address, unsigned char *buffer,
		     unsigned long count)
{
	unsigned char *buf;
	unsigned long addr;
	int roundoff;
	int pagesize = getpagesize();

	if ((unsigned long) address + count >= sdr_end
	    || (unsigned long) address < sdr_start) {
		fprintf(stderr, "%s: Invalid write to SDR memory. ",
			__FUNCTION__);
		fprintf(stderr, "address = %p, count = %ld\n", address,
			count);
		return;
	}
	addr = (unsigned long) address & ~(pagesize - 1);
	roundoff = (unsigned long) address - addr;
	buf =
	    (unsigned char *) m4iph_map_sdr_mem((void *) addr,
						count + roundoff);
	if (buf == NULL) {
		printf("%s: Aborting since mmap() failed.\n",
		       __FUNCTION__);
		printf("%s: address = %p, buffer = %p, count = %ld\n",
		       __FUNCTION__, address, buffer, count);
		abort();
	}
	memcpy(buf + roundoff, buffer, count);
	m4iph_unmap_sdr_mem(buf, count + roundoff);
}

void m4iph_sdr_memset(unsigned long *address, unsigned long data,
		      unsigned long count)
{
	unsigned char *buf;
	unsigned long addr;
	int roundoff;
	int pagesize = getpagesize();

	addr = (unsigned long) address & ~(pagesize - 1);
	roundoff = (unsigned long) address - addr;
	buf =
	    (unsigned char *) m4iph_map_sdr_mem((void *) addr,
						count + roundoff);
	if (buf == NULL) {
		printf("%s: Aborting since mmap() failed.\n",
		       __FUNCTION__);
		printf("%s: address = %p, data = %08lx, count = %ld\n",
		       __FUNCTION__, address, data, count);
		abort();
	}
	memset(buf, data, count);
	m4iph_unmap_sdr_mem(buf, count + roundoff);
}

/* Allocate sdr memory */
void *m4iph_sdr_malloc(unsigned long count, int align)
{
	unsigned long ret;
	int size;

	ret = ((sdr_base + (align - 1)) & ~(align - 1));
	size = ret - sdr_base + count;

	if (sdr_base + size >= sdr_end) {
		fprintf(stderr, "%s: Allocation of size %ld failed\n",
			__FUNCTION__, count);
		printf("sdr_base = %08lx, sdr_end = %08lx\n", sdr_base,
		       sdr_end);
		return NULL;
	}
	sdr_base += size;
	return (void *) ret;
}

void m4iph_sdr_free(void *address, unsigned long count)
{
	fprintf(stderr, "%s() is not required\n", __FUNCTION__);
}

#if 0
float logf(float x)
{
	return (float) log((double) x);
}

float ceilf(float x)
{
	return (float) ceil((double) x);
}
#endif

void m4iph_avcbd_perror(char *msg, int error)
{
	fprintf(stderr, "%s: ", msg);
	switch (error) {
	case AVCBD_PARAM_ERROR:
		fprintf(stderr, "Parameter Error\n");
		break;
	case AVCBD_PIC_TIMEOUT:
		fprintf(stderr, "Timeout\n");
		break;
	case AVCBD_PIC_ABORT:
		fprintf(stderr, "Abort\n");
		break;
	case AVCBD_PIC_EOS:
		fprintf(stderr, "End of Stream\n");
		break;
	case AVCBD_PIC_LARGE:
		fprintf(stderr, "Picture too large\n");
		break;
	case AVCBD_PIC_FEWERMB:
		fprintf(stderr, "AVCBD_PIC_FEWERMB Error\n");
		break;
	case AVCBD_PIC_FMTERROR:
		fprintf(stderr, "Format Error\n");
		break;
	case AVCBD_PIC_ERROR:
		fprintf(stderr, "Picture Error\n");
		break;
	case AVCBD_MB_OVERRUN:
		fprintf(stderr, "Overrun Error\n");
		break;
	case AVCBD_ILLEGAL_DATA:
		fprintf(stderr, "Illegal Data\n");
		break;
	case AVCBD_ILLEGAL_CODE:
		fprintf(stderr, "Illegal Code\n");
		break;
	case AVCBD_ILLEGAL_MTYPE:
		fprintf(stderr, "Illegal Mtype\n");
		break;
	case AVCBD_BUFFER_OVERRUN:
		fprintf(stderr, "Buffer Overrun\n");
		break;
	case AVCBD_PIC_EXCESS_FRAME:
		fprintf(stderr, "Excess Frames\n");
		break;
	case AVCBD_PIC_EXCESS_PPS:
		fprintf(stderr, "Excess PPS\n");
		break;
	case AVCBD_TEMPORARY_BUFFER_SHORT_ERROR:
		fprintf(stderr, "Temporary buffer too short\n");
		break;
	case AVCBD_VPU_ERROR:
		fprintf(stderr, "General VPU Error\n");
		break;
	default:
		fprintf(stderr, "Unknown error %d\n", error);
	}
}

void m4iph_avcbe_perror(char *msg, int error)
{
	fprintf(stderr, "%s: %d\n", msg, error);
}

void avcbd_get_time_code(long h, long m, long s)
{
}

int vpu4_clock_on(void)
{
	return 0;
}

int vpu4_clock_off(void)
{
	return 0;
}

int avcbd_idr_adjust(void *context)
{
	TAVCBD_VARIABLES *var;

	if ((!context) || ((int) context & 3)) {
		return AVCBD_PARAM_ERROR;
	}
	var = (TAVCBD_VARIABLES *) context;
	if (var->id != 1)
		return 0;
	if (var->seq_param->nal_unit_type == 5) {
		var->poc_info.prev_frame_num = 0;
	}
	return 0;
}
