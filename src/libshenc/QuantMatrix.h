/*****************************************************************************
*                                                                            *
*  SuperH MEPG-4 Video Encode Library Sample Source                          *
*                                                                            *
*    Copyright (C) Renesas Technology Corp., 2005. All rights reserved.      *
*                                                                            *
*    Version　1.0 :   QuantMatrix.h                                          *
*                                 2005/08/05 08:40  Renesas Technology Corp. *
*                                                                            *
*****************************************************************************/

/*--------------------------------------------------------------------------*/
/*  1.000  2005.07.29  start codes (サンプルソースのヘッダ)                  */
/*--------------------------------------------------------------------------*/

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
