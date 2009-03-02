/*
  Copyright (C) Renesas Technology Corp., 2003-2005. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include "m4driverif.h"
#include "m4iph_vpu4.h"		/* SuperH MEPG-4&H.264 Video Driver Library Header */

#include "encoder_private.h"

extern char *dummy_nal_buf;

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

extern unsigned long *my_work_area;

u_int32_t sdr_base;

int init_other_API_enc_param(OTHER_API_ENC_PARAM * other_API_enc_param)
{
	other_API_enc_param->out_local_decode_file = 0;
	other_API_enc_param->put_local_decode_when_frameskip = 1;

	memset(&(other_API_enc_param->vui_main_param), 0,
	       sizeof(avcbe_vui_main_param));

	other_API_enc_param->out_buffering_period_SEI = 0;
	other_API_enc_param->out_pic_timing_SEI = 0;
	other_API_enc_param->out_pan_scan_rect_SEI = 0;
	other_API_enc_param->out_filler_payload_SEI = 0;
	other_API_enc_param->out_recovery_point_SEI = 0;
	other_API_enc_param->out_dec_ref_pic_marking_repetition_SEI = 0;

	memset(&(other_API_enc_param->sei_buffering_period_param), 0,
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
					SHCodecs_Format format,
                                        APPLI_INFO * ainfo)
{
	SHCodecs_Encoder *encoder;
	long return_code;
        int i;

	encoder = malloc(sizeof(SHCodecs_Encoder));
	if (encoder == NULL)
		return NULL;

	m4iph_vpu_open();
	m4iph_sdr_open();

	encoder->width = width;
	encoder->height = height;
	encoder->format = format;

	encoder->input = NULL;
	encoder->output = NULL;

        encoder->frm = 0;
        encoder->frame_skip_num = 0;
	encoder->set_intra = AVCBE_ANY_VOP;
	encoder->output_type = AVCBE_OUTPUT_NONE;

	m4iph_sleep_time_init();

	/*--- set the parameters of VPU4 (one of the user application's own functions) ---*/
	set_VPU4_param(&(encoder->vpu4_param));

	/*--- The MPEG-4&H.264 Encoder Library API (common to MPEG-4&H.264 Decoder) 
	 * (required-1)@initialize VPU4 ---*/
	/* needs be called only once */
	return_code = m4iph_vpu4_init(&(encoder->vpu4_param));
	if (return_code < 0) {	/* error */
		if (return_code == -1) {
			fprintf(stderr,
				"encode_1file_mpeg4:m4iph_vpu4_init PARAMETER ERROR!\n");
		}
		return NULL;
	}

	init_other_API_enc_param(&encoder->other_API_enc_param);

	/* stream buffer 0 clear */
//      memset(sdr_read_my_stream_buff,0,sizeof(sdr_read_my_stream_buff));
	encode_time_init();
	vpu4_clock_on();
	width_height = ainfo->enc_exec_info.xpic * ainfo->enc_exec_info.ypic;
	width_height += (width_height / 2);
	max_frame = 2;
	sdr_base = m4iph_sdr_malloc(width_height * (max_frame + 3), 32);
	if (sdr_base == NULL)
		exit(1);
	kernel_memory_for_vpu_top = (unsigned long *) sdr_base;
	for (i = 0; i < max_frame; i++) {
		my_frame_memory_capt[i] =
		    (unsigned long *) (sdr_base + width_height * i);
		printf("my_frame_memory_capt[%d]=%x\n", i,
		       my_frame_memory_capt[i]);
	}
	my_frame_memory_ldec1 =
	    (unsigned long *) (sdr_base + width_height * i);
	i++;
	my_frame_memory_ldec2 =
	    (unsigned long *) (sdr_base + width_height * i);
	i++;
	my_frame_memory_ldec3 =
	    (unsigned long *) (sdr_base + width_height * i);
	my_stream_buff_bak = malloc(MY_STREAM_BUFF_SIZE + 31);
	my_stream_buff = ALIGN(my_stream_buff_bak, 32);
	my_end_code_buff_bak = malloc(MY_END_CODE_BUFF_SIZE + 31);
	my_end_code_buff = ALIGN(my_end_code_buff_bak, 32);

	if (encoder->format == SHCodecs_Format_H264) {
		encode_init_h264 (encoder, ainfo, AVCBE_H264);
	} else {
		encode_init_mpeg4 (encoder, ainfo, AVCBE_MPEG4);
	}

	return encoder;
}

/**
 * Deallocate resources used to initialize the VPU4 for encoding,
 * and reset it for future use.
 * \param encoder The SHCodecs_Encoder* handle
 */
void shcodecs_encoder_close(SHCodecs_Encoder * encoder)
{
	m4iph_sdr_free(sdr_base, width_height * (max_frame + 3));

	m4iph_sdr_free(encoder->vpu4_param.m4iph_temporary_buff_address,
		       MY_STREAM_BUFF_SIZE);

	m4iph_sdr_close();
	m4iph_vpu_close();

	if (encoder)
		free(encoder);

	free(my_stream_buff_bak);
	free(my_end_code_buff_bak);
	free(my_work_area);
	free(dummy_nal_buf);

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
		return encode_1file_h264(encoder, ainfo, AVCBE_H264);
	} else {
		return encode_1file_mpeg4(encoder, ainfo, AVCBE_MPEG4);
	}
}
