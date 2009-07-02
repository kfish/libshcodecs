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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <setjmp.h>
#include <sys/time.h>

#include "avcbe.h"		/* SuperH MEPG-4&H.264 Video Encode Library Header */
#include "m4iph_vpu4.h"		/* SuperH MEPG-4&H.264 Video Driver Library Header */
#include "encoder_common.h"		/* User Application Sample Header */

#include "encoder_private.h"

#define OUTPUT_ERROR_MSGS
#define OUTPUT_INFO_MSGS
//#define OUTPUT_STEAM_INFO

#define NUM_LDEC_FRAMES 3

extern long encode_time;

unsigned long m4iph_sleep_time_get(void);


#ifdef OUTPUT_ERROR_MSGS
/* Error messages */
const char *vpu_err_txt[] = {
	"",
	"Parameter error",
	"Parameter changed (maybe an error)",
	"Function call made out of order",
	"Output buffer too small",
	"",
	"VPU4 error",
	"",
	"Temporary buffer too small",
	"VUI parameter not specified error",
	"",
	"Quant matrix not specified",
};

static long
vpu_err_msg(SHCodecs_Encoder *encoder, const char *func, int line, long rc)
{
	long index = -rc;
	if (index > 0 && index < 11) {
		fprintf (stderr, " %s, line %d: returned %ld (%s)\n",
				 func, line, rc, vpu_err_txt[index]);
	} else if (rc == AVCBE_WORK_AREA_SHORT_ERROR) {	/* -99 */
		fprintf (stderr, " %s: OTHER WORK SIZE SHORT ERROR!\n", func);
	}
	encoder->error_return_code = rc;
	return rc;
}
#else
#define vpu_err_msg(encoder, func, line, rc) (rc)
#endif


#ifdef OUTPUT_INFO_MSGS
/* Information messages */
const char *vpu_info_txt[] = {
	"Sucess",
	"Frame skipped",
	"Empty VOP has been output",
	"",
	"B-VOP output",
	"Slice remain, picture not finished",
	"Output Sequence Parameter Set",
	"Output Picture Parameter Set",
};

static void
vpu_info_msg(SHCodecs_Encoder *encoder, const char *func, int line, long frm, long rc)
{
	if (rc > 0 && rc < 8) {
		fprintf (stderr, " %s, line %d: returned %ld (%s) frm=%d,seq_no=%d\n",
				func, line, rc, vpu_info_txt[rc],
				(int) frm,
				(int) encoder->frame_counter);
	}
} 
#else
#define vpu_info_msg(encoder, func, line, rc)
#endif


enum {SEI, SPS, PPS, AUD, DATA, FILL, END};
#ifdef OUTPUT_STEAM_INFO
static const char*data_name[] = {"SEI", "SPS", "PPS", "AUD", "DATA", "FILL", "END"};
#endif

static void
output_data(SHCodecs_Encoder *encoder, int type, void *buf, long length)
{
#ifdef OUTPUT_STEAM_INFO
	fprintf (stderr, "output %s (%d bytes)\n", data_name[type], length);
#endif
	if (encoder->output) {
		encoder->output(encoder,
				(unsigned char *)buf, length,
				encoder->output_user_data);
	}
}


int
h264_encode_init (SHCodecs_Encoder *encoder, long stream_type)
{
	long return_code = 0;

	encoder->error_return_code = 0;
	encoder->slice_total_size = 0;
	encoder->tmp_pic_total_bytes = 0;

	/* The MPEG-4 Encoder Library API(required-2) Start encoding */
	/* needs be called only once */
	avcbe_start_encoding();

	/* The MPEG-4&H.264 Encoder Library API(required-3)
	   Set default values for the parameters */
	/* can be called only once or for the number of streams */
	return_code =
	    avcbe_set_default_param(stream_type, AVCBE_RATE_NORMAL,
				    &(encoder->encoding_property),
				    (void *)&(encoder->other_options_h264));
	if (return_code != 0) {
		return vpu_err_msg(encoder, __func__, __LINE__, return_code);
	}

	return 0;
}

static int
h264_encode_deferred_init(SHCodecs_Encoder *encoder, long stream_type)
{
	long return_code = 0;
	unsigned long nldecfmem;
	unsigned char *addr_y, *addr_c;
	TAVCBE_WORKAREA WORK_ARRY[2];
	long area_width, area_height;
	int i;
	avcbe_other_options_h264 *options;

	/* The MPEG-4&H.264 Encoder Library API(required-4)
	   Initialize the variables */
	WORK_ARRY[0].area_size = MY_WORK_AREA_SIZE;
	WORK_ARRY[0].area_top = (unsigned char *) encoder->my_work_area;

#ifndef VPU4IP
	WORK_ARRY[1].area_size = MY_MB_WORK_AREA_SIZE;
	WORK_ARRY[1].area_top = (unsigned char *) my_mb_work_area;
#else
	WORK_ARRY[1].area_size = 0;
	WORK_ARRY[1].area_top = (unsigned char *) NULL;
#endif /* VPU4IP */

	options = &encoder->other_options_h264;

	if (options->avcbe_ratecontrol_cpb_buffer_mode == AVCBE_MANUAL) {
		options->avcbe_ratecontrol_cpb_offset = (unsigned long)
		    avcbe_calc_cpb_buff_offset(encoder->encoding_property.avcbe_bitrate,
		     	(options->avcbe_ratecontrol_cpb_max_size *
				 options->avcbe_ratecontrol_cpb_buffer_unit_size),
				90);
	}
	return_code =
	    avcbe_init_encode(&(encoder->encoding_property),
				&(encoder->paramR),
				options,
				(avcbe_buf_continue_userproc_ptr) NULL,
				&WORK_ARRY[0], &WORK_ARRY[1], &encoder->stream_info);
	if (return_code < 0) {
		return vpu_err_msg(encoder, __func__, __LINE__, return_code);
	}
	nldecfmem = 2;

	/* make size multiples of 16 */
	area_width = ((encoder->width + 15) / 16) * 16;
	area_height = ((encoder->height + 15) / 16) * 16;

	/* Local decode images */
	encoder->LDEC_ARRY[0].Y_fmemp = (unsigned char *) &encoder->my_frame_memory_ldec1[0];
	encoder->LDEC_ARRY[1].Y_fmemp = (unsigned char *) &encoder->my_frame_memory_ldec2[0];
	encoder->LDEC_ARRY[2].Y_fmemp = (unsigned char *) &encoder->my_frame_memory_ldec3[0];
	for (i=0; i<NUM_LDEC_FRAMES; i++) {
		encoder->LDEC_ARRY[i].C_fmemp = encoder->LDEC_ARRY[i].Y_fmemp + (area_width * area_height);
	}

	/* The MPEG-4&H.264 Encoder Library API(required-5)
	   Specify the address in the image-work-field area */
	return_code =
	    avcbe_init_memory(encoder->stream_info,
				encoder->ref_frame_num,
				nldecfmem, encoder->LDEC_ARRY,
				area_width, area_height);
	if (return_code != 0) {
		return vpu_err_msg(encoder, __func__, __LINE__, return_code);
	}

	/* Start address of the image-capture-field area must be arranged in 32 bytes alignment. */
	/* And, this area must be arranged in non-cacheable space. */
	addr_y = (unsigned char *)encoder->my_frame_memory_capt[0];
	addr_c = addr_y + (area_width * area_height);

	/* Capture-image */
	encoder->CAPTF_ARRY[0].Y_fmemp = addr_y;
	encoder->CAPTF_ARRY[0].C_fmemp = addr_c;

	encoder->initialized = 1;

	return (0);
}

/* returns 0 on success */
static long
h264_output_SEI_parameters(SHCodecs_Encoder *encoder)
{
	OTHER_API_ENC_PARAM *params = &encoder->other_API_enc_param;
	long return_value = 0;
	long length;
	int num_sei_msgs;
	int i;

	char sei_msg[] = {
		params->out_buffering_period_SEI,
		params->out_pic_timing_SEI,
		params->out_pan_scan_rect_SEI,
		params->out_filler_payload_SEI,
		params->out_recovery_point_SEI,
	};
	long sei_arg1[] = {
		AVCBE_SEI_MESSAGE_BUFFERING_PERIOD,
		AVCBE_SEI_MESSAGE_PIC_TIMING,
		AVCBE_SEI_MESSAGE_PAN_SCAN_RECT,
		AVCBE_SEI_MESSAGE_FILLER_PAYLOAD,
		AVCBE_SEI_MESSAGE_RECOVERY_POINT,
	};
	void *sei_arg2[] = {
		(void *)&(params->sei_buffering_period_param),
		(void *)&(params->sei_pic_timing_param),
		(void *)&(params->sei_pan_scan_rect_param),
		(void *)&(params->sei_filler_payload_param),
		(void *)&(params->sei_recovery_point_param),
	};

	num_sei_msgs = sizeof(sei_msg);

	/* output SEI parameters */
	for (i=0; i<num_sei_msgs; i++)
	{
		if (sei_msg[i] == AVCBE_ON) {
			length = avcbe_put_SEI_parameters(
						encoder->stream_info,
						sei_arg1[i],
						sei_arg2[i],
						&(encoder->sei_buf_info));

			if (length > 0) {
				output_data(encoder, SEI, encoder->sei_buf_info.buff_top, length);
			} else {
				vpu_err_msg(encoder, __func__, __LINE__, length);
				return_value |= (0x1 << i);
			}
		}
	}

	return (return_value);
}

static long
setup_veu_params(SHCodecs_Encoder *encoder)
{
	avcbe_vui_main_param *vui_param;
	long return_code, length;

	vui_param = &encoder->other_API_enc_param.vui_main_param;

	/* get the size of CPB-buffer to set 'cpb_size_scale' of HRD */
	length = avcbe_get_cpb_buffer_size(encoder->stream_info);
	if (length <= 0) {
		return vpu_err_msg(encoder, __func__, __LINE__, length);
	}

	if (vui_param->avcbe_nal_hrd_parameters_present_flag == AVCBE_ON) {
		vui_param->avcbe_nal_hrd_param.avcbe_cpb_size_scale = length;
	} else if (vui_param->avcbe_vcl_hrd_parameters_present_flag == AVCBE_ON) {
		vui_param->avcbe_vcl_hrd_param.avcbe_cpb_size_scale = length;
	}

	return_code = avcbe_set_VUI_parameters(encoder->stream_info, vui_param);
	if (return_code != 0) {
		return vpu_err_msg(encoder, __func__, __LINE__, return_code);
	}
	return 0;
}

/* Get SPS & PPS data, but do not output it */
static long
h264_encode_sps_pps(SHCodecs_Encoder *encoder, avcbe_slice_stat *slice_stat, long frm)
{
	long return_code;

	/* SPS data */
	return_code = avcbe_encode_picture(
					encoder->stream_info, frm,
					AVCBE_ANY_VOP,
					AVCBE_OUTPUT_SPS,
					&encoder->sps_buf_info,
					NULL);
	if (return_code != AVCBE_SPS_OUTPUTTED) {
		return vpu_err_msg(encoder, __func__, __LINE__, return_code);
	}

	/* get the size */
	avcbe_get_last_slice_stat(encoder->stream_info, slice_stat);


	/* PPS data */
	return_code = avcbe_encode_picture(
					encoder->stream_info, frm,
					AVCBE_ANY_VOP,
					AVCBE_OUTPUT_PPS,
					&encoder->pps_buf_info,
					NULL);
	if (return_code != AVCBE_PPS_OUTPUTTED) {
		return vpu_err_msg(encoder, __func__, __LINE__, return_code);
	}

	/* get the size */
	avcbe_get_last_slice_stat(encoder->stream_info, slice_stat);

	return 0;
}

/*----------------------------------------------------------------------*/
/* Encode by 1 picture unit without/with using slice division for H.264 */
/*----------------------------------------------------------------------*/
static long
h264_encode_picture_unit(SHCodecs_Encoder *encoder, long stream_type)
{
	unsigned long ldec, ref1, ref2;
	long streamsize_per_frame;
	long return_code;
	long frm;
	unsigned long *addr_y, *addr_c;
	long i;
	avcbe_slice_stat slice_stat;
	TAVCBE_STREAM_BUFF *extra_stream_buff;
	struct timeval tv, tv1;
	long tm;

	addr_y = (unsigned long *) encoder->CAPTF_ARRY[0].Y_fmemp;
	addr_c = (unsigned long *) encoder->CAPTF_ARRY[0].C_fmemp;

	/* Setup VUI parameters */
	if (encoder->other_options_h264.avcbe_out_vui_parameters == AVCBE_ON) {
		return_code = setup_veu_params(encoder);
		if (return_code != 0) {
			return (-113);
		}
	}

	ldec = 0;		/* Index number of the image-work-field area (0 to N-1) (for avcbe_set_image */
	ref1 = ref2 = 0;
	frm = 0;		/* Frame number to be encoded (for avcbe_encode_picture function) */
	encoder->frame_counter = 0;
	encoder->frame_skip_num = 0;
	encoder->slice_mb_counter = 0;
	encoder->mb_num_of_picture = 0;

	/* stream output buffer */
	encoder->my_stream_buff_info.buff_top = (unsigned char *) &encoder->my_stream_buff[0];
	encoder->my_stream_buff_info.buff_size = MY_STREAM_BUFF_SIZE;

	/* Access Unit Delimiter (AUD) output buffer */
	encoder->aud_buf_info.buff_top = (unsigned char *) &encoder->my_extra_stream_buff[0];
	encoder->aud_buf_info.buff_size = 16;
	extra_stream_buff = &encoder->aud_buf_info;

	/* SPS output buffer */
	encoder->sps_buf_info.buff_top = ALIGN(&encoder->my_sps_stream_buff[0], 32);
	encoder->sps_buf_info.buff_size = MY_SPS_STREAM_BUFF_SIZE;

	/* PPS output buffer */
	encoder->pps_buf_info.buff_top = ALIGN(&encoder->my_pps_stream_buff[0], 32);
	encoder->pps_buf_info.buff_size = MY_PPS_STREAM_BUFF_SIZE;

	/* SEI output buffer */
	encoder->sei_buf_info.buff_top = (unsigned char *) &encoder->my_sei_stream_buff[0];
	encoder->sei_buf_info.buff_size = MY_SEI_STREAM_BUFF_SIZE;

	/* Filler Data output buffer */
	encoder->my_filler_data_buff_info.buff_top = (unsigned char *) &encoder->my_filler_data_buff[0];
	encoder->my_filler_data_buff_info.buff_size = MY_FILLER_DATA_BUFF_SIZE;


	/*--------------------- Repeating by frame numbers --------------------------*/
	while (encoder->frame_counter < encoder->frame_number_to_encode) {

		/* Reset the amount of filler data used */	
		encoder->output_filler_data = 0;

		/* Encode SPS and PPS for 1st frame */
		if (encoder->frame_counter == 0) {
			return_code = h264_encode_sps_pps(encoder, &slice_stat, frm);
			if (return_code != 0) {
				return return_code;
			}
		}

		/* output SEI data (if AU delimiter is NOT used) */
		if (extra_stream_buff == NULL) {
			h264_output_SEI_parameters(encoder);
		}

		/* Get the encoder input frame */
		if (encoder->input) {
			encoder->addr_y = (unsigned char *)addr_y;
			encoder->addr_c = (unsigned char *)addr_c;
			return_code = encoder->input(encoder, encoder->input_user_data);
			if (return_code < 0) {
				fprintf (stderr, " %s: ERROR acquiring input image!\n", __func__);
				encoder->error_return_code = return_code;
				return (-108);
			}
		}

		/* The MPEG-4 Encoder Library API(required-7)
		   Specify the input frame address */
		return_code = avcbe_set_image_pointer(encoder->stream_info,
						&encoder->CAPTF_ARRY[0], ldec, ref1, ref2);
		if (return_code != 0) {
			return vpu_err_msg(encoder, __func__, __LINE__, return_code);
		}

		gettimeofday(&tv, NULL);

		/* The MPEG-4 Encoder Library API (required-8)
		   Encode each frame */
		return_code =
		    avcbe_encode_picture(encoder->stream_info, frm,
					 AVCBE_ANY_VOP,
					 AVCBE_OUTPUT_SLICE,
					 &encoder->my_stream_buff_info,
					 extra_stream_buff);
		gettimeofday(&tv1, NULL);
		tm = (tv1.tv_usec - tv.tv_usec) / 1000;
		if (tm < 0) {
			tm = 1000 - (tv.tv_usec - tv1.tv_usec) / 1000;
		}
		encode_time += tm;

		if (return_code < 0) {
			return vpu_err_msg(encoder, __func__, __LINE__, return_code);
		}
		vpu_info_msg(encoder, __func__, __LINE__, frm, return_code);

		/* get the information about the just encoded frame (slice) */
		avcbe_get_last_slice_stat(encoder->stream_info, &slice_stat);
		encoder->slice_mb_counter = slice_stat.avcbe_encoded_MB_num;
		encoder->mb_num_of_picture = slice_stat.avcbe_total_MB_in_frame;

		if (return_code == AVCBE_FRAME_SKIPPED) {
			encoder->frame_skip_num++;
		}
		else if (return_code == AVCBE_ENCODE_SUCCESS) {
			/* 1 picture unit */
			if (ldec == 0) {
				ldec = 1;
				ref1 = 0;
			} else {
				ldec = 0;
				ref1 = 1;
			}

			/* output AU delimiter */
			if (extra_stream_buff != NULL) {
				output_data(encoder, AUD, &encoder->my_extra_stream_buff[0],
				            slice_stat.avcbe_AU_unit_bytes);
			}

			/* If the type is IDR-picture, encode SPS and PPS data (for 2nd frame and later) */
			if ((encoder->frame_counter > 0)
			    && (slice_stat.avcbe_encoded_pic_type == AVCBE_IDR_PIC)) {
				return_code = h264_encode_sps_pps(encoder, &slice_stat, frm);
				if (return_code != 0) {
					return return_code;
				}
			}
			/* output SPS data and PPS data */
			output_data(encoder, SPS, encoder->sps_buf_info.buff_top,
			            slice_stat.avcbe_SPS_unit_bytes);
			output_data(encoder, PPS, encoder->pps_buf_info.buff_top,
			            slice_stat.avcbe_PPS_unit_bytes);

			/* output SEI parameter */
			h264_output_SEI_parameters(encoder);

			/* output Filler data (if CPB Buffer overflow) */
			if ((encoder->output_filler_enable == 1)
			    && (encoder->output_filler_data > 0)) {
				output_data(encoder, FILL, &encoder->my_filler_data_buff[0], encoder->output_filler_data);
			}

			/* Output slice data */
			streamsize_per_frame = ((slice_stat.avcbe_encoded_slice_bits + 7) / 8);
			output_data(encoder, DATA, &encoder->my_stream_buff[0], streamsize_per_frame);

		} /* if (return_code == AVCBE_ENCODE_SUCCESS) */

		frm += encoder->frame_no_increment;
		encoder->frame_counter++;

	} /* while */
	/*---------------------- End of repeating by frame numbers -----------------------------*/

	return (0);
}

/*------------------------------*/
/* Encode by NAL unit for H.264 */
/*------------------------------*/
static long
h264_encode_nal_unit(SHCodecs_Encoder *encoder, long stream_type)
{
	unsigned long ldec, ref1, ref2;
	long tmp_slice_size;
	long return_code;
	long frm;
	unsigned long *addr_y, *addr_c;
	avcbe_slice_stat slice_stat;
	TAVCBE_STREAM_BUFF *extra_stream_buff;
	struct timeval tv, tv1;
	long tm;

	addr_y = (unsigned long *) encoder->CAPTF_ARRY[0].Y_fmemp;
	addr_c = (unsigned long *) encoder->CAPTF_ARRY[0].C_fmemp;

	/* Setup VUI parameters */
	if (encoder->other_options_h264.avcbe_out_vui_parameters == AVCBE_ON) {
		return_code = setup_veu_params(encoder);
		if (return_code != 0) {
			return (-113);
		}
	}

	ldec = 0;		/* Index number of the image-work-field area (0 to N-1) (for avcbe_set_image_pointer function) */
	ref1 = ref2 = 0;
	frm = 0;		/* Frame number to be encoded (for avcbe_encode_picture function) */

	encoder->frame_counter = 0;
	encoder->frame_skip_num = 0;
	encoder->slice_mb_counter = 0;
	encoder->mb_num_of_picture = 0;

	/* stream output buffer */
	encoder->my_stream_buff_info.buff_top = (unsigned char *) &encoder->my_stream_buff[0];
	encoder->my_stream_buff_info.buff_size = MY_STREAM_BUFF_SIZE;

	/* Access Unit Delimiter (AUD) output buffer */
	encoder->aud_buf_info.buff_top = (unsigned char *) &encoder->my_extra_stream_buff[0];
	encoder->aud_buf_info.buff_size = 16;
	extra_stream_buff = &encoder->aud_buf_info;

	/* SPS output buffer */
	encoder->sps_buf_info.buff_top = ALIGN(&encoder->my_sps_stream_buff[0], 32);
	encoder->sps_buf_info.buff_size = MY_SPS_STREAM_BUFF_SIZE;

	/* PPS output buffer */
	encoder->pps_buf_info.buff_top = ALIGN(&encoder->my_pps_stream_buff[0], 32);
	encoder->pps_buf_info.buff_size = MY_PPS_STREAM_BUFF_SIZE;

	/* SEI output buffer */
	encoder->sei_buf_info.buff_top = (unsigned char *) &encoder->my_sei_stream_buff[0];
	encoder->sei_buf_info.buff_size = MY_SEI_STREAM_BUFF_SIZE;

	/* Filler Data output buffer */
	encoder->my_filler_data_buff_info.buff_top = (unsigned char *) &encoder->my_filler_data_buff[0];
	encoder->my_filler_data_buff_info.buff_size = MY_FILLER_DATA_BUFF_SIZE;

	encoder->dummy_nal_buf_addr = encoder->dummy_nal_buf;

	/*--------------------- Repeating by frame numbers --------------------------*/
	while (encoder->frame_counter < encoder->frame_number_to_encode) {

		/* Reset the amount of filler data used */	
		encoder->output_filler_data = 0;

		/* Encode SPS and PPS for 1st frame */
		if (encoder->frame_counter == 0) {
			return_code = h264_encode_sps_pps(encoder, &slice_stat, frm);
			if (return_code != 0) {
				return return_code;
			}
		}

		/* output SEI data (if AU delimiter is NOT used) */
		if (extra_stream_buff == NULL) {
		    h264_output_SEI_parameters(encoder);
		}

		/* Start of a new frame? */
		if (encoder->slice_mb_counter == 0) {

			/* Get the encoder input frame */
			if (encoder->input) {
				encoder->addr_y = (unsigned char *)addr_y;
				encoder->addr_c = (unsigned char *)addr_c;
				return_code = encoder->input(encoder, encoder->input_user_data);
				if (return_code < 0) {
					fprintf (stderr, " %s: ERROR acquiring input image!\n", __func__);
					encoder->error_return_code = return_code;
					return (-108);
				}
			}

			/* Tell the middleware about the new input buffer */
			return_code = avcbe_set_image_pointer(encoder->stream_info,
						    &encoder->CAPTF_ARRY[0], ldec, ref1, ref2);
			if (return_code != 0) {
				return vpu_err_msg(encoder, __func__, __LINE__, return_code);
			}
		}

		gettimeofday(&tv, NULL);

		/* output slice header and slice data */
		return_code =
		    avcbe_encode_picture(encoder->stream_info, frm,
					 AVCBE_ANY_VOP,
					 AVCBE_OUTPUT_SLICE,
					 &encoder->my_stream_buff_info,
					 extra_stream_buff);
		gettimeofday(&tv1, NULL);
		tm = (tv1.tv_usec - tv.tv_usec) / 1000;
		if (tm < 0) {
			tm = 1000 - (tv.tv_usec - tv1.tv_usec) / 1000;
		}
		encode_time += tm;

		if (return_code < 0) {
			return vpu_err_msg(encoder, __func__, __LINE__, return_code);
		}
		vpu_info_msg(encoder, __func__, __LINE__, frm, return_code);

		/* get the information about the just encoded slice */
		avcbe_get_last_slice_stat(encoder->stream_info, &slice_stat);
		encoder->slice_mb_counter = slice_stat.avcbe_encoded_MB_num;
		encoder->mb_num_of_picture = slice_stat.avcbe_total_MB_in_frame;

		/* get the size in bytes of the encoded slice */
		tmp_slice_size = (slice_stat.avcbe_encoded_slice_bits + 7) / 8;

		if (return_code == AVCBE_SLICE_REMAIN) {

			memcpy(encoder->dummy_nal_buf_addr, (char *) &encoder->my_stream_buff[0], tmp_slice_size);
			encoder->slice_total_size += tmp_slice_size;
			encoder->tmp_pic_total_bytes += tmp_slice_size;	/* total size of 1-frame */
			encoder->dummy_nal_buf_addr += tmp_slice_size;	/* update the address of buffer for 1-slice */

		} else if (return_code == AVCBE_FRAME_SKIPPED) {

			/* if the frame is skipped, the size of slice data which are already put, must be subtracted */
			encoder->slice_total_size -= encoder->tmp_pic_total_bytes;

			/* make the address which dummy_nal_buf_addr has, to the top address of dummy_nal_buf[] */
			encoder->dummy_nal_buf_addr = encoder->dummy_nal_buf;
			encoder->tmp_pic_total_bytes = 0;

			frm += encoder->frame_no_increment;
			encoder->frame_counter++;
			encoder->slice_mb_counter = 0;

			encoder->frame_skip_num++;

		} else if (return_code == AVCBE_ENCODE_SUCCESS) {	

			/* end of frame? */
			if (encoder->slice_mb_counter == encoder->mb_num_of_picture) {
				/* Switch the indexes to the reference and locally decoded frames */
				if (ldec == 0) {
					ldec = 1;
					ref1 = 0;
				} else {
					ldec = 0;
					ref1 = 1;
				}
			}

			/* output AU delimiter */
			if (extra_stream_buff != NULL) {
				output_data(encoder, AUD, &encoder->my_extra_stream_buff[0], slice_stat.avcbe_AU_unit_bytes);
			}

			/* If the type is IDR-picture, encode SPS and PPS data (for 2nd frame and later) */
			if ((encoder->frame_counter > 0)
			    && (slice_stat.avcbe_encoded_pic_type == AVCBE_IDR_PIC)) {
				return_code = h264_encode_sps_pps(encoder, &slice_stat, frm);
				if (return_code != 0) {
					return return_code;
				}
			}
			/* output SPS data and PPS data */
			output_data(encoder, SPS, encoder->sps_buf_info.buff_top, slice_stat.avcbe_SPS_unit_bytes);
			output_data(encoder, PPS, encoder->pps_buf_info.buff_top, slice_stat.avcbe_PPS_unit_bytes);

			/* output SEI parameter */
			h264_output_SEI_parameters(encoder);

			/* output Filler data (if CPB Buffer overflow) */
			if ((encoder->output_filler_enable == 1)
			    && (encoder->output_filler_data > 0)) {
				output_data(encoder, FILL, &encoder->my_filler_data_buff[0], encoder->output_filler_data);
			}

			/* copy the bitstream of 1-slice newly encoded, and add the size to the total size of 1-frame */
			memcpy(encoder->dummy_nal_buf_addr, (char *) &encoder->my_stream_buff[0], tmp_slice_size);
			encoder->slice_total_size += tmp_slice_size;	/* the size of 1-slice newly encoded */
			encoder->tmp_pic_total_bytes += tmp_slice_size;	/* total size of 1-frame */
			encoder->dummy_nal_buf_addr += tmp_slice_size;	/* update the address of buffer for 1-slice */

			/* Output slice data */
			output_data(encoder, DATA, encoder->dummy_nal_buf, encoder->tmp_pic_total_bytes);

			/* clear the size of 1-frame */
			encoder->tmp_pic_total_bytes = 0;
			/* make the address which dummy_nal_buf_addr has, to the top address of dummy_nal_buf[] */
			encoder->dummy_nal_buf_addr = encoder->dummy_nal_buf;

		} /* if (return_code == AVCBE_ENCODE_SUCCESS) */

		/* end of frame? */
		if (encoder->slice_mb_counter == encoder->mb_num_of_picture) {
			frm += encoder->frame_no_increment;
			encoder->frame_counter++;
			encoder->slice_mb_counter = 0;
		}

	} /* while */
	/*---------------------- End of repeating by frame numbers -----------------------------*/

	return (0);
}

int
h264_encode_run (SHCodecs_Encoder *encoder, long stream_type)
{
	long return_code, length;
	TAVCBE_STREAM_BUFF my_end_code_buff_info;

	if (!encoder->initialized)
		h264_encode_deferred_init (encoder, stream_type);

	encoder->error_return_code = 0;

	/* encode process function for H.264 (call avcbe_encode_picture func.) */
	if ((encoder->other_options_h264.avcbe_use_slice == AVCBE_ON) &&
	    (encoder->other_options_h264.avcbe_call_unit == AVCBE_CALL_PER_NAL)) {
		return_code = h264_encode_nal_unit(encoder, stream_type);
	} else {
		return_code = h264_encode_picture_unit(encoder, stream_type);
	}
	if (return_code != 0) {
		return (-115);
	}

	/* The MPEG-4&H.264 Encoder Library API (required-9) End encoding */
	my_end_code_buff_info.buff_top = (unsigned char *) &encoder->my_end_code_buff[0];
	my_end_code_buff_info.buff_size = MY_END_CODE_BUFF_SIZE;

	length = avcbe_put_end_code(encoder->stream_info, &my_end_code_buff_info, AVCBE_END_OF_STRM);
	if (length <= 0) {
		return vpu_err_msg(encoder, __func__, __LINE__, length);
	}
	output_data(encoder, END, &encoder->my_end_code_buff[0], length);

	if (encoder->output_filler_enable == 1) {
		return_code =
		    avcbe_put_filler_data(&encoder->my_stream_buff_info,
					  encoder->other_options_h264.avcbe_put_start_code, 2);
		// TODO shouldn't this be output?
	}
	return (0);
}

