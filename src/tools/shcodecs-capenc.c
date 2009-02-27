/*****************************************************************************
*                                                                            *
*  SuperH MEPG-4 Video Encode Library                                        *
*    (User Application Sample source for VPU4)                               *
*                                                                            *
*    Copyright (C) Renesas Technology Corp., 2005. All rights reserved.      *
*                                                                            *
*    Version@2.0 : avcbencsmp_mpeg4.c					     *
*				 2005/07/27 14:30 Renesas Technology Corp.   *
*                                                                            *
*****************************************************************************/

#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <setjmp.h>
#include <sys/time.h>

#include "avcbe.h"		/* SuperH MEPG-4&H.264 Video Encode Library Header */
#include "m4iph_vpu4.h"		/* SuperH MEPG-4&H.264 Video Driver Library Header */
#include "avcbencsmp.h"		/* User Application Sample Header */

#include "avcbe_inner.h"
#include "QuantMatrix.h"

#include "capture.h"
#include "veu_colorspace.h"

#include <shcodecs/shcodecs_encoder.h>

#define DEBUG

extern char *dummy_nal_buf;
#define USE_BVOP

/*** Stream-output buffer to receive an encoding result in every one frame ***/

//static unsigned long sdr_read_my_stream_buff[MY_STREAM_BUFF_SIZE/4];          /* 4 bytes alignmen */   

/*** Other work-field area ***/
// #ifdef MULTI_STREAM             /* In the case of multiple streams */
// #define MY_MB_WORK_AREA_SIZE       78000   /* QCIF size DataPartioning=ON, Bitrate=64000/256000 : (6500 + 21500) + (9500 + 40000 + more) */
// #else   /* In the case of the number of the streams to encode is one */
// #define MY_MB_WORK_AREA_SIZE       28000   /* QCIF size DataPartioning=ON, Bitrate=64000 : 6500 + 21500 */
// #endif /* MULTI_STREAM */

//#define MY_MB_WORK_AREA_SIZE  76800  /*TODO hardcoded, remove later */
//static unsigned long my_mb_work_area[MY_MB_WORK_AREA_SIZE/4]; /* 4 bytes alignmen */
//#define my_sdr_mb_work_area sdr_base+(MY_MB_WORK_AREA_SIZE*2) /* 4 bytes alignmen */

extern avcbe_stream_info *my_context;
extern TAVCBE_FMEM LDEC_ARRY[];
extern TAVCBE_FMEM CAPTF_ARRY[];

unsigned long *my_frame_memory_capt[19];

unsigned long *my_frame_memory_ldec1;
unsigned long *my_frame_memory_ldec2;
unsigned long *my_frame_memory_ldec3;
unsigned long *my_stream_buff;
unsigned long *my_end_code_buff;
unsigned long *my_stream_buff_bak;
unsigned long *my_end_code_buff_bak;
extern unsigned long *my_work_area;	/* 4 bytes alignment */


int open_input_image_file(APPLI_INFO *);
int open_output_file(APPLI_INFO *);
void disp_context_info(void *context);

extern int GetFromCtrlFTop(const char *control_filepath,
			   ENC_EXEC_INFO * enc_exec_info,
			   long *stream_type);

/* Top of the user application sample source to encode */
/*int mpeg4_enc(void) */
APPLI_INFO ainfo;		/* User Application Data */

u_int32_t sdr_base;

unsigned long m4iph_vpu4_reg_base = 0xfe900000;
#define KERNEL_MEMORY_FOR_VPU_BOTTOM 0xadffffff

unsigned long *kernel_memory_for_vpu_top;
void get_new_stream_buf(avcbe_stream_info * context,
			char *previous_stream_buff, long output_size,
			char **next_stream_buff, long *stream_buff_size);

void file_name_copy(void)
{
	strcpy(ainfo.file_path_buf_1, ainfo.enc_exec_info.buf_input_yuv_file_with_path);	/* 入力ディレクトリ */
	strcpy(ainfo.file_path_buf_2, ainfo.enc_exec_info.buf_output_directry);	/* 出力ディレクトリ */
	strcpy(ainfo.input_file_name_buf, ainfo.file_path_buf_1);
	strcat(ainfo.input_file_name_buf, "\\");
	strcpy(ainfo.output_file_name_buf, ainfo.file_path_buf_2);
	strcat(ainfo.output_file_name_buf, "\\");
	strcpy(ainfo.local_decode_file_name_buf, ainfo.file_path_buf_2);
	strcat(ainfo.local_decode_file_name_buf, "\\");
	strcpy(ainfo.log_file_name_buf, ainfo.file_path_buf_2);
//              strcat(ainfo.log_file_name_buf, "\\");  // <--- NG
	strcpy(ainfo.capt_file_name_buf, ainfo.file_path_buf_2);
	strcat(ainfo.capt_file_name_buf, "\\");
	strcpy(ainfo.rate_log_file_name_buf, ainfo.file_path_buf_2);
	strcat(ainfo.rate_log_file_name_buf, "\\");

	printf("ainfo.input_file_name_buf = %s \n",
	       ainfo.input_file_name_buf);
	printf("ainfo.output_file_name_buf = %s \n",
	       ainfo.output_file_name_buf);
}

/* SHCodecs_Encoder_Input callback for acquiring an image from the CEU */
static int get_input(SHCodecs_Encoder * encoder,
		     unsigned long *addr_y, unsigned long *addr_c,
		     void *user_data)
{
	APPLI_INFO *appli_info = (APPLI_INFO *) user_data;
	return capture_image(appli_info, addr_y, addr_c);
}

/* SHCodecs_Encoder_Output callback for writing encoded data to the output file */
static int write_output(SHCodecs_Encoder * encoder,
			unsigned char *data, int length, void *user_data)
{
	APPLI_INFO *appli_info = (APPLI_INFO *) user_data;
	return fwrite(data, 1, length, appli_info->output_file_fp);
}

int main(int argc, char *argv[])
{
	int encode_return_code, success_count, i;
	char message_buf[256];
	int return_code;
	long stream_type, width_height, max_frame;	/* 041201 */
	SHCodecs_Encoder *encoder;

	if (argc == 2) {	/* 第1引数=コントロールファイル */
		strcpy(ainfo.ctrl_file_name_buf, argv[1]);
		return_code =
		    GetFromCtrlFTop((const char *)
				    ainfo.ctrl_file_name_buf,
				    &(ainfo.enc_exec_info), &stream_type);
		if (return_code < 0) {
			printf("Can't Open to Ctrolfile : %s\n", ainfo.ctrl_file_name_buf);	/* 041217 */
			return (-1);
		}
#if 1
		strcpy(ainfo.file_path_buf_1, ainfo.enc_exec_info.buf_input_yuv_file_with_path);	/* 入力ディレクトリ */
		strcpy(ainfo.file_path_buf_2, ainfo.enc_exec_info.buf_output_directry);	/* 出力ディレクトリ */
		strcpy(ainfo.input_file_name_buf, ainfo.file_path_buf_1);
		strcat(ainfo.input_file_name_buf, "/");
		strcat(ainfo.input_file_name_buf,
		       ainfo.enc_exec_info.buf_input_yuv_file);
		strcpy(ainfo.output_file_name_buf, ainfo.file_path_buf_2);
		strcat(ainfo.output_file_name_buf, "/");
		strcat(ainfo.output_file_name_buf,
		       ainfo.enc_exec_info.buf_output_stream_file);
#if 1
		strcpy(ainfo.local_decode_file_name_buf,
		       ainfo.file_path_buf_2);
		strcat(ainfo.local_decode_file_name_buf, "/");
		strcpy(ainfo.log_file_name_buf, ainfo.file_path_buf_2);
//              strcat(ainfo.log_file_name_buf, "/");   // <--- NG
		strcpy(ainfo.capt_file_name_buf, ainfo.file_path_buf_2);
		strcat(ainfo.capt_file_name_buf, "/");
		strcpy(ainfo.rate_log_file_name_buf,
		       ainfo.file_path_buf_2);
		strcat(ainfo.rate_log_file_name_buf, "/");
		printf
		    ("!!!!!!!! ainfo.rate_log_file_name_buf = %s !!!!!!!!\n",
		     ainfo.rate_log_file_name_buf);
		printf("ainfo.input_file_name_buf = %s \n",
		       ainfo.input_file_name_buf);
		printf("ainfo.output_file_name_buf = %s \n",
		       ainfo.output_file_name_buf);
#endif

#else
		file_name_copy();
#endif
	} else {
		DisplayMessage
		    ("usage argv[0] case_number input_base_dir output_base_dir",
		     1);
		return -1;
	}

	ainfo.ceu =
	    sh_ceu_open("/dev/video0", ainfo.enc_exec_info.xpic,
			ainfo.enc_exec_info.ypic);

	sh_veu_open();

	printf("1: Got CEU\n");

	encoder =
	    shcodecs_encoder_init(ainfo.enc_exec_info.xpic,
				  ainfo.enc_exec_info.ypic, stream_type);

	shcodecs_encoder_set_input_callback(encoder, get_input, &ainfo);
	shcodecs_encoder_set_output_callback(encoder, write_output,
					     &ainfo);

	/* stream buffer 0 clear */
//      memset(sdr_read_my_stream_buff,0,sizeof(sdr_read_my_stream_buff));
	encode_time_init();
	vpu4_clock_on();
	width_height = ainfo.enc_exec_info.xpic * ainfo.enc_exec_info.ypic;
	width_height += (width_height / 2);

	printf("width_height: %d\n", width_height);

	max_frame = 2;
	sdr_base = m4iph_sdr_malloc(width_height * (max_frame + 3), 32);
	if (sdr_base == NULL)
		exit(1);
	kernel_memory_for_vpu_top = (unsigned long *) sdr_base;
	for (i = 0; i < max_frame; i++) {
		my_frame_memory_capt[i] =
		    (unsigned long *) (sdr_base + width_height * i);
		printf("my_frame_memory_capt[%d]=%x\n", i,
		       my_frame_memory_capt[i]);
	}
	my_frame_memory_ldec1 =
	    (unsigned long *) (sdr_base + width_height * i);
	i++;
	my_frame_memory_ldec2 =
	    (unsigned long *) (sdr_base + width_height * i);
	i++;
	my_frame_memory_ldec3 =
	    (unsigned long *) (sdr_base + width_height * i);
	my_stream_buff_bak = malloc(MY_STREAM_BUFF_SIZE + 31);
	my_stream_buff = ALIGN(my_stream_buff_bak, 32);
	my_end_code_buff_bak = malloc(MY_END_CODE_BUFF_SIZE + 31);
	my_end_code_buff = ALIGN(my_end_code_buff_bak, 32);
	success_count = 0;

	/*--- open output file (one of the user application's own functions) ---*/
	return_code = open_output_file(&ainfo);
	if (return_code != 0) {	/* error */
		DisplayMessage("  encode_1file:open_output_file ERROR! ",
			       1);
		return (-6);
	}

	sh_ceu_start_capturing(ainfo.ceu);

	encode_return_code = shcodecs_encoder_run(encoder, &ainfo);

	if (encode_return_code < 0) {	/* encode error */
		sprintf(message_buf, "Encode Error  code=%d ",
			encode_return_code);
		DisplayMessage(message_buf, 1);
	} else {		/* encode success */
		sprintf(message_buf, "Encode Success ");
		DisplayMessage(message_buf, 1);
		success_count++;
	}

	sh_ceu_stop_capturing(ainfo.ceu);

	m4iph_sdr_free(sdr_base, width_height * (max_frame + 3));

	shcodecs_encoder_close(encoder);

	free(my_stream_buff_bak);
	free(my_end_code_buff_bak);
	free(my_work_area);
	free(dummy_nal_buf);
	printf("Total encode time = %d(msec)\n", encode_time_get());
	printf("Total sleep  time = %d(msec)\n", m4iph_sleep_time_get());
	/* TODO vpu4_reg_close(); */

	sh_veu_close();

	sh_ceu_close(ainfo.ceu);
}
