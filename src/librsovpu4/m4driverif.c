/*****************************************************************************
 *                                                                           *
 *  m4driverif.c                                                             *
 *  Copyright (C) Renesas Technology Corp., 2007. All rights reserved.       *
 *****************************************************************************/
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
#include <asm/page.h>
#include <sh7722_vpu.h>
#include <m4iph_vpu4.h>
#include <avcbd.h>
#include "avcbd_optionaldata.h"
#include "avcbd_inner_typedef.h"

#define SDR_SIZE	(1024*1024*1)
#define VPU_DEV		"/dev/vpu"
#define	MEM_DEV		"/dev/mem"
#define FLG_VPUEND	0x08
                            
extern void *global_context;
static int vpufd, memfd;
static int clock_on,sleep_time;
static unsigned long sdr_base, sdr_start, sdr_end;
unsigned long _BM4VSD_BGN, _BM4VSD_END;
pthread_mutex_t vpu_mutex = PTHREAD_MUTEX_INITIALIZER;

static void usr_memcpy32(unsigned long *dst, unsigned long *src, long size);
int avcbd_idr_adjust( void *context );

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
	struct timeval tv,tv1;
	struct timezone tz;
	long tm;

	gettimeofday(&tv, &tz);
#ifdef DISABLE_INT
	while( m4iph_vpu4_status()!=0);
	m4iph_vpu4_int_handler();
#else
	/* Wait for an IRQ from the VPU. */
	ioctl(vpufd, VPU4CTRL_SLEEPON, NULL);
	m4iph_vpu4_int_handler();
#endif
	gettimeofday(&tv1, &tz);
	tm = (tv1.tv_usec-tv.tv_usec)/1000;
	if ( tm<0 ) {
		tm = 1000-(tv.tv_usec-tv1.tv_usec)/1000;
	}
	sleep_time += tm;
	avcbd_idr_adjust( global_context );
	return 0;
}

void m4iph_restart(void)
{
	/* Do nothing for Linux */
}

int m4iph_vpu_open(void)
{
	if ((vpufd = open(VPU_DEV, O_RDWR)) < 0) {
		vpufd = 0;
		return -1;
	}
	if (!clock_on) {
		ioctl(vpufd, VPU4CTRL_CLKON, NULL);
		clock_on = 1;
	}
	return 0;
}

void m4iph_vpu_close(void)
{
	close(vpufd);
	vpufd = 0;
	if (clock_on) {
		ioctl(vpufd, VPU4CTRL_CLKOFF, NULL);
		clock_on = 0;
	}
}

unsigned long m4iph_reg_table_read(unsigned long *addr, unsigned long *data, long size)
{
	unsigned long offset;

	offset = (unsigned long)addr - VP4_CTRL;
	if (lseek(vpufd, (off_t)offset, SEEK_SET) == (off_t)-1) {
		fprintf(stderr, "%s: Could not seek to register %lx\n",	__FUNCTION__, offset);
		return 0;
	} 
	if (read(vpufd, data, size) < 0) {
		perror("m4iph_reg_table_read");
		return 0;
	}
#if DEBUG
	for (offset = 0; offset < size; offset++)
		printf("%s: addr = %p, data = %08lx\n", __FUNCTION__, addr++, *data++);
#endif
	return size;
}

void m4iph_reg_table_write(unsigned long *addr, unsigned long *data, long size)
{
	unsigned long offset;

	offset = (unsigned long)addr - VP4_CTRL;
	if (lseek(vpufd, (off_t)offset, SEEK_SET) == (off_t)-1) {
		fprintf(stderr, "Could not seek to register %lx\n", offset);
		return;
	} 
	if (write(vpufd, data, size) < 0) 
		perror("m4iph_reg_table_write");
#if DEBUG
	for (offset = 0; offset < size; offset++) {
		printf("%s: addr = %p, data = %08lx\n", __FUNCTION__, addr, *data);
		addr++;
		data++;
	}
#endif
}

int m4iph_sdr_open(void)
{
	if ((memfd = open(MEM_DEV, O_RDWR)) < 0) {
		perror(MEM_DEV);
		memfd = 0;
		return -1;
	}
	if (ioctl(vpufd, VPU4CTRL_GET_SDR_BASE, &sdr_start)) {
		perror("VPU4CTRL_GET_SDR_BASE");
		return -1;
	}
	sdr_base = sdr_start;
	sdr_end = sdr_base + SDR_SIZE;
	return 0;
}

void m4iph_sdr_close(void)
{
	if (memfd > 0)
		close(memfd);
	memfd = 0;
	sdr_base = sdr_start = sdr_end = 0;
}

void *m4iph_map_sdr_mem(void *address, int size)
{
	void *ret;

	ret = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, (off_t)address);
	if (ret == (void *)MAP_FAILED)
		return NULL;
	return ret;
}

int m4iph_unmap_sdr_mem(void *address, int size)
{
	return munmap(address, size);
}

int m4iph_sync_sdr_mem(void *address, int size)
{
	return msync(address, size, MS_SYNC);
}

unsigned long m4iph_sdr_read(unsigned char *address, unsigned char *buffer,
				unsigned long count)
{
	unsigned char *buf;
	unsigned long addr;
	int roundoff;

	if ((unsigned long)address + count >= sdr_end || (unsigned long)address < sdr_start) {
		fprintf(stderr, "%s: Invalid read from SDR memory. ", __FUNCTION__);
		fprintf(stderr, "address = %p, count = %ld\n", address,	count);
		return 0;
	}
	addr = (unsigned long)address & ~(PAGE_SIZE - 1);
	roundoff = (unsigned long)address - addr;
	buf = (unsigned char *)m4iph_map_sdr_mem((void *)addr, count + roundoff);
	if (buf == NULL) {
		printf("%s: Aborting since mmap() failed.\n", __FUNCTION__);
		printf("%s: address = %p, buffer = %p, count = %ld\n", __FUNCTION__, address, buffer, count);
		abort();
	}
	memcpy(buffer, buf + roundoff, count);
	m4iph_unmap_sdr_mem(buf, count + roundoff);
	return count;
}

void m4iph_sdr_write(unsigned char *address, unsigned char *buffer, 
		unsigned long count)
{
	unsigned char *buf;
	unsigned long addr;
	int roundoff;

	if ((unsigned long)address + count >= sdr_end || (unsigned long)address < sdr_start) {
		fprintf(stderr, "%s: Invalid write to SDR memory. ", __FUNCTION__);
		fprintf(stderr, "address = %p, count = %ld\n", address, count);
		return;
	}
	addr = (unsigned long)address & ~(PAGE_SIZE - 1);
	roundoff = (unsigned long)address - addr;
	buf = (unsigned char *)m4iph_map_sdr_mem((void *)addr, count + roundoff);
	if (buf == NULL) {
		printf("%s: Aborting since mmap() failed.\n", __FUNCTION__);
		printf("%s: address = %p, buffer = %p, count = %ld\n", __FUNCTION__, address, buffer, count);
		abort();
	}
	memcpy(buf + roundoff, buffer, count);
	m4iph_unmap_sdr_mem(buf, count + roundoff);
}

void m4iph_sdr_memset(unsigned long *address, unsigned long data, unsigned long count)
{
	unsigned char *buf;
	unsigned long addr;
	int roundoff;

	addr = (unsigned long)address & ~(PAGE_SIZE - 1);
	roundoff = (unsigned long)address - addr;
	buf = (unsigned char *)m4iph_map_sdr_mem((void *)addr, count + roundoff);
	if (buf == NULL) {
		printf("%s: Aborting since mmap() failed.\n", __FUNCTION__);
		printf("%s: address = %p, data = %08lx, count = %ld\n", __FUNCTION__, address, data, count);
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
		fprintf(stderr, "%s: Allocation of size %ld failed\n", __FUNCTION__, count);
		printf("sdr_base = %08lx, sdr_end = %08lx\n", sdr_base, sdr_end);
		return NULL;
	}
	sdr_base += size;
	return (void *)ret;
}

void m4iph_sdr_free(void *address, unsigned long count)
{
	fprintf(stderr, "%s() is not required\n", __FUNCTION__);
}

float logf(float x)
{
	return (float)log((double)x);
}

float ceilf(float x)
{
	return (float)ceil((double)x);
}

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
	return(ioctl(vpufd,VPU4CTRL_CLKON,NULL));
}

int vpu4_clock_off(void)
{
	return(ioctl(vpufd,VPU4CTRL_CLKOFF,NULL));
}

int avcbd_idr_adjust( void *context )
{
	TAVCBD_VARIABLES *var;

	if ( ( !context ) || ( (short)context & 3 ) ) {
		return AVCBD_PARAM_ERROR;
	}
	var = (TAVCBD_VARIABLES *)context;
	if ( var->id != 1 ) return 0;
	if ( var->seq_param->nal_unit_type == 5 ) {
		var->poc_info.prev_frame_num = 0;
	}
	return 0;
}
