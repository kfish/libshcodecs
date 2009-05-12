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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "encoder_private.h"

/*
 * Get the frame_number_to_encode
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The frame_number_to_encode
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_get_frame_number_to_encode (SHCodecs_Encoder * encoder)
{
  if (encoder == NULL) return -1;

  return encoder->frame_number_to_encode;
}

/*
 * Set the frame_number_to_encode
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The previous value of \a frame_number_to_encode
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_frame_number_to_encode (SHCodecs_Encoder * encoder, long frame_number_to_encode)
{
  long old_value;

  if (encoder == NULL) return -1;

  old_value = encoder->frame_number_to_encode;
  encoder->frame_number_to_encode = frame_number_to_encode;

  return old_value;
}

/*
 * Get the frame_no_increment
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The frame_no_increment
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_get_frame_no_increment (SHCodecs_Encoder * encoder)
{
  if (encoder == NULL) return -1;

  return encoder->frame_no_increment;
}

/*
 * Set the frame_no_increment
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The previous value of \a frame_no_increment
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_frame_no_increment (SHCodecs_Encoder * encoder, long frame_no_increment)
{
  long old_value;

  if (encoder == NULL) return -1;

  old_value = encoder->frame_no_increment;
  encoder->frame_no_increment = frame_no_increment;

  return old_value;
}

