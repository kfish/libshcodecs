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

#include "avcbe.h"		/* SuperH MEPG-4&H.264 Video Encode Library Header */
#include "m4iph_vpu4.h"		/* SuperH MEPG-4&H.264 Video Driver Library Header */
#include "sh7722_vpu.h"
#include "encoder_common.h"		/* User Application Sample Header */
#include "m4driverif.h"

extern unsigned long sdr_base;

#define KERNEL_MEMORY_FOR_VPU_TOP sdr_base

/*----------------------------------------------------------*/
/*       User Application Data                              */
/*----------------------------------------------------------*/
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
extern unsigned long *my_stream_buff;
extern unsigned long *my_end_code_buff;

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
extern volatile long m4iph_vpu_cn_flg;
#endif				/* DISABLE_INT */


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
		my_filler_data_buff_info.buff_top =
		    (unsigned char *) &my_filler_data_buff[0];
		my_filler_data_buff_info.buff_size =
		    MY_FILLER_DATA_BUFF_SIZE;
		if (input_filler_size > MY_FILLER_DATA_BUFF_SIZE) {
			input_filler_size = MY_FILLER_DATA_BUFF_SIZE;	/* clip to buffer max size */
		}
		/* Input Filler Data */
		encoder->output_filler_data =
		    avcbe_put_filler_data(&my_filler_data_buff_info,
					  AVCBE_ON, input_filler_size);
	} else {
		encoder->output_filler_data = input_filler_size;
	}
	return (encoder->output_filler_data);
#else
	return input_filler_size;
#endif
}

/*----------------------------------------------------------------------------------------------*/
/* Top of the user application sample source to encode */
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
/* set the parameters of VPU4 */
/*----------------------------------------------------------------------------------------------*/
void set_VPU4_param(M4IPH_VPU4_INIT_OPTION * vpu4_param)
{
	unsigned long tb;

	/* VPU4 Base Address For SH-Mobile 3A */
	vpu4_param->m4iph_vpu_base_address = 0xFE900000;

	/* Endian */
#ifdef _LIT
	vpu4_param->m4iph_vpu_endian = 0x000003FF;	/* for Little Endian */
#else
	vpu4_param->m4iph_vpu_endian = 0x00000000;	/* for Big Endian */
#endif				/* _LIT */


	/* Interrupt */
#ifdef DISABLE_INT
	vpu4_param->m4iph_vpu_interrupt_enable = M4IPH_OFF;
#else
	vpu4_param->m4iph_vpu_interrupt_enable = M4IPH_ON;
#endif				/* DISABLE_INT */

	/* Supply of VPU4 Clock */
	vpu4_param->m4iph_vpu_clock_supply_control = 0;	/* 'clock_supply_enable' -> 'clock_supply_control' changed when Version2 */

	/* Address Mask chage */
	vpu4_param->m4iph_vpu_mask_address_disable = M4IPH_OFF;

	/* Temporary Buffer Address */
	tb = m4iph_sdr_malloc(MY_STREAM_BUFF_SIZE, 32);
	vpu4_param->m4iph_temporary_buff_address = tb;
	printf("m4iph_temporary_buff_address=%X, size=%d\n", tb,
	       MY_STREAM_BUFF_SIZE);
	/* Temporary Buffer Size */
	vpu4_param->m4iph_temporary_buff_size = MY_STREAM_BUFF_SIZE;
	// vpu4_param->m4iph_temporary_buff_size = MY_WORK_AREA_SIZE;
	if (my_work_area == NULL) {
		my_work_area = malloc(MY_WORK_AREA_SIZE);	/* 4 bytes alignment */
		dummy_nal_buf = malloc(MY_DUMMY_NAL_BUFF_SIZE);
		memset(dummy_nal_buf, 0, MY_DUMMY_NAL_BUFF_SIZE);
		printf("my_work_area=%X\n", my_work_area);
		if (my_work_area == NULL) {
			printf("Memoey allocation error\n");
			exit(-200);
		}
	}
}

/*-------------------------------------------------------------------------*/
/*      display characters on terminal                                     */
/*-------------------------------------------------------------------------*/
void DisplayMessage(char *message_text, int line_feed_flag)
{
	printf("%s", message_text);
	if (line_feed_flag == 1) {
		printf("\n");
	}
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
