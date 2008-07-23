 /*****************************************************************************
*                                                                            *
*  SuperH MEPG-4 Video Encode Library TOOL                                   *
*                                                                            *
*    Copyright (C) Renesas Technology Corp., 2005. All rights reserved.      *
*                                                                            *
*    Version　1.0 :   ControlFileUtil.h                                      *
*                                 2005/04/26 13:30  Renesas Technology Corp. *
*                                                                            *
*****************************************************************************/

/*--------------------------------------------------------------------------*/
/*  1.000  2004.08.04  start codes                                          */
/*--------------------------------------------------------------------------*/
/* コントロールファイルからの読み出し関数群 */

#ifndef	CONTROL_FILE_UTIL_H
#define	CONTROL_FILE_UTIL_H

// michioka
//#include "avcbe.h"
//#include "m4iph_vpu4.h"
//#include "VPU4EncDef.h"

/*----- structures -----*/



/*----- prototype -----*/
#ifdef __cplusplus
extern "C" {
#endif

int GetFromCtrlFTop(const char *control_filepath, ENC_EXEC_INFO *enc_exec_info,
						   long *stream_type); /* 041201 */

int GetFromCtrlFtoEncParam(const char *control_filepath, ENC_EXEC_INFO *enc_exec_info,
						   avcbe_encoding_property *encoding_property,
						   avcbe_other_options_h264 *other_options_h264,
						   avcbe_other_options_mpeg4 *other_options_mpeg4);

int GetFromCtrlFtoEncParamAfterInitEncode(const char *control_filepath, avcbe_stream_info *context,
							OTHER_API_ENC_PARAM *other_API_enc_param,
							avcbe_encoding_property *encoding_property);

int GetFromCtrlFtoEncExecInfo(FILE *fp_in, ENC_EXEC_INFO *enc_exec_info);

int GetFromCtrlFtoEncoding_property(FILE *fp_in, avcbe_encoding_property *encoding_property);

int GetFromCtrlFtoOther_options_H264(FILE *fp_in, avcbe_other_options_h264 *other_options_h264);

int GetFromCtrlFtoOther_options_MPEG4(FILE *fp_in, avcbe_other_options_mpeg4 *other_options_mpeg4);

int GetFromCtrlFtoOTHER_API_ENC_PARAM(FILE *fp_in, OTHER_API_ENC_PARAM *other_API_enc_param,
							avcbe_encoding_property *encoding_property,
							avcbe_other_options_h264 *other_options_h264,
							avcbe_other_options_mpeg4 *other_options_mpeg4);

void GetFromCtrlFtoOTHER_API_ENC_PARAM_VUI(FILE *fp_in, OTHER_API_ENC_PARAM *other_API_enc_param);

void GetFromCtrlFtoOTHER_API_ENC_PARAM_SEI(FILE *fp_in, OTHER_API_ENC_PARAM *other_API_enc_param);

int GetFromCtrlFtoVPU4_ENC(FILE *fp_in, M4IPH_VPU4_ENC *vpu4_enc, avcbe_encoding_property *encoding_property); /* 050106 第３引数追加 */

long GetValueFromCtrlFile(FILE *fp_in, const char *key_word, int *status_flag);

void GetStringFromCtrlFile(FILE *fp_in, const char *key_word, char *return_string, int *status_flag);

int ReadUntilKeyMatch(FILE *fp_in, const char *key_word, char *buf_value);

#ifdef __cplusplus
}
#endif  

#endif /* CONTROL_FILE_UTIL */
