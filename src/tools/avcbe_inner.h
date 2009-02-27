/*****************************************************************************
*                                                                            *
*  SuperH MEPG-4 Video Encode Library                                        *
*                                                                            *
*    Copyright (C) Renesas Technology Corp., 2004-2006. All rights reserved. *
*                                                                            *
*    Version　2.1 :   avcbe_inner.h                                          *
*                                 2006/03/07 22:33  Renesas Technology Corp. *
*                                                                            *
*****************************************************************************/


/*--------------------------------------------------------------------------*/
/*  1.000  2004.05.17  start codes                                          */
/*--------------------------------------------------------------------------*/


#ifndef	AVCBE_INNER_H
#define	AVCBE_INNER_H

#ifdef __cplusplus
extern "C" {
#endif

// michioka
//#include <stdio.h>
//#include <setjmp.h> /* 050523 */
//#include "avcbe.h"
//#include "m4iph_vpu4.h"

#define AVCBE_INTRA_OFFSET_NUM		52	/* イントラオフセットの個数 */

/* avcbe_output_byte関数の出力タイプ(H.264 only) */
#define AVCBE_PUT_HEADER	0	/* 060125 */
#define AVCBE_PUT_END_CODE	1
#define AVCBE_PUT_OTHERS	2

/* avcbe_add_trailing_bits関数の出力タイプ(H.264 only) */
#define AVCBE_NOT_TRB	0	/* 060127 */
#define AVCBE_ADD_TRB	1	/* 060127 */

/*----- avcbe_rate_control_info_t構造体 -----*/
	typedef struct {/*--- VPU4版エンコーダのレートコントロールの情報 ---*/
		char quant_init_i;	/* I-VOPのQ初期値 */
		char quant_init_p;	/* P-VOPのQ初期値 */
		char quant_init_b;	/* B-VOPのQ初期値 */
		char use_dquant;	/* DQUANTの使用フラグ */
		char clip_dquant;	/* DQUANTありのときにクリップする範囲(MPEG-4のときは使用しない) */
		char gvop_m;	/* GVOP M値(1:Low Delay(B-VOPなし), 0:Low Delayでない(B-VOPあり))　ただし、VPU4版エンコーダはH.264ストリームでは常に=1 */
		char coef_cut_enable;	/* 最大符号量により,Qのエリアを制限(H.264の場合は常に0（制限しない）) */
		char quality_mode;	/* 量子化実行モード（H.264の場合は常に0（使用しない）) */

		char quant_min;	/* Qの取りうる最小値 */
		char quant_max;	/* Qの取りうる最大値 */
		char vop_min_stuffing;	/* VOPの最低符号量になるようスタッフィングを詰めるか（0:詰めない、1:詰める、。ただしH.264の場合は常に0（使用しない）) */
		char coef_enc_area;	/* MBの量子化実行エリア(H.264の場合は常に0（使用しない）) */
		char first_p_pic_out;	/* Pピクチャを出力したか（0: まだPピクチャを出力していない、1: Pピクチャを出力済み） */
		char ratecontrol_skip_enable;	/* フレームスキップを行うか否か */
		char dummy_3;	/* 4バイト境界にするためのダミー */

		long cur_quant;	/* 今回のフレームのQ値（取りうる値はchar型ですむが処理効率上long型にする） */
		float cur_f_quant;	/* 今回のフレームのQ値（取りうる値はchar型ですむが処理効率上long型にする） *//* 041213 */
		long prev_quant;	/* 直前のフレームのQ値 */

		long Ni;	/* Nbr I-VOP remaining in GOV */
		long Np;	/* Nbr P-VOP remaining in GOV */
		long Nb;	/* Nbr B-VOP remaining in GOV */

		long d0i;	/* I-VOP virtual vuffer occupancy */
		long d0p;	/* P-VOP virtual vuffer occupancy */
		long d0b;	/* B-VOP virtual vuffer occupancy */

		unsigned long cpb_buf_size;	/* cpb 最大バッファサイズ *//* Rename vbv -> cpb 040729 Y.Funabashi */
		long cpb_buf_remain;	/* cpb バッファ残量 *//* Rename vbv -> cpb 040729 Y.Funabashi  */

		long vbv_buf_size;
		long vbv_buf_remain;

		long T_tm5;	/* mb_target_bitsとして設定する値 */
		long T_min;	/* vop_min_bitsとして設定する値(H.264の場合は常に0（使用しない）) */
		long T_max;	/* mb_max_bitsとして設定する値 */
		long r_tm5;	/* reaction_paramとして設定する値 */
		long d_tm5;	/* virtual_buf_sizeとして設定する値 */

		long R_tm5;	/* remaining bits for GOV */

		float Xi;	/* I-VOP complexity *//* ハードチームが作成したエンコーダだとlong型なので、float型とどちらがよいか検討すること */
		float Xp;	/* P-VOP complexity */
		float Xb;	/* B-VOP complexity */

		unsigned long avcbe_target_Ipic_frame_bits;	/* 目標のIピクチャ割り当てビット量 040726 Add Y.Funabashi  */
		unsigned long avcbe_target_Ppic_frame_bits;	/* 目標のPピクチャ割り当てビット量 040726 Add Y.Funabashi  */
		unsigned long avcbe_cumurative_bits_rc_unit;	/* 実績のピクチャ割り当てビット量累積(RC区間) 040726 Add Y.Funabashi  */
		unsigned long avcbe_cumurative_target_bits_rc_unit;	/* 目標のピクチャ割り当てビット量累積(RC区間) 040726 Add Y.Funabashi  */
		unsigned long avcbe_rate_ctrl_type;	/* Q値コントロールアルゴリズムをTM5にするかForDSCにするかを決めるフラグ。 041208 Add Y.Funabashi  *//* 041213 */
		unsigned long avcbe_cpb_start_size;	/* Q値コントロールアルゴリズムをTM5にするかForDSCにするかを決めるフラグ。 041208 Add Y.Funabashi  *//* 041213 */

#ifdef SH
		unsigned long long avcbe_cumurative_bits;	/* 実績のピクチャ割り当てビット量累積 040726 Add Y.Funabashi  */
		unsigned long long avcbe_cumurative_target_bits;	/* 目標のピクチャ割り当てビット量累積 040726 Add Y.Funabashi  */
#else
		unsigned _int64 avcbe_cumurative_bits;	/* 実績のピクチャ割り当てビット量累積 040726 Add Y.Funabashi  *//* VC++では、64bit Integerを_int64と宣言する 041020 */
		unsigned _int64 avcbe_cumurative_target_bits;	/* 目標のピクチャ割り当てビット量累積 040726 Add Y.Funabashi  *//* 041020 */
#endif

		unsigned long avcbe_activity_sum_pic;	/* アクティビティの合計値（1フレーム分） *//* 050422 */
		unsigned long avcbe_activity_sum_slice;	/* アクティビティの合計値（1スライス分） *//* 050422 */
		unsigned long avcbe_activity_sum_mb;	/* アクティビティの合計値（1フレーム分）をマクロブロック数で割って1/2した値 *//* 050422 */

	} avcbe_rate_control_info_t;

/*----- avcbe nal unit info構造体 -----*/
	typedef struct {
/*	long	avcbe_forbidden_zero_bit; */
		long avcbe_nal_ref_idc;
		long avcbe_nal_unit_type;
	} AVCBE_NAL_UNIT_INFO;

/*----- avcbe sequence parameter set info構造体 -----*/
	typedef struct {
		long avcbe_profile_idc;
		long avcbe_constraint_set0_flag;	/* 041102 */
		long avcbe_constraint_set1_flag;	/* 041102 */
		long avcbe_constraint_set2_flag;	/* 041102 */
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
		long avcbe_SPS_header_bits;	/* SPSのビット数(RateControlで使用) */
	} AVCBE_SPS_INFO;

/*----- avcbe picture parameter set info構造体 -----*/
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
		long avcbe_deblocking_filter_control_present_flag;	/* 041106 名称変更 */
		long avcbe_constrained_intra_pred_flag;
		long avcbe_redundant_pic_cnt_present_flag;
		long avcbe_PPS_header_bits;	/* PPSのビット数(RateControlで使用) */
	} AVCBE_PPS_INFO;

	/*----- avcbe_slice_info構造体 -----*//* コメントの番号は変更しないこと!! */
	typedef struct {
		long avcbe_first_mb_in_slice;	/* (1) *//* M4IPH_VPU4_ENC構造体のメンバにも値を渡す(m4iph_avc_first_mb_in_slice) */
		long avcbe_slice_type;	/* (2) */
		long avcbe_pic_parameter_set_id;	/* (3) */
		long avcbe_frame_num;	/* (4) */
		long avcbe_prev_frame_num;	/* (5) *//* SEIで参照するためにバックアップを取る *//* 041214 */

		long avcbe_idr_pic_id;	/* (5) */
		long avcbe_pic_order_cnt_lsb;	/* (6) */
		long avcbe_delta_pic_order_cnt[2];	/* (7) */

		long avcbe_num_ref_idx_active_override_flag;	/* (8) */
		long avcbe_slice_qp_delta;	/* (9) */
		long avcbe_disable_deblocking_filter_idc;	/* (10) */
		long avcbe_slice_alpha_c0_offset_div2;	/* (11) *//* M4IPH_VPU4_ENC構造体のメンバにも値を渡す(m4iph_avc_alpha_offset) */
		long avcbe_slice_beta_offset_div2;	/* (12) *//* M4IPH_VPU4_ENC構造体のメンバにも値を渡す(m4iph_avc_beta_offset) */
		long avcbe_ref_pic_list_reordering_flag_l0;	/* (13) */
		long avcbe_reordering_of_pic_nums_idc;	/* (14) *//* 041106 */
		long avcbe_abs_diff_pic_num_minus1;	/* (15) *//* 041106 */
		long avcbe_no_output_od_prior_pics_flag;	/* (16) */
		long avcbe_long_term_reference_flag;	/* (17) */
		long avcbe_adaptive_ref_pic_making_mode_flag;	/* (18) */

		long avcbe_vop_type;	/* (19) *//* 1回のavcbe_encode_picture()での発生ビット量 */
		long avcbe_deblock_disable;	/* (20) *//* M4IPH_VPU4_ENC構造体のメンバにも値を渡す(m4iph_avc_deblock_disable) */
		long avcbe_vop_quant;	/* (21) *//* M4IPH_VPU4_ENC構造体のメンバにも値を渡す(m4iph_vop_quant) */
		long avcbe_slice_next_quant;	/* (22) *//* 041118 */

		long avcbe_use_multi_reference;	/* (23) */
		long *avcbe_stream_out;	/* (24) *//* ストリームバッファ(スライスヘッダ出力時に使用) */
		long avcbe_slice_header_total_bits;	/* (25) *//* スライスヘッダのトータルビット数 */
		long avcbe_slice_total_bits_for_frame;	/* (26) *//* NAL単位エンコードでの累積発生ビット量(1フレーム分) *//* 041118 */
		long avcbe_outputed_continue_bits;	/* (27) *//* continue関数で出力したビット数合計 *//* 050707 */
		long avcbe_last_slice_data_continue_bits;	/* (28) *//* continueモード時、スライスの終わりのデータのビット数 *//* 050707 */
		long avcbe_used_continue_in_slice;	/* (29) *//* continueモード時、1スライスの中でコンティニューしたかどうか(1回でもコンティニューした場合は「１」にする) *//* 050708 */

	} AVCBE_SLICE_INFO;

	/* avcbe.hから移動　ここから（外部に見せないため） *//* 050218 */

/* API関数avcbe_set_weightedQ()に渡すための構造体(3)avcbe_weightedQ_mode = AVCBE_WEIGHTEDQ_BY_USERのときに使用する */
	typedef struct {
		long avcbe_weight_type;	/* (1) */
		long avcbe_mode_for_bit1;	/* (2) */
		long avcbe_mode_for_bit2;	/* (3) */
		long avcbe_mode_for_bit3;	/* (4) */
		long avcbe_Qweight_for_bit1;	/* (5) */
		long avcbe_Qweight_for_bit2;	/* (6) */
		long avcbe_Qweight_for_bit3;	/* (7) */
		char *avcbe_MB_table;	/* (8) */
		unsigned long avcbe_num_of_MB_table;	/* (9) */
	} TAVCBE_WEIGHTEDQ_USER;

#if 0				/* 050526 */
/* API関数avcbe_set_VUI_parameters()に渡す情報（内部自動設定用パラメータのみ）(H.264 only) */
	typedef struct {
		long avcbe_bit_rate_scale;	/* (1)        *//* 内部で自動設定する仕様に変更 *//* 050524 */
		long avcbe_cpb_size_scale;	/* (2)        *//* 内部で自動設定する仕様に変更 *//* 050524 */
	} avcbe_inner_vui_hrd_param;
#endif

/* API関数avcbe_set_SEI_parameters()に渡すための構造体(6)DEC_REF_PIC_MARKING_REPETITION用 (H.264 only) */
	typedef struct {
		unsigned long avcbe_original_idr_flag;	/* (1) */
		unsigned long avcbe_original_frame_num;	/* (2) */
		unsigned long avcbe_original_field_pic_flag;	/* (3) */
		unsigned long avcbe_original_bottom_field_flag;	/* (4) */

		unsigned long avcbe_use_output_of_prior_pics_long_term_reference;	/* (5) */
		unsigned long avcbe_no_output_of_prior_pics_flag;	/* (6) */
		unsigned long avcbe_adaptive_ref_pic_making_mode_flag;	/* (7) *//* 041122 */
		unsigned long avcbe_long_term_reference_flag;	/* (8) */
	} avcbe_sei_dec_ref_pic_marking_repetition_param;

/* Work area information */
	typedef struct {
		jmp_buf jmpbuf;	/* for setjmp, longjmp用 */
		jmp_buf jmpbuf2;	/* for setjmp, longjmp用 *//* 返り値=-4で使用 *//* 050523 */
		unsigned char *work_area_top;	/* アプリケーションから渡されたワークエリアの先頭アドレス */
		unsigned long alloc_work_area_size;	/* ワークエリアの使用可能サイズ（バイト数） */
		unsigned long used_work_area_size;	/* 上記のうち、どこまで使用したかのバイト数 */
		unsigned long used_work_area_size_previous;	/* １回前にアロケートしたときに使用していたバイト数 */
	} AVCBE_WORK_AREA_INFO_t;

	/* avcbe.hから移動　ここまで（外部に見せないため） *//* 050218 */


/*----- avcbe_H264_stream_info_t構造体 -----*/

	typedef struct AVCBE_H264_STREAM_INFO {
					/*--- エンコーダコア部のH.264ストリームごとの情報 ---*/

		/*      unsigned long avcbe_stream_ID; *//* このストリームの識別番号（0, 1, 2, ・・・, ストリーム数−1） */
		unsigned long avcbe_stream_type;	/* このストリームのタイプ（AVCBE_MPEG4, AVCBE_H263, AVCBE_H264） */

		/* ストリームごとのコーリングシーケンスチェック用 */
		long avcbe_stream_check_flg;

		AVCBE_WORK_AREA_INFO_t avcbe_work_area_info_for_stream;	/* このストリームのワーク領域の情報 */

		unsigned char *avcbe_stream_buff;	/* avcbe_encode_picture()で渡されたストリーム出力バッファの先頭アドレス */
		unsigned long avcbe_stream_buff_size;	/* 同ストリーム出力バッファのサイズ（単位：バイト） */
		unsigned long avcbe_used_stream_buff_size_bits;	/* ストリーム出力バッファの使用量(ソフトで出力した分のみ)（バイト） */
		long avcbe_stream_output_flag;	/* =0：正常、-1：バッファサイズが不足した */

/*--- ここまでは、MPEG-4用の構造体m4vse_MPEG4_stream_info_tと同じサイズにしてあるのでいじらないこと ---*/

		long avcbe_output_type;	/* avcbe_encode_picture関数の引数output_typeの値 *//* 050105 */
		/* AU delimiter用 *//* 041115 */
		unsigned char *avcbe_AUD_stream_buff;	/* avcbe_encode_picture()で渡されたAUD用ストリーム出力バッファの先頭アドレス */
		unsigned long avcbe_AUD_stream_buff_size;	/* 同ストリーム出力バッファのサイズ（単位：バイト） */

		/* SEI用 *//* 041210 */
		unsigned char *avcbe_SEI_stream_buff;	/* SEI用ストリーム出力バッファの先頭アドレス */
		unsigned long avcbe_SEI_stream_buff_size;	/* 同ストリーム出力バッファのサイズ（単位：バイト） */

	/*--- ここまでは、MPEG-4,H.263用の構造体m4vse_MPEG4_stream_info_tと同じサイズにしてある ---*/
		/* その他のストリームごとのグローバル変数 */
		char avcbe_EndEncoding;
		char avcbe_start_flag;	/* avcbe_encode_picture()がコールされたとき　0:２番目以降のフレーム、1:先頭のフレーム、2:２番目以降のフレームで再初期化する */
		char avcbe_prev_start_flag;	/* avcbe_start_flagのバックアップ用 *//* 041224 */
		char avcbe_vpu_continue_flag;	/* =1:continueした、=0:continueしてない *//* 050203 */

		/* API関数avcbe_init_encode()で指定されたエンコードパラメータ */
		avcbe_encoding_property avcbe_encode_option;
		avcbe_other_options_h264 avcbe_encode_other_opt_h264;

		/* 画像ワーク領域の情報（ローカルデコード画像の出力先、参照フレーム） */
		unsigned char avcbe_ldec_index;	/* ローカルデコード画像の出力先(画像ワーク領域のインデックス番号:0 to N-1) フレーム数は2以上 */
		unsigned char avcbe_ref1_index;	/* 参照フレーム(画像ワーク領域のインデックス番号:0 to N-1) */
		unsigned char avcbe_ref2_index;	/* マルチリファレンスのとき、時間的にref1より前の画像ワーク領域内の参照画像の番号:0 to N-1) */


		long avcbe_num_of_mb;	/* 画像の幅と高さから求まるマクロブロック数(((幅+15)/16)x16)x(((高さ+15)/16)x16) */
		long avcbe_num_of_mb_4x;	/* 4の倍数にしたマクロブロック数 *//* 041027 */

		long avcbe_vop_bits;	/* フレーム単位のビット発生量 */
		long avcbe_out_stream_bits;	/* ビット発生量の累計 */

		TAVCBE_FMEM avcbe_captbuf[1];	/* キャプチャ画像領域 */

		unsigned char avcbe_nrefframe;	/* 参照フレーム数(VPU4版エンコーダでは1 or 2) */

		unsigned char avcbe_nldecfmem;	/* 画像ワーク領域のフレーム数 */
		TAVCBE_FMEM *avcbe_ldecbuf;	/* 画像ワーク領域のアドレステーブル（フレーム数は2以上で上限なしとする。ただし255まで） */

		unsigned char avcbe_npfiltfmem;	/* ポストフィルタ画像領域のフレーム数 */
		TAVCBE_FMEM avcbe_pfiltfbuf[1];	/* ポストフィルタ画像領域 */

		/* 0x03コード埋め込み用のtempバッファ *//* 041128 */
		/* avcbe_output_byte()で出力していたが、4byte単位で挿入していたので、その単位を跨いで0x000000コードが出力される
		   場合があったので一度tempバッファにヘッダ等をすべて溜めてから0x03コード埋め込みにいく */
		char avcbe_temp_stream_buff_1[500];	/* 256->300->500byte *//* 060213 *//* 060307 */
		char avcbe_temp_stream_buff_2[500];	/* 256->300->500byte *//* 060213 *//* 060307 */

		long avcbe_h264_profile;
		long avcbe_h264_level;

		long avcbe_set_intra;

		/*      long avcbe_quant_mod_ratio; *//* 100%増減 *//* 使用しないので削除 050518 */

		long avcbe_frm_interval;	/* 構造体avcbe_frame_statのメンバのうち必要なもの 040909 */
		avcbe_slice_stat avcbe_last_slice_stat;

		/* ヘッダ出力用 *//* 060127 */
		avcbe_SPS_header_info_stat avcbe_last_SPS_header_info;
		avcbe_PPS_header_info_stat avcbe_last_PPS_header_info;
		avcbe_slice_header_info_stat avcbe_last_slice_header_info;

		/* ビットレート監視用 */
		long avcbe_bitrate_watch_index;	/* 最後に処理した区切りのインデックス番号 */
		long avcbe_bitrate_remainder_size;	/* 区切りごとのビットサイズの残り容量 */


		/* ストリーム出力バッファ不足が発生したときに呼び出すべきユーザ作成関数 */
		 avcbe_buf_continue_userproc_ptr
		    avcbe_buf_continue_userproc;
		long avcbe_slice_header_output_bits;	/* ストリーム出力バッファにソフトで出力したサイズ（ビット単位）ストリーム出力バッファ不足のときに使用する *//* 050628 */

		/* VPU4ドライバ部にエンコードパラメータを渡すため */
		M4IPH_VPU4_ENC avcbe_vpu4_enc_info;

		/* VPU4ドライバ部からエンコード結果を受け取るため */
		M4IPH_VPU4_ENC_RES avcbe_vpu4_enc_res;

		/* NAL Unit Syntax用の構造体 */
		AVCBE_NAL_UNIT_INFO avcbe_nal_info;

		/* Sequence Parameter Set用の構造体 */
		AVCBE_SPS_INFO avcbe_SPS_info;

		/* Picture Parameter Set用の構造体 */
		AVCBE_PPS_INFO avcbe_PPS_info;

		/* スライスヘッダ用の構造体 */
		AVCBE_SLICE_INFO avcbe_slice_header_info;

		/* VUIパラメータ用の構造体 */
		avcbe_vui_main_param *avcbe_vui_info;	/* エンコードパラメータavcbe_use_vui_parametersがONならば、ワーク領域内に割り当てる */
		long avcbe_non_vcl_bits;	/* nal_hrd_present_flag=ONの場合のヘッダ部分合計値(Ratecontrolのcpb_buf_remainから引く値) *//* 050526 */


#if 0				/* 050526 */
		avcbe_inner_vui_hrd_param inner_vui_hrd_param;	/* 050524 */
#endif

		long *avcbe_mb_table;	/* B-VOPのときにノットコーデット情報を保持するためのテーブル、要素数はマクロブロック数 */


		/* ストリームタイプがH.264のとき専用のもの */
		long avcbe_encoded_mb_num;	/* スライスに分割するとき、何個のマクロブロックが処理されたか */

		long avcbe_insert_emulation_prevention_flag;	/* 0x03コード（emulation_prevention）を挿入するか否か（デフォルトは挿入する） *//* 060124 */
		unsigned char avcbe_primary_pic_type;	/* AU Delimiterのprimary_pic_typeの出力指定 *//* 060124 */
		unsigned char avcbe_use_I_PCM;	/* I-PCM出力用　avcbe_set_use_I_PCM関数でセットされる *//* 060124 */
		unsigned char avcbe_slice_next_Q;	/* nextQのON/OFF(スライス分割時に使用) *//* 060124 */
		long avcbe_use_intra_offset;	/* イントラオフセット用 *//* =1:イントラオフセットを隠しAPI関数avcbe_set_intra_offsetで設定する。=0:イントラオフセットを指定しない（デフォルト） *//* 060124 */


		char avcbe_use_multi_ref;	/* マルチリファレンスか否かの区別 */
		/* 050408       char avcbe_put_three_byte;      *//* prevention_emulation_three_byteを挿入するか（デフォルト値はAVCBE_ON=する） */

		/* レートコントロール関連 */
		char avcbe_pic_type;	/* 今回のフレームのピクチャの種類（AVCBE_IDR_PIC/AVCBE_I_PIC/AVCBE_P_PIC） */
		char avcbe_prev_pic_type;	/* 直前にエンコードしたフレームのピクチャの種類（フレームスキップしたフレームは除く） */
		char avcbe_I_vop_skip_value;	/* I-VOPをスキップした場合1をセット *//* グローバル変数だったが、ストリームごとに持たせる *//* 050425 */
		char avcbe_prev_I_pic_flag;	/* ストリームごとに持たせるように変更 050425 */
		char avcbe_prev_I_quant_value;	/* ストリームごとに持たせるように変更 050425 */
		char avcbe_ncram_weightq_flag;	/* ストリームごとに持たせるように変更 050425 */
		char avcbe_wq_read_flag;	/* avcbe_g_nc_ram_endに代わるフラグ 050518 */
		char avcbe_wq_write_flag;	/* avcbe_g_median_filter_endに代わるフラグ 050518 */

		long avcbe_x_mb_size;	/* 横方向のマクロブロック数 *//* 050518 */
		long avcbe_y_mb_size;	/* 縦方向のマクロブロック数 *//* 050518 */

		long avcbe_I_pic_count;	/* 周期的に挿入するための、Iピクチャ（IDRピクチャを含む）のカウンタ */
		long avcbe_frm_rate_count;	/* 1秒周期にビットレート制御するためのフレーム数カウンタ（全P-Picモードで利用） */
		float avcbe_skip_counter;	/* 連続スキップカウンタ */
		float avcbe_skip_limit;	/* 連続スキップ最大値 */

		long avcbe_I_quant_offset_minus_range_flag;	/* ストリームごとに持たせる 050425 */
		long avcbe_prev_remain_buff;	/* ストリームごとに持たせる 050425 */
		float avcbe_f_cpb_offset;	/* エンコードパラメータavcbe_ratecontrol_cpb_offset(long型)の値を1/10した値を入れる 050520 */
		float avcbe_f_cpb_offset_rate;	/* cpb buffer に対するcpb_lowerの範囲とcpb_minの範囲の比率 *//* エンコードパラメータavcbe_ratecontrol_cpb_offset_rate(long型)の値を1/10した値を入れる 050520 */
		/*      float avcbe_cpb_stack_rate; *//* cpb buffer に開始時点で貯めておく割合 *//* avcbe_g_cpb_offsetの値はエンコードパラメータで指定するように変更したので不要 050520 */

		long avcbe_remain_sum;	/* ストリームごとに持たせる 050427 */
		long avcbe_cpb_buff_per_frm_over_cnt;	/* ストリームごとに持たせる 050427 */

		long avcbe_1_thr;	/* ストリームごとに持たせる 050425 */
		long avcbe_2_thr;	/* ストリームごとに持たせる 050425 */
		long avcbe_3_thr;	/* ストリームごとに持たせる 050425 */
		long avcbe_4_thr;	/* ストリームごとに持たせる 050425 */
		long avcbe_5_thr;	/* ストリームごとに持たせる 050425 */

		long avcbe_quant_max_thr1;	/* ストリームごとに持たせる 050425 */
		long avcbe_quant_max_thr2;	/* ストリームごとに持たせる 050425 */
		long avcbe_quant_max_thr3;	/* ストリームごとに持たせる 050425 */
		long avcbe_quant_max_thr4;	/* ストリームごとに持たせる 050425 */

		avcbe_rate_control_info_t avcbe_rate_con;	/* ターゲットビット量やQ値を算出するための情報 */

		/* 周期的なINTRAマクロブロックリフレッシュ関連 */
		long avcbe_intraRefreshMBnum;	/* フレームごとにリフレッシュするマクロブロック数 */
		long avcbe_cyclicIntraCounter;	/* 周期的にリフレッシュするためのカウンタ */

		long avcbe_frm;	/* 今回エンコードするフレームのフレーム番号（時間） */
		long avcbe_prev_frm;	/* 直前にエンコードしたフレームのフレーム番号（時間） */

		long avcbe_skip_frame_flag;	/* 0: 今回のフレームはスキップしない、1: スキップする */

		long avcbe_qsum;	/* フレーム単位のQの合計(rate_controlで使用中) */

		/* 量子化係数の重み付け関連 */
		char *avcbe_weightedQ_table1;	/* マクロブロックごとに量子化係数の重み付けを指定したテーブル（要素数は、マクロブロック数） */
		char *avcbe_weightedQ_table2;	/* 同テーブル2（avcbe_weightedQ_mode=AVCBE_WEIGHTEDQ_AUTOのとき、median fileterをかけた重み値を入れる） *//* 050518 */

		/* 2パス制御関連 *//* 041026 動的に確保するように変更 */
		TAVCBE_WORKAREA avcbe_mb_sads_work_buf;	/* マクロブロックごとのSAD値格納先 */
		short *avcbe_mb_sads_buf;	/* マクロブロックごとのSAD値コピー先（要素数は「マクロブロック数/2」） */
		short *avcbe_mb_sads_buf_lowpath;	/* マクロブロックごとのSAD値コピー先（要素数は「マクロブロック数/2」） */
		char *avcbe_mb_quant;	/* マクロブロックごとのQ値（要素数はマクロブロック数) */

		/* 内部で自動的に取るバックアップ *//* 041027 */
		struct AVCBE_H264_STREAM_INFO *avcbe_backup_H264_stream_info;	/* 構造体avcbe_H264_stream_info_tの全メンバ用 */

		avcbe_sei_dec_ref_pic_marking_repetition_param dec_ref_pic_marking_repet_param;	/* 050426 */

		/* 隠し機能のイントラオフセット用 *//* 041101 */
		long avcbe_intra_offset_set_flag;	/* =1:イントラオフセット値のテーブルに値が設定された */
		long *avcbe_intra_offset_table;	/* イントラオフセット値のテーブル（隠しAPI関数avcbe_set_intra_offset()で設定する） */

		/* 隠し機能のMBデバッグ情報用 */
		long avcbe_mb_debug_enable;	/* デバッグ情報のON/OFF *//* 060112 */
		long avcbe_mb_debug_addr;	/* デバッグ情報の出力先のアドレス *//* 060112 */

	} avcbe_H264_stream_info_t;



#ifdef __cplusplus
}
#endif
#endif				/* ABCBE_INNER_H */
