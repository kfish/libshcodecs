#include <unistd.h>
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>		/* Definition of uint64_t */

#include "framerate.h"

#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define N_SEC_PER_SEC 1000000000

struct framerate * framerate_new_measurer (void)
{
	struct framerate * framerate;
	struct timespec * now;

	framerate = calloc (1, sizeof(*framerate));
	if (framerate == NULL)
		return NULL;

	now = &framerate->start;

	if (clock_gettime(CLOCK_MONOTONIC, now) == -1)
		goto err_out;

	framerate->curr.tv_sec = now->tv_sec;
	framerate->curr.tv_nsec = now->tv_nsec;

	return framerate;

err_out:
	free (framerate);
	return NULL;
}

struct framerate * framerate_new_timer (double fps)
{
	struct framerate * framerate;
	struct itimerspec new_value;
	struct timespec * now;
        long interval;

	if (fps < 0.0)
		return NULL;

	framerate = framerate_new_measurer ();
	if (framerate == NULL)
		return NULL;

	/* Treat fps == 0.0 as a measurer, and avoid divide-by-zero */
	if (fps == 0.0)
		return framerate;

        interval = (long) (N_SEC_PER_SEC / fps);

	now = &framerate->start;

	/* Create a CLOCK_REALTIME absolute timer with initial
	 * expiration "now" and interval for the given framerate */

	new_value.it_value.tv_sec = now->tv_sec;
	new_value.it_value.tv_nsec = now->tv_nsec;
	new_value.it_interval.tv_sec = 0;
	new_value.it_interval.tv_nsec = interval;

	framerate->timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (framerate->timer_fd == -1)
		goto err_out;

	if (timerfd_settime(framerate->timer_fd, TFD_TIMER_ABSTIME,
				&new_value, NULL) == -1)
		goto err_out;

        return framerate;

err_out:
	free (framerate);
	return NULL;
}

int framerate_destroy (struct framerate * framerate)
{
	int ret=0;

	if (framerate->timer_fd != 0)
	    ret = close (framerate->timer_fd);

	free (framerate);

	return ret;
}

double framerate_elapsed_time (struct framerate * framerate)
{
	return framerate->total_elapsed;
}

double framerate_mean_fps (struct framerate * framerate)
{
	double elapsed = framerate_elapsed_time (framerate);

	return (double)framerate->nr_handled/framerate->total_elapsed;
}

double framerate_instantaneous_fps (struct framerate * framerate)
{
	double curr_fps;

	if (framerate->curr_elapsed == 0.0)
		return 0.0;

	curr_fps = 1.0/framerate->curr_elapsed;
	curr_fps += 127.0 * framerate->prev_fps;
	curr_fps /= 128.0;

	framerate->prev_fps = curr_fps;

	return curr_fps;
}

int framerate_mark (struct framerate * framerate)
{
	long secs, nsecs;
	double prev_elapsed;
	int ret;

	framerate->nr_handled++;

	ret = clock_gettime(CLOCK_MONOTONIC, &framerate->curr);
	if (ret == -1) return ret;

	secs = framerate->curr.tv_sec - framerate->start.tv_sec;
	nsecs = framerate->curr.tv_nsec - framerate->start.tv_nsec;
	if (nsecs < 0) {
		secs--;
		nsecs += N_SEC_PER_SEC;
	}

	prev_elapsed = framerate->total_elapsed;

	framerate->total_elapsed = secs + (double)nsecs/N_SEC_PER_SEC;
	framerate->curr_elapsed = framerate->total_elapsed - prev_elapsed;

	return ret;
}

uint64_t
framerate_wait (struct framerate * framerate)
{
	ssize_t s;
        uint64_t exp;

	s = read(framerate->timer_fd, &exp, sizeof(uint64_t));
	if (s != sizeof(uint64_t))
		handle_error("read");

	framerate_mark (framerate);

	framerate->nr_dropped += exp-1;

        return exp;
}
