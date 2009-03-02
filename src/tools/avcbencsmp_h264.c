/*****************************************************************************
*                                                                            *
*  SuperH MEPG-4 Video Encode Library                                        *
*    (User Application Sample source for VPU4)                               *
*                                                                            *
*    Copyright (C) Renesas Technology Corp., 2004-2006. All rights reserved. *
*                                                                            *
*    Version@2.0 : avcbencsmp_h264.c					     *
*				2005/07/27 14:30 Renesas Technology Corp.    *
*                                                                            *
*****************************************************************************/

#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <setjmp.h>
#include <sys/time.h>

#include "avcbe.h"		/* SuperH MEPG-4&H.264 Video Encode Library Header */
#include "m4iph_vpu4.h"		/* SuperH MEPG-4&H.264 Video Driver Library Header */
#include "avcbencsmp.h"		/* User Application Sample Header */

#include "capture.h"
#include "encoder_private.h"

extern avcbe_stream_info *my_context;
extern TAVCBE_FMEM LDEC_ARRY[];
extern TAVCBE_FMEM CAPTF_ARRY;

extern unsigned long *my_frame_memory_capt[];
extern unsigned long *my_frame_memory_ldec1;
extern unsigned long *my_frame_memory_ldec2;
extern unsigned long *my_frame_memory_ldec3;
extern unsigned long *my_work_area;
extern unsigned long *my_stream_buff;
extern unsigned long *my_end_code_buff;	/* for End Code */
extern unsigned long my_extra_stream_buff[];	/* for AUD */
extern unsigned long my_sps_stream_buff[];	/* for SPS */
extern unsigned long my_pps_stream_buff[];	/* for PPS */
extern unsigned long my_sei_stream_buff[];	/* for SEI */
extern unsigned long my_filler_data_buff[];	/* for FillerData(CPB  Buffer) */

extern TAVCBE_STREAM_BUFF my_stream_buff_info, my_extra_stream_buff_info;
extern TAVCBE_STREAM_BUFF my_sps_stream_buff_info;	/* for SPS */
extern TAVCBE_STREAM_BUFF my_pps_stream_buff_info;	/* for PPS */
extern TAVCBE_STREAM_BUFF my_sei_stream_buff_info;	/* for SEI */
extern TAVCBE_STREAM_BUFF my_filler_data_buff_info;	/* for FillerData(CPB  Buffer) */

extern long encode_time;

/*----------------------------------------------------------*/
/*       data for NAL unit encoding                         */
/*----------------------------------------------------------*/
extern char *dummy_nal_buf;
extern char *dummy_nal_buf_addr;
extern long slice_total_size;
extern long tmp_pic_total_bytes;
extern long tmp_slice_size;

/*-------------------------------------------------------------------------------*/

int encode_init_h264 (SHCodecs_Encoder * encoder, APPLI_INFO * appli_info, long stream_type)
{
	long return_code;
	TAVCBE_STREAM_BUFF my_end_code_buff_info;

	appli_info->error_return_function = 0;
	appli_info->error_return_code = 0;

	/*--- The MPEG-4 Encoder Library API(required-2)@start encoding ---*/
	avcbe_start_encoding();	/* needs be called only once */

	/* Initialize Function Of Encoder(avcbe_set_default_param, avcbe_init_encode, avcbe_init_memory) */
	return_code =
	    init_for_encoder_h264(encoder, appli_info,
				  stream_type, &my_context);
	if (return_code != 0) {
		return (-114);
	}

        return 0;
}

int encode_1file_h264(SHCodecs_Encoder * encoder, APPLI_INFO * appli_info,
		      long stream_type)
{
	long return_code;
	TAVCBE_STREAM_BUFF my_end_code_buff_info;

	appli_info->error_return_function = 0;
	appli_info->error_return_code = 0;

	DisplayMessage("H.264 Encode Start! ", 1);

	/* encode process function for H.264 (call avcbe_encode_picture func.) */
	if ((appli_info->other_options_h264.avcbe_use_slice == AVCBE_ON) &&
	    (appli_info->other_options_h264.avcbe_call_unit ==
	     AVCBE_CALL_PER_NAL)) {
		return_code =
		    encode_nal_unit(encoder, appli_info,
				    stream_type, my_context);
	} else {
		return_code =
		    encode_picture_unit(encoder, appli_info,
					stream_type, my_context);
	}
	if (return_code != 0) {
		return (-115);
	}

	/*--- The MPEG-4&H.264 Encoder Library API (required-9)@ends encoding ---*/
	my_end_code_buff_info.buff_top =
	    (unsigned char *) &my_end_code_buff[0];
	my_end_code_buff_info.buff_size = MY_END_CODE_BUFF_SIZE;

	return_code = avcbe_put_end_code(my_context, &my_end_code_buff_info, AVCBE_END_OF_STRM);	/* return value is byte unit */
	if (return_code <= 0) {
		if (return_code == -4) {
			DisplayMessage
			    (" encode_1file_h264:avcbe_close_encode OUTPUT BUFFER SIZE SHORT ERROR! ",
			     1);
		}
		appli_info->error_return_function = -116;
		appli_info->error_return_code = return_code;
		return (-116);
	} else {
		if (encoder->output) {
			encoder->output(encoder, (unsigned char *)
					&my_end_code_buff[0], return_code,
					encoder->output_user_data);
		}
	}
	if (appli_info->output_filler_enable == 1) {
		return_code =
		    avcbe_put_filler_data(&my_stream_buff_info,
					  appli_info->other_options_h264.
					  avcbe_put_start_code, 2);
	}
	return (0);
}

/*-------------------------------------------------------------------------------*/
/* init for encoder                                                              */
/*-------------------------------------------------------------------------------*/
long init_for_encoder_h264(SHCodecs_Encoder * encoder,
			   APPLI_INFO * appli_info, long stream_type,
			   avcbe_stream_info ** context)
{
	long return_code = 0;
	unsigned long nrefframe, nldecfmem, addr_temp;
	unsigned long *addr_y, *addr_c;
	TAVCBE_WORKAREA WORK_ARRY[2];
	long area_width, area_height;

	/*--- The MPEG-4&H.264 Encoder Library API(required-3)@set default values for the parameters  ---*/
	/* can be called only once or for the number of streams */
	return_code =
	    avcbe_set_default_param(stream_type, AVCBE_RATE_NORMAL,
				    &(appli_info->param), (void *)
				    &(appli_info->other_options_h264));
	if (return_code != 0) {	/* error */
		DisplayMessage
		    (" encode_1file_h264:avcbe_set_default_param ERROR! ",
		     1);
		appli_info->error_return_function = -102;
		appli_info->error_return_code = return_code;
		return (-102);
	}
	/*--- set parameters for use in encoding (one of the user application's own functions) ---*/
	return_code = select_inputfile_set_param(encoder, appli_info);
	if (return_code == -1) {	/* error */
		DisplayMessage
		    (" encode_1file_h264:select_inputfile_set_param ERROR! ",
		     1);
		appli_info->error_return_function = -103;
		appli_info->error_return_code = return_code;
		return (-103);
	}

	/* Capt Image Memory Size Check */
	if (WIDTH_HEIGHT_1_5 < (encoder->width * encoder->height * 3 / 2)) {
		printf("Size OVER\n");
		while (1);
	}

	/*--- The MPEG-4&H.264 Encoder Library API(required-4)@initialize the variables ---*/
	WORK_ARRY[0].area_size = MY_WORK_AREA_SIZE;
	WORK_ARRY[0].area_top = (unsigned char *) my_work_area;

#ifndef VPU4IP
	WORK_ARRY[1].area_size = MY_MB_WORK_AREA_SIZE;
	WORK_ARRY[1].area_top = (unsigned char *) my_mb_work_area;
#else
	WORK_ARRY[1].area_size = 0;
	WORK_ARRY[1].area_top = (unsigned char *) NULL;
#endif				/* VPU4IP */

	if (appli_info->other_options_h264.
	    avcbe_ratecontrol_cpb_buffer_mode == AVCBE_MANUAL) {
		appli_info->other_options_h264.
		    avcbe_ratecontrol_cpb_offset = (unsigned long)
		    avcbe_calc_cpb_buff_offset(appli_info->param.
					       avcbe_bitrate,
					       (appli_info->
						other_options_h264.
						avcbe_ratecontrol_cpb_max_size
						*
						appli_info->
						other_options_h264.
						avcbe_ratecontrol_cpb_buffer_unit_size),
					       90);
	}
	return_code =
	    avcbe_init_encode(&(appli_info->param), &(encoder->paramR),
			      &(appli_info->other_options_h264),
			      (avcbe_buf_continue_userproc_ptr) NULL,
			      &WORK_ARRY[0], &WORK_ARRY[1], context);
	if (return_code < 0) {	/* error */
		if (return_code == -1) {
			DisplayMessage
			    (" encode_1file_h264:avcbe_init_encode PARAMETER ERROR! ",
			     1);
		} else if (return_code == -2) {
			DisplayMessage
			    (" encode_1file_h264:avcbe_init_encode PARAMETER CHANGED! (Maybe Error) ",
			     1);
		} else if (return_code == -3) {
			DisplayMessage
			    (" encode_1file_h264:avcbe_init_encode SEQUENCE ERROR! ",
			     1);
		} else if (return_code == -4) {
			DisplayMessage
			    (" encode_1file_h264:avcbe_init_encode WORK-AREA SIZE SHORT ERROR! ",
			     1);
		}
		appli_info->error_return_function = -106;
		appli_info->error_return_code = return_code;
		return (-106);
	}
	nrefframe = appli_info->enc_exec_info.ref_frame_num;
	nldecfmem = 2;
	/* Local-decode-image Y */
	LDEC_ARRY[0].Y_fmemp = (unsigned char *) &my_frame_memory_ldec1[0];
	LDEC_ARRY[1].Y_fmemp = (unsigned char *) &my_frame_memory_ldec2[0];
	LDEC_ARRY[2].Y_fmemp = (unsigned char *) &my_frame_memory_ldec3[0];

	area_width = ((encoder->width + 15) / 16) * 16;	/* make it multiples of 16 */
	area_height = ((encoder->height + 15) / 16) * 16;

	/* Local-decode-image C */
	LDEC_ARRY[0].C_fmemp =
	    (unsigned char *) (LDEC_ARRY[0].Y_fmemp +
			       (area_width * area_height));
	LDEC_ARRY[1].C_fmemp =
	    (unsigned char *) (LDEC_ARRY[1].Y_fmemp +
			       (area_width * area_height));
	LDEC_ARRY[2].C_fmemp =
	    (unsigned char *) (LDEC_ARRY[2].Y_fmemp +
			       (area_width * area_height));

	/* Start address of the image-capture-field area must be arranged in 32 bytes alignment. */
	/* And, this area must be arranged in non-cacheable space. */
	addr_temp = (unsigned long) my_frame_memory_capt[0];
	addr_y = (unsigned long *) addr_temp;
	addr_c =
	    (unsigned long *) (addr_temp + (area_width * area_height));

	/* Capture-image */
	CAPTF_ARRY.Y_fmemp = (unsigned char *) addr_y;
	CAPTF_ARRY.C_fmemp = (unsigned char *) addr_c;
	/*--- The MPEG-4&H.264 Encoder Library API(required-5)@specify the address in the image-work-field area ---*/
	return_code =
	    avcbe_init_memory(*context, nrefframe, nldecfmem, LDEC_ARRY,
			      area_width, area_height);
	if (return_code != 0) {
		if (return_code == -1) {
			DisplayMessage
			    (" encode_1file_h264:avcbe_init_memory PARAMETER ERROR! ",
			     1);
		} else if (return_code == -3) {
			DisplayMessage
			    (" encode_1file_h264:avcbe_init_memory SEQUENCE ERROR! ",
			     1);
		} else if (return_code == -4) {
			DisplayMessage
			    (" encode_1file_h264:avcbe_init_encode WORK-AREA SIZE SHORT ERROR! ",
			     1);
		}
		appli_info->error_return_function = -107;
		appli_info->error_return_code = return_code;
		return (-107);
	}
	return (0);
}

/*----------------------------------------------------------------------------------------------*/
/* Encode by 1 picture unit without/with using slice division for H.264                         */
/*----------------------------------------------------------------------------------------------*/
long encode_picture_unit(SHCodecs_Encoder * encoder,
			 APPLI_INFO * appli_info, long stream_type,
			 avcbe_stream_info * context)
{

	unsigned long ldec, ref1, ref2;
	long streamsize_total, streamsize_per_frame;
	long return_code;
	long frm;
	char messeage_buf[256];
	unsigned char *ptr;
	unsigned long *addr_y, *addr_c;
	long area_width, area_height, i;
	avcbe_slice_stat slice_stat;	/* for H.264 */
	long header_output_flag;	/* set 1 if output SPS and PPS *//* for H.264 */
	TAVCBE_STREAM_BUFF *extra_stream_buff;	/* set address if output AUD *//* for H.264 */
	struct timeval tv, tv1;
	struct timezone tz;
	long tm, ret;

#ifdef DEBUG
	printf("encode_picture_unit IN\n");
#endif

	addr_y = (unsigned long *) CAPTF_ARRY.Y_fmemp;
	addr_c = (unsigned long *) CAPTF_ARRY.C_fmemp;

	area_width = ((encoder->width + 15) / 16) * 16;	/* make it multiples of 16 */
	area_height = ((encoder->height + 15) / 16) * 16;

	if (appli_info->other_options_h264.avcbe_out_vui_parameters == AVCBE_ON) {	/* output VUI parameters */
		/* get the size of CPB-buffer to set 'cpb_size_scale' of HRD */
		return_code = avcbe_get_cpb_buffer_size(context);
		if (return_code <= 0) {
			appli_info->error_return_function = -112;
			appli_info->error_return_code = return_code;
			return (-112);
		} else {
			if (encoder->other_API_enc_param.vui_main_param.
			    avcbe_nal_hrd_parameters_present_flag ==
			    AVCBE_ON) {
				encoder->other_API_enc_param.
				    vui_main_param.avcbe_nal_hrd_param.
				    avcbe_cpb_size_scale = return_code;

			} else if (encoder->other_API_enc_param.
				   vui_main_param.
				   avcbe_vcl_hrd_parameters_present_flag ==
				   AVCBE_ON) {
				encoder->other_API_enc_param.
				    vui_main_param.avcbe_vcl_hrd_param.
				    avcbe_cpb_size_scale = return_code;
			}
		}
		encoder->other_API_enc_param.vui_main_param.avcbe_video_signal_type_present_flag = AVCBE_OFF;	//@061215 AVCBE_ON;
		return_code =
		    avcbe_set_VUI_parameters(context,
					     &
					     (encoder->other_API_enc_param.
					      vui_main_param));
		if (return_code != 0) {
			appli_info->error_return_function = -113;
			appli_info->error_return_code = return_code;
			return (-113);
		}
	}
	ldec = 0;		/* Index number of the image-work-field area (0 to N-1) (for avcbe_set_image */
	ref1 = ref2 = 0;
	frm = 0;		/* Frame number to be encoded (for avcbe_encode_picture function) */
	encoder->frame_counter = 0;
	encoder->frame_skip_num = 0;
	streamsize_total = 0;
	encoder->set_intra = AVCBE_ANY_VOP;	/* Forced intra-mode flag */
	encoder->slice_mb_counter = 0;
	encoder->mb_num_of_picture = 0;
	header_output_flag = 1;	/* set to output SPS and PPS for 1st frame */
	/* stream-output-buffer */
	my_stream_buff_info.buff_top =
	    (unsigned char *) &my_stream_buff[0];
	my_stream_buff_info.buff_size = MY_STREAM_BUFF_SIZE;
	/* Access Unit Delimiter-output-buffer */
	my_extra_stream_buff_info.buff_top =
	    (unsigned char *) &my_extra_stream_buff[0];
	my_extra_stream_buff_info.buff_size = 16;
	extra_stream_buff = &my_extra_stream_buff_info;
	/* SPS-output-buffer */
	my_sps_stream_buff_info.buff_top =
	    ALIGN(&my_sps_stream_buff[0], 32);
	my_sps_stream_buff_info.buff_size = MY_SPS_STREAM_BUFF_SIZE;

	/* PPS-output-buffer */
	my_pps_stream_buff_info.buff_top =
	    ALIGN(&my_pps_stream_buff[0], 32);
	my_pps_stream_buff_info.buff_size = MY_PPS_STREAM_BUFF_SIZE;

	/* SEI-output-buffer */
	my_sei_stream_buff_info.buff_top =
	    (unsigned char *) &my_sei_stream_buff[0];
	my_sei_stream_buff_info.buff_size = MY_SEI_STREAM_BUFF_SIZE;

	/* Filler Data-output-buffer */
	my_filler_data_buff_info.buff_top =
	    (unsigned char *) &my_filler_data_buff[0];
	my_filler_data_buff_info.buff_size = MY_FILLER_DATA_BUFF_SIZE;
	appli_info->frame_number_to_encode =
	    appli_info->enc_exec_info.frame_number_to_encode;
	while (1) {	/*--------------------- Repeating by frame numbers --------------------------*/
		if (appli_info->frame_number_to_encode ==
		    encoder->frame_counter) {
			break;
		}
		appli_info->output_filler_data = 0;	/* for Filler data(CPB Buffer) */

		/* output SPS and PPS for 1st frame */
		if (header_output_flag == 1) {
			/* output SPS data */
			encoder->output_type = AVCBE_OUTPUT_SPS;
			return_code =
			    avcbe_encode_picture(context, frm,
						 encoder->set_intra,
						 encoder->output_type,
						 &my_sps_stream_buff_info,
						 NULL);
			if (return_code == AVCBE_SPS_OUTPUTTED) {	/* 6 */
#if 0
				sprintf(messeage_buf,
					" encode_1file_h264:avcbe_encode_picture OUTPUT SEQUENCE PARAMETER SET frm=%d,seq_no=%d ",
					(int) frm,
					(int) encoder->frame_counter);
				DisplayMessage(messeage_buf, 1);
#endif
				/* get the size of SPS data in byte unit */
				avcbe_get_last_slice_stat(context,
							  &slice_stat);
				encoder->SPS_PPS_header_bytes =
				    slice_stat.avcbe_SPS_unit_bytes;
			} else {
				sprintf(messeage_buf,
					"%s:%d: ERROR OUTPUT SEQUENCE PARAMETER SET return_code=%d ",
					__FUNCTION__, __LINE__,
					(int) return_code);
				DisplayMessage(messeage_buf, 1);
				appli_info->error_return_function = -111;
				appli_info->error_return_code =
				    return_code;
				return (-111);
			}

			/* output PPS data */
			encoder->output_type = AVCBE_OUTPUT_PPS;
			return_code =
			    avcbe_encode_picture(context, frm,
						 encoder->set_intra,
						 encoder->output_type,
						 &my_pps_stream_buff_info,
						 NULL);
			if (return_code == AVCBE_PPS_OUTPUTTED) {	/* 7 */
				//sprintf(messeage_buf, " encode_1file:avcbe_encode_picture OUTPUT PICTURE PARAMETER SET frm=%d,seq_no=%d ",
				//                       (int)frm, (int)encoder->frame_counter);
				sprintf(messeage_buf,
					"Encoded frame %5d, sequence no %5d",
					(int) frm,
					(int) encoder->frame_counter);
				DisplayMessage(messeage_buf, 1);

				/* get the size of PPS data in byte unit */
				avcbe_get_last_slice_stat(context,
							  &slice_stat);
				encoder->SPS_PPS_header_bytes +=
				    slice_stat.avcbe_PPS_unit_bytes;
			} else {
				sprintf(messeage_buf,
					" ERROR OUTPUT PICTURE PARAMETER SET return_code=%d ",
					(int) return_code);
				DisplayMessage(messeage_buf, 1);
				appli_info->error_return_function = -111;
				appli_info->error_return_code =
				    return_code;
				return (-111);
			}
		}

		/* output SEI data (if AU delimiter is NOT used) */
		if (extra_stream_buff == NULL) {
			return_code =
			    output_SEI_parameters(encoder, appli_info,
						  context,
						  &my_sei_stream_buff_info);
			if (return_code != 0) {
				sprintf(messeage_buf,
					" Not put SEI parameter : return_code = %06x\n",
					(int) return_code);
				DisplayMessage(messeage_buf, 1);
			} else {
#if 0
				sprintf(messeage_buf,
					"%s:%d: Put SEI parameter : return_code = %d\n",
					__FUNCTION__, __LINE__,
					(int) return_code);
				DisplayMessage(messeage_buf, 1);
#endif
			}
		}

		/*--- copy yuv data to the image-capture-field area each frame (one of the user application's own functions) ---*/
		if (encoder->input) {
			/* return_code = load_1frame_from_image_file(appli_info, addr_y, addr_c); */
			/* return_code = capture_image (appli_info, addr_y, addr_c); */
			return_code =
			    encoder->input(encoder, addr_y, addr_c,
					   appli_info);
			if (return_code < 0) {	/* error */
				DisplayMessage
				    (" encode_1file_h264: ERROR acquiring input image! ",
				     1);
				appli_info->error_return_function = -108;
				appli_info->error_return_code =
				    return_code;
				return (-108);
			}
		}

		/*--- The MPEG-4 Encoder Library API(required-7)@specify the address in the capture-image-field area ---*/
		return_code =
		    avcbe_set_image_pointer(context, &CAPTF_ARRY, ldec,
					    ref1, ref2);
		if (return_code != 0) {
			if (return_code == -1) {
				DisplayMessage
				    (" encode_1file_h264:avcbe_set_image_pointer PARAMETER ERROR! ",
				     1);
			}
			appli_info->error_return_function = -109;
			appli_info->error_return_code = return_code;
			return (-109);
		}
		/* set to output slice header and slice data */
		encoder->output_type = AVCBE_OUTPUT_SLICE;	/* Header insertion flag */
		/*--- The MPEG-4 Encoder Library API (required-8)@encode each screen of display data ---*/
//              printf("enc_pic0=%ld,",tv.tv_usec);
		gettimeofday(&tv, &tz);
		return_code =
		    avcbe_encode_picture(context, frm,
					 encoder->set_intra,
					 encoder->output_type,
					 &my_stream_buff_info,
					 extra_stream_buff);
		gettimeofday(&tv1, &tz);
//              printf("ret=%d,enc_pic1=%ld,",return_code,tv1.tv_usec);
		tm = (tv1.tv_usec - tv.tv_usec) / 1000;
		if (tm < 0) {
			tm = 1000 - (tv.tv_usec - tv1.tv_usec) / 1000;
		}
		encode_time += tm;

#ifdef DEBUG
		printf
		    ("encode_picture_unit: avcbe_encode_picture() returned %d\n",
		     return_code);
#endif

//              printf("Total encode time = %d(msec),",encode_time_get());
//              printf("Total sleep  time = %d(msec)\n",m4iph_sleep_time_get());
		if (return_code < 0) {	/* error */
			if (return_code == AVCBE_ENCODE_ERROR) {	/* -1 */
				printf("%s:%d:", __FUNCTION__, __LINE__);
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture PARAMETER ERROR! ",
				     1);

			} else if (return_code == AVCBE_NOT_IN_ORDER_ERROR) {	/* -3 */
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture SEQUENCE ERROR! ",
				     1);

			} else if (return_code == AVCBE_OUTPUT_BUFFER_SHORT_ERROR) {	/* -4 */
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture OUTPUT BUFFER SIZE SHORT ERROR! ",
				     1);

			} else if (return_code == AVCBE_VPU_ERROR_AFTER_ENCODING) {	/* -6 */
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture VPU4 ERROR! ",
				     1);

			} else if (return_code == AVCBE_TEMPORARY_BUFFER_SHORT_ERROR) {	/* -8 */
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture TEMPORARY_BUFFER SIZE SHORT ERROR! ",
				     1);

			} else if (return_code == AVCBE_VUI_PARAMETERS_NOT_SPECIFIED_ERROR) {	/* -9 */
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture VUI_PARAMETERS NOT SPECIFIED ERROR! ",
				     1);

			} else if (return_code == AVCBE_WORK_AREA_SHORT_ERROR) {	/* -99 */
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture OTHER WORK SIZE SHORT ERROR! ",
				     1);

			}
			appli_info->error_return_function = -111;
			appli_info->error_return_code = return_code;
			return (-111);

		} else if (return_code == AVCBE_ENCODE_SUCCESS) {	/* 0 */
//                      sprintf(messeage_buf, " encode_1file_h264:avcbe_encode_picture SUCCESS  frm=%d,seq_no=%d ", (int)frm, (int)encoder->frame_counter);
//                      DisplayMessage(messeage_buf, 1);
		} else if (return_code == AVCBE_FRAME_SKIPPED) {	/* 1 */
			sprintf(messeage_buf,
				" encode_1file_h264:avcbe_encode_picture THIS FRAME SKIPPED(Not Encoded)  frm=%d,seq_no=%d ",
				(int) frm,
				(int) encoder->frame_counter);
			DisplayMessage(messeage_buf, 1);

			/* the second parameter 'ldec' value must NOT be changed when the avcbe_set_image_pointer function is called on next time. */
			encoder->frame_skip_num++;

		} else if (return_code == AVCBE_SLICE_REMAIN) {	/* 5 */
			sprintf(messeage_buf,
				" encode_1file_h264:avcbe_encode_picture YET 1 PICTURE NOT FINISHED frm=%d,seq_no=%d ",
				(int) frm,
				(int) encoder->frame_counter);
			DisplayMessage(messeage_buf, 1);
		} else {
			sprintf(messeage_buf,
				" encode_1file_h264:avcbe_encode_picture UNKNOWN RETURN CODE=%d  frm=%d,seq_no=%d ",
				(int) return_code, (int) frm,
				(int) encoder->frame_counter);
			DisplayMessage(messeage_buf, 1);
		}
		/* get the information about the just encoded frame (slice) */
		avcbe_get_last_slice_stat(context, &slice_stat);
		encoder->slice_mb_counter =
		    slice_stat.avcbe_encoded_MB_num;
		encoder->mb_num_of_picture =
		    slice_stat.avcbe_total_MB_in_frame;
		if (return_code == AVCBE_ENCODE_SUCCESS) {	/* If the return value of avcbe_encode_picture function is 1 */
			/* 1 picture unit */
			if (ldec == 0) {
				ldec = 1;
				ref1 = 0;
			} else {
				ldec = 0;
				ref1 = 1;
			}
			/*--- copy H.264 bitstream of the frame encoded now (one of the user application's own functions) ---*/
			/* output AU delimiter */
			if (extra_stream_buff != NULL) {
				if (encoder->output) {
					encoder->output(encoder,
							(unsigned char *)
							&my_extra_stream_buff
							[0],
							slice_stat.avcbe_AU_unit_bytes,
							encoder->output_user_data);
				}
			}

			/* If the type is IDR-picture, output SPS and PPS data (for 2nd frame and later) */
			if ((header_output_flag == 0)
			    && (slice_stat.avcbe_encoded_pic_type ==
				AVCBE_IDR_PIC)) {
				/* set to output SPS data */
				encoder->output_type = AVCBE_OUTPUT_SPS;
				return_code =
				    avcbe_encode_picture(context, frm,
							 encoder->
							 set_intra,
							 encoder->
							 output_type,
							 &my_sps_stream_buff_info,
							 NULL);
				if (return_code == AVCBE_SPS_OUTPUTTED) {	/* 6 */
#if 0
					sprintf(messeage_buf,
						" encode_1file_h264:avcbe_encode_picture OUTPUT SEQUENCE PARAMETER SET frm=%d,seq_no=%d ",
						(int) frm, (int)
						encoder->frame_counter);
					DisplayMessage(messeage_buf, 1);
#endif

					/* get the size of SPS data in byte unit */
					avcbe_get_last_slice_stat(context,
								  &slice_stat);
					encoder->SPS_PPS_header_bytes =
					    slice_stat.
					    avcbe_SPS_unit_bytes;
					/* concatenate the SPS data */
					if (encoder->output) {
						encoder->output(encoder,
								(unsigned
								 char *)
								my_sps_stream_buff_info.buff_top,
								slice_stat.avcbe_SPS_unit_bytes,
								encoder->output_user_data);
					}
				} else {
					sprintf(messeage_buf,
						" ERROR OUTPUT SEQUENCE PARAMETER SET return_code=%d ",
						(int) return_code);
					DisplayMessage(messeage_buf, 1);
					appli_info->error_return_function =
					    -111;
					appli_info->error_return_code =
					    return_code;
					return (-111);
				}

				/* set to output PPS data */
				encoder->output_type = AVCBE_OUTPUT_PPS;
				return_code =
				    avcbe_encode_picture(context, frm,
							 encoder->
							 set_intra,
							 encoder->
							 output_type,
							 &my_pps_stream_buff_info,
							 NULL);
				if (return_code == AVCBE_PPS_OUTPUTTED) {	/* 7 */
					//sprintf(messeage_buf, " encode_1file_h264:avcbe_encode_picture OUTPUT PICTURE PARAMETER SET frm=%d,seq_no=%d ",
					//                       (int)frm, (int)encoder->frame_counter);
					sprintf(messeage_buf,
						"Encoded frame %5d, sequence no %5d",
						(int) frm, (int)
						encoder->frame_counter);
					DisplayMessage(messeage_buf, 1);

					/* get the size of PPS data in byte unit */
					avcbe_get_last_slice_stat(context,
								  &slice_stat);
					encoder->SPS_PPS_header_bytes +=
					    slice_stat.
					    avcbe_PPS_unit_bytes;
					/* concatenate the PPS data */
					if (encoder->output) {
						encoder->output(encoder,
								(unsigned
								 char *)
								my_pps_stream_buff_info.buff_top,
								slice_stat.avcbe_PPS_unit_bytes,
								encoder->output_user_data);
					}
				} else {
					sprintf(messeage_buf,
						" ERROR OUTPUT PICTURE PARAMETER SET return_code=%d ",
						(int) return_code);
					DisplayMessage(messeage_buf, 1);
					appli_info->error_return_function =
					    -111;
					appli_info->error_return_code =
					    return_code;
					return (-111);
				}
			} else if (header_output_flag == 1) {

				/* output SPS data and PPS data after AU Delimiter */
				/* concatenate the SPS data */
				if (encoder->output) {
					encoder->output(encoder,
							(unsigned char *)
							my_sps_stream_buff_info.buff_top,
							slice_stat.avcbe_SPS_unit_bytes,
							encoder->output_user_data);
				}
				/* concatenate the PPS data */
				if (encoder->output) {
					encoder->output(encoder,
							(unsigned char *)
							my_pps_stream_buff_info.buff_top,
							slice_stat.avcbe_PPS_unit_bytes,
							encoder->output_user_data);
				}
				/* clear the 1st-frame-flag */
				header_output_flag = 0;
			}

			/* output SEI parameter (if AU delimiter is used, SEI parameter must be output after AU delimiter) */
			return_code =
			    output_SEI_parameters(encoder, appli_info,
						  context,
						  &my_sei_stream_buff_info);
			if (return_code != 0) {
				sprintf(messeage_buf,
					" Not put SEI parameter : return_code = %06x\n",
					(int) return_code);
				DisplayMessage(messeage_buf, 1);
			} else {
#if 0
				sprintf(messeage_buf,
					"%s:%d Put SEI parameter : return_code = %d\n",
					__FUNCTION__, __LINE__,
					(int) return_code);
				DisplayMessage(messeage_buf, 1);
#endif
			}

			/* concatenate Filler data(if CPB Buffer overflow) */
			if ((appli_info->output_filler_enable == 1)
			    && (appli_info->output_filler_data > 0)) {
				if (encoder->output) {
					encoder->output(encoder,
							(unsigned char *)
							&my_filler_data_buff
							[0],
							appli_info->output_filler_data,
							encoder->output_user_data);
				}
			}

			/* concatenate slice header and slice data */
			streamsize_per_frame =
			    ((slice_stat.avcbe_encoded_slice_bits +
			      7) >> 3);
			if (encoder->output) {
				encoder->output(encoder, (unsigned char *)
						&my_stream_buff[0],
						streamsize_per_frame,
						encoder->output_user_data);
			}
#if 0
			sprintf(messeage_buf,
				" encode_1file_h264:avcbe_encode_picture SUCCESS  frm=%d,seq_no=%d,size=%d ",
				(int) frm, (int) encoder->frame_counter,
				streamsize_per_frame);
			DisplayMessage(messeage_buf, 1);
#endif
		}		/* the end of 'if (return_code == AVCBE_ENCODE_SUCCESS)' */
		if (return_code == AVCBE_ENCODE_SUCCESS) {
			streamsize_total +=
			    (slice_stat.avcbe_encoded_slice_bits / 8);
		}
		frm += appli_info->frame_no_increment;
		encoder->frame_counter++;
	}			/* while */
	/*---------------------- End of repeating by frame numbers -----------------------------*/
#ifdef DEBUG
	printf("encode_picture_unit OUT\n");
#endif

	return (0);
}

/*----------------------------------------------------------------------------------------------*/
/* Encode by NAL unit for H.264                                                                 */
/*----------------------------------------------------------------------------------------------*/
long encode_nal_unit(SHCodecs_Encoder * encoder,
		     APPLI_INFO * appli_info, long stream_type,
		     avcbe_stream_info * context)
{
	unsigned long ldec, ref1, ref2;
	long streamsize_total;
	long return_code;
	long frm;
	char messeage_buf[256];
	unsigned long *addr_y, *addr_c;
	long area_width, area_height;

	avcbe_slice_stat slice_stat;	/* for H.264 */
	long header_output_flag;	/* set 1 if output SPS and PPS *//* for H.264 */
	TAVCBE_STREAM_BUFF *extra_stream_buff;	/* set address if output AUD *//* for H.264 */
	long index;
	struct timeval tv, tv1;
	struct timezone tz;
	long tm, ret;

#ifdef DEBUG
	printf("encode_nal_unit\n");
#endif

	addr_y = (unsigned long *) CAPTF_ARRY.Y_fmemp;
	addr_c = (unsigned long *) CAPTF_ARRY.C_fmemp;

	area_width = ((encoder->width + 15) / 16) * 16;	/* make it multiples of 16 */
	area_height = ((encoder->height + 15) / 16) * 16;

	if (appli_info->other_options_h264.avcbe_out_vui_parameters == AVCBE_ON) {	/* output VUI parameters */

		/* get the size of CPB-buffer to set 'cpb_size_scale' of HRD */
		return_code = avcbe_get_cpb_buffer_size(context);

		if (return_code <= 0) {
			appli_info->error_return_function = -112;
			appli_info->error_return_code = return_code;
			return (-112);
		} else {
			if (encoder->other_API_enc_param.vui_main_param.
			    avcbe_nal_hrd_parameters_present_flag ==
			    AVCBE_ON) {
				encoder->other_API_enc_param.
				    vui_main_param.avcbe_nal_hrd_param.
				    avcbe_cpb_size_scale = return_code;

			} else if (encoder->other_API_enc_param.
				   vui_main_param.
				   avcbe_vcl_hrd_parameters_present_flag ==
				   AVCBE_ON) {
				encoder->other_API_enc_param.
				    vui_main_param.avcbe_vcl_hrd_param.
				    avcbe_cpb_size_scale = return_code;
			}
		}

		return_code =
		    avcbe_set_VUI_parameters(context,
					     &(encoder->
					       other_API_enc_param.
					       vui_main_param));
		if (return_code != 0) {
			appli_info->error_return_function = -113;
			appli_info->error_return_code = return_code;
			return (-113);
		}
	}

	ldec = 0;		/* Index number of the image-work-field area (0 to N-1) (for avcbe_set_image_pointer function) */
	ref1 = ref2 = 0;
	frm = 0;		/* Frame number to be encoded (for avcbe_encode_picture function) */

	encoder->frame_counter = 0;
	encoder->frame_skip_num = 0;

	streamsize_total = 0;

	encoder->set_intra = AVCBE_ANY_VOP;	/* Forced intra-mode flag */

	encoder->slice_mb_counter = 0;
	encoder->mb_num_of_picture = 0;

	header_output_flag = 1;	/* set to output SPS and PPS for 1st frame */

	/* stream-output-buffer */
	my_stream_buff_info.buff_top =
	    (unsigned char *) &my_stream_buff[0];
	my_stream_buff_info.buff_size = MY_STREAM_BUFF_SIZE;

	/* Access Unit Delimiter-output-buffer */
	my_extra_stream_buff_info.buff_top =
	    (unsigned char *) &my_extra_stream_buff[0];
	my_extra_stream_buff_info.buff_size = 16;
	extra_stream_buff = &my_extra_stream_buff_info;

	/* SPS-output-buffer */
	my_sps_stream_buff_info.buff_top =
	    ALIGN(&my_sps_stream_buff[0], 32);
	my_sps_stream_buff_info.buff_size = MY_SPS_STREAM_BUFF_SIZE;

	/* PPS-output-buffer */
	my_pps_stream_buff_info.buff_top =
	    ALIGN(&my_pps_stream_buff[0], 32);
	my_pps_stream_buff_info.buff_size = MY_PPS_STREAM_BUFF_SIZE;

	/* SEI-output-buffer */
	my_sei_stream_buff_info.buff_top =
	    (unsigned char *) &my_sei_stream_buff[0];
	my_sei_stream_buff_info.buff_size = MY_SEI_STREAM_BUFF_SIZE;

	/* Filler Data-output-buffer */
	my_filler_data_buff_info.buff_top =
	    (unsigned char *) &my_filler_data_buff[0];
	my_filler_data_buff_info.buff_size = MY_FILLER_DATA_BUFF_SIZE;

	dummy_nal_buf_addr = dummy_nal_buf;
	appli_info->frame_number_to_encode =
	    appli_info->enc_exec_info.frame_number_to_encode;
	while (1) {
		/*--------------------- Repeating by frame numbers --------------------------*/

		if (appli_info->frame_number_to_encode ==
		    encoder->frame_counter) {
			break;
		}
		appli_info->output_filler_data = 0;	/* for Filler data(CPB Buffer) */

		/*--- The MPEG-4 Encoder Library API (Not required) sets the target stream ---*/
		encoder->frm = frm;

		/* output SPS and PPS for 1st frame */
		if (header_output_flag == 1) {
			/* output SPS data */
			encoder->output_type = AVCBE_OUTPUT_SPS;
			return_code =
			    avcbe_encode_picture(context, frm,
						 encoder->set_intra,
						 encoder->output_type,
						 &my_sps_stream_buff_info,
						 NULL);

			if (return_code == AVCBE_SPS_OUTPUTTED) {	/* 6 */
#if 0
				sprintf(messeage_buf,
					" encode_1file:avcbe_encode_picture OUTPUT SEQUENCE PARAMETER SET frm=%d,seq_no=%d ",
					(int) frm,
					(int) encoder->frame_counter);
				DisplayMessage(messeage_buf, 1);
#endif

				/* get the size of SPS data in byte unit */
				avcbe_get_last_slice_stat(context,
							  &slice_stat);
				encoder->SPS_PPS_header_bytes =
				    slice_stat.avcbe_SPS_unit_bytes;
			} else {
				sprintf(messeage_buf,
					" ERROR OUTPUT SEQUENCE PARAMETER SET return_code=%d ",
					(int) return_code);
				DisplayMessage(messeage_buf, 1);
				appli_info->error_return_function = -111;
				appli_info->error_return_code =
				    return_code;
				return (-111);
			}
			/* output PPS data */
			encoder->output_type = AVCBE_OUTPUT_PPS;
			return_code =
			    avcbe_encode_picture(context, frm,
						 encoder->set_intra,
						 encoder->output_type,
						 &my_pps_stream_buff_info,
						 NULL);
			if (return_code == AVCBE_PPS_OUTPUTTED) {	/* 7 */
				//sprintf(messeage_buf, " encode_1file:avcbe_encode_picture OUTPUT PICTURE PARAMETER SET frm=%d,seq_no=%d ",
				//                       (int)frm, (int)encoder->frame_counter);
				sprintf(messeage_buf,
					"Encoded frame %5d, sequence no %5d",
					(int) frm,
					(int) encoder->frame_counter);
				DisplayMessage(messeage_buf, 1);

				/* get the size of PPS data in byte unit */
				avcbe_get_last_slice_stat(context,
							  &slice_stat);
				encoder->SPS_PPS_header_bytes +=
				    slice_stat.avcbe_PPS_unit_bytes;
			} else {
				sprintf(messeage_buf,
					" ERROR OUTPUT PICTURE PARAMETER SET return_code=%d ",
					(int) return_code);
				DisplayMessage(messeage_buf, 1);
				appli_info->error_return_function = -111;
				appli_info->error_return_code =
				    return_code;
				return (-111);
			}
		}
		/* output SEI data (if AU delimiter is NOT used) */
		if (extra_stream_buff == NULL) {
			return_code =
			    output_SEI_parameters(encoder, appli_info,
						  context,
						  &my_sei_stream_buff_info);
			if (return_code != 0) {
				sprintf(messeage_buf,
					" Not put SEI parameter : return_code = %06x\n",
					(int) return_code);
				DisplayMessage(messeage_buf, 1);
			} else {
				sprintf(messeage_buf,
					"%s:%d Put SEI parameter : return_code = %d\n",
					__FUNCTION__, __LINE__,
					(int) return_code);
				DisplayMessage(messeage_buf, 1);
			}
		}

	/*--- copy yuv data to the image-capture-field area each frame (one of the user application's own functions) ---*/
		if (encoder->slice_mb_counter == 0) {
			if (encoder->input) {
				/* return_code = load_1frame_from_image_file(appli_info, addr_y, addr_c); */
				/* return_code = capture_image (appli_info, addr_y, addr_c); */
				return_code =
				    encoder->input(encoder, addr_y, addr_c,
						   appli_info);
				if (return_code < 0) {	/* error */
					DisplayMessage
					    (" encode_1file_h264: ERROR acquiring input image! ",
					     1);
					appli_info->error_return_function =
					    -108;
					appli_info->error_return_code =
					    return_code;
					return (-108);
				}
			}
		}

		/* If NAL unit, the avcbe_set_image_pointer function is called at 1st slice */
		/*--- The MPEG-4 Encoder Library API(required-7)@specify the address in the capture-image-field area ---*/
		if (encoder->slice_mb_counter == 0) {
			return_code =
			    avcbe_set_image_pointer(context, &CAPTF_ARRY,
						    ldec, ref1, ref2);

			if (return_code != 0) {
				if (return_code == -1) {
					DisplayMessage
					    (" encode_1file_h264:avcbe_set_image_pointer PARAMETER ERROR! ",
					     1);
				}
				appli_info->error_return_function = -109;
				appli_info->error_return_code =
				    return_code;
				return (-109);
			}
		}

		/* set to output slice header and slice data */
		encoder->output_type = AVCBE_OUTPUT_SLICE;	/* Header insertion flag */
		gettimeofday(&tv, &tz);
		printf("enc_pic0=%ld,", tv.tv_usec);
		/*--- The MPEG-4 Encoder Library API (required-8)@encode each screen of display data ---*/
		return_code =
		    avcbe_encode_picture(context, frm,
					 encoder->set_intra,
					 encoder->output_type,
					 &my_stream_buff_info,
					 extra_stream_buff);
		gettimeofday(&tv1, &tz);
		printf("ret=%d,enc_pic1=%ld,", return_code, tv1.tv_usec);
		tm = (tv1.tv_usec - tv.tv_usec) / 1000;
		if (tm < 0) {
			tm = 1000 - (tv.tv_usec - tv1.tv_usec) / 1000;
		}
		encode_time += tm;
		printf("Total encode time = %d(msec),", encode_time_get());
		printf("Total sleep  time = %d(msec)\n",
		       m4iph_sleep_time_get());
		if (return_code < 0) {	/* error */
			if (return_code == AVCBE_ENCODE_ERROR) {	/* -1 */
				printf("%s:", __FUNCTION__);
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture PARAMETER ERROR! ",
				     1);

			} else if (return_code == AVCBE_NOT_IN_ORDER_ERROR) {	/* -3 */
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture SEQUENCE ERROR! ",
				     1);

			} else if (return_code == AVCBE_OUTPUT_BUFFER_SHORT_ERROR) {	/* -4 */
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture OUTPUT BUFFER SIZE SHORT ERROR! ",
				     1);

			} else if (return_code == AVCBE_VPU_ERROR_AFTER_ENCODING) {	/* -6 */
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture VPU4 ERROR! ",
				     1);

			} else if (return_code == AVCBE_TEMPORARY_BUFFER_SHORT_ERROR) {	/* -8 */
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture TEMPORARY_BUFFER SIZE SHORT ERROR! ",
				     1);

			} else if (return_code == AVCBE_VUI_PARAMETERS_NOT_SPECIFIED_ERROR) {	/* -9 */
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture VUI_PARAMETERS NOT SPECIFIED ERROR! ",
				     1);

			} else if (return_code == AVCBE_WORK_AREA_SHORT_ERROR) {	/* -99 */
				DisplayMessage
				    (" encode_1file_h264:avcbe_encode_picture OTHER WORK SIZE SHORT ERROR! ",
				     1);

			}

			appli_info->error_return_function = -111;
			appli_info->error_return_code = return_code;
			return (-111);

		} else if (return_code == AVCBE_ENCODE_SUCCESS) {	/* 0 */
			sprintf(messeage_buf,
				" encode_1file_h264:avcbe_encode_picture SUCCESS  frm=%d,seq_no=%d ",
				(int) frm,
				(int) encoder->frame_counter);
			DisplayMessage(messeage_buf, 1);

		} else if (return_code == AVCBE_FRAME_SKIPPED) {	/* 1 */
			sprintf(messeage_buf,
				" encode_1file_h264:avcbe_encode_picture THIS FRAME SKIPPED(Not Encoded)  frm=%d,seq_no=%d ",
				(int) frm,
				(int) encoder->frame_counter);
			DisplayMessage(messeage_buf, 1);
			/* the second parameter 'ldec' value must NOT be changed when the avcbe_set_image_pointer function is called on next time. */
			encoder->frame_skip_num++;

		} else if (return_code == AVCBE_SLICE_REMAIN) {	/* 5 */
			sprintf(messeage_buf,
				" encode_1file_h264:avcbe_encode_picture YET 1 PICTURE NOT FINISHED frm=%d,seq_no=%d ",
				(int) frm,
				(int) encoder->frame_counter);
			DisplayMessage(messeage_buf, 1);

		} else {
			sprintf(messeage_buf,
				" encode_1file_h264:avcbe_encode_picture UNKNOWN RETURN CODE=%d  frm=%d,seq_no=%d ",
				(int) return_code, (int) frm,
				(int) encoder->frame_counter);
			DisplayMessage(messeage_buf, 1);
		}

		/* get the information about the just encoded slice */
		avcbe_get_last_slice_stat(context, &slice_stat);
		encoder->slice_mb_counter =
		    slice_stat.avcbe_encoded_MB_num;
		encoder->mb_num_of_picture =
		    slice_stat.avcbe_total_MB_in_frame;

		/* get the size in bit unit about the just encoded slice */
		tmp_slice_size = (slice_stat.avcbe_encoded_slice_bits + 7) >> 3;	/* into bit unit */

		if (return_code == AVCBE_SLICE_REMAIN) {	/* 5 */

			memcpy(dummy_nal_buf_addr, (char *) &my_stream_buff[0], tmp_slice_size);	/* in byte unit */
			slice_total_size += tmp_slice_size;
			tmp_pic_total_bytes += tmp_slice_size;	/* total size of 1-frame */
			dummy_nal_buf_addr += tmp_slice_size;	/* update the address of buffer for 1-slice */

		} else if (return_code == AVCBE_FRAME_SKIPPED) {	/* 1 */

			/* if the frame is skipped, the size of slice data which are already put, must be subtracted */
			slice_total_size -= tmp_pic_total_bytes;

			/* make the address which dummy_nal_buf_addr has, to the top address of dummy_nal_buf[] */
			dummy_nal_buf_addr = dummy_nal_buf;
			tmp_pic_total_bytes = 0;

			frm += appli_info->frame_no_increment;
			encoder->frame_counter++;
			encoder->slice_mb_counter = 0;

			/* the second parameter 'ldec' value must NOT be changed when the m4vse_set_image_pointer function is called on next time. */
			encoder->frame_skip_num++;

		} else if (return_code == AVCBE_ENCODE_SUCCESS) {	/* 0 */

			/* the second parameter 'ldec' value must be changed when the m4vse_set_image_pointer function is called on next time. */

			if ((appli_info->other_options_h264.avcbe_use_slice == AVCBE_ON) && (appli_info->other_options_h264.avcbe_call_unit == AVCBE_CALL_PER_NAL)) {	/* when the avcbe_encode_picture function returns on each 1 slice */

				if (encoder->slice_mb_counter == encoder->mb_num_of_picture) {	/* when all slices of 1-picture are finished */
					if (ldec == 0) {
						ldec = 1;
						ref1 = 0;
					} else {
						ldec = 0;
						ref1 = 1;
					}
				}
			}

			/*--- copy H.264 bitstream of the frame encoded now (one of the user application's own functions) ---*/
			/* output AU delimiter */
			if (extra_stream_buff != NULL) {
				if (encoder->output) {
					encoder->output(encoder,
							(unsigned char *)
							&my_extra_stream_buff
							[0],
							slice_stat.avcbe_AU_unit_bytes,
							encoder->output_user_data);
				}
				DisplayMessage("Put Access Unit Delimiter",
					       1);
			}

			/* If the type is IDR-picture, output SPS and PPS data (for 2nd frame and later) */
			if ((header_output_flag == 0)
			    && (slice_stat.avcbe_encoded_pic_type ==
				AVCBE_IDR_PIC)) {
				/* set to output SPS data */
				encoder->output_type = AVCBE_OUTPUT_SPS;

				return_code =
				    avcbe_encode_picture(context, frm,
							 encoder->
							 set_intra,
							 encoder->
							 output_type,
							 &my_sps_stream_buff_info,
							 NULL);

				if (return_code == AVCBE_SPS_OUTPUTTED) {	/* 6 */
#if 0
					sprintf(messeage_buf,
						" encode_1file:avcbe_encode_picture OUTPUT SEQUENCE PARAMETER SET frm=%d,seq_no=%d ",
						(int) frm, (int)
						encoder->frame_counter);
					DisplayMessage(messeage_buf, 1);
#endif

					/* get the size of SPS data in byte unit */
					avcbe_get_last_slice_stat(context,
								  &slice_stat);
					encoder->SPS_PPS_header_bytes =
					    slice_stat.
					    avcbe_SPS_unit_bytes;

					/* concatenate the SPS data */
					if (encoder->output) {
						encoder->output(encoder,
								(unsigned
								 char *)
								my_sps_stream_buff_info.buff_top,
								slice_stat.avcbe_SPS_unit_bytes,
								encoder->output_user_data);
					}

				} else {
					sprintf(messeage_buf,
						"%s: ERROR OUTPUT SEQUENCE PARAMETER SET return_code=%d ",
						__FUNCTION__,
						(int) return_code);
					DisplayMessage(messeage_buf, 1);
					appli_info->error_return_function =
					    -111;
					appli_info->error_return_code =
					    return_code;
					return (-111);
				}

				/* set to output PPS data */
				encoder->output_type = AVCBE_OUTPUT_PPS;
				return_code =
				    avcbe_encode_picture(context, frm,
							 encoder->
							 set_intra,
							 encoder->
							 output_type,
							 &my_pps_stream_buff_info,
							 NULL);
				if (return_code == AVCBE_PPS_OUTPUTTED) {	/* 7 */
					//sprintf(messeage_buf, " encode_1file:avcbe_encode_picture OUTPUT PICTURE PARAMETER SET frm=%d,seq_no=%d ", 
					//                      (int)frm, (int)encoder->frame_counter);
					sprintf(messeage_buf,
						"Encoded frame %5d, sequence no %5d",
						(int) frm, (int)
						encoder->frame_counter);
					DisplayMessage(messeage_buf, 1);

					/* set to output PPS data */
					avcbe_get_last_slice_stat(context,
								  &slice_stat);
					encoder->SPS_PPS_header_bytes +=
					    slice_stat.
					    avcbe_PPS_unit_bytes;

					/* concatenate the PPS data */
					if (encoder->output) {
						encoder->output(encoder,
								(unsigned
								 char *)
								my_pps_stream_buff_info.buff_top,
								slice_stat.avcbe_PPS_unit_bytes,
								encoder->output_user_data);
					}

				} else {
					sprintf(messeage_buf,
						" ERROR OUTPUT PICTURE PARAMETER SET return_code=%d ",
						(int) return_code);
					DisplayMessage(messeage_buf, 1);
					appli_info->error_return_function =
					    -111;
					appli_info->error_return_code =
					    return_code;
					return (-111);
				}

			} else if (header_output_flag == 1) {
				/* output SPS data and PPS data after AU Delimiter */
				/* concatenate the SPS data */
				if (encoder->output) {
					encoder->output(encoder,
							(unsigned char *)
							my_sps_stream_buff_info.buff_top,
							slice_stat.avcbe_SPS_unit_bytes,
							encoder->output_user_data);
				}
				/* concatenate the PPS data */
				if (encoder->output) {
					encoder->output(encoder,
							(unsigned char *)
							my_pps_stream_buff_info.buff_top,
							slice_stat.avcbe_PPS_unit_bytes,
							encoder->output_user_data);
				}
				/* clear the 1st-frame-flag */
				header_output_flag = 0;
			}

			/* output SEI parameter (if AU delimiter is used, SEI parameter must be output after AU delimiter) */
			return_code =
			    output_SEI_parameters(encoder, appli_info,
						  context,
						  &my_sei_stream_buff_info);
			if (return_code != 0) {
				sprintf(messeage_buf,
					" Not put SEI parameter : return_code = %06x\n",
					(int) return_code);
				DisplayMessage(messeage_buf, 1);
			} else {
				sprintf(messeage_buf,
					"%s:%d Put SEI parameter : return_code = %d\n",
					__FUNCTION__, __LINE__,
					(int) return_code);
				DisplayMessage(messeage_buf, 1);
			}

			/* concatenate Filler data(if CPB Buffer overflow) */
			if ((appli_info->output_filler_enable == 1)
			    && (appli_info->output_filler_data > 0)) {
				if (encoder->output) {
					encoder->output(encoder,
							(unsigned char *)
							&my_filler_data_buff
							[0],
							appli_info->output_filler_data,
							encoder->output_user_data);
				}
			}
			/* copy the bitstream of 1-slice newly encoded, and add the size to the total size of 1-frame */
			memcpy(dummy_nal_buf_addr, (char *) &my_stream_buff[0], tmp_slice_size);	/* in byte unit */
			slice_total_size += tmp_slice_size;	/* the size of 1-slice newly encoded */
			tmp_pic_total_bytes += tmp_slice_size;	/* total size of 1-frame */
			dummy_nal_buf_addr += tmp_slice_size;	/* update the address of buffer for 1-slice */
			/* concatenate slice header and slice data */
			if (encoder->output) {
				encoder->output(encoder,
						dummy_nal_buf,
						tmp_pic_total_bytes,
						encoder->output_user_data);
			}
			/* clear the size of 1-frame */
			tmp_pic_total_bytes = 0;
			/* make the address which dummy_nal_buf_addr has, to the top address of dummy_nal_buf[] */
			dummy_nal_buf_addr = dummy_nal_buf;
		}		/* the end of 'if (return_code == AVCBE_ENCODE_SUCCESS)' */
		streamsize_total +=
		    (slice_stat.avcbe_encoded_slice_bits / 8);
		/* if the avcbe_encode_picture function returns on each 1-slice, when all slices of 1-picture are finished */
		if ((appli_info->other_options_h264.avcbe_use_slice == AVCBE_ON) && (appli_info->other_options_h264.avcbe_call_unit == AVCBE_CALL_PER_NAL)) {	/* when the avcbe_encode_picture function returns on each 1-slice */
			if (encoder->slice_mb_counter == encoder->mb_num_of_picture) {	/* when all slices of 1-picture are finished */
				frm += appli_info->frame_no_increment;
				encoder->frame_counter++;
				encoder->slice_mb_counter = 0;
			}
		}
	}			/* while */
	/*---------------------- End of repeating by frame numbers -----------------------------*/
	return (0);
}

/*------------------------------------------------------------------*/
/* put SEI parameter 											    */
/*------------------------------------------------------------------*/
/* return	0:normal end											*/
/*			1:error (out_buffering_period_SEI is OFF)				*/
/*			2:error (out_pic_timing_SEI is OFF)						*/
/*			4:error (out_pan_scan_rect_SEI is OFF)					*/
/*			8:error (out_filler_payload_SEI is OFF)					*/
/*		   16:error (out_recovery_point_SEI is OFF)					*/
/*		   32:vout_dec_ref_pic_marking_repetition_SEI is OFF)		*/
/*------------------------------------------------------------------*/
long output_SEI_parameters(SHCodecs_Encoder * encoder,
			   APPLI_INFO * appli_info,
			   avcbe_stream_info * context,
			   TAVCBE_STREAM_BUFF * sei_stream_buff_info)
{
	long return_code;
	long return_value = 0;

	/* output SEI parameter (step1) */
	if (encoder->other_API_enc_param.out_buffering_period_SEI ==
	    AVCBE_ON) {
		return_code =
		    avcbe_put_SEI_parameters(context,
					     AVCBE_SEI_MESSAGE_BUFFERING_PERIOD,
					     (void *)
					     &(encoder->
					       other_API_enc_param.
					       sei_buffering_period_param),
					     sei_stream_buff_info);
		/* concatenate SEI parameter (return_code means the size of SEI parameter in byte unit) */
		if (return_code > 0) {
			if (encoder->output) {
				encoder->output(encoder, (unsigned char *)
						sei_stream_buff_info->buff_top,
						return_code,
						encoder->output_user_data);
			}
			return_value = 0;
		} else {	/* error */
			return_value |= 0x1;
		}
	}

	/* output SEI parameter (step2) */
	if (encoder->other_API_enc_param.out_pic_timing_SEI == AVCBE_ON) {
		return_code =
		    avcbe_put_SEI_parameters(context,
					     AVCBE_SEI_MESSAGE_PIC_TIMING,
					     (void *)
					     &
					     (encoder->other_API_enc_param.
					      sei_pic_timing_param),
					     sei_stream_buff_info);
		/* concatenate SEI parameter (return_code means the size of SEI parameter in byte unit) */
		if (return_code > 0) {
			if (encoder->output) {
				encoder->output(encoder, (unsigned char *)
						sei_stream_buff_info->buff_top,
						return_code,
						encoder->output_user_data);
			}
			return_value = 0;
		} else {	/* error */
			return_value |= 0x2;
		}
	}
	/* output SEI parameter (step3) */
	if (encoder->other_API_enc_param.out_pan_scan_rect_SEI == AVCBE_ON) {
		return_code =
		    avcbe_put_SEI_parameters(context,
					     AVCBE_SEI_MESSAGE_PAN_SCAN_RECT,
					     (void *)
					     &
					     (encoder->other_API_enc_param.
					      sei_pan_scan_rect_param),
					     sei_stream_buff_info);
		/* concatenate SEI parameter (return_code means the size of SEI parameter in byte unit) */
		if (return_code > 0) {
			if (encoder->output) {
				encoder->output(encoder, (unsigned char *)
						sei_stream_buff_info->buff_top,
						return_code,
						encoder->output_user_data);
			}
			return_value = 0;
		} else {	/* error */
			return_value |= 0x4;
		}
	}
	/* output SEI parameter (step4) */
	if (encoder->other_API_enc_param.out_filler_payload_SEI ==
	    AVCBE_ON) {
		return_code =
		    avcbe_put_SEI_parameters(context,
					     AVCBE_SEI_MESSAGE_FILLER_PAYLOAD,
					     (void *)
					     &(encoder->
					       other_API_enc_param.
					       sei_filler_payload_param),
					     sei_stream_buff_info);
		/* concatenate SEI parameter (return_code means the size of SEI parameter in byte unit) */
		if (return_code > 0) {
			if (encoder->output) {
				encoder->output(encoder, (unsigned char *)
						sei_stream_buff_info->buff_top,
						return_code,
						encoder->output_user_data);
			}
			return_value = 0;
		} else {	/* error */
			return_value |= 0x8;
		}
	}
	/* output SEI parameter (step5) */
	if (encoder->other_API_enc_param.out_recovery_point_SEI ==
	    AVCBE_ON) {
		return_code =
		    avcbe_put_SEI_parameters(context,
					     AVCBE_SEI_MESSAGE_RECOVERY_POINT,
					     (void *)
					     &(encoder->
					       other_API_enc_param.
					       sei_recovery_point_param),
					     sei_stream_buff_info);
		/* concatenate SEI parameter (return_code means the size of SEI parameter in byte unit) */
		if (return_code > 0) {
			if (encoder->output) {
				encoder->output(encoder, (unsigned char *)
						&sei_stream_buff_info->buff_top,
						return_code,
						encoder->output_user_data);
			}
			return_value = 0;
		} else {	/* error */
			return_value |= 0x10;
		}
	}
	return (return_value);
}
