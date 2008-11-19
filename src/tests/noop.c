#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <shcodecs/shcodecs_decoder.h>

#include "shcodecs_tests.h"

int
main (int argc, char *argv[])
{
  SHCodecs_Decoder * decoder;
  int ret;

  INFO ("Opening SHCodecs_Decoder");
  decoder = shcodecs_decoder_init(320, 240, SHCodecs_Format_MPEG4);
  if (decoder == NULL)
    FAIL ("Opening SHCodecs_Decoder");

  INFO ("Closing SHCodecs_Decoder");
  shcodecs_decoder_close(decoder);

  exit (0);
}
