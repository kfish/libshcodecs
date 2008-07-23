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
//#include "mpeg4venc.h"

#define QMAT_MPEG_TYPE_DEFAULT_INTRA	1		/* MPEG-4の規格書に載っている、quant_type=1(MPEGタイプ)のデフォルトの量子化行列(Intra用) */
#define QMAT_MPEG_TYPE_DEFAULT_NONINTRA	101		/* 同Non-Intra(Inter)用 */

#define QMAT_MPEG_TYPE_ANIME1_INTRA		2		/* 丸尾が考案した、MPEG-4でのアニメ向けの量子化行列その1(Intra用) */
#define QMAT_MPEG_TYPE_ANIME1_NONINTRA	102		/* 同Non-Intra(Inter)用 */

#define QMAT_MPEG_TYPE_ANIME2_INTRA		3		/* 丸尾が考案した、MPEG-4でのアニメ向けの量子化行列その2(Intra用) */
#define QMAT_MPEG_TYPE_ANIME2_NONINTRA	103		/* 同Non-Intra(Inter)用 */

#define QMAT_MPEG_TYPE_SPECIAL_INTRA	4		/* 丸尾が考案した、MPEG-4でのビット発生量を減らすための量子化行列(Intra用) */
#define QMAT_MPEG_TYPE_SPECIAL_NONINTRA	104		/* 同Non-Intra(Inter)用 */

/*----- structures -----*/

#ifdef __cplusplus
extern "C" {
#endif

/*----- prototype -----*/
int SetQuantMatrix(avcbe_stream_info *context, int qmattype_for_Intra,
//int SetQuantMatrix(m4vse_stream_info *context, int qmattype_for_Intra,
						   int qmattype_for_NonIntra);
	/* 引数qmattype_for_Intraには、QMAT_MPEG_TYPE_DEFAULT_INTRA, QMAT_MPEG_TYPE_ANIME1_INTRA, QMAT_MPEG_TYPE_ANIME2_INTRA, QMAT_MPEG_TYPE_SPECIAL_INTRA
	　　のいずれかが指定できます。 */
	/* 引数qmattype_for_NonIntraには、QMAT_MPEG_TYPE_DEFAULT_NONINTRAなどが指定できます。 */
	/* 引数qmattype_for_Intraと引数qmattype_for_NonIntraの組み合わせは自由であり、例えば「qmattype_for_Intra=QMAT_MPEG_TYPE_DEFAULT_INTRA＋
		qmattype_for_NonIntra=QMAT_MPEG_TYPE_ANIME1_NONINTRA」という指定もできます。 */

#ifdef __cplusplus
}
#endif  

#endif /* QUANT_MATRIX_H */