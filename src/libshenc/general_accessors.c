/*
  Copyright (C) Renesas Technology Corp., 2003-2005. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "encoder_private.h"

/**
 * Get the "frame_number_to_encode" field
 */
long
shcodecs_encoder_get_frame_number_to_encode (SHCodecs_Encoder * encoder)
{
  if (encoder == NULL) return -1;

  return encoder->frame_number_to_encode;
}

/**
 * Set the "frame_number_to_encode" field
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
 * Set the "frame_no_increment" field
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

/*
 * Set the "ref_frame_num" field
 * H.264 only
 */
int
shcodecs_encoder_set_ref_frame_num (SHCodecs_Encoder * encoder, int ref_frame_num)
{
  int old_value;

  if (encoder == NULL) return -1;

  old_value = encoder->ref_frame_num;
  encoder->ref_frame_num = ref_frame_num;

  return old_value;
}

/*
 * Set the "output_filler_enable" field
 * H.264 only
 */
int
shcodecs_encoder_set_output_filler_enable (SHCodecs_Encoder * encoder, int output_filler_enable)
{
  int old_value;

  if (encoder == NULL) return -1;

  old_value = encoder->output_filler_enable;
  encoder->output_filler_enable = output_filler_enable;

  return old_value;
}
