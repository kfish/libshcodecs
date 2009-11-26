#ifndef __FRAMERATE_H__
#define __FRAMERATE_H__

#include <stdint.h> /* Definition of uint64_t */

int framerate_init (double fps);
uint64_t framerate_wait (int fd);

#endif /* __FRAMERATE_H__ */
