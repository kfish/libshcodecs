/*
  Copyright (C) Renesas Technology Corp., 2003-2005. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "m4driverif.h"
#include "m4iph_vpu4.h"		/* SuperH MEPG-4&H.264 Video Driver Library Header */

#include "encoder_private.h"

extern char *dummy_nal_buf;

int vpu4_clock_on(void);
int vpu4_clock_off(void);

static void
set_dimensions (SHCodecs_Encoder * encoder, int width, int height)
{
	encoder->width = width;
	encoder->height = height;

	encoder->y_bytes = (((width + 15) / 16) * 16) * (((height + 15) / 16) * 16);
}

/*----------------------------------------------------------------------------------------------*/
/* set the parameters of VPU4 */
/*----------------------------------------------------------------------------------------------*/
static void
set_VPU4_param(SHCodecs_Encoder * encoder)
{
	M4IPH_VPU4_INIT_OPTION * vpu4_param = &(encoder->vpu4_param);
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
	tb = (unsigned long)m4iph_sdr_malloc(MY_STREAM_BUFF_SIZE, 32);
	vpu4_param->m4iph_temporary_buff_address = tb;
	printf("m4iph_temporary_buff_address=%lX, size=%d\n", tb,
	       MY_STREAM_BUFF_SIZE);
	/* Temporary Buffer Size */
	vpu4_param->m4iph_temporary_buff_size = MY_STREAM_BUFF_SIZE;
	// vpu4_param->m4iph_temporary_buff_size = MY_WORK_AREA_SIZE;
	if (encoder->my_work_area == NULL) {
		encoder->my_work_area = malloc(MY_WORK_AREA_SIZE);	/* 4 bytes alignment */
		dummy_nal_buf = malloc(MY_DUMMY_NAL_BUFF_SIZE);
		memset(dummy_nal_buf, 0, MY_DUMMY_NAL_BUFF_SIZE);
		printf("my_work_area=%pX\n", encoder->my_work_area);
		if (encoder->my_work_area == NULL) {
			printf("Memoey allocation error\n");
			exit(-200);
		}
	}
}

static int
init_other_API_enc_param(OTHER_API_ENC_PARAM * other_API_enc_param)
{
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

	return 0;
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
        long width_height, max_frame=2;
	long return_code;
        int i;

	encoder = malloc(sizeof(SHCodecs_Encoder));
	if (encoder == NULL)
		return NULL;

	m4iph_vpu_open();
	m4iph_sdr_open();

        set_dimensions (encoder, width, height);
	encoder->format = format;

	encoder->input = NULL;
	encoder->output = NULL;

        encoder->error_return_function = 0;
        encoder->error_return_code = 0;

        encoder->initialized = 0;

        encoder->frame_number_to_encode = 1;
        encoder->frm = 0;
        encoder->frame_no_increment = 1;
        encoder->frame_skip_num = 0;
	encoder->set_intra = AVCBE_ANY_VOP;
	encoder->output_type = AVCBE_OUTPUT_NONE;

	encoder->mb_num_of_picture = 0;
	encoder->slice_mb_counter = 0;
	encoder->SPS_PPS_header_bytes = 0;
	encoder->output_filler_enable = 0;
	encoder->output_filler_data = 0;

	m4iph_sleep_time_init();

	/*--- set the parameters of VPU4 (one of the user application's own functions) ---*/
	set_VPU4_param(encoder);

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
	encode_time_init();
	vpu4_clock_on();
	width_height = width + height;
	width_height += (width_height / 2);
	max_frame = 2;
	encoder->sdr_base = m4iph_sdr_malloc(width_height * (max_frame + 3), 32);
	if (encoder->sdr_base == NULL)
		exit(1);
	for (i = 0; i < max_frame; i++) {
		encoder->my_frame_memory_capt[i] =
		    (unsigned long *) (encoder->sdr_base + width_height * i);
		printf("my_frame_memory_capt[%d]=%p\n", i,
		       encoder->my_frame_memory_capt[i]);
	}
	encoder->my_work_area = NULL;
	encoder->my_frame_memory_ldec1 =
	    (unsigned long *) (encoder->sdr_base + width_height * i);
	i++;
	encoder->my_frame_memory_ldec2 =
	    (unsigned long *) (encoder->sdr_base + width_height * i);
	i++;
	encoder->my_frame_memory_ldec3 =
	    (unsigned long *) (encoder->sdr_base + width_height * i);
	encoder->my_stream_buff_bak = malloc(MY_STREAM_BUFF_SIZE + 31);
	encoder->my_stream_buff = ALIGN(encoder->my_stream_buff_bak, 32);
	encoder->my_end_code_buff_bak = malloc(MY_END_CODE_BUFF_SIZE + 31);
	encoder->my_end_code_buff = ALIGN(encoder->my_end_code_buff_bak, 32);

	if (encoder->format == SHCodecs_Format_H264) {
		h264_encode_init (encoder, AVCBE_H264);
	} else {
		mpeg4_encode_init (encoder, AVCBE_MPEG4);
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
        long width_height, max_frame=2;

	if (encoder == NULL) return;

	width_height = encoder->width + encoder->height;
	width_height += (width_height / 2);
	m4iph_sdr_free(encoder->sdr_base, width_height * (max_frame + 3));

	m4iph_sdr_free((unsigned char *)encoder->vpu4_param.m4iph_temporary_buff_address,
		       MY_STREAM_BUFF_SIZE);

	m4iph_sdr_close();
	m4iph_vpu_close();

	free(encoder->my_work_area);
	free(encoder);

	free(encoder->my_stream_buff_bak);
	free(encoder->my_end_code_buff_bak);
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
int shcodecs_encoder_run(SHCodecs_Encoder * encoder)
{
	if (encoder->format == SHCodecs_Format_H264) {
		return h264_encode_run (encoder, AVCBE_H264);
	} else {
		return mpeg4_encode_run (encoder, AVCBE_MPEG4);
	}
}

int
shcodecs_encoder_input_provide (SHCodecs_Encoder * encoder, 
                                unsigned char * y_input, unsigned char * c_input)
{
	/* Write image data to kernel memory for VPU */
	m4iph_sdr_write(encoder->addr_y, y_input, encoder->y_bytes);
	m4iph_sdr_write(encoder->addr_c, c_input, encoder->y_bytes / 2);

        return 0;
}



/*
 * Get the width in pixels of the encoded image
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The width in pixels
 * \retval -1 \a encoder invalid
 */
int
shcodecs_encoder_get_width (SHCodecs_Encoder * encoder)
{
  if (encoder == NULL) return -1;

  return encoder->width;
}

/*
 * Get the height in pixels of the encoded image
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The height in pixels
 * \retval -1 \a encoder invalid
 */
int
shcodecs_encoder_get_height (SHCodecs_Encoder * encoder)
{
  if (encoder == NULL) return -1;

  return encoder->height;
}

/**
 * Get the size in bytes of a Y plane of input data.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns size in bytes of Y plane.
 * \retval -1 \a encoder invalid
 */
int
shcodecs_encoder_get_y_bytes (SHCodecs_Encoder * encoder)
{
  if (encoder == NULL) return -1;

  return encoder->y_bytes;
}

/**
 * Get the size in bytes of a CbCr plane of input data.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns size in bytes of CbCr plane.
 * \retval -1 \a encoder invalid
 */
int
shcodecs_encoder_get_c_bytes (SHCodecs_Encoder * encoder)
{
  if (encoder == NULL) return -1;

  return encoder->y_bytes/2;
}

