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

#include "ControlFileUtil.h"

#include <shcodecs/shcodecs_encoder.h>

#define DEBUG

#define USE_BVOP

int open_output_file(APPLI_INFO *);

APPLI_INFO ainfo;		/* User Application Data */


/* SHCodecs_Encoder_Input callback for acquiring an image from the CEU */
static int get_input(SHCodecs_Encoder * encoder,
		     unsigned long *addr_y, unsigned long *addr_c,
		     void *user_data)
{
	APPLI_INFO *appli_info = (APPLI_INFO *) user_data;
	return capture_image(encoder, appli_info, addr_y, addr_c);
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
	SHCodecs_Encoder *encoder;
	int encode_return_code;
	int return_code;
	long stream_type;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s control-file.ctl", argv[0]);
		return -1;
	}

	strcpy(ainfo.ctrl_file_name_buf, argv[1]);
	return_code = GetFromCtrlFTop((const char *) ainfo.ctrl_file_name_buf,
				      &ainfo, &stream_type);
	if (return_code < 0) {
		perror("Error opening control file");
		return (-1);
	}

	/* 入力ディレクトリ */
	snprintf(ainfo.input_file_name_buf, 256, "%s/%s",
		 ainfo.buf_input_yuv_file_with_path,
		 ainfo.buf_input_yuv_file);

	/* 出力ディレクトリ */
	snprintf(ainfo.output_file_name_buf, 256, "%s/%s",
		 ainfo.buf_output_directry,
		 ainfo.buf_output_stream_file);

	printf("ainfo.input_file_name_buf = %s \n",
	       ainfo.input_file_name_buf);
	printf("ainfo.output_file_name_buf = %s \n",
	       ainfo.output_file_name_buf);
	ainfo.ceu =
	    sh_ceu_open("/dev/video0", ainfo.xpic, ainfo.ypic);

	sh_veu_open();

	encoder =
	    shcodecs_encoder_init(ainfo.xpic, ainfo.ypic,
                                  stream_type, &ainfo);

	shcodecs_encoder_set_input_callback(encoder, get_input, &ainfo);
	shcodecs_encoder_set_output_callback(encoder, write_output,
					     &ainfo);

	/*--- open output file ---*/
	return_code = open_output_file(&ainfo);
	if (return_code != 0) {	/* error */
		perror("Error opening output file");
		return (-6);
	}

	sh_ceu_start_capturing(ainfo.ceu);

	encode_return_code = shcodecs_encoder_run(encoder, &ainfo);

	if (encode_return_code < 0) {
		fprintf(stderr, "Error encoding, error code=%d\n",
			encode_return_code);
	} else {
		fprintf(stderr, "Encode Success\n");
	}

	sh_ceu_stop_capturing(ainfo.ceu);

	shcodecs_encoder_close(encoder);

	printf("Total encode time = %d(msec)\n", encode_time_get());
	printf("Total sleep  time = %d(msec)\n", m4iph_sleep_time_get());
	/* TODO vpu4_reg_close(); */

	sh_veu_close();

	sh_ceu_close(ainfo.ceu);
}
