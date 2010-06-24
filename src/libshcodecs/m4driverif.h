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

#ifndef _M4DRIVERIF_H_
#define _M4DRIVERIF_H_

/* Align address in w bytes boundary. VPU needs 16 bytes alignment.
 * Length of a cache line of SH4/SH4AL-DSP is 32 bytes.
 */
#define ALIGN_NBYTES(a, w) \
	(void *)(((unsigned long)(a) + (w) - 1) & ~((w) - 1))

#define CHECK_ALLOC(ptr, size, errmsg, label)				\
	do {								\
		if ((ptr) == NULL) {					\
			fprintf(stderr, "Failed to allocate %d bytes for " \
					errmsg ".\n", (size));		\
			goto label;					\
		}							\
	} while (0)


int m4iph_vpu_open(int stream_buf_size);
void m4iph_vpu_close(void);

void m4iph_vpu_lock(void);
void m4iph_vpu_unlock(void);

void *m4iph_sdr_malloc(unsigned long count, int align);
void m4iph_sdr_free(void *address, unsigned long count);
void m4iph_avcbd_perror(char *msg, int error);
void m4iph_avcbe_perror(char *msg, int error);
void *m4iph_addr_to_virt(void *address);

#endif
