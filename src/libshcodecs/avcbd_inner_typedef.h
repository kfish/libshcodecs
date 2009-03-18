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

/** \file avcbd_inner_typedef.h
 *  \brief structure definitions used in decoder.
 */

#ifndef _AVCBD_INNER_TYPEDEF_H_
#define _AVCBD_INNER_TYPEDEF_H_

#ifdef VPU4IP
#include "m4iph_vpu4.h"
#endif

/** Pointer to the input stream buffer and current readpoint.
 */
typedef struct {
	int bitpos; /**< Bit position in the current read point.
		       zero means MSB.*/
	unsigned int cache_byte; /**< One byte cache. */
	unsigned char *current; /**< Current read point.*/
	unsigned char *tail; /**< End of the input stream + 1.*/
	unsigned char *head; /**< Head of the input stream.*/
} TAVCBD_STREAM_BUFFER;

/** Frame memory related informations.
 */
typedef struct {
	short width; /**< horizontal size of frame memory. */
	short height; /**< vertical size of frame memory. */
	long frame_max; /**< number of frame memories. */
	long dec; /**< current frame to be decoded. */
	unsigned long used_in_dec; /**< bit-field flags  */									/* 060807 VPU4RevUp2.2 for reodering @P002-DMDT-H264 */
	TAVCBD_FMEM frame[ AVCBD_MAX_FRAME_NUM ]; /**< pointers to frame memories. */
	unsigned long used_in_dpb; /**< bit-field flags  */
	unsigned long displayable; /**< bit-field flags  */
	unsigned long frame_lru[ AVCBD_MAX_FRAME_NUM ];										/* 060807 VPU4RevUp2.2 for reodering @P002-DMDT-H264 */
	long poc_list[ AVCBD_MAX_FRAME_NUM ]; /**<picture order counter of the picture */
} TAVCBD_MEM_INFO;

/** Supplimental informations about POC.
 */
typedef struct {
	long poc_top;
	long poc_bottom;
	long prev_frame_num_offset;
	unsigned long prev_frame_num;
	unsigned long prev_memory_management_control_5_detect;
	unsigned long prev_poc_msb;
	unsigned long prev_poc_lsb;
	unsigned long cycle_delta; /**< sum of offset_for_ref_frame[]. */
} TAVCBD_POC_INFO;

/** Slice Header information.
 */
typedef struct {
	unsigned long frame_num;
	unsigned short first_mb_in_slice;
	unsigned char slice_type;
	unsigned char same_slice_type_pic;
	unsigned char pic_parameter_set_id;
	unsigned char MbaffFrameFlag;
	unsigned char field_flag;
	unsigned short idr_pic_id;
	short frame_id; /**< frame_num (or idr_pic_id if IDR picture) */
	unsigned short pic_order_cnt_lsb;
	signed long delta_pic_order_cnt_bottom;
	signed long delta_pic_order_cnt[ 2 ];
	signed char redundant_pic_cnt; /**< [0,127] */
	signed char direct_spatial_mv_pred_flag; /**< one bit flag */
	unsigned char num_ref_idx_active_override_flag; /**< one bit flag */
	unsigned char num_ref_idx_l0_active; /**< [1,32] */
	unsigned char num_ref_idx_l1_active; /**< [1,32] */
	signed char slice_qp_delta;
	unsigned char disable_deblocking_filter_idc; /**< [0,2] */
	signed char slice_alpha_c0_offset; /**< [-12,12] */
	signed char slice_beta_offset; /**< [-12,12] */
	int picNumL0Pred;
	unsigned long mb_field_decoding_flag;
} TAVCBD_SLICE_HEADER;

/** Store what operation is specified in reordering(). */
typedef struct {
	unsigned short reordering_of_pic_nums_idc;
	unsigned short pic_num;
} TAVCBD_REORDERING_ARRAY;

typedef struct {
	unsigned char ref_pic_list_reordering_flag_l0;
	unsigned char ref_pic_list_reordering_flag_l1;
	TAVCBD_REORDERING_ARRAY array[ 16 ];
} TAVCBD_REORDERING;

#define AVCBD_ERROR_HISTORY_MAX 8
typedef struct {
	signed long error_code;
	unsigned long bitpos; /**< bit position at where error is detected. */
	unsigned long mbpos; /**< macroblock position at where error is detected. */
	int frame_id; /**< frame_num (or idr_pic_id if IDR picture) */
} TAVCBD_ERROR_NOTE;

/* from CRL decoder */
#define	AVCBD_MAX_PIC_X	1280
#define	AVCBD_MAX_PIC_Y	1024
#define	AVCBD_MAX_MB_X	((AVCBD_MAX_PIC_X+15)/16)
#define	AVCBD_MAX_MB_Y	((AVCBD_MAX_PIC_Y+15)/16)
#define AVCBD_4X4BLK_X	(AVCBD_MAX_MB_X*4)
#define AVCBD_4X4BLK_Y	(AVCBD_MAX_MB_Y*4)
#define	AVCBD_C_OFFSET	(AVCBD_MAX_PIC_X*AVCBD_MAX_PIC_Y)
#define AVCBD_LUMA_SIZE	(AVCBD_MAX_PIC_X*AVCBD_MAX_PIC_Y)
#define AVCBD_CHROMA_SIZE	(AVCBD_LUMA_SIZE>>1)
/*	CAVLC type index	*/
#define CAVLC_INTRADC	0
#define CAVLC_INTRAAC	1
#define CAVLC_LUMACOEFF	2
#define CAVLC_CHROMADC	3
#define CAVLC_CHROMAAC	4
/*	Macroblock type	*/
/*	For I slice	*/
#define	INTRA_4x4		0
#define	INTRA_16x16		1
#define	I_PCM			25
/*	For P slice	*/
#define MBTYPE_PSLICE_BASE	50
#define P_L0_16x16		50
#define P_L0_L0_16x8	51
#define P_L0_L0_8x16	52
#define	P_8x8			53
#define	P_8x8REF0		54
#define P_SKIP			99

#define	PRED_L0			80
#define	PRED_L1			81
#define DIRECT_MODE		82

/*	For B_silce	*/
#define B_DIRECT_8x8	50

/*	Intra4x4 pred Mode	*/
#define	I4x4_V			0
#define	I4x4_H			1
#define	I4x4_DC			2
#define	I4x4_DDL		3
#define	I4x4_DDR		4
#define	I4x4_VR			5
#define	I4x4_HD			6
#define	I4x4_VL			7
#define	I4x4_HU			8

/*	Intra16x16 pred Mode	*/
#define	I16x16_V		0
#define	I16x16_H		1
#define	I16x16_DC		2
#define	I16x16_PL		3

/*	Intra Chroma Pred Mode	*/
#define ICPM_DC			0
#define ICPM_HORI		1
#define ICPM_VERT		2
#define ICPM_PLAN		3

/*	VLD	*/
/*	Exp_Golomb type index	*/
#define	EG_MAPPED_I4X4	2
#define EG_MAPPED_P		3

/**Quantization coefficients.
 */
typedef struct {
	short *QPyS;
	short *QPcS;
	int QPy;
	int QPc;
	int chromaOffset;
	short QPyA[ 16 ];
	short QPcA[ 16 ];
} TAVCBD_QUANT_INFO;

/** Record of slices for deblocking filter.
* disable flag and A/B offset value.
*/
typedef struct {
	short mb_pos; /** position of head of the slice in macroblock unit. -1 indicates tail of list. */
	char disable_deblocking_filter; /** disable_deblocking_filter_idc flag. */
	char offsetA : 4; /** Q offset alpha for deblocking filter. */
	char offsetB : 4; /** Q offset beta for deblocking filter. */
} TAVCBD_SLICE_RECORD;

#ifdef EXP01
/** Record of Q.
*/
typedef struct {
	short mb_pos; /** position of macroblock of which Q is modified. */
	char qp_y; /** Q of luma */
	char qp_c; /** Q of chroma */
} TAVCBD_Q_RECORD;

/** Record of cbp(coded block bit), left/above availability
*for each macroblock.
*/
typedef struct {
	char intra; /** 0 when inter. */
	char cbp; /** cbp bits of chroma and luma. */
} TAVCBD_MB_RECORD;
#endif /* EXP01 */

#ifndef VPU4IP
typedef struct {
	char					mb_type;
	char					Intra16x16PredMode;
	char					NumMbPart;
	char					MbPredType;
	unsigned char			CodedBlockPatternChroma;
	unsigned char			CodedBlockPatternLuma;
	short					MbPartWidth;
	short					MbPartHeight;
	short					MbSliceID;
	char					MbPartPredMode[4];
	char					MbRefIdx0[4];
} TAVCBD_MB_INFO;
typedef struct {
	char mb_type;
	unsigned char CodedBlockPatternChroma;
	unsigned char CodedBlockPatternLuma;
	char MbQPy;
	char MbQPc;
	char MbAvail;
	short MbSliceID;
	char MbRefIdx0[ 4 ];
	unsigned char DisableDeblockingFilterIdc;
	char FilterOffsetA;
	char FilterOffsetB;
	long MbCbpBlk;
} TAVCBD_MB_INFO_ST;
#ifndef OLD_MVINFO
/**
 *\brief Motion vectors of a macroblock. Used only in loop filter.
 */
typedef struct {
	int type;
	short mv[(16 * 16)/(4 * 4)];
} TAVCBD_BLK_MV;
#endif
typedef struct {
	short					sub_mb_type;
	short					SubMbPartPredMode;
	short					NumSubMbPart;
	short					SubMbPartWidth;
	short					SubMbPartHeight;
} TAVCBD_SUB_MB_INFO;
typedef struct {
	short					Blk4x4Idx;
	short					TotalCoeff;
	short					SliceID;
	short					MBPredType;
	short					MVL0[2];
	short					MVL1[2];
} TAVCBD_4x4BLK_INFO;
typedef struct {
	unsigned char			*pSource;
	unsigned char			*pDestination;
	int						*FracData;
	int						PredXSize;
	int						PredYSize;
	int						*MVdata;
	int frame_width;
} TAVCBD_PRED_INFO;
#endif /* VPU4IP */
typedef struct {
	unsigned char *pCurFrameY, *pCurFrameC;
	TAVCBD_FMEM pRefFrame[ 4 ];
	int CurrMBAddr;
#ifndef VPU4IP
	int MBHPosition, MBVPosition;
	int frameMbWidth;
	int frameWidth;
	int constrained;
	TAVCBD_MB_INFO *pMbInfo;
	TAVCBD_SUB_MB_INFO *pSubMbInfo;
#endif /* VPU4IP */
} TAVCBD_MB_INFO_ALL;

/* avcbd_rbsp.c */
#define P_SLICE 0
#define B_SLICE 1
#define I_SLICE 2
#define SP_SLICE 3
#define SI_SLICE 4

#define SHORT_TERM 1
#define LONG_TERM 2
typedef struct {
	unsigned long frame_num; /**< frame number of reference picture */
	unsigned char used; /**< flag that indicates long-term or short-term */
	unsigned char frame_ptr; /**< index number of the actual frame */
} TAVCBD_REF_PIC_LIST;

/** Struct to hold whole internal state for each stream.
 */
typedef struct {
	unsigned long id; /**< identify type of stream */
#ifndef VPU4IP
	TAVCBD_MB_INFO_ST *mb_info;
#endif
	TAVCBD_STREAM_BUFFER streambuf; /**< Instance of avcbd_streambuf */
	TAVCBD_REF_PIC_LIST ref_pic_list_l0[ 16 ];
	TAVCBD_MEM_INFO image; /**< frame memory information */
	TAVCBD_SLICE_HEADER *slice_header;
	AVCBD_SEQ_PARAMETER_SET *seq_param; /**< pointer to seq_parameter_set array */
	AVCBD_PIC_PARAMETER_SET *current_pic_param; /**< pointer to pic_parameter_set in use */
	AVCBD_PIC_PARAMETER_SET *pic_param; /**< pointer to pic_parameter_set array */
	long pic_param_max;
	short decode_unit; /**< decode unit is frame/slice/not_annex_b */
	long error_resume_mode; /**< whether ignore error or not */
	long error_conceal_mode; /**< type of concealment */
	long error_conceal_strength; /**< strength of concealment */
	long first_frame_flag; /**< indicates first frame or not */
	TAVCBD_ERROR_NOTE error_history[ AVCBD_ERROR_HISTORY_MAX + 1 ];
	TAVCBD_REORDERING reorder; /**< reordering related */
	TAVCBD_MARKING marking; /**< ref_pic_marking related */
	TAVCBD_POC_INFO poc_info;
	TAVCBD_LAST_FRAME_STATUS status; /**< last frame status */
	TAVCBD_VUI_PARAMETERS *vui_parameters; /**< vui_parameters() */
	TAVCBD_REF_PIC_LIST ref_pic_list_l1[ 16 ];
	TAVCBD_SEI *sei;
	long now_decoding; /**< To notify active context in continue mode */
	TAVCBD_DEC_CONTINUE_FUNC continue_func;
#ifdef VPU4IP
	M4IPH_VPU4_DEC vpu_dec_option;
	M4IPH_VPU4_DEC_RES vpu_dec_result;
#endif

	TAVCBD_SLICE_RECORD *slice_history;
#ifdef EXP01
	struct {
		int tail;
		TAVCBD_Q_RECORD *record;
	} q_history;
	TAVCBD_MB_RECORD *mb_history;
#endif /* EXP01 */
} TAVCBD_VARIABLES;

/** Container struct to store the variables.
 */
typedef struct {
	long mixed_mode;
	TAVCBD_VARIABLES *var; /**< array of internal state for streams. */
} TAVCBD_STATUS;

/**Gets median value of three variables.
 */
#define MEDIAN(a, b, c) (((a) > (b)) ? (((b) > (c)) ? (b) : ((a) > (c)) ? (c) : (a)) : (((a) > (c)) ? (a) : ((b) > (c)) ? (c) : (b)))

/**Saturate signed integer value in the range of [-32768, +32768) into [0, 255].
 * Higher 3 bytes are dirty.
 */
#define clip255char( a ) ( !( (a) & ~255 ) ? (a) : ~( (unsigned)(a) >> 16 ) )
/* valid only for range of ( -32768, +511 ) */
/* #define clip255char( a ) ( ( (a) | ( ( (unsigned)~(a) >> 8 ) + 1 ) ) & ( (unsigned)~(a) >> 16 ) ) */

/**Saturate signed integer value in the range of [-8388608, +8388608) into [0, 255].
 * Higher 3 bytes are zeros.
 */
#define clip255int( a ) ( !( (a) & ~255 ) ? (a) : ( (unsigned)~(a) >> 24 ) )

#define  IClip( Min, Max, Val) (((Val)<(Min))? (Min):(((Val)>(Max))? (Max):(Val)))

#if defined(__HITACHI_VERSION__) || defined(__RENESAS_VERSION__)
	#include <umachine.h>
	#define MUL_EXTEND
	#define bswap_32(a) end_cnvl(a)
	#define _INLINE_
	#pragma inline(avcbd_add_clip, avcbd_sub_clip, avcbd_get_strength_intra, avcbd_get_strength_inter, avcbd_deblock_mb, avcbd_deblock_slice)
#elif defined(__GNUC__)
	#define _INLINE_ inline
	#if defined(__i386__)
		static inline unsigned int bswap_32(unsigned int a) {
			__asm__( "bswap %0" : "=r" (a) : "0" (a) );
			return a;
		}
	#endif /* __i386__ */
#else
	#define bswap_32(a) ( ( (a) << 24 ) | ( ( (a) & 0xff00 ) << 8 ) | ( ( (a) >> 8 ) & 0xff00 ) | ( ( (a) >> 24 ) & 0xff ) )
	#if defined(_M_IX86)
		#define _INLINE_ __inline
	#else
		#define _INLINE_
	#endif
#endif

#ifdef MUL_EXTEND
/**Spread one byte to packed-4bytes.
 */
#define EXTEND_BYTE(a) ((a) * 0x01010101)
#else
#define EXTEND_BYTE(a) ((a = (((a) << 8) | (a))), (((a) << 16) | (a)))
#endif

#endif /* _AVCBD_INNER_TYPEDEF_H_ */

