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

/****************************************************************************
*	Modification history													*
*	Ver.1.00	2004/02/06	R.Imaoka (register spec. Rev.01-00 based)		*
*	Ver.1.10	2004/04/20	M.Nagata										*
*	Ver.1.11	2004/05/24	M.Nagata										*
*	Ver.1.12	2004/06/30	M.Nagata										*
*	Ver.1.13	2004/07/28	M.Nagata										*
*	Ver.1.14	2004/08/18	M.Nagata										*
*	Ver.1.15	2004/09/17	M.Nagata										*
*	Ver.1.16	2004/09/24	M.Nagata										*
*	Ver.1.17	2004/11/09	M.Nagata										*
*	Ver.1.18	2004/12/08	M.Nagata										*
*	Ver.1.19	2005/01/07	M.Nagata										*
*	Ver.1.20	2005/01/20	M.Nagata										*
*	Ver.1.21	2005/02/10	M.Nagata										*
*	Ver.1.22	2005/03/02	M.Nagata										*
*	Ver.1.23	2005/03/04	M.Nagata										*
*	Ver.1.24	2005/03/08	M.Nagata										*
*	Ver.1.25	2005/03/11	M.Nagata										*
*	Ver.1.26	2005/03/16	M.Nagata										*
*	Ver.1.27	2005/03/16	M.Nagata										*
*	Ver.1.28	2005/04/01	M.Nagata										*
*	Ver.2.00	2005/04/27	N.Maruo (VPU4SÂÐ±þ)								*
****************************************************************************/

#ifndef	M4IPH_VPU4_H
#define	M4IPH_VPU4_H

#define	M4IPH_VPU4_VERSION	2000 /* Ver.2.0 because Phase 2 */	/* 050426 */

/* definition of retern value */
#define M4IPH_OK			0	/* success */
#define M4IPH_PAR			-1	/* parameter error */
#define M4IPH_VPU_ENCODING	1	/* VPU status encoding */
#define M4IPH_VPU_DECODING	2	/* VPU status decoding */
#define M4IPH_VPU_PROCESSING	-256	/* VPU status processing */

/* definition of interrupt block */
#define M4IPH_VPU4_NE	0x00000001	/* normal end */
#define M4IPH_VPU4_EE	0x00000002	/* error end */
#define M4IPH_VPU4_SBE	0x00000004	/* stream buffer end */
#define M4IPH_VPU4_EDT	0x00000100	/* hardware error detect */

/* definition of image size and the numbers of macroblocks */
#define M4IPH_MAX_WIDTH			720	/* max image width for VPU4 */
#define M4IPH_MAX_HEIGHT		576	/* max image height for VPU4 */

#define M4IPH_MAX_WIDTH_4S			1280					/* max image width for VPU4S */	/* 050425 */
#define M4IPH_MAX_HEIGHT_4S			1024					/* max image height for VPU4S */	/* 050425 */

#define M4IPH_MIN_WIDTH_DEC		16	/* min image width for decoding */
#define M4IPH_MIN_HEIGHT_DEC	16	/* min image height for decoding */
#define M4IPH_MIN_WIDTH_ENC		48	/* min image width for encoding */
#define M4IPH_MIN_HEIGHT_ENC	48	/* min image height for encoding */
#define M4IPH_MAX_MB			(M4IPH_MAX_WIDTH*M4IPH_MAX_HEIGHT/16/16)	/* max image width */
#define M4IPH_MAX_MB_4S			(M4IPH_MAX_WIDTH_4S*M4IPH_MAX_HEIGHT_4S/16/16)	/* max MBs for VPU4S */	/* 050425 */

/* stream type */
#define M4IPH_MPEG4		0	/* MPEG-4 */
#define M4IPH_H263		1	/* H.263 */
#define M4IPH_H264		2	/* H.264 */

/* processinng mode */
#define M4IPH_VPU_ENC_MODE	0	/* encode */
#define M4IPH_VPU_DEC_MODE	1	/* decode */

/* Not Coded RAM size of VPU4 */
#define M4IPH_VP4_NC_MAX_APL	51	/* size of 'NC' (MAX_MB/8/4) */

/* Not Coded RAM size of VPU4S */
#define M4IPH_VP4_NC_MAX_APL_4S	160			/* size of 'NC + NCE' (MAX_MB_4S/8/4) */	/* 050520 maruo */

/* definition of VPU4's encoding result code */
enum { M4IPH_VPU4_SUCCESS = 0,	/* success(normal end) */
	M4IPH_VPU4_ERROR_END = 1,	/* error(error end) */
	M4IPH_VPU4_STREAM_BUFFER_SHORT_ERROR = 2,	/* error(stream buffer end) */
	M4IPH_VPU4_ERROR_DETECT = 3	/* error(hardware error detect) */
};

/* definition of parameter's ON/OFF */
#define M4IPH_ON 1		/* ON */
#define M4IPH_OFF 0		/* OFF */

/* definition of clock control */
#define M4IPH_CTL_BLOCK_UNIT  0
#define M4IPH_NOT_CTL         1
#define M4IPH_CTL_FRAME_UNIT  2

typedef struct {
	unsigned long m4iph_vpu_base_address;	/* VPU base address */
	long m4iph_vpu_endian;	/* VPU endian */
	long m4iph_vpu_interrupt_enable;	/* enable VPU interrupt */
	long m4iph_vpu_clock_supply_control;	/* 0:BlockUnit, 1:NotControl, 2:FrameUnit */
	long m4iph_vpu_mask_address_disable;	/* disable mask address */
	unsigned long m4iph_temporary_buff_address;	/* address of temporary buffer */
	unsigned long m4iph_temporary_buff_size;	/* size of temporary buffer */
} M4IPH_VPU4_INIT_OPTION;


typedef struct {
	/* VP4_CTRL */
	long m4iph_jpeg_format;	/* JPEG format */
	long m4iph_error_mode;	/* error mode */
	long m4iph_VLD_thr_mode;	/* enable VLD through mode */
	long m4iph_stream_type;	/* stream type */

	/* VP4_VOL_CTRL */
	long m4iph_vop_time_inc_res;	/* vop time increment resolution */
	long m4iph_ignr_colocated_not_coded;	/* DON'T use colocated_not_coded flag */
	long m4iph_use_ref_select_code;	/* use ref_select_code flag */
	long m4iph_low_delay;	/* low delay */
	long m4iph_reversible_VLC;	/* reversible VLC */
	long m4iph_data_partitioned;	/* data partitioned */
	long m4iph_resync_marker;	/* resync marker */
	long m4iph_quant_type;	/* quant type */
	long m4iph_interlaced;	/* interlaced */

	/* VP4_IMAGE_SIZE */
	long m4iph_img_width;	/* image width */
	long m4iph_img_height;	/* image height */

	/* VP4_DWY_ADDR */
	long m4iph_decode_write_frame_addr_Y;	/* decoded frame address for luminance Y */
	/* VP4_DWC_ADDR */
	long m4iph_decode_write_frame_addr_C;	/* decoded frame address for chrominance C */
	/* VP4_D2WY_ADDR */
	long m4iph_decode_write_frame_addr_FY;	/* filter out frame address for luminance Y */
	/* VP4_D2WC_ADDR */
	long m4iph_decode_write_frame_addr_FC;	/* filter out frame address for chrominance C */

	/* VP4_DP1_ADDR */
	long m4iph_data_partition_addr_1;	/* address of 1st data-partitioned information */
	/* VP4_DP2_ADDR */
	long m4iph_data_partition_addr_2;	/* address of 2nd data-partitioned information */

	/* VP4_STRS_ADDR */
	long m4iph_strm_start_addr;	/* stream buffer start address */
	/* VP4_STRE_ADDR */
	long m4iph_strm_end_addr;	/* stream buffer end address */

	/* VP4_VOP_CTRL */
	long m4iph_vop_quant;	/* vop quant(slice qp for h.264) */
	long m4iph_vop_coding_type;	/* vop_coding_type */
	long m4iph_vop_rounding_type;	/* vop_rounding_type for VLD through mode */
	long m4iph_vop_coded;	/* vop_coded for VLD through mode */

	/* VP4_VOP_TIME */
	long m4iph_modulo_time_base;	/* modulo_time_base for VLD through mode */
	long m4iph_vop_time_inc;	/* vop_time_increment for VLD through mode */

	/* VP4_263_CTRL */
	long m4iph_force_mpeg4_pmv;	/* use MPEG-4 type PMV at H.263 */
	long m4iph_gfid_first_slice;	/* enable GFID in first slice */
	long m4iph_op_ptype;	/* Optional Ptype */
	long m4iph_plus_ptype;	/* PlusPtype */
	long m4iph_annex_t;	/* enable H.263 Annex T */
	long m4iph_annex_k;	/* enable H.263 Annex K */
	long m4iph_annex_j;	/* enable H.263 Annex J */
	long m4iph_annex_i;	/* enable H.263 Annex I */

	/* VP4_264_CTRL */
	long m4iph_avc_qp_c_offset;	/* qp_c_offset */
	long m4iph_avc_first_mb_in_slice;	/* first_mb_in_slice */
	long m4iph_avc_ref_index_last;	/* ref_index_last */
	long m4iph_avc_dec_end_point;	/* decode end point flag */
	long m4iph_avc_bit_offset;	/* bit offset @ slice_data */

	/* VP4_VLC_CTRL */
	long m4iph_cncl_ref_filter_mode;	/* conceal filter mode for MB which referes to concealed block */
	long m4iph_cncl_filter_mode;	/* conceal filter mode */
	long m4iph_complexity_estimation_header;	/* bit number of complexity estimation header */
	long m4iph_tframe_ignore;	/* ignore TFRAME */
	long m4iph_err_cncl_mode;	/* error conceal mode */
	long m4iph_mb_info_out;	/* enable macroblock information output */

	/* VP4_CMD */
	long m4iph_continue_mode;	/* enable continue */

	/* VP4_MC_CTRL */
	long m4iph_field_chroma_mode;	/* field chroma mode */
	long m4iph_avc_beta_offset;	/* beta offset */
	long m4iph_avc_alpha_offset;	/* alpha offset */
	long m4iph_avc_deblock_filter_mode;	/* deblocking filter mode */
	long m4iph_forced_deblock_filter;	/* forced deblocking filter */

	/* VP4_PRED_CTRL */
	long m4iph_idct_mode;	/* IDCT calculation mode */
	long m4iph_avc_constrained_intra_pred;	/* constrained_intra_pred */

	/* VP4_VLC_CLK */
	long m4iph_custom_pic_clock_freq_enable;	/* custom picture clock frequency */

	/* VP4_FWD_TIME */
	long m4iph_fwd_modulo_time_base;	/* modulo_time_base of forward reference */
	long m4iph_fwd_time_inc;	/* vop_time_increment of forward reference */

	/* VP4_BWD_TIME */
	long m4iph_bwd_modulo_time_base;	/* modulo_time_base of backward reference */
	long m4iph_bwd_time_inc;	/* vop_time_increment of backward reference */

	/* VP4_PST_TIME */
	long m4iph_present_time_enable;	/* enable the setting of m4iph_present_modulo_time_base and m4iph_present_vop_time_inc */
	long m4iph_present_modulo_time_base;	/* present modulo_time_base */
	long m4iph_present_vop_time_inc;	/* present vop_time_increment */

	/* VP4_ILTFRAME */
	long m4iph_tframe_modulo_time_base;	/* modulo_time_base of TFRAME */
	long m4iph_tframe_time_inc;	/* vop_time_increment of TFRAME */

	/* VP4_DEBUG */
	long m4iph_debug_mode;	/* debug mode */
	long m4iph_debug_status;	/* debug status */

	/* VP4_EC_REF */
	long m4iph_ref_cncl_start_x;	/* X-axis start position of concealed reference image */
	long m4iph_ref_cncl_start_y;	/* Y-axis start position of concealed reference image */
	long m4iph_ref_cncl_end_x;	/* X-axis end position of concealed reference image */
	long m4iph_ref_cncl_end_y;	/* Y-axis end position of concealed reference image */

	/* REFERENCE FRAME ADDR */
	long m4iph_r00y_addr;	/* forward reference frame address for luminance Y at MPEG-4 */
	long m4iph_r00c_addr;	/* forward reference frame address for chrominance C at MPEG-4 */
	long m4iph_r01y_addr;	/* backward reference frame address for luminance Y at MPEG-4 */
	long m4iph_r01c_addr;	/* backward reference frame address for chrominance C at MPEG-4 */
	long m4iph_r02y_addr;
	long m4iph_r02c_addr;
	long m4iph_r03y_addr;
	long m4iph_r03c_addr;
	long m4iph_r04y_addr;
	long m4iph_r04c_addr;
	long m4iph_r05y_addr;
	long m4iph_r05c_addr;
	long m4iph_r06y_addr;
	long m4iph_r06c_addr;
	long m4iph_r07y_addr;
	long m4iph_r07c_addr;
	long m4iph_r08y_addr;
	long m4iph_r08c_addr;
	long m4iph_r09y_addr;
	long m4iph_r09c_addr;
	long m4iph_r10y_addr;
	long m4iph_r10c_addr;
	long m4iph_r11y_addr;
	long m4iph_r11c_addr;
	long m4iph_r12y_addr;
	long m4iph_r12c_addr;
	long m4iph_r13y_addr;
	long m4iph_r13c_addr;
	long m4iph_r14y_addr;
	long m4iph_r14c_addr;
	long m4iph_r15y_addr;
	long m4iph_r15c_addr;
} M4IPH_VPU4_DEC;


typedef struct {
	/* VP4_VOP_CTRL */
	long m4iph_ref_select_code;	/* ref_select_code */
	long m4iph_intra_dc_VLC_thr;	/* intra_dc_vlc_thr */
	long m4iph_vop_quant;	/* vop_quant */
	long m4iph_vop_coding_type;	/* vop_coding_type */
	long m4iph_vop_roudnig_type;	/* vop_rounding_type */
	long m4iph_vop_coded;	/* vop_coded */
	long m4iph_top_field_first;	/* top_field_first */
	long m4iph_alternate_vertical_scan;	/* alternate_vertical_scan */
	long m4iph_vop_fcode_fwd;	/* vop_fcode_forward */
	long m4iph_vop_fcode_bwd;	/* vop_fcode_backward */

	/* VP4_VOP_TIME */
	long m4iph_modulo_time_base;	/* modulo_time_base */
	long m4iph_vop_time_inc;	/* vop_time_increment */

	/* VP4_263_CTRL */
	long m4iph_optional_mode;	/* Optional Mode Flag */
	long m4iph_op_ptype;	/* Optional Ptype */
	long m4iph_plus_ptype;	/* enable PlusPtype */
	long m4iph_split_screen_indicator;	/* split_screen_indicator */
	long m4iph_doc_camera_indicator;	/* document_camera_indicator */
	long m4iph_full_pic_freeze_release;	/* full_picture_freeze_release */
	long m4iph_source_format;	/* source format */
	long m4iph_gob_frame_id;	/* gob frame id */

	/* VP4_VLC_PIC */
	long m4iph_par_code;	/* PAR code */
	long m4iph_pic_width_indicator;	/* Picture Width Indicator */
	long m4iph_pic_height_indicator;	/* Picture Height Indicator */

	/* VP4_VLC_CLK */
	long m4iph_ext_pixel_aspect_ratio_width;	/* Extended Pixel Aspect Ratio */
	long m4iph_ext_pixel_aspect_ratio_height;	/* Extended Pixel Aspect Ratio */
	long m4iph_custom_pic_clock_conv_code;	/* Custom Picture Clock Conversion Code */
	long m4iph_clock_divider;	/* Clock Divider */

	/* VP4_VOP_BIT */
	long m4iph_vop_bits;	/* vop_bits */

	/* VP4_DEC_ERR */
	long m4iph_dec_err_code;	/* decode-error code */
	long m4iph_dec_err_position;	/* decode-error position */

	/* VP4_NEXT_CODE */
	long m4iph_next_start_code;	/* next start code */

	/* VP4_ERR_AREA */
	long m4iph_cncl_start_x;	/* X-axis start position of error concealed area */
	long m4iph_cncl_start_y;	/* Y-axis start position of error concealed area */
	long m4iph_cncl_end_x;	/* X-axis end position of error concealed area */
	long m4iph_cncl_end_y;	/* Y-axis end position of error concealed area */

	/* VP4_SLC_MB */
	long m4iph_decoded_mbs;	/* decoded MBs */
	long m4iph_vpu_status_flag;	/* vpu status flag */
} M4IPH_VPU4_DEC_RES;


typedef struct {
	/* VP4_CTRL */
	long m4iph_jpeg_rotation;	/* JPEG rotation */
	long m4iph_jpeg_format;	/* JPEG format */
	long m4iph_stream_type;	/* stream type */

	/* VP4_VOL_CTRL */
	long m4iph_vop_time_inc_res;	/* vop_time_increment_resolution */
	long m4iph_ignr_colocated_not_coded;	/* DON'T use colocated_not_coded flag */
	long m4iph_use_ref_select_code;	/* use ref_select_code flag */
	long m4iph_low_delay;	/* low delay */
	long m4iph_reversible_VLC;	/* reversible VLC */
	long m4iph_data_partitioned;	/* data partitioned */
	long m4iph_resync_marker;	/* resync marker */
	long m4iph_quant_type;	/* quant type */
	long m4iph_interlaced;	/* interlaced */

	/* VP4_IMAGE_SIZE */
	long m4iph_img_width;	/* image width */
	long m4iph_img_height;	/* image height */

	/* NC RAM */
	long m4iph_enc_frame_addr_Y;	/* encode capture frame address for luminance Y */
	long m4iph_enc_frame_addr_C;	/* encode capture frame address for chrominance C */

	/* VP4_DWY_ADDR */
	long m4iph_decode_write_frame_addr_Y;	/* decoded frame address for luminance Y */
	/* VP4_DWC_ADDR */
	long m4iph_decode_write_frame_addr_C;	/* decoded frame address for chrominance C */

	/* VP4_DP2_ADDR */
	long m4iph_data_partition_addr_2;	/* address of 2nd data-partitioned information */

	/* VP4_STRS_ADDR */
	long m4iph_strm_start_addr;	/* stream buffer start address */
	/* VP4_STRE_ADDR */
	long m4iph_strm_end_addr;	/* stream buffer end address */

	/* VP4_VOP_CTRL */
	long m4iph_ref_select_code;	/* ref_select_code */
	long m4iph_vop_quant;	/* vop_quant(slice qp for h.264) */
	long m4iph_vop_coding_type;	/* vop_coding_type */
	long m4iph_vop_rounding_type;	/* vop_rounding_type */
	long m4iph_top_field_first;	/* top_field_first for interlace */
	long m4iph_alternate_vertical_scan;	/* enable alternate_vertical_scan for interlace */
	long m4iph_vop_fcode_fwd;	/* vop_fcode_forward */

	/* VP4_VOP_TIME */
	long m4iph_modulo_time_base;	/* modulo_time_base */
	long m4iph_vop_time_inc;	/* vop_time_increment */

	/* VP4_263_CTRL */
	long m4iph_force_mpeg4_pmv;	/* use MPEG-4 type PMV at H.263 */
	long m4iph_gfid_first_slice;	/* enable GFID in first slice */
	long m4iph_op_ptype;	/* Optional Ptype */
	long m4iph_plus_ptype;	/* enable PlusPtype */
	long m4iph_annex_t;	/* enbale H.263 Annex T */
	long m4iph_annex_k;	/* enable H.263 Annex K */
	long m4iph_annex_j;	/* enable H.263 Annex J */
	long m4iph_split_screen_indicator;	/* split_screen_indicator in Ptype */
	long m4iph_doc_camera_indicator;	/* document_camera_indicator in Ptype */
	long m4iph_full_pic_freeze_release;	/* full_picture_freeze_release in Ptype */
	long m4iph_source_format;	/* source format in Ptype */
	long m4iph_gob_frame_id;	/* gob frame id */

	/* VP4_264_CTRL */
	long m4iph_avc_qp_c_offset;	/* qp_c_offset */
	long m4iph_avc_first_mb_in_slice;	/* first_mb_in_slice */
	long m4iph_avc_ref_index_last;	/* ref_index_last */
	long m4iph_avc_i_pcm_enable;	/* enable I-PCM */
	long m4iph_avc_prevent_eml_three_byte;	/* enable to insert prevention emulation three byte */
	long m4iph_avc_bit_offset;	/* bit offset @ slice_data */

	/* VP4_VLC_CTRL */
	long m4iph_header_extension;	/* header extension */
	long m4iph_mb_info_out;	/* enable macroblock information output */

	/* VP4_CMD */
	long m4iph_continue_mode;	/* enable continue */

	/* VP4_ME_TH1 */
	long m4iph_intra_th1;	/* intra_th1 */
	long m4iph_intra_th2;	/* intra_th2 */

	/* VP4_ME_TH2 */
	long m4iph_intra_bias;	/* intra_bias */
	long m4iph_noise_reduction_param;	/* noize reduction parameter */

	/* VP4_ME_COST */
	long m4iph_cost_mv_multiplier;	/* MB partition cost smvl */
	long m4iph_cost_Q_th1;	/* MB partition cost thq1 */
	long m4iph_cost_Q_th2;	/* MB partition cost thq2 */
	long m4iph_cost_Q_th3;	/* MB partition cost thq3 */
	long m4iph_cost_Q_th4;	/* MB partition cost thq4 */
	long m4iph_cost_Q_th5;	/* MB partition cost thq5 */

	/* VP4_ME_SKIP */
	long m4iph_avc_skip_mode;	/* forced skip mxd */
	long m4iph_avc_skip_th1;	/* forced skip th1 */
	long m4iph_avc_skip_th2;	/* forced skip th2 */
	long m4iph_avc_skip_th3;	/* forced skip th3 */
	long m4iph_avc_skip_th4;	/* forced skip th4 */
	long m4iph_avc_skip_th5;	/* forced skip th5 */

	/* VP4_ME_CTRL */
	long m4iph_search_fixed_step;	/* fixed search times */
	long m4iph_search_mode;	/* search mode */
	long m4iph_search_time;	/* search times */
	long m4iph_cost_enable;	/* enable MB partition cost */
	long m4iph_avc_mb_part_mode;	/* MB partition mode */
	long m4iph_umv;		/* enable unrestricted motion vector */
	long m4iph_force_fwd;	/* enable forced forward prediction */
	long m4iph_force_bwd;	/* enable forced backward prediction */
	long m4iph_intra_refresh;	/* enable intra-refresh */
	long m4iph_intra_decision_mode;	/* intra/inter decision mode */

	/* VP4_MBRF_CTRL */
	long m4iph_intra_refresh_start_mb;	/* intra-refresh start position(macroblock#) */
	long m4iph_intra_refresh_end_mb;	/* intra-refresh end position(macroblock#) */

	/* VP4_MC_CTRL */
	long m4iph_field_chroma_mode;	/* field chroma mode */
	long m4iph_avc_beta_offset;	/* beta offset */
	long m4iph_avc_alpha_offset;	/* alpha offset */
	long m4iph_avc_deblock_filter_mode;	/* deblocking filter mode */
	long m4iph_output_ldec_img_bvop;	/* output local decode image of B-VOP */

	/* VP4_PRED_CTRL */
	long m4iph_idct_mode;	/* IDCT calculation mode */
	long m4iph_avc_ext_intra_pred_offset;	/* use MAT_RAM offset */
	long m4iph_avc_constrained_intra_pred;	/* constrained_intra_pred */
	long m4iph_scan_mode;	/* scan mode */
	long m4iph_ac_pred_disable;	/* disable ac prediction */

	/* VP4_SLC_SIZE */
	long m4iph_video_packet_mb_size;	/* video packet MB size */
	long m4iph_video_packet_size;	/* video packet size */

	/* VP4_VOP_MINBIT */
	long m4iph_vop_min_bits;	/* VOP min bits */

	/* VP4_MB_MAXBIT */
	long m4iph_mb_max_bits;	/* VOP max bits */

	/* VP4_MB_TBIT */
	long m4iph_mb_target_bits;	/* VOP target bits */

	/* VP4_RCQNT */
	long m4iph_quality_mode;	/* quality mode */
	long m4iph_coef_enc_area;	/* coef enc area */
	long m4iph_clipping_dq;	/* clipping delta-Q */
	long m4iph_qcost_enable;	/* enable q cost */
	long m4iph_coef_cut_enable;	/* enable coef cut control */
	long m4iph_dquant_enable;	/* enable dquant control */
	long m4iph_max_quant;	/* max quant value */
	long m4iph_min_quant;	/* min quant value */
	/* VP4_RCRP */
	long m4iph_reaction_param;	/* reaction parameter */

	/* VP4_RCDJ */
	long m4iph_virtual_buf_size;	/* vertical buffer size */

	/* VP4_RCWQ */
	long m4iph_dq_param3;	/* weighted Q param3 */
	long m4iph_dq_param2;	/* weighted Q param2 */
	long m4iph_dq_param1;	/* weighted Q param1 */
	long m4iph_dq_mode3;	/* weighted Q mode3 */
	long m4iph_dq_mode2;	/* weighted Q mode2 */
	long m4iph_dq_mode1;	/* weighted Q mode1 */
	long m4iph_weighted_q_enable;	/* enable weighted Q */

	/* VP4_VLC_PIC */
	long m4iph_par_code;	/* PAR code */
	long m4iph_pic_width_indicator;	/* picture width indicator */
	long m4iph_pic_height_indicator;	/* picture height indicator */
	long m4iph_custom_pic_format;	/* custom picture format */

	/* VP4_VLC_CLK */
	long m4iph_ext_pixel_aspect_ratio_width;	/* Extended Pixel Aspect Ratio(width) */
	long m4iph_ext_pixel_aspect_ratio_height;	/* Extended Pixel Aspect Ratio(height) */
	long m4iph_custom_pic_clock_conv_code;	/* custom picture clock conversion code */
	long m4iph_clock_divider;	/* clock divider */
	long m4iph_custom_pic_clock_freq_enable;	/* enable custom picture clock frequency */

	/* VP4_MQ_CTRL */
	long m4iph_average_activity;	/* average activity *//* 050425 */
	long m4iph_mq_act_enable;	/* modify QP by activity enable *//* 050425 */
	long m4iph_mq_intra_enable;	/* modify QP at Intra_16x16 enable *//* 050425 */

	/* VP4_CTD_CTRL */
	long m4iph_cut_diff_mode;	/* cut diff mode *//* 050425 */
	long m4iph_cut_diff_mv_th;	/* cut diff MV th *//* 050425 */

	/* VP4_CTD_SADTHR */
	long m4iph_cut_diff_sad_th0;	/* cut diff MV th0 *//* 050425 */
	long m4iph_cut_diff_sad_th1;	/* cut diff MV th1 *//* 050425 */

	/* REFERENCE FRAME ADDR */
	long m4iph_r00y_addr;	/* forward reference frame address for luminance Y at MPEG-4 */
	long m4iph_r00c_addr;	/* forward reference frame address for chrominance C at MPEG-4 */
	long m4iph_r01y_addr;	/* backward reference frame address for luminance Y at MPEG-4 */
	long m4iph_r01c_addr;	/* backward reference frame address for chrominance C at MPEG-4 */
} M4IPH_VPU4_ENC;


typedef struct {
	long m4iph_virtual_buf_size_res;	/* virtual buffer size result */
	long m4iph_vop_bits;	/* vop_bits */
	long m4iph_avc_eml_prevent_byte_bits;	/* emulation prevention bytes bits */
	long m4iph_slice_mbs;	/* the number of MBs in slice */
	long m4iph_nextQ;	/* candidate value of next quant value */
	long m4iph_Qsum;	/* the sum of quant value */
	long m4iph_enc_err_code;	/* encode-error code */
	long m4iph_enc_err_position_y;	/* Y-axis encode error position */
	long m4iph_enc_err_position_x;	/* X-axis encode error position */
	long m4iph_intra_mbs;	/* the number of intra macroblocks */
	long m4iph_inter_mbs;	/* the number of inter macroblocks */
	long m4iph_inter_sad;	/* inter sad */
	long m4iph_intra_sad;	/* intra sad */
	long m4iph_vpu_status_flag;	/* vpu status flag */
	long m4iph_activity_sum;	/* sum of activity *//* 050425 */
} M4IPH_VPU4_ENC_RES;

#ifdef __cplusplus
extern "C" {
#endif

/* common functions */
	long m4iph_vpu4_init(M4IPH_VPU4_INIT_OPTION * pOption);
	long m4iph_vpu4_endian(long endian);
	long m4iph_vpu4_reset(void);
	long m4iph_vpu4_clock_stop(long stop_mode);
	long m4iph_vpu4_ie(long vpu4_ie, long int_enable);
	void m4iph_vpu4_ie_set_all(void);
	long m4iph_vpu4_if(long vpu4_if);
	long m4iph_vpu4_is(void);
	long m4iph_vpu4_status(void);
	long m4iph_vpu4_qmat_read(long mode, long component, char *qmat);
	long m4iph_vpu4_qmat_write(long mode, long component, char *qmat);
	long m4iph_vpu4_nc_read(long *nc, long size);
	long m4iph_vpu4_nc_write(long *nc, long size);
	long m4iph_vpu4_wq_read(char *wt_table, long size);
	long m4iph_vpu4_wq_write(char *wt_table, long size);
	long m4iph_vpu4_wq_read_1mb(long mb_num);
	long m4iph_vpu4_wq_write_1mb(long mode, long mb_num);
	long m4iph_vpu4_aip_offset_write(long *aip);
	long m4iph_vpu4_err_det(long count);
	void m4iph_vpu4_int_handler(void);
	long m4iph_vpu4_version(void);
	long m4iph_check_vpu_version(void);	/* 050422 KM */
	unsigned long m4iph_get_vpu_version_flag(void);	/* 050425 KM */
	unsigned long m4iph_get_temporary_buff_address(void);	/* 050425 KM */
	unsigned long m4iph_get_temporary_buff_size(void);	/* 050425 KM */


/* for decode */
	long m4iph_vpu4_decode(M4IPH_VPU4_DEC * pDecOption,
			       M4IPH_VPU4_DEC_RES * pDecResult,
			       long int_off_continue);
	long m4iph_vpu4_dec_res(M4IPH_VPU4_DEC_RES * pResult);
	long m4iph_vpu4_set_dec_default_param(M4IPH_VPU4_DEC * pDecOption,
					      M4IPH_VPU4_DEC_RES *
					      pDecResult);

/* for encode */
	long m4iph_vpu4_encode(M4IPH_VPU4_ENC * pEncOption,
			       M4IPH_VPU4_ENC_RES * pEncResult,
			       long int_off_continue);
	long m4iph_vpu4_enc_res(M4IPH_VPU4_ENC_RES * pEncResult);
	long m4iph_vpu4_set_enc_default_param(long stream_type,
					      M4IPH_VPU4_ENC * pEncOption,
					      M4IPH_VPU4_ENC_RES *
					      pEncResult);

/* the functions which are coded by user */
	void m4iph_start(void);
	long m4iph_sleep(void);
	void m4iph_restart(void);
	void m4iph_reg_table_write(unsigned long *addr,
				   unsigned long *table, long size);
	unsigned long m4iph_reg_table_read(unsigned long *addr,
					   unsigned long *table,
					   long size);
	unsigned long m4iph_sdr_read(unsigned char *address,
				     unsigned char *buffer,
				     unsigned long count);
	void m4iph_sdr_write(unsigned char *address, unsigned char *buffer,
			     unsigned long count);
	long m4iph_dec_continue(void);	/* 050713 */
	long m4iph_enc_continue(long output_bits);	/* 050713 */


	extern volatile long m4iph_vpu_cn_flg;

#ifdef __cplusplus
}
#endif
#endif				/* M4IPH_VPU4_H */
