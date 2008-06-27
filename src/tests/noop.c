#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <uiomux/uiomux.h>

#include "uiomux_tests.h"

int
main (int argc, char *argv[])
{
  UIOMux * uiomux;
  int ret;

  INFO ("Opening UIOMux");
  uiomux = uiomux_open(UIOMUX_BLOCK_SH_BEU);
  if (uiomux == NULL)
    FAIL ("Opening UIOMux");

  INFO ("Closing UIOMux");
  ret = uiomux_close(uiomux);
  if (ret != 0)
    FAIL ("Closing UIOMux");

  exit (0);
}
