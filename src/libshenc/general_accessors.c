/*
  Copyright (C) Renesas Technology Corp., 2003-2005. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "encoder_private.h"

/**
 * Get the frame_number_to_encode
 */
long
shcodecs_encoder_get_frame_number_to_encode (SHCodecs_Encoder * encoder)
{
  if (encoder == NULL) return -1;

  return encoder->frame_number_to_encode;
}

/**
 * Set the frame_number_to_encode
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

/**
 * Get the frame_number_increment
 */
long
shcodecs_encoder_get_frame_no_increment (SHCodecs_Encoder * encoder)
{
  if (encoder == NULL) return -1;

  return encoder->frame_no_increment;
}

/*
 * Set the frame_no_increment
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

