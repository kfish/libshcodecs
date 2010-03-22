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

#include <shcodecs/shcodecs_decoder.h>

/* #define DEBUG */
#ifdef DEBUG
#define debug_printf	printf
#else
#define debug_printf(...)	/* none */
#endif

#define DEFAULT_WIDTH 320
#define DEFAULT_HEIGHT 240

struct shdec {
	int		input_fd;	/* Input file descriptor */
	int		output_fd;	/* Output file descriptor */
	unsigned char	*input_buffer;	/* Pointer to input buffer */
	size_t		si_isize;	/* Total size of input data */

	long max_nal_size;
	long total_input_consumed;
	long total_output_bytes;
};


/***********************************************************/

/* Forward declarations */
static int local_init (struct shdec * dec, char *pInputfile, char *pOutputfile);
static int local_close (struct shdec * dec);

static void
usage (const char * progname)
{
	printf ("Usage: %s [options] ...\n", progname);
	printf ("Decode an H.264 or MPEG-4 Elementary Stream to a YCbCr 4:2:0 file using the SH-Mobile VPU\n");
	printf ("\nFile options\n");
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


/***********************************************************/

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

int main(int argc, char **argv)
{
	struct shdec dec1;
	struct shdec * dec = &dec1;
	SHCodecs_Decoder * decoder;
	int ret=0, stream_type = -1, i, w, h, c;
	char input_filename[MAXPATHLEN], output_filename[MAXPATHLEN];
	struct sched_param stSchePara;
	int bytes_decoded, frames_decoded;
	ssize_t n;
	char * ext;
	char * progname = argv[0];
	int show_version = 0;
	int show_help = 0;

	if (argc == 1) {
		usage(progname);
		exit(0);
	}

	setvbuf(stdout, NULL, _IONBF, 0);
	output_filename[0] = input_filename[0] = '\0';
	memset(input_filename, 0, sizeof(input_filename));
	memset(output_filename, 0, sizeof(output_filename));

	/* Set defaults */
	w = DEFAULT_WIDTH;
	h = DEFAULT_HEIGHT;

	while (1) {
#ifdef HAVE_GETOPT_LONG
		c = getopt_long(argc, argv, optstring, long_options, &i);
#else
		c = getopt (argc, argv, optstring);
#endif
		if (c == -1)
			break;
		if (c == ':') {
			usage (progname);
			goto exit_err;
		}

		switch (c) {
		case 'H': /* --help */
			show_help = 1;
			break;
		case 'v': /* --version */
			show_version = 1;
			break;
		case 'f':
			if (strncmp(optarg, "mpeg4", 5) == 0)
				stream_type = SHCodecs_Format_MPEG4;
			else if (strncmp(optarg, "h264", 4) == 0)
				stream_type = SHCodecs_Format_H264;
			else {
				debug_printf("Unknown video format: %s.\n", optarg);
				exit(-1);
			}
			break;
		case 'o':
			if (optarg)
				strncpy(output_filename, optarg, sizeof(output_filename) - 1);
			break;
		case 'i':
			if (optarg)
				strncpy(input_filename, optarg, sizeof(input_filename) - 1);
			break;
		case 'w':
			if (optarg)
				w = strtoul(optarg, NULL, 10);
			break;
		case 'h':
			if (optarg)
				h = strtoul(optarg, NULL, 10);
			break;
		case 's':
			if (optarg) {
				if (!strncasecmp (optarg, "qcif", 4)) {
					w = 176;
					h = 144;
				} else if (!strncmp (optarg, "cif", 3)) {
					w = 352;
					h = 288;
				} else if (!strncmp (optarg, "qvga", 4)) {
					w = 320;
					h = 240;
				} else if (!strncmp (optarg, "vga", 3)) {
					w = 640;
					h = 480;
				} else if (!strncmp (optarg, "720p", 4)) {
					w = 1280;
					h = 720;
				}
			}
			break;
		default:
			usage(progname);
			exit(-2);
		}
	}

	if (show_version) {
		printf ("%s version " VERSION "\n", progname);
	}

	if (show_help) {
		usage (progname);
	}

	if (show_version || show_help) {
		return 0;
	}

	if (w == -1 || h == -1){
		debug_printf("Invalid width and/or height specified.\n");
		exit(-3);
	}
	if ( (strcmp(input_filename, "-") == 0) || (input_filename[0] == '\0') ){
		debug_printf("Invalid input file.\n");
		exit(-4);
	}
#if 0
	if ( (strcmp(output_filename, "-") == 0) || (output_filename[0] == '\0') ){
		debug_printf("Invalid output file.\n");
		exit(-5);
	}
#endif
	if (w < SHCODECS_MIN_FX || w > SHCODECS_MAX_FX || h < SHCODECS_MIN_FY || h > SHCODECS_MAX_FY) {
		debug_printf("Invalid width and/or height specified.\n");
		exit(-6);
	}
	if (optind > argc){
		debug_printf("Too many arguments.\n");
		exit(-7);
	}

	if (stream_type == -1) {
		ext = strrchr (input_filename, '.');
		if (ext == NULL || !strncmp (ext, ".264", 4))
			stream_type = SHCodecs_Format_H264;
		else
			stream_type = SHCodecs_Format_MPEG4;
	}

	debug_printf("Format: %s\n", stream_type == SHCodecs_Format_H264 ? "H.264" : "MPEG4");
	debug_printf("Resolution: %dx%d\n", w, h);
	debug_printf("Input  file: %s\n", input_filename);
	debug_printf("Output file: %s\n", output_filename);

 	stSchePara.sched_priority = sched_get_priority_max(SCHED_FIFO);
	if (sched_setscheduler(0, SCHED_RR, &stSchePara) != 0) {
		perror("sched_setscheduler");
		exit(-8);
	}

	/* H.264 spec: Max NAL size is the size of an uncomrpessed immage divided
	   by the "Minimum Compression Ratio", MinCR. This is 2 for most levels
	   but is 4 for levels 3.1 to 4. Since we don't know the level, we just
	   use MinCR=2. */
	dec->max_nal_size = (w * h * 3) / 2; /* YCbCr420 */
	dec->max_nal_size /= 2;              /* Apply MinCR */

	dec->total_input_consumed = 0;
	dec->total_output_bytes = 0;

	/* Open file descriptors to talk to the VPU and SDR drivers */

	if ((decoder = shcodecs_decoder_init(w, h, stream_type)) == NULL) {
		exit (-9);
	}

	local_init(dec, input_filename, output_filename);

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

	debug_printf ("\nshcodecs-dec: Finalizing ...\n");

	/* Finalize the decode output, in case a final frame is available */
	shcodecs_decoder_finalize (decoder);

	frames_decoded = shcodecs_decoder_get_frame_count (decoder);
	fprintf (stderr, "Total frames decoded: %d\n", frames_decoded);

	local_close (dec);

	shcodecs_decoder_close(decoder);

	fprintf (stderr, "Total bytes consumed: %ld\n", dec->total_input_consumed);
	fprintf (stderr, "Total bytes output: %ld\n", dec->total_output_bytes);

exit_ok:
	exit (0);

exit_err:
	exit (1);
}

/*
 * open_input
 *
 */
static int open_input(struct shdec * dec, char *input_filename)
{
	dec->input_fd = open(input_filename, O_RDONLY);
	if (dec->input_fd == -1) {
		perror(input_filename);
		return -1;
	}
	return 0;
}
 
/*
 * open_output
 *
 */
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
	if (input_filename[0] != '\0' && open_input(dec, input_filename))
		return -50;
	if (output_filename[0] != '\0' && open_output(dec, output_filename))
		return -51;

	/* Allocate memory for input buffer */
	dec->input_buffer = malloc(dec->max_nal_size);
	debug_printf("input buffer = %X\n",(int)dec->input_buffer);
	if (dec->input_buffer == NULL) goto err2;

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
err2:
	return -1;
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
