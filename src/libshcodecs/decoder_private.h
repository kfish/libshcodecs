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

#ifndef _SAMPLE_MAIN_H_
#define _SAMPLE_MAIN_H_

#define		F_NONE		0
#define		F_MPEG4		1
#define		F_H264		2

#define CFRAME_NUM		4
#define NAL_BUF_SIZE	(128 * 1024 * 4)
#define WORK_BUF_SIZE	(256 * 1024 * 4)

typedef unsigned char uint8;
typedef unsigned int uint32;
typedef TAVCBD_FMEM FrameInfo;

extern int vpu4_decode(char *ifile, char *ofile, int x, int y, int format,
		int op);
extern int vpu4_encode(char *ifile, char *ofile, int x, int y, int format,
		int op);
extern int vpu4_loopback(int x, int y, int format);
extern int get_decoded_frames(void);

typedef struct _SHCodecs_Decoder SHCodecs_Decoder;

typedef int (*SHCodecs_Decoded_Callback) (SHCodecs_Decoder * decoder,
                                          unsigned char * y_buf, int y_size,
                                          unsigned char * c_buf, int c_size,
                                          void * user_data);

struct _SHCodecs_Decoder {
	int		si_valid;	/* Is stream valid ? */
	int		*si_ctxt;	/* Pointer to context */
	int		si_ctxt_size;	/* Size of context */
	int		si_type;	/* Type of stream */
	unsigned char   *si_input;	/* Pointer to input buffer */
	unsigned char	*si_nalb;	/* NAL Buffer for H.264 */ 
	int		si_ipos;	/* Current position in input stream */
	int		si_ilen;	/* Size of current frame/slice */
	size_t		si_isize;	/* Total size of input data */
	FrameInfo	*si_flist;
	int		si_fnum;	/* Number of frames in temp frame list */
	int		si_fx;		/* Width of frame */
	int		si_fy;		/* Height of frame */
	int		si_max_fx;	/* Maximum frame width */
	int		si_max_fy;	/* Maximum frame height */
	int		si_mbnum;	/* Size in macro blocks */
	long		*si_dp_264;	/* Data partition pointers */
	long		*si_dp_m4;	/* Only valid for MPEG-4 data. */
	TAVCBD_VUI_PARAMETERS *si_vui; 	/* Only for H.264 data. */
	TAVCBD_SEI 	*si_sei;	/* Only for H.264 data. */
	FrameInfo	si_ff;		/* Filtered frame */

	TAVCBD_LAST_FRAME_STATUS last_frame_status;

        SHCodecs_Decoded_Callback decoded_cb;
        void            *decoded_cb_data;

	long		index_old;
	int		needs_finalization;
	int		frame_by_frame;
        int             use_physical;
	int		frame_count;
	int		last_cb_ret;
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
SHCodecs_Decoder *
shcodecs_decoder_init(int width, int height, int format);

/*
 * close ()
 */
void
shcodecs_decoder_close (SHCodecs_Decoder * decoder);

int
shcodecs_decoder_preferred_length (SHCodecs_Decoder * decoder);

int
shcodecs_decode (SHCodecs_Decoder * decoder, unsigned char * data, int len);

int
shcodecs_decode_finalize (SHCodecs_Decoder * decoder);

#endif /* _SAMPLE_MAIN_H_ */
