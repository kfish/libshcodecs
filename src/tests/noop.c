#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <uiomux/uiomux.h>

#include "uiomux_tests.h"

int
main (int argc, char *argv[])
{
  int ret;

  INFO ("Opening UIOMux");
  ret = uiomux_open();
  if (ret != 0)
    FAIL ("Opening UIOMux");

  INFO ("Closing UIOMux");
  ret = uiomux_close();
  if (ret != 0)
    FAIL ("Closing UIOMux");

  exit (0);
}
