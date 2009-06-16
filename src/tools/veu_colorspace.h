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

/* Image/Video processing: Scale, rotate, crop, color conversion */

#ifndef __VEU_COLORSPACE_H__
#define __VEU_COLORSPACE_H__

enum {
	NO_ROT=0,
	ROT_90,
};

/* Image formats */
enum {
	RGB565=0,
	YCbCr420,
	YCbCr422,
};
#define FMT_MASK (RGB565 | YCbCr420 | YCbCr422)
#define YCBCR_COMP_RANGE (0 << 16)
#define YCBCR_FULL_RANGE (1 << 16)
#define YCBCR_BT601      (0 << 17)
#define YCBCR_BT709      (1 << 17)

int sh_veu_open(void);

void sh_veu_close(void);

/* Perform (scale|rotate) & crop between YCbCr 4:2:0 & RG565 surfaces */
int
sh_veu_operation(
	unsigned int veu_index,
	unsigned char *src_py,
	unsigned char *src_pc,
	unsigned long src_width,
	unsigned long src_height,
	unsigned long src_pitch,
	int src_fmt,
	unsigned char *dst_py,
	unsigned char *dst_pc,
	unsigned long dst_width,
	unsigned long dst_height,
	unsigned long dst_pitch,
	int dst_fmt,
	int rotate);

/* Perform scale from RG565 to YCbCr 4:2:0 surface */
int sh_veu_rgb565_to_nv12(
	unsigned char *rgb565_in,
	unsigned char *y_out,
	unsigned char *c_out,
	unsigned long width,
	unsigned long height);

/* Perform color conversion & crop from YCbCr 4:2:0 to RG565 surface */
int
sh_veu_nv12_to_rgb565(
	unsigned char *y_in,
	unsigned char *c_in,
	unsigned char *rgb565_out,
	unsigned long width,
	unsigned long height,
	unsigned long pitch_in,
	unsigned long pitch_out);


#endif				/* __VEU_COLORSPACE_H__ */
