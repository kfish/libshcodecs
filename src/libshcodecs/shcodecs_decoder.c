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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sched.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#include "avcbd.h"
#include "avcbd_optionaldata.h"
#include "decoder_private.h"
#include "m4iph_vpu4.h"
#include "m4driverif.h"
#include "m4vsd_h263dec.h"

/* #define DEBUG */
/* #define OUTPUT_ERROR_MSGS */

#ifdef OUTPUT_ERROR_MSGS
#define MSG_LEN 127
static long
vpu_err(SHCodecs_Decoder *dec, const char *func, int line, long rc)
{
	char msg[MSG_LEN+1];
	msg[MSG_LEN] = 0;
	snprintf(msg, MSG_LEN, "%s, line %d: returned %ld", func, line, rc);
	m4iph_avcbd_perror(msg, rc);
	exit(1);
	return rc;
}
#else
#define vpu_err(enc, func, line, rc) (rc)
#endif


/* XXX: Forward declarations */
static int decode_frame(SHCodecs_Decoder * decoder);
static int extract_frame(SHCodecs_Decoder * decoder, long frame_index);
static int get_input(SHCodecs_Decoder * decoder, void *dst);

static int stream_init(SHCodecs_Decoder * decoder);
static int decoder_init(SHCodecs_Decoder * decoder);
static int decoder_start(SHCodecs_Decoder * decoder);

/***********************************************************/

/*
 * init ()
 */
SHCodecs_Decoder *shcodecs_decoder_init(int width, int height, int format)
{
	SHCodecs_Decoder *decoder;

	if ((decoder = malloc(sizeof(*decoder))) == NULL)
		return NULL;

	memset(decoder, 0, sizeof(*decoder));

	decoder->si_type = format;
	decoder->si_max_fx = width;
	decoder->si_max_fy = height;

	decoder->decoded_cb = NULL;
	decoder->decoded_cb_data = NULL;

	decoder->frame_count = 0;
	decoder->last_cb_ret = 0;

        /* H.264 spec: Max NAL size is the size of an uncomrpessed immage divided
           by the "Minimum Compression Ratio", MinCR. This is 2 for most levels
           but is 4 for levels 3.1 to 4. Since we don't know the level, we use
           MinCR=2 for sizes upto D1 and MinCR=4 for over D1. */
        decoder->max_nal_size = (width * height * 3) / 2; /* YCbCr420 */
        decoder->max_nal_size /= 2; /* Apply MinCR */
        if (width*height > 720*576)
                decoder->max_nal_size /= 2; /* Apply MinCR */

	/* Initialize m4iph */
	if (m4iph_vpu_open(decoder->max_nal_size) < 0) {
		free (decoder);
		return NULL;
	}

	/* Stream initialize */
	if (stream_init(decoder)) {
		/* stream_init() prints the specific error message */
		free(decoder);
		return NULL;
	}
	/* Decoder initialize */
	if (decoder_init(decoder)) {
		free(decoder);
		return NULL;
	}

	return decoder;
}

/*
 * close ()
 */
void shcodecs_decoder_close(SHCodecs_Decoder * decoder)
{
	/* Stream finalization */
	if (decoder->si_vui)
		free(decoder->si_vui);
	if (decoder->si_sei)
		free(decoder->si_sei);

	m4iph_vpu_close();

	free(decoder);
}

int
shcodecs_decoder_set_frame_by_frame (SHCodecs_Decoder * decoder, int frame_by_frame)
{
        if (!decoder) return -1;

        decoder->frame_by_frame = frame_by_frame;

        return 0;
}

int
shcodecs_decoder_set_use_physical (SHCodecs_Decoder * decoder, int use_physical)
{
        if (!decoder) return -1;

        decoder->use_physical = use_physical;

        return 0;
}
int
shcodecs_decoder_set_decoded_callback(SHCodecs_Decoder * decoder,
				      SHCodecs_Decoded_Callback decoded_cb,
				      void *user_data)
{
	if (!decoder)
		return -1;

	decoder->decoded_cb = decoded_cb;
	decoder->decoded_cb_data = user_data;

	return 0;
}

/*
 * Returns number of bytes used.
 */
int
shcodecs_decode(SHCodecs_Decoder * decoder, unsigned char *data, int len)
{
        int nused=0, total_used=0;

        decoder->si_input = data;
	decoder->last_cb_ret = 0;

        while (len > 0) {
                decoder->si_input += nused;
	        decoder->si_ipos = 0;
                decoder->si_ilen = MIN (decoder->max_nal_size, len);
        	decoder->si_isize = decoder->si_ilen;

                if ((nused = decoder_start(decoder)) <= 0)
                        break;

                total_used += nused;
                len -= nused;

		if (decoder->last_cb_ret != 0)
			break;
        }

        return total_used;
}

int
shcodecs_decoder_finalize (SHCodecs_Decoder * decoder)
{
	decoder->needs_finalization = 1;

	return decoder_start (decoder);
}

static int
shcodecs_decoder_output_partial (SHCodecs_Decoder * decoder)
{
	M4VSD_MULTISTREAM_VARIABLES *var;
	struct M4VSD_IMAGE_TABLE *image;
	int cb_ret = 0;

	if (!decoder->needs_finalization) {
                return 0;
        }

	var = (M4VSD_MULTISTREAM_VARIABLES *) decoder->si_ctxt;
	image = &var->image;

#ifdef DEBUG
	fprintf (stderr, "shcodecs_decoder_output_partial: DecW=%d, list[0]=%d, list[1]=%d\n",
		     image->DecW,
		     image->ref_pic_list[0],
		     image->ref_pic_list[1]);
#endif
	if (decoder->index_old != image->ref_pic_list[0]) {
		cb_ret = extract_frame(decoder, image->ref_pic_list[0]);
	} else if (decoder->index_old != image->DecW) {
		cb_ret = extract_frame(decoder, image->DecW);
	} else if (decoder->index_old != image->ref_pic_list[1]) {
		cb_ret = extract_frame(decoder, image->ref_pic_list[1]);
	}

	decoder->needs_finalization--;

	return cb_ret;
}

int
shcodecs_decoder_get_frame_count (SHCodecs_Decoder * decoder)
{
	if (decoder == NULL) return -1;

	return decoder->frame_count;
}

/***********************************************************/

void *global_context;

/***********************************************************/

/*
 * stream_init.
 *
 */
static int stream_init(SHCodecs_Decoder * decoder)
{
	int i,j;
	int iContext_ReqWorkSize;
	int size_of_Y;
	void *pv_wk_buff;
	size_t dp_size;
	TAVCBD_FMEM *frame_list;
	long stream_mode;
	unsigned char *pBuf;
	long rc;

	avcbd_start_decoding();

	/* Get context size */
	iContext_ReqWorkSize =
	    avcbd_get_workarea_size(decoder->si_type ==
				    F_H264 ? AVCBD_TYPE_AVC :
				    AVCBD_TYPE_MPEG4, decoder->si_max_fx,
				    decoder->si_max_fy, 2) + 16;
	if (iContext_ReqWorkSize < 0)
		return vpu_err(decoder, __func__, __LINE__, iContext_ReqWorkSize);

	/* Allocate context memory */
	decoder->si_ctxt = calloc(iContext_ReqWorkSize, 1);
	CHECK_ALLOC(decoder->si_ctxt, iContext_ReqWorkSize,
		    "stream context", err2);
	decoder->si_ctxt_size = iContext_ReqWorkSize;
	memset(decoder->si_ctxt, 0, iContext_ReqWorkSize);
	global_context = decoder->si_ctxt;
	if (decoder->si_type == F_H264) {
		decoder->si_nalb = malloc(decoder->max_nal_size);
		CHECK_ALLOC(decoder->si_nalb, decoder->max_nal_size, "NAL buffer",
			    err1);
	}

        /* Number of reference frames */
        /* For > D1, limit the number of reference frames to 2. This is a
           pragmatic approach when we don't know the number of reference
           frames in the stream... */
        decoder->si_fnum = CFRAME_NUM;
	size_of_Y = ((decoder->si_max_fx + 15) & ~15) * ((decoder->si_max_fy + 15) & ~15);
        if (size_of_Y > (720*576)) {
                decoder->si_fnum = 2;
        }
	decoder->si_flist = calloc(decoder->si_fnum, sizeof(FrameInfo));
	CHECK_ALLOC(decoder->si_flist,
		    decoder->si_fnum * sizeof(FrameInfo), "frame list",
		    err1);

	decoder->si_mbnum = size_of_Y >> 8;
	for (i = 0; i < decoder->si_fnum; i++) {
		/*
 		 * Frame memory should be aligned on a 32-byte boundary.
		 * Although the VPU requires 16 bytes alignment, the
		 * cache line size is 32 bytes on the SH4.
		 */

		pBuf = m4iph_sdr_malloc(size_of_Y + size_of_Y/2, 32);
		/* printf("%02d--Y=%X,",i,(int)pBuf); */
		CHECK_ALLOC(pBuf,
			    size_of_Y + size_of_Y/2,
			    "Y component (kernel memory)", err1);

		decoder->si_flist[i].Y_fmemp = pBuf;
		decoder->si_flist[i].C_fmemp = pBuf + size_of_Y;
	}

	if (decoder->si_type == F_H264) {
		decoder->si_vui = calloc(sizeof(TAVCBD_VUI_PARAMETERS), 1);
		CHECK_ALLOC(decoder->si_vui,
			    sizeof(TAVCBD_VUI_PARAMETERS),
			    "VUI parameters", err1);
		decoder->si_sei = malloc(sizeof(TAVCBD_SEI));
		CHECK_ALLOC(decoder->si_sei, sizeof(TAVCBD_SEI), "SEI",
			    err1);
	}
	/* 16 bytes for each macroblocks */
	dp_size = (size_of_Y * 16) >> 8;

	decoder->si_dp_264 = m4iph_sdr_malloc(dp_size, 32);
	CHECK_ALLOC(decoder->si_dp_264, dp_size, "data partition 1", err1);

	decoder->si_dp_m4 = m4iph_sdr_malloc(dp_size, 32);
	CHECK_ALLOC(decoder->si_dp_m4, dp_size, "data partition 1", err1);

	pBuf = m4iph_sdr_malloc(size_of_Y + size_of_Y/2, 32);
	CHECK_ALLOC(pBuf, size_of_Y + size_of_Y/2,
		    "Y component of filtered frame", err1);
	decoder->si_ff.Y_fmemp = pBuf;
	decoder->si_ff.C_fmemp = pBuf + size_of_Y;

	stream_mode = (decoder->si_type == F_H264) ? AVCBD_TYPE_AVC : AVCBD_TYPE_MPEG4;

	/* Temp frame */
	frame_list = malloc(decoder->si_fnum * sizeof(TAVCBD_FMEM));
	for (j = 0; j < decoder->si_fnum; j++) {

		/* 32 bytes alignemnt to cache line */
		frame_list[j].Y_fmemp =
		    ALIGN_NBYTES(decoder->si_flist[j].Y_fmemp, 32);
		frame_list[j].C_fmemp =
		    ALIGN_NBYTES(decoder->si_flist[j].C_fmemp, 32);
	}
	rc = avcbd_init_sequence(decoder->si_ctxt, decoder->si_ctxt_size,
			    decoder->si_fnum, frame_list,
			    decoder->si_max_fx, decoder->si_max_fy, 2,
			    ALIGN_NBYTES(decoder->si_dp_264, 32),
			    ALIGN_NBYTES(decoder->si_dp_m4, 32), stream_mode,
			    &pv_wk_buff);
	free(frame_list);
	if (rc != 0)
		return vpu_err(decoder, __func__, __LINE__, rc);


	if (decoder->si_type == F_H264) {
		avcbd_set_resume_err (decoder->si_ctxt, 0, AVCBD_CNCL_REF_TYPE1);
	}

	return 0;

      err1:
      err2:
	return -1;
}

/*
 * decoder_init
 *
 */
static int decoder_init(SHCodecs_Decoder * decoder)
{
	if (decoder->si_type == F_H264) {
		avcbd_init_memory_optional(decoder->si_ctxt, AVCBD_VUI,
					   decoder->si_vui,
					   sizeof(TAVCBD_VUI_PARAMETERS));
		avcbd_init_memory_optional(decoder->si_ctxt, AVCBD_SEI,
					   decoder->si_sei,
					   sizeof(TAVCBD_SEI));
#ifdef ANNEX_B
		avcbd_set_decode_mode(decoder->si_ctxt, AVCBD_UNIT_NAL);
#else
		avcbd_set_decode_mode(decoder->si_ctxt, AVCBD_UNIT_NO_ANNEX_B);
#endif
	} else {
		TAVCBD_FMEM filtered;
		filtered.Y_fmemp = ALIGN_NBYTES(decoder->si_ff.Y_fmemp, 32);
		filtered.C_fmemp = ALIGN_NBYTES(decoder->si_ff.C_fmemp, 32);
		avcbd_set_filter_mode(decoder->si_ctxt, AVCBD_FILTER_DBL,
				      AVCBD_POST, &filtered);
	}

	decoder->si_fnum = 0;
	return 0;
      err1:
	return -1;
}

/*
 * decoder_start
 *
 */
static int decoder_start(SHCodecs_Decoder * decoder)
{
	int decoded, dpb_mode;
	M4VSD_MULTISTREAM_VARIABLES *var;
	struct M4VSD_IMAGE_TABLE *image;
        int cb_ret=0;

	/* decode */
	var = (M4VSD_MULTISTREAM_VARIABLES *) decoder->si_ctxt;
	image = &var->image;
	/* printf("\n\nDecoding Frames:                 "); */

	decoder->si_valid = 1;

	do {
		decoded = 0;

		if (decode_frame(decoder) < 0) {
#ifdef DEBUG
			fprintf(stderr, "decoder_start:: %d frames decoded\n", decoder->si_fnum);
#endif

#if 1
                        if (!decoder->needs_finalization) {
#ifdef DEBUG
			fprintf(stderr, "decoder_start:: need data!\n");
#endif
                                goto need_data;
                        }
#endif

			decoded = 0;
			if (decoder->si_type == F_H264)
				dpb_mode = 0;
			else {
                                dpb_mode = 1;
                                //m4iph_vpu4_reset();
                        }
                } else {
			decoded = 1;
			dpb_mode = 0;
		}

		while (cb_ret == 0) {
			long index = avcbd_get_decoded_frame(decoder->si_ctxt, dpb_mode);

			if (index < 0) {
	 			if ((decoded == 0) && (decoder->si_type != F_H264)) {
					cb_ret = shcodecs_decoder_output_partial (decoder);
					decoder->last_cb_ret = cb_ret;
				}

				if (decoder->last_frame_status.error_num == AVCBD_PIC_NOTCODED_VOP) {
					cb_ret = extract_frame(decoder, image->ref_pic_list[0]);
					decoder->last_cb_ret = cb_ret;
				}
				break;
			} else {
		                cb_ret = extract_frame(decoder, index);
				decoder->last_cb_ret = cb_ret;
		                decoder->index_old = index;
                        }
		}

#ifdef DEBUG
		fprintf(stderr, "%16d,dpb_mode=%d\n", decoder->si_fnum++, dpb_mode);
#endif
	} while (decoded && cb_ret == 0);

need_data:

	/* Return number of bytes consumed */
	return decoder->si_ipos;
}

/*
 * increment_input()
 *
 * Work through the passed-in buffer.
 */
static int increment_input (SHCodecs_Decoder * decoder, int len)
{
	int current_pos = decoder->si_ipos + len;
	int rem = decoder->si_isize - current_pos;
	int count;

#ifdef DEBUG
	fprintf(stderr, "shcodecs_decoder::increment_input IN, rem=%d\n", rem);
#endif

	if ((size_t)current_pos <= decoder->si_isize) {
		decoder->si_ipos = current_pos;
	} else {
		return -1;
	}

	return 0;
}

/*
 * decode_frame()
 *
 * Decode one frame.
 */
static int decode_frame(SHCodecs_Decoder * decoder)
{
	int err, ret, i;
	int max_mb;
	TAVCBD_FRAME_SIZE frame_size;
	static long counter = 0;
        int input_len;

	max_mb = decoder->si_mbnum;
	do {
		int curr_len = 0;
		err = -1;

		if ((input_len = get_input(decoder, decoder->si_nalb) <= 0)) {
			return -2;
		}
		if (decoder->si_type == F_H264) {
			unsigned char *input = (unsigned char *)decoder->si_nalb;
			long len = decoder->si_ilen;

#ifdef DEBUG
			fprintf
			    (stderr, "shcodecs_decoder::decode_frame: H.264 len %d: %02x%02x%02x%02x %02x%02x%02x%02x\n",
			     decoder->si_ilen, input[0], input[1],
			     input[2], input[3], input[4], input[5],
			     input[6], input[7]);
#endif

#ifndef ANNEX_B
			/* skip "00.. 00 01" to simulate RTP */
			while (*input == 0) {
				input++;
				len--;
			}
			input++;
			len--;
#endif
			ret = avcbd_set_stream_pointer(decoder->si_ctxt, input, len, NULL);
			if (ret < 0)
				return vpu_err(decoder, __func__, __LINE__, ret);
		} else {
			unsigned char *ptr;
			long hosei = 0;

#ifdef DEBUG
                        fprintf (stderr, "shcodecs_decoder::decode_frame: MPEG4 ptr = input + ipos %d\n", decoder->si_ipos);
#endif

			ptr = decoder->si_input + decoder->si_ipos;

			ret = avcbd_search_vop_header(decoder->si_ctxt,
						      ptr,
						      decoder->si_ilen);

			if (ret < 0) {
#ifdef DEBUG
                                fprintf (stderr, "shcodecs_decoder::decode_frame: MPEG4 search_vop_header < 0\n");
#endif
                                if (decoder->needs_finalization) {
				        if (*ptr != 0 || *(ptr + 1) != 0) {
                                                break;
                                        }
                                } else {
                                        break;
                                }
                        }

			if (decoder->si_ilen & 31)
				hosei = 31;
			else
				hosei = 0;
#if 0 /* WTF? */
			if (counter) {
				for (i = 0; i < 16; i++) {
					*(ptr + decoder->si_ilen + i) = 0;
				}
			}
#endif

			ret = avcbd_set_stream_pointer(decoder->si_ctxt,
						 decoder->si_input + decoder->si_ipos,
						 decoder->si_ilen + hosei, NULL);
			if (ret < 0)
				return vpu_err(decoder, __func__, __LINE__, ret);
		}

		m4iph_vpu_lock();
		ret = avcbd_decode_picture(decoder->si_ctxt, decoder->si_ilen * 8);
		if (ret < 0)
			(void) vpu_err(decoder, __func__, __LINE__, ret);

#ifdef DEBUG
		fprintf
		    (stderr, "shcodecs_decoder::decode_frame: avcbd_decode_picture returned %d\n", ret);
#endif
		ret = avcbd_get_last_frame_stat(decoder->si_ctxt, &decoder->last_frame_status);
		if (ret < 0)
			return vpu_err(decoder, __func__, __LINE__, ret);

		counter = 1;
		m4iph_vpu_unlock();

		if (decoder->si_type == F_H264) {
			curr_len = decoder->si_ilen;
		} else {
			curr_len = (unsigned) (decoder->last_frame_status.read_bits + 7) >> 3;
			decoder->si_ilen -= curr_len;
			avcbd_get_frame_size(decoder->si_ctxt, &frame_size);
			decoder->si_fx = frame_size.width;
			decoder->si_fy = frame_size.height;
		}

		if (decoder->last_frame_status.error_num < 0) {
#ifdef DEBUG
			m4iph_avcbd_perror("avcbd_decode_picture()",
					   decoder->last_frame_status.error_num);
#endif
#if 1
			switch (decoder->last_frame_status.error_num) {
			case AVCBD_MB_OVERRUN:
				increment_input(decoder, curr_len);
				err = 0;
				break;
			default:
				break;
			}
#endif
			break;
		}

		/* This is where actual input data is read */
		if (increment_input(decoder, curr_len) < 0) {
			err = 0;
			break;
		}

#ifdef DEBUG
		fprintf
		    (stderr, "shcodecs_decoder::decode_frame: decoder->last_frame_status.read_slices = %d\n",
		     decoder->last_frame_status.read_slices);
		fprintf
		    (stderr, "shcodecs_decoder::decode_frame: decoder->last_frame_status.last_macroblock_pos = %d (< max_mb %d?)\n",
		     decoder->last_frame_status.last_macroblock_pos,
		     max_mb);
#endif

		if (decoder->last_frame_status.error_num == AVCBD_PIC_NOTCODED_VOP) {
			err = 0;
			break;
		}
		if (decoder->last_frame_status.detect_param & AVCBD_SPS) {
			avcbd_get_frame_size(decoder->si_ctxt, &frame_size);
			decoder->si_fx = frame_size.width;
			decoder->si_fy = frame_size.height;
			max_mb = ((unsigned)(frame_size.width + 15) >> 4) *
				((unsigned)(frame_size.height + 15) >> 4);
			decoder->si_mbnum = max_mb;
		}
		err = 0;
	}
	while ((decoder->last_frame_status.read_slices == 0)
	       || (decoder->last_frame_status.last_macroblock_pos < max_mb));

	if (!err) {
		return 0;
	} else {
		decoder->si_valid = 0;
		return -1;
	}
}

/*
 * extract_frame: extract a decoded frame from the VPU
 *
 */
static int extract_frame(SHCodecs_Decoder * decoder, long frame_index)
{
	FrameInfo *frame = &decoder->si_flist[frame_index];
	unsigned char *yf, *cf;
	int cb_ret=0;
	int size_of_Y = decoder->si_max_fx * decoder->si_max_fy;

#ifdef DEBUG
	fprintf(stderr, "extract_frame: output frame %d, frame_index=%d\n", decoder->frame_count, frame_index);
#endif

	/* Call user's output callback */
	if (decoder->decoded_cb) {
		if (decoder->use_physical) {
			yf = frame->Y_fmemp;
		} else {
			yf = m4iph_addr_to_virt(frame->Y_fmemp);
		}

		cf = yf + size_of_Y;

		cb_ret = decoder->decoded_cb(decoder,
			yf, size_of_Y,
			cf, size_of_Y/2,
			decoder->decoded_cb_data);
	}

	decoder->frame_count++;

	return cb_ret;
}

/*
 * usr_get_input_h264()
 *
 * Set up a slice (H.264)
 *
 */
static int usr_get_input_h264(SHCodecs_Decoder * decoder, void *dst)
{
	long len, size = 0;

        len = decoder->si_isize - decoder->si_ipos;

	/* Always keep a buffer of lookahead data, unless we are finalizing.
         * The amount to keep is a heuristic based on the likely size of a
         * large encoded frame.
         * By returning 0 early, we force the application to either push more
         * data or (if there is no more) to finalize.
         */
        if (!decoder->needs_finalization && len < (decoder->si_max_fx*decoder->si_max_fy/4)) {
#ifdef DEBUG
	        fprintf (stderr, "usr_get_input_h264: not enough data, going back for more\n");
#endif
                return 0;
        }

	/* skip pre-gap */
	size = avcbd_search_start_code(
                  decoder->si_input + decoder->si_ipos,
                  (decoder->si_isize - decoder->si_ipos) * 8,
		  0x01);

	if (size < 0) {
#ifdef DEBUG
		m4iph_avcbd_perror("avcbd_search_start_code()", size);
#endif
		return -1;
	}

	decoder->si_ipos += size;

	/* transfer one block excluding "(00 00) 03" */
	size = avcbd_extract_nal(
		decoder->si_input + decoder->si_ipos,
		dst,
		decoder->si_isize - decoder->si_ipos,
		3);

	if (size <= 0) {
		m4iph_avcbd_perror("avcbd_extract_nal()", size);
	} else
		decoder->si_ilen = size;

	return size;
}

/*
 * usr_get_input_mpeg4()
 *
 * Set up one frame (MPEG-4/H.263)
 *
 */
static int usr_get_input_mpeg4(SHCodecs_Decoder * decoder, void *dst)
{
        long len, ret=0;
        int i, zeroes=0;
        unsigned char * c = decoder->si_input + decoder->si_ipos;

        /* When receiving data frame-by-frame, there is no need to go
         * looking for frame boundaries.  */
        if (decoder->frame_by_frame)
                return decoder->si_ilen;

        len = decoder->si_isize - decoder->si_ipos;
        if (len < 3) return -2;

        if (decoder->needs_finalization) {
#ifdef DEBUG
                fprintf (stderr, "my usr_get_input_mpeg4: finalizing, returning %ld\n", len);
#endif
                        return len;
        }

	/* Always keep a buffer of lookahead data, unless we are finalizing.
         * The amount to keep is a heuristic based on the likely size of a
         * large encoded frame.
         * By returning 0 early, we force the application to either push more
         * data or (if there is no more) to finalize.
         */
        if (len < (decoder->si_max_fx*decoder->si_max_fy/4)) {
#ifdef DEBUG
	        fprintf (stderr, "usr_get_input_mpeg4: not enough data, going back for more\n");
#endif
                return 0;
        }

        return len;
}

/*
 * get_input()
 *
 * Set up a slice (H.264) or frame (MPEG-4/H.263)
 */
static int get_input(SHCodecs_Decoder * decoder, void *dst)
{
	if (decoder->si_type == F_H264) {
		return usr_get_input_h264(decoder, dst);
	} else {
		return usr_get_input_mpeg4(decoder, dst);
	}
}
