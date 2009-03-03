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
#include "avcbencsmp.h"		/* User Application Sample Header */
#include "m4driverif.h"

#include "encoder_private.h"	/* XXX */


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
#if 1
	return input_filler_size;
#else
	if (ainfo.output_filler_enable == 1) {
		/* Output Stream Buffer Infomation For Filler Data */
		my_filler_data_buff_info.buff_top =
		    (unsigned char *) &my_filler_data_buff[0];
		my_filler_data_buff_info.buff_size =
		    MY_FILLER_DATA_BUFF_SIZE;
		if (input_filler_size > MY_FILLER_DATA_BUFF_SIZE) {
			input_filler_size = MY_FILLER_DATA_BUFF_SIZE;	/* clip to buffer max size */
		}
		/* Input Filler Data */
		ainfo.output_filler_data =
		    avcbe_put_filler_data(&my_filler_data_buff_info,
					  AVCBE_ON, input_filler_size);
	} else {
		ainfo.output_filler_data = input_filler_size;
	}
	return (ainfo.output_filler_data);
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

/*----------------------------------------------------------------------------------------------*/
/* set parameters for using in encoding */
/*----------------------------------------------------------------------------------------------*/
int select_inputfile_set_param(SHCodecs_Encoder * encoder,
			       APPLI_INFO * appli_info)
{
	int return_code;

	return_code =
	    GetFromCtrlFtoEncParam(appli_info->ctrl_file_name_buf,
				   &appli_info->enc_exec_info,
				   &encoder->encoding_property,
				   &encoder->other_options_h264,
				   &encoder->other_options_mpeg4);
	if (return_code == -1) {
		printf("Control file not found !\n");
		/* コントロールファイルが見つからないなどのエラー */
		return (-256);
	}
	printf("Control file read OK!\n");

	/* same parameters to case No. */
//      appli_info->input_file_name_buf[0] = '\0';
	//appli_info->input_yuv_fp = NULL;

	appli_info->frame_no_increment = 1;
	appli_info->b_vop_num = 0;

	appli_info->output_filler_enable = 0;	/* add at Version2 */

	appli_info->output_filler_data = 0;	/* add at Version2 */

	/* same parameters to stream_type */
#if 0
//              strcpy(&appli_info->input_file_name_buf[0], "sample.yuv");
//              strcpy(&appli_info->output_file_name_buf[0], "output.264");

		/*** avcbe_encoding_property ***/
//01            encoder->encoding_property.avcbe_stream_type = encoder->encoding_property.avcbe_stream_type;
//02            encoder->encoding_property.avcbe_bitrate = 384000;
//03            encoder->encoding_property.avcbe_xpic_size = 320;
//04            encoder->encoding_property.avcbe_ypic_size = 240;
//05            encoder->encoding_property.avcbe_frame_rate = 150; 

#ifdef CAPT_INPUT
//06            encoder->encoding_property.avcbe_I_vop_interval = 0;
#else
//06            encoder->encoding_property.avcbe_I_vop_interval = 30;
#endif				/* CAPT_INPUT */

//07            encoder->encoding_property.avcbe_mv_mode = AVCBE_WITH_UMV;
//08            encoder->encoding_property.avcbe_fcode_forward = AVCBE_MVR_FCODE_2; /* '1' -> '2' changed at Version2 */

//09            encoder->encoding_property.avcbe_search_mode = AVCBE_MVM_WEIGHT_10; /* 10 means 127 times */
//10            encoder->encoding_property.avcbe_search_time_fixed = AVCBE_ON;

//11            encoder->encoding_property.avcbe_ratecontrol_skip_enable = AVCBE_ON;
//12            encoder->encoding_property.avcbe_ratecontrol_use_prevquant = AVCBE_ON;

//13            encoder->encoding_property.avcbe_ratecontrol_respect_type = AVCBE_RESPECT_BITRATE;       /* added at Version2 */

	/*      encoder->encoding_property.avcbe_ratecontrol_rcperiod_skipcheck_enable = AVCBE_ON;       deleted at Version2 */
	/*      encoder->encoding_property.avcbe_ratecontrol_rcperiod_ivop_noskip = AVCBE_ON;            deleted at Version2 */
	/*      encoder->encoding_property.avcbe_ratecontrol_vbv_skipcheck_enable = AVCBE_OFF;           deleted at Version2 */
	/*      encoder->encoding_property.avcbe_ratecontrol_vbv_ivop_noskip = AVCBE_ON;                         deleted at Version2 */

//14            encoder->encoding_property.avcbe_ratecontrol_intra_thr_changeable = AVCBE_OFF;
//15            encoder->encoding_property.avcbe_control_bitrate_length = 0;
//16            encoder->encoding_property.avcbe_intra_macroblock_refresh_cycle = 0;
	/* 'AVCBE_MPEG4_NTSC' -> 'AVCBE_VIDEO_FORMAT_NTSC' changed at Version2 */
//17            encoder->encoding_property.avcbe_video_format = AVCBE_VIDEO_FORMAT_NTSC;
//18            encoder->encoding_property.avcbe_frame_num_resolution = 30;
//19            encoder->encoding_property.avcbe_noise_reduction = 0;
//20            encoder->encoding_property.avcbe_reaction_param_coeff = 10;
//21            encoder->encoding_property.avcbe_weightedQ_mode = AVCBE_WEIGHTEDQ_NONE;
#endif

	/* parameters depend on stream_type */
	if ((encoder->encoding_property.avcbe_stream_type == AVCBE_MPEG4) ||
	    (encoder->encoding_property.avcbe_stream_type == AVCBE_H263)) {
	} else if (encoder->encoding_property.avcbe_stream_type == AVCBE_H264) {	/* added at Version2 */
			/*** avcbe_other_options_h264 ***/
#if 0
//01                    appli_info->other_options_h264.avcbe_Ivop_quant_initial_value = 30;
//02                    appli_info->other_options_h264.avcbe_Pvop_quant_initial_value = 30;
//03                    appli_info->other_options_h264.avcbe_use_dquant = AVCBE_ON;     
//04                    appli_info->other_options_h264.avcbe_clip_dquant_next_mb = 4;
//05                    appli_info->other_options_h264.avcbe_clip_dquant_frame = 7;
//06                    appli_info->other_options_h264.avcbe_quant_min = 10;
//07                    appli_info->other_options_h264.avcbe_quant_min_Ivop_under_range = 4;
//08                    appli_info->other_options_h264.avcbe_quant_max = 40;

//09                    appli_info->other_options_h264.avcbe_ratecontrol_cpb_skipcheck_enable = AVCBE_ON;
//10                    appli_info->other_options_h264.avcbe_ratecontrol_cpb_Ivop_noskip = AVCBE_ON;
//11                    appli_info->other_options_h264.avcbe_ratecontrol_cpb_remain_zero_skip_enable = AVCBE_ON;

//12                    appli_info->other_options_h264.avcbe_ratecontrol_cpb_buffer_unit_size = 1000;
//13                    appli_info->other_options_h264.avcbe_ratecontrol_cpb_buffer_mode = AVCBE_AUTO;
//14                    appli_info->other_options_h264.avcbe_ratecontrol_cpb_max_size = 1152;
//15                    appli_info->other_options_h264.avcbe_ratecontrol_cpb_offset = 20;
//16                    appli_info->other_options_h264.avcbe_ratecontrol_cpb_offset_rate = 50;

//17                    appli_info->other_options_h264.avcbe_intra_thr_1 = 0;
//18                    appli_info->other_options_h264.avcbe_intra_thr_2 = 5000;
//19                    appli_info->other_options_h264.avcbe_sad_intra_bias = 0;

//20                    appli_info->other_options_h264.avcbe_regularly_inserted_I_type = 
//                                                              AVCBE_I_PIC;
#if 0
		if (case_no == CASE1_H264_001) {
			appli_info->other_options_h264.avcbe_call_unit =
			    AVCBE_CALL_PER_PIC;
			appli_info->other_options_h264.avcbe_use_slice =
			    AVCBE_OFF;
			appli_info->
			    other_options_h264.avcbe_slice_size_mb = 0;
			appli_info->
			    other_options_h264.avcbe_slice_size_bit = 0;
		} else if (case_no == CASE2_H264_002) {
			appli_info->other_options_h264.avcbe_call_unit =
			    AVCBE_CALL_PER_PIC;
			appli_info->other_options_h264.avcbe_use_slice =
			    AVCBE_ON;
			appli_info->
			    other_options_h264.avcbe_slice_size_mb = 100;
			appli_info->
			    other_options_h264.avcbe_slice_size_bit = 0;
		} else if (case_no == CASE3_H264_003) {
			appli_info->other_options_h264.avcbe_call_unit =
			    AVCBE_CALL_PER_NAL;
			appli_info->other_options_h264.avcbe_use_slice =
			    AVCBE_ON;
			appli_info->
			    other_options_h264.avcbe_slice_size_mb = 100;
			appli_info->
			    other_options_h264.avcbe_slice_size_bit = 0;
		}
#endif
//25                    appli_info->other_options_h264.avcbe_slice_type_value_pattern = 
//                                                              AVCBE_SLICE_TYPE_VALUE_HIGH;
//26                    appli_info->other_options_h264.avcbe_use_mb_partition = AVCBE_ON;
//27                    appli_info->other_options_h264.avcbe_mb_partition_vector_thr = 0;
//28                    appli_info->other_options_h264.avcbe_deblocking_mode =
//                                                              AVCBE_DBF_EXC_FRAME_EDGE;
//29                    appli_info->other_options_h264.avcbe_use_deblocking_filter_control =
//                                                              AVCBE_OFF;
//30                    appli_info->other_options_h264.avcbe_deblocking_alpha_offset = 0;
//31                    appli_info->other_options_h264.avcbe_deblocking_beta_offset = 0;

//32                    appli_info->other_options_h264.avcbe_me_skip_mode =
//                                                              AVCBE_ME_FORCE_SKIP_MID;

//33                    appli_info->other_options_h264.avcbe_put_start_code = AVCBE_ON;
//34                    appli_info->other_options_h264.avcbe_param_changeable = AVCBE_OFF;
//35                    appli_info->other_options_h264.avcbe_changeable_max_bitrate = 0;

		/* SequenceHeaderParameter */
//36                    appli_info->other_options_h264.avcbe_seq_param_set_id = 0;
//37                    appli_info->other_options_h264.avcbe_profile =
//                                                              AVCBE_H264_PROFILE_BASELINE;
//38                    appli_info->other_options_h264.avcbe_constraint_set_flag = 0;
//39                    appli_info->other_options_h264.avcbe_level_type = AVCBE_AUTO;
//40                    appli_info->other_options_h264.avcbe_level_value = 10;
//41                    appli_info->other_options_h264.avcbe_out_vui_parameters = AVCBE_OFF;

		/* Picture Header Parameter */
//42                    appli_info->other_options_h264.avcbe_chroma_qp_index_offset = 0;
//43                    appli_info->other_options_h264.avcbe_constrained_intra_pred = AVCBE_OFF;
#endif
	}
	/* end of 'if (stream_type == AVCBE_H264)' */
	appli_info->frame_no_increment =
	    encoder->encoding_property.avcbe_frame_num_resolution /
	    (encoder->encoding_property.avcbe_frame_rate / 10);


#if 1
/*----- avcbe_encoding_property (common parameter for MPEG-4 & H.264) -----*/
	printf("(01) stream_type                      = %d\n",
	       (int) encoder->encoding_property.avcbe_stream_type);
	printf("(02) bitrate                          = %d\n",
	       (int) encoder->encoding_property.avcbe_bitrate);
	printf("(03) xpic_size                        = %d\n",
	       (int) encoder->encoding_property.avcbe_xpic_size);
	printf("(04) ypic_size                        = %d\n",
	       (int) encoder->encoding_property.avcbe_ypic_size);
	printf("(05) frame_rate                       = %d\n",
	       (int) encoder->encoding_property.avcbe_frame_rate);
	printf("(06) I_vop_interval                   = %d\n",
	       (int) encoder->encoding_property.avcbe_I_vop_interval);
	printf("(07) mv_mode                          = %d\n",
	       (int) encoder->encoding_property.avcbe_mv_mode);
	printf("(08) fcode_forward                    = %d\n",
	       (int) encoder->encoding_property.avcbe_fcode_forward);
	printf("(09) search_mode                      = %d\n",
	       (int) encoder->encoding_property.avcbe_search_mode);
	printf("(10) time_fixed                       = %d\n",
	       (int) encoder->encoding_property.avcbe_search_time_fixed);

	printf("(11) skip_enable                      = %d\n",
	       (int) encoder->encoding_property.avcbe_ratecontrol_skip_enable);
	printf("(12) use_prevquant                    = %d\n",
	       (int) encoder->encoding_property.avcbe_ratecontrol_use_prevquant);
	printf("(13) respect_type                     = %d\n",
	       (int) encoder->encoding_property.avcbe_ratecontrol_respect_type);

	printf("(14) ratecontrol_intra_thr_changeable = %d\n",
	       (int) encoder->encoding_property.avcbe_ratecontrol_intra_thr_changeable);
	printf("(15) control_bitrate_lengt            = %d\n",
	       (int) encoder->encoding_property.avcbe_control_bitrate_length);
	printf("(16) intra_macroblock_refresh_cycle   = %d\n",
	       (int) encoder->encoding_property.avcbe_intra_macroblock_refresh_cycle);

	printf("(17) video_format                     = %d\n",
	       (int) encoder->encoding_property.avcbe_video_format);
	printf("(18) frame_num_resolution             = %d\n",
	       (int) encoder->encoding_property.avcbe_frame_num_resolution);
	printf("(19) noise_reduction                  = %d\n",
	       (int) encoder->encoding_property.avcbe_noise_reduction);
	printf("(20) reaction_param_coeff             = %d\n",
	       (int) encoder->encoding_property.avcbe_reaction_param_coeff);
	printf("(21) weightedQ_mode                   = %d\n",
	       (int) encoder->encoding_property.avcbe_weightedQ_mode);
#endif

#if 0
/*----- avcbe_other_options_h264 (H.264 Only) -----*/
	printf("(01) Ivop_quant_initial_value                  = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_Ivop_quant_initial_value);
	printf("(02) Pvop_quant_initial_value                  = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_Pvop_quant_initial_value);
	printf("(03) use_dquant   = %d\n",
	       (int) appli_info->other_options_h264.avcbe_use_dquant);
	printf("(04) clip_dquant_next_mb                       = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_clip_dquant_next_mb);
	printf("(05) clip_dquant_frame                         = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_clip_dquant_frame);
	printf("(06) quant_min                                 = %d\n",
	       (int) appli_info->other_options_h264.avcbe_quant_min);
	printf("(07) Ivop_under_range                          = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_quant_min_Ivop_under_range);
	printf("(08) quant_max                                 = %d\n",
	       (int) appli_info->other_options_h264.avcbe_quant_max);

	printf("(09) ratecontrol_cpb_skipcheck_enable          = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_ratecontrol_cpb_skipcheck_enable);
	printf("(10) ratecontrol_cpb_Ivop_noskip               = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_ratecontrol_cpb_Ivop_noskip);
	printf("(11) ratecontrol_cpb_remain_zero_skip_enable   = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_ratecontrol_cpb_remain_zero_skip_enable);

	printf("(12) ratecontrol_cpb_buffer_unit_size          = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_ratecontrol_cpb_buffer_unit_size);
	printf("(13) ratecontrol_cpb_buffer_mode               = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_ratecontrol_cpb_buffer_mode);
	printf("(14) ratecontrol_cpb_max_size                  = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_ratecontrol_cpb_max_size);
	printf("(15) ratecontrol_cpb_offset                    = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_ratecontrol_cpb_offset);
	printf("(16) ratecontrol_cpb_offset_rate               = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_ratecontrol_cpb_offset_rate);

	printf("(17) intra_thr_1                               = %d\n",
	       (int) appli_info->other_options_h264.avcbe_intra_thr_1);
	printf("(18) intra_thr_2                               = %d\n",
	       (int) appli_info->other_options_h264.avcbe_intra_thr_2);
	printf("(19) sad_intra_bias                            = %d\n",
	       (int) appli_info->other_options_h264.avcbe_sad_intra_bias);

	printf("(20) regularly_inserted_I_type                 = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_regularly_inserted_I_type);
	printf("(21) call_unit                                 = %d\n",
	       (int) appli_info->other_options_h264.avcbe_call_unit);
	printf("(22) use_slice                                 = %d\n",
	       (int) appli_info->other_options_h264.avcbe_use_slice);
	printf("(23) slice_size_mb                             = %d\n",
	       (int) appli_info->other_options_h264.avcbe_slice_size_mb);
	printf("(24) slice_size_bit                            = %d\n",
	       (int) appli_info->other_options_h264.avcbe_slice_size_bit);
	printf("(25) slice_type_value_pattern                  = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_slice_type_value_pattern);
	printf("(26) use_mb_partition                          = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_use_mb_partition);
	printf("(27) mb_partition_vector_thr                   = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_mb_partition_vector_thr);
	printf("(28) deblocking_mode                           = %d\n",
	       (int) appli_info->other_options_h264.avcbe_deblocking_mode);
	printf("(29) use_deblocking_filter_control             = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_use_deblocking_filter_control);
	printf("(30) deblocking_alpha_offset                   = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_deblocking_alpha_offset);
	printf("(31) deblocking_beta_offset                    = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_deblocking_beta_offset);

	printf("(32) me_skip_mode                              = %d\n",
	       (int) appli_info->other_options_h264.avcbe_me_skip_mode);
	printf("(33) put_start_code                            = %d\n",
	       (int) appli_info->other_options_h264.avcbe_put_start_code);
	printf("(34) param_changeable                          = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_param_changeable);
	printf("(35) changeable_max_bitrate                    = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_changeable_max_bitrate);

	/* SequenceHeaderParameter */
	printf("(36) seq_param_set_id                          = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_seq_param_set_id);
	printf("(37) profile                                   = %d\n",
	       (int) appli_info->other_options_h264.avcbe_profile);
	printf("(38) constraint_set_flag                       = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_constraint_set_flag);
	printf("(39) level_type                                = %d\n",
	       (int) appli_info->other_options_h264.avcbe_level_type);
	printf("(40) level_value                               = %d\n",
	       (int) appli_info->other_options_h264.avcbe_level_value);
	printf("(41) out_vui_parameters                        = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_out_vui_parameters);

	/* PictureHeaderParameter */
	printf("(42) chroma_qp_index_offset                    = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_chroma_qp_index_offset);
	printf("(43) constrained_intra_pred                    = %d\n",
	       (int) appli_info->
	       other_options_h264.avcbe_constrained_intra_pred);
#endif

	return (return_code);
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
