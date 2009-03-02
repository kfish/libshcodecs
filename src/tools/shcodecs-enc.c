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

#include <shcodecs/shcodecs_encoder.h>

extern avcbe_stream_info *my_context;
extern TAVCBE_FMEM LDEC_ARRY[];
extern TAVCBE_FMEM CAPTF_ARRY[];

int open_input_image_file(APPLI_INFO *);
int open_output_file(APPLI_INFO *);
void disp_context_info(void *context);

extern int GetFromCtrlFTop(const char *control_filepath,
			   ENC_EXEC_INFO * enc_exec_info,
			   long *stream_type);

APPLI_INFO ainfo;		/* User Application Data */

unsigned long m4iph_vpu4_reg_base = 0xfe900000;
#define KERNEL_MEMORY_FOR_VPU_BOTTOM 0xadffffff

/* SHCodecs_Encoder_Input callback for acquiring an image from the input file */
static int get_input(SHCodecs_Encoder * encoder,
		     unsigned long *addr_y, unsigned long *addr_c,
		     void *user_data)
{
	APPLI_INFO *appli_info = (APPLI_INFO *) user_data;
	return load_1frame_from_image_file(appli_info, addr_y, addr_c);
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
	int encode_return_code;
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

                /* 入力ディレクトリ */
                snprintf (ainfo.input_file_name_buf, 256, "%s/%s",
                          ainfo.enc_exec_info.buf_input_yuv_file_with_path,
                          ainfo.enc_exec_info.buf_input_yuv_file);

                /* 出力ディレクトリ */
                snprintf (ainfo.output_file_name_buf, 256, "%s/%s",
                          ainfo.enc_exec_info.buf_output_directry,
		          ainfo.enc_exec_info.buf_output_stream_file);

		printf("ainfo.input_file_name_buf = %s \n",
		       ainfo.input_file_name_buf);
		printf("ainfo.output_file_name_buf = %s \n",
		       ainfo.output_file_name_buf);
	} else {
		DisplayMessage
		    ("usage argv[0] case_number input_base_dir output_base_dir",
		     1);
		return -1;
	}

	encoder =
	    shcodecs_encoder_init(ainfo.enc_exec_info.xpic,
				  ainfo.enc_exec_info.ypic, stream_type, &ainfo);

	shcodecs_encoder_set_input_callback(encoder, get_input, &ainfo);
	shcodecs_encoder_set_output_callback(encoder, write_output,
					     &ainfo);

	/*--- open input YUV data file (one of the user application's own
	 *  functions) ---*/
	return_code = open_input_image_file(&ainfo);
	if (return_code != 0) {	/* error */
		DisplayMessage(" open_input_image_file ERROR! ", 1);
		return (-6);
	}

	/*--- open output file (one of the user application's own functions) ---*/
	return_code = open_output_file(&ainfo);
	if (return_code != 0) {	/* error */
		DisplayMessage("  encode_1file:open_output_file ERROR! ",
			       1);
		return (-6);
	}

	encode_return_code = shcodecs_encoder_run(encoder, &ainfo);

	if (encode_return_code < 0) {	/* encode error */
		sprintf(message_buf, "Encode Error  code=%d ",
			encode_return_code);
		DisplayMessage(message_buf, 1);
	} else {		/* encode success */
		sprintf(message_buf, "Encode Success ");
		DisplayMessage(message_buf, 1);
	}

	shcodecs_encoder_close(encoder);

	printf("Total encode time = %d(msec)\n", encode_time_get());
	printf("Total sleep  time = %d(msec)\n", m4iph_sleep_time_get());
	/* TODO vpu4_reg_close(); */
}
