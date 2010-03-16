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

struct private_data {
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
	struct private_data *pvt = (struct private_data *)user_data;
	size_t size = (pvt->width * pvt->height * 3) / 2;

	fputc('.', stderr);
	fflush(stderr);

	if (fwrite(frame_data, 1, size, stdout) != size)
		exit(EXIT_FAILURE);

	capture_queue_buffer (ceu, frame_data);

	framerate_wait(pvt->cap_framerate);
}


int main(int argc, char **argv)
{
	struct private_data pvt;
	unsigned int count, x;
	double time;

	pvt.width = 320;
	pvt.height = 240;
	pvt.dev_name = "/dev/video0";
	pvt.mode = 0;

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
			pvt.dev_name = optarg;
			break;

		case 'w':
			if (optarg)
				pvt.width = strtoul(optarg, NULL, 10);
			break;
		case 'h':
			if (optarg)
				pvt.height = strtoul(optarg, NULL, 10);
			break;
		case 's':
			if (optarg) {
				if (!strncasecmp (optarg, "qcif", 4)) {
					pvt.width = 176;
					pvt.height = 144;
				} else if (!strncmp (optarg, "cif", 3)) {
					pvt.width = 352;
					pvt.height = 288;
				} else if (!strncmp (optarg, "qvga", 4)) {
					pvt.width = 320;
					pvt.height = 240;
				} else if (!strncmp (optarg, "vga", 3)) {
					pvt.width = 640;
					pvt.height = 480;
				} else if (!strncmp (optarg, "d1", 2)) {
					pvt.width = 720;
					pvt.height = 480;
				} else if (!strncmp (optarg, "720p", 4)) {
					pvt.width = 1280;
					pvt.height = 720;
				}
			}
			break;
		case 'm':
			if (optarg) {
				if (!strncasecmp (optarg, "mmap", 4)) {
					pvt.mode = 0;
				} else if (!strncmp (optarg, "user", 4)) {
					pvt.mode = 1;
				}
			}
			break;

		default:
			usage(stderr, argc, argv);
			exit(EXIT_FAILURE);
		}
	}

	if (pvt.mode == 0) {
		fprintf(stderr, "Capture at %dx%d using mmap\n", pvt.width, pvt.height);
		pvt.ceu = capture_open(pvt.dev_name, pvt.width, pvt.height);
	} else {
		fprintf(stderr, "Capture at %dx%d using user\n", pvt.width, pvt.height);
		if (shveu_open() < 0) {
			fprintf (stderr, "Could not open VEU, exiting\n");
			exit (EXIT_FAILURE);
		}
		pvt.ceu = capture_open_userio(pvt.dev_name, pvt.width, pvt.height);
	}
	pvt.cap_framerate = framerate_new_timer (30.0);

	capture_start_capturing(pvt.ceu);

	count = 5;

	for (x=0; x<count; x++)
		capture_get_frame(pvt.ceu, process_image, &pvt);

	capture_stop_capturing(pvt.ceu);
	capture_close(pvt.ceu);

	time = (double)framerate_elapsed_time (pvt.cap_framerate);
	time /= 1000000;
	fprintf(stderr, "\nCaptured %d frames at %dx%d (%.2f fps)\n",
		count, pvt.width, pvt.height, (double)count/time);

	framerate_destroy (pvt.cap_framerate);
	fflush(stdout);
	if (pvt.mode == 1) {
		shveu_close();
	}

	exit(EXIT_SUCCESS);

	return 0;
}
