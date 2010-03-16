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
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "avcbe.h"		/* SuperH MEPG-4&H.264 Video Encode Library Header */
#include "m4iph_vpu4.h"		/* SuperH MEPG-4&H.264 Video Driver Library Header */
#include "sh7722_vpu.h"
#include "encoder_common.h"		/* User Application Sample Header */
#include "m4driverif.h"

/*----------------------------------------------------------------------------------------------*/
/* Example of function created by the user, which is called when VPU4 reads register. */
/* (This function is common to MPEG-4&H.264 Encoder for VPU4.)  */
/*----------------------------------------------------------------------------------------------*/
unsigned long m4iph_sdr_read_vpu4(unsigned char *address,
				  unsigned char *buffer,
				  unsigned long count)
{
	m4iph_sdr_read(address, buffer, count);
	return (count);
}

/*----------------------------------------------------------------------------------------------*/
/* Example of function created by the user, which is called when VPU4 writes register. */
/* (This function is common to MPEG-4&H.264 Encoder for VPU4.)  */
/*----------------------------------------------------------------------------------------------*/
void m4iph_sdr_write_vpu4(unsigned char *address, unsigned char *buffer,
			  unsigned long count)
{
	m4iph_sdr_write(address, buffer, count);
}

/*-----------------------------------------------------------------------------*/
/* Example of function created by the user, which is called when cpb_buffer 
 * became over follow. */
/*-----------------------------------------------------------------------------*/
unsigned long avcbe_insert_filler_data_for_cpb_buffer(unsigned long
						      input_filler_size)
{
#if 0
	if (encoder->output_filler_enable == 1) {
		/* Output Stream Buffer Infomation For Filler Data */
		filler_data_buff_info.buff_top =
		    (unsigned char *) &filler_data_buff[0];
		filler_data_buff_info.buff_size =
		    FILLER_DATA_BUFF_SIZE;
		if (input_filler_size > FILLER_DATA_BUFF_SIZE) {
			input_filler_size = FILLER_DATA_BUFF_SIZE;	/* clip to buffer max size */
		}
		/* Input Filler Data */
		encoder->output_filler_data =
		    avcbe_put_filler_data(&filler_data_buff_info,
					  AVCBE_ON, input_filler_size);
	} else {
		encoder->output_filler_data = input_filler_size;
	}
	return (encoder->output_filler_data);
#else
	return input_filler_size;
#endif
}

/*-------------------------------------------------------------------------*/
/*      dummy function			                                           */
/*-------------------------------------------------------------------------*/
long m4iph_dec_continue()
{
	return (0);		/* dummy function */
}

/*float logf(float x) // dummy function
{
	float y;
	y = (float) log(x);
	return y;
}*/

long encode_time;

void encode_time_init(void)
{
	encode_time = 0;
}

unsigned long encode_time_get(void)
{
	return encode_time;
}

