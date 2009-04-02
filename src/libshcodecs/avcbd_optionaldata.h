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

/** \file avcbd_optionaldata.h
 *  \brief definitions of additional buffer such as vui_parameters() or sei_payload().
 */

#ifndef _AVCBD_OPTIONALDATA_H_
#define _AVCBD_OPTIONALDATA_H_

#ifdef __cplusplus
extern "C" {
#endif

#define AVCBD_MAX_CPB_CNT 32
#define AVCBD_MAX_CLOCK_TS 3
#define AVCBD_MAX_PAN_SCAN_CNT 3

/** Structure to store hrd_parameters().
 */
	typedef struct {
		unsigned char cpb_cnt_minus1;	/* range [0,31] */
		unsigned char bit_rate_scale;
		unsigned char cpb_size_scale;
		unsigned char cbr_flag[AVCBD_MAX_CPB_CNT];
		unsigned long bit_rate_value_minus1[AVCBD_MAX_CPB_CNT];	/* range [0,ULONG_MAX-2] */
		unsigned long cpb_size_value_minus1[AVCBD_MAX_CPB_CNT];	/* range [0,ULONG_MAX-2] */
		unsigned char initial_cpb_removal_delay_length_minus1;
		unsigned char cpb_removal_delay_length_minus1;
		unsigned char dpb_output_delay_length_minus1;
		unsigned char time_offset_length;
	} TAVCBD_HRD_PARAMETERS;

/** Structure to store vui_parameters().
 */
	typedef struct {
		char aspect_ratio_info_present_flag;
		unsigned char aspect_ratio_idc;	/* 8 bits */
		unsigned char colour_primaries;	/* 8 bits */
		unsigned char transfer_characteristics;	/* 8 bits */
		unsigned char matrix_coefficients;	/* 8 bits */
		unsigned char chroma_loc_info_present_flag;
		unsigned char timing_info_present_flag;
		unsigned char fixed_frame_rate_flag;
		unsigned short sar_width;	/* 16 bits */
		unsigned short sar_height;	/* 16 bits */
		unsigned short chroma_sample_loc_type_top_field;	/* range: [0,5] */
		unsigned short chroma_sample_loc_type_bottom_field;	/* range: [0,5] */
		unsigned long num_units_in_tick;	/* 32 bits */
		unsigned long time_scale;	/* 32 bits */
		long nal_hrd_parameters_present_flag;
		TAVCBD_HRD_PARAMETERS nal_hrd_parameters;
		long vcl_hrd_parameters_present_flag;
		TAVCBD_HRD_PARAMETERS vcl_hrd_parameters;

		unsigned int overscan_info_present_flag:1;
		unsigned int overscan_appropriate_flag:1;
		unsigned int video_signal_type_present_flag:1;
		unsigned int video_format:3;	/* 3 bits */
		unsigned int video_full_range_flag:1;
		unsigned int colour_description_present_flag:1;
		unsigned int low_delay_hrd_flag:1;
		unsigned int pic_struct_present_flag:1;
		unsigned int bitstream_restriction_flag:1;
		unsigned int motion_vectors_over_pic_boundaries_flag:1;

		unsigned long max_bytes_per_pic_denom;
		unsigned long max_bits_per_mb_denom;
		unsigned long log2_max_mv_length_horizontal;
		unsigned long log2_max_mv_length_vertical;
		unsigned long num_reorder_frames;
		unsigned long max_dec_frame_buffering;
		long reserved1;
	} TAVCBD_VUI_PARAMETERS;

/** buffering_period SEI structure.
 */
	typedef struct {
		long initial_cpb_removal_delay[AVCBD_MAX_CPB_CNT];	/* 24bits by default. */
		long initial_cpb_removal_delay_offset[AVCBD_MAX_CPB_CNT];	/* 24bits by default. */
	} TAVCBD_SEI_INITIAL_CPB;

	typedef struct {
		char seq_parameter_set_id;	/* [0,31] */
		TAVCBD_SEI_INITIAL_CPB nal_hrd;
		TAVCBD_SEI_INITIAL_CPB vcl_hrd;
	} TAVCBD_SEI_BUFFERING_PERIOD;

/** Sub-block of pic_timing().
 */
	typedef struct {
		char clock_timestamp_flag;
		char ct_type;
		char nuit_field_based_flag;
		char counting_type;
		char full_timestamp_flag;
		char discontinuity_flag;
		char cnt_dropped_flag;
		unsigned char n_frames;
		char seconds_flag;
		char minutes_flag;
		char hours_flag;
		char seconds_value;
		char minutes_value;
		char hours_value;
		long time_offset;	/* 24 bits by default. */
	} TAVCBD_SEI_PIC_TIMING_TIMESTAMP;

/** pic_timing SEI structure.
 */
	typedef struct {
		char cpb_dpb_delays_present_flag;	/* set 1 if necessary. otherwise, set zero. */
		char pic_struct;
		long cpb_removal_delay;	/* 24bits by default. */
		long dpb_output_delay;	/* 24bits by default. */
		TAVCBD_SEI_PIC_TIMING_TIMESTAMP
		    timestamp[AVCBD_MAX_CLOCK_TS];
	} TAVCBD_SEI_PIC_TIMING;

/** pan_scan_rect SEI structure.
 */
	typedef struct {
		char pan_scan_rect_cancel_flag;
		char pan_scan_cnt_minus1;
		short pan_scan_rect_repetition_period;
		unsigned long pan_scan_rect_id;
		struct {
			signed long left, right, top, bottom;
		} offset[AVCBD_MAX_PAN_SCAN_CNT];	/* pan-scan rect offset. */
	} TAVCBD_SEI_PAN_SCAN_RECT;

/** recovery_point SEI structure.
 */
	typedef struct {
		unsigned long recovery_frame_cnt;
		char exact_match_flag;
		char broken_link_flag;
		char changing_slice_group_idc;
	} TAVCBD_SEI_RECOVERY_POINT;

/** dec_ref_pic_marking structure.
 */
	typedef struct {
		unsigned char no_output_of_prior_pics_flag;
		unsigned char long_term_reference_flag;
		unsigned char adaptive_ref_pic_marking_mode_flag;
		int detect_op5;
		struct {
			unsigned long op;	/* zero means end of list. */
			unsigned long arg1, arg2;
		} memory_management_control[16];
	} TAVCBD_MARKING;

/** dec_ref_pic_marking_repetition SEI structure.
 */
	typedef struct {
		unsigned long original_frame_num;
		char original_idr_flag;
		char original_field_pic_flag;
		char original_bottom_field_flag;
		TAVCBD_MARKING marking;
	} TAVCBD_SEI_DEC_REF_PIC_MARKING_REPETITION;

	typedef struct {
		TAVCBD_SEI_BUFFERING_PERIOD buffering_period;
		TAVCBD_SEI_PIC_TIMING pic_timing;
		TAVCBD_SEI_PAN_SCAN_RECT pan_scan_rect;
		TAVCBD_SEI_RECOVERY_POINT recovery_point;
		TAVCBD_SEI_DEC_REF_PIC_MARKING_REPETITION
		    marking_repetition;
	} TAVCBD_SEI;

/** pointer definition to user function sei_payload().
 *\param type is last_payload_type.
 *\param size is last_payload_size.
 *\param data points the sei_payload() data stream.
 *\return Shall be size of the read data.
 */
	typedef long (*TAVCBD_SEI_PAYLOAD_FUNC) (void *context,
						 unsigned long type,
						 unsigned long size,
						 unsigned char *data);
	typedef void (*TAVCBD_MP4_USERDATA_CONTI_FUNC) (long);
	extern TAVCBD_SEI_PAYLOAD_FUNC avcbd_sei_payload;
	extern TAVCBD_MP4_USERDATA_CONTI_FUNC avcbd_mp4_userdata;

	long avcbd_init_memory_optional(void *context,
					unsigned long buffer_type,
					void *buffer, long size);

#ifdef __cplusplus
}
#endif
#endif				/* _AVCBD_OPTIONALDATA_H_ */
