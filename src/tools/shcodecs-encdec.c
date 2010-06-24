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
#include <getopt.h>
#include <fcntl.h>
#include <setjmp.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include <shcodecs/shcodecs_encoder.h>
#include <shcodecs/shcodecs_decoder.h>

#include "ControlFileUtil.h"
#include "avcbencsmp.h"
#include "framerate.h"

/* #define DEBUG */
#ifdef DEBUG
#define debug_printf	printf
#else
#define debug_printf(...)	/* none */
#endif

static void
usage (const char * progname)
{
	printf ("Usage: %s [options] ...\n", progname);
	printf ("Encode or decode an H.264 or MPEG-4 Elementary Stream using the SH-Mobile VPU\n");
	printf ("Separate decoder/encoder instances can be specified by separating the options\n");
	printf ("with ' , '.\n");
	printf ("\nEncode options\n");
	printf ("~~~~~~~~~~~~~~\n");
	printf ("  <control file>         File specifying input & output file & settings\n");
	printf ("\nDecode options\n");
	printf ("~~~~~~~~~~~~~~\n");
	printf ("\n File options\n");
	printf ("  -i, --input            Set the input filename\n");
	printf ("  -o, --output           Set the output filename\n");
	printf ("\nEncoding format\n");
	printf ("  -f, --format           Set the encoding format [h264, mpeg4]\n");
	printf ("\nDimensions\n");
	printf ("  -w, --width            Set the input image width in pixels\n");
	printf ("  -h, --height           Set the input image height in pixels\n");
	printf ("  -s, --size             Set the input image size [qcif, cif, qvga, vga, 720p]\n");
	printf ("\nMiscellaneous options\n");
	printf ("  --help                 Display this help and exit\n");
	printf ("  -v, --version          Output version information and exit\n");
	printf ("\nFile extensions are interpreted as follows unless otherwise specified:\n");
	printf ("  .m4v    MPEG4\n");
	printf ("  .264    H.264\n");
	printf ("\nPlease report bugs to <linux-sh@vger.kernel.org>\n");
}

static char * optstring = "f:o:i:w:h:s:Hv";

#ifdef HAVE_GETOPT_LONG
static struct option long_options[] = {
	{ "format", required_argument, NULL, 'f'},
	{ "output", required_argument, NULL, 'o'},
	{ "input" , required_argument, NULL, 'i'},
	{ "width" , required_argument, NULL, 'w'},
	{ "height", required_argument, NULL, 'h'},
	{ "size", required_argument, NULL, 's'},
	{ "help", no_argument, 0, 'H'},
	{ "version", no_argument, 0, 'v'},
};
#endif


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
		fprintf(stderr, "Error opening control file (%s)\n", filename);
		goto err;
	}

	fprintf(stderr, "Input file: %s\n", shenc->ainfo.input_file_name_buf);
	fprintf(stderr, "Output file: %s\n", shenc->ainfo.output_file_name_buf);

	/* Setup encoder */
	shenc->encoder = shcodecs_encoder_init(shenc->ainfo.xpic, shenc->ainfo.ypic, shenc->stream_type);
	if (!shenc->encoder) {
		fprintf(stderr, "Error initialising encoder\n");
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
		fprintf(stderr, "Error opening input file\n");
		goto err;
	}

	/* open output file */
	shenc->output_fp = open_output_file(shenc->ainfo.output_file_name_buf);
	if (!shenc->output_fp) {
		fprintf(stderr, "Error opening output file\n");
		goto err;
	}

	return shenc;

err:
	cleanup(shenc);
	return NULL;
}


/***********************************************************/

#define DEFAULT_WIDTH 320
#define DEFAULT_HEIGHT 240


/* getopt isn't thread safe */
pthread_mutex_t mutex;

struct argst {
	int argc;
	char **argv;
};

struct dec_opts {
	char *file_in;
	char *file_out;
	int w;
	int h;
	int format;
};

struct shdec {
	int		input_fd;	/* Input file descriptor */
	int		output_fd;	/* Output file descriptor */
	unsigned char	*input_buffer;	/* Pointer to input buffer */
	size_t		si_isize;	/* Total size of input data */

	long max_nal_size;
	long total_input_consumed;
	long total_output_bytes;
};


/* local output callback */
static int
local_vpu4_decoded (SHCodecs_Decoder * decoder,
		    unsigned char * y_buf, int y_size,
		    unsigned char * c_buf, int c_size,
		    void * user_data)
{
	struct shdec * dec = (struct shdec *)user_data;
	ssize_t len;

	if (dec->output_fd != -1) {
		len = write(dec->output_fd, y_buf, y_size);
		write(dec->output_fd, c_buf, c_size);
	}

	dec->total_output_bytes += y_size+c_size;

	return 0;
}

/***********************************************************/
int get_dec_opts(int argc, char **argv, struct dec_opts *opts)
{
	int c, i;

	opts->file_in = NULL;
	opts->file_out = NULL;
	opts->w = DEFAULT_WIDTH;
	opts->h = DEFAULT_HEIGHT;
	opts->format = -1;

	while (1) {
#ifdef HAVE_GETOPT_LONG
		c = getopt_long(argc, argv, optstring, long_options, &i);
#else
		c = getopt (argc, argv, optstring);
#endif
		if (c == -1)
			break;
		if (c == ':') {
			return -1;
		}
		switch (c) {
		case 'f':
			if (strncmp(optarg, "mpeg4", 5) == 0)
				opts->format = SHCodecs_Format_MPEG4;
			else if (strncmp(optarg, "h264", 4) == 0)
				opts->format = SHCodecs_Format_H264;
			break;
		case 'o':
			opts->file_out = optarg;
			break;
		case 'i':
			opts->file_in = optarg;
			break;
		case 'w':
			if (optarg)
				opts->w = strtoul(optarg, NULL, 10);
			break;
		case 'h':
			if (optarg)
				opts->h = strtoul(optarg, NULL, 10);
			break;
		case 's':
			if (optarg) {
				if (!strncasecmp (optarg, "qcif", 4)) {
					opts->w = 176;
					opts->h = 144;
				} else if (!strncmp (optarg, "cif", 3)) {
					opts->w = 352;
					opts->h = 288;
				} else if (!strncmp (optarg, "qvga", 4)) {
					opts->w = 320;
					opts->h = 240;
				} else if (!strncmp (optarg, "vga", 3)) {
					opts->w = 640;
					opts->h = 480;
				} else if (!strncmp (optarg, "720p", 4)) {
					opts->w = 1280;
					opts->h = 720;
				}
			}
			break;
		default:
			return -1;
		}
	}

	while (optind < argc) {
		opts->file_in = argv[optind++];
	}

	if (optind > argc){
		fprintf(stderr, "Too many arguments.\n");
		return -1;
	}

	if ( (opts->file_in == NULL) || !strcmp(opts->file_in, "-") ) {
		fprintf(stderr, "Invalid input file.\n");
		return -1;
	}
	if (opts->w < SHCODECS_MIN_FX || opts->w > SHCODECS_MAX_FX ||
	    opts->h < SHCODECS_MIN_FY || opts->h > SHCODECS_MAX_FY) {
		fprintf(stderr, "Invalid width and/or height specified.\n");
		return -1;
	}
	if (opts->format == -1) {
		char *ext = strrchr (opts->file_in, '.');
		if (ext == NULL || !strncmp (ext, ".264", 4))
			opts->format = SHCodecs_Format_H264;
		else
			opts->format = SHCodecs_Format_MPEG4;
	}

	return 0;
}

static int open_input(struct shdec * dec, char *input_filename)
{
	dec->input_fd = open(input_filename, O_RDONLY);
	if (dec->input_fd == -1) {
		perror(input_filename);
		return -1;
	}
	return 0;
}
 
static int open_output(struct shdec * dec, char *output_filename)
{
	dec->output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (dec->output_fd == -1) {
		perror(output_filename);
		return -1;
	}
	return 0;
}

static int
local_init (struct shdec * dec, char *input_filename, char *output_filename)
{
	struct timeval tv;
	struct timezone tz;

	/* Open input/output stream */
	dec->input_fd = dec->output_fd = -1;
	if (input_filename != NULL && open_input(dec, input_filename))
		return -50;
	if (output_filename != NULL && open_output(dec, output_filename))
		return -51;

	/* Allocate memory for input buffer */
	dec->input_buffer = malloc(dec->max_nal_size);
	if (dec->input_buffer == NULL)
		return -1;

	if (dec->input_fd != -1) {
		/* Fill input buffer */
		if ((dec->si_isize = read(dec->input_fd, dec->input_buffer, dec->max_nal_size)) <= 0) {
			perror(input_filename);
			return -54;
		}
	}

	gettimeofday(&tv, &tz);
	debug_printf("decode start %ld,%ld\n",tv.tv_sec,tv.tv_usec);

	return 0;
}
	
	
static int
local_close (struct shdec * dec)
{
	struct timeval tv;
	struct timezone tz;

	if (dec->output_fd != -1 && dec->output_fd > 2)
		close(dec->output_fd);
	if (dec->input_fd != -1 && dec->input_fd > 2)
		close(dec->input_fd);

	if (dec->input_buffer)
		free(dec->input_buffer);

	gettimeofday(&tv, &tz);
	debug_printf("%ld,%ld\n",tv.tv_sec,tv.tv_usec);
	return 0;
}

int decode(struct dec_opts *opts)
{
	struct shdec dec1;
	struct shdec * dec = &dec1;
	SHCodecs_Decoder * decoder;
	int bytes_decoded, frames_decoded;
	ssize_t n;

	debug_printf("Format: %s\n", opts->format == SHCodecs_Format_H264 ? "H.264" : "MPEG4");
	debug_printf("Resolution: %dx%d\n", opts->w, opts->h);
	debug_printf("Input  file: %s\n", opts->file_in);
	debug_printf("Output file: %s\n", opts->file_out);

	/* H.264 spec: Max NAL size is the size of an uncomrpessed immage divided
	   by the "Minimum Compression Ratio", MinCR. This is 2 for most levels
	   but is 4 for levels 3.1 to 4. Since we don't know the level, we just
	   use MinCR=2. */
	dec->max_nal_size = (opts->w * opts->h * 3) / 2; /* YCbCr420 */
	dec->max_nal_size /= 2;                          /* Apply MinCR */

	dec->total_input_consumed = 0;
	dec->total_output_bytes = 0;

	/* Open file descriptors to talk to the VPU and SDR drivers */

	if ((decoder = shcodecs_decoder_init(opts->w, opts->h, opts->format)) == NULL) {
		return -1;
	}

	if (local_init(dec, opts->file_in, opts->file_out) < 0)
		return -1;

	shcodecs_decoder_set_decoded_callback (decoder, local_vpu4_decoded, dec);

	/* decode main loop */
	do {
		int rem;

		bytes_decoded = shcodecs_decode (decoder, dec->input_buffer, dec->si_isize);
		frames_decoded = shcodecs_decoder_get_frame_count (decoder);
		if (bytes_decoded > 0) dec->total_input_consumed += bytes_decoded;
		
		rem = dec->si_isize - bytes_decoded;
		memmove(dec->input_buffer, dec->input_buffer + bytes_decoded, rem);
		n = read (dec->input_fd, dec->input_buffer + rem, dec->max_nal_size - rem);
		if (n < 0) break;

		dec->si_isize = rem + n;
	} while (!(n == 0 && bytes_decoded == 0));

	bytes_decoded = shcodecs_decode (decoder, dec->input_buffer, dec->si_isize);
	if (bytes_decoded > 0) dec->total_input_consumed += bytes_decoded;

	/* Finalize the decode output, in case a final frame is available */
	shcodecs_decoder_finalize (decoder);

	frames_decoded = shcodecs_decoder_get_frame_count (decoder);
	debug_printf("Total frames decoded: %d\n", frames_decoded);

	local_close (dec);

	shcodecs_decoder_close(decoder);

	debug_printf("Total bytes consumed: %ld\n", dec->total_input_consumed);
	debug_printf("Total bytes output: %ld\n", dec->total_output_bytes);

	return 0;
}

int dec_main(int argc, char **argv)
{
	struct dec_opts opts;
	int ret;

	/* getopt isn't thread safe & it's index needs resetting */
	pthread_mutex_lock(&mutex);
	optind = 1;
	ret = get_dec_opts(argc, argv, &opts);
	pthread_mutex_unlock(&mutex);

	if (ret == 0)
		ret = decode(&opts);

	return ret;
}

void *dec_thread(void *data)
{
	struct argst *args = (struct argst *) data;
	
	return (void *)dec_main(args->argc, args->argv);
}

pthread_t *
create_dec_thread(int nr, int argc, char **argv)
{
	struct argst * args;
	pthread_t * thread;
	int ret;

	int n;
	fprintf(stderr, "%d Decode: ", nr);
	for (n=1; n<argc; n++)
		fprintf(stderr, "%s ", argv[n]);
	fprintf(stderr, "\n");

	args = malloc (sizeof(*args));
	if (!args)
		return NULL;
	args->argc = argc;
	args->argv = argv;

	thread = malloc (sizeof(*thread));
	if (thread) {
		ret = pthread_create(thread, NULL, dec_thread, args);
		if (ret < 0) {
			perror(NULL);
			free(args);
			free(thread);
			thread = NULL;
		}
	}

	return thread;
}

void *enc_thread(void *data)
{
	char * ctl_file = data;
	struct shenc *shenc;
	int ret = -1;

	shenc = setup_enc (ctl_file);
	if (shenc) {
		ret = shcodecs_encoder_run (shenc->encoder);
		cleanup (shenc);
	}

	return (void *)ret;
}

pthread_t *
create_enc_thread(int nr, char *ctl_file)
{
	pthread_t * thread;
	int ret;

	fprintf(stderr, "%d Encode: %s\n", nr, ctl_file);

	thread = malloc (sizeof(*thread));
	if (thread) {
		ret = pthread_create(thread, NULL, enc_thread, ctl_file);
		if (ret < 0) {
			perror(NULL);
			free(thread);
			thread = NULL;
		}
	}

	return thread;
}


int main(int argc, char **argv)
{
	static const char * type[2] = {"Encode", "Decode"};
	int ret=0, i, j;
	char * progname = argv[0];
	int show_help = 0, show_version = 0;
	int nr_instances;
	int idx = 0;
	pthread_t ** threads;
	int * is_dec;
	void *thread_ret;

	if (argc == 1) {
		usage(progname);
		return 0;
	}

	/* -h is used for decoder height. If user just uses -h, show the help */
	if (argc == 2) {
		if (!strcmp(argv[1], "-h"))
			show_help = 1;
	}

	/* Check for help or version */
	for (i=1; i<argc; i++) {
		if (!strcmp(argv[i], "-H") || !strcmp(argv[i], "--help"))
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


	is_dec = calloc (argc, sizeof (*is_dec));
	threads = calloc (argc, sizeof (*threads));
	if (!is_dec || !threads) {
		perror(NULL);
		return -1;
	}

	pthread_mutex_init(&mutex, NULL);

	i = 0;
	idx = 0;

	for (j=1; j<argc; j++) {
		int len;
		/* Look for arg ending in .ctl */
		if (((len = strlen(argv[j]) - 4) > 0)
		    && (strcmp(argv[j] + len, ".ctl") == 0)) {

			/* Have we had some args before this ctl file? */
			if (idx != j-1) {
				is_dec[i] = 1;
				threads[i] = create_dec_thread(i, j-idx, &argv[idx]);
				i++;
			}

			is_dec[i] = 0;
			threads[i] = create_enc_thread(i, argv[j]);
			i++;

			idx = j;

		} else if (strcmp(argv[j], ",") == 0) {

			/* Have we had some args before this comma? */
			if (idx != j-1) {
				is_dec[i] = 1;
				threads[i] = create_dec_thread(i, j-idx, &argv[idx]);
				i++;
			}

			idx = j;

		} else {
			/* Nothing, it's just a decoder option */
		}
	}

	/* Any remaining args? */
	if (idx != j-1) {
		is_dec[i] = 1;
		threads[i] = create_dec_thread(i, j-idx, &argv[idx]);
		i++;
	}
	nr_instances = i;


	/* Wait for the threads */
	for (i=0; i<nr_instances; i++) {
		if (threads[i] != 0) {
			pthread_join(*threads[i], &thread_ret);
			if ((int)thread_ret < 0) {
				ret = (int)thread_ret;
				fprintf(stderr, "%d %s: Error (%d)\n", i, type[is_dec[i]], ret);
			} else {
				fprintf(stderr, "%d %s: Success\n", i, type[is_dec[i]]);
			}
		}
	}

	pthread_mutex_destroy (&mutex);
	free(threads);
	free(is_dec);
	return ret;
}


