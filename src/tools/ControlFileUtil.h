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

int GetFromCtrlFTop(const char *control_filepath,
		    ENC_EXEC_INFO * enc_exec_info, long *stream_type);

int GetFromCtrlFtoEncParam(const char *control_filepath,
			   ENC_EXEC_INFO * enc_exec_info,
			   avcbe_encoding_property *
			   encoding_property,
			   avcbe_other_options_h264 *
			   other_options_h264,
			   avcbe_other_options_mpeg4 *
			   other_options_mpeg4);

int GetFromCtrlFtoEncParamAfterInitEncode(const char
					  *control_filepath,
					  avcbe_stream_info *
					  context,
					  OTHER_API_ENC_PARAM *
					  other_API_enc_param,
					  avcbe_encoding_property *
					  encoding_property);

#endif				/* CONTROL_FILE_UTIL_H */
