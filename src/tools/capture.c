/*
 *  V4L2 video capture example
 *
 *  This program can be used and distributed without restrictions.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>		/* getopt_long() */

#include <fcntl.h>		/* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>		/* for videodev2.h */

#include <linux/videodev2.h>

#define CLEAR(x) memset (&(x), 0, sizeof (x))

struct buffer {
	void *start;
	size_t length;
};

typedef struct _sh_ceu {
	const char *dev_name;
	int fd;
	struct buffer *buffers;
	unsigned int n_buffers;
	int width;
	int height;
	unsigned int pixel_format;
} sh_ceu;

typedef void (*sh_process_callback) (sh_ceu * ceu, const void *frame_data,
				     size_t length, void *user_data);


static void errno_exit(const char *s)
{
	fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));

	exit(EXIT_FAILURE);
}

static int xioctl(int fd, int request, void *arg)
{
	int r;

	do
		r = ioctl(fd, request, arg);
	while (-1 == r && EINTR == errno);

	return r;
}

static int
read_frame(sh_ceu * ceu, sh_process_callback cb, void *user_data)
{
	struct v4l2_buffer buf;
	unsigned int i;

	CLEAR(buf);

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(ceu->fd, VIDIOC_DQBUF, &buf)) {
		switch (errno) {
		case EAGAIN:
			return 0;

		case EIO:
			/* Could ignore EIO, see spec. */

			/* fall through */

		default:
			errno_exit("VIDIOC_DQBUF");
		}
	}

	assert(buf.index < ceu->n_buffers);

	cb(ceu, ceu->buffers[buf.index].start,
	   ceu->buffers[buf.index].length, user_data);

	if (-1 == xioctl(ceu->fd, VIDIOC_QBUF, &buf))
		errno_exit("VIDIOC_QBUF");

	return 1;
}

void
sh_ceu_capture_frame(sh_ceu * ceu, sh_process_callback cb, void *user_data)
{

	for (;;) {
		fd_set fds;
		struct timeval tv;
		int r;

		FD_ZERO(&fds);
		FD_SET(ceu->fd, &fds);

		/* Timeout. */
		tv.tv_sec = 2;
		tv.tv_usec = 0;

		r = select(ceu->fd + 1, &fds, NULL, NULL, &tv);

		if (-1 == r) {
			if (EINTR == errno)
				continue;

			errno_exit("select");
		}

		if (0 == r) {
			fprintf(stderr, "select timeout\n");
			exit(EXIT_FAILURE);
		}

		if (read_frame(ceu, cb, user_data))
			break;
		/* EAGAIN - continue select loop. */
	}
}

void sh_ceu_stop_capturing(sh_ceu * ceu)
{
	enum v4l2_buf_type type;

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl(ceu->fd, VIDIOC_STREAMOFF, &type))
		errno_exit("VIDIOC_STREAMOFF");

}

void sh_ceu_start_capturing(sh_ceu * ceu)
{
	unsigned int i;
	enum v4l2_buf_type type;

	for (i = 0; i < ceu->n_buffers; ++i) {
		struct v4l2_buffer buf;

		CLEAR(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (-1 == xioctl(ceu->fd, VIDIOC_QBUF, &buf))
			errno_exit("VIDIOC_QBUF");
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl(ceu->fd, VIDIOC_STREAMON, &type))
		errno_exit("VIDIOC_STREAMON");

}

static void uninit_device(sh_ceu * ceu)
{
	unsigned int i;

	for (i = 0; i < ceu->n_buffers; ++i)
		if (-1 ==
		    munmap(ceu->buffers[i].start, ceu->buffers[i].length))
			errno_exit("munmap");
	free(ceu->buffers);
}

static void init_mmap(sh_ceu * ceu)
{
	struct v4l2_requestbuffers req;

	CLEAR(req);

	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(ceu->fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support "
				"memory mapping\n", ceu->dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_REQBUFS");
		}
	}

	if (req.count < 2) {
		fprintf(stderr, "Insufficient buffer memory on %s\n",
			ceu->dev_name);
		exit(EXIT_FAILURE);
	}

	ceu->buffers = calloc(req.count, sizeof(*ceu->buffers));

	if (!ceu->buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (ceu->n_buffers = 0; ceu->n_buffers < req.count;
	     ++ceu->n_buffers) {
		struct v4l2_buffer buf;

		CLEAR(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = ceu->n_buffers;

		if (-1 == xioctl(ceu->fd, VIDIOC_QUERYBUF, &buf))
			errno_exit("VIDIOC_QUERYBUF");

		ceu->buffers[ceu->n_buffers].length = buf.length;
		ceu->buffers[ceu->n_buffers].start =
		    mmap(NULL /* start anywhere */ ,
			 buf.length,
			 PROT_READ | PROT_WRITE /* required */ ,
			 MAP_SHARED /* recommended */ ,
			 ceu->fd, buf.m.offset);

		if (MAP_FAILED == ceu->buffers[ceu->n_buffers].start)
			errno_exit("mmap");
	}
}

static void init_device(sh_ceu * ceu)
{
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	unsigned int min;

	if (-1 == xioctl(ceu->fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s is no V4L2 device\n",
				ceu->dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_QUERYCAP");
		}
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "%s is no video capture device\n",
			ceu->dev_name);
		exit(EXIT_FAILURE);
	}

	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		fprintf(stderr, "%s does not support streaming i/o\n",
			ceu->dev_name);
		exit(EXIT_FAILURE);
	}


	/* Select video input, video standard and tune here. */


	CLEAR(cropcap);

	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (0 == xioctl(ceu->fd, VIDIOC_CROPCAP, &cropcap)) {
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c = cropcap.defrect;	/* reset to default */

		if (-1 == xioctl(ceu->fd, VIDIOC_S_CROP, &crop)) {
			switch (errno) {
			case EINVAL:
				/* Cropping not supported. */
				break;
			default:
				/* Errors ignored. */
				break;
			}
		}
	} else {
		/* Errors ignored. */
	}


	CLEAR(fmt);

	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = ceu->width;
	fmt.fmt.pix.height = ceu->height;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_NV12;
	fmt.fmt.pix.field = V4L2_FIELD_ANY;

	if (-1 == xioctl(ceu->fd, VIDIOC_S_FMT, &fmt)) {
		fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
		if (-1 == xioctl(ceu->fd, VIDIOC_S_FMT, &fmt)) {
			fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB565;
			if (-1 == xioctl(ceu->fd, VIDIOC_S_FMT, &fmt)) {
				errno_exit("VIDIOC_S_FMT");
			}
		}
	}
	ceu->pixel_format = fmt.fmt.pix.pixelformat;

	/* Note VIDIOC_S_FMT may change width and height. */

	/* Buggy driver paranoia. */
	min = fmt.fmt.pix.width * 2;
	if (fmt.fmt.pix.bytesperline < min)
		fmt.fmt.pix.bytesperline = min;
	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min)
		fmt.fmt.pix.sizeimage = min;

	init_mmap(ceu);
}

static void close_device(sh_ceu * ceu)
{
	if (-1 == close(ceu->fd))
		errno_exit("close");

	ceu->fd = -1;
}

static void open_device(sh_ceu * ceu)
{
	struct stat st;


	if (-1 == stat(ceu->dev_name, &st)) {
		fprintf(stderr, "Cannot identify '%s': %d, %s\n",
			ceu->dev_name, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (!S_ISCHR(st.st_mode)) {
		fprintf(stderr, "%s is no device\n", ceu->dev_name);
		exit(EXIT_FAILURE);
	}

	ceu->fd =
	    open(ceu->dev_name, O_RDWR /* required */  | O_NONBLOCK, 0);

	if (-1 == ceu->fd) {
		fprintf(stderr, "Cannot open '%s': %d, %s\n",
			ceu->dev_name, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void sh_ceu_close(sh_ceu * ceu)
{
	uninit_device(ceu);

	close_device(ceu);

	free(ceu);
}

sh_ceu *sh_ceu_open(const char *device_name, int width, int height)
{
	sh_ceu *ceu;

	ceu = malloc(sizeof(*ceu));

	ceu->dev_name = device_name;
	ceu->width = width;
	ceu->height = height;

	open_device(ceu);

	init_device(ceu);

	return ceu;
}

unsigned int sh_ceu_get_pixel_format(sh_ceu * ceu)
{
	return ceu->pixel_format;
}
