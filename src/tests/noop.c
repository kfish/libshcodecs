#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rsovpu4/rsovpu4_decoder.h>

#include "rsovpu4_tests.h"

int
main (int argc, char *argv[])
{
  RSOVPU4_Decoder * decoder;
  int ret;

  INFO ("Opening RSOVPU4_Decoder");
  decoder = rsovpu4_decoder_init(320, 240, RSOVPU4_Format_MPEG4);
  if (decoder == NULL)
    FAIL ("Opening RSOVPU4_Decoder");

  INFO ("Closing RSOVPU4_Decoder");
  rsovpu4_decoder_close(decoder);

  exit (0);
}
