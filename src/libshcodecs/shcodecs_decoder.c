/*

  Sample Program Code.
  Copyright (C) Renesas Technology Corp., 2007. All rights reserved.

 */

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
#define VPU3IP
#define VPU4IP
#include "m4vsd_h263dec.h"

//#define DEBUG

/* XXX: extern declarations, implemented in m4driverif.c, req'd by middleware ... */
extern void m4iph_sleep_time_init (void);
extern unsigned long m4iph_sleep_time_get (void);
extern int vpu4_clock_on (void);
extern int vpu4_clock_off (void);

/*extern void UserDisp(__const char *__restrict __format, ...);*/

static int stream_init (SHCodecs_Decoder * decoder);
static int decoder_init (SHCodecs_Decoder * decoder);
static int decoder_start (SHCodecs_Decoder *decoder);

/***********************************************************/

/*
 * init ()
 */
SHCodecs_Decoder *
shcodecs_decoder_init (int width, int height, int format)
{
        SHCodecs_Decoder * decoder;

        if ((decoder = malloc (sizeof (*decoder))) == NULL)
                return NULL;

        memset (decoder, 0, sizeof(*decoder));

	decoder->si_type = format;
	decoder->si_max_fx = width;
	decoder->si_max_fy = height;

        decoder->decoded_cb = NULL;
	decoder->decoded_cb_data = NULL;

        /* Initialize m4iph */
        m4iph_vpu_open();
        m4iph_sdr_open();
        m4iph_sleep_time_init();

    //- Stream initialize .
	if (stream_init(decoder)) {
		/* stream_init() prints the specific error message */
                free (decoder);
		return NULL;
	}

    //- Decoder initialize .
	if (decoder_init(decoder)) {
                free (decoder);
		return NULL;
	}

        return decoder;
}

/*
 * close ()
 */
void
shcodecs_decoder_close (SHCodecs_Decoder * decoder)
{

	/* XXX: from stream_Finalization() */
        if (decoder->si_vui)
                free(decoder->si_vui);
        if (decoder->si_sei)
                free(decoder->si_sei);

        /* XXX: from main() */
        m4iph_sdr_close();
        m4iph_vpu_close();

        free (decoder);
}

int
shcodecs_decoder_set_decoded_callback (SHCodecs_Decoder * decoder,
                                      SHCodecs_Decoded_Callback decoded_cb,
                                      void * user_data)
{
        if (!decoder) return -1;

        decoder->decoded_cb = decoded_cb;
        decoder->decoded_cb_data = user_data;

        return 0;
}

int
shcodecs_decoder_preferred_length (SHCodecs_Decoder * decoder)
{
        return decoder->preferred_len;
}

int
shcodecs_decode (SHCodecs_Decoder * decoder, unsigned char * data, int len)
{
        decoder->input_data = data;
        decoder->input_len = len;

	decoder->si_ilen = len;

	return decoder_start (decoder);
}

/***********************************************************/


/* XXX: Forward declarations, ignore for now */
static int decode_frame (SHCodecs_Decoder *decoder);
static int extract_frame (SHCodecs_Decoder *decoder, long frame_index);
static int usr_get_input_h264 (SHCodecs_Decoder * decoder, void *dst);
static int usr_get_input_mpeg4 (SHCodecs_Decoder * decoder, void *dst);
static int get_input (SHCodecs_Decoder * decoder, void *dst);

long m4iph_enc_continue (long output_bits);

void *global_context;

/* TODO: actually, this could be local where it is used */
static M4IPH_VPU4_INIT_OPTION vpu_init_option;

/* XXX: local */
static long decode_time;
/* XXX: this just skips all decode processing, with -S option */
static long performance_flag=0;

/* TODO: This could just be local where it is used */
static TAVCBD_FRAME_SIZE frame_size;

/***********************************************************/

/* XXX: init */
/*
 * stream_init.
 *
 */
static int stream_init (SHCodecs_Decoder * decoder)
{
	int i;
	int iContext_ReqWorkSize;
	size_t dp_size;

    //- Get context size.
	iContext_ReqWorkSize = avcbd_get_workarea_size(decoder->si_type == F_H264 ? 
			AVCBD_TYPE_AVC : AVCBD_TYPE_MPEG4, 
			decoder->si_max_fx, decoder->si_max_fy, 
			2) + 16;
	if (iContext_ReqWorkSize == -1) {
		/*UserDisp("Invalid parameters for avcbd_get_workarea_size()\n");*/
		return -1;
	}

    //- Allocate context memory.
	decoder->si_ctxt = calloc(iContext_ReqWorkSize, 1);
	CHECK_ALLOC(decoder->si_ctxt, iContext_ReqWorkSize, "stream context", err2);
	decoder->si_ctxt_size = iContext_ReqWorkSize;
	memset(decoder->si_ctxt, 0, iContext_ReqWorkSize);
	global_context = decoder->si_ctxt;
	if (decoder->si_type == F_H264) {
 		decoder->si_nalb = malloc(NAL_BUF_SIZE);
		CHECK_ALLOC(decoder->si_nalb, NAL_BUF_SIZE, "NAL buffer", err1);
	}

	decoder->si_fnum = CFRAME_NUM;
	iContext_ReqWorkSize = decoder->si_max_fx * decoder->si_max_fy;
#if 0
	if ( iContext_ReqWorkSize > (352*288) ) {
		decoder->si_fnum = decoder->si_fnum/4;
	}
#endif
	decoder->si_flist = calloc(decoder->si_fnum, sizeof(FrameInfo));
	CHECK_ALLOC(decoder->si_flist, 
		decoder->si_fnum * sizeof(FrameInfo), "frame list", err1);
	
	decoder->si_mbnum = iContext_ReqWorkSize >> 8;
	for (i = 0; i < decoder->si_fnum; i++) {
	    //- Frame memory should be aligned on a 32-byte boundary.
	    //- Although the VPU requires 16 bytes alignment, the
	    //  cache line size is 32 bytes on the SH4.
		pthread_mutex_lock(&vpu_mutex);

	    //- luma frame.
		decoder->si_flist[i].Y_fmemp = m4iph_sdr_malloc(iContext_ReqWorkSize, 32);

		/*UserDisp("%02d--Y=%X,",i,(int)decoder->si_flist[i].Y_fmemp);*/
		CHECK_ALLOC(decoder->si_flist[i].Y_fmemp,
				iContext_ReqWorkSize + (iContext_ReqWorkSize >> 1), 
				"Y component (kernel memory)", err1);

	    //- chroma frame.
		decoder->si_flist[i].C_fmemp 
				= m4iph_sdr_malloc(iContext_ReqWorkSize >> 1, 32);
		/*UserDisp("C=%X\n",(int)decoder->si_flist[i].C_fmemp);*/
		CHECK_ALLOC(decoder->si_flist[i].C_fmemp,
				iContext_ReqWorkSize >> 1, 
				"C component (kernel memory)", err1);
		pthread_mutex_unlock(&vpu_mutex);
	}

	if (decoder->si_type == F_H264) {
		decoder->si_vui = calloc(sizeof(TAVCBD_VUI_PARAMETERS), 1);
		CHECK_ALLOC(decoder->si_vui, sizeof(TAVCBD_VUI_PARAMETERS), "VUI parameters", err1);
		decoder->si_sei = malloc(sizeof(TAVCBD_SEI));
		CHECK_ALLOC(decoder->si_sei, sizeof(TAVCBD_SEI), "SEI", err1);
	}

	//- 16 bytes for each macroblocks
	dp_size = (iContext_ReqWorkSize * 16) >> 8; 

	pthread_mutex_lock(&vpu_mutex);
		
		decoder->si_dp_264 = m4iph_sdr_malloc(dp_size, 32);
		CHECK_ALLOC(decoder->si_dp_264, dp_size, "data partition 1", err1);

		decoder->si_dp_m4 = m4iph_sdr_malloc(dp_size, 32);
		CHECK_ALLOC(decoder->si_dp_m4, dp_size, "data partition 1", err1);

		decoder->si_ff.Y_fmemp = m4iph_sdr_malloc(iContext_ReqWorkSize, 32);
		CHECK_ALLOC(decoder->si_ff.Y_fmemp, iContext_ReqWorkSize,
				"Y component of filtered frame", err1);

		decoder->si_ff.C_fmemp = m4iph_sdr_malloc(iContext_ReqWorkSize >> 1, 32);
		CHECK_ALLOC(decoder->si_ff.C_fmemp, (iContext_ReqWorkSize >> 1),
				"C component of filtered frame", err1);

	pthread_mutex_unlock(&vpu_mutex);

	return 0;

err1:
err2:
	return -1;
}

/* XXX: init */
/*
 * decoder_init
 *
 */
static int decoder_init (SHCodecs_Decoder * decoder)
{
	TAVCBD_FMEM *frame_list;
	void *pv_wk_buff;
	long stream_mode;
	int j;

	pv_wk_buff = m4iph_sdr_malloc(WORK_BUF_SIZE, 32);
	/*UserDisp("work buffer = %X\n",(int)pv_wk_buff);*/
	CHECK_ALLOC(pv_wk_buff, WORK_BUF_SIZE, "work buffer (kernel)", err1);

	vpu_init_option.m4iph_vpu_base_address = 0xfe900000;
	vpu_init_option.m4iph_vpu_endian = 0x3ff;

	#ifdef DISABLE_INT
	vpu_init_option.m4iph_vpu_interrupt_enable = M4IPH_OFF;
	#else
	vpu_init_option.m4iph_vpu_interrupt_enable = M4IPH_ON;
	#endif

	vpu_init_option.m4iph_vpu_clock_supply_control = M4IPH_CTL_FRAME_UNIT;
	vpu_init_option.m4iph_vpu_mask_address_disable = M4IPH_OFF;
	vpu_init_option.m4iph_temporary_buff_address = (unsigned long)ALIGN(pv_wk_buff, 32);
	vpu_init_option.m4iph_temporary_buff_size = WORK_BUF_SIZE;
	m4iph_vpu4_init(&vpu_init_option);

	avcbd_start_decoding();
	stream_mode = (decoder->si_type == F_H264) ? AVCBD_TYPE_AVC : AVCBD_TYPE_MPEG4;

	//- Temp frame.
	frame_list = malloc(decoder->si_fnum * sizeof(TAVCBD_FMEM)); 
	for (j = 0; j < decoder->si_fnum; j++) {
	
		//- 32 bytes alignemnt to cache line.
		frame_list[j].Y_fmemp = ALIGN(decoder->si_flist[j].Y_fmemp, 32);
		frame_list[j].C_fmemp = ALIGN(decoder->si_flist[j].C_fmemp, 32);
	}
	avcbd_init_sequence(decoder->si_ctxt, decoder->si_ctxt_size, decoder->si_fnum,
				frame_list, decoder->si_max_fx, decoder->si_max_fy,
				2, ALIGN(decoder->si_dp_264, 32), 
				ALIGN(decoder->si_dp_m4, 32),
				stream_mode, &pv_wk_buff);
	
	free(frame_list);
	if (decoder->si_type == F_H264) {
		avcbd_init_memory_optional(decoder->si_ctxt, AVCBD_VUI, 
				decoder->si_vui, sizeof(TAVCBD_VUI_PARAMETERS));
		avcbd_init_memory_optional(decoder->si_ctxt, AVCBD_SEI, 
				decoder->si_sei, sizeof(TAVCBD_SEI));
		#ifdef ANNEX_B
		avcbd_set_decode_mode(decoder->si_ctxt, AVCBD_UNIT_NAL);
		#else
		avcbd_set_decode_mode(decoder->si_ctxt, AVCBD_UNIT_NO_ANNEX_B);
		#endif
	} else {
		TAVCBD_FMEM filtered;
		filtered.Y_fmemp = ALIGN(decoder->si_ff.Y_fmemp, 32);
		filtered.C_fmemp = ALIGN(decoder->si_ff.C_fmemp, 32);
		avcbd_set_filter_mode(decoder->si_ctxt, AVCBD_FILTER_DBL, AVCBD_POST, &filtered);
	}

	decoder->si_fnum = 0;
	return 0;
err1:
	return -1;
}

/* XXX: decode */
/*
 * decoder_start
 *
 */
static int decoder_start (SHCodecs_Decoder * decoder)
{
	static int index_old = 0;
	int decoded, dpb_mode;
	M4VSD_MULTISTREAM_VARIABLES *var;
	struct M4VSD_IMAGE_TABLE *image;
	

	//- decode .
	var = (M4VSD_MULTISTREAM_VARIABLES *)decoder->si_ctxt;
	image = &var->image;
	/*UserDisp("\n\nDecoding Frames:                 ");*/
	
	do {
		decoded = 0;

		if (decode_frame(decoder) == -1) {
			/*UserDisp("%d frames decoded\n", decoder->si_fnum);*/
#ifdef DEBUG
			printf ("%d frames decoded\n", decoder->si_fnum);
#endif
			decoded = 0;
			if (decoder->si_type == F_H264) dpb_mode = 2;
			else dpb_mode = 1;
		} else {
#ifdef DEBUG
			printf ("shcodecs_decoder::decoder_start: decoded\n");
#endif
			decoded = 1;
			dpb_mode = 1;
		}

		while(1) {
			long index = avcbd_get_decoded_frame(decoder->si_ctxt, dpb_mode);
#ifdef DEBUG
			printf ("shcodecs_decoder:: avcbd_get_decoded_frame returned index %ld\n",
                                index);
#endif
			if (index < 0) {
				if ( (decoded==0) && (decoder->si_type != F_H264)) {
					/*UserDisp("DecW=%d, list[0]=%d, list[1]=%d\n",image->DecW,image->ref_pic_list[0],image->ref_pic_list[1]);*/
#ifdef DEBUG
					printf ("DecW=%d, list[0]=%d, list[1]=%d\n",image->DecW,image->ref_pic_list[0],image->ref_pic_list[1]);
#endif
					if (index_old!=image->ref_pic_list[0]) {
						extract_frame(decoder, image->ref_pic_list[0]);
					} else 	if (index_old!=image->DecW) {
						extract_frame(decoder, image->DecW);
					} else 	if (index_old!=image->ref_pic_list[1]) {
						extract_frame(decoder, image->ref_pic_list[1]);
					}
				} 
#ifdef DEBUG
				printf ("shcodecs_decoder::decoder->last_frame_status.error_num: %d\n",
                                        decoder->last_frame_status.error_num);
#endif
				if (decoder->last_frame_status.error_num == AVCBD_PIC_NOTCODED_VOP){ /* Not coded vop */
					extract_frame(decoder, image->ref_pic_list[0]);
				}
				break;
			}
			extract_frame(decoder, index);
			index_old = index;
		}

		decoder->si_fnum++;
		/*UserDisp("%16d,dpb_mode=%d\n", decoder->si_fnum, dpb_mode);*/
#ifdef DEBUG
		printf ("%16d,dpb_mode=%d\n", decoder->si_fnum, dpb_mode);
#endif
	} while (decoded);

	return 0;
}

/*
 * decode_frame
 *
 */
int decode_frame (SHCodecs_Decoder * decoder)
{
	int err, ret, len, i;
	int max_mb;
	unsigned char *input;
	struct timeval tv, tv1;
	struct timezone tz;
	long tm;
	static long counter=0, max_size=0;
	char Ct=0;

#ifdef DEBUG
	printf ("shcodecs_decoder::decode_frame IN\n");
#endif
	
	max_mb = decoder->si_mbnum;
	do {
		int curr_len;
		err = -1;
		
		/*UserDisp("******** Count = %d ********\n", ++Ct);*/
		if(Ct > 250)
			Ct=0;
	
		if (get_input(decoder, decoder->si_nalb) <= 0) {
#ifdef DEBUG
			printf("get_input failed\n");
#endif
			break;
		}
		if (decoder->si_type == F_H264) {
			input = (unsigned char *)decoder->si_nalb;
			len = decoder->si_ilen;

#ifdef DEBUG
			printf ("shcodecs_decoder::decode_frame: H.264 len %d: %02x%02x%02x%02x %02x%02x%02x%02x\n",
				decoder->si_ilen, input[0], input[1], input[2], input[3],
				input[4], input[5], input[6], input[7]);
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
#ifdef DEBUG
                        printf ("shcodecs_decoder::decode_frame: H.264 avcbd_set_stream_pointer returned %ld\n", ret);
#endif
		} else {
			unsigned char *ptr;
			long hosei;

			/*ptr = (pbInput_BufferMemory + si_ipos);*/
                        ptr = decoder->input_data;

                        ret = avcbd_search_vop_header(decoder->si_ctxt, ptr, decoder->si_ilen);
                        if ( ret<0 ) {
                                if ( *ptr!=0 || *(ptr+1)!=0 ) {
                                        break;
                                }
                        }
                        if ( decoder->si_ilen & 31 ) hosei=31;
                        else hosei = 0;
                        if ( counter ) {
                                for ( i=0; i<16; i++ ) {
                                        *(ptr+decoder->si_ilen+i)=0;
                                }
                        }
                        avcbd_set_stream_pointer(decoder->si_ctxt,
                                        /*pbInput_BufferMemory + si_ipos,*/
                                        decoder->input_data,
					decoder->si_ilen+hosei, NULL);
		}

		pthread_mutex_lock(&vpu_mutex);
		gettimeofday(&tv, &tz);
		/*UserDisp("dec_pic0=%ld,",tv.tv_usec);*/
		vpu4_clock_on();
		ret = avcbd_decode_picture(decoder->si_ctxt, decoder->si_ilen * 8);
		vpu4_clock_off();
		gettimeofday(&tv1, &tz);
		/*UserDisp("ret=%d,dec_pic1=%ld,",ret,tv1.tv_usec);*/
#ifdef DEBUG
		printf ("ret=%d,dec_pic1=%ld,",ret,tv1.tv_usec);
#endif
		tm = (tv1.tv_usec-tv.tv_usec)/1000;
		if ( tm<0 ) {
			tm = 1000-(tv.tv_usec-tv1.tv_usec)/1000;
		}
		decode_time += tm;
		/*UserDisp("Total decode time = %d(msec),",(int)decode_Get_time());*/
		/*UserDisp("Total sleep  time = %d(msec),",(int)m4iph_sleep_time_get(),decoder->si_ilen);*/
		ret = avcbd_get_last_frame_stat(decoder->si_ctxt, &decoder->last_frame_status);
#ifdef DEBUG	
		printf ("avcbd_get_last_frame_stat ret %ld\n", ret);
#endif
		max_size += decoder->si_ilen;
		/*UserDisp(" stream_size=%dbyte, max_size=%dbyte\n",decoder->si_ilen, max_size);*/
		counter = 1;
		pthread_mutex_unlock(&vpu_mutex);
#ifdef DEBUG
		printf (" stream_size=%d bytes, max_size=%d bytes\n",decoder->si_ilen, max_size);
#endif
		
		if (decoder->last_frame_status.error_num < 0) {
			m4iph_avcbd_perror("avcbd_decode_picture()", decoder->last_frame_status.error_num);
#if 0			
			switch(decoder->last_frame_status.error_num){
				case AVCBD_MB_OVERRUN:
					increment_Input(decoder, curr_len);
					err = 0;
					break;
				default:
					break;
			}
#endif
			break;
		}
		if (decoder->si_type == F_H264) {
			curr_len = decoder->si_ilen;
		} else {
			curr_len = (unsigned)(decoder->last_frame_status.read_bits + 7) >> 3;
			decoder->si_ilen -= curr_len;
			avcbd_get_frame_size(decoder->si_ctxt, &frame_size);
			decoder->si_fx = frame_size.width;
			decoder->si_fy = frame_size.height;
		}

                /* XXX: this is where actual input data is read */
#if 0
		if (increment_Input(decoder, curr_len) < 0)
			break;
#else
		if (decoder->input_len - decoder->si_ilen < curr_len)
			break;

		decoder->input_data += curr_len;

                decoder->preferred_len = curr_len;

#if 0
		if (decoder->si_ilen <= 0)
			break;
#endif

#if 0
		/* XXX: these bits are taken care of in increment_Input() */
		if ((si_isize - si_ipos - curr_len) <= 0)
                        break;
                else if (si_ipos + curr_len < si_isize)
                        si_ipos += curr_len;
#endif
#endif

#ifdef DEBUG
		printf ("shcodecs_decoder::decode_frame: decoder->last_frame_status.read_slices = %d\n",
                        decoder->last_frame_status.read_slices);
		printf ("shcodecs_decoder::decode_frame: decoder->last_frame_status.last_macroblock_pos = %d (< max_mb %d?)\n",
                        decoder->last_frame_status.last_macroblock_pos, max_mb);
#endif

		if (decoder->last_frame_status.error_num == AVCBD_PIC_NOTCODED_VOP) {
			err = 0;
			break;
		}
		if (decoder->last_frame_status.detect_param & AVCBD_SPS) {
			avcbd_get_frame_size(decoder->si_ctxt, &frame_size);
			decoder->si_fx = frame_size.width;
			decoder->si_fy = frame_size.height;
			max_mb = ((unsigned)(frame_size.width + 15) >> 4) 
					* ((unsigned)(frame_size.height + 15) >> 4);
			decoder->si_mbnum = max_mb;
		}
		//break;
		err = 0;
	} while ((decoder->last_frame_status.read_slices == 0) || (decoder->last_frame_status.last_macroblock_pos < max_mb));

	if (!err) {
		return 0;
	} else {
		/*UserDisp("Error decoding frame.\n");*/
		decoder->si_valid = 0;
		return -1;
	}
}

#if 1
/*
 * m4iph_enc_continue
 *
 */
long m4iph_enc_continue (long output_bits)
{
	return 1;
}
#endif

/*
 * extract_frame: extract a decoded frame from the VPU
 *
 */
static int extract_frame (SHCodecs_Decoder * decoder, long frame_index)
{
	FrameInfo *frame = &decoder->si_flist[frame_index];
	int luma_size = decoder->si_fx * decoder->si_fy;
	unsigned long ymem, cmem, page;
	int ry;
	unsigned char *yf, *cf;
	static long frame_cnt=1;
	int pagesize = getpagesize();

	if ( performance_flag==1 ) {
		return 0;
	}
	/*UserDisp("Output Frame %d, frame_index=%d\n",frame_cnt++,frame_index);*/
#ifdef DEBUG
	printf ("Output Frame %d, frame_index=%d\n",frame_cnt++,frame_index);
#endif
	ymem = (unsigned long)frame->Y_fmemp;
	cmem = (unsigned long)frame->C_fmemp;
	page = ymem & ~(pagesize - 1);
	ry = (unsigned long)ymem - page;
	yf = m4iph_map_sdr_mem((void *)page, luma_size + (luma_size >> 1) + ry + 31);
	if (yf == NULL) {
		printf("%s: Aborting since mmap() failed.\n", __FUNCTION__);
		abort();
	}
	
	//- C component should immediately follow the Y component .
	cf = ALIGN(yf + ry + luma_size, 32);

        /* Call user's output callback */
        if (decoder->decoded_cb) {
            decoder->decoded_cb (decoder, yf + ry, luma_size, cf, luma_size>>1,
                                 decoder->decoded_cb_data);
        }

	m4iph_unmap_sdr_mem(yf, luma_size + (luma_size >> 1) + ry + 31);
	//wait(1000);
	return 0;
}

/*
 * usr_get_input_h264
 *
 */
static int usr_get_input_h264 (SHCodecs_Decoder * decoder, void *dst)
{
	long size=0;

	//- skip pre-gap .
	size = avcbd_search_start_code(
		/*pbInput_BufferMemory + si_ipos,*/
                decoder->input_data,
		/*(si_isize - si_ipos) * 8,*/
                decoder->input_len * 8,
 		0x01);
	if (size < 0) {
		m4iph_avcbd_perror("avcbd_search_start_code()", size);
		return -1;
	}
	/*si_ipos += size;*/

	//- tranfer one block excluding "(00 00) 03" .
	size = avcbd_extract_nal(
		/* pbInput_BufferMemory + si_ipos, */
                decoder->input_data + size,
		dst,
		/*si_isize - si_ipos,*/
                decoder->input_len,
		3 );
	if (size <= 0) {
		m4iph_avcbd_perror("avcbd_extract_nal()", size);
		printf("usr_Get_Input_H264 error\n");
	} else
		decoder->si_ilen = size;

	return size;
}

/*
 * usr_get_input_mpeg4
 *
 */
static int usr_get_input_mpeg4 (SHCodecs_Decoder * decoder, void *dst)
{
	return decoder->si_ilen;
}

/*
 * get_input
 *
 */
static int get_input (SHCodecs_Decoder * decoder, void *dst)
{
	if (decoder->si_type == F_H264) {
		return usr_get_input_h264(decoder, dst);
	} else {
		return usr_get_input_mpeg4(decoder, dst);
	}
}
