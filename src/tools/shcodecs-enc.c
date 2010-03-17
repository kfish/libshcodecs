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
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <setjmp.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include <shcodecs/shcodecs_encoder.h>

#include "ControlFileUtil.h"
#include "avcbencsmp.h"
#include "framerate.h"

static void
usage (const char * progname)
{
	printf ("Usage: %s <control file>\n", progname);
	printf ("Encode a video file using the SH-Mobile VPU\n");
	printf ("\nMiscellaneous options\n");
	printf ("  -h, --help             Display this help and exit\n");
	printf ("  -v, --version          Output version information and exit\n");
	printf ("\nPlease report bugs to <linux-sh@vger.kernel.org>\n");
}

struct shenc {
        APPLI_INFO ainfo; /* Application Data */
        SHCodecs_Encoder *encoder; /* Encoder */
	FILE * output_fp;
	struct framerate * enc_framerate;
	long stream_type;
};

struct shenc *
shenc_new (void)
{
        struct shenc * shenc;

        shenc = malloc (sizeof(*shenc));
        if (shenc == NULL) return NULL;

        memset (shenc, 0, sizeof(*shenc));

        return shenc;
}

/* SHCodecs_Encoder_Input callback for acquiring an image from the input file */
static int get_input(SHCodecs_Encoder * encoder, void *user_data)
{
	struct shenc * shenc = (struct shenc *)user_data;

	if (shenc->enc_framerate == NULL) {
		shenc->enc_framerate = framerate_new_measurer ();
	}

	return load_1frame_from_image_file(shenc->encoder, &shenc->ainfo);
}

/* SHCodecs_Encoder_Output callback for writing encoded data to the output file */
static int write_output(SHCodecs_Encoder * encoder,
			unsigned char *data, int length, void *user_data)
{
	struct shenc * shenc = (struct shenc *)user_data;
	double ifps, mfps;

	if (shcodecs_encoder_get_frame_num_delta(encoder) > 0 &&
			shenc->enc_framerate != NULL) {
		framerate_mark (shenc->enc_framerate);
		ifps = framerate_instantaneous_fps (shenc->enc_framerate);
		mfps = framerate_mean_fps (shenc->enc_framerate);
		if (shenc->enc_framerate->nr_handled % 10 == 0) {
			fprintf (stderr, "  Encoding @ %4.2f fps \t(avg %4.2f fps)\r", ifps, mfps);
		}
	}

	if (fwrite(data, 1, length, shenc->output_fp) == (size_t)length) {
		return 0;
	} else {
		return -1;
	}
}

void cleanup (struct shenc * shenc)
{
	double time;

	if (shenc->enc_framerate) {
		time = (double)framerate_elapsed_time (shenc->enc_framerate);
		time /= 1000000;

		fprintf (stderr, "Elapsed time (encode): %0.3g s\n", time);
		fprintf (stderr, "Encoded %d frames (%.2f fps)\n",
				shenc->enc_framerate->nr_handled,
			 	framerate_mean_fps (shenc->enc_framerate));

		framerate_destroy (shenc->enc_framerate);
	}

	if (shenc->encoder != NULL)
		shcodecs_encoder_close(shenc->encoder);

	close_output_file(shenc->output_fp);

	free (shenc);
}

void sig_handler(int sig)
{
	//cleanup ();

#ifdef DEBUG
	fprintf (stderr, "Got signal %d\n", sig);
#endif

	/* Send ourselves the signal: see http://www.cons.org/cracauer/sigint.html */
	signal(sig, SIG_DFL);
	kill(getpid(), sig);
}

static struct shenc * setup_file (char * filename)
{
	struct shenc * shenc;
	int return_code;

	shenc = shenc_new();

	return_code = ctrlfile_get_params(filename, &shenc->ainfo, &shenc->stream_type);
	if (return_code < 0) {
		perror("Error opening control file");
		exit (-1);
	}

	fprintf(stderr, "Input file: %s\n", shenc->ainfo.input_file_name_buf);
	fprintf(stderr, "Output file: %s\n", shenc->ainfo.output_file_name_buf);

	shenc->encoder = NULL;
	signal (SIGINT, sig_handler);
	signal (SIGPIPE, sig_handler);

	/* Setup encoder */
	shenc->encoder = shcodecs_encoder_init(shenc->ainfo.xpic, shenc->ainfo.ypic, shenc->stream_type);

	shcodecs_encoder_set_input_callback(shenc->encoder, get_input, shenc);
	shcodecs_encoder_set_output_callback(shenc->encoder, write_output, shenc);

	/* set parameters for use in encoding */
	return_code = ctrlfile_set_enc_param(shenc->encoder, filename);
	if (return_code < 0) {
		perror("Problem with encoder params in control file!\n");
		exit (-3);
	}

	/* open input YUV data file */
	return_code = open_input_image_file(&shenc->ainfo);
	if (return_code != 0) {
		perror("Error opening input file");
		exit (-6);
	}

	/* open output file */
	shenc->output_fp = open_output_file(shenc->ainfo.output_file_name_buf);
	if (shenc->output_fp == NULL) {
		perror("Error opening output file");
		exit (-6);
	}

	return shenc;
}

void *convert_main(void *data)
{
	struct shenc * shenc = (struct shenc *)data;
	int ret;

	ret = shcodecs_encoder_run (shenc->encoder);
	if (ret < 0) {
		fprintf(stderr, "Error encoding, error code=%d\n", ret);
	} else {
		fprintf(stderr, "Encode Success\n");
	}

	cleanup (shenc);
	return NULL;
}

int main(int argc, char *argv[])
{
	struct shenc ** shencs;
	int i, nr_encoders, ret;
	pthread_t * threads;

	if (argc < 2 || !strncmp (argv[1], "-h", 2) || !strncmp (argv[1], "--help", 6)) {
		usage (argv[0]);
		return -1;
        }

	if (!strncmp (argv[1], "-v", 2) || !strncmp (argv[1], "--version", 9)) {
		printf ("%s version " VERSION "\n", argv[0]);
		return -1;
	}

        signal (SIGINT, sig_handler);
        signal (SIGPIPE, sig_handler);

	nr_encoders = argc-1;

	shencs = calloc (sizeof (struct shenc *), nr_encoders);
	if (shencs == NULL) {
		fprintf (stderr, "Out of memory\n");
		exit (-1);
	}

	threads = calloc (sizeof (pthread_t), nr_encoders);
	if (threads == NULL) {
		fprintf (stderr, "Out of memory\n");
		exit (-1);
	}

	for (i=0; i < nr_encoders; i++) {
		shencs[i] = setup_file (argv[i+1]);

		ret = pthread_create(&threads[i], NULL, convert_main, shencs[i]);
		if (ret){
			fprintf(stderr, "pthread_create failed (%d)\n", ret);
			exit (-1);
		}
	}

	for (i=0; i < nr_encoders; i++) {
		if (threads[i] != NULL)
			pthread_join(threads[i], NULL);
	}

	free (shencs);

	return 0;
}
