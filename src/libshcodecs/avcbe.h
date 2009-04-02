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

/**\file avcbe.h
 *\brief Definitions, Structures, and ProtoTypes for User-Appllication using the MPEG-4 Video Encoder.
 */


#ifndef	AVCBE_H
#define	AVCBE_H

#ifndef VPU4
#define	VPU4	1
#endif				/* VPU4 */


/* switch ON/OFF */
#define AVCBE_OFF 			    0
#define AVCBE_ON 			    1

/* use_dquant */
#define AVCBE_I_OFF_P_ON 		2	/* for the avcbe_use_dquant encoding parameter(only MPEG-4) */
#define AVCBE_DQ_ACT 			3

/* stream type */
#define AVCBE_MPEG4				0
#define AVCBE_H263				1
#define AVCBE_H264				2

/* error_resilience_mode */
#define AVCBE_ERM_NORMAL		0
#define AVCBE_ERM_VIDEO_PACKET	1

/* mv_mode */
#define AVCBE_WITHOUT_UMV		0
#define AVCBE_WITH_UMV			1

/* fcode_forward */
#define AVCBE_MVR_FCODE_1		1
#define AVCBE_MVR_FCODE_2		2

/* search_mode */
#define AVCBE_MVM_WEIGHT_1		1
#define AVCBE_MVM_WEIGHT_2		2
#define AVCBE_MVM_WEIGHT_3		3
#define AVCBE_MVM_WEIGHT_4		4
#define AVCBE_MVM_WEIGHT_5		5
#define AVCBE_MVM_WEIGHT_6		6
#define AVCBE_MVM_WEIGHT_7		7
#define AVCBE_MVM_WEIGHT_8		8
#define AVCBE_MVM_WEIGHT_9		9
#define AVCBE_MVM_WEIGHT_10		10

/* high_quality */
#define AVCBE_HQ_NORMAL			0
#define AVCBE_HQ_QUALITY		1
#define AVCBE_HQ_FRAME			2
#define AVCBE_HQ_FAST			3

/* deblocking_mode */
#define AVCBE_DBF_EXC_FRAME_EDGE		0
#define AVCBE_DBF_NONE					1
#define AVCBE_DBF_EXC_SLICE_EDGE		2

/* frame_size */
#define	AVCBE_CIF				0
#define	AVCBE_SIF				1
#define	AVCBE_QCIF				2
#define	AVCBE_SQCIF				3
#define	AVCBE_QVGA				4
#define	AVCBE_16VGA				5
#define	AVCBE_OTHER_SIZE		6

/* ratecontrol_mode */
#define	AVCBE_RATE_NORMAL			1
#define	AVCBE_RATE_QUALITY			2
#define	AVCBE_RATE_FRAME			3
#define	AVCBE_RATE_NO_SKIP			10

/* auto/manual */
#define AVCBE_AUTO				1
#define AVCBE_MANUAL			2

/* calling type of the avcbe_encode_picture function when use slice(H.264 only) */
#define AVCBE_CALL_PER_PIC		1
#define AVCBE_CALL_PER_NAL		2

				    /* video_format *//* changed at Version2 */
#define AVCBE_VIDEO_FORMAT_COMPONENT		0
#define AVCBE_VIDEO_FORMAT_PAL				1
#define AVCBE_VIDEO_FORMAT_NTSC				2
#define AVCBE_VIDEO_FORMAT_SECAM			3
#define AVCBE_VIDEO_FORMAT_MAC				4
#define AVCBE_VIDEO_FORMAT_UNSPECIFIED		5

/* aspect ratio info value(MPEG-4 only) */
#define M4VSE_ASPECT_RATIO_INFO_PAL				2
#define M4VSE_ASPECT_RATIO_INFO_NTSC			3

/* user_data(MPEG-4 only) */
#define	AVCBE_UD_VOS			0
#define	AVCBE_UD_VO				1
#define	AVCBE_UD_VOL			2
#define	AVCBE_UD_GVOP			3
#define AVCBE_MAX_UDATA_SIZE	256

/* MPEG-4 Profile & Level(MPEG-4 only) */
#define	AVCBE_SIMPLE_LEVEL1		1
#define	AVCBE_SIMPLE_LEVEL2		2
#define	AVCBE_SIMPLE_LEVEL3		3
#define	AVCBE_SIMPLE_LEVEL0		8

#define	AVCBE_ADVANCED_SIMPLE_LEVEL0		240
#define	AVCBE_ADVANCED_SIMPLE_LEVEL1		241
#define	AVCBE_ADVANCED_SIMPLE_LEVEL2		242
#define	AVCBE_ADVANCED_SIMPLE_LEVEL3		243
#define	AVCBE_ADVANCED_SIMPLE_LEVEL4		244
#define	AVCBE_ADVANCED_SIMPLE_LEVEL5		245

/* H.264 Profile(H.264 only) */
#define AVCBE_H264_PROFILE_BASELINE		66
#define AVCBE_H264_PROFILE_MAIN			77
#define AVCBE_H264_PROFILE_EXTENDED		88

/* H.264 Level(H.264 only) */
#define AVCBE_H264_LEVEL1		10
#define AVCBE_H264_LEVEL1_1		11
#define AVCBE_H264_LEVEL1_2		12
#define AVCBE_H264_LEVEL1_3		13
#define AVCBE_H264_LEVEL2		20
#define AVCBE_H264_LEVEL2_1		21
#define AVCBE_H264_LEVEL2_2		22
#define AVCBE_H264_LEVEL3		30
#define AVCBE_H264_LEVEL3_1		31
#define AVCBE_H264_LEVEL3_2		32
#define AVCBE_H264_LEVEL4		40
#define AVCBE_H264_LEVEL4_1		41
#define AVCBE_H264_LEVEL4_2		42
#define AVCBE_H264_LEVEL5		50
#define AVCBE_H264_LEVEL5_1		51

/* MPEG-4 object-type(MPEG-4 only) */
#define	AVCBE_OBJECT_TYPE_LISTED_IN_TABLE_9_1		1
#define	AVCBE_OBJECT_TYPE_LISTED_IN_TABLE_AMD2_13	5

#define	AVCBE_SIMPLE_OBJECT_TYPE			1
#define	AVCBE_CORE_OBJECT_TYPE				3
#define	AVCBE_MAIN_OBJECT_TYPE				4
#define	AVCBE_ADVANCED_SIMPLE_OBJECT_TYPE	17

/* set_intra */
#define AVCBE_ANY_VOP				0	/* output P-VOP or I_VOP(according to m4vse_I_vop_interval) */
#define AVCBE_FORCE_I_VOP			1	/* output I-VOP(or I-Picture) */
#define AVCBE_EMPTY_VOP				4	/* output Empty-VOP(VideoObjectPlane's vop_coded=0)(MPEG-4 only) */
#define AVCBE_EMPTY_BLOCK			8	/* output Empty-VOP(macroblock's not_coded=1, VideoObjectPlane's vop_coded=1)(MPEG-4 only) */
#define AVCBE_FORCE_I_VOP_THIS_CAPT	16	/* output I-VOP(after B-VOP)(MPEG-4 only) */
#define AVCBE_FORCE_IDR_VOP			32	/* output IDR-Picture(H.264 only) */

/* operation of change_enc_param */
#define AVCBE_ENCODING_RESET			1	/* reset rate-control */
#define AVCBE_CHANGE_ENCODING_PROPERTY	2	/* change bitrate etc. */

/* output_type of the avcbe_encode_picture function  */
#define AVCBE_OUTPUT_NONE		0	/* no output */
#define AVCBE_OUTPUT_VOL		1	/* output Video_Object_Layer_Header(MPEG-4 only) */
#define AVCBE_OUTPUT_VOS		2	/* output Visual_Object_Sequence_Header(MPEG-4 only) */
#define AVCBE_OUTPUT_SPS		3	/* output Sequence_Header(H.264 only) */
#define AVCBE_OUTPUT_PPS		4	/* output Sequence_Header(H.264 only) */
#define AVCBE_OUTPUT_SLICE		5	/* output Sequence_Header(H.264 only) */

/* output_type of the avcbe_put_end_code function */
#define AVCBE_END_OF_STRM		1	/* output End_Of_Stream (H.264 only) */
#define AVCBE_END_OF_SEQ		2	/* output End_Of_Sequence (H.264 only) */
#define AVCBE_FILLER_DATA		3	/* output Filler_Data (H.264 only) */
#define AVCBE_VOSE				11	/* output VOSE (MPEG-4) or EndMarker (H.263) */

/* status of FRAME_CHECK */
#define AVCBE_UNLOCK			0	/* not using */
#define AVCBE_LOCK				1	/* using */

/* return-value of the avcbe_encode_picture() function */
#define AVCBE_PPS_OUTPUTTED					7	/* Picture_Parameter_Set has been outputted (H.264 only) */
#define AVCBE_SPS_OUTPUTTED					6	/* Sequence_Parameter_Set has been outputted (H.264 only) */
#define AVCBE_SLICE_REMAIN					5	/* Slice data to be encoded remains (H.264 only) */
#define AVCBE_B_VOP_OUTPUTTED				4	/* B-VOP has been outputted (MPEG-4 only) */
#define AVCBE_EMPTY_VOP_OUTPUTTED			2	/* Empty-VOP has been outputted (MPEG-4 only) */
#define AVCBE_FRAME_SKIPPED					1	/* Frame skipped */
#define AVCBE_ENCODE_SUCCESS				0	/* Success */
#define AVCBE_ENCODE_ERROR					-1	/* Error (parameter value is bad) */
#define AVCBE_NOT_IN_ORDER_ERROR			-3	/* Error (Function call not-in-order error) */
#define AVCBE_OUTPUT_BUFFER_SHORT_ERROR		-4	/* Error (The size of the stream-output buffer is short) */
#define AVCBE_VPU_ERROR_AFTER_ENCODING		-6	/* Error (VPU4 is unusual status after encoding) */
#define AVCBE_TEMPORARY_BUFFER_SHORT_ERROR	-8	/* Error (m4iph_temporary_buff_size < stream_buff_size) */
#define AVCBE_VUI_PARAMETERS_NOT_SPECIFIED_ERROR	-9	/* Error (The avcbe_set_vui_parameters function is not called)(H.264 only) */

#define AVCBE_QUANT_MATRIX_NOT_SPECIFIED_ERROR	-11	/* Error( The avcbe_set_quant_type1 function is not called)(MPEG-4 only) */
#define AVCBE_WORK_AREA_SHORT_ERROR			-99	/* Error (The size of work-field area is short) */

/* frame_type */
#define AVCBE_I_VOP			1	/* (MPEG-4,H.263 only) */
#define AVCBE_P_VOP			2	/* (MPEG-4,H.263 only) */
#define AVCBE_B_VOP			3	/* (MPEG-4 only) */
#define AVCBE_IDR_PIC		11	/* (H.264 only) */
#define AVCBE_I_PIC			12	/* (H.264 only) */
#define AVCBE_P_PIC			13	/* (H.264 only) */
#define AVCBE_SPS			14	/* (H.264 only) */
#define AVCBE_PPS			15	/* (H.264 only) */

/* Compute algorithm for Quantization */
#define AVCBE_RESPECT_BITRATE 0
#define AVCBE_RESPECT_QUALITY 1
#define AVCBE_RESPECT_OLD_RATE_CONTROL	10	/* (MPEG-4,H.263 only)  */

/* me_skip_mode (H.264 only) */
#define AVCBE_ME_FORCE_SKIP_NO_MXD	0
#define AVCBE_ME_FORCE_SKIP_LOW		1
#define AVCBE_ME_FORCE_SKIP_MID		2
#define AVCBE_ME_FORCE_SKIP_HIGH	3

/* quant_type (MPEG-4 only) */
#define AVCBE_QUANTISATION_TYPE_1	1	/* MPEG-1 type */
#define AVCBE_QUANTISATION_TYPE_2	2	/* H.263 type */

/* weightedQ_mode */
#define AVCBE_WEIGHTEDQ_NONE		0
#define AVCBE_WEIGHTEDQ_CENTER		1
#define AVCBE_WEIGHTEDQ_RECT		2
#define AVCBE_WEIGHTEDQ_AUTO		20
#define AVCBE_WEIGHTEDQ_BY_USER		21

/* avcbe_zone_size */
#define AVCBE_WQ_HIGH_ZONE_WIDE		1
#define AVCBE_WQ_HIGH_ZONE_MIDDLE	2
#define AVCBE_WQ_HIGH_ZONE_NARROW	3

/* avcbe_Qweight_range */
#define AVCBE_WQ_QWEIGHT_RANGE_1	1
#define AVCBE_WQ_QWEIGHT_RANGE_2	2
#define AVCBE_WQ_QWEIGHT_RANGE_3	3
#define AVCBE_WQ_QWEIGHT_RANGE_4	4
#define AVCBE_WQ_QWEIGHT_RANGE_5	5

/* avcbe_zone2_Qweight_type */
#define AVCBE_WQ_QWEIGHT_SAME_AS_ZONE1	0
#define AVCBE_WQ_QWEIGHT_SMALLER_1		1
#define AVCBE_WQ_QWEIGHT_SMALLER_2		2
#define AVCBE_WQ_QWEIGHT_SMALLER_3		3
#define AVCBE_WQ_QWEIGHT_SMALLER_4		4
#define AVCBE_WQ_QWEIGHT_SMALLER_5		5

/* weight_type */
#define AVCBE_WQ_ENCODE_MODE		1
#define AVCBE_WQ_Q_VALUE			2
#define AVCBE_WQ_BOTH				3

/* slice_type_value_pattern (H.264 only) */
#define AVCBE_SLICE_TYPE_VALUE_LOW		1
#define AVCBE_SLICE_TYPE_VALUE_HIGH		2

/* avcbe_constraint_set_flag (H.264 only) */
#define AVCBE_H264_CONSTRAINT_SET0      0x1
#define AVCBE_H264_CONSTRAINT_SET1      0x2
#define AVCBE_H264_CONSTRAINT_SET2      0x4

/* sei_type of the avcbe_set_SEI_parameters function */
#define AVCBE_SEI_MESSAGE_BUFFERING_PERIOD		1
#define AVCBE_SEI_MESSAGE_PIC_TIMING			2
#define AVCBE_SEI_MESSAGE_PAN_SCAN_RECT			3
#define AVCBE_SEI_MESSAGE_FILLER_PAYLOAD		4
#define AVCBE_SEI_MESSAGE_USER_DATA_REGISTERED_ITU_T_T35	5
#define AVCBE_SEI_MESSAGE_USER_DATA_UNREGISTERED	6
#define AVCBE_SEI_MESSAGE_RECOVERY_POINT		7
#define AVCBE_SEI_MESSAGE_DEC_REF_PIC_MARKING_REPETITION	8
#define AVCBE_SEI_MESSAGE_SPARE_PIC	9
#define AVCBE_SEI_MESSAGE_SCENE_INFO	10
#define AVCBE_SEI_MESSAGE_SUB_SEQ_INFO	11
#define AVCBE_SEI_MESSAGE_SUB_SEQ_LAYER_CHARACTERISTICS		12
#define AVCBE_SEI_MESSAGE_SUB_SEQ_CHARACTERISTICS		13
#define AVCBE_SEI_MESSAGE_FULL_FRAME_FREEZE		14
#define AVCBE_SEI_MESSAGE_FULL_FRAME_FREEZE_RELEASE		15
#define AVCBE_SEI_MESSAGE_FULL_FRAME_SNAPSHOT	16
#define AVCBE_SEI_MESSAGE_PROGRESSIVE_REFINEMENT_SEGMENT_START	17
#define AVCBE_SEI_MESSAGE_PROGRESSIVE_REFINEMENT_SEGMENT_END	18
#define AVCBE_SEI_MESSAGE_MOTION_CONSTRAINED_SLICE_GROUP_SET	19

/* VPU4 error code (MPEG-4 or H.264) */
#define AVCBE_ERRE_AVC_PCM_ZERO		0xFA04	/* H.264 only */
#define AVCBE_ERRE_VOPSIZE			0xFF00	/* MPEG-4, H.264 */
#define AVCBE_ERRE_COEF_CLIP		0xFF10	/* MPEG-4, H.264 */

/* VPU4 warning flag code (MPEG-4 or H.264) */
#define AVCBE_PCM_ZERO					 0x00000001	/* H.264 only */
#define AVCBE_VOPSIZE					 0x00000002	/* MPEG-4, H.264 */
#define AVCBE_COEF_CLIP					 0x00000004	/* MPEG-4, H.264 */
#define AVCBE_EXCEEDED_BITRATE			 0x00000008	/* MPEG-4, H.264 */
#define AVCBE_CPB_UNDER_FLOW_NON_SKIP	 0x00000010	/* MPEG-4, H.264 */

/* primary_pic_type of AU delimiter (H.264 only) */
#define AVCBE_ONLY_I	1
#define AVCBE_I_AND_P	2
#define AVCBE_ARIB		3


/*----- structures -----*/
typedef struct {
	unsigned long stream_type;
	void *streamp;
} avcbe_stream_info;

/*----- avcbe_encoding_property (common parameter for MPEG-4 & H.264) -----*/
typedef struct {
	long avcbe_stream_type;	/* (1)  */
	long avcbe_bitrate;	/* (2)  */
	long avcbe_xpic_size;	/* (3)  */
	long avcbe_ypic_size;	/* (4)  */
	long avcbe_frame_rate;	/* (5)  */
	long avcbe_I_vop_interval;	/* (6)  */
	long avcbe_mv_mode;	/* (7)  */
	long avcbe_fcode_forward;	/* (8)  */
	long avcbe_search_mode;	/* (9)  */
	long avcbe_search_time_fixed;	/* (10) */

	long avcbe_ratecontrol_skip_enable;	/* (11) */
	long avcbe_ratecontrol_use_prevquant;	/* (12) */
	long avcbe_ratecontrol_respect_type;	/* (13) */

	long avcbe_ratecontrol_intra_thr_changeable;	/* (14) */
	long avcbe_control_bitrate_length;	/* (15) */
	long avcbe_intra_macroblock_refresh_cycle;	/* (16) */

	long avcbe_video_format;	/* (17) */
	long avcbe_frame_num_resolution;	/* (18) */
	long avcbe_noise_reduction;	/* (19) */
	long avcbe_reaction_param_coeff;	/* (20) */
	long avcbe_weightedQ_mode;	/* (21) */

} avcbe_encoding_property;


/*----- avcbe_other_options_h264 (H.264 Only) -----*/
typedef struct {
	unsigned long avcbe_Ivop_quant_initial_value;	/* (1)  */
	unsigned long avcbe_Pvop_quant_initial_value;	/* (2)  */
	unsigned long avcbe_use_dquant;	/* (3)  */
	unsigned long avcbe_clip_dquant_next_mb;	/* (4)  */
	unsigned long avcbe_clip_dquant_frame;	/* (5)  */
	unsigned long avcbe_quant_min;	/* (6)  */
	unsigned long avcbe_quant_min_Ivop_under_range;	/* (7)  */
	unsigned long avcbe_quant_max;	/* (8)  */

	unsigned long avcbe_ratecontrol_cpb_skipcheck_enable;	/* (9) */
	unsigned long avcbe_ratecontrol_cpb_Ivop_noskip;	/* (10) */
	unsigned long avcbe_ratecontrol_cpb_remain_zero_skip_enable;	/* (11) */

	unsigned long avcbe_ratecontrol_cpb_buffer_unit_size;	/* (12)    */
	unsigned long avcbe_ratecontrol_cpb_buffer_mode;	/* (13) */
	unsigned long avcbe_ratecontrol_cpb_max_size;	/* (14) */
	unsigned long avcbe_ratecontrol_cpb_offset;	/* (15) */
	unsigned long avcbe_ratecontrol_cpb_offset_rate;	/* (16) */

	unsigned long avcbe_intra_thr_1;	/* (17) */
	unsigned long avcbe_intra_thr_2;	/* (18) */
	unsigned long avcbe_sad_intra_bias;	/* (19) */

	unsigned long avcbe_regularly_inserted_I_type;	/* (20) */
	unsigned long avcbe_call_unit;	/* (21) */
	unsigned long avcbe_use_slice;	/* (22) */
	unsigned long avcbe_slice_size_mb;	/* (23) */
	unsigned long avcbe_slice_size_bit;	/* (24) */
	unsigned long avcbe_slice_type_value_pattern;	/* (25) */
	unsigned long avcbe_use_mb_partition;	/* (26) */
	unsigned long avcbe_mb_partition_vector_thr;	/* (27) */
	unsigned long avcbe_deblocking_mode;	/* (28) */
	unsigned long avcbe_use_deblocking_filter_control;	/* (29) */
	long avcbe_deblocking_alpha_offset;	/* (30) */
	long avcbe_deblocking_beta_offset;	/* (31) */

	unsigned long avcbe_me_skip_mode;	/* (32) */
	unsigned long avcbe_put_start_code;	/* (33) */
	unsigned long avcbe_param_changeable;	/* (34) */
	unsigned long avcbe_changeable_max_bitrate;	/* (35) */

	/* SequenceHeaderParameter */
	unsigned long avcbe_seq_param_set_id;	/* (36) */
	unsigned long avcbe_profile;	/* (37) */
	unsigned long avcbe_constraint_set_flag;	/* (38) */
	unsigned long avcbe_level_type;	/* (39) */
	unsigned long avcbe_level_value;	/* (40) */
	unsigned long avcbe_out_vui_parameters;	/* (41) */

	/* PictureHeaderParameter */
	long avcbe_chroma_qp_index_offset;	/* (42) */
	unsigned long avcbe_constrained_intra_pred;	/* (43) */


} avcbe_other_options_h264;


/*----- avcbe_other_options_mpeg4 (MPEG-4,H.263 only) -----*/
typedef struct {
	unsigned long avcbe_out_vos;	/* (1)  */
	unsigned long avcbe_out_gov;	/* (2)  */
	unsigned long avcbe_aspect_ratio_info_type;	/* (3)  */
	unsigned long avcbe_aspect_ratio_info_value;	/* (4)  */

	unsigned long avcbe_vos_profile_level_type;	/* (5)  */
	unsigned long avcbe_vos_profile_level_value;	/* (6)  */
	unsigned long avcbe_out_visual_object_identifier;	/* (7)  */
	unsigned long avcbe_visual_object_verid;	/* (8)  */
	unsigned long avcbe_visual_object_priority;	/* (9)  */
	unsigned long avcbe_video_object_type_indication;	/* (10) */
	unsigned long avcbe_out_object_layer_identifier;	/* (11) */
	unsigned long avcbe_video_object_layer_verid;	/* (12) */
	unsigned long avcbe_video_object_layer_priority;	/* (13) */

	unsigned long avcbe_error_resilience_mode;	/* (14) */
	unsigned long avcbe_video_packet_size_mb;	/* (15) */
	unsigned long avcbe_video_packet_size_bit;	/* (16) */
	unsigned long avcbe_video_packet_header_extention;	/* (17) */
	unsigned long avcbe_data_partitioned;	/* (18) */
	unsigned long avcbe_reversible_vlc;	/* (19) */
	unsigned long avcbe_high_quality;	/* (20) */
	unsigned long avcbe_param_changeable;	/* (21) */
	unsigned long avcbe_changeable_max_bitrate;	/* (22) */

	unsigned long avcbe_Ivop_quant_initial_value;	/* (23) */
	unsigned long avcbe_Pvop_quant_initial_value;	/* (24) */
	unsigned long avcbe_use_dquant;	/* (25) */
	unsigned long avcbe_clip_dquant_frame;	/* (26) */
	unsigned long avcbe_quant_min;	/* (27) */
	unsigned long avcbe_quant_min_Ivop_under_range;	/* (28) */
	unsigned long avcbe_quant_max;	/* (29) */

	unsigned long avcbe_ratecontrol_vbv_skipcheck_enable;	/* (30) */
	unsigned long avcbe_ratecontrol_vbv_Ivop_noskip;	/* (31) */
	unsigned long avcbe_ratecontrol_vbv_remain_zero_skip_enable;	/* (32) */

	unsigned long avcbe_ratecontrol_vbv_buffer_unit_size;	/* (33) */
	unsigned long avcbe_ratecontrol_vbv_buffer_mode;	/* (34) */
	unsigned long avcbe_ratecontrol_vbv_max_size;	/* (35) */
	unsigned long avcbe_ratecontrol_vbv_offset;	/* (36) */
	unsigned long avcbe_ratecontrol_vbv_offset_rate;	/* (37) */

	unsigned long avcbe_quant_type;	/* (38) */
	unsigned long avcbe_use_AC_prediction;	/* (39) */
	unsigned long avcbe_vop_min_mode;	/* (40) */
	unsigned long avcbe_vop_min_size;	/* (41) */
	unsigned long avcbe_intra_thr;	/* (42) */
	unsigned long avcbe_b_vop_num;	/* (43) */

} avcbe_other_options_mpeg4;


typedef struct {
	unsigned long area_size;	/* size of work-field area (byte unit) */
	unsigned char *area_top;	/* top address of work-field area */
} TAVCBE_WORKAREA;


typedef struct {
	unsigned char *Y_fmemp;
	unsigned char *C_fmemp;
} TAVCBE_FMEM;

typedef struct {
	unsigned long buff_size;	/* size of stream-output buffer (byte unit) */
	unsigned char *buff_top;	/* top address of stream-output buffer */
} TAVCBE_STREAM_BUFF;

typedef struct {
	unsigned long mem1;
	unsigned long mem2;
} TAVCBE_VUI_PARAM;


/* MPEG-4, H.263 only */
typedef struct {
	long avcbe_FrmN;	/* (1) */
	long avcbe_frm_interval;	/* (2) */
	long avcbe_bitrate;	/* (3) */
	long avcbe_quant;	/* (4) */
	long avcbe_frame_n_bits;	/* (5) */
	long avcbe_frame_type;	/* (6) */
	long avcbe_warning_flag;	/* (7) *//* this code is returned from VPU4(It is supported a cord in a bit.) */
	/* 0x2 : The value of encoded-bits of one picture or one slice was beyond 16Mbit. */
	/* 0x4 : A coefficient was done a clip of to the sake maximum beyond the maximum. */
	/* 0x8 : The value of encoded-bits in appointed section (avcbe_control_bitrate_length) was over. */
} avcbe_frame_stat;


/* H.264 only */
typedef struct {
	char avcbe_encoded_pic_type;	/* (1) *//* A kind of the NAL which was encoded. */
	long avcbe_total_MB_in_frame;	/* (2) *//* The number of macroblocks in 1 frame */
	long avcbe_SPS_unit_bits;	/* (3) *//* The number of bits of Sequence Parameter Set which was outputed. */
	long avcbe_SPS_unit_bytes;	/* (4) *//* The number of bytes of Sequence Parameter Set which was outputed. */
	long avcbe_PPS_unit_bits;	/* (5) *//* The number of bits of Picture Parameter Set which was outputed. */
	long avcbe_PPS_unit_bytes;	/* (6) *//* The number of bytes of Picture Parameter Set which was outputed. */
	long avcbe_AU_type;	/* (7) *//* A kind of Access Unit Delimiter which was encoded. */
	long avcbe_AU_unit_bytes;	/* (8) *//* The number of bytes of Access Unit Delimiter which was outputed. */
	long avcbe_slice_header_bits;	/* (9) *//* The number of bits of Slice Header which was outputed. Only Slice unit return */
	long avcbe_encoded_slice_bits;	/* (10) *//* A total of the number of the bits of the slice header which was encoded just before that. */
	long avcbe_inserted_3byte_bytes;	/* (11) *//* The number of bytes of Emulation_prevention_3_byte which was outputed. */
	long avcbe_encoded_MB_num;	/* (12) *//* The number of macroblocks which was encoded. */
	long avcbe_quant;	/* (13) *//* A quantization coefficient of NAL (slice) which was encoded just before that. */
	long avcbe_warning_flag;	/* (14) *//* this code is returned from VPU4(It is supported a cord in a bit.)  */
	/* 0x1 : In the case of I-PCM, it was replaced a value of 0x00 with 0x01 in a pixel value of a stream.(H.264 only) */
	/* 0x2 : The value of encoded-bits of one picture or one slice was beyond 16Mbit. */
	/* 0x4 : A coefficient was done a clip of to the sake maximum beyond the maximum. */
	/* 0x8 : The value of encoded-bits in appointed section (avcbe_control_bitrate_length) was over. */

} avcbe_slice_stat;

/* H.264 only */
/* structure for the avcbe_get_header_information() function */
typedef struct {
	long avcbe_profile_idc;
	long avcbe_constraint_set0_flag;
	long avcbe_constraint_set1_flag;
	long avcbe_constraint_set2_flag;
	long avcbe_level_idc;
	long avcbe_seq_param_set_ID;
	long avcbe_log2_max_frame_num_minus4;
	long avcbe_pic_order_cnt_type;
	long avcbe_log2_max_pic_order_cnt_lsb_minus4;
	long avcbe_delta_pic_order_always_zero_flag;
	long avcbe_offset_for_non_ref_pic;
	long avcbe_offset_for_top_to_bottom_field;
	long avcbe_num_ref_frames_in_pic_order_cnt_cycle;
	long avcbe_offset_for_ref_frame[256];
	long avcbe_num_ref_frames;
	long avcbe_gaps_in_frame_num_value_allowed_flag;
	long avcbe_pic_width_in_mbs_minus1;
	long avcbe_pic_height_in_map_units_minus1;
	long avcbe_frame_mbs_only_flag;
	long avcbe_mb_adaptive_frame_field_flag;
	long avcbe_direct_8x8_inference_flag;
	long avcbe_frame_cropping_flag;
	long avcbe_frame_crop_left_offset;
	long avcbe_frame_crop_right_offset;
	long avcbe_frame_crop_top_offset;
	long avcbe_frame_crop_bottom_offset;
	long avcbe_vui_parameters_present_flag;
	long avcbe_SPS_header_bits;
} avcbe_SPS_header_info_stat;

typedef struct {
	long avcbe_pic_parameter_set_ID;
	long avcbe_seq_param_set_ID;
	long avcbe_entropy_coding_mode_flag;
	long avcbe_pic_order_present_flag;
	long avcbe_num_slice_groups_minus1;
	long avcbe_num_ref_idx_l0_active_minus1;
	long avcbe_num_ref_idx_l1_active_minus1;
	long avcbe_weighted_pred_flag;
	long avcbe_weighted_bipred_idc;
	long avcbe_pic_init_qp_minus26;
	long avcbe_pic_init_qs_minus26;
	long avcbe_chroma_qp_index_offset;
	long avcbe_deblocking_filter_control_present_flag;
	long avcbe_constrained_intra_pred_flag;
	long avcbe_redundant_pic_cnt_present_flag;
	long avcbe_PPS_header_bits;
} avcbe_PPS_header_info_stat;

typedef struct {
	long avcbe_first_mb_in_slice;
	long avcbe_slice_type;
	long avcbe_pic_parameter_set_id;
	long avcbe_frame_num;
	long avcbe_prev_frame_num;

	long avcbe_idr_pic_id;
	long avcbe_pic_order_cnt_lsb;
	long avcbe_delta_pic_order_cnt[2];

	long avcbe_num_ref_idx_active_override_flag;
	long avcbe_slice_qp_delta;
	long avcbe_disable_deblocking_filter_idc;
	long avcbe_slice_alpha_c0_offset_div2;
	long avcbe_slice_beta_offset_div2;
	long avcbe_ref_pic_list_reordering_flag_l0;
	long avcbe_reordering_of_pic_nums_idc;
	long avcbe_abs_diff_pic_num_minus1;
	long avcbe_no_output_od_prior_pics_flag;
	long avcbe_long_term_reference_flag;
	long avcbe_adaptive_ref_pic_making_mode_flag;

	long avcbe_vop_type;
	long avcbe_deblock_disable;
	long avcbe_vop_quant;
	long avcbe_slice_next_quant;

	long avcbe_use_multi_reference;
	long *avcbe_stream_out;
	long avcbe_slice_header_total_bits;
	long avcbe_slice_total_bits_for_frame;
	long avcbe_outputed_continue_bits;
	long avcbe_last_slice_data_continue_bits;
	long avcbe_used_continue_in_slice;
} avcbe_slice_header_info_stat;

/* structure for the avcbe_set_weightedQ() function (1) when avcbe_weightedQ_mode=AVCBE_WEIGHTEDQ_CENTER */
typedef struct {
	long avcbe_zone_size;	/* (1) */
	long avcbe_Qweight_range;	/* (2) */
} TAVCBE_WEIGHTEDQ_CENTER;

/* structure for the avcbe_set_weightedQ() function (2)-1 when avcbe_weightedQ_mode=AVCBE_WEIGHTEDQ_RECT */
typedef struct {
	long avcbe_left_column;	/* (1) */
	long avcbe_top_row;	/* (2) */
	long avcbe_right_column;	/* (3) */
	long avcbe_bottom_row;	/* (4) */
} TAVCBE_WEIGHTEDQ_RECT_POSITION;

/* structure for the avcbe_set_weightedQ() function (2)-2 when avcbe_weightedQ_mode=AVCBE_WEIGHTEDQ_RECT */
typedef struct {
	long avcbe_zone_num;	/* (1) */
	TAVCBE_WEIGHTEDQ_RECT_POSITION avcbe_zone1_pos;	/* (2) */
	TAVCBE_WEIGHTEDQ_RECT_POSITION avcbe_zone2_pos;	/* (3) */
	long avcbe_zone1_Qweight_range;	/* (4) */
	long avcbe_zone2_Qweight_type;	/* (5) */
} TAVCBE_WEIGHTEDQ_RECT;


/* structure for the avcbe_set_VUI_parameters function (H.264 only) */
typedef struct {
	long avcbe_bit_rate_value_minus1;	/* (1)  */
	long avcbe_cpb_size_value_minus1;	/* (2)  */
	long avcbe_cbr_flag;	/* (3)  */
} avcbe_vui_schedsel_param;


/* structure for the avcbe_set_VUI_parameters function (H.264 only) */
typedef struct {
	long avcbe_cpb_cnt_minus1;	/* (1)  */
	long avcbe_bit_rate_scale;	/* (2)      */
	long avcbe_cpb_size_scale;	/* (3)      */
	avcbe_vui_schedsel_param avcbe_schedsel_table[32];	/* (4)  */
	long avcbe_initial_cpb_removal_delay_length_minus1;	/* (5)  */
	long avcbe_cpb_removal_delay_length_minus1;	/* (6)  */
	long avcbe_dpb_output_delay_length_minus1;	/* (7)  */
	long avcbe_time_offset_length;	/* (8)  */
} avcbe_vui_hrd_param;


/* structure for the avcbe_set_VUI_parameters function (H.264 only) */
typedef struct {
	long avcbe_aspect_ratio_info_present_flag;	/* (1)  */
	long avcbe_aspect_ratio_idc;	/* (2)  */
	long avcbe_sar_width;	/* (3)  */
	long avcbe_sar_height;	/* (4)  */

	long avcbe_overscan_info_present_flag;	/* (5)  */
	long avcbe_overscan_appropriate_flag;	/* (6)  */

	long avcbe_video_signal_type_present_flag;	/* (7)  */
	long avcbe_video_full_range_flag;	/* (8)  */
	long avcbe_colour_description_present_flag;	/* (9)  */
	long avcbe_colour_primaries;	/* (10) */
	long avcbe_transfer_characteristics;	/* (11) */
	long avcbe_matrix_coefficients;	/* (12) */

	long avcbe_chroma_loc_info_present_flag;	/* (13) */
	long avcbe_chroma_sample_loc_type_top_field;	/* (14) */
	long avcbe_chroma_sample_loc_type_bottom_field;	/* (15) */

	long avcbe_timing_info_present_flag;	/* (16) */
	long avcbe_num_units_in_tick;	/* (17) */
	long avcbe_time_scale;	/* (18) */
	long avcbe_fixed_frame_rate_flag;	/* (19) */

	long avcbe_nal_hrd_parameters_present_flag;	/* (20) */
	avcbe_vui_hrd_param avcbe_nal_hrd_param;	/* (21) */

	long avcbe_vcl_hrd_parameters_present_flag;	/* (22) */
	avcbe_vui_hrd_param avcbe_vcl_hrd_param;	/* (23) */

	long avcbe_low_delay_hrd_flag;	/* (24) */

	long avcbe_pic_struct_present_flag;	/* (25) */

	long avcbe_bitstream_restriction_present_flag;	/* (26) */
	long avcbe_motion_vectors_over_pic_boundaries_flag;	/* (27) */
	long avcbe_max_bytes_per_pic_denom;	/* (28) */
	long avcbe_max_bits_per_mb_denom;	/* (29) */
	long avcbe_log2_max_mv_length_horizontal;	/* (30) */
	long avcbe_log2_max_mv_length_vertical;	/* (31) */
	long avcbe_num_reorder_frames;	/* (32) */
	long avcbe_max_dec_frame_buffering;	/* (33) */
} avcbe_vui_main_param;


/* structure for the avcbe_set_SEI_parameters function (BUFFERING_PERIOD) (H.264 only) */
typedef struct {
	unsigned long avcbe_initial_cpb_removal_delay;	/* (1)  */
	unsigned long avcbe_initial_cpb_removal_delay_offset;	/* (2)  */
} avcbe_sei_delay_table;


/* structure for the avcbe_set_SEI_parameters function (BUFFERING_PERIOD) (H.264 only) */
typedef struct {
	unsigned long avcbe_seq_parameter_set_id;	/* (1)  */
	avcbe_sei_delay_table avcbe_NalHrdBp[1];	/* (2)  */
	avcbe_sei_delay_table avcbe_VclHrdBp[1];	/* (3)  */
} avcbe_sei_buffering_period_param;


/* structure for the avcbe_set_SEI_parameters function (PIC_TIMING) (H.264 only) */
typedef struct {
	unsigned long avcbe_clock_timestamp;	/* (1)  */
	unsigned long avcbe_ct_type;	/* (2)  */
	unsigned long avcbe_nuit_field_based_flag;	/* (3)  */
	unsigned long avcbe_counting_type;	/* (4)  */
	unsigned long avcbe_full_timestamp_flag;	/* (5)  */
	unsigned long avcbe_discontinuity_flag;	/* (6)  */
	unsigned long avcbe_cnt_dropped_flag;	/* (7)  */
	unsigned long avcbe_n_frames;	/* (8)  */
	unsigned long avcbe_seconds_flag;	/* (9)  */
	unsigned long avcbe_seconds_value;	/* (10) */
	unsigned long avcbe_minutes_flag;	/* (11) */
	unsigned long avcbe_minutes_value;	/* (12) */
	unsigned long avcbe_hours_flag;	/* (13) */
	unsigned long avcbe_hours_value;	/* (14) */
	long avcbe_time_offset;	/* (15) */
} avcbe_sei_clockts_table;


/* structure for the avcbe_set_SEI_parameters function (PIC_TIMING) (H.264 only) */
typedef struct {
	unsigned long avcbe_cpb_removal_delay;	/* (1)  */
	unsigned long avcbe_dpb_output_delay;	/* (2)  */

	unsigned long avcbe_pic_struct;	/* (3)  */
	avcbe_sei_clockts_table avcbe_clockts[3];	/* (4)  */
} avcbe_sei_pic_timing_param;


/* structure for the avcbe_set_SEI_parameters function (PAN_SCAN_RECT) (H.264 only) */
typedef struct {
	long avcbe_pan_scan_rect_left_offset;	/* (1)  */
	long avcbe_pan_scan_rect_right_offset;	/* (2)  */
	long avcbe_pan_scan_rect_top_offset;	/* (3)  */
	long avcbe_pan_scan_rect_bottom_offset;	/* (4)  */
} avcbe_sei_rect_offset_table;


/* structure for the avcbe_set_SEI_parameters function (PAN_SCAN_RECT) (H.264 only) */
typedef struct {
	unsigned long avcbe_pan_scan_rect_id;	/* (1)  */
	unsigned long avcbe_pan_scan_rect_cancel_flag;	/* (2)  */
	unsigned long avcbe_pan_scan_cnt_minus1;	/* (3)  */
	avcbe_sei_rect_offset_table avcbe_rect_offset[3];	/* (4)  */
	unsigned long avcbe_pan_scan_rect_repetition_period;	/* (5)      */
} avcbe_sei_pan_scan_rect_param;


/* structure for the avcbe_set_SEI_parameters function (FILLER_PAYLOAD) (H.264 only) */
typedef struct {
	unsigned long avcbe_filler_payload_size;	/* (1) */
} avcbe_sei_filler_payload_param;


/* structure for the avcbe_set_SEI_parameters function (RECOVERY_POINT) (H.264 only) */
typedef struct {
	unsigned long avcbe_recovery_frame_cnt;	/* (1)  */
	unsigned long avcbe_exact_match_flag;	/* (2)  */
	unsigned long avcbe_broken_link_flag;	/* (3)  */
	unsigned long avcbe_changing_slice_group_idc;	/* (4)  */
} avcbe_sei_recovery_point_param;


/* structure for the avcbe_change_enc_param function */
typedef struct {
	long avcbe_bitrate;	/* (1)  */
	long avcbe_frame_rate;	/* (2)  */
	long avcbe_I_vop_interval;	/* (3)  */
} avcbe_property_after_change;


/* structure for the avcbe_get_buffer_check function (MPEG-4 only) */
typedef struct {
	long avcbe_status;	/* (1) */
	long avcbe_frame_no;	/* (2) */
	char *avcbe_frame_pointer;	/* (3) */
} AVCBE_FRAME_CHECK;

/* structure for the avcbe_set_quant_type1 funcion (MPEG-4 only) */
typedef struct {
	char avcbe_intra_qmatrix[64];	/* (1) */
	char avcbe_inter_qmatrix[64];	/* (2) */
} avcbe_quant_type1_matrix;



#ifdef __cplusplus
extern "C" {
#endif

/*----- prototype -----*/
/** API functions **/
	void avcbe_start_encoding(void);

	long avcbe_set_default_param(long stream_type,
				     unsigned long ratecontrol_mode,
				     avcbe_encoding_property * param,
				     void *other_options);

	typedef void (*avcbe_buf_continue_userproc_ptr) (avcbe_stream_info
							 * context,
							 unsigned char
							 *previous_stream_buff,
							 long output_size,
							 unsigned char
							 **next_stream_buff,
							 long
							 *stream_buff_size);

	long avcbe_init_encode(avcbe_encoding_property * param,
			       avcbe_encoding_property * paramR,
			       void *other_options,
			       avcbe_buf_continue_userproc_ptr
			       avcbe_buf_continue_userproc,
			       TAVCBE_WORKAREA * workarea_info,
			       TAVCBE_WORKAREA * dp_workarea_info,
			       avcbe_stream_info ** context);

	long avcbe_init_memory(avcbe_stream_info * context,
			       unsigned long nrefframe,
			       unsigned long nldecfmem,
			       TAVCBE_FMEM ldecfmemp[], long ldecwx,
			       long ldecwy);

	long avcbe_set_image_pointer(avcbe_stream_info * context,
				     TAVCBE_FMEM * captfmemp,
				     unsigned long ldec,
				     unsigned long ref1,
				     unsigned long ref2);

	long avcbe_encode_picture(avcbe_stream_info * context, long frm,
				  long set_intra, long output_type,
				  TAVCBE_STREAM_BUFF * stream_buff,
				  TAVCBE_STREAM_BUFF * extra_stream_buff);

	long avcbe_put_end_code(avcbe_stream_info * context,
				TAVCBE_STREAM_BUFF * stream_buff,
				unsigned long output_type);

	long avcbe_put_filler_data(TAVCBE_STREAM_BUFF * stream_buff,
				   long out_start_code, long output_size);

	long avcbe_set_weightedQ(avcbe_stream_info * context,
				 long weightedQ_enable, void *weightedQ);

	long avcbe_get_backup(avcbe_stream_info * context,
			      TAVCBE_WORKAREA * backup);

	long avcbe_set_backup(avcbe_stream_info * context,
			      TAVCBE_WORKAREA * backup);

	long avcbe_get_last_frame_stat(avcbe_stream_info * context,
				       avcbe_frame_stat * fstat);

	long avcbe_get_frame_size(avcbe_stream_info * context, long *xs,
				  long *ys);

	unsigned long avcbe_get_version(void);

	long avcbe_get_last_slice_stat(avcbe_stream_info * context,
				       avcbe_slice_stat * slice_stat);

	long avcbe_get_header_information(avcbe_stream_info * context,
					  void *header_stat,
					  long output_type);

	long avcbe_set_VUI_parameters(avcbe_stream_info * context,
				      avcbe_vui_main_param * vui_param);

	long avcbe_put_SEI_parameters(avcbe_stream_info * context,
				      long sei_type, void *sei_param,
				      TAVCBE_STREAM_BUFF * stream_buff);

	long avcbe_change_enc_param(avcbe_stream_info * context,
				    long operation,
				    avcbe_property_after_change *
				    param_after_change);

	long avcbe_clear_user_data(avcbe_stream_info * context,
				   long area_mode);

	long avcbe_set_user_data(avcbe_stream_info * context,
				 unsigned char *usrdata, long area_mode);

	long avcbe_set_gov_time(avcbe_stream_info * context, long tH,
				long tM, long tS);

	long avcbe_get_buffer_check(avcbe_stream_info * context,
				    AVCBE_FRAME_CHECK * parray);

	long avcbe_set_quant_type1(avcbe_stream_info * context,
				   avcbe_quant_type1_matrix *
				   quant_matrix);

	long avcbe_get_cpb_buffer_size(avcbe_stream_info * context);

	long avcbe_calc_cpb_buff_offset(long bitrate,
					long max_cpb_buff_size, long rate);

	long avcbe_calculate_initQ(long stream_type, long bitrate,
				   long xpic_size, long ypic_size,
				   long frame_rate);

/* the functions which are coded by user */
	unsigned long avcbe_insert_filler_data_for_cpb_buffer(unsigned long
							      input_filler_size);

#ifdef __cplusplus
}
#endif
#endif				/* ABCBE_H */
