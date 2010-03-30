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

/*
 *  V4L2 video capture example
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>		/* getopt_long() */
#include <unistd.h>
#include <errno.h>

#include <shveu/shveu.h>

#include "capture.h"
#include "framerate.h"

struct camera_data {
	capture *ceu;
	char *dev_name;
 	unsigned int width;
 	unsigned int height;
	int mode;
	struct framerate * cap_framerate;
};

static void usage(FILE * fp, int argc, char **argv)
{
	fprintf(fp,
		"Usage: %s [options]\n\n"
		"Options:\n"
		"-d | --device        Video device name [/dev/video0]\n"
		"-w | --width         Capture width [320]\n"
		"-h | --height        Capture height [240]\n"
		"-s | --size          Capture size [qcif, cif, qvga, vga, d1, 720p]\n"
		"-m | --mode          Capture mode [mmap, user]\n"
		"", argv[0]);
	fprintf (fp, "\nPlease report bugs to <linux-sh@vger.kernel.org>\n");
}

static const char short_options[] = "d:w:h:s:m:";

static const struct option long_options[] = {
	{"device", required_argument, NULL, 'd'},
	{"width",  required_argument, NULL, 'w'},
	{"height", required_argument, NULL, 'h'},
	{"size",   required_argument, NULL, 's'},
	{"mode",   required_argument, NULL, 'm'},
	{0, 0, 0, 0}
};

static void
process_image(capture * ceu, const unsigned char *frame_data, size_t length, void *user_data)
{
	struct camera_data *cam = (struct camera_data *)user_data;
	size_t size = (cam->width * cam->height * 3) / 2;

	fputc('.', stderr);
	fflush(stderr);

	if (fwrite(frame_data, 1, size, stdout) != size)
		exit(EXIT_FAILURE);

	capture_queue_buffer (ceu, frame_data);

	framerate_wait(cam->cap_framerate);
}


int main(int argc, char **argv)
{
	struct camera_data cam;
	unsigned int count, x;
	double time;

	cam.width = 320;
	cam.height = 240;
	cam.dev_name = "/dev/video0";
	cam.mode = 0;

	for (;;) {
		int index;
		int c;

		c = getopt_long(argc, argv,
				short_options, long_options, &index);

		if (-1 == c)
			break;

		switch (c) {
		case 0:	/* getopt_long() flag */
			break;

		case 'd':
			cam.dev_name = optarg;
			break;

		case 'w':
			if (optarg)
				cam.width = strtoul(optarg, NULL, 10);
			break;
		case 'h':
			if (optarg)
				cam.height = strtoul(optarg, NULL, 10);
			break;
		case 's':
			if (optarg) {
				if (!strncasecmp (optarg, "qcif", 4)) {
					cam.width = 176;
					cam.height = 144;
				} else if (!strncmp (optarg, "cif", 3)) {
					cam.width = 352;
					cam.height = 288;
				} else if (!strncmp (optarg, "qvga", 4)) {
					cam.width = 320;
					cam.height = 240;
				} else if (!strncmp (optarg, "vga", 3)) {
					cam.width = 640;
					cam.height = 480;
				} else if (!strncmp (optarg, "d1", 2)) {
					cam.width = 720;
					cam.height = 480;
				} else if (!strncmp (optarg, "720p", 4)) {
					cam.width = 1280;
					cam.height = 720;
				}
			}
			break;
		case 'm':
			if (optarg) {
				if (!strncasecmp (optarg, "mmap", 4)) {
					cam.mode = 0;
				} else if (!strncmp (optarg, "user", 4)) {
					cam.mode = 1;
				}
			}
			break;

		default:
			usage(stderr, argc, argv);
			exit(EXIT_FAILURE);
		}
	}

	if (cam.mode == 0) {
		fprintf(stderr, "Capture at %dx%d using mmap\n", cam.width, cam.height);
		cam.ceu = capture_open(cam.dev_name, cam.width, cam.height);
	} else {
		fprintf(stderr, "Capture at %dx%d using user\n", cam.width, cam.height);
		if (shveu_open() < 0) {
			fprintf (stderr, "Could not open VEU, exiting\n");
			exit (EXIT_FAILURE);
		}
		cam.ceu = capture_open_userio(cam.dev_name, cam.width, cam.height);
	}
	cam.cap_framerate = framerate_new_timer (30.0);

	capture_start_capturing(cam.ceu);

	count = 5;

	for (x=0; x<count; x++)
		capture_get_frame(cam.ceu, process_image, &cam);

	capture_stop_capturing(cam.ceu);
	capture_close(cam.ceu);

	time = (double)framerate_elapsed_time (cam.cap_framerate);
	time /= 1000000;
	fprintf(stderr, "\nCaptured %d frames at %dx%d (%.2f fps)\n",
		count, cam.width, cam.height, (double)count/time);

	framerate_destroy (cam.cap_framerate);
	fflush(stdout);
	if (cam.mode == 1) {
		shveu_close();
	}

	exit(EXIT_SUCCESS);

	return 0;
}
