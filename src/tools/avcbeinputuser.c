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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <setjmp.h>

#include "avcbencsmp.h"

#include "capture.h"

#include "veu_colorspace.h"

#include <shcodecs/shcodecs_encoder.h>

#include <time.h>		/* for timestamp struct
				   used in linux/videodev2.h*/
#include <linux/videodev2.h>	/* For pixel formats */

extern unsigned long my_yuv_data[];	/* array for input YUV data */

long frame_counter_of_input;	/* the number of input frames for stream-1 */

int open_input_image_file(APPLI_INFO * appli_info)
{
	frame_counter_of_input = 0;
	appli_info->input_yuv_fp = NULL;
	appli_info->input_yuv_fp =
	    fopen(appli_info->input_file_name_buf, "rb");

	if (appli_info->input_yuv_fp == NULL) {
		fprintf(stderr, "ERROR: can't open file %s \n",
		        appli_info->input_file_name_buf);
		return (-1);
	}

	return (0);
}

int open_output_file(APPLI_INFO * appli_info)
{
	appli_info->output_file_fp = NULL;

        if (!strcmp (appli_info->output_file_name_buf, "-")) {
	        appli_info->output_file_fp = stdout;
        } else {
	        appli_info->output_file_fp =
	            fopen(appli_info->output_file_name_buf, "wb");
        }

	if (appli_info->output_file_fp == NULL) {
		return (-1);
	}

	return (0);
}

struct capture {
	SHCodecs_Encoder *encoder;
	APPLI_INFO *appli_info;
};

static void
capture_image_cb(sh_ceu * ceu, const unsigned char *frame_data, size_t length,
		 void *user_data)
{
	struct capture *cap = (struct capture *) user_data;
	unsigned int pixel_format;
	int odd = 0;
	long hsiz, ysiz, i, j;
	const unsigned char *d;
	const unsigned short *ds;
	short rgb;
	unsigned char *y, *c;
	unsigned char *w_addr_yuv;
	unsigned long wnum;
	unsigned char *CbCr_ptr, *Cb_buf_ptr, *Cr_buf_ptr, *ptr;
	int r, g, b;
	unsigned int oy;
	int ou, ov;

	hsiz = shcodecs_encoder_get_width(cap->encoder);
	ysiz = shcodecs_encoder_get_height(cap->encoder);

	/* write memory data size offset make to multiples of 16 */
	wnum = (((hsiz + 15) / 16) * 16) * (((ysiz + 15) / 16) * 16);

	/* Input data to user memory */
	CbCr_ptr = malloc(hsiz * ysiz / 2);

	/* Input file data to user memory */
	w_addr_yuv = malloc(hsiz * ysiz * 2);
	if (w_addr_yuv == NULL) {
		fprintf(stderr, "capture_image_cb: malloc error.\n");
		exit(-1);
	}
	memset(w_addr_yuv, 0, hsiz * ysiz);
	memset(CbCr_ptr, 0, hsiz * ysiz / 2);

	pixel_format = sh_ceu_get_pixel_format(ceu);

	if (pixel_format == V4L2_PIX_FMT_NV12) {
		memcpy(w_addr_yuv, frame_data, hsiz * ysiz);
		memcpy(CbCr_ptr, &frame_data[hsiz * ysiz],
		       hsiz * ysiz / 2);
	} else if (pixel_format == V4L2_PIX_FMT_UYVY) {
		/* Convert from captured UYVY to NV12 */
		d = frame_data;
		y = (unsigned char *) w_addr_yuv;
		c = CbCr_ptr;

		for (i = 0; i < ysiz; i++) {
			if (odd) {
				for (j = 0; j < hsiz / 2; j++) {
					//*c += *d++;   /* Cb */
					//*c++ >>= 1;
					d++;
					*y++ = *d++;	/* Y */
					//*c += *d++;   /* Cr */
					//*c++ >>= 1;
					d++;
					*y++ = *d++;	/* Y */
				}
			} else {
				for (j = 0; j < hsiz / 2; j++) {
					*c++ = *d++;	/* Cb */
					*y++ = *d++;	/* Y */
					*c++ = *d++;	/* Cr */
					*y++ = *d++;	/* Y */
				}
				//c -= hsiz;
			}

			/* toggle odd line flag */
			odd = 1 - odd;
		}
	} else if (pixel_format == V4L2_PIX_FMT_RGB565) {
		/* Convert from captured RGB565 to NV12 */
		d = frame_data;
		ds = (unsigned short *) frame_data;
		y = (unsigned char *) w_addr_yuv;
		c = CbCr_ptr;

#if 0
		sh_veu_rgb565_to_nv12(d, y, c, 320, 240);
#else
		for (i = 0; i < ysiz; i++) {
			if (odd) {
				for (j = 0; j < hsiz; j++) {
					rgb = *ds++;
					r = (rgb & 0x1f) << 3;
					g = ((rgb & 0x7e0) >> 5) << 2;
					b = ((rgb & 0xf800) >> 11) << 3;;

					if (r < 0)
						r = 0;
					if (r > 255)
						r = 255;
					if (g < 0)
						g = 0;
					if (g > 255)
						g = 255;
					if (b < 0)
						b = 0;
					if (b > 255)
						b = 255;

					//      printf ("Got (r,g,b) values (%d, %d, %d)\n", r, g, b);

					//oy = (((66 * r) + (129 * g) + (25 * b))>>8) + 16;
					oy = ((77 * r) + (150 * g) +
					      (29 * b)) >> 8;
					if (oy < 16)
						oy = 16;
					if (oy > 235)
						oy = 235;

					*y++ = (unsigned char) oy;

					//printf ("Got odd  oy value %d, y value %d\n", oy, *y);
				}
			} else {
				for (j = 0; j < hsiz; j++) {
					//r = (d[0] & 0x1f)<<3;
					//g = (((d[0] & 0xe0)>>5) | ((d[1] & 0x07)<<3))<<2;
					//b = (d[1] & 0xf8);
					//d += 2;
					rgb = *ds++;
					r = (rgb & 0x1f) << 3;
					g = ((rgb & 0x7e0) >> 5) << 2;
					b = ((rgb & 0xf800) >> 11) << 3;;

					if (r < 0)
						r = 0;
					if (r > 255)
						r = 255;
					if (g < 0)
						g = 0;
					if (g > 255)
						g = 255;
					if (b < 0)
						b = 0;
					if (b > 255)
						b = 255;

					//      printf ("Got (r,g,b) values (%d, %d, %d)\n", r, g, b);

					//      Y  =      (0.257 * R) + (0.504 * G) + (0.098 * B) + 16
					//      Cr = V =  (0.439 * R) - (0.368 * G) - (0.071 * B) + 128
					//      Cb = U = -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128

					//oy = (((66 * r) + (129 * g) + (25 * b))>>8) + 16;

					//oy = 0.299 * r + 0.587 * g + 0.114 * b;
					//ou = (b-oy)*0.565;
					//ov = (r-oy)*0.713;
					//oy = ((77 * r) + (150 * g) + (29 * b))>>9;
					oy = ((77 * r) + (150 * g) +
					      (29 * b)) >> 8;
					if (oy < 16)
						oy = 16;
					if (oy > 235)
						oy = 235;

					*y++ = (unsigned char) oy;

					//printf ("Got even oy value %d, y value %d\n", oy, *y);

					if ((j % 2) == 1) {
						//ov = ((112 * r) - (94 * g) - (18 * b))>>9 + 128;
						//ou = ((-38 * r) - (74 * g) + (112 * b))>>9 + 128;

						//ou = (((b-oy)*145)/300) + 128;
						//ov = (((r-oy)*183)/300) + 128;
						//ou = (((b-oy)*145)/512);
						//ov = (((r-oy)*183)/512);

						//Ey = 0.299R+0.587G+0.114B
						//Ecr = 0.713(R - Ey) = 0.500R-0.419G-0.081B
						//Ecb = 0.564(B - Ey) = -0.169R-0.331G+0.500B

						ou = (((r * 128) -
						       (g * 107) -
						       (b * 21)) >> 8) +
						    128;
						ov = (((r * (-43)) -
						       (g * 85) +
						       (b * 128)) >> 8) +
						    128;

						//ou = 128;
						//ov = 128;

						if (ou < 16)
							ou = 16;
						if (ou > 239)
							ou = 239;
						if (ov < 16)
							ov = 16;
						if (ov > 239)
							ov = 239;

						*c++ = (unsigned char) ou;
						*c++ = (unsigned char) ov;
					}
				}
			}

			/* toggle odd line flag */
			odd = 1 - odd;
		}
#endif
	}

	/* Write image data to kernel memory for VPU */
        shcodecs_encoder_input_provide (cap->encoder, w_addr_yuv, CbCr_ptr);
#if 0
	m4iph_sdr_write((unsigned char *) cap->addr_y,
			(unsigned char *) w_addr_yuv, wnum);
	m4iph_sdr_write((unsigned char *) cap->addr_c,
			(unsigned char *) CbCr_ptr, wnum / 2);
#endif
	free(w_addr_yuv);
	free(CbCr_ptr);
}

/* capture yuv data to the image-capture-field area each frame */
int capture_image(SHCodecs_Encoder * encoder, APPLI_INFO * appli_info)
{
	struct capture cap;

	cap.encoder = encoder;
	cap.appli_info = appli_info;

	sh_ceu_capture_frame(appli_info->ceu, capture_image_cb, &cap);

	return 0;
}

/* copy yuv data to the image-capture-field area each frame */
int load_1frame_from_image_file(SHCodecs_Encoder * encoder,
                                APPLI_INFO * appli_info)
{
	int read_size;
	long hsiz, ysiz, index, index2;
	FILE *input_yuv_fp;
	unsigned char *w_addr_yuv;
	unsigned long wnum;
	unsigned char *CbCr_ptr, *Cb_buf_ptr, *Cr_buf_ptr, *ptr;

	if (frame_counter_of_input == appli_info->frames_to_encode) {
		return (1);
	}
	input_yuv_fp = appli_info->input_yuv_fp;
	hsiz = shcodecs_encoder_get_width(encoder);
	ysiz = shcodecs_encoder_get_height(encoder);
	frame_counter_of_input++;

	if (input_yuv_fp == NULL) {
		return (-1);
	}
	/* Input file data to user memory */
	w_addr_yuv = malloc(hsiz * ysiz * 2);
	if (w_addr_yuv == NULL) {
		fprintf(stderr, "load_1frame_from_image_file: malloc error.\n");
		exit(-1);
	}
	memset(w_addr_yuv, 0, hsiz * ysiz);
	read_size = fread(w_addr_yuv, 1, hsiz * ysiz, input_yuv_fp);
	if (read_size <= 1) {
		free(w_addr_yuv);
		return (1);
	}

	/* write memory data size offset make to multiples of 16 */
	wnum = (((hsiz + 15) / 16) * 16) * (((ysiz + 15) / 16) * 16);
	/* Input file data to user memory */
	CbCr_ptr = malloc(hsiz * ysiz / 2);
	Cb_buf_ptr = malloc(hsiz * ysiz / 2);
	Cr_buf_ptr = malloc(hsiz * ysiz / 2);
	if (Cb_buf_ptr == NULL || Cr_buf_ptr == NULL) {
		fprintf(stderr, "malloc error \n");
		exit(-1);
	}
	if (appli_info->yuv_CbCr_format == 1) {	/* 1:CbSCrS */
		ptr = CbCr_ptr;
		read_size =
		    fread(Cb_buf_ptr, 1, (hsiz * ysiz / 4), input_yuv_fp);
		read_size =
		    fread(Cr_buf_ptr, 1, (hsiz * ysiz / 4), input_yuv_fp);
		for (index = 0; index < (hsiz * ysiz / 4); index++) {
			*ptr++ = *(Cb_buf_ptr + index);
			*ptr++ = *(Cr_buf_ptr + index);
		}
	} else if (appli_info->yuv_CbCr_format == 2) {	/* 2:Cb0,Cr0,Cb1,Cr1,... */
		read_size =
		    fread(CbCr_ptr, 1, ((hsiz * ysiz / 4) * 2),
			  input_yuv_fp);
		if (read_size <= 1) {
			return (-1);
		}
	} else if (appli_info->yuv_CbCr_format == 3) {	/* 3:Cb1C,Cr1C,... */
		ptr = CbCr_ptr;
		for (index = 0; index < (ysiz / 2); index++) {
			read_size =
			    fread(Cb_buf_ptr, (sizeof(unsigned char)),
				  (hsiz / 2), input_yuv_fp);
			read_size =
			    fread(Cr_buf_ptr, (sizeof(unsigned char)),
				  (hsiz / 2), input_yuv_fp);
			for (index2 = 0; index2 < (hsiz / 2); index2++) {
				*ptr++ = *(Cb_buf_ptr + index2);
				*ptr++ = *(Cr_buf_ptr + index2);
			}
		}
	} else {
		read_size =
		    fread(CbCr_ptr, 1, ((hsiz * ysiz / 4) * 2),
			  input_yuv_fp);
		if (read_size <= 1) {
			return (-1);
		}
	}

	/* Write image data to kernel memory for VPU */
        shcodecs_encoder_input_provide (encoder, w_addr_yuv, CbCr_ptr);
#if 0
	m4iph_sdr_write((unsigned char *) addr_y,
			(unsigned char *) w_addr_yuv, wnum);
	m4iph_sdr_write((unsigned char *) addr_c,
			(unsigned char *) CbCr_ptr, wnum / 2);
#endif
	free(w_addr_yuv);
	free(Cb_buf_ptr);
	free(Cr_buf_ptr);
	free(CbCr_ptr);

	return (0);
}

/* close input YUV data file */
void close_input_image_file(APPLI_INFO * appli_info)
{
	if (appli_info->input_yuv_fp != NULL) {
		fclose(appli_info->input_yuv_fp);
		appli_info->input_yuv_fp = NULL;
	}
}

/* close output file */
void close_output_file(APPLI_INFO * appli_info)
{
	if (appli_info->output_file_fp != NULL) {
		fflush(appli_info->output_file_fp);
		fclose(appli_info->output_file_fp);
		appli_info->output_file_fp = NULL;
	}
}

