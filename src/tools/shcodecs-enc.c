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

#include "ControlFileUtil.h"
#include "avcbencsmp.h"
#include "framerate.h"


SHCodecs_Encoder *encoder; /* Encoder */
APPLI_INFO ainfo;	/* Control file data */
FILE *output_fp;
struct framerate * enc_framerate = NULL;

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

	if (enc_framerate == NULL) {
		enc_framerate = framerate_new_measurer ();
	}

	return load_1frame_from_image_file(encoder, appli_info);
}

/* SHCodecs_Encoder_Output callback for writing encoded data to the output file */
static int write_output(SHCodecs_Encoder * encoder,
			unsigned char *data, int length, void *user_data)
{
	APPLI_INFO *appli_info = (APPLI_INFO *) user_data;
	double ifps, mfps;

	if (shcodecs_encoder_get_frame_num_delta(encoder) > 0 &&
			enc_framerate != NULL) {
		framerate_mark (enc_framerate);
		ifps = framerate_instantaneous_fps (enc_framerate);
		mfps = framerate_mean_fps (enc_framerate);
		if (enc_framerate->nr_handled % 10 == 0) {
			fprintf (stderr, "  Encoding @ %4.2f fps \t(avg %4.2f fps)\r", ifps, mfps);
		}
	}

	if (fwrite(data, 1, length, output_fp) == (size_t)length) {
		return 0;
	} else {
		return -1;
	}
}

void cleanup (void)
{
	double time;

	time = (double)framerate_elapsed_time (enc_framerate);
	time /= 1000000;

	fprintf (stderr, "Elapsed time (encode): %0.3g s\n", time);
	fprintf (stderr, "Encoded %d frames (%.2f fps)\n",
			enc_framerate->nr_handled,
		 	framerate_mean_fps (enc_framerate));
	framerate_destroy (enc_framerate);

	if (encoder != NULL)
		shcodecs_encoder_close(encoder);

	close_output_file(output_fp);
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
	int return_code;
	long stream_type;
	const char *ctrl_filename;

	if (argc != 2 || !strncmp (argv[1], "-h", 2) || !strncmp (argv[1], "--help", 6)) {
		usage (argv[0]);
		return -1;
	}

	ctrl_filename = argv[1];
	return_code = ctrlfile_get_params(ctrl_filename, &ainfo, &stream_type);
	if (return_code < 0) {
		perror("Error opening control file");
		return (-1);
	}

	fprintf(stderr, "Input file: %s\n", ainfo.input_file_name_buf);
	fprintf(stderr, "Output file: %s\n", ainfo.output_file_name_buf);

	encoder = NULL;
	signal (SIGINT, sig_handler);
	signal (SIGPIPE, sig_handler);

	/* Setup encoder */
	encoder = shcodecs_encoder_init(ainfo.xpic, ainfo.ypic, stream_type);

	shcodecs_encoder_set_input_callback(encoder, get_input, &ainfo);
	shcodecs_encoder_set_output_callback(encoder, write_output, &ainfo);

	return_code = ctrlfile_set_enc_param(encoder, ctrl_filename);
	if (return_code < 0) {
		perror("Problem with encoder params in control file!\n");
		return (-3);
	}

	/* open input YUV data file */
	return_code = open_input_image_file(&ainfo);
	if (return_code != 0) {
		perror("Error opening input file");
		return (-6);
	}

	/* open output file */
	output_fp = open_output_file(ainfo.output_file_name_buf);
	if (output_fp == NULL) {
		perror("Error opening output file");
		return (-6);
	}

	return_code = shcodecs_encoder_run(encoder);

	if (return_code < 0) {
		fprintf(stderr, "Error encoding, error code=%d\n", return_code);
	} else {
		fprintf(stderr, "Encode Success\n");
	}

	cleanup ();

	return 0;
}
