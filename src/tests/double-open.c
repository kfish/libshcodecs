#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <uiomux/uiomux.h>

#include "uiomux_tests.h"

int
main (int argc, char *argv[])
{
  UIOMux * uiomux, * uiomux_2;
  int ret;

  INFO ("Opening UIOMux for BEU");
  uiomux = uiomux_open(UIOMUX_BLOCK_SH_BEU);
  if (uiomux == NULL)
    FAIL ("Opening UIOMux");

  INFO ("Re-opening UIOMux for BEU");
  uiomux_2 = uiomux_open(UIOMUX_BLOCK_SH_BEU);
  if (uiomux_2 != NULL)
    FAIL ("Re-opening UIOMux for BEU");

  INFO ("Closing UIOMux");
  ret = uiomux_close(uiomux);
  if (ret != 0)
    FAIL ("Closing UIOMux");

  INFO ("Re-opening UIOMux for BEU");
  uiomux_2 = uiomux_open(UIOMUX_BLOCK_SH_BEU);
  if (uiomux_2 == NULL)
    FAIL ("Re-opening UIOMux for BEU");

  INFO ("Closing UIOMux");
  ret = uiomux_close(uiomux_2);
  if (ret != 0)
    FAIL ("Closing UIOMux");

  exit (0);
}
