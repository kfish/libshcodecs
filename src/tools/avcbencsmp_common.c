/*****************************************************************************
*                                                                            *
*  SuperH MEPG-4 Video Encode Library                                        *
*    (User Application Sample source for VPU4)                               *
*                                                                            *
*    Copyright (C) Renesas Technology Corp., 2005. All rights reserved.      *
*                                                                            *
*    Version@2.0 : avcbencsmp_common.c	(for MPEG-4 and H.264)    			 *
*								 2005/07/27 14:50 Renesas Technology Corp.   *
*                                                                            *
*****************************************************************************/

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

