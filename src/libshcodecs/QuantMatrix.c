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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "avcbe.h"
#include "QuantMatrix.h"

#ifdef __cplusplus
extern "C" {
#endif

/* MPEG-4の規格書に載っている、quant_type=1(MPEGタイプ)のデフォルトの量子化行列。*/
	const char Qmat_for_Intra_MPEG4_quanttype1_default[64] = {
		8, 17, 18, 19, 21, 23, 25, 27,
		17, 18, 19, 21, 23, 25, 27, 28,
		20, 21, 22, 23, 24, 26, 28, 30,
		21, 22, 23, 24, 26, 28, 30, 32,
		22, 23, 24, 26, 28, 30, 32, 35,
		23, 24, 26, 28, 30, 32, 35, 38,
		25, 26, 28, 30, 32, 35, 38, 41,
		27, 28, 30, 32, 35, 38, 41, 45,
	};

	const char Qmat_for_NonIntra_MPEG4_quanttype1_default[64] = {
		16, 17, 18, 19, 20, 21, 22, 23,
		17, 18, 19, 20, 21, 22, 23, 24,
		18, 19, 20, 21, 22, 23, 24, 25,
		19, 20, 21, 22, 23, 24, 26, 27,
		20, 21, 22, 23, 25, 26, 27, 28,
		21, 22, 23, 24, 26, 27, 28, 30,
		22, 23, 24, 26, 27, 28, 30, 31,
		23, 24, 25, 27, 28, 30, 31, 33,
	};

/* 丸尾が考案した、MPEG-4でのアニメ向けの量子化行列その１。*/
	const char Qmat_for_Intra_MPEG4_Anime1[64] = {
		28, 28, 28, 28, 28, 28, 28, 28,	/* 8, 28, 28, 28, 28, 28, 28, 28, */
		28, 28, 28, 28, 28, 28, 28, 28,
		28, 28, 28, 28, 28, 28, 28, 28,
		28, 28, 28, 28, 28, 28, 28, 28,
		28, 28, 28, 28, 28, 28, 28, 28,
		28, 28, 28, 28, 28, 28, 28, 28,
		28, 28, 28, 28, 28, 28, 28, 28,
		28, 28, 28, 28, 28, 28, 28, 28,
	};

	const char Qmat_for_NonIntra_MPEG4_Anime1[64] = {
		17, 17, 17, 17, 17, 17, 17, 17,
		17, 17, 17, 17, 17, 17, 17, 17,
		17, 17, 17, 17, 17, 17, 17, 17,
		17, 17, 17, 17, 17, 17, 17, 17,
		17, 17, 17, 17, 17, 17, 17, 17,
		17, 17, 17, 17, 17, 17, 17, 17,
		17, 17, 17, 17, 17, 17, 17, 17,
		17, 17, 17, 17, 17, 17, 17, 17,
	};

/*丸尾が考案した、MPEG-4でのアニメ向けの量子化行列その2。 */
	const char Qmat_for_Intra_MPEG4_Anime2[64] = {
		8, 35, 35, 35, 35, 35, 35, 35,
		35, 35, 35, 35, 35, 35, 35, 35,
		35, 35, 35, 35, 35, 35, 35, 35,
		35, 35, 35, 35, 35, 35, 35, 35,
		35, 35, 35, 35, 35, 35, 35, 35,
		35, 35, 35, 35, 35, 35, 35, 35,
		35, 35, 35, 35, 35, 35, 35, 35,
		35, 35, 35, 35, 35, 35, 35, 35,
	};

	const char Qmat_for_NonIntra_MPEG4_Anime2[64] = {
		23, 23, 23, 23, 23, 23, 23, 23,
		23, 23, 23, 23, 23, 23, 23, 23,
		23, 23, 23, 23, 23, 23, 23, 23,
		23, 23, 23, 23, 23, 23, 23, 23,
		23, 23, 23, 23, 23, 23, 23, 23,
		23, 23, 23, 23, 23, 23, 23, 23,
		23, 23, 23, 23, 23, 23, 23, 23,
		23, 23, 23, 23, 23, 23, 23, 23,
	};

/* 丸尾が考案した、MPEG-4でのビット発生量を減らすための量子化行列。*/
	const char Qmat_for_Intra_MPEG4_maruo_special1[64] = {
		8, 17, 18, 19, 21, 23, 25, 30,
		17, 18, 19, 21, 23, 25, 30, 32,
		20, 21, 22, 23, 24, 30, 32, 38,
		21, 22, 23, 24, 30, 32, 38, 45,
		22, 23, 24, 30, 32, 38, 45, 45,
		23, 24, 30, 32, 38, 45, 45, 45,
		25, 30, 32, 38, 45, 45, 45, 45,
		30, 32, 38, 45, 45, 45, 45, 45,
	};

	const char Qmat_for_NonIntra_MPEG4_maruo_special1[64] = {
		16, 17, 18, 19, 20, 21, 22, 23,
		17, 18, 19, 20, 21, 22, 23, 26,
		18, 19, 20, 21, 22, 23, 26, 28,
		19, 20, 21, 22, 23, 26, 28, 33,
		20, 21, 22, 23, 26, 28, 33, 33,
		21, 22, 23, 26, 28, 33, 33, 33,
		22, 23, 26, 28, 33, 33, 33, 33,
		23, 26, 28, 33, 33, 33, 33, 33,
	};


#ifdef __cplusplus
}
#endif

/**
 ************************************************************************
 *\brief 引数で指定された量子化行列を設定する
 *\param context API関数avcbe_init_encode()が返したコンテキスト
 *\param qmattype_for_Intra Intra用量子化行列の種類（詳しくはQuantMatrix.hを参照）
 *\param qmattype_for_NonIntra Non-Intra用量子化行列の種類（同上）
 *\return  0 正常終了
 *\return -1 エラー
 ************************************************************************
 */
#ifdef __cplusplus
extern "C"
#endif
int SetQuantMatrix(avcbe_stream_info * context, int qmattype_for_Intra,
		   int qmattype_for_NonIntra) {
	avcbe_quant_type1_matrix quant_type1_matrix;
	long return_code = 0;
	int index;
	char *matrix_ptr;

	if (context == NULL) {
		return (-1);
	}

	/* Intra用の行列選択 */
	switch (qmattype_for_Intra) {
	case QMAT_MPEG_TYPE_DEFAULT_INTRA:
		matrix_ptr =
		    (char *) &Qmat_for_Intra_MPEG4_quanttype1_default[0];
		break;
	case QMAT_MPEG_TYPE_ANIME1_INTRA:
		matrix_ptr = (char *) &Qmat_for_Intra_MPEG4_Anime1[0];
		break;
	case QMAT_MPEG_TYPE_ANIME2_INTRA:
		matrix_ptr = (char *) &Qmat_for_Intra_MPEG4_Anime2[0];
		break;
	case QMAT_MPEG_TYPE_SPECIAL_INTRA:
		matrix_ptr =
		    (char *) &Qmat_for_Intra_MPEG4_maruo_special1[0];
		break;
	default:
		return_code = -1;
		break;
	}

	if (return_code != 0) {
		return (-1);
	}

	for (index = 0; index < 64; index++) {
		quant_type1_matrix.avcbe_intra_qmatrix[index] =
		    (char) *matrix_ptr++;
	}


	/* NonIntra用の行列選択 */
	switch (qmattype_for_NonIntra) {
	case QMAT_MPEG_TYPE_DEFAULT_NONINTRA:
		matrix_ptr = (char *)
		    &Qmat_for_NonIntra_MPEG4_quanttype1_default[0];
		break;
	case QMAT_MPEG_TYPE_ANIME1_NONINTRA:
		matrix_ptr = (char *) &Qmat_for_NonIntra_MPEG4_Anime1[0];
		break;
	case QMAT_MPEG_TYPE_ANIME2_NONINTRA:
		matrix_ptr = (char *) &Qmat_for_NonIntra_MPEG4_Anime2[0];
		break;
	case QMAT_MPEG_TYPE_SPECIAL_NONINTRA:
		matrix_ptr =
		    (char *) &Qmat_for_NonIntra_MPEG4_maruo_special1[0];
		break;
	default:
		return_code = -1;
		break;
	}

	if (return_code != 0) {
		return (-1);
	}

	for (index = 0; index < 64; index++) {
		quant_type1_matrix.avcbe_inter_qmatrix[index] =
		    (char) *matrix_ptr++;
	}

	return_code = avcbe_set_quant_type1(context, &quant_type1_matrix);

	return (return_code);
}
