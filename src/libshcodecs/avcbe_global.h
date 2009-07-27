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

/**\file avcbe_global.h
 *\brief declare Extern of the global variables of the MPEG-4 Video Encoder.
 */

#ifndef	AVCBE_GLOBAL_H
#define	AVCBE_GLOBAL_H


#ifdef __cplusplus
extern "C" {
#endif

	/* エンコードパラメータの上下限チェック用 *//* 041202,041207,050106 */
	extern long avcbe_system_min_bitrate;	/* 内部スペック *//* 050107 */
	extern long avcbe_system_max_bitrate;	/* 内部スペック *//* 050107 */
	extern long avcbe_user_min_bitrate;	/* 外部スペック *//* 050107 */
	extern long avcbe_user_max_bitrate;	/* 外部スペック *//* 050107 */

	extern long avcbe_min_xpic_size;
	extern long avcbe_system_max_xpic_size;	/* 内部スペック *//* 050107 */
	extern long avcbe_user_max_xpic_size;	/* 外部スペック *//* 050107 */

	extern long avcbe_min_ypic_size;
	extern long avcbe_system_max_ypic_size;	/* 内部スペック *//* 050107 */
	extern long avcbe_user_max_ypic_size;	/* 外部スペック *//* 050107 */

	extern long avcbe_min_frame_rate;
	extern long avcbe_max_frame_rate;

	extern long avcbe_min_I_vop_interval;
	extern long avcbe_max_I_vop_interval;

/* 050106 追加したグローバル変数 ここから */
	extern long avcbe_system_min_frame_num_resolution;	/* 内部スペック *//* 050107 */
	extern long avcbe_user_min_frame_num_resolution;	/* 外部スペック *//* 050107 */
	extern long avcbe_system_max_frame_num_resolution;	/* 内部スペック *//* 050309 */
	extern long avcbe_user_max_frame_num_resolution;	/* 外部スペック *//* 050309 */

	extern long avcbe_min_slice_video_packet_size_mb_h264;	/* 050107 */
	extern long avcbe_min_slice_video_packet_size_mb_mpeg4;	/* 050107 */
	extern long avcbe_max_slice_video_packet_size_mb;

	extern long avcbe_min_slice_video_packet_size_bit;
	extern long avcbe_max_slice_video_packet_size_bit;

	extern long avcbe_min_Q_encoder_limit_mpeg4;
	extern long avcbe_max_Q_encoder_limit_mpeg4;

	extern long avcbe_max_clip_dquant_next_mb_h264;
	extern long avcbe_max_clip_dquant_frame_h264;
	extern long avcbe_max_clip_dquant_frame_mpeg4;

	extern long avcbe_g_max_reaction_param_val;	/* 050618 */
	extern long avcbe_g_min_reaction_param_val;	/* 050618 */

/*extern long avcbe_max_vop_min_size_mpeg4; avcbe_vop_min_max_sizeを使うように変更（VPU4とVPU4Sで値が異なる）050525 */

	extern long avcbe_max_intra_thr_1_h264;
	extern long avcbe_max_intra_thr_2_h264;
	extern long avcbe_max_sad_intra_bias_h264;
	extern long avcbe_max_intra_thr_mpeg4;

	extern long avcbe_nc_max_apl;	/* 050426 */
/* 050106 追加したグローバル変数 ここまで */

	extern long avcbe_vop_min_max_size;	/* 0500525 */

/* SPS関係のグローバル変数 */
	extern long avcbe_g_nal_ref_idc;
	extern long avcbe_g_seq_parameter_set_ID;
	extern long avcbe_g_log2_max_frame_num_minus4;
	extern long avcbe_g_pic_order_cnt_type;
	extern long avcbe_g_gaps_in_frame_num_value_allowed_flag;	/* 041104 */
	extern long avcbe_g_log2_max_pic_order_cnt_lsb_minus4;
	extern long avcbe_g_delta_pic_order_always_zero_flag;
	extern long avcbe_g_offset_for_non_ref_pic;
	extern long avcbe_g_offset_for_top_to_bottom_field;
	extern long avcbe_g_num_ref_frames_in_pic_order_cnt_cycle;
	extern long avcbe_g_offset_for_ref_frame[256];	/* 検討要 */
	extern long avcbe_g_num_ref_frames;
	/* extern long avcbe_g_frame_mbs_only_flag; *//* baselineなので常に「1」 *//* 041106 */
	extern long avcbe_g_direct_8x8_inference_flag;	/* 041104 */
	extern long avcbe_g_mb_adaptive_frame_field_flag;
	extern long avcbe_g_vui_parameters_present_flag;

/* PPS関係のグローバル変数 */
	extern long avcbe_g_pic_parameter_set_ID;
	extern long avcbe_g_entropy_coding_mode_flag;	/* 041104 */
	extern long avcbe_g_pic_order_present_flag;	/* 041104 */
	extern long avcbe_g_num_slice_groups_minus1;	/* 041104 */
	extern long avcbe_g_num_ref_idx_l0_active_minus1;
	extern long avcbe_g_deblocking_filter_variables_present_flag;

/* Slice Header関係のグローバル変数 */
	extern long avcbe_g_slice_type;
	extern long avcbe_g_pic_parameter_set_id;
	extern long avcbe_g_frame_num;
	extern long avcbe_g_idr_pic_id;
	extern long avcbe_g_pic_order_cnt_lsb;
	extern long avcbe_g_delta_pic_order_cnt[2];	/* 検討要 */
	extern long avcbe_g_num_ref_idx_active_override_flag;
	extern long avcbe_g_slice_qp_delta;
	extern long avcbe_g_ref_pic_list_reordering_flag_l0;
	extern long avcbe_g_adaptive_ref_pic_making_mode_flag;
	extern long avcbe_g_field_pic_flag;	/* pic timing SEIで使用 *//* 041227 */
	extern long avcbe_g_bottom_field_flag;	/* pic timing SEIで使用 *//* 041227 */

	/* SEI関係のglobal変数 *//* 041214 */
	extern long avcbe_g_changing_slice_group_idc;
	extern long avcbe_g_cpb_removal_delay;	/* 041224 */
	extern long avcbe_g_max_pic_struct;	/* Picure Timing SEIのpic_struct用 *//* 050531 */
	extern long avcbe_g_max_ct_type;	/* Picure Timing SEIのct_type用  Table D-2(ISO/IEC JTC 1/SC 29/WG 11 N6540) *//* 050531 */
	extern long avcbe_g_max_counting_type;	/* Picure Timing SEIのcounting_type用  Table D-3(ISO/IEC JTC 1/SC 29/WG 11 N6540) *//* 050531 */


/* レートコントロール関係のグローバル変数 */
	/*extern unsigned long  avcbe_g_cpb_check_flg; *//* Add 040730 Y.Funabashi */
	/*extern unsigned long  avcbe_g_cpb_buffer_units; *//* Add 040729 Y.Funabashi */
	/*extern float                  avcbe_g_cpb_offset; *//* Add 041206 *//* cpb_init_delayの代わり */
	/*extern float                  avcbe_g_cpb_offset_rate; *//* Add 041213 *//* cpb buffer に対するcpb_lowerの範囲とcpb_minの範囲の比率 */
	/*extern float                  avcbe_g_cpb_stack_rate; *//* Add 050126 *//* cpb buffer に開始時点で貯めておく割合 */
	extern float avcbe_g_cpb_offset_start_rate;	/* 050601 */

	extern unsigned long avcbe_g_skip_limit_max;	/* Add 041207 *//* skip_limitの最大値 */
	extern unsigned long avcbe_g_skip_limit_min;	/* Add 041207 *//* skip_limitの最小値 */
	extern float avcbe_g_add_skip_value;	/* Add 041207 *//* skip_counterの加算値 */
	extern float avcbe_g_sub_skip_value;	/* Add 041207 *//* skip_counterの減算値 */
	/*extern unsigned long  avcbe_g_I_vop_skip_value; *//* Del 050421 *//* I-VOPをスキップした場合の値をセット */
	/*extern unsigned long  avcbe_g_filler_data_size; *//* Del 050428 */
	extern unsigned long avcbe_g_start_bits_factor;	/* Add 040915 Y.Funabashi */
	extern float avcbe_g_total_bits_clear_factor;	/* Add 040916 Y.Funabashi */
	/*extern unsigned long  avcbe_g_cpb_buffer_mode; *//* 041213 */
	/*extern long                           avcbe_g_cpb_max_size; *//* Add 041227 */
/*extern long				avcbe_g_remain_sum;	050427 */
/*extern long				avcbe_g_cpb_buff_per_frm_over_cnt; 050427 */
	extern long avcbe_g_stuffing_insert_vbv_buffer_rate_mpeg4;	/* 050524 */
	extern long avcbe_g_cpb_over_skip_enable_H264;	/* 050524 */
	extern long avcbe_g_vbv_over_skip_enable_mpeg4;	/* 050524 */

/* Qコントロール関係のグローバル変数 */
	extern unsigned long avcbe_g_P_quant_range_value;	/* modify 005124 *//* PフレームのQ値の変動範囲設定 */
	extern unsigned long avcbe_g_P_quant_range_value_mpeg4;	/* MPEG-4専用 *//* Add 050428 */
/*extern long				avcbe_g_1_thr; 050425 */
/*extern long				avcbe_g_2_thr; 050425 */
/*extern long				avcbe_g_3_thr; 050425 */
/*extern long				avcbe_g_4_thr; 050425 */
/*extern long				avcbe_g_5_thr; 050425 */
	extern float avcbe_g_q_change_plus_res4;	/* Add 041227 */
	extern float avcbe_g_q_change_plus_res5;	/* Add 041227 */
	/*extern unsigned long  avcbe_rate_ctrl_type; *//* Add 041208 Y.Funabashi */
/*extern long				avcbe_g_qunat_max_thr1; 050425 */
/*extern long				avcbe_g_qunat_max_thr2; 050425 */
/*extern long				avcbe_g_qunat_max_thr3; 050425 */
/*extern long				avcbe_g_qunat_max_thr4; 050425 */

	extern long avcbe_g_I_quant_offset;	/* Add 050107 */
	extern long avcbe_g_I_quant_offset_range;	/* Add 050127 */
	extern long avcbe_g_I_quant_offset_mpeg4;	/* MPEG-4専用 *//* Add 050420 */
	extern long avcbe_g_I_quant_offset_range_mpeg4;	/* MPEG-4専用 *//* Add 050420 */

/*extern long				avcbe_g_I_qunat_offset_minus_range_flag; 050421 */
	extern long avcbe_g_I_quant_offset_limit;	/* Add 050124 */
	extern long avcbe_g_I_quant_offset_limit_mpeg4;	/* MPEG-4専用 *//* Add 050428 */

	/*extern long                           avcbe_g_cost_mv_multiplier;     *//* Del 050421 */
/*extern long				avcbe_g_prev_remain_buff; 050425 */
/*extern long				avcbe_g_prev_I_pic_flag;	DEL 050421 */
/*extern long				avcbe_g_prev_I_quant_value;		DEL 050421 */
	extern long avcbe_g_intra_inter_sad_diff_value;	/* Add 050114 */
	extern long avcbe_g_reset_intra_thr_1;	/* Add 050114 */
/*extern long				avcbe_g_ncram_weightq_flag;	DEL 050421 */
	extern long avcbe_g_ncram_q_thr;	/* Add 050115 */
	extern long avcbe_g_ncram_q_thr_mpeg4;	/* 050427 */
	/**extern long				avcbe_g_ncram_control_flag;	**//* Add 050115 */
	extern long avcbe_g_intra_inter_sad_diff_ncram_value;	/* Add 050115 */
	/*extern long                           avcbe_g_me_control_filter_reg_on;       *//* Del 050421 */
	/*extern long                           avcbe_g_me_control_filter;      *//* Del 050421 */

	/*extern char                   avcbe_g_nc_ram_matrix[2][1620]; *//* Add 050407 *//* Del 050518 */
	/*extern char                   avcbe_g_nc_ram_matrix_4S[2][5120]; *//* Add 050425 *//* for VPU4S *//* Del 050518 */
	/*extern char                           avcbe_g_nc_ram_end;     *//* Add 050407 *//* Del 050524 */
	/*extern char                           avcbe_g_median_filter_end; *//* Add 050407 *//* Del 050524 */

	extern long avcbe_g_active_Q_control_enable;
	extern long avcbe_g_mq_act_enable;
	extern long avcbe_g_mq_intra_enable;
	extern unsigned long avcbe_vpu4s_enable_flag;

	extern unsigned char avcbe_g_use_I_PCM;	/* 041109 *//* 名称変更 *//* 060124 */
	/*extern unsigned char avcbe_put_EP_three_byte; *//* 041109 *//* avcbe_g_insert_emulation_prevention_flagに統一 *//* 060124 */
	extern unsigned char avcbe_g_primary_pic_type;	/* 041115 *//* 名称変更 *//* 060124 */
	extern unsigned char avcbe_g_slice_next_Q;	/* 名称変更 *//* 060124 */

	/* extern long avcbe_gvop_bits; *//* 041117 */


	extern long avcbe_call_vpu_reset;

	extern long avcbe_g_use_intra_offset;	/* 041101 *//* 名称変更 *//* 060124 */

	extern avcbe_stream_info *avcbe_current_context;	/* 041101 */

	extern long avcbe_g_insert_emulation_prevention_flag;	/* 041102 *//* ストリームごとに持たせる変数に代入するように変更 *//* 060124 */

	extern const char *avcbe_get_copyright();


#ifdef __cplusplus
}
#endif
#endif				/* ABCBE_GLOBAL_H */
