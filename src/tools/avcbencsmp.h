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

#ifndef	AVCBENCSMP_H
#define	AVCBENCSMP_H

#include <stdio.h>

#include "capture.h"

//#define MAX_D1

#define CAPTURE_VGA 1

#ifdef MAX_D1
/*----------------------------------------------------------*/
/*       Image width x Image height x 1.5                   */
/*----------------------------------------------------------*/
#define WIDTH_HEIGHT_1_5	622080	/* 622080=720 x 576 x 1.5(D1 Size) */
#define MAX_FRAME 2
/*----------------------------------------------------------*/
/*       Other work-field area size                         */
/*----------------------------------------------------------*/
#define MY_WORK_AREA_SIZE	1024*512

/*----------------------------------------------------------*/
/*       Stream-output buffer size for 1 frame              */
/*----------------------------------------------------------*/
#define MY_STREAM_BUFF_SIZE	1024*256     /* byte unit */	/* this value must be multiple of 32 */

#elif defined(CAPTURE_VGA)
/*----------------------------------------------------------*/
/*       Image width x Image height x 1.5                   */
/*----------------------------------------------------------*/
#define WIDTH_HEIGHT_1_5	460800	/* 460800=640 x 480 x 1.5(VGA Size) */
#define MAX_FRAME 2
/*----------------------------------------------------------*/
/*       Other work-field area size                         */
/*----------------------------------------------------------*/
#define MY_WORK_AREA_SIZE	(101376*4) /*76800 One stream */	/* QVGA */

/*----------------------------------------------------------*/
/*       Stream-output buffer size for 1 frame              */
/*----------------------------------------------------------*/
#define MY_STREAM_BUFF_SIZE	(400000)     /* byte unit */	/* this value must be multiple of 32 */

#else				/* Not D1, or CAPTURE_VGA */
/*----------------------------------------------------------*/
/*       Image width x Image height x 1.5                   */
/*----------------------------------------------------------*/
#define WIDTH_HEIGHT_1_5	152064	/* 115200=320 x 240 x 1.5(QVGA Size) */
#define MAX_FRAME 4
/*----------------------------------------------------------*/
/*       Other work-field area size                         */
/*----------------------------------------------------------*/
#define MY_WORK_AREA_SIZE	101376 /*76800 One stream */	/* QVGA */

/*----------------------------------------------------------*/
/*       Stream-output buffer size for 1 frame              */
/*----------------------------------------------------------*/
#define MY_STREAM_BUFF_SIZE	160000     /* byte unit */	/* this value must be multiple of 32 */
#endif

/*----------------------------------------------------------*/
/*       Output buffer size for EOS                         */
/*----------------------------------------------------------*/
#define MY_END_CODE_BUFF_SIZE 32	/* byte unit */

/*----------------------------------------------------------*/
/*       SPS-stream-output buffer size                      */
/*----------------------------------------------------------*/
#define MY_SPS_STREAM_BUFF_SIZE 256 /* byte unit */	/* this value must be multiple of 32 */

/*----------------------------------------------------------*/
/*       PPS-stream-output buffer size                      */
/*----------------------------------------------------------*/
#define MY_PPS_STREAM_BUFF_SIZE 256 /* byte unit */	/* this value must be multiple of 32 */

/*----------------------------------------------------------*/
/*       SEI-stream-output buffer size                      */
/*----------------------------------------------------------*/
							 /*#define MY_SEI_STREAM_BUFF_SIZE       175000 *//* Level 1 */
							  /*#define MY_SEI_STREAM_BUFF_SIZE       2000000 *//* Level 2 */
							   /*#define MY_SEI_STREAM_BUFF_SIZE       10000000 *//* Level 3 */
#define MY_SEI_STREAM_BUFF_SIZE	100000

/*----------------------------------------------------------*/
/*    Filler-data-stream-output buffer size                 */
/*----------------------------------------------------------*/
#define MY_FILLER_DATA_BUFF_SIZE 256

/*----------------------------------------------------------*/
/* Buffer size which is used if the avcbe_encode_picture function returns on each 1-slice */
/*----------------------------------------------------------*/
#define MY_DUMMY_NAL_BUFF_SIZE (1024*256)

/*----------------------------------------------------------*/
/*    Input YUV-data buffer size			                */
/*----------------------------------------------------------*/
//#define MY_INPUT_YUV_DATA_BUFF_SIZE (WIDTH_HEIGHT_1_5 *10)

// ORIG #define  OUTPUT_BUF_SIZE (50*1024)
#define  OUTPUT_BUF_SIZE (256*1024)
//#define  OUTPUT_BUF_SIZE (256*1024*4)

/*----- structures -----*/

typedef struct {

	/* Table to store an encoding result (MPEG-4 bitstream) */

	/* Size of encoding result (byte unit) */

	/* Input imformation in encoding */

	/* Input YUV file name and it's file pointer */
	char input_file_name_buf[256];	/* for stream-1 */
	FILE *input_yuv_fp;	/* for stream-1 */
	long yuv_CbCr_format;	/* YUVデータ内（出力するローカルデコードファイルも）
				   のCb,Crデータの並び順（1:Cb全部Cr全部、2:Cb0,Cr0,Cb1,Cr1,...、
				   3:Cbの1ライン分,Crの1ライン分,...） */

	char output_file_name_buf[256];	/* 出力m4vファイル名 */
	char ctrl_file_name_buf[256];	/* 入力YUVファイル名 */

	char buf_input_yuv_file_with_path[256 + 8];	/* 入力YUVファイル名（パス付き） *//* 041201 */
	char buf_input_yuv_file[64 + 8];	/* 入力YUVファイル名（パスなし） */

	char buf_output_directry[256 + 8];	/* 出力先のディレクトリ *//* 041201 */
	char buf_output_stream_file[64 + 8];	/* 出力ストリームファイル名（パスなし） */
	long xpic;
	long ypic;

	/* Output imformation in encoding */

	long return_code;	/* return_value of current frame or NAL *//* 041123 */

	FILE *output_file_fp;	/* for output stream-2 */

	sh_ceu *ceu;

} APPLI_INFO;


#define ALIGN(a, w) (void *)(((unsigned long)(a) + (w) - 1) & ~((w) - 1))

#endif				/* AVCBENCSMP */
