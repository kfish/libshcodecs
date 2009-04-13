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

#ifndef __VEU_COLORSPACE_H__
#define __VEU_COLORSPACE_H__

int sh_veu_open(void);

void sh_veu_close(void);

int sh_veu_rgb565_to_nv12(unsigned char *rgb565_in,
			  unsigned char *y_out,
			  unsigned char *c_out,
			  unsigned long width, unsigned long height);

int sh_veu_nv12_to_rgb565(unsigned char *y_in,
			  unsigned char *c_in,
			  unsigned char *rgb565_out,
			  unsigned long width, unsigned long height,
			  unsigned long pitch);

#endif				/* __VEU_COLORSPACE_H__ */
