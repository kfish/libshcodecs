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

#ifndef __ENCODER_PRIVATE_H__
#define __ENCODER_PRIVATE_H__

#include <shcodecs/shcodecs_common.h>

#include "avcbe.h"
#include "avcbe_inner.h"
#include "m4iph_vpu4.h"

#include "encoder_common.h"

#define MAX_NUM_REF_FRAMES 2
#define NUM_INPUT_FRAMES 2
#define NUM_LDEC_FRAMES (MAX_NUM_REF_FRAMES+1)

#define ROUND_UP_16(x) ((((x)+15) / 16) * 16)

/**
 * An opaque handle to the VPU4 for encoding.
 */
typedef struct _SHCodecs_Encoder SHCodecs_Encoder;

/**
 * Signature of a callback for libshcodecs to call when it requires YUV 4:2:0 data.
 * To pause encoding, return 1 from this callback.
 * \param encoder The SHCodecs_Encoder* handle
 * \param user_data Arbitrary data supplied by user
 * \retval 0 Continue encoding
 * \retval 1 Pause encoding, return from shcodecs_encode()
 */
typedef int (*SHCodecs_Encoder_Input) (SHCodecs_Encoder * encoder, void *user_data);

/**
 * Signature of a callback for libshcodecs to call when it no longer requires
 * access to a previously input YUV buffer.
 * To pause encoding, return 1 from this callback.
 * \param encoder The SHCodecs_Encoder* handle
 * \param user_data Arbitrary data supplied by user
 * \retval 0 Continue encoding
 * \retval 1 Pause encoding, return from shcodecs_encode()
 */
typedef int (*SHCodecs_Encoder_Input_Release) (SHCodecs_Encoder * encoder,
                                               unsigned char * y_input,
                                               unsigned char * c_input,
                                               void * user_data);
/**
 * Signature of a callback for libshcodecs to call when it has encoded data.
 * To pause encoding, return 1 from this callback.
 * \param encoder The SHCodecs_Encoder* handle
 * \param data The encoded data
 * \param length Length of encoded data in bytes
 * \param user_data Arbitrary data supplied by user
 * \retval 0 Continue encoding
 * \retval 1 Pause encoding, return from shcodecs_encode()
 */
typedef int (*SHCodecs_Encoder_Output) (SHCodecs_Encoder * encoder,
					unsigned char *data, int length,
					void *user_data);

typedef struct {
	long weightdQ_enable;
	TAVCBE_WEIGHTEDQ_CENTER weightedQ_info_center;	/* API´Ø¿ôavcbe_set_weightedQ()¤ËÅÏ¤¹¤¿¤á¤Î¹½Â¤ÂÎ(1) */
	TAVCBE_WEIGHTEDQ_RECT weightedQ_info_rect;	/* API´Ø¿ôavcbe_set_weightedQ()¤ËÅÏ¤¹¤¿¤á¤Î¹½Â¤ÂÎ(2) */
	TAVCBE_WEIGHTEDQ_USER weightedQ_info_user;	/* API´Ø¿ôavcbe_set_weightedQ()¤ËÅÏ¤¹¤¿¤á¤Î¹½Â¤ÂÎ(3) */
	char weightedQ_table_filepath[256];	/* ½Å¤ßÉÕ¤±¥Æ¡¼¥Ö¥ë¥Õ¥¡¥¤¥ë¤Î¥Ñ¥¹Ì¾ */

	/* H.264: VUI parameters */
	avcbe_vui_main_param vui_main_param;

	/* H.264: Flags for whether SEI messages are output or not */
	char out_buffering_period_SEI;
	char out_pic_timing_SEI;
	char out_pan_scan_rect_SEI;
	char out_filler_payload_SEI;
	char out_recovery_point_SEI;
	char out_dec_ref_pic_marking_repetition_SEI; /* TODO: Not used */

	/* H.264: SEI data structures */
	avcbe_sei_buffering_period_param sei_buffering_period_param;
	avcbe_sei_pic_timing_param sei_pic_timing_param;
	avcbe_sei_pan_scan_rect_param sei_pan_scan_rect_param;
	avcbe_sei_filler_payload_param sei_filler_payload_param;
	avcbe_sei_recovery_point_param sei_recovery_point_param;
} OTHER_API_ENC_PARAM;

struct _SHCodecs_Encoder {
	int width;
	int height;

	SHCodecs_Format format;

	SHCodecs_Encoder_Input input;
	void *input_user_data;

	SHCodecs_Encoder_Input_Release release;
	void *release_user_data;

	SHCodecs_Encoder_Output output;
	void *output_user_data;

	/* Internal encode error tracking */
	long error_return_code;	/* return_value of the API function when error ocuured */

	/* Internal */
	int initialized; /* Is avcbe_encode_init() done? */
	int y_bytes; /* Bytes used by Y input plane; CbCr plane uses y_bytes/2 */
	unsigned char * addr_y; /* VPU address to write next Y plane; updated by encoder backends */
	unsigned char * addr_c; /* VPU address to write next C plane; updated by encoder backends */

	avcbe_stream_info *stream_info;
	long frm; /* Current frame */
	long ldec;	/* Index to current working frame */
	long ref1;	/* Index to reference frame */
	long frame_skip_num; /* Number of frames skipped */
	long frame_counter; /* The number of encoded frames */
	long set_intra;	/* Forced intra-mode flag */
	int frame_num_delta;

	/* Working values */
	TAVCBE_FMEM local_frames[NUM_LDEC_FRAMES];
	TAVCBE_FMEM input_frames[NUM_INPUT_FRAMES];
	TAVCBE_WORKAREA work_area;

	TAVCBE_STREAM_BUFF stream_buff_info;
	TAVCBE_STREAM_BUFF end_code_buff_info;

	/* General encoder internals (general_accessors.c) */
	long frame_no_increment;	/* Increment value of Frame number to be encoded for 
					   m4vse_encode_picture function */
	/* encoding parameters */
	avcbe_encoding_property encoding_property;
	avcbe_encoding_property paramR;	/* for stream-1 */
	OTHER_API_ENC_PARAM other_API_enc_param;

	/* MPEG-4 specific internals */
	avcbe_other_options_mpeg4 other_options_mpeg4;

	/* H.264 specific internals */
	TAVCBE_STREAM_BUFF aud_buf_info;
	TAVCBE_STREAM_BUFF sps_buf_info;
	TAVCBE_STREAM_BUFF pps_buf_info;
	TAVCBE_STREAM_BUFF sei_buf_info;
	TAVCBE_STREAM_BUFF filler_data_buff_info;	/* for FillerData(CPB  Buffer) */
	unsigned long filler_data_buff[FILLER_DATA_BUFF_SIZE / 4];	/* for FillerData */

	avcbe_other_options_h264 other_options_h264;	/* parameters to control details */
	unsigned char ref_frame_num;	/* »²¾È¥Õ¥ì¡¼¥à¿ô¡Ê1 or 2) (H.264¤Î¤ß¡Ë */
	long output_filler_enable;	/* enable flag to put Filler Data for CPB Buffer Over */
	long output_filler_data;	/* for FillerData(CPB  Buffer) */

};

/* Internal prototypes of functions using SHCodecs_Encoder */

int h264_encode_init  (SHCodecs_Encoder * encoder, long stream_type);
void h264_encode_close(SHCodecs_Encoder *encoder);
int h264_encode_run (SHCodecs_Encoder * encoder, long stream_type);
int h264_encode_run_multiple (SHCodecs_Encoder *encs[], int nr_encoders, long stream_type);

int mpeg4_encode_init (SHCodecs_Encoder * encoder, long stream_type);
int mpeg4_encode_run (SHCodecs_Encoder * encoder, long stream_type);

#endif				/* __ENCODER_PRIVATE_H__ */

