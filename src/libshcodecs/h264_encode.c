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
#include <string.h>
#include <malloc.h>
#include <sys/time.h>

#include "avcbe.h"		/* SuperH MEPG-4&H.264 Video Encode Library Header */
#include "m4iph_vpu4.h"		/* SuperH MEPG-4&H.264 Video Driver Library Header */
#include "encoder_common.h"
#include "m4driverif.h"

#include "encoder_private.h"

#define OUTPUT_ERROR_MSGS
//#define OUTPUT_INFO_MSGS
//#define OUTPUT_STREAM_INFO


#ifdef OUTPUT_ERROR_MSGS
#define MSG_LEN 127
static long
vpu_err(SHCodecs_Encoder *enc, const char *func, int line, long rc)
{
	char msg[MSG_LEN+1];
	msg[MSG_LEN] = 0;
	snprintf(msg, MSG_LEN, "%s, line %d: returned %ld", func, line, rc);
	m4iph_avcbe_perror(msg, rc);
	enc->error_return_code = rc;
	return rc;
}
#else
#define vpu_err(enc, func, line, rc) (rc)
#endif


#ifdef OUTPUT_INFO_MSGS
/* Information messages */
static const char *vpu_info_txt[] = {
	"Success",
	"Frame skipped",
	"Empty VOP has been output",
	"",
	"B-VOP output",
	"Slice remain, picture not finished",
	"Output Sequence Parameter Set",
	"Output Picture Parameter Set",
};

static void
vpu_info_msg(SHCodecs_Encoder *enc, const char *func, int line, long frm, long rc)
{
	if (rc > 0 && rc < 8) {
		fprintf (stderr, "%s, line %d: returned %ld (%s) frm=%d,seq_no=%d\n",
			func, line, rc, vpu_info_txt[rc],
			(int) frm,
			(int) enc->frame_counter);
	}
} 
#else
#define vpu_info_msg(encoder, func, line, frm, rc)
#endif


enum {SEI, SPS, PPS, AUD, IDATA, PDATA, FILL, END};
#ifdef OUTPUT_STREAM_INFO
static const char*data_name[] =
	{"SEI", "SPS", "PPS", "AUD", "I", "P", "FILL", "END"};
#endif

static int
output_data(SHCodecs_Encoder *enc, int type, void *buf, long length)
{
#ifdef OUTPUT_STREAM_INFO
	fprintf (stderr, "output %s (%d bytes)\n", data_name[type], (int)length);
#endif
	if (enc->output) {
		return enc->output(enc, (unsigned char *)buf, length,
				   enc->output_user_data);
	}

	return 0;
}


int
h264_encode_init (SHCodecs_Encoder *enc, long stream_type)
{
	long rc;

	enc->error_return_code = 0;

	/* Access Unit Delimiter (AUD) output buffer */
	enc->aud_buf_info.buff_size = 16;
	enc->aud_buf_info.buff_top = memalign(4, 16);
	if (!enc->aud_buf_info.buff_top)
		goto err;

	/* SPS output buffer */
	enc->sps_buf_info.buff_size = SPS_STREAM_BUFF_SIZE;
	enc->sps_buf_info.buff_top = memalign(32, SPS_STREAM_BUFF_SIZE);
	if (!enc->sps_buf_info.buff_top)
		goto err;

	/* PPS output buffer */
	enc->pps_buf_info.buff_size = PPS_STREAM_BUFF_SIZE;
	enc->pps_buf_info.buff_top = memalign(32, PPS_STREAM_BUFF_SIZE);
	if (!enc->pps_buf_info.buff_top)
		goto err;

	/* SEI output buffer */
	enc->sei_buf_info.buff_size = SEI_STREAM_BUFF_SIZE;
	enc->sei_buf_info.buff_top = memalign(4, SEI_STREAM_BUFF_SIZE);
	if (!enc->sei_buf_info.buff_top)
		goto err;

	/* Set default values for the parameters */
	rc = avcbe_set_default_param(stream_type, AVCBE_RATE_NORMAL,
				    &(enc->encoding_property),
				    (void *)&(enc->other_options_h264));
	if (rc != 0)
		return vpu_err(enc, __func__, __LINE__, rc);

	return 0;

err:
	h264_encode_close(enc);
	return -1;
}

static int
h264_encode_deferred_init(SHCodecs_Encoder *enc, long stream_type)
{
	long rc;
	avcbe_other_options_h264 *options;

	/* Initialize VPU parameters & local frame memory */
	options = &enc->other_options_h264;

	if (options->avcbe_ratecontrol_cpb_buffer_mode == AVCBE_MANUAL) {
		options->avcbe_ratecontrol_cpb_offset = (unsigned long)
		    avcbe_calc_cpb_buff_offset(enc->encoding_property.avcbe_bitrate,
		     	(options->avcbe_ratecontrol_cpb_max_size *
				 options->avcbe_ratecontrol_cpb_buffer_unit_size),
				90);
	}
	rc = avcbe_init_encode(&(enc->encoding_property),
				&(enc->paramR),
				options,
				(avcbe_buf_continue_userproc_ptr) NULL,
				&enc->work_area, NULL, &enc->stream_info);
	if (rc < 0)
		return vpu_err(enc, __func__, __LINE__, rc);

	rc = avcbe_init_memory(enc->stream_info,
				enc->ref_frame_num,
				(enc->ref_frame_num+1), enc->local_frames,
				ROUND_UP_16(enc->width), ROUND_UP_16(enc->height));
	if (rc != 0)
		return vpu_err(enc, __func__, __LINE__, rc);

	enc->initialized = 2;

	return 0;
}

void
h264_encode_close(SHCodecs_Encoder *enc)
{
	free(enc->aud_buf_info.buff_top);
	free(enc->sps_buf_info.buff_top);
	free(enc->pps_buf_info.buff_top);
	free(enc->sei_buf_info.buff_top);
}

/* returns 0 on success */
static long
h264_output_SEI_parameters(SHCodecs_Encoder *enc)
{
	OTHER_API_ENC_PARAM *params = &enc->other_API_enc_param;
	long return_value = 0;
	long length;
	int num_sei_msgs;
	int i;
	int cb_ret;

	/* H.264 Spec: The buffer period SEI message is the first SEI NAL unit */
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

	/* Output SEI parameters */
	for (i=0; i<num_sei_msgs; i++)
	{
		if (sei_msg[i] == AVCBE_ON) {
			length = avcbe_put_SEI_parameters(
						enc->stream_info,
						sei_arg1[i],
						sei_arg2[i],
						&(enc->sei_buf_info));

			if (length > 0) {
				cb_ret = output_data(enc, SEI,
							enc->sei_buf_info.buff_top, length);
				if (cb_ret != 0)
					return cb_ret;
			} else {
				vpu_err(enc, __func__, __LINE__, length);
				return_value |= (0x1 << i);
			}
		}
	}

	return return_value;
}

static long
setup_vui_params(SHCodecs_Encoder *enc)
{
	avcbe_vui_main_param *vui_param;
	long rc, length;

	vui_param = &enc->other_API_enc_param.vui_main_param;

	/* Get the size of CPB-buffer to set 'cpb_size_scale' of HRD */
	length = avcbe_get_cpb_buffer_size(enc->stream_info);
	if (length <= 0)
		return vpu_err(enc, __func__, __LINE__, length);

	if (vui_param->avcbe_nal_hrd_parameters_present_flag == AVCBE_ON) {
		vui_param->avcbe_nal_hrd_param.avcbe_cpb_size_scale = length;
	} else if (vui_param->avcbe_vcl_hrd_parameters_present_flag == AVCBE_ON) {
		vui_param->avcbe_vcl_hrd_param.avcbe_cpb_size_scale = length;
	}

	rc = avcbe_set_VUI_parameters(enc->stream_info, vui_param);
	if (rc != 0)
		return vpu_err(enc, __func__, __LINE__, rc);

	return 0;
}

/* Get SPS & PPS data, but do not output it */
static long
h264_encode_sps_pps(SHCodecs_Encoder *enc, avcbe_slice_stat *slice_stat, long frm)
{
	long rc;

	/* SPS data */
	rc = avcbe_encode_picture(
				enc->stream_info, frm,
				AVCBE_ANY_VOP,
				AVCBE_OUTPUT_SPS,
				&enc->sps_buf_info,
				NULL);
	if (rc != AVCBE_SPS_OUTPUTTED)
		return vpu_err(enc, __func__, __LINE__, rc);

	/* Get the size */
	avcbe_get_last_slice_stat(enc->stream_info, slice_stat);


	/* PPS data */
	rc = avcbe_encode_picture(
				enc->stream_info, frm,
				AVCBE_ANY_VOP,
				AVCBE_OUTPUT_PPS,
				&enc->pps_buf_info,
				NULL);
	if (rc != AVCBE_PPS_OUTPUTTED)
		return vpu_err(enc, __func__, __LINE__, rc);

	/* Get the size */
	avcbe_get_last_slice_stat(enc->stream_info, slice_stat);

	return 0;
}

/* Encode a whole frame for H.264 */
static long
h264_encode_frame(SHCodecs_Encoder *enc, unsigned char *py, unsigned char *pc)
{
	long rc, enc_rc;
	avcbe_slice_stat slice_stat;
	TAVCBE_FMEM input_buf;
	TAVCBE_STREAM_BUFF *extra_stream_buff;
	char pic_type;
	int start_of_frame;
	long nal_size;
	int cb_ret = 0;

	start_of_frame = 1;

	input_buf.Y_fmemp = py;
	input_buf.C_fmemp = pc;
	extra_stream_buff = &enc->aud_buf_info;

	/* Specify the input frame address */
	rc = avcbe_set_image_pointer(enc->stream_info,
				    &input_buf, enc->ldec, enc->ref1, 0);
	if (rc != 0)
		return vpu_err(enc, __func__, __LINE__, rc);

	if (enc->frame_counter != 0) {
		/* Restore stream context */
		rc = avcbe_set_backup(enc->stream_info, &enc->backup_area);
		if (rc != 0)
			return vpu_err(enc, __func__, __LINE__, rc);
	}

	/* Encode SPS and PPS for 1st frame */
	if (enc->frame_counter == 0) {
		rc = h264_encode_sps_pps(enc, &slice_stat, enc->frm);
		if (rc != 0)
			return rc;
	}

#ifdef OUTPUT_STREAM_INFO
	fprintf(stderr, "\nFrame %d:\n", enc->frame_counter);
#endif

	/* Continue encoding until a frame has been encoded or skipped */
	while (1) {

		/* Reset the amount of filler data used */	
		enc->output_filler_data = 0;

		/* Output SEI data (if AU delimiter is NOT used) */
		if (extra_stream_buff == NULL) {
			rc = h264_output_SEI_parameters(enc);
			if (rc != 0)
				return rc;
		}

		/* Encode the frame */
		enc_rc = avcbe_encode_picture(enc->stream_info, enc->frm,
					 AVCBE_ANY_VOP,
					 AVCBE_OUTPUT_SLICE,
					 &enc->stream_buff_info,
					 extra_stream_buff);
		if (enc_rc < 0)
			return vpu_err(enc, __func__, __LINE__, enc_rc);
		vpu_info_msg(enc, __func__, __LINE__, enc->frm, enc_rc);

		if (enc_rc == AVCBE_FRAME_SKIPPED) {
			enc->frame_num_delta++;
			enc->frame_skip_num++;
		}

		if ((enc_rc == AVCBE_SLICE_REMAIN)
		    || (enc_rc == AVCBE_ENCODE_SUCCESS)) {

			/* Get the information about the encoded slice */
			avcbe_get_last_slice_stat(enc->stream_info, &slice_stat);
			nal_size = (slice_stat.avcbe_encoded_slice_bits + 7) / 8;
			pic_type = slice_stat.avcbe_encoded_pic_type;

			if (start_of_frame) {

				/* Output AU delimiter */
				if (extra_stream_buff != NULL) {
					cb_ret = output_data(enc, AUD, enc->aud_buf_info.buff_top,
							slice_stat.avcbe_AU_unit_bytes);
					if (cb_ret != 0)
						return cb_ret;
				}

				/* If the type is IDR-picture, encode SPS and PPS data (after 1st frame) */
				if ((enc->frame_counter > 0)
				     && (pic_type == AVCBE_IDR_PIC)) {
					rc = h264_encode_sps_pps(enc, &slice_stat, enc->frm);
					if (rc != 0)
						return rc;
				}
				if ((enc->frame_counter == 0)
				    || (pic_type == AVCBE_IDR_PIC)) {
					/* output SPS data and PPS data */
					cb_ret = output_data(enc, SPS, enc->sps_buf_info.buff_top,
						slice_stat.avcbe_SPS_unit_bytes);
					if (cb_ret != 0)
						return cb_ret;
					cb_ret = output_data(enc, PPS, enc->pps_buf_info.buff_top,
						slice_stat.avcbe_PPS_unit_bytes);
					if (cb_ret != 0)
						return cb_ret;
				}

				/* output SEI parameter */
				rc = h264_output_SEI_parameters(enc);
				if (rc != 0)
					return rc;

				/* output Filler data (if CPB Buffer overflow) */
				if ((enc->output_filler_enable == 1)
				    && (enc->output_filler_data > 0)) {
					cb_ret = output_data(enc, FILL,
						enc->filler_data_buff_info.buff_top,
						enc->output_filler_data);
					if (cb_ret != 0)
						return cb_ret;
				}

				enc->frame_num_delta++;
				start_of_frame = 0;
			}

			/* Output slice data */
			if ((pic_type == AVCBE_IDR_PIC)
			    || (pic_type == AVCBE_I_PIC)) {
				cb_ret = output_data(enc, IDATA,
					enc->stream_buff_info.buff_top, nal_size);
				if (cb_ret != 0)
					return cb_ret;
			} else {
				cb_ret = output_data(enc, PDATA,
					enc->stream_buff_info.buff_top, nal_size);
				if (cb_ret != 0)
					return cb_ret;
			}

			enc->frame_num_delta = 0;
		}

	
		/* End of a frame? */
		if (enc_rc == AVCBE_ENCODE_SUCCESS) {	
			/* Switch the indexes to the reference and locally decoded frames */
			if (enc->ldec == 0) {
				enc->ldec = 1;
				enc->ref1 = 0;
			} else {
				enc->ldec = 0;
				enc->ref1 = 1;
			}
		}

		if ((enc_rc == AVCBE_FRAME_SKIPPED)
		    || (enc_rc == AVCBE_ENCODE_SUCCESS)) {
			/* Move to next input frame */
			enc->frm += enc->frame_no_increment;
			enc->frame_counter++;
			break;
		}

	} /* while */

	/* Save stream context */
	rc = avcbe_get_backup(enc->stream_info, &enc->backup_area);
	if (rc != 0)
		return vpu_err(enc, __func__, __LINE__, rc);

	return 0;
}

static long
h264_encode_multiple(SHCodecs_Encoder *encs[], int nr_encoders)
{
	SHCodecs_Encoder * enc;
	long rc;
	int i, j, cb_ret;

	for (i=0; i < nr_encoders; i++) {
		enc = encs[i];

		/* Fixed input buffer if client doesn't change it */
		enc->addr_y = enc->input_frames[0].Y_fmemp;
		enc->addr_c = enc->input_frames[0].C_fmemp;

		/* Release all input buffers */
		for (j=0; j < NUM_INPUT_FRAMES; j++) {
			if (enc->release) {
				enc->release (enc,
					enc->input_frames[j].Y_fmemp,
					enc->input_frames[j].C_fmemp,
					enc->release_user_data);
			}
		}

		/* Setup VUI parameters */
		if (enc->other_options_h264.avcbe_out_vui_parameters == AVCBE_ON) {
			rc = setup_vui_params(enc);
			if (rc != 0)
				return rc;
		}

		enc->ldec = 0;		/* Index number of the image-work-field area */
		enc->ref1 = 0;
		enc->frm = 0;		/* Frame number to be encoded */

		enc->frame_counter = 0;
		enc->frame_skip_num = 0;

		/* Filler Data output buffer */
		enc->filler_data_buff_info.buff_top = (unsigned char *) &enc->filler_data_buff[0];
		enc->filler_data_buff_info.buff_size = FILLER_DATA_BUFF_SIZE;
	}

	/* For all frames to encode */
	while (1) {
		for (i=0; i < nr_encoders; i++) {
			enc = encs[i];
			/* Get the encoder input frame */
			if (enc->input) {
				cb_ret = enc->input(enc, enc->input_user_data);
				if (cb_ret != 0) {
#ifdef OUTPUT_STREAM_INFO
					fprintf (stderr, "%s: ERROR acquiring input image!\n", __func__);
#endif
					enc->error_return_code = (long)cb_ret ;
					return cb_ret ;
				}
			}

			/* Encode the frame */
			m4iph_vpu_lock();
			rc = h264_encode_frame(enc, enc->addr_y, enc->addr_c);
			m4iph_vpu_unlock();

			if (enc->release) {
				enc->release(enc, enc->addr_y, enc->addr_c, enc->release_user_data);
			}

			if (rc != 0)
				return rc;
		}
	}

	return 0;
}

int
h264_encode_run_multiple (SHCodecs_Encoder *encs[], int nr_encoders, long stream_type)
{
	SHCodecs_Encoder * enc;
	long i, rc, length;

	for (i=0; i < nr_encoders; i++) {
		enc = encs[i];
		if (enc->initialized < 2) {
			rc = h264_encode_deferred_init(enc, stream_type);
			if (rc != 0)
				return rc;
		}
	}

	rc = h264_encode_multiple(encs, nr_encoders);
	if (rc != 0)
		return rc;

	/* End encoding */
	for (i=0; i < nr_encoders; i++) {
		enc = encs[i];
		length = avcbe_put_end_code(enc->stream_info, &enc->end_code_buff_info, AVCBE_END_OF_STRM);
		if (length <= 0)
			return vpu_err(enc, __func__, __LINE__, length);
		rc = output_data(enc, END, enc->end_code_buff_info.buff_top, length);
		if (rc != 0)
			return rc;

		if (enc->output_filler_enable == 1) {
			rc = avcbe_put_filler_data(&enc->stream_buff_info,
					enc->other_options_h264.avcbe_put_start_code, 2);
			// TODO shouldn't this be output?
		}
	}

	return 0;
}

int
h264_encode_run (SHCodecs_Encoder *enc, long stream_type)
{
	return h264_encode_run_multiple (&enc, 1, stream_type);
}
