#ifndef __FRAMERATE_H__
#define __FRAMERATE_H__

#include <stdint.h> /* Definition of uint64_t */
#include <time.h>

struct framerate {
	double fps;
	int timer_fd;
	struct timespec start;
	struct timespec curr;
};

struct framerate * framerate_new (double fps);
int framerate_destroy (struct framerate * framerate);

uint64_t framerate_wait (struct framerate * framerate);
double framerate_elapsed_time (struct framerate * framerate);

#endif /* __FRAMERATE_H__ */
