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

#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

#include "avcbencsmp.h"		/* User Application Sample Header */

#include "ControlFileUtil.h"

/*----------------------------------------------------------------------------------------------*/
/* set parameters for using in encoding */
/*----------------------------------------------------------------------------------------------*/
int select_inputfile_set_param(SHCodecs_Encoder * encoder,
			       APPLI_INFO * appli_info)
{
	int return_code;

	return_code =
	    GetFromCtrlFtoEncParam(encoder, appli_info);

	if (return_code == -1) {
		printf("Control file not found !\n");
		/* コントロールファイルが見つからないなどのエラー */
		return (-256);
	}

	shcodecs_encoder_set_frame_no_increment(encoder, 1);
	shcodecs_encoder_set_frame_no_increment(encoder,
	    shcodecs_encoder_get_frame_num_resolution(encoder) /
	    (shcodecs_encoder_get_frame_rate(encoder) / 10));

	return (return_code);
}

