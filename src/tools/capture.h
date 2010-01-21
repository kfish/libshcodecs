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

#ifndef __CAPTURE_H__
#define __CAPTURE_H__

struct sh_ceu_;
typedef struct sh_ceu_ sh_ceu;

typedef void (*sh_process_callback) (sh_ceu * ceu, const unsigned char *frame_data,
				     size_t length, void *user_data);

sh_ceu *sh_ceu_open(const char *device_name, int width, int height);

sh_ceu *sh_ceu_open_userio(const char *device_name, int width, int height);

void sh_ceu_close(sh_ceu * ceu);

/**
 * Set the data output mode to use physical addresses.
 * If the calling application is interfacing to other IP blocks such as
 * the VEU, then set this function. Otherwise, captured frame output will
 * be mapped to userspace addresses, usable by normal applications.
 * \param ceu The ceu handle
 * \param use_physical Flag: Physical addresses will be reported for
 * output frame data if set to a non-zero value.
 * \retval 0 Success
 */
int
sh_ceu_set_use_physical(sh_ceu * ceu, int use_physical);

void sh_ceu_start_capturing(sh_ceu * ceu);

void sh_ceu_stop_capturing(sh_ceu * ceu);

void sh_ceu_capture_frame(sh_ceu * ceu, sh_process_callback cb,
			  void *user_data);

/* Get the properties of the captured frames 
 * The v4l device may not support the request size
 */
int sh_ceu_get_width(sh_ceu * ceu);
int sh_ceu_get_height(sh_ceu * ceu);
unsigned int sh_ceu_get_pixel_format(sh_ceu * ceu);

#endif				/* __CAPTURE_H__ */
