 /*****************************************************************************
*                                                                            *
*  SuperH MEPG-4 Video Encode Library TOOL                                   *
*                                                                            *
*    Copyright (C) Renesas Technology Corp., 2005. All rights reserved.      *
*                                                                            *
*    Version¡¡1.0 :   ControlFileUtil.h                                      *
*                                 2005/04/26 13:30  Renesas Technology Corp. *
*                                                                            *
*****************************************************************************/

#ifndef	CONTROL_FILE_UTIL_H
#define	CONTROL_FILE_UTIL_H

#include "avcbencsmp.h"

#include <shcodecs/shcodecs_encoder.h>

int GetFromCtrlFTop(const char *control_filepath,
		    APPLI_INFO * appli_info, long *stream_type);

int GetFromCtrlFtoEncParam(SHCodecs_Encoder * encoder, APPLI_INFO * appli_info);

#endif				/* CONTROL_FILE_UTIL_H */
