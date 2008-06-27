#ifndef __UIOMUX_PRIVATE_H__
#define __UIOMUX_PRIVATE_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <uiomux/uiomux_blocks.h>

typedef struct _UIOMux {
  int fd;
  uiomux_blockmask_t blocks;
} UIOMux;

#endif /* __UIOMUX_PRIVATE_H__ */
