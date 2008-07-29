/*
 *  V4L2 video capture example
 *
 *  This program can be used and distributed without restrictions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>

#include "capture.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))

static void
usage                           (FILE *                 fp,
                                 int                    argc,
                                 char **                argv)
{
        fprintf (fp,
                 "Usage: %s [options]\n\n"
                 "Options:\n"
                 "-d | --device name   Video device name [/dev/video]\n"
                 "-h | --help          Print this message\n"
                 "",
		 argv[0]);
}

static const char short_options [] = "d:hmru";

static const struct option
long_options [] = {
        { "device",     required_argument,      NULL,           'd' },
        { "help",       no_argument,            NULL,           'h' },
        { 0, 0, 0, 0 }
};

static void
process_image                   (const void * frame_data, size_t length, void * user_data)
{
        fputc ('.', stdout);
        fflush (stdout);
}

int
main                            (int                    argc,
                                 char **                argv)
{
	sh_veu * veu;
        char * dev_name = "/dev/video0";
	unsigned int count;

        for (;;) {
                int index;
                int c;
                
                c = getopt_long (argc, argv,
                                 short_options, long_options,
                                 &index);

                if (-1 == c)
                        break;

                switch (c) {
                case 0: /* getopt_long() flag */
                        break;

                case 'd':
                        dev_name = optarg;
                        break;

                case 'h':
                        usage (stdout, argc, argv);
                        exit (EXIT_SUCCESS);

                default:
                        usage (stderr, argc, argv);
                        exit (EXIT_FAILURE);
                }
        }

        veu = sh_veu_open (dev_name, 320, 240);

        sh_veu_start_capturing (veu);

        count = 100;

        while (count-- > 0)
          sh_veu_capture_frame (veu, process_image, NULL);

        sh_veu_stop_capturing (veu);

        sh_veu_close (veu);

        exit (EXIT_SUCCESS);

        return 0;
}
