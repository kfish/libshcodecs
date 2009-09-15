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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "m4driverif.h"
#include "m4iph_vpu4.h"		/* SuperH MEPG-4&H.264 Video Driver Library Header */

#include "encoder_private.h"

#define NUM_LDEC_FRAMES 3

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
#endif


	/* Interrupt */
#ifdef DISABLE_INT
	vpu4_param->m4iph_vpu_interrupt_enable = M4IPH_OFF;
#else
	vpu4_param->m4iph_vpu_interrupt_enable = M4IPH_ON;
#endif

	/* Supply of VPU4 Clock */
	vpu4_param->m4iph_vpu_clock_supply_control = 0;	/* 'clock_supply_enable' -> 'clock_supply_control' changed when Version2 */

	/* Address Mask chage */
	vpu4_param->m4iph_vpu_mask_address_disable = M4IPH_OFF;

	/* Temporary Buffer */
	tb = (unsigned long)m4iph_sdr_malloc(MY_STREAM_BUFF_SIZE, 32);
	vpu4_param->m4iph_temporary_buff_address = tb;
	vpu4_param->m4iph_temporary_buff_size = MY_STREAM_BUFF_SIZE;
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
	int i, j;
	unsigned char *pFramesBase, *pY;

	encoder = calloc(1, sizeof(SHCodecs_Encoder));
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

	encoder->output_filler_enable = 0;
	encoder->output_filler_data = 0;

	m4iph_sleep_time_init();

	/* Set the VPU parameters */
	set_VPU4_param(encoder);

	/* Initialize VPU */
	return_code = m4iph_vpu4_init(&(encoder->vpu4_param));
	if (return_code < 0) {
		if (return_code == -1) {
			fprintf(stderr,
				"%s: m4iph_vpu4_init PARAMETER ERROR!\n", __func__);
		}
		goto err;
	}

	init_other_API_enc_param(&encoder->other_API_enc_param);

	/* stream buffer 0 clear */
	encode_time_init();
	vpu4_clock_on();
	width_height = width * height;
	width_height += (width_height / 2);
	max_frame = 2;
	pFramesBase = m4iph_sdr_malloc(width_height * (max_frame + 3), 32);
	encoder->sdr_base = (unsigned long)pFramesBase;
	if (!pFramesBase)
		goto err;

	/* Input buffers */
	for (i=0; i < max_frame; i++) {
		pY = pFramesBase + width_height * i;
		encoder->input_frames[i].Y_fmemp = pY;
		encoder->input_frames[i].C_fmemp = pY + encoder->y_bytes;
	}

	/* Local decode images */
	for (j=0; j<NUM_LDEC_FRAMES; j++) {
		pY = pFramesBase + width_height * i;
		encoder->local_frames[j].Y_fmemp = pY;
		encoder->local_frames[j].C_fmemp = pY + encoder->y_bytes;
		i++;
	}

	encoder->work_area.area_size = MY_WORK_AREA_SIZE;
	encoder->work_area.area_top = memalign(MY_WORK_AREA_SIZE, 4);
	if (!encoder->work_area.area_top)
		goto err;

	encoder->stream_buff_info.buff_size = MY_STREAM_BUFF_SIZE;
	encoder->stream_buff_info.buff_top = memalign(MY_STREAM_BUFF_SIZE, 32);
	if (!encoder->stream_buff_info.buff_top)
		goto err;

	encoder->end_code_buff_info.buff_size = MY_END_CODE_BUFF_SIZE;
	encoder->end_code_buff_info.buff_top = memalign(MY_END_CODE_BUFF_SIZE, 32);
	if (!encoder->end_code_buff_info.buff_top)
		goto err;

	if (encoder->format == SHCodecs_Format_H264) {
		return_code = h264_encode_init (encoder, AVCBE_H264);
	} else {
		return_code = mpeg4_encode_init (encoder, AVCBE_MPEG4);
	}
	if (return_code < 0)
		goto err;

	return encoder;

err:
	shcodecs_encoder_close(encoder);
	return NULL;
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

	width_height = encoder->width * encoder->height;
	width_height += (width_height / 2);
	m4iph_sdr_free((unsigned char *)encoder->sdr_base, width_height * (max_frame + 3));

	m4iph_sdr_free((unsigned char *)encoder->vpu4_param.m4iph_temporary_buff_address,
		       MY_STREAM_BUFF_SIZE);

	m4iph_sdr_close();
	m4iph_vpu_close();

	if (encoder->format == SHCodecs_Format_H264) {
		h264_encode_close(encoder);
	}

	free(encoder->work_area.area_top);
	free(encoder->stream_buff_info.buff_top);
	free(encoder->end_code_buff_info.buff_top);

	free(encoder);
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

/**
 * Get the physical address of input data.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns size in bytes of CbCr plane.
 * \retval -1 \a encoder invalid
 */
int
shcodecs_encoder_get_input_physical_addr (SHCodecs_Encoder * encoder, 
                     unsigned int *addr_y, unsigned int *addr_C)
{
  if (encoder == NULL) return -1;
  *addr_y = (unsigned int)encoder->addr_y;
  *addr_C = (unsigned int)encoder->addr_c;
  return 0;
}


