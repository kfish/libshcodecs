
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "uiomux_private.h"

#if 0
static char * devname = "/dev/uio0";
#else
static char * devname = "/dev/zero";
#endif

static uiomux_blockmask_t allocated = UIOMUX_BLOCK_NONE;

uiomux_blockmask_t
uiomux_query(void)
{
  return UIOMUX_BLOCK_SH_BEU |
         UIOMUX_BLOCK_SH_CEU |
         UIOMUX_BLOCK_SH_VPU ;
}

const char *
uiomux_name(uiomux_blockmask_t block)
{
  switch (block) {
    case UIOMUX_BLOCK_SH_BEU:
      return "BEU";
      break;
    case UIOMUX_BLOCK_SH_CEU:
      return "CEU";
      break;
    case UIOMUX_BLOCK_SH_VPU:
      return "VPU";
      break;
    default:
      return NULL;
  }
}

UIOMux *
uiomux_open (uiomux_blockmask_t blocks)
{
  UIOMux * uiomux;

  /* Check if the allocation is allowed */
  if ((blocks & allocated) != 0) {
    return NULL;
  }

  /* If so, go for it ... */
  uiomux = malloc (sizeof(*uiomux));
  if ((uiomux->fd = open (devname, O_RDONLY)) == -1) {
    free (uiomux);
    return NULL;
  } 

  uiomux->blocks = blocks;

  allocated |= blocks;

  return uiomux;
}

int
uiomux_close (UIOMux * uiomux)
{
  if (uiomux == NULL) {
    return -1;
  }

  /* Deallocate blocks */
  allocated ^= uiomux->blocks;

  free (uiomux);

  return 0;
}

int
uiomux_poll(UIOMux * uiomux)
{
  return 0;
}

int
uiomux_read(UIOMux * uiomux)
{
  /* Read from UIO */

  return 0;
}

int
uiomux_write(UIOMux * uiomux)
{
  /* Write to UIO */

  return 0;
}

int
uiomux_mmap(UIOMux * uiomux)
{
  /* Handle mmap */

  return 0;
}
