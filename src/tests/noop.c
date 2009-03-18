/*
 * libshcodecs: A library for controlling SH-Mobile hardware codecs
 * Copyright (C) 2009 Renesas Technology Corp.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA  02110-1301 USA
 */

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
