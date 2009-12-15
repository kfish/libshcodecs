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

#include "ControlFileUtil.h"

int open_input_image_file(APPLI_INFO *);
int load_1frame_from_image_file(SHCodecs_Encoder * encoder, APPLI_INFO * appli_info);

int open_output_file(APPLI_INFO *);
int select_inputfile_set_param(SHCodecs_Encoder * encoder,
			       APPLI_INFO * appli_info);


SHCodecs_Encoder *encoder; /* Encoder */
APPLI_INFO ainfo; /* Application Data */

static void
usage (const char * progname)
{
	printf ("Usage: %s <control file>\n", progname);
	printf ("Encode a video file using the SH-Mobile VPU\n");
	printf ("\nPlease report bugs to <linux-sh@vger.kernel.org>\n");
}

/* SHCodecs_Encoder_Input callback for acquiring an image from the input file */
static int get_input(SHCodecs_Encoder * encoder, void *user_data)
{
	APPLI_INFO *appli_info = (APPLI_INFO *) user_data;
	return load_1frame_from_image_file(encoder, appli_info);
}

/* SHCodecs_Encoder_Output callback for writing encoded data to the output file */
static int write_output(SHCodecs_Encoder * encoder,
			unsigned char *data, int length, void *user_data)
{
	APPLI_INFO *appli_info = (APPLI_INFO *) user_data;
	return fwrite(data, 1, length, appli_info->output_file_fp);
}

void cleanup (void)
{
	if (encoder != NULL)
		shcodecs_encoder_close(encoder);
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
	return_code = GetFromCtrlFTop((const char *)
				      ainfo.ctrl_file_name_buf,
				      &ainfo,
				      &stream_type);
	if (return_code < 0) {
		perror("Error opening control file");
		return (-1);
	}

	/* Input path */
	snprintf(ainfo.input_file_name_buf, 256, "%s/%s",
		 ainfo.buf_input_yuv_file_with_path,
		 ainfo.buf_input_yuv_file);
	fprintf(stderr, "Input file: %s\n", ainfo.input_file_name_buf);

	/* Output path */
	if (!strcmp (ainfo.buf_output_stream_file, "-")) {
		snprintf (ainfo.output_file_name_buf, 256, "-");
	} else {
		snprintf(ainfo.output_file_name_buf, 256, "%s/%s",
			 ainfo.buf_output_directry,
			 ainfo.buf_output_stream_file);
	}
	fprintf(stderr, "Output file: %s\n", ainfo.output_file_name_buf);

	encoder = NULL;
	ainfo.ceu = NULL;
	signal (SIGINT, sig_handler);
	signal (SIGPIPE, sig_handler);

	encoder = shcodecs_encoder_init(ainfo.xpic, ainfo.ypic, stream_type);

	shcodecs_encoder_set_input_callback(encoder, get_input, &ainfo);
	shcodecs_encoder_set_output_callback(encoder, write_output,
					     &ainfo);

	/*open input YUV data file */
	return_code = open_input_image_file(&ainfo);
	if (return_code != 0) {	/* error */
		perror("Error opening input file");
		return (-6);
	}

	/* open output file */
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

	encode_return_code = shcodecs_encoder_run(encoder);

	if (encode_return_code < 0) {
		fprintf(stderr, "Error encoding, error code=%d\n",
			encode_return_code);
	} else {
		fprintf(stderr, "Encode Success\n");
	}

	cleanup ();

	return 0;
}
