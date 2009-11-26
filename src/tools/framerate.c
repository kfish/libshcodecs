#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>		/* Definition of uint64_t */

#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)

int framerate_init (double fps)
{
	struct itimerspec new_value;
	struct timespec now;
        long interval;
	int fd;

        interval = (long) (1000000000 / fps);

	if (clock_gettime(CLOCK_REALTIME, &now) == -1)
		handle_error("clock_gettime");

	/* Create a CLOCK_REALTIME absolute timer with initial
	 * expiration and interval as specified in command line */

	new_value.it_value.tv_sec = now.tv_sec;
	new_value.it_value.tv_nsec = now.tv_nsec;
	new_value.it_interval.tv_sec = 0;
	new_value.it_interval.tv_nsec = interval;

	fd = timerfd_create(CLOCK_REALTIME, 0);
	if (fd == -1)
		handle_error("timerfd_create");

	if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) == -1)
		handle_error("timerfd_settime");

        return fd;
}

uint64_t
framerate_wait (int fd)
{
	ssize_t s;
        uint64_t exp;

	s = read(fd, &exp, sizeof(uint64_t));
	if (s != sizeof(uint64_t))
		handle_error("read");

        return exp;
}
