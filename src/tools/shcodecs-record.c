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

/*
 * This program captures v4l2 input (e.g. from a camera), optionally crops
 * and rotates this, encodes this and shows it on the display.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stropts.h>
#include <stdarg.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <linux/videodev2.h>	/* For pixel formats */
#include <linux/ioctl.h>
#include <pthread.h>
#include <errno.h>

#include <uiomux/uiomux.h>
#include <shveu/shveu.h>
#include <shcodecs/shcodecs_encoder.h>

#include "avcbencsmp.h"
#include "capture.h"
#include "ControlFileUtil.h"
#include "framerate.h"
#include "display.h"
#include "thrqueue.h"

#define DEBUG

/* Maximum number of cameras handled by the program */
#define MAX_CAMERAS 8

/* Maximum number of encoders per camera */
#define MAX_ENCODERS 8

struct camera_data {
	char * devicename;

	/* Captured frame information */
	capture *ceu;
	unsigned long cap_w;
	unsigned long cap_h;
	int captured_frames;

	pthread_t capture_thread;

	struct Queue * captured_queue;
	pthread_mutex_t capture_start_mutex;
};

struct encode_data {
	pthread_t thread;

	char ctrl_filename[MAXPATHLEN];
	APPLI_INFO ainfo;

	struct camera_data * camera;

	long stream_type;

	struct Queue * enc_input_q;
	struct Queue * enc_input_empty_q;
	FILE *output_fp;

	unsigned long enc_w;
	unsigned long enc_h;

	struct framerate * enc_framerate;
	int skipsize;
	int skipcount;

	double ifps;
	double mfps;
};

struct private_data {
	UIOMux * uiomux;

	int nr_cameras;
	struct camera_data cameras[MAX_CAMERAS];

	int nr_encoders;
	SHCodecs_Encoder *encoders[MAX_ENCODERS];
	struct encode_data encdata[MAX_ENCODERS];

	int do_preview;
	void *display;

	int rotate_cap;
};


static char * optstring = "r:Phv";

#ifdef HAVE_GETOPT_LONG
static struct option long_options[] = {
	{ "rotate", required_argument, NULL, 'r'},
	{ "no-preview", no_argument, NULL, 'P'},
	{ "help", no_argument, 0, 'h'},
	{ "version", no_argument, 0, 'v'},
};
#endif

static void
usage (const char * progname)
{
  printf ("Usage: %s [options] <control file> ...\n", progname);
  printf ("Encode video from a V4L2 device using the SH-Mobile VPU, with preview\n");
  printf ("\nCapture options\n");
  printf ("  -r 90, --rotate 90   Rotate the camera capture buffer 90 degrees and crop\n");
  printf ("\nDisplay options\n");
  printf ("  -P, --no-preview     Disable framebuffer preview\n");
  printf ("\nMiscellaneous options\n");
  printf ("  -h, --help           Display this help and exit\n");
  printf ("  -v, --version        Output version information and exit\n");
  printf ("\nPlease report bugs to <linux-sh@vger.kernel.org>\n");
}

void debug_printf(const char *fmt, ...)
{
#ifdef DEBUG
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
#endif
}

struct private_data pvt_data;

static int alive=1;

/*****************************************************************************/

/* Callback for frame capture */
static void
capture_image_cb(capture *ceu, const unsigned char *frame_data, size_t length,
		 void *user_data)
{
	struct camera_data *cam = (struct camera_data*)user_data;
	struct private_data *pvt = &pvt_data;
	struct encode_data *encdata;
	int pitch, offset;
	void *ptr;
	unsigned long enc_y, enc_c;
	unsigned long cap_y, cap_c;
	int i;

	cap_y = (unsigned long) frame_data;
	cap_c = cap_y + (cam->cap_w * cam->cap_h);

	for (i=0; i < pvt->nr_encoders; i++) {
		if (pvt->encdata[i].camera != cam) continue;

		encdata = &pvt->encdata[i];

		if (encdata->skipcount == 0) {
		/* Get an empty encoder input frame */
		enc_y = (unsigned long) queue_deq(encdata->enc_input_empty_q);
		enc_c = enc_y + (encdata->enc_w * encdata->enc_h);

		/* We are clipping, not scaling, as we need to perform a rotation,
	   	but the VEU cannot do a rotate & scale at the same time. */
		uiomux_lock (pvt->uiomux, UIOMUX_SH_VEU);
		shveu_operation(0,
			cap_y, cap_c,
			cam->cap_w, cam->cap_h, cam->cap_w, SHVEU_YCbCr420,
			enc_y, enc_c,
			encdata->enc_w, encdata->enc_h, encdata->enc_w, SHVEU_YCbCr420,
			pvt->rotate_cap);
		uiomux_unlock (pvt->uiomux, UIOMUX_SH_VEU);

		queue_enq(encdata->enc_input_q, (void*)enc_y);
		}

		encdata->skipcount++;
		encdata->skipcount %= encdata->skipsize;
	}

	if (cam == pvt->encdata[0].camera && pvt->do_preview) {
		/* Use the VEU to scale the capture buffer to the frame buffer */
		uiomux_lock (pvt->uiomux, UIOMUX_SH_VEU);
		display_update(pvt->display,
				cap_y, cap_c,
				cam->cap_w, cam->cap_h, cam->cap_w,
				V4L2_PIX_FMT_NV12);
		uiomux_unlock (pvt->uiomux, UIOMUX_SH_VEU);
	}

	capture_queue_buffer (cam->ceu, (void *)cap_y);

	cam->captured_frames++;
}

void *capture_main(void *data)
{
	struct camera_data *cam = (struct camera_data*)data;

	while(alive){
		capture_get_frame(cam->ceu, capture_image_cb, cam);
	}

	capture_stop_capturing(cam->ceu);

	return NULL;
}


/* SHCodecs_Encoder_Input_Release callback */
static int release_input_buf(SHCodecs_Encoder * encoder,
                             unsigned char * y_input,
                             unsigned char * c_input,
                             void * user_data)
{
	struct encode_data *encdata = (struct encode_data*)user_data;

	queue_enq (encdata->enc_input_empty_q, y_input);

	return 0;
}

/* SHCodecs_Encoder_Input callback for acquiring an image */
static int get_input(SHCodecs_Encoder *encoder, void *user_data)
{
	struct encode_data *encdata = (struct encode_data*)user_data;
	unsigned long enc_y, enc_c;

	/* Get a scaled frame from the queue */
	enc_y = (unsigned long) queue_deq(encdata->enc_input_q);
	enc_c = enc_y + (encdata->enc_w * encdata->enc_h);
	shcodecs_encoder_set_input_physical_addr (encoder, enc_y, enc_c);

	if (encdata->enc_framerate == NULL) {
		encdata->enc_framerate = framerate_new_measurer ();
	}

	return (alive?0:1);
}

/* SHCodecs_Encoder_Output callback for writing out the encoded data */
static int write_output(SHCodecs_Encoder *encoder,
			unsigned char *data, int length, void *user_data)
{
	struct encode_data *encdata = (struct encode_data*)user_data;

	if (shcodecs_encoder_get_frame_num_delta(encoder) > 0 &&
			encdata->enc_framerate != NULL) {
		if (encdata->enc_framerate->nr_handled >= encdata->ainfo.frames_to_encode &&
				encdata->ainfo.frames_to_encode > 0)
			return 1;
		framerate_mark (encdata->enc_framerate);
		encdata->ifps = framerate_instantaneous_fps (encdata->enc_framerate);
		encdata->mfps = framerate_mean_fps (encdata->enc_framerate);
	}

	if (fwrite(data, 1, length, encdata->output_fp) < (size_t)length)
		return -1;

	return (alive?0:1);
}

void cleanup (void)
{
	double time;
	struct private_data *pvt = &pvt_data;
	int i;

	for (i=0; i < pvt->nr_encoders; i++) {
		time = (double)framerate_elapsed_time (pvt->encdata[i].enc_framerate);
		time /= 1000000;

		debug_printf("[%d] Elapsed time (encode): %0.3g s\n", i, time);
		debug_printf("[%d] Encoded %d frames (%.2f fps)\n", i,
				pvt->encdata[i].enc_framerate->nr_handled,
			 	framerate_mean_fps (pvt->encdata[i].enc_framerate));

		shcodecs_encoder_close(pvt->encoders[i]);

		framerate_destroy (pvt->encdata[i].enc_framerate);
	}

	alive=0;

	for (i=0; i < pvt->nr_cameras; i++) {
		struct camera_data * cam = &pvt->cameras[i];

		pthread_join (cam->capture_thread, NULL);

		capture_close(cam->ceu);
	}

	if (pvt->do_preview)
		display_close(pvt->display);
	shveu_close();

	for (i=0; i < pvt->nr_encoders; i++) {
		close_output_file(pvt->encdata[i].output_fp);
	}

	uiomux_close (pvt->uiomux);
}

void sig_handler(int sig)
{
	cleanup ();

	/* Send ourselves the signal: see http://www.cons.org/cracauer/sigint.html */
	signal(sig, SIG_DFL);
	kill(getpid(), sig);
}

struct camera_data * get_camera (char * devicename, int width, int height)
{
	struct private_data *pvt = &pvt_data;
	int i;

	for (i=0; i < MAX_CAMERAS; i++) {
		if (pvt->cameras[i].devicename == NULL)
			break;

		if (!strcmp (pvt->cameras[i].devicename, devicename)) {
			return &pvt->cameras[i];
		}
	}

	if (i == MAX_CAMERAS) return NULL;

	pvt->cameras[i].devicename = devicename;
	pvt->cameras[i].cap_w = width;
	pvt->cameras[i].cap_h = height;

	pvt->nr_cameras = i+1;

	return &pvt->cameras[i];
}

void * encode_main (void * data)
{
	SHCodecs_Encoder * encoder = (SHCodecs_Encoder *)data;
	int ret = -1;

	ret = shcodecs_encoder_run (encoder);

	return (void *)ret;
}

int main(int argc, char *argv[])
{
	struct private_data *pvt;
	int return_code, rc;
	void *thread_ret;
	unsigned int pixel_format;
	int c, i=0;
	long target_fps10;
	unsigned long rotate_input;

	char * progname;
	int show_version = 0;
	int show_help = 0;

	progname = argv[0];

	if (argc == 1) {
		usage(progname);
		return 0;
	}

	pvt = &pvt_data;

	memset (pvt, 0, sizeof(struct private_data));

	pvt->do_preview = 1;

	pvt->rotate_cap = SHVEU_NO_ROT;

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
		case 'h': /* help */
			show_help = 1;
			break;
		case 'v': /* version */
			show_version = 1;
			break;
		case 'r':
			if (optarg) {
				rotate_input = strtoul(optarg, NULL, 10);
				if (rotate_input == 1 || rotate_input == 90) {
					pvt->rotate_cap = SHVEU_ROT_90;
				}
			}
			break;
		case 'P':
			pvt->do_preview = 0;
			break;
		default:
			usage(progname);
			goto exit_err;
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

	if (optind >= argc) {
	      usage (progname);
	      goto exit_err;
	}

	while (optind < argc) {
		strncpy(pvt->encdata[i++].ctrl_filename, argv[optind++], MAXPATHLEN-1);
	}

	pvt->nr_encoders = i;

	pvt->uiomux = uiomux_open ();

	for (i=0; i < pvt->nr_encoders; i++) {
		if ( (strcmp(pvt->encdata[i].ctrl_filename, "-") == 0) ||
				(pvt->encdata[i].ctrl_filename[0] == '\0') ){
			fprintf(stderr, "Invalid v4l2 configuration file.\n");
			return -1;
		}

		return_code = ctrlfile_get_params(pvt->encdata[i].ctrl_filename,
				&pvt->encdata[i].ainfo, &pvt->encdata[i].stream_type);
		if (return_code < 0) {
			fprintf(stderr, "Error opening control file %s.\n", pvt->encdata[i].ctrl_filename);
			return -2;
		}

		pvt->encdata[i].camera = get_camera (pvt->encdata[i].ainfo.input_file_name_buf, pvt->encdata[i].ainfo.xpic, pvt->encdata[i].ainfo.ypic);

		debug_printf("[%d] Input file: %s\n", i, pvt->encdata[i].ainfo.input_file_name_buf);
		debug_printf("[%d] Output file: %s\n", i, pvt->encdata[i].ainfo.output_file_name_buf);

		/* Initialize the queues */
		pvt->encdata[i].enc_input_q = queue_init();
		pvt->encdata[i].enc_input_empty_q = queue_init();
	}

	for (i=0; i < pvt->nr_cameras; i++) {
		/* Camera capture initialisation */
		pvt->cameras[i].ceu = capture_open_userio(pvt->cameras[i].devicename, pvt->cameras[i].cap_w, pvt->cameras[i].cap_h, pvt->uiomux);
		if (pvt->cameras[i].ceu == NULL) {
			fprintf(stderr, "capture_open failed, exiting\n");
			return -3;
		}
		capture_set_use_physical(pvt->cameras[i].ceu, 1);
		pvt->cameras[i].cap_w = capture_get_width(pvt->cameras[i].ceu);
		pvt->cameras[i].cap_h = capture_get_height(pvt->cameras[i].ceu);

		pixel_format = capture_get_pixel_format (pvt->cameras[i].ceu);
		if (pixel_format != V4L2_PIX_FMT_NV12) {
			fprintf(stderr, "Camera capture pixel format is not supported\n");
			return -4;
		}
		debug_printf("Camera %d resolution:  %dx%d\n", i, pvt->cameras[i].cap_w, pvt->cameras[i].cap_h);
	}

	signal (SIGINT, sig_handler);
	signal (SIGPIPE, sig_handler);

	/* VEU initialisation */
	if (shveu_open() < 0) {
		fprintf (stderr, "Could not open VEU, exiting\n");
	}

	if (pvt->do_preview) {
		pvt->display = display_open(0);
		if (!pvt->display) {
			return -5;
		}
	}

	fprintf (stderr, "\nEncoders\n");
	fprintf (stderr, "--------\n");
	fprintf (stderr, "Size:     ");
	for (i=0; i < pvt->nr_encoders; i++) {
#if 0
		if (pvt->rotate_cap == SHVEU_NO_ROT) {
			pvt->encdata[i].enc_w = pvt->cap_w;
			pvt->encdata[i].enc_h = pvt->cap_h;
		} else {
			pvt->encdata[i].enc_w = pvt->cap_h;
			pvt->encdata[i].enc_h = pvt->cap_h * pvt->cap_h / pvt->cap_w;
			/* Round down to nearest multiple of 16 for VPU */
			pvt->encdata[i].enc_w = pvt->encdata[i].enc_w - (pvt->encdata[i].enc_w % 16);
			pvt->encdata[i].enc_h = pvt->encdata[i].enc_h - (pvt->encdata[i].enc_h % 16);
			debug_printf("[%d] Rotating & cropping camera image ...\n", i);
		}
#else
		/* Override the encoding frame size in case of rotation */
		if (pvt->rotate_cap == SHVEU_NO_ROT) {
			pvt->encdata[i].enc_w = pvt->encdata[i].ainfo.xpic;
			pvt->encdata[i].enc_h = pvt->encdata[i].ainfo.ypic;
		} else {
			pvt->encdata[i].enc_w = pvt->encdata[i].ainfo.ypic;
			pvt->encdata[i].enc_h = pvt->encdata[i].ainfo.xpic;
		}
		debug_printf("\t%dx%d", pvt->encdata[i].enc_w, pvt->encdata[i].enc_h);
#endif

		/* VPU Encoder initialisation */
		pvt->encdata[i].output_fp = open_output_file(pvt->encdata[i].ainfo.output_file_name_buf);
		if (pvt->encdata[i].output_fp == NULL) {
			fprintf(stderr, "Error opening output file\n");
			return -8;
		}

		pvt->encoders[i] = shcodecs_encoder_init(pvt->encdata[i].enc_w, pvt->encdata[i].enc_h, pvt->encdata[i].stream_type);
		if (pvt->encoders[i] == NULL) {
			fprintf(stderr, "shcodecs_encoder_init failed, exiting\n");
			return -5;
		}
		shcodecs_encoder_set_input_callback(pvt->encoders[i], get_input, &pvt->encdata[i]);
		shcodecs_encoder_set_output_callback(pvt->encoders[i], write_output, &pvt->encdata[i]);
		shcodecs_encoder_set_input_release_callback(pvt->encoders[i], release_input_buf, &pvt->encdata[i]);

		return_code = ctrlfile_set_enc_param(pvt->encoders[i], pvt->encdata[i].ctrl_filename);
		if (return_code < 0) {
			fprintf (stderr, "Problem with encoder params in control file!\n");
			return -9;
		}

		//shcodecs_encoder_set_xpic_size(pvt->encoders[i], pvt->encdata[i].enc_w);
		//shcodecs_encoder_set_ypic_size(pvt->encoders[i], pvt->encdata[i].enc_h);

	}

	/* Set up framedropping */
	fprintf (stderr, "\nTarget   @");
	for (i=0; i < pvt->nr_encoders; i++) {
		target_fps10 = shcodecs_encoder_get_frame_rate(pvt->encoders[i]);
		fprintf (stderr, "\t%4.2f  ", target_fps10/10.0);

		pvt->encdata[i].skipsize = 300 / target_fps10;
		pvt->encdata[i].skipcount = 0;

		pvt->encdata[i].ifps = 0;
		pvt->encdata[i].mfps = 0;
	}
	fprintf (stderr, "\tFPS\n");

	for (i=0; i < pvt->nr_cameras; i++) {
		capture_start_capturing(pvt->cameras[i].ceu);

		rc = pthread_create(&pvt->cameras[i].capture_thread, NULL, capture_main, &pvt->cameras[i]);
		if (rc){
			fprintf(stderr, "pthread_create failed, exiting\n");
			return -6;
		}
	}

	for (i=0; i < pvt->nr_encoders; i++) {
		rc = pthread_create (&pvt->encdata[i].thread, NULL, encode_main, pvt->encoders[i]);
		if (rc)
			fprintf (stderr, "Thread %d failed: %s\n", i, strerror(rc));
	}

	while (alive) {
		fprintf (stderr, "Encoding @");
		for (i=0; i < pvt->nr_encoders; i++) {
			fprintf (stderr, "\t%4.2f  ", pvt->encdata[i].ifps);
		}
		fprintf (stderr, "\tFPS\r");
		usleep (300000);
	}

	fprintf (stderr, "\nEncoded  @");
	for (i=0; i < pvt->nr_encoders; i++) {
		fprintf (stderr, "\t%4.2f  ", pvt->encdata[i].mfps);
	}
	fprintf (stderr, "\tFPS\n");

	fprintf (stderr, "Status   :");
	rc = 0;
	for (i=0; i < pvt->nr_encoders; i++) {
		if (pvt->encdata[i].thread != 0) {
			pthread_join (pvt->encdata[i].thread, &thread_ret);
			if ((int)thread_ret < 0) {
				rc = (int)thread_ret;
				fprintf (stderr, "\tErr %d", i);
			} else {
				fprintf (stderr, "\tOK");
			}
		}
	}
	fprintf (stderr, "\n");

	cleanup ();

	return rc;

exit_err:
	/* General exit, prior to thread creation */
	exit (1);
}

