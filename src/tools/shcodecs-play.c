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
 * Program to decode video and show on the display.
 *
 * This plays an Elementary Stream. Whilst it is possible to get the width & 
 * height of the video from this, libshcodecs does not support this as the
 * information is available in the file container. Therefore, for streams
 * other than QVGA, you must specify the width & height of the video being
 * decoded.
 *
 * The code relies on the fact that the VPU never writes the same output buffer
 * for two consecutive frames that are output. This allows the output buffer to
 * be treated as though it is double-buffered, i.e. whilst the VPU is decoding
 * the next frame, in parallel the image is sent to the display.
 *
 */

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
#include <linux/fb.h>
#include <linux/ioctl.h>
#include <aio.h>
#include <pthread.h>
#include <errno.h>
#include <shcodecs/shcodecs_decoder.h>
#include "veu_colorspace.h"
#include "framerate.h"

#define DEFAULT_WIDTH 320
#define DEFAULT_HEIGHT 240
#define DEFAULT_FPS	25

#define _DEBUG
#ifdef _DEBUG
        #define debug_printf printf
#else
        void debug_printf(const char *format, ...) {}
#endif


/* limitation of VEU2H on SH7723 */
//#define VEU2H_MAX_WIDTH 640
#define VEU2H_MAX_WIDTH 1280

#define LCD_BPP 2
#define U_SEC_PER_SEC 1000000

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

struct private_data {
	/* Display data */
	int fb_handle;
	int fb_index;
	struct fb_fix_screeninfo fb_fix;
	struct fb_var_screeninfo fb_var;
	unsigned char *fb_base;
	unsigned char *fb_screenMem;

	/* Output thread related data */
	pthread_mutex_t	 	mutex;			/* Protect data */
	pthread_cond_t	  	avail;			/* Frame decoded */
	pthread_cond_t	  	ready;			/* Scale ready for next frame */
	int				 	busy;			/* Scale being performed */
	unsigned char *p_frame_y;
	unsigned char *p_frame_c;

	/* File read & input data */
	struct aiocb read_aiocb;
	unsigned char *input_buf;
	unsigned char *read_buf;
	int input_buf_len;

	int loop;

	struct framerate * framerate;

	int lcd_w;          /* LCD size */
	int lcd_h;

	int dst_w;          /* Size of video output */
	int dst_h;

	int dst_p;          /* Position of video output */
	int dst_q;

	int src_w;          /* Size of video input */
	int src_h;

	int max_nal_size;
};

static void
usage (const char *progname)
{
        printf ("Usage: %s [options] filename\n", progname);
        printf ("Decode a MPEG-4 or H.264 elementry stream and show on the LCD.\n");
        printf ("\nFile options\n");
        printf ("  -i, --input        Set the video input filename\n");
	printf ("  -l, --loop         Loop playback\n");
        printf ("  -r                 Set the playback speed, frames per second\n");
        printf ("\nEncoding format\n");
        printf ("  -f, --format       The file format [h264, mpeg4]\n");
        printf ("\nDimensions of encoded stream\n");
        printf ("  -w, --width        The image width in pixels of the file\n");
        printf ("  -h, --height       The image height in pixels of the file\n");
        printf ("  -s, --input size   Set the input image size [qcif, cif, qvga, vga, d1, 720p]\n");
        printf ("\nDimensions of video on the display\n");
        printf ("  -x,                The image width in pixels on the display\n");
        printf ("  -y,                The image height in pixels on the display\n");
        printf ("  -S, --output-size  Set the image display size [qcif, cif, qvga, vga, d1, 720p]\n");
        printf ("\nPosition of video on the display\n");
        printf ("  -p,                The horizontal offset in pixels\n");
        printf ("  -q,                The vertical offset in pixels\n");
        printf ("\nFile extensions are interpreted as follows unless otherwise specified:\n");
        printf ("  .m4v    MPEG4\n");
        printf ("  .264    H.264\n");
        printf ("\nPlease report bugs to <linux-sh@vger.kernel.org>\n");
}

static char * optstring = "f:li:w:h:r:x:y:a:s:S:p:q:";

#ifdef HAVE_GETOPT_LONG
static struct option long_options[] = {
	{ "format", required_argument, NULL, 'f'},
	{ "input" , required_argument, NULL, 'i'},
	{ "loop" , no_argument, NULL, 'l'},
	{ "width" , required_argument, NULL, 'w'},
	{ "height", required_argument, NULL, 'h'},
	{ "input-size"  , required_argument, NULL, 's'},
	{ "output-size"  , required_argument, NULL, 'S'},
};
#endif

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


static int display_open(struct private_data *pvt, const char *device)
{
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
		fprintf(stderr, "This program can only handle packed pixel frame buffers.\n");
		return 0;
	}

	pvt->lcd_w = pvt->fb_var.xres;
	pvt->lcd_h = pvt->fb_var.yres;

	pvt->fb_screenMem = pvt->fb_base = (unsigned char*)pvt->fb_fix.smem_start;
	pvt->fb_index = 0;
	display_flip(pvt);

	return 1;
}

static void display_close(struct private_data *pvt)
{
	close(pvt->fb_handle);
}

/*****************************************************************************/

/*
 * Get the next pointer to the input bitstream after decoding a frame/slice.
 * consumed: the number of bytes that have just been consumed.
 *
 * The decoder must be presented with compressed data that consists of at least
 * one whole unit (i.e. a NAL or VOP).
 */
static int update_input(struct private_data *pvt, int consumed)
{
	struct aiocb *aio = &pvt->read_aiocb;
        const struct aiocb * cblist[1];
	int read_len, ret;

	/* Copy the unused stream data to the start of the buffer */
	pvt->input_buf_len -= consumed;
	memmove(pvt->input_buf, pvt->input_buf+consumed, pvt->input_buf_len);

        /* Set up cblist */
        cblist[0] = (const struct aiocb *)aio;

	if (pvt->input_buf_len < pvt->max_nal_size) {
		/* Wait for pending read to complete */
		aio_suspend(cblist, 1, NULL);

		read_len = aio_return(aio);

		if (pvt->loop && read_len == 0) {
			/* Seek to the beginning of the file */
			if (lseek (pvt->read_aiocb.aio_fildes, 0, SEEK_SET) == -1)
				return 0;

			aio->aio_nbytes = pvt->max_nal_size;
			aio->aio_offset = 0;

			/* Start a new read */
			ret = aio_read(aio);
			if (ret < 0) {
				fprintf(stderr, "aio_read failed!\n");
				return ret;
			}

			/* Wait for pending read to complete */
			aio_suspend(cblist, 1, NULL);

			read_len = aio_return(aio);
		}

		if (read_len > 0) {
			/* Copy the new stream data after the unused data */
			memcpy(pvt->input_buf + pvt->input_buf_len, pvt->read_buf, read_len);
			pvt->input_buf_len += read_len;

			aio->aio_nbytes = pvt->max_nal_size;
			aio->aio_offset += read_len;

			/* Start a new read */
			ret = aio_read(aio);
			if (ret < 0) {
				fprintf(stderr, "aio_read failed!\n");
				return ret;
			}
		}
	}

	return pvt->input_buf_len;
}

static int file_read_init(struct private_data *pvt, char *video_filename)
{
	struct aiocb *aio = &pvt->read_aiocb;
	int ret;

	pvt->input_buf_len = 0;
	bzero((char*)aio, sizeof(struct aiocb));

	aio->aio_fildes = open(video_filename, O_RDONLY);
	if (aio->aio_fildes == -1) {
		fprintf(stderr, "Open %s: %s.\n", video_filename, strerror(errno));
		return -1;
	}

	/* The input buffer is twice the size of the largest NAL/VOP becuase we
	   could have 99% of one NAL/VOP unused in the buffer and need to store
	   the next read data after this */
	pvt->input_buf = malloc(pvt->max_nal_size * 2);
	pvt->read_buf  = malloc(pvt->max_nal_size);
	if (pvt->input_buf == NULL || pvt->read_buf == NULL ) {
		fprintf(stderr, "Memory allocation failed!\n");
		return -1;
	}

	aio->aio_buf = pvt->read_buf;
	aio->aio_nbytes = pvt->max_nal_size;
	aio->aio_offset = 0;

	/* Start a new read */
	ret = aio_read(&pvt->read_aiocb);
	if (ret < 0) {
		fprintf(stderr, "aio_read failed!\n");
		return ret;
	}

	/* Wait for initial read to complete */
	return update_input(pvt, 0);
}

static void file_read_deinit(struct private_data *pvt)
{
	close(pvt->read_aiocb.aio_fildes);
	free(pvt->input_buf);
	free(pvt->read_buf);
}

/*****************************************************************************/

void *output_thread(void *data)
{
	int dst_offset;
	struct private_data *pvt = (struct private_data*)data;

	/* Position */
	int x_offset = pvt->dst_p;
	int y_offset = pvt->dst_q;

	pthread_mutex_lock (&pvt->mutex);

	while (1)
	{
		/* Wait for new frame from the decoder */
		while (pvt->busy != 1) {
			pthread_cond_wait (&pvt->avail, &pvt->mutex);
		}

		/* Use the VEU to scale & perform colour space conversion */
		dst_offset = ((y_offset * pvt->lcd_w) + x_offset) * LCD_BPP;
		sh_veu_operation(0, pvt->p_frame_y, pvt->p_frame_c,
			      pvt->src_w, pvt->src_h, pvt->src_w, YCbCr420,
			      pvt->fb_screenMem + dst_offset, NULL,
			      pvt->dst_w, pvt->dst_h, pvt->lcd_w, RGB565, 0);

		display_flip(pvt);

		/* Signal that we are ready for the next frame */
		pvt->busy = 0;
		pthread_cond_signal (&pvt->ready);
	}

	pthread_exit(NULL);
}


static void
frame_ready(struct private_data *pvt, unsigned char *y_in, unsigned char *c_in)
{
	pthread_mutex_lock (&pvt->mutex);

	/* Ensure the output thread is ready, we don't want the decoder running
        faster than the output */
	while (pvt->busy) {
		pthread_cond_wait (&pvt->ready, &pvt->mutex);
	}

	/* Variables shared between the threads */
	pvt->p_frame_y = y_in;
	pvt->p_frame_c = c_in;
	pvt->busy = 1;

	/* Signal the output thread that a frame is available */
	pthread_cond_signal (&pvt->avail);
	pthread_mutex_unlock (&pvt->mutex);
}


/* local output callback */
static int
local_vpu4_decoded (SHCodecs_Decoder *decoder,
	unsigned char *y_buf, int y_size,
	unsigned char *c_buf, int c_size,
	void *user_data)
{
	struct timeval tcurrent, tsleep, texpected;
	struct private_data *pvt = (struct private_data*)user_data;

	framerate_wait(pvt->framerate);

	frame_ready(pvt, y_buf, c_buf);

	return 0;
}


/*****************************************************************************/

int main(int argc, char **argv)
{
	SHCodecs_Decoder *decoder;
	int stream_type = -1;
	int c, i, rc, bytes_decoded;
	char video_filename[MAXPATHLEN];
	const char *fbname;
	double fps, time;
	struct private_data pvt_data;
	struct private_data *pvt;
	pthread_t thread_output;
	char * ext;

	if (argc == 1) {
		usage(argv[0]);
		exit(0);
	}

	pvt = &pvt_data;

	/* Initialize display */
	fbname = getenv("FRAMEBUFFER");
	if (!fbname) {
		if (access("/dev/.devfsd", F_OK) == 0) {
			fbname = "/dev/fb/0";
		} else {
			fbname = "/dev/fb0";
		}
	}
	if (!display_open(pvt, fbname)) {
		exit(-3);
	}

	setvbuf(stdout, NULL, _IONBF, 0);
	video_filename[MAXPATHLEN-1] = '\0';

	/* Set defaults */
	pvt->loop = 0;
	pvt->src_w  = DEFAULT_WIDTH;
	pvt->src_h = DEFAULT_HEIGHT;
	pvt->dst_p = 0;
	pvt->dst_q = 0;
	pvt->dst_w = pvt->lcd_w;
	pvt->dst_h = pvt->lcd_h;

	fps = DEFAULT_FPS;

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
		case 'f':
			if (strncmp(optarg, "mpeg4", 5) == 0)
				stream_type = SHCodecs_Format_MPEG4;
			else if (strncmp(optarg, "h264", 4) == 0)
				stream_type = SHCodecs_Format_H264;
			else{
				debug_printf("argument: Unknown video format: %s.\n",optarg);
				exit(-1);
			}
			break;
		case 'i':
			if (optarg)
				strncpy(video_filename, optarg, MAXPATHLEN-1);
			break;
		case 'l':
			pvt->loop = 1;
			break;
		case 'w':
			if (optarg)
				pvt->src_w = strtoul(optarg, NULL, 10);
			break;
		case 'h':
			if (optarg)
				pvt->src_h = strtoul(optarg, NULL, 10);
			break;
		case 's':
			if (optarg) {
				if (!strncasecmp (optarg, "qcif", 4)) {
					pvt->src_w = 176;
					pvt->src_h = 144;
				} else if (!strncmp (optarg, "cif", 3)) {
					pvt->src_w = 352;
					pvt->src_h = 288;
				} else if (!strncmp (optarg, "qvga", 4)) {
					pvt->src_w = 320;
					pvt->src_h = 240;
				} else if (!strncmp (optarg, "vga", 3)) {
					pvt->src_w = 640;
					pvt->src_h = 480;
				} else if (!strncmp (optarg, "d1", 2)) {
					pvt->src_w = 720;
					pvt->src_h = 480;
				} else if (!strncmp (optarg, "720p", 4)) {
					pvt->src_w = 1280;
					pvt->src_h = 720;
				}
			}
			break;
		case 'r':
			if (optarg)
				fps = strtod(optarg, NULL);
			break;
		case 'x':
			if (optarg)
				pvt->dst_w = strtoul(optarg, NULL, 10);
				if (pvt->dst_w > pvt->lcd_w) {
					pvt->dst_w = pvt->lcd_w;
					debug_printf("Limiting output width to LCD width (%d)\n", pvt->dst_w);
				}
			break;
		case 'y':
			if (optarg)
				pvt->dst_h = strtoul(optarg, NULL, 10);
				if (pvt->dst_h > pvt->lcd_h) {
					pvt->dst_h = pvt->lcd_h;
					debug_printf("Limiting output height to LCD height (%d)\n", pvt->dst_h);
				}
			break;
		case 'p':
			if (optarg)
				pvt->dst_p = strtoul(optarg, NULL, 10);
			break;
		case 'q':
			if (optarg)
				pvt->dst_q = strtoul(optarg, NULL, 10);
			break;
		case 'S':
			if (optarg) {
				if (!strncasecmp (optarg, "qcif", 4)) {
					pvt->dst_w = 176;
					pvt->dst_h = 144;
				} else if (!strncmp (optarg, "cif", 3)) {
					pvt->dst_w = 352;
					pvt->dst_h = 288;
				} else if (!strncmp (optarg, "qvga", 4)) {
					pvt->dst_w = 320;
					pvt->dst_h = 240;
				} else if (!strncmp (optarg, "vga", 3)) {
					pvt->dst_w = 640;
					pvt->dst_h = 480;
				} else if (!strncmp (optarg, "d1", 2)) {
					pvt->dst_w = 720;
					pvt->dst_h = 480;
				} else if (!strncmp (optarg, "720p", 4)) {
					pvt->dst_w = 1280;
					pvt->dst_h = 720;
				}
			}
			break;
		default:
			usage(argv[0]);
			exit(-2);
		}
	}

        if (optind > argc) {
	      usage (argv[0]);
              goto exit_err;
        }

	if (optind == (argc-1) && video_filename[0] == '\0') {
		strncpy(video_filename, argv[optind++], MAXPATHLEN-1);
	}
	if ( (strcmp(video_filename, "-") == 0) || (video_filename[0] == '\0') ){
		fprintf(stderr, "Invalid input file.\n");
		exit(-4);
	}

	if (pvt->src_w == -1 || pvt->src_h == -1){
		fprintf(stderr, "Invalid source width and/or height specified.\n");
		exit(-3);
	}
	if (pvt->src_w < SHCODECS_MIN_FX || pvt->src_w > SHCODECS_MAX_FX
	    || pvt->src_h < SHCODECS_MIN_FY || pvt->src_h > SHCODECS_MAX_FY) {
		fprintf(stderr, "Invalid source width and/or height specified.\n");
		exit(-6);
	}

        /* H.264 spec: Max NAL size is the size of an uncomrpessed immage divided
           by the "Minimum Compression Ratio", MinCR. This is 2 for most levels
           but is 4 for levels 3.1 to 4. Since we don't know the level, we just
           use MinCR=2. */
        pvt->max_nal_size = (pvt->src_w * pvt->src_h * 3) / 2; /* YCbCr420 */
        pvt->max_nal_size /= 2; /* Apply MinCR */

	/* Ensure the output is no bigger than the LCD */
	pvt->dst_p = min(pvt->dst_p, pvt->lcd_w);
	pvt->dst_q = min(pvt->dst_q, pvt->lcd_h);
	pvt->dst_w = min(pvt->dst_w, pvt->lcd_w - pvt->dst_p);
	pvt->dst_h = min(pvt->dst_h, pvt->lcd_h - pvt->dst_q);


	/* If scaling down and destination is larger than VGA width, limit to VGA */
	if (pvt->src_w > pvt->dst_w)
		pvt->dst_w = min(pvt->dst_w, VEU2H_MAX_WIDTH);
	if (pvt->src_h > pvt->dst_h)
		pvt->dst_h = min(pvt->dst_h, VEU2H_MAX_WIDTH);

	if (stream_type == -1) {
		ext = strrchr (video_filename, '.');
		if (ext == NULL || !strncmp (ext, ".264", 4))
			stream_type = SHCodecs_Format_H264;
		else
			stream_type = SHCodecs_Format_MPEG4;
	}

	debug_printf("Input video file:   %s\n", video_filename);
	debug_printf("Format:             %s\n", stream_type == SHCodecs_Format_H264 ? "H.264" : "MPEG4");
	debug_printf("File resolution:    %dx%d\n", pvt->src_w, pvt->src_h);
	debug_printf("Display resolution: %dx%d\n", pvt->lcd_w, pvt->lcd_h);
	debug_printf("Output resolution:  %dx%d\n", pvt->dst_w, pvt->dst_h);
	debug_printf("Output position:    %dx%d\n", pvt->dst_p, pvt->dst_q);

	/* Output thread initialisation */
	pthread_mutex_init (&pvt->mutex, NULL);
	pthread_cond_init (&pvt->avail, NULL);
	pthread_cond_init (&pvt->ready, NULL);
	pvt->busy = 0;

	rc = pthread_create(&thread_output, NULL, output_thread, pvt);
	if (rc){
		fprintf(stderr, "Creating output thread failed, exiting\n");
		exit(-1);
	}

	/* Open the VEU UIO */
	sh_veu_open ();

	/* Open file descriptors to talk to the VPU driver */
	if ((decoder = shcodecs_decoder_init(pvt->src_w, pvt->src_h, stream_type)) == NULL)
		exit (-9);

	/* setup callback for frame decoded  & make the decode use physical addresses */
	shcodecs_decoder_set_decoded_callback (decoder, local_vpu4_decoded, pvt);
	shcodecs_decoder_set_use_physical (decoder, 1);

	/* File read initialisation */
	if (file_read_init(pvt, video_filename) < 0)
		exit(-1);

	/* Initialize framerate timer */
	pvt->framerate = framerate_new_timer (fps);

	/* decode main loop */
	do {
		bytes_decoded = shcodecs_decode(decoder, pvt->input_buf, pvt->input_buf_len);

	} while (bytes_decoded > 0 && update_input(pvt, bytes_decoded) > 0);

	/* Finalize the decode output, in case a final MPEG4 frame is available */
	shcodecs_decoder_finalize (decoder);

	time = framerate_elapsed_time (pvt->framerate);

	/* Wait for output thread to finish */
	pthread_mutex_lock (&pvt->mutex);
	while (pvt->busy) {
		pthread_cond_wait (&pvt->ready, &pvt->mutex);
	}

	shcodecs_decoder_close(decoder);
	file_read_deinit(pvt);
	sh_veu_close();
	display_close(pvt);

	pthread_mutex_destroy(&pvt->mutex);
	pthread_cond_destroy(&pvt->avail);
	pthread_cond_destroy(&pvt->ready);

	debug_printf("Elapsed time:       %0.3g s\n", time);
	debug_printf("Late frames:        %d\n", pvt->framerate->nr_dropped);
	debug_printf("Displayed %d frames (%.2f fps)\n",
			pvt->framerate->nr_handled,
			framerate_mean_fps (pvt->framerate));

	framerate_destroy (pvt->framerate);

exit_ok:
	exit (0);

exit_err:
	exit (1);
}

