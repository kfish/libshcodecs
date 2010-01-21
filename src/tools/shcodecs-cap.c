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

#include "capture.h"
#include "framerate.h"


static void usage(FILE * fp, int argc, char **argv)
{
	fprintf(fp,
		"Usage: %s [options]\n\n"
		"Options:\n"
		"-d | --device name   Video device name [/dev/video0]\n"
		"-w | --width         Capture width [320]\n"
		"-h | --height        Capture height [240]\n"
		"-s | --size          Capture size [qcif, cif, qvga, vga, d1, 720p]\n"
		"", argv[0]);
	fprintf (fp, "\nPlease report bugs to <linux-sh@vger.kernel.org>\n");
}

static const char short_options[] = "d:w:h:s:";

static const struct option long_options[] = {
	{"device", required_argument, NULL, 'd'},
	{"width",  required_argument, NULL, 'w'},
	{"height", required_argument, NULL, 'h'},
	{"size",   required_argument, NULL, 's'},
	{0, 0, 0, 0}
};

static void
process_image(capture * ceu, const unsigned char *frame_data, size_t length, void *user_data)
{
	struct framerate *cap_framerate = (struct framerate *)user_data;
	fputc('.', stdout);
	fflush(stdout);
	framerate_wait(cap_framerate);
}

int main(int argc, char **argv)
{
	capture *ceu;
	char *dev_name = "/dev/video0";
 	unsigned int width = 320;
 	unsigned int height = 240;
	unsigned int count, x;
	double time;
	struct framerate * cap_framerate;

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
			dev_name = optarg;
			break;

		case 'w':
			if (optarg)
				width = strtoul(optarg, NULL, 10);
			break;
		case 'h':
			if (optarg)
				height = strtoul(optarg, NULL, 10);
			break;
		case 's':
			if (optarg) {
				if (!strncasecmp (optarg, "qcif", 4)) {
					width = 176;
					height = 144;
				} else if (!strncmp (optarg, "cif", 3)) {
					width = 352;
					height = 288;
				} else if (!strncmp (optarg, "qvga", 4)) {
					width = 320;
					height = 240;
				} else if (!strncmp (optarg, "vga", 3)) {
					width = 640;
					height = 480;
				} else if (!strncmp (optarg, "d1", 2)) {
					width = 720;
					height = 480;
				} else if (!strncmp (optarg, "720p", 4)) {
					width = 1280;
					height = 720;
				}
			}
			break;

		default:
			usage(stderr, argc, argv);
			exit(EXIT_FAILURE);
		}
	}

	ceu = capture_open(dev_name, width, height);
	cap_framerate = framerate_new_timer (30.0);

	capture_start_capturing(ceu);

	count = 100;

	for (x=0; x<count; x++)
		capture_get_frame(ceu, process_image, cap_framerate);

	capture_stop_capturing(ceu);
	capture_close(ceu);

	time = (double)framerate_elapsed_time (cap_framerate);
	time /= 1000000;
	printf("\nCaptured %d frames at %dx%d (%.2f fps)\n",
		count, width, height, (double)count/time);

	framerate_destroy (cap_framerate);

	exit(EXIT_SUCCESS);

	return 0;
}
