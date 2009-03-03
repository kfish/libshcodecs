/*****************************************************************************
*                                                                            *
*  SuperH MEPG-4 Video Encode Library                                        *
*                                                                            *
*    Copyright (C) Renesas Technology Corp., 2004-2005. All rights reserved. *
*                                                                            *
*    Version　1.0 :   VPU4EncDef.h                                           *
*                                 2005/06/01 18:00  Renesas Technology Corp. *
*                                                                            *
*****************************************************************************/

/*--------------------------------------------------------------------------*/
/*  1.000  2004.08.02  start codes (TOOL用のヘッダ)                         */
/*--------------------------------------------------------------------------*/

#ifndef	VPU4ENCDEF_H
#define	VPU4ENCDEF_H

/*----- structures -----*/

/* VPU4版エンコーダミドルのAPI関数（avcbe_init_encode()を除く）で設定するエンコード条件の構造体 */
typedef struct {
	unsigned char ref_frame_num;	/* 参照フレーム数（1 or 2) (H.264のみ） */

	long weightdQ_enable;
	TAVCBE_WEIGHTEDQ_CENTER weightedQ_info_center;	/* API関数avcbe_set_weightedQ()に渡すための構造体(1) */
	TAVCBE_WEIGHTEDQ_RECT weightedQ_info_rect;	/* API関数avcbe_set_weightedQ()に渡すための構造体(2) */
	TAVCBE_WEIGHTEDQ_USER weightedQ_info_user;	/* API関数avcbe_set_weightedQ()に渡すための構造体(3) */
	char weightedQ_table_filepath[256];	/* 重み付けテーブルファイルのパス名 */

	avcbe_vui_main_param vui_main_param;	/* API関数avcbe_set_VUI_parameters()に渡すための構造体 (H.264のみ） */

	char out_buffering_period_SEI;	/* SEIメッセージの出力の有無（1:出力する） */
	char out_pic_timing_SEI;
	char out_pan_scan_rect_SEI;
	char out_filler_payload_SEI;
	char out_recovery_point_SEI;
	char out_dec_ref_pic_marking_repetition_SEI;

	avcbe_sei_buffering_period_param sei_buffering_period_param;	/* API関数avcbe_put_SEI_parameters()に渡すための構造体(1) (H.264のみ） */
	avcbe_sei_pic_timing_param sei_pic_timing_param;	/* API関数avcbe_put_SEI_parameters()に渡すための構造体(2) (H.264のみ） */
	avcbe_sei_pan_scan_rect_param sei_pan_scan_rect_param;	/* API関数avcbe_put_SEI_parameters()に渡すための構造体(3) (H.264のみ） */
	avcbe_sei_filler_payload_param sei_filler_payload_param;	/* API関数avcbe_put_SEI_parameters()に渡すための構造体(4) (H.264のみ） */
	avcbe_sei_recovery_point_param sei_recovery_point_param;	/* API関数avcbe_put_SEI_parameters()に渡すための構造体(5) (H.264のみ） */

	long use_I_PCM_flag;	/*I_PCMを出力するかどうか　041109 */

} OTHER_API_ENC_PARAM;

#if 0
/* エンコード実行条件の構造体 */
typedef struct {

	long frame_number_to_encode;	/* エンコードするフレーム数 */

	long output_filler_enable;	/* CPBバッファのオーバーフロー時にFillerを挿入するかどうか *//* 050519 */
	char yuv_CbCr_format;	/* YUVデータ内（出力するローカルデコードファイルも）のCb,Crデータの並び順（1:Cb全部Cr全部、2:Cb0,Cr0,Cb1,Cr1,...、3:Cbの1ライン分,Crの1ライン分,...） *//* 050520 */

	char buf_input_yuv_file_with_path[256 + 8];	/* 入力YUVファイル名（パス付き） *//* 041201 */
	char buf_input_yuv_file[64 + 8];	/* 入力YUVファイル名（パスなし） */

	char buf_output_directry[256 + 8];	/* 出力先のディレクトリ *//* 041201 */
	char buf_output_stream_file[64 + 8];	/* 出力ストリームファイル名（パスなし） */
	unsigned char ref_frame_num;	/* 参照フレーム数（1 or 2) (H.264のみ） */
	long xpic;
	long ypic;
} ENC_EXEC_INFO;
#endif

#endif				/* VPU4ENCDEF_H */
