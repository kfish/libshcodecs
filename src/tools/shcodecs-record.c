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
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <linux/videodev2.h>	/* For pixel formats */
#include <linux/fb.h>
#include <linux/ioctl.h>
#include <pthread.h>
#include <errno.h>

#include <shcodecs/shcodecs_encoder.h>
#include "avcbencsmp.h"
#include "capture.h"
#include "veu_colorspace.h"
#include "ControlFileUtil.h"
#include "framerate.h"

#define _DEBUG

#define LCD_BPP 2
#define U_SEC_PER_SEC 1000000

struct private_data {
	/* Display data */
	int fb_handle;
	int fb_index;
	struct fb_fix_screeninfo fb_fix;
	struct fb_var_screeninfo fb_var;
	unsigned char *fb_base;
	unsigned char *fb_screenMem;
	int lcd_w;
	int lcd_h;

	pthread_t thread_blit;
	pthread_t thread_capture;

	APPLI_INFO ainfo;	/* Capture params */
	SHCodecs_Encoder *encoder;

	pthread_mutex_t capture_start_mutex;
	pthread_mutex_t capture_done_mutex;
	pthread_mutex_t encode_start_mutex;

	/* Captured frame information */
	unsigned char *cap_y;
	unsigned char *cap_c;
	int rotate_cap;
	int cap_w;
	int cap_h;
	unsigned long cap_fmt;

	int enc_w;
	int enc_h;

	struct framerate * framerate;

	int captured_frames;
	int output_frames;
};


int open_output_file(APPLI_INFO *);
void close_output_file(APPLI_INFO * appli_info);
int select_inputfile_set_param(SHCodecs_Encoder *encoder,
			       APPLI_INFO *appli_info);

static char * optstring = "i:r:";

#ifdef HAVE_GETOPT_LONG
static struct option long_options[] = {
	{ "input" , required_argument, NULL, 'i'},
	{ "rotate", required_argument, NULL, 'r'},
};
#endif

static void
usage (const char * progname)
{
  printf ("Usage: %s [options] <control file>\n", progname);
  printf ("Encode V4L2 video input using the SH-Mobile VPU with preview\n");
  printf ("\nOptions\n");
  printf ("  -i, --input      Set the v4l2 configuration file\n");
  printf ("  -r, --rotate     Rotate the camera capture buffer 90 degrees and crop\n");
}

void debug_printf(__const char *__restrict __format, ...)
{
#ifdef _DEBUG
	printf(__format);
#endif
}


/*****************************************************************************/

static int display_flip(struct private_data *pvt)
{
	struct fb_var_screeninfo fb_screen = pvt->fb_var;

	fsync(pvt->fb_handle);

	fb_screen.xoffset = 0;
	fb_screen.yoffset = 0;
	if (pvt->fb_index==0)
		fb_screen.yoffset = pvt->fb_var.yres;
	if (-1 == ioctl(pvt->fb_handle, FBIOPAN_DISPLAY, &fb_screen))
		return 0;

	/* Point to the back buffer */
	pvt->fb_screenMem = pvt->fb_base;
	if (pvt->fb_index!=0)
		pvt->fb_screenMem += pvt->fb_var.yres * pvt->fb_var.xres * LCD_BPP;

	pvt->fb_index = (pvt->fb_index+1) & 1;
	return 1;
}


static int display_open(struct private_data *pvt)
{
	const char *device;

	/* Initialize display */
	device = getenv("FRAMEBUFFER");
	if (!device) {
		if (access("/dev/.devfsd", F_OK) == 0) {
			device = "/dev/fb/0";
		} else {
			device = "/dev/fb0";
		}
	}

	if ((pvt->fb_handle = open(device, O_RDWR)) < 0) {
		fprintf(stderr, "Open %s: %s.\n", device, strerror(errno));
		return 0;
	}
	if (ioctl(pvt->fb_handle, FBIOGET_FSCREENINFO, &pvt->fb_fix) < 0) {
		fprintf(stderr, "Ioctl FBIOGET_FSCREENINFO error.\n");
		return 0;
	}
	if (ioctl(pvt->fb_handle, FBIOGET_VSCREENINFO, &pvt->fb_var) < 0) {
		fprintf(stderr, "Ioctl FBIOGET_VSCREENINFO error.\n");
		return 0;
	}
	if (pvt->fb_fix.type != FB_TYPE_PACKED_PIXELS) {
		fprintf(stderr, "Frame buffer isn't packed pixel.\n");
		return 0;
	}

	pvt->lcd_w = pvt->fb_var.xres;
	pvt->lcd_h = pvt->fb_var.yres;

	pvt->fb_screenMem = pvt->fb_base = (unsigned char*)pvt->fb_fix.smem_start;
	pvt->fb_index = 0;
	display_flip(pvt);

	debug_printf("Display resolution: %dx%d\n", pvt->lcd_w, pvt->lcd_h);

	return 1;
}

static void display_close(struct private_data *pvt)
{
	close(pvt->fb_handle);
}


/*****************************************************************************/

struct timeval start, finish, diff;

/* Callback for frame capture */
static void
capture_image_cb(sh_ceu *ceu, const unsigned char *frame_data, size_t length,
		 void *user_data)
{
	struct private_data *pvt = (struct private_data*)user_data;

	pvt->cap_y = (unsigned char *)frame_data;
	pvt->cap_c = pvt->cap_y + (pvt->cap_w * pvt->cap_h);

	pvt->captured_frames++;

	pthread_mutex_unlock(&pvt->capture_done_mutex);
}

void *capture_thread(void *data)
{
	struct private_data *pvt = (struct private_data*)data;

	while(1){
		/* This mutex is unlocked once the capture buffer is free */
		pthread_mutex_lock(&pvt->capture_start_mutex);

		framerate_wait(pvt->framerate);
		sh_ceu_capture_frame(pvt->ainfo.ceu, capture_image_cb, pvt);
		gettimeofday(&finish, 0);
	}
}


void *process_capture_thread(void *data)
{
	struct private_data *pvt = (struct private_data*)data;
	int pitch, offset;
	void *ptr;
	unsigned char *enc_y, *enc_c;
	unsigned int src_w, src_h;

	while(1){
		pthread_mutex_lock(&pvt->capture_done_mutex); 

		shcodecs_encoder_get_input_physical_addr (pvt->encoder, (unsigned int *)&enc_y, (unsigned int *)&enc_c);

		if (!pvt->rotate_cap) {
			src_w = pvt->cap_w;
			src_h = pvt->cap_h;
		} else {
			src_w = pvt->enc_h;
			src_h = pvt->enc_w;
		}

		/* We are clipping, not scaling, as we need to perform a rotation,
		   but the VEU cannot do a rotate & scale at the same time. */
		sh_veu_operation(0,
			pvt->cap_y, pvt->cap_c,
			src_w, src_h, pvt->cap_w, YCbCr420,
			enc_y, enc_c,
			pvt->enc_w, pvt->enc_h, pvt->enc_w, YCbCr420,
			pvt->rotate_cap);

		/* Setup the VEU to scale the encoder input buffer (physical addr) to
		   the LCD frame buffer (physical addr) */
		sh_veu_operation(0, 
			enc_y, enc_c, pvt->enc_w, pvt->enc_h, pvt->enc_w, YCbCr420,
			pvt->fb_screenMem, NULL,
			pvt->lcd_w, pvt->lcd_h, pvt->lcd_w, RGB565, NO_ROT);

		display_flip(pvt);

		pvt->output_frames++;

		/* Let the encoder get_input function return */
		pthread_mutex_unlock(&pvt->encode_start_mutex);
	}
}


/* SHCodecs_Encoder_Input callback for acquiring an image */
static int get_input(SHCodecs_Encoder *encoder, void *user_data)
{
	struct private_data *pvt = (struct private_data*)user_data;

	/* This mutex is unlocked once the capture buffer has been copied to the
	   encoder input buffer */
	pthread_mutex_lock(&pvt->encode_start_mutex);

	pthread_mutex_unlock(&pvt->capture_start_mutex);
	return 0;
}

/* SHCodecs_Encoder_Output callback for writing out the encoded data */
static int write_output(SHCodecs_Encoder *encoder,
			unsigned char *data, int length, void *user_data)
{
	struct private_data *pvt = (struct private_data*)user_data;

	if (fwrite(data, 1, length, pvt->ainfo.output_file_fp) < (size_t)length)
		return -1;

	return 0;
}

struct private_data pvt_data;

void cleanup (void)
{
	float time;
	struct private_data *pvt = &pvt_data;

	timersub(&finish, &start, &diff);
	time = diff.tv_sec + (float)diff.tv_usec/U_SEC_PER_SEC;

	debug_printf("Captured %d frames (%.2f fps)\n", pvt->captured_frames,
		 	(float)pvt->captured_frames/time);
	debug_printf("Displayed %d frames (%.2f fps)\n", pvt->output_frames,
			(float)pvt->output_frames/time);

	framerate_destroy (pvt->framerate);

	sh_ceu_stop_capturing(pvt->ainfo.ceu);
	shcodecs_encoder_close(pvt->encoder);
	sh_ceu_close(pvt->ainfo.ceu);
	close_output_file(&pvt->ainfo);

	pthread_cancel (pvt->thread_blit);
	display_close(pvt);

	pthread_cancel (pvt->thread_capture);
	sh_veu_close();

	pthread_mutex_destroy (&pvt->capture_done_mutex);
	pthread_mutex_destroy (&pvt->encode_start_mutex);
	pthread_mutex_destroy (&pvt->capture_start_mutex);
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
	struct private_data *pvt;
	int return_code, rc;
	long stream_type;
	unsigned int pixel_format;
	char v4l2_filename[MAXPATHLEN];
	int c, i;
	long target_fps10;

	pvt = &pvt_data;

	if (argc == 1) {
		usage(argv[0]);
		return 0;
	}
	v4l2_filename[0] = '\0';
	v4l2_filename[MAXPATHLEN-1] = '\0';

	pvt->captured_frames = 0;
	pvt->output_frames = 0;
	pvt->rotate_cap = 0;

	while (1) {
#ifdef HAVE_GETOPT_LONG
		c = getopt_long(argc, argv, optstring, long_options, &i);
#else
		c = getopt (argc, argv, optstring);
#endif
		if (c == -1)
			break;
                if (c == ':') {
                        usage (argv[0]);
                        goto exit_err;
                }

		switch (c) {
		case 'i':
			if (optarg)
				strncpy(v4l2_filename, optarg, MAXPATHLEN-1);
			break;
		case 'r':
			if (optarg)
				pvt->rotate_cap = strtoul(optarg, NULL, 10);
			break;
		default:
			usage(argv[0]);
			return -2;
		}
	}

        if (optind >= argc) {
	      usage (argv[0]);
              goto exit_err;
        }

	if (optind == (argc-1) && v4l2_filename[0] == '\0') {
		strncpy(v4l2_filename, argv[optind++], MAXPATHLEN-1);
	}
	if ( (strcmp(v4l2_filename, "-") == 0) || (v4l2_filename[0] == '\0') ){
		fprintf(stderr, "Invalid v4l2 configuration file.\n");
		return -1;
	}

	strcpy(pvt->ainfo.ctrl_file_name_buf, v4l2_filename);
	return_code = GetFromCtrlFTop(pvt->ainfo.ctrl_file_name_buf,
				      &pvt->ainfo, &stream_type);
	if (return_code < 0) {
		fprintf(stderr, "Error opening control file.\n");
		return -2;
	}


	snprintf(pvt->ainfo.input_file_name_buf, 256, "%s/%s",
		 pvt->ainfo.buf_input_yuv_file_with_path,
		 pvt->ainfo.buf_input_yuv_file);

	if (!strcmp (pvt->ainfo.buf_output_stream_file, "-")) {
			snprintf (pvt->ainfo.output_file_name_buf, 256, "-");
	} else {
		snprintf(pvt->ainfo.output_file_name_buf, 256, "%s/%s",
				pvt->ainfo.buf_output_directry,
				pvt->ainfo.buf_output_stream_file);
	}

	debug_printf("Input file: %s\n", pvt->ainfo.input_file_name_buf);
	debug_printf("Output file: %s\n", pvt->ainfo.output_file_name_buf);

	/* Initalise the mutexes */
	pthread_mutex_init(&pvt->capture_start_mutex, NULL);
	pthread_mutex_lock(&pvt->capture_start_mutex);

	pthread_mutex_init(&pvt->encode_start_mutex, NULL);
	pthread_mutex_unlock(&pvt->encode_start_mutex);

	pthread_mutex_init(&pvt->capture_done_mutex, NULL);
	pthread_mutex_lock(&pvt->capture_done_mutex);

	/* Create the threads */
	rc = pthread_create(&pvt->thread_capture, NULL, capture_thread, pvt);
	if (rc){
		fprintf(stderr, "pthread_create failed, exiting\n");
		return -6;
	}
	rc = pthread_create(&pvt->thread_blit, NULL, process_capture_thread, pvt);
	if (rc){
		fprintf(stderr, "pthread_create failed, exiting\n");
		return -7;
	}

        signal (SIGINT, sig_handler);
        signal (SIGPIPE, sig_handler);

	/* VEU Scaler initialisation */
	sh_veu_open();

	/* Camera capture initialisation */
	pvt->ainfo.ceu = sh_ceu_open_userio(pvt->ainfo.input_file_name_buf, pvt->ainfo.xpic, pvt->ainfo.ypic);
	if (pvt->ainfo.ceu == NULL) {
		fprintf(stderr, "sh_ceu_open failed, exiting\n");
		return -3;
	}
	sh_ceu_set_use_physical(pvt->ainfo.ceu, 1);
	pvt->cap_w = sh_ceu_get_width(pvt->ainfo.ceu);
	pvt->cap_h = sh_ceu_get_height(pvt->ainfo.ceu);

	pixel_format = sh_ceu_get_pixel_format (pvt->ainfo.ceu);
	if (pixel_format == V4L2_PIX_FMT_NV12) {
		pvt->cap_fmt = YCbCr420;
	} else {
		fprintf(stderr, "Camera capture pixel format is not supported\n");
		return -4;
	}

	if (!pvt->rotate_cap) {
		pvt->enc_w = pvt->cap_w;
		pvt->enc_h = pvt->cap_h;
	} else {
		pvt->enc_w = pvt->cap_h;
		pvt->enc_h = pvt->cap_h * pvt->cap_h / pvt->cap_w;
		/* Round down to nearest multiple of 16 for VPU */
		pvt->enc_w = pvt->enc_w - (pvt->enc_w % 16);
		pvt->enc_h = pvt->enc_h - (pvt->enc_h % 16);
		debug_printf("Rotating & croping camera image...\n");
	}

	debug_printf("Camera resolution:  %dx%d\n", pvt->cap_w, pvt->cap_h);
	debug_printf("Encode resolution:  %dx%d\n", pvt->enc_w, pvt->enc_h);

	if (!display_open(pvt)) {
		return -5;
	}

	/* VPU Encoder initialisation */
	pvt->encoder = shcodecs_encoder_init(pvt->enc_w, pvt->enc_h, stream_type);
	if (pvt->encoder == NULL) {
		fprintf(stderr, "shcodecs_encoder_init failed, exiting\n");
		return -5;
	}
	shcodecs_encoder_set_input_callback(pvt->encoder, get_input, pvt);
	shcodecs_encoder_set_output_callback(pvt->encoder, write_output, pvt);

	/* open output file */
	return_code = open_output_file(&pvt->ainfo);
	if (return_code != 0) {
		fprintf(stderr, "Error opening output file\n");
		return -8;
	}

	/* set parameters for use in encoding */
	return_code = select_inputfile_set_param(pvt->encoder, &pvt->ainfo);
	if (return_code == -1) {
		fprintf (stderr, "select_inputfile_set_param ERROR! \n");
		return -9;
	}
	/* Override the encoding frame size as it may not be the same size as the
	   camera capture size */
	shcodecs_encoder_set_xpic_size(pvt->encoder, pvt->enc_w);
	shcodecs_encoder_set_ypic_size(pvt->encoder, pvt->enc_h);

	/* Set up the frame rate timer to match the encode framerate */
	target_fps10 = shcodecs_encoder_get_frame_rate(pvt->encoder);
	fprintf (stderr, "Target framerate:   %.1f fps\n", target_fps10 / 10.0);

	/* Initialize framerate timer */
	pvt->framerate = framerate_new (target_fps10 / 10.0);

	sh_ceu_start_capturing(pvt->ainfo.ceu);

	gettimeofday(&start, 0);
	
	rc = shcodecs_encoder_run(pvt->encoder);
	if (rc != 0) {
		fprintf(stderr, "Error encoding, error code=%d\n", rc);
		rc = -10;
	}

	cleanup ();

	return rc;

exit_err:
	/* General exit, prior to thread creation */
	exit (1);
}

