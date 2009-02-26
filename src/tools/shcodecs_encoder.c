/*
  Copyright (C) Renesas Technology Corp., 2003-2005. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include "m4driverif.h"
#include "m4iph_vpu4.h"		/* SuperH MEPG-4&H.264 Video Driver Library Header */

#include "encoder_private.h"

int
init_other_API_enc_param (OTHER_API_ENC_PARAM * other_API_enc_param)
{
	other_API_enc_param->out_local_decode_file = 0;
	other_API_enc_param->put_local_decode_when_frameskip = 1;
	other_API_enc_param->out_capt_file = 0;
	other_API_enc_param->out_rate_log_file = 0;

	memset(&(other_API_enc_param->vui_main_param), 0, sizeof(avcbe_vui_main_param));

	other_API_enc_param->out_buffering_period_SEI = 0;
	other_API_enc_param->out_pic_timing_SEI = 0;
	other_API_enc_param->out_pan_scan_rect_SEI = 0;
	other_API_enc_param->out_filler_payload_SEI = 0;
	other_API_enc_param->out_recovery_point_SEI = 0;
	other_API_enc_param->out_dec_ref_pic_marking_repetition_SEI = 0;

	memset(& (other_API_enc_param->sei_buffering_period_param), 0,
	       sizeof(avcbe_sei_buffering_period_param));
	memset(&(other_API_enc_param->sei_pic_timing_param), 0,
	       sizeof(avcbe_sei_pic_timing_param));
	memset(&(other_API_enc_param->sei_pan_scan_rect_param),
	       0, sizeof(avcbe_sei_pan_scan_rect_param));
	memset(&(other_API_enc_param->sei_filler_payload_param),
	       0, sizeof(avcbe_sei_filler_payload_param));
	memset(&(other_API_enc_param->sei_recovery_point_param),
	       0, sizeof(avcbe_sei_recovery_point_param));
}

/**
 * Initialize the VPU4 for encoding a given video format.
 * \param width The video image width
 * \param height The video image height
 * \param format SHCodecs_Format_MPEG4 or SHCODECS_Format_H264
 * \return encoder The SHCodecs_Encoder* handle
 */
SHCodecs_Encoder *shcodecs_encoder_init(int width, int height,
					SHCodecs_Format format)
{
	SHCodecs_Encoder *encoder;
        long return_code;

	encoder = malloc(sizeof(SHCodecs_Encoder));
        if (encoder == NULL) return NULL;

	m4iph_vpu_open();
	m4iph_sdr_open();

	encoder->width = width;
	encoder->height = height;
	encoder->format = format;

	encoder->input = NULL;
	encoder->output = NULL;

	m4iph_sleep_time_init();

	/*--- set the parameters of VPU4 (one of the user application's own functions) ---*/
	set_VPU4_param(&(encoder->vpu4_param));

	/*--- The MPEG-4&H.264 Encoder Library API (common to MPEG-4&H.264 Decoder) 
	 * (required-1)@initialize VPU4 ---*/
	/* needs be called only once */
	return_code = m4iph_vpu4_init(&(encoder->vpu4_param));
	if (return_code < 0) {	/* error */
		if (return_code == -1) {
			fprintf (stderr, "encode_1file_mpeg4:m4iph_vpu4_init PARAMETER ERROR!\n");
		}
#if 0
		appli_info->error_return_function = -1;
		appli_info->error_return_code = return_code;
#endif
		return NULL;
	}

        init_other_API_enc_param (&encoder->other_API_enc_param);

	return encoder;
}

/**
 * Deallocate resources used to initialize the VPU4 for encoding,
 * and reset it for future use.
 * \param encoder The SHCodecs_Encoder* handle
 */
void shcodecs_encoder_close(SHCodecs_Encoder * encoder)
{
	m4iph_sdr_free(encoder->vpu4_param.m4iph_temporary_buff_address, MY_STREAM_BUFF_SIZE);

	m4iph_sdr_close();
	m4iph_vpu_close();

	if (encoder)
		free(encoder);

	return;
}

/**
 * Set the callback for libshcodecs to call when raw YUV data is required.
 * \param encoder The SHCodecs_Encoder* handle
 * \param get_input_cb The callback function
 * \param user_data Additional data to pass to the callback function
 */
int
shcodecs_encoder_set_input_callback(SHCodecs_Encoder * encoder,
				    SHCodecs_Encoder_Input input_cb,
				    void *user_data)
{
	encoder->input = input_cb;
	encoder->input_user_data = user_data;

	return 0;
}

/**
 * Set the callback for libshcodecs to call when encoded data is available.
 * \param encoder The SHCodecs_Encoder* handle
 * \param encodec_cb The callback function
 * \param user_data Additional data to pass to the callback function
 */
int
shcodecs_encoder_set_output_callback(SHCodecs_Encoder * encoder,
				     SHCodecs_Encoder_Output output_cb,
				     void *user_data)
{
	encoder->output = output_cb;
	encoder->output_user_data = user_data;

	return 0;
}

/**
 * Run the encoder.
 * \param encoder The SHCodecs_Encoder* handle
 * \retval 0 Success
 */
int shcodecs_encoder_run(SHCodecs_Encoder * encoder, APPLI_INFO * ainfo)
{
	if (encoder->format == SHCodecs_Format_H264) {
		return encode_1file_h264(encoder, ainfo->case_no, ainfo,
					 AVCBE_H264);
	} else {
		return encode_1file_mpeg4(encoder, ainfo->case_no, ainfo,
					  AVCBE_MPEG4);
	}
}
