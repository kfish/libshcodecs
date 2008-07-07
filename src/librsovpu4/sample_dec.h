/**Sample Program Header.
 */
/*
  Copyright (C) Renesas Technology Corp., 2003-2005. All rights reserved.
  $Date: 2005/03/23 05:09:12 $
 */

#ifndef _SAMPLE_MAIN_H_
#define _SAMPLE_MAIN_H_

#define		F_NONE		0
#define		F_MPEG4		1
#define		F_H264		2

#define CFRAME_NUM		4
#define NAL_BUF_SIZE	(128 * 1024)
#define WORK_BUF_SIZE	(256 * 1024)

typedef unsigned char uint8;
typedef unsigned int uint32;
typedef TAVCBD_FMEM FrameInfo;

extern int vpu4_decode(char *ifile, char *ofile, int x, int y, int format,
		int op);
extern int vpu4_encode(char *ifile, char *ofile, int x, int y, int format,
		int op);
extern int vpu4_loopback(int x, int y, int format);
extern int get_decoded_frames(void);

typedef struct _RSOVPU4_Decoder RSOVPU4_Decoder;

typedef int (*RSOVPU4_Decoded_Callback) (RSOVPU4_Decoder * decoder,
                                         unsigned char * y_buf, int y_size,
                                         unsigned char * c_buf, int c_size,
                                         void * user_data);

struct _RSOVPU4_Decoder {
	int		si_valid;	/* Is stream valid ? */
	int		iStream_type;	/* Type of stream */
	int		iStream_User_FrameWidth;	/* user frame width */
	int		iStream_User_FrameHeight;	/* user frame height */
	int		*piContext;	/* Pointer to context */
	int		iContextSize;	/* Size of context */
	unsigned char	*pbNAL_H264BufferMemory;	/* NAL Buffer for H.264 */ 
	int		iFrame_number;	/* Frame number */
	FrameInfo	*pstFrameInfo_Fnlist;
	int		iMacroBlocks_Size;	/* Size in macro blocks */
	TAVCBD_VUI_PARAMETERS *pst_vui; 	/* Only for H.264 data. */
	TAVCBD_SEI 	*pst_sei;	/* Only for H.264 data. */
	long		*pl_dp_264;	/* Data partition pointers */
	long		*pl_dp_m4;	/* Only valid for MPEG-4 data. */
	FrameInfo	st_ff;		/* Filtered frame */
	int			i_fx;		/* Width of frame */
	int			i_fy;		/* Height of frame */
	int		si_ilen;	/* Size of current frame/slice */
	TAVCBD_LAST_FRAME_STATUS last_frame_status;
        int             preferred_len;  /* Preferred length of next input */
	unsigned char   *input_data;
        int             input_len;
        RSOVPU4_Decoded_Callback decoded_cb;
        void            *decoded_cb_data;
};


/**Align address in w bytes boundary.
 *VPU needs 16 bytes alignment.
 *Length of a cache line of SH4/SH4AL-DSP is 32 bytes.
 */

/**
 *Convert address from non-cache space to cache space and vice versa.
 */
#ifdef __RENESAS_VERSION__
#define TO_CACHE(addr) ((void *)((long)addr & ~0xa0000000))
#define TO_NONCACHE(addr) ((void *)((long)addr | 0xa0000000))
#else
#define TO_CACHE(addr) (addr)
#define TO_NONCACHE(addr) (addr)
#endif


#if !defined(__RENESAS_VERSION__) || defined(SOFT_ONLY)
void workaround_vpu4_cmodel(void);
#define usr_log printf
#else
#ifndef VPU4
#define VPU4
#endif
#define usr_log
#endif


/*
 * init ()
 */
RSOVPU4_Decoder *
rsovpu4_decoder_init(int width, int height, int format);

/*
 * close ()
 */
void
rsovpu4_decoder_close (RSOVPU4_Decoder * decoder);

int
rsovpu4_decoder_preferred_length (RSOVPU4_Decoder * decoder);

int
rsovpu4_decode (RSOVPU4_Decoder * decoder, unsigned char * data, int len);


#endif /* _SAMPLE_MAIN_H_ */
