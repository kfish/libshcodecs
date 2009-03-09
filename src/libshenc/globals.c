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
#include <stdlib.h>

#include "avcbe.h"		/* SuperH MEPG-4&H.264 Video Encode Library Header */
#include "m4iph_vpu4.h"		/* SuperH MEPG-4&H.264 Video Driver Library Header */
#include "sh7722_vpu.h"
#include "encoder_common.h"		/* User Application Sample Header */
#include "m4driverif.h"

#define KERNEL_MEMORY_FOR_VPU_TOP sdr_base

/* From encoder_common.c */

TAVCBE_FMEM LDEC_ARRY[3];	/* Always set the head of STREAMAREA section! */
TAVCBE_FMEM CAPTF_ARRY[3];

avcbe_stream_info *my_context;

TAVCBE_STREAM_BUFF my_stream_buff_info, my_extra_stream_buff_info;
TAVCBE_STREAM_BUFF my_sps_stream_buff_info;	/* for SPS */
TAVCBE_STREAM_BUFF my_pps_stream_buff_info;	/* for PPS */
TAVCBE_STREAM_BUFF my_sei_stream_buff_info;	/* for SEI */
TAVCBE_STREAM_BUFF my_filler_data_buff_info;	/* for FillerData(CPB  Buffer) */

/*----------------------------------------------------------*/
/* This section must be arranged in 32 bytes alignmen, and, */
/*                         arranged in non-cacheable space. */
/*----------------------------------------------------------*/
/*----------------------------------------------------------*/
/*       Image-capture-field area                           */
/*----------------------------------------------------------*/
// unsigned long my_frame_memory_capt_1[WIDTH_HEIGHT_1_5/4];  
/*----------------------------------------------------------*/
/*       Image-work-field area                              */
/*----------------------------------------------------------*/
// unsigned long my_frame_memory_ldec1[WIDTH_HEIGHT_1_5/4];

/*----------------------------------------------------------*/
/* Table and variables which are used if the avcbe_encode_picture 
 * function returns on each 1-slice */
/*----------------------------------------------------------*/
char *dummy_nal_buf;
char *dummy_nal_buf_addr;
long slice_total_size = 0;
long tmp_pic_total_bytes = 0;
long tmp_slice_size;


/*----------------------------------------------------------*/
/*       Other work-field area                              */
/*----------------------------------------------------------*/
//unsigned long my_work_area[MY_WORK_AREA_SIZE/4];      /* 4 bytes alignment */
unsigned long *my_work_area = NULL;

/*----------------------------------------------------------*/
/*       Stream-output buffer                               */
/*----------------------------------------------------------*/
/* 4 bytes alignment*/
//unsigned long sdr_read_my_stream_buff[MY_STREAM_BUFF_SIZE/4]; 

/* for AU delimiter */
unsigned long my_extra_stream_buff[16 / 4];
unsigned long my_sps_stream_buff[MY_SPS_STREAM_BUFF_SIZE / 4 + 8];	/* for SPS */
unsigned long my_pps_stream_buff[MY_PPS_STREAM_BUFF_SIZE / 4 + 8];	/* for PPS */
unsigned long my_filler_data_buff[MY_FILLER_DATA_BUFF_SIZE / 4];	/* for FillerData */
unsigned long my_sei_stream_buff[MY_SEI_STREAM_BUFF_SIZE / 4];	/* for SEI */

/*----------------------------------------------------------------------------------------------*/
//unsigned long my_yuv_data[MY_INPUT_YUV_DATA_BUFF_SIZE/4];     /* array for input YUV data. */   

/*----------------------------------------------------------------------------------------------*/

#ifndef DISABLE_INT
volatile long m4iph_vpu_cn_flg;
#endif				/* DISABLE_INT */

/* From shcodecs_encoder.c */

unsigned long *my_frame_memory_capt[19];

unsigned long *my_frame_memory_ldec1;
unsigned long *my_frame_memory_ldec2;
unsigned long *my_frame_memory_ldec3;
unsigned long *my_stream_buff;
unsigned long *my_end_code_buff;
unsigned long *my_stream_buff_bak;
unsigned long *my_end_code_buff_bak;
long width_height, max_frame;
unsigned long *kernel_memory_for_vpu_top;

unsigned long * sdr_base;
