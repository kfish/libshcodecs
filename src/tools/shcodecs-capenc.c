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

#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <setjmp.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include <shcodecs/shcodecs_encoder.h>

#include "avcbencsmp.h"		/* User Application Sample Header */

#include "capture.h"
#include <shveu/shveu.h>
#include <shveu/veu_colorspace.h>

#include "ControlFileUtil.h"

/* #define DEBUG */

#define USE_BVOP

#define U_SEC_PER_SEC 1000000

/* capture yuv data to the image-capture-field area each frame */
int capture_image(SHCodecs_Encoder * encoder, APPLI_INFO * appli_info);

int open_output_file(APPLI_INFO *);
int select_inputfile_set_param(SHCodecs_Encoder * encoder,
			       APPLI_INFO * appli_info);


SHCodecs_Encoder *encoder; /* Encoder */
APPLI_INFO ainfo; /* Application Data */

/* Timing */
int num_frames=0;
struct timeval start, finish, diff;

static void
usage (const char * progname)
{
	printf ("Usage: %s <control file>\n", progname);
	printf ("Encode V4L2 video input using the SH-Mobile VPU\n");
	printf ("\nPlease report bugs to <linux-sh@vger.kernel.org>\n");
}

/* SHCodecs_Encoder_Input callback for acquiring an image from the CEU */
static int get_input(SHCodecs_Encoder * encoder, void *user_data)
{
	APPLI_INFO *appli_info = (APPLI_INFO *) user_data;

	num_frames++;
	return capture_image(encoder, appli_info);
}

/* SHCodecs_Encoder_Output callback for writing encoded data to the output file */
static int write_output(SHCodecs_Encoder * encoder,
			unsigned char *data, int length, void *user_data)
{
	APPLI_INFO *appli_info = (APPLI_INFO *) user_data;

	if (fwrite(data, 1, length, appli_info->output_file_fp) < (size_t)length)
		return -1;

	return 0;
}

void cleanup (void)
{
	float time;

	gettimeofday(&finish, 0);

	timersub(&finish, &start, &diff);
	time = diff.tv_sec + (float)diff.tv_usec/U_SEC_PER_SEC;

	if (time > 0)
		fprintf(stderr, "%d frames captured at %.2f fps\n", num_frames, (float)num_frames/time);

	if (ainfo.ceu != NULL)
		sh_ceu_stop_capturing(ainfo.ceu);

	if (encoder != NULL)
		shcodecs_encoder_close(encoder);

	shveu_close();

	if (ainfo.ceu != NULL)
		sh_ceu_close(ainfo.ceu);
}

void sig_handler(int sig)
{
	cleanup ();

#ifdef DEBUG
	fprintf (stderr, "Got signal %d\n", sig);
#endif

	/* Send ourselves the signal: see http://www.cons.org/cracauer/sigint.html */
	signal(sig, SIG_DFL);
	kill(getpid(), sig);
}

int main(int argc, char *argv[])
{
	int encode_return_code;
	int return_code;
	long stream_type;

	if (argc != 2 || !strncmp (argv[1], "-h", 2) || !strncmp (argv[1], "--help", 6)) {
		usage (argv[0]);
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
	if (!strcmp (ainfo.buf_output_stream_file, "-")) {
		snprintf (ainfo.output_file_name_buf, 256, "-");
	} else {
		snprintf(ainfo.output_file_name_buf, 256, "%s/%s",
			 ainfo.buf_output_directry,
			 ainfo.buf_output_stream_file);
	}

	fprintf(stderr, "Input device: %s\n", ainfo.input_file_name_buf);
	fprintf(stderr, "Output file: %s\n", ainfo.output_file_name_buf);

	encoder = NULL;
	ainfo.ceu = NULL;
	signal (SIGINT, sig_handler);
	signal (SIGPIPE, sig_handler);

	ainfo.ceu = sh_ceu_open(ainfo.input_file_name_buf, ainfo.xpic, ainfo.ypic);

	shveu_open();

	encoder =
	    shcodecs_encoder_init(ainfo.xpic, ainfo.ypic,
				  stream_type);

	shcodecs_encoder_set_input_callback(encoder, get_input, &ainfo);
	shcodecs_encoder_set_output_callback(encoder, write_output,
					     &ainfo);

	/*--- open output file ---*/
	return_code = open_output_file(&ainfo);
	if (return_code != 0) {	/* error */
		perror("Error opening output file");
		return (-6);
	}

	/* set parameters for use in encoding */
	return_code = select_inputfile_set_param(encoder, &ainfo);
	if (return_code == -1) {	/* error */
		fprintf (stderr, "select_inputfile_set_param ERROR! \n");
		return (-3);
	}

	sh_ceu_start_capturing(ainfo.ceu);

	gettimeofday(&start, 0);
	encode_return_code = shcodecs_encoder_run(encoder);

	if (encode_return_code < 0) {
		fprintf(stderr, "Error encoding, error code=%d\n",
			encode_return_code);
	} else {
		fprintf(stderr, "Encode Success\n");
	}

	cleanup ();

#if 0
	printf("Total encode time = %d(msec)\n", encode_time_get());
	printf("Total sleep  time = %d(msec)\n", m4iph_sleep_time_get());
	/* TODO vpu4_reg_close(); */
#endif

	return 0;
}
