/******************************************************************************
* SuperH Series MPEG-4 Video Decoder Module                                   *
*                                                                             *
*                                                                             *
*    Copyright (C) Renesas Technology Corp., 2002-2003. All rights reserved.  *
*                                                                             *
*    Version 1.1 : m4vsd_h263dec.h  2003/10/15 12:12 Renesas Technology Corp. *
*                                                                             *
******************************************************************************/
#ifndef _M4VSDEC_H_
#define _M4VSDEC_H_

#ifdef VPU3IP
#ifdef VPU4IP
#include "m4iph_vpu4.h"
#include "avcbd.h"
#else /*VPU4IP*/
#include "m4iph_vpu3_decode.h"
#endif /*VPU4IP*/
#else
#ifdef VPU2IP
#include "m4iph_vpu2.h"
#endif
#endif

#include "Mpeg4vdec.h"

#define DELETE_VLD 1

#define Uint unsigned int
#define Uchar unsigned char
#define SINT short
#define CINT char
typedef unsigned char UINT8;

/* #define USE_LOCAL_VAL */
	/* use local variable rather than global variables */
#ifndef PRINT_ENABLE
#   undef KEY_ON
	/* KEY_ON enable only if PRINT_ENABLE */
#endif
#ifndef KEY_ON
#   undef TRACE
#endif

#ifndef VPU3IP
#define USE_SKIP_FLG
#endif
	/* use skipflg for fast display */

//#define DISP_PRED_IMG

//kasai del #define LINUX
	/* LINUX(little endian) or SPARC(big endian)*/
	/* In the current version LINUX is selected when compiling */

#define RVLC 2
	/* 1= large table version (m4rvlc.c). fast */
	/* 2= small table version (m4rvlc2.c). two times slower */

#define ERR_FLG_HIST_VAL 10
	/* #of frames switched to error suspection mode after error detection */
	/* @000601 add by J.K. */


#define TRUE (-1)
#define FALSE (0)
#define ABS(x) ( (x >= 0)? (x) : -(x) )
/*#define SGN(x) ( (x > 0)? (1) : ((x<0)? (-1):0) )*/
#define SGN(x) ( 1-((((unsigned)(x))>>31)<<1) )
#define MAX(x,y) ( (x > y)? (x) : (y) )
#define MIN(x,y) ( (x > y)? (y) : (x) )
#define CLIP(x,mn,mx) (((x)>(mx))? (mx) : ((x)<(mn))? (mn) : (x))

#define ON (1)
#define OFF (0)
#define X (0)
#define Y (1)

typedef struct {
  int xmax,ymax ;
} Cinfo  ;

typedef struct {
  char data,len ;
}vlcode ;

typedef struct {
  char run,lvl,eocf,len ;
  char runI,lvlI,eocfI ;
}coef_code ;

typedef struct {
  short x,y ;
  short type,cbp ;
}MV_value ;

/*#define SHIFT_CODE(sft) { CodeBPos += (sft) ; if (CodeBPos>=BUFSIZE<<3) fill_buf(0) ;}*/
#define SHIFT_CODE(sft) m4vsd_shift_code(sft)

#ifndef _BIG
#define MSB 0
#define MSB2 8
#define MSB3 16
#define LSB 24
#define BYTE0(x) ((unsigned int)(x)>>24)
#define BYTE1(x) (((x)>>16)&255)
#define BYTE2(x) (((x)>>8)&255)
#define BYTE3(x) ((x)&255)
#define CURBYTE(x) ((x)&255)
#define NEXTBYTE(x) ((x)>>=8)
#define SKIPBIT(x,n) ((x)>>=(n))

#else	/* SPARC */

#define MSB 24
#define MSB2 16
#define MSB3 8
#define LSB 0
#define BYTE3(x) ((unsigned int)(x)>>24)
/*#define BYTE2(x) (((x)>>16)&255)*/
/*#define BYTE1(x) (((x)>>8)&255)*/
/*#define BYTE0(x) ((x)&255)*/
#define BYTE2(x) (((x)<<8)>>24)
#define BYTE1(x) (((x)<<16)>>24)
#define BYTE0(x) (((x)<<24)>>24)
#define CURBYTE(x) ((unsigned int)(x)>>24)
#define NEXTBYTE(x) (x<<=8)
#define SKIPBIT(x,n) ((x)<<=(n))
#endif



#define UMV_ENABLE
	/* enable  unrestricted mv mode (mandatory for MPEG-4 decoder)*/

#ifdef VPU4IP
#define MAX_X_SIZE 1280
#define MAX_Y_SIZE 1024
#else
#define MAX_X_SIZE 720
#define MAX_Y_SIZE 576
#endif
#define MAX_MB_IN_LINE (MAX_X_SIZE/16)
/* should be larger than (maxMBx+2) */
#define MAX_MB_IN_LINE2 (MAX_MB_IN_LINE*2)

#define ERROUT stdout

#define MAX_MB_IN_PACKET (MAX_X_SIZE*MAX_Y_SIZE/256)

typedef struct {
#ifndef VPU3IP
	CINT MBposx, MBposy;
	SINT MBnum;
#endif /* VPU3IP */
	int maxMBx, maxMBy;
	int maxMBn;
} MBaddress;

typedef struct {
  CINT cbp,mbtype,intramb ;
  CINT f_mvd ;
  CINT dquant ;
  CINT ac_pred_flg ;
  CINT notcod ;
  CINT dummy ;
} MBattribute ;

/* "NCNCL" was defined in m4cncl.c(32) and m4mbdec.c(16) */
#define NCNCL 32

/* Symbol "SCMASK" was defined in m4dp.c and m4mbdec.c */
#define SCMASK 0xffff0000

/* Symbol "SCMASK16bit" and "SCMASK8bit" were defined only in m4vldH.c */
#define SCMASK16bit 0xffff0000
#define SCMASK8bit  0xff000000

/* */
#define LMAX_RUN_INTRA 24
#define LMAX_RUN_INTER 48
#define RMAX_LVL_INTRA 32
#define RMAX_LVL_INTER 16

/* buffer length of user_data */
#define MAX_USR_DATA_SIZE 64

/* used in m4vld_init.c and m4vldH.c. */
#define CHECK_COEF_TABLE

#ifdef VPU3IP
typedef struct {
	long trd; /* must be 32bits. "short" may cause overflow. */
	long modulo_time_base;
	long time_increment;
} M4VSD_TIME;

/**Store frame memory information.
 */
struct M4VSD_IMAGE_TABLE {
	char ref_pic_list[ 2 ]; /**< index number of previously decoded non-B frame. */
	char DecW;
	char bvop_mode; /*!< Auto (B-VOP) mode when -1. B-VOP mode when non-zero. */
	long nimage_pointer;
	long last_tr;
	long trd_old;
	unsigned long used_frame;
	unsigned long displayable;
	TAVCBD_FMEM filterd_image;
	M4VSD_TIME time[ M4VSD_MAX_FM ];
	TAVCBD_FMEM image_pointer[ M4VSD_MAX_FM ];
/*	char *image_pointer[ M4VSD_MAX_FM ];*/
};

typedef struct {
        unsigned char *head;
        unsigned char *tail;
        unsigned char *current;
        int bitpos;
} M4VSD_BUF;

struct M4VSD_VPU_DEC_STRUCT {
#ifdef VPU4IP
	M4IPH_VPU4_DEC option;
	M4IPH_VPU4_DEC_RES result;
#else /*VPU4IP*/
	M4IPH_VPU3_DEC option;
	M4IPH_VPU3_DEC_RES result;
#endif /*VPU4IP*/
	long matrix_intra[ 16 ];
	long matrix_inter[ 16 ];
};

/* Structure for secret function "m4vsd_set_image_pointer_tng()" */
typedef struct {
	char DecW, RefF, RefB, DisP;
	void *pOption;
} M4VSD_IMAGE_POINTERS;

typedef struct {
	CINT identifier;
	CINT verid;
	CINT priority;
	CINT type;
	CINT signal_type;
	CINT misc;
	Uint colour_desc;
} M4VSD_VO_PARAM;

typedef struct {
	CINT vol_identifier;
	UINT8 par_width;
	UINT8 par_height;
	CINT vol_control_parameters;
	CINT vbv_parameters;
	CINT latter_half_vbv_buffer_size;
	CINT hierarchy_type; /* for temporal scalability */
	CINT enhancement_type; /* for temporal scalability */
	SINT first_half_bit_rate;
	SINT latter_half_bit_rate;
	SINT first_half_vbv_buffer_size;
	SINT first_half_vbv_occupancy;
	SINT latter_half_vbv_occupancy;
} M4VSD_VBV_PARAM;

/**group_of_vop information.
 */
typedef struct {
		CINT time_codeH;
		CINT time_codeM;
		CINT time_codeS;
		CINT closed_gov;
		CINT broken_link;
} M4VSD_GOV;

/**complexity_extimation_header information.
 */
typedef struct {
	short ceh_i, ceh_p, ceh_b;
} M4VSD_CEH_PARAM;
#endif /* VPU3IP */

/* Variables used for multistream                                       */
typedef struct {
	unsigned long id; /**< identify type of stream */
#ifndef VPU3IP
	CINT reversible_vlc;
	CINT data_partitioned;
#endif
	CINT vol_detected;
	CINT detect_gvop;	// add 030221 to avoid skipping gvop
	CINT profile_level;
	CINT vo_verid;
	CINT vo_priority;
	CINT vo_type;
	CINT vol_id;
	CINT short_video_header;
	CINT pic_format_save;
#ifndef VPU3IP
	CINT umv_mode;
#endif
	CINT vol_lock_flg;
	CINT random_accessible_vol;
	CINT vol_verid;
	CINT vol_priority;
	CINT aspect_ratio_info;
	CINT chroma_format;
#ifndef VPU3IP
	CINT low_delay;
#endif
	CINT vol_shape;
#ifndef VPU3IP
	CINT interlaced;
#endif
	CINT obmc_disable;
	CINT spright_enable;
	CINT not_8bit;
#ifndef VPU3IP
	CINT quant_type;
#endif
	CINT complexity_estimation_disable;
	CINT resync_marker_disable;
	CINT parsed; /* indicates this vop is already parsed (for temporal scalability).*/
	CINT scalability;
	CINT ref_already_set; /**< flag to avoid uninitialized reference on pseudo-temporal scalability data. */
	CINT ref_layer_id; /* in temporal scalability */
	CINT ref_select_code;
	char wrapped; /* for temporal scalability */
	int modulo_previous; /* for temporal scalability */
	int timeinc_previous; /* for temporal scalability */
	M4VSD_GOV gov;
#ifdef VPU3IP
	char sticky_search_data[ 4 ];
#else
	SINT modulo_time_base; // CINT --> SINT 030205
#endif
	SINT vol_width;
	SINT vol_height;
	SINT MemWidth;
	SINT MemYHeight;
	SINT MemHeight;
#ifndef VPU3IP
	SINT MemHWidth;
	SINT nimage_pointer;
	SINT nMB_aft_gob_sc;
#endif
	long BufSize;
#ifndef VPU3IP
	unsigned char *Buf;
	int CodeBPos;
	unsigned char *wimgp, *fimgp;
	unsigned char *wimgpc, *fimgpc;
	int MemCPos;
#endif
	int fixed_vop_rate;
	int fixed_vop_inc;
	int vop_time_inc_res_nbit;
#ifndef VPU3IP
	int maxMBn;
#endif
	int maxMBn_bit;
	int vo_type_id;
	int usr_data_size;
	int usr_data_count;
	unsigned char *usr_data;
	int vop_time_inc_resolution;
#ifndef VPU3IP
	int P_tr;
	int TRd;
	int lastTR;
	unsigned char *image_pointer[ M4VSD_MAX_FM ];
#endif
	MBaddress mbadr;
        int new_input_method; /**< Indicates not use fread mode */
	M4VSD_BUF streambuf;
#ifdef VPU3IP
	struct M4VSD_IMAGE_TABLE image;
	struct M4VSD_VPU_DEC_STRUCT vpu_dec;
	M4VSD_CONTINUE_FUNC continue_func;
	int now_decoding;
#ifdef VPU4IP
	TAVCBD_LAST_FRAME_STATUS status;
#endif /*VPU4IP*/
	M4VSD_VO_PARAM vo;
	M4VSD_VBV_PARAM vbv;
	M4VSD_CEH_PARAM ceh;
#endif
	int resume_mode; /* F=stop when error, T=cont when error */
	int conceal_mode;/*B'00:Notcoded/01:inter/10:intra*/
	int filter_mode;/* 0=off, 1=DBF, 2=LPL, 3=LPH */
	int filter_select;/* 0=POST, 1=CONCEAL */
	int first_frame_flg;
	int bkup_cncl_start_x;
	int bkup_cncl_start_y;
	int bkup_cncl_end_x;
	int bkup_cncl_end_y;
	int i_vop_flg; /* m4vsd_serch_I_VOP() related */
	long nc_ram[ M4IPH_VP4_NC_MAX_APL_4S ];
} M4VSD_MULTISTREAM_VARIABLES;

#endif /* _M4VSDEC_H_ */
