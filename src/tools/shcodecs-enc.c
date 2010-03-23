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
	printf ("Usage: %s <control file> ...\n", progname);
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

static struct shenc * setup_enc(char * filename)
{
	struct shenc * shenc;
	int ret;

	shenc = calloc (1, sizeof(*shenc));
	if (!shenc)
		return NULL;

	ret = ctrlfile_get_params(filename, &shenc->ainfo, &shenc->stream_type);
	if (ret < 0) {
		fprintf(stderr, "Error opening control file");
		goto err;
	}

	fprintf(stderr, "Input file: %s\n", shenc->ainfo.input_file_name_buf);
	fprintf(stderr, "Output file: %s\n", shenc->ainfo.output_file_name_buf);

	/* Setup encoder */
	shenc->encoder = shcodecs_encoder_init(shenc->ainfo.xpic, shenc->ainfo.ypic, shenc->stream_type);
	if (!shenc->encoder) {
		fprintf(stderr, "Error initialising encoder");
		goto err;
	}

	shcodecs_encoder_set_input_callback(shenc->encoder, get_input, shenc);
	shcodecs_encoder_set_output_callback(shenc->encoder, write_output, shenc);

	/* set parameters for use in encoding */
	ret = ctrlfile_set_enc_param(shenc->encoder, filename);
	if (ret < 0) {
		fprintf(stderr, "Problem with encoder params in control file!\n");
		goto err;
	}

	/* open input YUV data file */
	ret = open_input_image_file(&shenc->ainfo);
	if (ret != 0) {
		fprintf(stderr, "Error opening input file");
		goto err;
	}

	/* open output file */
	shenc->output_fp = open_output_file(shenc->ainfo.output_file_name_buf);
	if (!shenc->output_fp) {
		fprintf(stderr, "Error opening output file");
		goto err;
	}

	return shenc;

err:
	cleanup(shenc);
	return NULL;
}

void *convert_main(void *data)
{
	struct shenc * shenc = (struct shenc *)data;
	int ret;

	ret = shcodecs_encoder_run (shenc->encoder);
	cleanup (shenc);

	return (void *)ret;
}

int main(int argc, char *argv[])
{
	char * progname = argv[0];
	int show_help = 0, show_version = 0;
	struct shenc ** shencs;
	int i, nr_encoders, ret;
	pthread_t * threads;
	void *thread_ret;

	if (argc == 1) {
		usage(progname);
		return 0;
	}

	/* Check for help or version */
	for (i=1; i<argc; i++) {
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
			show_help = 1;

		if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
			show_version = 1;
	}
	if (show_version)
		printf ("%s version " VERSION "\n", progname);

	if (show_help)
		usage (progname);

	if (show_version || show_help)
		return 0;


	signal (SIGINT, sig_handler);
	signal (SIGPIPE, sig_handler);

	nr_encoders = argc-1;

	shencs = calloc (nr_encoders, sizeof (struct shenc *));
	if (shencs == NULL) {
		perror(NULL);
		exit (-1);
	}

	threads = calloc (nr_encoders, sizeof (pthread_t));
	if (threads == NULL) {
		perror(NULL);
		exit (-1);
	}

	for (i=0; i < nr_encoders; i++) {
		shencs[i] = setup_enc (argv[i+1]);
		if (shencs[i]) {
			ret = pthread_create(&threads[i], NULL, convert_main, shencs[i]);
			if (ret)
				fprintf(stderr, "Thread %d failed: %s\n", i, strerror(ret));
		}
	}

	ret = 0;
	for (i=0; i < nr_encoders; i++) {
		if (threads[i] != NULL) {
			pthread_join(threads[i], &thread_ret);
			if ((int)thread_ret < 0) {
				ret = (int)thread_ret;
				fprintf(stderr, "Error encoding %d\n", i);
			} else {
				fprintf(stderr, "Encode %d Success\n", i);
			}
		}
	}

	free (shencs);

	return ret;
}
