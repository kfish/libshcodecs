#ifndef __FRAMERATE_H__
#define __FRAMERATE_H__

#include <stdint.h> /* Definition of uint64_t */
#include <time.h>

struct framerate {
	double fps;
	int timer_fd;
	int nr_handled;
	int nr_dropped;
	struct timespec start;
	struct timespec curr;
	double total_elapsed;
	double curr_elapsed;
	double prev_fps;
};

/* Create a framerate object without timer */
struct framerate * framerate_new_measurer (void);

/* Create a framerate object with timer */
struct framerate * framerate_new_timer (double fps);

/* Destroy a framerate object */
int framerate_destroy (struct framerate * framerate);

/* Mark a frame as done, without waiting. Increments nr_handled */
int framerate_mark (struct framerate * framerate);

/* Wait for the next timeout. Increments nr_handled, and will
 * increment nr_dropped by the number of events missed since the last
 * call to framerate_wait() */
uint64_t framerate_wait (struct framerate * framerate);

/* Time in seconds since calling framerate_new_*() */
double framerate_elapsed_time (struct framerate * framerate);

/* Mean average FPS over the entire elapsed time */
double framerate_mean_fps (struct framerate * framerate);

/* Instantaneous FPS ... */
double framerate_instantaneous_fps (struct framerate * framerate);

#endif /* __FRAMERATE_H__ */
