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

#ifndef	QUANT_MATRIX_H
#define	QUANT_MATRIX_H

#include "avcbe.h"

#define QMAT_MPEG_TYPE_DEFAULT_INTRA	1	/* MPEG-4の規格書に載っている、quant_type=1(MPEGタイプ)のデフォルトの量子化行列(Intra用) */
#define QMAT_MPEG_TYPE_DEFAULT_NONINTRA	101	/* 同Non-Intra(Inter)用 */

#define QMAT_MPEG_TYPE_ANIME1_INTRA		2	/* 丸尾が考案した、MPEG-4でのアニメ向けの量子化行列その1(Intra用) */
#define QMAT_MPEG_TYPE_ANIME1_NONINTRA	102	/* 同Non-Intra(Inter)用 */

#define QMAT_MPEG_TYPE_ANIME2_INTRA		3	/* 丸尾が考案した、MPEG-4でのアニメ向けの量子化行列その2(Intra用) */
#define QMAT_MPEG_TYPE_ANIME2_NONINTRA	103	/* 同Non-Intra(Inter)用 */

#define QMAT_MPEG_TYPE_SPECIAL_INTRA	4	/* 丸尾が考案した、MPEG-4でのビット発生量を減らすための量子化行列(Intra用) */
#define QMAT_MPEG_TYPE_SPECIAL_NONINTRA	104	/* 同Non-Intra(Inter)用 */

/*----- structures -----*/

#ifdef __cplusplus
extern "C" {
#endif

/*----- prototype -----*/
	int SetQuantMatrix(avcbe_stream_info * context,
			   int qmattype_for_Intra,
			   int qmattype_for_NonIntra);

#ifdef __cplusplus
}
#endif
#endif				/* QUANT_MATRIX_H */
