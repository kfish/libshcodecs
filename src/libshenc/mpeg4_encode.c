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
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <setjmp.h>
#include <sys/time.h>

#include "avcbe.h"		/* SuperH MEPG-4&H.264 Video Encode Library Header */
#include "m4iph_vpu4.h"		/* SuperH MEPG-4&H.264 Video Driver Library Header */
#include "encoder_common.h"		/* User Application Sample Header */

#include "avcbe_inner.h"
#include "QuantMatrix.h"

#include "encoder_private.h"

/* #define DEBUG */

#define USE_BVOP

/*** Image-capture-field area ***/

extern long encode_time;

int vpu4_clock_on(void);
int vpu4_clock_off(void);
unsigned long m4iph_sleep_time_get(void);


/*** Stream-output buffer to receive an encoding result in every one frame ***/

/*** Other work-field area ***/
// #ifdef MULTI_STREAM             /* In the case of multiple streams */
// #define MY_MB_WORK_AREA_SIZE       78000   /* QCIF size DataPartioning=ON, Bitrate=64000/256000 : (6500 + 21500) + (9500 + 40000 + more) */
// #else   /* In the case of the number of the streams to encode is one */
// #define MY_MB_WORK_AREA_SIZE       28000   /* QCIF size DataPartioning=ON, Bitrate=64000 : 6500 + 21500 */
// #endif /* MULTI_STREAM */

static int
mpeg4_encode_init_other_options (SHCodecs_Encoder * encoder)
{
	/* This was commented out in the original sample code */
#if 0
	/*** avcbe_other_options_mpeg4 ***/
	encoder->other_options_mpeg4.avcbe_out_vos = AVCBE_ON;
	encoder->other_options_mpeg4.avcbe_out_gov = AVCBE_ON;
	encoder->other_options_mpeg4.avcbe_aspect_ratio_info_type =
	    AVCBE_AUTO;
	encoder->other_options_mpeg4.avcbe_aspect_ratio_info_value = 3;

	encoder->other_options_mpeg4.avcbe_vos_profile_level_type =
	    AVCBE_AUTO;
	encoder->other_options_mpeg4.avcbe_vos_profile_level_value = 1;
	encoder->other_options_mpeg4.avcbe_out_visual_object_identifier =
	    AVCBE_OFF;
	encoder->other_options_mpeg4.avcbe_visual_object_verid = 0;
	encoder->other_options_mpeg4.avcbe_visual_object_priority = 7;
	encoder->other_options_mpeg4.avcbe_video_object_type_indication =
	    0;
	encoder->other_options_mpeg4.avcbe_out_object_layer_identifier =
	    AVCBE_OFF;
	encoder->other_options_mpeg4.avcbe_video_object_layer_verid = 0;
	encoder->other_options_mpeg4.avcbe_video_object_layer_priority = 7;

	/* 'AVCBE_ERM_VIDEO_PACKET' -> 'AVCBE_ERM_NORMAL' changed at Version2 */
	encoder->other_options_mpeg4.avcbe_error_resilience_mode =
	    AVCBE_ERM_NORMAL;
	encoder->other_options_mpeg4.avcbe_video_packet_size_mb = 0;
	encoder->other_options_mpeg4.avcbe_video_packet_size_bit = 0;
	encoder->other_options_mpeg4.avcbe_video_packet_header_extention =
	    AVCBE_OFF;
	/* 'AVCBE_ON' -> 'AVCBE_OFF' changed at Version2 */
	encoder->other_options_mpeg4.avcbe_data_partitioned = AVCBE_OFF;
	encoder->other_options_mpeg4.avcbe_reversible_vlc = AVCBE_OFF;

	encoder->other_options_mpeg4.avcbe_high_quality = AVCBE_HQ_QUALITY;
	encoder->other_options_mpeg4.avcbe_param_changeable = AVCBE_OFF;
	encoder->other_options_mpeg4.avcbe_changeable_max_bitrate = 0;
	encoder->other_options_mpeg4.avcbe_Ivop_quant_initial_value = 16;
	encoder->other_options_mpeg4.avcbe_Pvop_quant_initial_value = 16;
	encoder->other_options_mpeg4.avcbe_use_dquant = AVCBE_ON;
	/* '10' -> '4' changed at Version2 */
	encoder->other_options_mpeg4.avcbe_clip_dquant_frame = 4;
	encoder->other_options_mpeg4.avcbe_quant_min = 6;
	/* added at Version2 */
	encoder->other_options_mpeg4.avcbe_quant_min_Ivop_under_range = 2;
	encoder->other_options_mpeg4.avcbe_quant_max = 26;	/* '31' -> '26' changed at Version2 */

	encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_skipcheck_enable = AVCBE_ON;	/* added at Version2 */
	encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_Ivop_noskip = AVCBE_ON;	/* added at Version2 */
	encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_remain_zero_skip_enable = AVCBE_ON;	/* added at Version2 */

	encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_buffer_unit_size = 16384;	/* added at Version2 */
	encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_buffer_mode = AVCBE_AUTO;	/* added at Version2 */
	encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_max_size = 70;	/* added at Version2 */
	encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_offset = 20;	/* added at Version2 */
	encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_offset_rate = 50;	/* added at Version2 */

	encoder->other_options_mpeg4.avcbe_quant_type =
	    AVCBE_QUANTISATION_TYPE_2;
	encoder->other_options_mpeg4.avcbe_use_AC_prediction = AVCBE_ON;
	encoder->other_options_mpeg4.avcbe_vop_min_mode = AVCBE_MANUAL;	/* added at Version2 */
	encoder->other_options_mpeg4.avcbe_vop_min_size = 0;
	encoder->other_options_mpeg4.avcbe_intra_thr = 6000;
	encoder->other_options_mpeg4.avcbe_b_vop_num = 0;
#endif

	return 0;
}
/*---------------------------------------------------------------------*/

int
mpeg4_encode_init (SHCodecs_Encoder * encoder, long stream_type)
{
	long return_code = 0;
	long i;

	encoder->error_return_function = 0;	/* add at Version2 */
	encoder->error_return_code = 0;	/* add at Version2 */

        mpeg4_encode_init_other_options (encoder);

	/* needs be called only once */
	avcbe_start_encoding();	/* initializes the encoder */

	/*--- The MPEG-4&H.264 Encoder Library API(required-3)@set default
	 *  values for the parameters  ---*/
	/* can be called only once or for the number of streams */
	return_code =
	    avcbe_set_default_param(stream_type, AVCBE_RATE_NORMAL,
				    &(encoder->encoding_property), (void *)
				    &(encoder->other_options_mpeg4));
	if (return_code != 0) {	/* error */
		DisplayMessage
		    (" encode_1file_mpeg4:avcbe_set_default_param ERROR! ",
		     1);
		encoder->error_return_function = -2;
		encoder->error_return_code = return_code;
		return (-2);
	}

        return 0;
}

static int
mpeg4_encode_deferred_init(SHCodecs_Encoder * encoder, long stream_type)
{
	long return_code = 0;
	unsigned long nrefframe, nldecfmem, addr_temp;
	unsigned long *addr_y, *addr_c, *ptr;
	TAVCBE_WORKAREA WORK_ARRY[2];
	long area_width, area_height;
        unsigned long i;

	DisplayMessage(" 1 calling avcbe_init_encode ", 1);

	/*--- The MPEG-4&H.264 Encoder Library API(required-4)@initialize the 
	 * variables ---*/
	WORK_ARRY[0].area_size = MY_WORK_AREA_SIZE;
	WORK_ARRY[0].area_top = (unsigned char *) encoder->my_work_area;
#ifndef VPU4IP
	WORK_ARRY[1].area_size = MY_MB_WORK_AREA_SIZE;
	WORK_ARRY[1].area_top = (unsigned char *) my_mb_work_area;
#else
	WORK_ARRY[1].area_size = 0;
	WORK_ARRY[1].area_top = (unsigned char *) NULL;
#endif				/* VPU4IP */

#ifndef VPU4IP
	return_code = avcbe_init_encode(&(encoder->encoding_property),
					&(encoder->paramR),
					&(encoder->other_options_mpeg4),
					// RSM (avcbe_buf_continue_userproc_ptr)NULL, &WORK_ARRY[0], 
					(avcbe_buf_continue_userproc_ptr) NULL,
					&WORK_ARRY[0], &WORK_ARRY[1],
					&encoder->stream_info);
#else
	return_code = avcbe_init_encode(&(encoder->encoding_property),
					&(encoder->paramR),
					&(encoder->other_options_mpeg4),
					// RSM (avcbe_buf_continue_userproc_ptr)NULL, &WORK_ARRY[0], NULL, 
					(avcbe_buf_continue_userproc_ptr) NULL,
					&WORK_ARRY[0], NULL,
					&encoder->stream_info);
#endif				/* VPU4IP */
	if (return_code < 0) {	/* error */
		if (return_code == -1) {
			DisplayMessage
			    (" encode_1file_mpeg4:avcbe_init_encode PARAMETER ERROR! ",
			     1);
		} else if (return_code == -2) {
			DisplayMessage
			    (" encode_1file_mpeg4:avcbe_init_encode PARAMETER CHANGED! (Maybe Error) ",
			     1);
		} else if (return_code == -3) {
			DisplayMessage
			    (" encode_1file_mpeg4:avcbe_init_encode SEQUENCE ERROR! ",
			     1);
		} else if (return_code == -4) {
			DisplayMessage
			    (" encode_1file_mpeg4:avcbe_init_encode WORK-AREA SIZE SHORT ERROR! ",
			     1);
		}
		encoder->error_return_function = -6;
		encoder->error_return_code = return_code;
		return (-6);
	}

	if (encoder->other_options_mpeg4.avcbe_quant_type == 1) {	/* add @061121 */
		printf("avcbe_set_quant_type1()\n");
		return_code =
		    SetQuantMatrix(encoder->stream_info, QMAT_MPEG_TYPE_ANIME1_INTRA,
				   QMAT_MPEG_TYPE_ANIME1_NONINTRA);
	}

	nrefframe = 1;
	nldecfmem = 2;

	/* Local-decode-image Y */
	encoder->LDEC_ARRY[0].Y_fmemp = (unsigned char *) &encoder->my_frame_memory_ldec1[0];
	encoder->LDEC_ARRY[1].Y_fmemp = (unsigned char *) &encoder->my_frame_memory_ldec2[0];
	encoder->LDEC_ARRY[2].Y_fmemp = (unsigned char *) &encoder->my_frame_memory_ldec3[0];

	/* make it multiples of 16 */
	area_width = ((encoder->width + 15) / 16) * 16;
	area_height = ((encoder->height + 15) / 16) * 16;

	/* Local-decode-image C */
	encoder->LDEC_ARRY[0].C_fmemp =
	    (unsigned char *) (encoder->LDEC_ARRY[0].Y_fmemp +
			       (area_width * area_height));
	encoder->LDEC_ARRY[1].C_fmemp =
	    (unsigned char *) (encoder->LDEC_ARRY[1].Y_fmemp +
			       (area_width * area_height));
	encoder->LDEC_ARRY[2].C_fmemp =
	    (unsigned char *) (encoder->LDEC_ARRY[2].Y_fmemp +
			       (area_width * area_height));

	/* Start address of the image-capture-field area must be arranged 
	 * in 32 bytes alignment. And, this area must be arranged in 
	 * non-cacheable space.*/
	addr_temp = (unsigned long) encoder->my_frame_memory_capt[0];
	addr_y = (unsigned long *) addr_temp;
	addr_c =
	    (unsigned long *) (addr_temp + (area_width * area_height));
	/* Capture-image */
	encoder->CAPTF_ARRY[0].Y_fmemp = (unsigned char *) addr_y;
	encoder->CAPTF_ARRY[0].C_fmemp = (unsigned char *) addr_c;
#ifdef USE_BVOP			/* 050106 */
	if (encoder->other_options_mpeg4.avcbe_b_vop_num > 0) {
		for (i = 0;
		     i < encoder->other_options_mpeg4.avcbe_b_vop_num + 1;
		     i++) {
			addr_temp =
			    (unsigned long) encoder->my_frame_memory_capt[i];
			addr_y = (unsigned long *) addr_temp;
			addr_c =
			    (unsigned long *) (addr_temp +
					       (area_width * area_height));
			encoder->CAPTF_ARRY[i].Y_fmemp = (unsigned char *) addr_y;
			encoder->CAPTF_ARRY[i].C_fmemp = (unsigned char *) addr_c;
			printf("addr_y=%p,addr_c=%p\n", addr_y, addr_c);
		}
		nldecfmem = encoder->other_options_mpeg4.avcbe_b_vop_num;	/* LDEC_ARRY[2]AB-VOP[JfR[ho 050106 */
		printf("b_vop_num=%lu\n", nldecfmem);
//              m4vse_output_local_image_of_b_vop = AVCBE_ON; /* 050302 */
	}
#endif				/* USE_BVOP */
	/*--- The MPEG-4&H.264 Encoder Library API(required-5)@specify the 
	 * address in the image-work-field area ---*/
	return_code =
	    avcbe_init_memory(encoder->stream_info, nrefframe, nldecfmem, encoder->LDEC_ARRY,
			      area_width, area_height);
	printf("avcbe_init_memory=%ld\n", return_code);

	if (return_code != 0) {
		if (return_code == -1) {
			DisplayMessage
			    (" encode_1file_mpeg4:avcbe_init_memory PARAMETER ERROR! ",
			     1);
		} else if (return_code == -3) {
			DisplayMessage
			    (" encode_1file_mpeg4:avcbe_init_memory SEQUENCE ERROR! ",
			     1);
		} else if (return_code == -4) {
			DisplayMessage
			    (" encode_1file_mpeg4:avcbe_init_encode WORK-AREA SIZE SHORT ERROR! ",
			     1);
		}
		encoder->error_return_function = -7;
		encoder->error_return_code = return_code;
		return (-7);
	}

        encoder->initialized = 1;

	return (0);
}

static int
clip_image_data_for_H263(SHCodecs_Encoder * encoder,
			 unsigned long *addr_y, unsigned long *addr_c)
{
	long width, height, index;
	unsigned char *YCbCr_ptr;
	int return_value = 0;
	unsigned long *addr_y_2, *addr_c_2;

	width = encoder->width;
	height = encoder->height;
	printf("clip_image: width=%ld, height=%ld,addr_y=%p,addr_c=%p\n",
	       width, height, addr_y, addr_c);
	addr_y_2 = malloc(width * height);
	addr_c_2 = malloc(width * height / 2);
	m4iph_sdr_read((unsigned char *) addr_y,
		       (unsigned char *) addr_y_2, width * height);
	/* Clip Y-data */
	YCbCr_ptr = (unsigned char *) addr_y_2;
	for (index = 0; index < (width * height); index++) {
		if ((*YCbCr_ptr) < 1) {
			*YCbCr_ptr = 1;
			return_value = 1;	/* clipped */
		} else if ((*YCbCr_ptr) > 254) {
			*YCbCr_ptr = 254;
			return_value = 1;	/* clipped */
		}
		YCbCr_ptr++;
	}
	m4iph_sdr_write((unsigned char *) addr_y,
			(unsigned char *) addr_y_2, width * height);
	/* Clip Cb-data, Cr-data */
	m4iph_sdr_read((unsigned char *) addr_c,
		       (unsigned char *) addr_c_2, width * height / 2);
	YCbCr_ptr = (unsigned char *) addr_c_2;
	for (index = 0; index < ((width * height) / 2); index++) {
		if ((*YCbCr_ptr) < 1) {
			*YCbCr_ptr = 1;
			return_value = 1;	/* clipped */
		} else if ((*YCbCr_ptr) > 254) {
			*YCbCr_ptr = 254;
			return_value = 1;	/* clipped */
		}
		YCbCr_ptr++;
	}
	m4iph_sdr_write((unsigned char *) addr_c,
			(unsigned char *) addr_c_2, width * height / 2);
	free(addr_y_2);
	free(addr_c_2);
	return (return_value);
}

/*------------------------------------------------------------------------*/
/* Encode process function for MPEG-4/H.263                               */
/*------------------------------------------------------------------------*/
static long
mpeg4_encode_picture (SHCodecs_Encoder * encoder,
		      long stream_type)
{
	unsigned long ldec, ref1, ref2;
	long stream_bits, streamsize_per_frame, streamsize_total;
	long return_code;
	long frm;
	char messeage_buf[256];
	unsigned long *addr_y, *addr_c;
	TAVCBE_FMEM captfmem;
	long area_width, area_height;
	avcbe_frame_stat frame_stat;
	TAVCBE_STREAM_BUFF my_stream_buff_info;
	long my_size = 0;
	struct timeval tv, tv1;
	struct timezone tz;
	long tm, ret;
#ifdef USE_BVOP			/* 050106 */
	long index;
	unsigned long *addr_y_tbl[17], *addr_c_tbl[17];
	AVCBE_FRAME_CHECK frame_check_array[17];
#endif				/* USE_BVOP */

	addr_y = (unsigned long *) encoder->CAPTF_ARRY[0].Y_fmemp;
	addr_c = (unsigned long *) encoder->CAPTF_ARRY[0].C_fmemp;

	/* make it multiples of 16 */
	area_width = ((encoder->width + 15) / 16) * 16;
	area_height = ((encoder->height + 15) / 16) * 16;

#ifdef USE_BVOP			/* 050106 */
	for (index = 0;
	     index <
	     (long) (encoder->other_options_mpeg4.avcbe_b_vop_num + 1);
	     index++) {
		addr_y_tbl[index] =
		    (unsigned long *) encoder->CAPTF_ARRY[index].Y_fmemp;
		addr_c_tbl[index] =
		    (unsigned long *) encoder->CAPTF_ARRY[index].C_fmemp;
	}
#endif				/* USE_BVOP */
	/* Index number of the image-work-field area (0 to N-1) 
	 * (for avcbe_set_image_pointer function) */
	ldec = 0;
	ref1 = ref2 = 0;
	frm = 0;		/* Frame number to be encoded (for avcbe_encode_picture function) */

	encoder->frame_counter = 0;
	encoder->frame_skip_num = 0;

	streamsize_total = 0;

	encoder->set_intra = AVCBE_ANY_VOP;	/* Forced intra-mode flag */
	encoder->output_type = AVCBE_OUTPUT_NONE;	/* Header insertion flag */

	captfmem.Y_fmemp = (unsigned char *) encoder->CAPTF_ARRY[0].Y_fmemp;
	captfmem.C_fmemp = (unsigned char *) encoder->CAPTF_ARRY[0].C_fmemp;
	while (1) {	/*---- Repeating by frame numbers -------------------------*/
		printf("while---");
		if (encoder->frame_number_to_encode == encoder->frame_counter) {
			break;
		}
#ifdef USE_BVOP			/* 050106 */
		if (encoder->other_options_mpeg4.avcbe_b_vop_num > 0) {
			return_code =
			    avcbe_get_buffer_check(encoder->stream_info,
						   &frame_check_array[0]);
			if (return_code < 0) {	/* error */
				if (return_code == -1) {
					DisplayMessage
					    (" encode_1file:avcbe_get_buffer_check PARAMETER ERROR! ",
					     1);
				}
				return (-14);
			}
			for (index = 0; index < (long) (encoder->other_options_mpeg4.avcbe_b_vop_num + 1); index++) {	/* 050121 */
//                      for (index=0; index < 3; index++) { /* 050121 */
				if (frame_check_array[index].avcbe_status
				    == AVCBE_UNLOCK) {
					addr_y = addr_y_tbl[index];
					addr_c = addr_c_tbl[index];
					captfmem.Y_fmemp = (unsigned char *) addr_y;	/* 050210 captfmem add */
					captfmem.C_fmemp = (unsigned char *) addr_c;	/* 050210 captfmem add */
					printf("BVOP index=%ld\n", index);
					break;
				}
			}
		}
#endif				/* USE_BVOP */

		if (encoder->input) {
                        encoder->addr_y = (unsigned char *)addr_y;
                        encoder->addr_c = (unsigned char *)addr_c;
			return_code =
			    encoder->input(encoder, encoder->input_user_data);
			if (return_code < 0) {	/* error */
				DisplayMessage
				    (" encode_1file_mpeg4: ERROR acquiring input image! ",
				     1);
				encoder->error_return_function = -8;
				encoder->error_return_code =
				    return_code;
				return (-8);
			}
		}

		/*--- The MPEG-4 Encoder Library API(required-7)@specify the *
		 * address in the capture-image-field area ---*/
		if (stream_type != AVCBE_MPEG4) {
			return_code =
			    clip_image_data_for_H263(encoder, addr_y, addr_c);
			printf("H.263...clip_image_sata=%ld\n",
			       return_code);
		}
		return_code =
		    avcbe_set_image_pointer(encoder->stream_info, &captfmem, ldec, ref1,
					    ref2);
		if (return_code != 0) {
			if (return_code == -1) {
				DisplayMessage
				    (" encode_1file_mpeg4:avcbe_set_image_pointer PARAMETER ERROR! ",
				     1);
			}
			encoder->error_return_function = -9;
			encoder->error_return_code = return_code;
			return (-9);
		}

		/*--- The MPEG-4 Encoder Library API (required-8)@encode each 
		 * screen of display data ---*/
		my_stream_buff_info.buff_top =
		    (unsigned char *) &encoder->my_stream_buff[0];
		my_stream_buff_info.buff_size = MY_STREAM_BUFF_SIZE;	// TODO: buf size
		memset(&encoder->my_stream_buff[0], 0, MY_STREAM_BUFF_SIZE);
#ifdef CAPT_INPUT
		vpu4_clock_on();
		return_code =
		    avcbe_encode_picture(encoder->stream_info, frm, intra_judge,
					 encoder->output_type,
					 &my_stream_buff_info, NULL);
		vpu4_clock_off();
#else
#ifdef DEBUG
		printf
		    ("encode_picture_for_mpeg4: avcbe_encode_picture (%p, %ld, %ld, %ld, {%ld, %p})\n",
		     encoder->stream_info, frm, encoder->set_intra,
		     encoder->output_type,
		     my_stream_buff_info.buff_size,
		     my_stream_buff_info.buff_top);
#endif
		vpu4_clock_on();
		gettimeofday(&tv, &tz);
//printf("enc_pic0=%ld,",tv.tv_usec);
		return_code =
		    avcbe_encode_picture(encoder->stream_info, frm,
					 encoder->set_intra,
					 encoder->output_type,
					 &my_stream_buff_info, NULL);
		gettimeofday(&tv1, &tz);

#ifdef DEBUG
		printf
		    ("encode_picture_for_mpeg4: avcbe_encode_picture returned %ld\n",
		     return_code);
#endif


//printf("ret=%d,enc_pic1=%ld,",return_code,tv1.tv_usec);
		tm = (tv1.tv_usec - tv.tv_usec) / 1000;
		if (tm < 0) {
			tm = 1000 - (tv.tv_usec - tv1.tv_usec) / 1000;
		}
		encode_time += tm;
		printf("Total encode time = %ldmsec(%lumsec),", tm,
		       encode_time_get());
		printf("Total sleep  time = %ld(msec)\n",
		       m4iph_sleep_time_get());
		vpu4_clock_off();
#endif				/* CAPT_INPUT */

		if (return_code < 0) {	/* error */
			if (return_code == AVCBE_ENCODE_ERROR) {	/* -1 */
				DisplayMessage
				    (" encode_1file_mpeg4:avcbe_encode_picture PARAMETER ERROR! ",
				     1);

			} else if (return_code == AVCBE_NOT_IN_ORDER_ERROR) {	/* -3 */
				DisplayMessage
				    (" encode_1file_mpeg4:avcbe_encode_picture SEQUENCE ERROR! ",
				     1);

			} else if (return_code == AVCBE_OUTPUT_BUFFER_SHORT_ERROR) {	/* -4 */
				DisplayMessage
				    (" encode_1file_mpeg4:avcbe_encode_picture OUTPUT BUFFER SIZE SHORT ERROR! ",
				     1);

			} else if (return_code == AVCBE_VPU_ERROR_AFTER_ENCODING) {	/* -6 */
				DisplayMessage
				    (" encode_1file_mpeg4:avcbe_encode_picture VPU4 ERROR! ",
				     1);

			} else if (return_code == AVCBE_TEMPORARY_BUFFER_SHORT_ERROR) {	/* -8 */
				DisplayMessage
				    (" encode_1file_mpeg4:avcbe_encode_picture TEMPORARY_BUFFER SIZE SHORT ERROR! ",
				     1);

			} else if (return_code == AVCBE_WORK_AREA_SHORT_ERROR) {	/* -99 */
				DisplayMessage
				    (" encode_1file_mpeg4:avcbe_encode_picture OTHER WORK SIZE SHORT ERROR! ",
				     1);

			} else if (return_code == AVCBE_QUANT_MATRIX_NOT_SPECIFIED_ERROR) {	/* -11 */
				DisplayMessage
				    (" encode_1file_mpeg4:avcbe_encode_picture QUANT_MATRIX_NOT_SPECIFIED ERROR! ",
				     1);
			}
			encoder->error_return_function = -11;
			encoder->error_return_code = return_code;
			return (-11);
		} else if (return_code == AVCBE_ENCODE_SUCCESS) {	/* 0 */
			sprintf(messeage_buf,
				" encode_1file_mpeg4:avcbe_encode_picture SUCCESS  frm=%ld,seq_no=%ld ",
				frm, encoder->frame_counter);
			DisplayMessage(messeage_buf, 1);
			/* the second parameter 'ldec' value must be changed when *
			 * the avcbe_set_image_pointer function is called on next time. */
		} else if (return_code == AVCBE_FRAME_SKIPPED) {	/* 1 */
			sprintf(messeage_buf,
				" encode_1file_mpeg4:avcbe_encode_picture THIS FRAME SKIPPED(Not Encoded)  frm=%ld,seq_no=%ld ",
				frm, encoder->frame_counter);
			DisplayMessage(messeage_buf, 1);
			/* the second parameter 'ldec' value must NOT be changed *
			 * when the avcbe_set_image_pointer function is called on next time. */
			encoder->frame_skip_num++;
		} else if (return_code == AVCBE_EMPTY_VOP_OUTPUTTED) {	/* 2 */
			sprintf(messeage_buf,
				" encode_1file_mpeg4:avcbe_encode_picture EMPTY VOP CREATED  frm=%ld,seq_no=%ld ",
				frm, encoder->frame_counter);
			DisplayMessage(messeage_buf, 1);
			/* the second parameter 'ldec' value must NOT be changed *
			 * when the avcbe_set_image_pointer function is called on next time. */

		} else if (return_code == AVCBE_B_VOP_OUTPUTTED) {	/* 4 */
			sprintf(messeage_buf,
				" encode_1file_mpeg4:avcbe_encode_picture B-VOP OUTPUTTED  frm=%ld,seq_no=%ld ",
				frm, encoder->frame_counter);
			DisplayMessage(messeage_buf, 1);
			/* the second parameter 'ldec' value is ignored when B-VOP is used */
		} else {
			sprintf(messeage_buf,
				" encode_1file_mpeg4:avcbe_encode_picture UNKNOWN RETURN CODE=%ld  frm=%ld,seq_no=%ld ",
				return_code, frm,
				encoder->frame_counter);
			DisplayMessage(messeage_buf, 1);
		}
		avcbe_get_last_frame_stat(encoder->stream_info, &frame_stat);
		if ((return_code == AVCBE_ENCODE_SUCCESS)
		    || (return_code == AVCBE_B_VOP_OUTPUTTED)
		    || (return_code == AVCBE_EMPTY_VOP_OUTPUTTED)) {
			if (return_code == AVCBE_ENCODE_SUCCESS) {	/* add at Version2 */
				if (ldec == 0) {
					ldec = 1;
					ref1 = 0;
				} else {
					ldec = 0;
					ref1 = 1;
				}
			}

			encoder->frm = frm;
			stream_bits = frame_stat.avcbe_frame_n_bits;
			/* get the size of bitstream (byte unit) */
			streamsize_per_frame = (stream_bits / 8);

			if (encoder->output) {
				my_size =
				    encoder->output(encoder,
						    (unsigned char *)
						    &encoder->my_stream_buff[0],
						    streamsize_per_frame,
						    encoder->
						    output_user_data);
			}
		}
		frm += encoder->frame_no_increment;
		encoder->frame_counter++;
	}

	return (0);
}

int
mpeg4_encode_run (SHCodecs_Encoder * encoder, long stream_type)
{
	long return_code;
	TAVCBE_STREAM_BUFF my_end_code_buff_info;
	long my_size = 0;

        if (!encoder->initialized)
                mpeg4_encode_deferred_init (encoder, stream_type);

	encoder->error_return_function = 0;	/* add at Version2 */
	encoder->error_return_code = 0;	/* add at Version2 */

	if (stream_type == AVCBE_MPEG4) {
		DisplayMessage("MPEG-4 Encode Start! ", 1);
	} else {
		DisplayMessage("H.263 Encode Start! ", 1);
	}

	/*--- The MPEG-4 Encoder Library API(required-2)@start encoding ---*/

	/* encode process function for mpeg-4/H.263 (call avcbe_encode_picture func.) */
	return_code =
	    mpeg4_encode_picture (encoder, stream_type);
	if (return_code != 0) {
		return (-15);
	}

	/*--- The MPEG-4&H.264 Encoder Library API (required-9)@ends encoding ---*/
	my_end_code_buff_info.buff_top =
	    (unsigned char *) &encoder->my_end_code_buff[0];
	my_end_code_buff_info.buff_size = MY_END_CODE_BUFF_SIZE;

	/* return value is byte unit */
	return_code =
	    avcbe_put_end_code(encoder->stream_info, &my_end_code_buff_info,
			       AVCBE_VOSE);
	if (return_code <= 0) {
		if (return_code == -4) {
			DisplayMessage
			    (" encode_1file_mpeg4:avcbe_close_encode OUTPUT BUFFER SIZE SHORT ERROR! ",
			     1);
		}
		encoder->error_return_function = -16;
		encoder->error_return_code = return_code;

		return (-16);
	} else {
		if (encoder->output) {
			my_size =
			    encoder->output(encoder,
					    (unsigned char *)
					    &encoder->my_end_code_buff[0],
					    return_code,
					    encoder->output_user_data);
		}
	}

	return (0);
}

