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

#include <stdlib.h>

#include "encoder_private.h"

/*
 * Get the ref_frame_num.
 * Valid for H.264 only.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The ref_frame_num
 * \retval -1 \a encoder invalid
 */
int
shcodecs_encoder_get_ref_frame_num (SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->ref_frame_num;
}

/*
 * Set the ref_frame_num.
 * Valid for H.264 only.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The previous value of \a ref_frame_num
 * \retval -1 \a encoder invalid
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
 * Get the output_filler_enable.
 * Valid for H.264 only.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The output_filler_enable
 * \retval -1 \a encoder invalid
 */
int
shcodecs_encoder_get_output_filler_enable (SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->output_filler_enable;
}

/*
 * Set the output_filler_enable.
 * Valid for H.264 only.
* \param encoder The SHCodecs_Encoder* handle
* \returns The previous value of \a output_filler_enable
* \retval -1 \a encoder invalid
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

/**
* Get the h264_Ivop_quant_initial_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_Ivop_quant_initial_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_Ivop_quant_initial_value(SHCodecs_Encoder *
						   encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_Ivop_quant_initial_value;
}

/**
* Set the h264_Ivop_quant_initial_value.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_Ivop_quant_initial_value The new value for \a h264_Ivop_quant_initial_value
* \returns The previous value of \a h264_Ivop_quant_initial_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_Ivop_quant_initial_value(SHCodecs_Encoder *
						   encoder,
						   unsigned long
						   h264_Ivop_quant_initial_value)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_Ivop_quant_initial_value;
	encoder->other_options_h264.avcbe_Ivop_quant_initial_value =
	    h264_Ivop_quant_initial_value;

	return old_value;
}

/**
* Get the h264_Pvop_quant_initial_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_Pvop_quant_initial_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_Pvop_quant_initial_value(SHCodecs_Encoder *
						   encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_Pvop_quant_initial_value;
}

/**
* Set the h264_Pvop_quant_initial_value.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_Pvop_quant_initial_value The new value for \a h264_Pvop_quant_initial_value
* \returns The previous value of \a h264_Pvop_quant_initial_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_Pvop_quant_initial_value(SHCodecs_Encoder *
						   encoder,
						   unsigned long
						   h264_Pvop_quant_initial_value)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_Pvop_quant_initial_value;
	encoder->other_options_h264.avcbe_Pvop_quant_initial_value =
	    h264_Pvop_quant_initial_value;

	return old_value;
}

/**
* Get the h264_use_dquant.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_use_dquant
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_use_dquant(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_use_dquant;
}

/**
* Set the h264_use_dquant.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_use_dquant The new value for \a h264_use_dquant
* \returns The previous value of \a h264_use_dquant
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_use_dquant(SHCodecs_Encoder * encoder,
				     unsigned long h264_use_dquant)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_use_dquant;
	encoder->other_options_h264.avcbe_use_dquant = h264_use_dquant;

	return old_value;
}

/**
* Get the h264_clip_dquant_next_mb.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_clip_dquant_next_mb
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_clip_dquant_next_mb(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_clip_dquant_next_mb;
}

/**
* Set the h264_clip_dquant_next_mb.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_clip_dquant_next_mb The new value for \a h264_clip_dquant_next_mb
* \returns The previous value of \a h264_clip_dquant_next_mb
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_clip_dquant_next_mb(SHCodecs_Encoder * encoder,
					      unsigned long
					      h264_clip_dquant_next_mb)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_clip_dquant_next_mb;
	encoder->other_options_h264.avcbe_clip_dquant_next_mb =
	    h264_clip_dquant_next_mb;

	return old_value;
}

/**
* Get the h264_clip_dquant_frame.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_clip_dquant_frame
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_clip_dquant_frame(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_clip_dquant_frame;
}

/**
* Set the h264_clip_dquant_frame.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_clip_dquant_frame The new value for \a h264_clip_dquant_frame
* \returns The previous value of \a h264_clip_dquant_frame
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_clip_dquant_frame(SHCodecs_Encoder * encoder,
					    unsigned long
					    h264_clip_dquant_frame)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_clip_dquant_frame;
	encoder->other_options_h264.avcbe_clip_dquant_frame =
	    h264_clip_dquant_frame;

	return old_value;
}

/**
* Get the h264_quant_min.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_quant_min
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_quant_min(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_quant_min;
}

/**
* Set the h264_quant_min.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_quant_min The new value for \a h264_quant_min
* \returns The previous value of \a h264_quant_min
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_quant_min(SHCodecs_Encoder * encoder,
				    unsigned long h264_quant_min)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_quant_min;
	encoder->other_options_h264.avcbe_quant_min = h264_quant_min;

	return old_value;
}

/**
* Get the h264_quant_min_Ivop_under_range.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_quant_min_Ivop_under_range
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_quant_min_Ivop_under_range(SHCodecs_Encoder *
						     encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.
	    avcbe_quant_min_Ivop_under_range;
}

/**
* Set the h264_quant_min_Ivop_under_range.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_quant_min_Ivop_under_range The new value for \a h264_quant_min_Ivop_under_range
* \returns The previous value of \a h264_quant_min_Ivop_under_range
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_quant_min_Ivop_under_range(SHCodecs_Encoder *
						     encoder,
						     unsigned long
						     h264_quant_min_Ivop_under_range)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_quant_min_Ivop_under_range;
	encoder->other_options_h264.avcbe_quant_min_Ivop_under_range =
	    h264_quant_min_Ivop_under_range;

	return old_value;
}

/**
* Get the h264_quant_max.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_quant_max
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_quant_max(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_quant_max;
}

/**
* Set the h264_quant_max.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_quant_max The new value for \a h264_quant_max
* \returns The previous value of \a h264_quant_max
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_quant_max(SHCodecs_Encoder * encoder,
				    unsigned long h264_quant_max)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_quant_max;
	encoder->other_options_h264.avcbe_quant_max = h264_quant_max;

	return old_value;
}

/**
* Get the h264_ratecontrol_cpb_skipcheck_enable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_skipcheck_enable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_skipcheck_enable(SHCodecs_Encoder
							   * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.
	    avcbe_ratecontrol_cpb_skipcheck_enable;
}

/**
* Set the h264_ratecontrol_cpb_skipcheck_enable.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_ratecontrol_cpb_skipcheck_enable The new value for \a h264_ratecontrol_cpb_skipcheck_enable
* \returns The previous value of \a h264_ratecontrol_cpb_skipcheck_enable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_ratecontrol_cpb_skipcheck_enable(SHCodecs_Encoder
							   * encoder,
							   unsigned long
							   h264_ratecontrol_cpb_skipcheck_enable)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.
	    avcbe_ratecontrol_cpb_skipcheck_enable;
	encoder->other_options_h264.
	    avcbe_ratecontrol_cpb_skipcheck_enable =
	    h264_ratecontrol_cpb_skipcheck_enable;

	return old_value;
}

/**
* Get the h264_ratecontrol_cpb_Ivop_noskip.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_Ivop_noskip
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_Ivop_noskip(SHCodecs_Encoder *
						      encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.
	    avcbe_ratecontrol_cpb_Ivop_noskip;
}

/**
* Set the h264_ratecontrol_cpb_Ivop_noskip.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_ratecontrol_cpb_Ivop_noskip The new value for \a h264_ratecontrol_cpb_Ivop_noskip
* \returns The previous value of \a h264_ratecontrol_cpb_Ivop_noskip
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_ratecontrol_cpb_Ivop_noskip(SHCodecs_Encoder *
						      encoder,
						      unsigned long
						      h264_ratecontrol_cpb_Ivop_noskip)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_ratecontrol_cpb_Ivop_noskip;
	encoder->other_options_h264.avcbe_ratecontrol_cpb_Ivop_noskip =
	    h264_ratecontrol_cpb_Ivop_noskip;

	return old_value;
}

/**
* Get the h264_ratecontrol_cpb_remain_zero_skip_enable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_remain_zero_skip_enable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_remain_zero_skip_enable
(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.
	    avcbe_ratecontrol_cpb_remain_zero_skip_enable;
}

/**
* Set the h264_ratecontrol_cpb_remain_zero_skip_enable.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_ratecontrol_cpb_remain_zero_skip_enable The new value for \a h264_ratecontrol_cpb_remain_zero_skip_enable
* \returns The previous value of \a h264_ratecontrol_cpb_remain_zero_skip_enable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_ratecontrol_cpb_remain_zero_skip_enable
(SHCodecs_Encoder * encoder,
unsigned long h264_ratecontrol_cpb_remain_zero_skip_enable)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.
	    avcbe_ratecontrol_cpb_remain_zero_skip_enable;
	encoder->other_options_h264.
	    avcbe_ratecontrol_cpb_remain_zero_skip_enable =
	    h264_ratecontrol_cpb_remain_zero_skip_enable;

	return old_value;
}

/**
* Get the h264_ratecontrol_cpb_buffer_unit_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_buffer_unit_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_buffer_unit_size(SHCodecs_Encoder
							   * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.
	    avcbe_ratecontrol_cpb_buffer_unit_size;
}

/**
* Set the h264_ratecontrol_cpb_buffer_unit_size.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_ratecontrol_cpb_buffer_unit_size The new value for \a h264_ratecontrol_cpb_buffer_unit_size
* \returns The previous value of \a h264_ratecontrol_cpb_buffer_unit_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_ratecontrol_cpb_buffer_unit_size(SHCodecs_Encoder
							   * encoder,
							   unsigned long
							   h264_ratecontrol_cpb_buffer_unit_size)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.
	    avcbe_ratecontrol_cpb_buffer_unit_size;
	encoder->other_options_h264.
	    avcbe_ratecontrol_cpb_buffer_unit_size =
	    h264_ratecontrol_cpb_buffer_unit_size;

	return old_value;
}

/**
* Get the h264_ratecontrol_cpb_buffer_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_buffer_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_buffer_mode(SHCodecs_Encoder *
						      encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.
	    avcbe_ratecontrol_cpb_buffer_mode;
}

/**
* Set the h264_ratecontrol_cpb_buffer_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_ratecontrol_cpb_buffer_mode The new value for \a h264_ratecontrol_cpb_buffer_mode
* \returns The previous value of \a h264_ratecontrol_cpb_buffer_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_ratecontrol_cpb_buffer_mode(SHCodecs_Encoder *
						      encoder,
						      unsigned long
						      h264_ratecontrol_cpb_buffer_mode)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_ratecontrol_cpb_buffer_mode;
	encoder->other_options_h264.avcbe_ratecontrol_cpb_buffer_mode =
	    h264_ratecontrol_cpb_buffer_mode;

	return old_value;
}

/**
* Get the h264_ratecontrol_cpb_max_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_max_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_max_size(SHCodecs_Encoder *
						   encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_ratecontrol_cpb_max_size;
}

/**
* Set the h264_ratecontrol_cpb_max_size.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_ratecontrol_cpb_max_size The new value for \a h264_ratecontrol_cpb_max_size
* \returns The previous value of \a h264_ratecontrol_cpb_max_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_ratecontrol_cpb_max_size(SHCodecs_Encoder *
						   encoder,
						   unsigned long
						   h264_ratecontrol_cpb_max_size)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_ratecontrol_cpb_max_size;
	encoder->other_options_h264.avcbe_ratecontrol_cpb_max_size =
	    h264_ratecontrol_cpb_max_size;

	return old_value;
}

/**
* Get the h264_ratecontrol_cpb_offset.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_offset
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_offset(SHCodecs_Encoder *
						 encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_ratecontrol_cpb_offset;
}

/**
* Set the h264_ratecontrol_cpb_offset.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_ratecontrol_cpb_offset The new value for \a h264_ratecontrol_cpb_offset
* \returns The previous value of \a h264_ratecontrol_cpb_offset
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_ratecontrol_cpb_offset(SHCodecs_Encoder *
						 encoder,
						 unsigned long
						 h264_ratecontrol_cpb_offset)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_ratecontrol_cpb_offset;
	encoder->other_options_h264.avcbe_ratecontrol_cpb_offset =
	    h264_ratecontrol_cpb_offset;

	return old_value;
}

/**
* Get the h264_ratecontrol_cpb_offset_rate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_offset_rate
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_offset_rate(SHCodecs_Encoder *
						      encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.
	    avcbe_ratecontrol_cpb_offset_rate;
}

/**
* Set the h264_ratecontrol_cpb_offset_rate.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_ratecontrol_cpb_offset_rate The new value for \a h264_ratecontrol_cpb_offset_rate
* \returns The previous value of \a h264_ratecontrol_cpb_offset_rate
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_ratecontrol_cpb_offset_rate(SHCodecs_Encoder *
						      encoder,
						      unsigned long
						      h264_ratecontrol_cpb_offset_rate)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_ratecontrol_cpb_offset_rate;
	encoder->other_options_h264.avcbe_ratecontrol_cpb_offset_rate =
	    h264_ratecontrol_cpb_offset_rate;

	return old_value;
}

/**
* Get the h264_intra_thr_1.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_intra_thr_1
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_intra_thr_1(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_intra_thr_1;
}

/**
* Set the h264_intra_thr_1.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_intra_thr_1 The new value for \a h264_intra_thr_1
* \returns The previous value of \a h264_intra_thr_1
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_intra_thr_1(SHCodecs_Encoder * encoder,
				      unsigned long h264_intra_thr_1)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_intra_thr_1;
	encoder->other_options_h264.avcbe_intra_thr_1 = h264_intra_thr_1;

	return old_value;
}

/**
* Get the h264_intra_thr_2.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_intra_thr_2
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_intra_thr_2(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_intra_thr_2;
}

/**
* Set the h264_intra_thr_2.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_intra_thr_2 The new value for \a h264_intra_thr_2
* \returns The previous value of \a h264_intra_thr_2
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_intra_thr_2(SHCodecs_Encoder * encoder,
				      unsigned long h264_intra_thr_2)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_intra_thr_2;
	encoder->other_options_h264.avcbe_intra_thr_2 = h264_intra_thr_2;

	return old_value;
}

/**
* Get the h264_sad_intra_bias.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_sad_intra_bias
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_sad_intra_bias(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_sad_intra_bias;
}

/**
* Set the h264_sad_intra_bias.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_sad_intra_bias The new value for \a h264_sad_intra_bias
* \returns The previous value of \a h264_sad_intra_bias
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_sad_intra_bias(SHCodecs_Encoder * encoder,
					 unsigned long h264_sad_intra_bias)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_sad_intra_bias;
	encoder->other_options_h264.avcbe_sad_intra_bias =
	    h264_sad_intra_bias;

	return old_value;
}

/**
* Get the h264_regularly_inserted_I_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_regularly_inserted_I_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_regularly_inserted_I_type(SHCodecs_Encoder *
						    encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_regularly_inserted_I_type;
}

/**
* Set the h264_regularly_inserted_I_type.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_regularly_inserted_I_type The new value for \a h264_regularly_inserted_I_type
* \returns The previous value of \a h264_regularly_inserted_I_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_regularly_inserted_I_type(SHCodecs_Encoder *
						    encoder,
						    unsigned long
						    h264_regularly_inserted_I_type)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_regularly_inserted_I_type;
	encoder->other_options_h264.avcbe_regularly_inserted_I_type =
	    h264_regularly_inserted_I_type;

	return old_value;
}

/**
* Get the h264_call_unit.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_call_unit
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_call_unit(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_call_unit;
}

/**
* Set the h264_call_unit.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_call_unit The new value for \a h264_call_unit
* \returns The previous value of \a h264_call_unit
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_call_unit(SHCodecs_Encoder * encoder,
				    unsigned long h264_call_unit)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_call_unit;
	encoder->other_options_h264.avcbe_call_unit = h264_call_unit;

	return old_value;
}

/**
* Get the h264_use_slice.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_use_slice
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_use_slice(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_use_slice;
}

/**
* Set the h264_use_slice.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_use_slice The new value for \a h264_use_slice
* \returns The previous value of \a h264_use_slice
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_use_slice(SHCodecs_Encoder * encoder,
				    unsigned long h264_use_slice)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_use_slice;
	encoder->other_options_h264.avcbe_use_slice = h264_use_slice;

	return old_value;
}

/**
* Get the h264_slice_size_mb.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_slice_size_mb
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_slice_size_mb(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_slice_size_mb;
}

/**
* Set the h264_slice_size_mb.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_slice_size_mb The new value for \a h264_slice_size_mb
* \returns The previous value of \a h264_slice_size_mb
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_slice_size_mb(SHCodecs_Encoder * encoder,
					unsigned long h264_slice_size_mb)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_slice_size_mb;
	encoder->other_options_h264.avcbe_slice_size_mb =
	    h264_slice_size_mb;

	return old_value;
}

/**
* Get the h264_slice_size_bit.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_slice_size_bit
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_slice_size_bit(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_slice_size_bit;
}

/**
* Set the h264_slice_size_bit.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_slice_size_bit The new value for \a h264_slice_size_bit
* \returns The previous value of \a h264_slice_size_bit
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_slice_size_bit(SHCodecs_Encoder * encoder,
					 unsigned long h264_slice_size_bit)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_slice_size_bit;
	encoder->other_options_h264.avcbe_slice_size_bit =
	    h264_slice_size_bit;

	return old_value;
}

/**
* Get the h264_slice_type_value_pattern.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_slice_type_value_pattern
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_slice_type_value_pattern(SHCodecs_Encoder *
						   encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_slice_type_value_pattern;
}

/**
* Set the h264_slice_type_value_pattern.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_slice_type_value_pattern The new value for \a h264_slice_type_value_pattern
* \returns The previous value of \a h264_slice_type_value_pattern
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_slice_type_value_pattern(SHCodecs_Encoder *
						   encoder,
						   unsigned long
						   h264_slice_type_value_pattern)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_slice_type_value_pattern;
	encoder->other_options_h264.avcbe_slice_type_value_pattern =
	    h264_slice_type_value_pattern;

	return old_value;
}

/**
* Get the h264_use_mb_partition.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_use_mb_partition
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_use_mb_partition(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_use_mb_partition;
}

/**
* Set the h264_use_mb_partition.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_use_mb_partition The new value for \a h264_use_mb_partition
* \returns The previous value of \a h264_use_mb_partition
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_use_mb_partition(SHCodecs_Encoder * encoder,
					   unsigned long
					   h264_use_mb_partition)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_use_mb_partition;
	encoder->other_options_h264.avcbe_use_mb_partition =
	    h264_use_mb_partition;

	return old_value;
}

/**
* Get the h264_mb_partition_vector_thr.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_mb_partition_vector_thr
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_mb_partition_vector_thr(SHCodecs_Encoder *
						  encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_mb_partition_vector_thr;
}

/**
* Set the h264_mb_partition_vector_thr.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_mb_partition_vector_thr The new value for \a h264_mb_partition_vector_thr
* \returns The previous value of \a h264_mb_partition_vector_thr
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_mb_partition_vector_thr(SHCodecs_Encoder *
						  encoder,
						  unsigned long
						  h264_mb_partition_vector_thr)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_mb_partition_vector_thr;
	encoder->other_options_h264.avcbe_mb_partition_vector_thr =
	    h264_mb_partition_vector_thr;

	return old_value;
}

/**
* Get the h264_deblocking_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_deblocking_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_deblocking_mode(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_deblocking_mode;
}

/**
* Set the h264_deblocking_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_deblocking_mode The new value for \a h264_deblocking_mode
* \returns The previous value of \a h264_deblocking_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_deblocking_mode(SHCodecs_Encoder * encoder,
					  unsigned long
					  h264_deblocking_mode)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_deblocking_mode;
	encoder->other_options_h264.avcbe_deblocking_mode =
	    h264_deblocking_mode;

	return old_value;
}

/**
* Get the h264_use_deblocking_filter_control.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_use_deblocking_filter_control
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_use_deblocking_filter_control(SHCodecs_Encoder *
							encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.
	    avcbe_use_deblocking_filter_control;
}

/**
* Set the h264_use_deblocking_filter_control.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_use_deblocking_filter_control The new value for \a h264_use_deblocking_filter_control
* \returns The previous value of \a h264_use_deblocking_filter_control
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_use_deblocking_filter_control(SHCodecs_Encoder *
							encoder,
							unsigned long
							h264_use_deblocking_filter_control)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.
	    avcbe_use_deblocking_filter_control;
	encoder->other_options_h264.avcbe_use_deblocking_filter_control =
	    h264_use_deblocking_filter_control;

	return old_value;
}

/**
* Get the h264_deblocking_alpha_offset.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_deblocking_alpha_offset
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get_h264_deblocking_alpha_offset(SHCodecs_Encoder *
						  encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_deblocking_alpha_offset;
}

/**
* Set the h264_deblocking_alpha_offset.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_deblocking_alpha_offset The new value for \a h264_deblocking_alpha_offset
* \returns The previous value of \a h264_deblocking_alpha_offset
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_h264_deblocking_alpha_offset(SHCodecs_Encoder *
						  encoder,
						  long
						  h264_deblocking_alpha_offset)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_deblocking_alpha_offset;
	encoder->other_options_h264.avcbe_deblocking_alpha_offset =
	    h264_deblocking_alpha_offset;

	return old_value;
}

/**
* Get the h264_deblocking_beta_offset.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_deblocking_beta_offset
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get_h264_deblocking_beta_offset(SHCodecs_Encoder *
						 encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_deblocking_beta_offset;
}

/**
* Set the h264_deblocking_beta_offset.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_deblocking_beta_offset The new value for \a h264_deblocking_beta_offset
* \returns The previous value of \a h264_deblocking_beta_offset
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_h264_deblocking_beta_offset(SHCodecs_Encoder *
						 encoder,
						 long
						 h264_deblocking_beta_offset)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_deblocking_beta_offset;
	encoder->other_options_h264.avcbe_deblocking_beta_offset =
	    h264_deblocking_beta_offset;

	return old_value;
}

/**
* Get the h264_me_skip_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_me_skip_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_me_skip_mode(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_me_skip_mode;
}

/**
* Set the h264_me_skip_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_me_skip_mode The new value for \a h264_me_skip_mode
* \returns The previous value of \a h264_me_skip_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_me_skip_mode(SHCodecs_Encoder * encoder,
				       unsigned long h264_me_skip_mode)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_me_skip_mode;
	encoder->other_options_h264.avcbe_me_skip_mode = h264_me_skip_mode;

	return old_value;
}

/**
* Get the h264_put_start_code.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_put_start_code
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_put_start_code(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_put_start_code;
}

/**
* Set the h264_put_start_code.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_put_start_code The new value for \a h264_put_start_code
* \returns The previous value of \a h264_put_start_code
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_put_start_code(SHCodecs_Encoder * encoder,
					 unsigned long h264_put_start_code)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_put_start_code;
	encoder->other_options_h264.avcbe_put_start_code =
	    h264_put_start_code;

	return old_value;
}

/**
* Get the h264_param_changeable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_param_changeable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_param_changeable(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_param_changeable;
}

/**
* Set the h264_param_changeable.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_param_changeable The new value for \a h264_param_changeable
* \returns The previous value of \a h264_param_changeable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_param_changeable(SHCodecs_Encoder * encoder,
					   unsigned long
					   h264_param_changeable)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_param_changeable;
	encoder->other_options_h264.avcbe_param_changeable =
	    h264_param_changeable;

	return old_value;
}

/**
* Get the h264_changeable_max_bitrate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_changeable_max_bitrate
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_changeable_max_bitrate(SHCodecs_Encoder *
						 encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_changeable_max_bitrate;
}

/**
* Set the h264_changeable_max_bitrate.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_changeable_max_bitrate The new value for \a h264_changeable_max_bitrate
* \returns The previous value of \a h264_changeable_max_bitrate
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_changeable_max_bitrate(SHCodecs_Encoder *
						 encoder,
						 unsigned long
						 h264_changeable_max_bitrate)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_changeable_max_bitrate;
	encoder->other_options_h264.avcbe_changeable_max_bitrate =
	    h264_changeable_max_bitrate;

	return old_value;
}

/**
* Get the h264_seq_param_set_id.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_seq_param_set_id
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_seq_param_set_id(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_seq_param_set_id;
}

/**
* Set the h264_seq_param_set_id.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_seq_param_set_id The new value for \a h264_seq_param_set_id
* \returns The previous value of \a h264_seq_param_set_id
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_seq_param_set_id(SHCodecs_Encoder * encoder,
					   unsigned long
					   h264_seq_param_set_id)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_seq_param_set_id;
	encoder->other_options_h264.avcbe_seq_param_set_id =
	    h264_seq_param_set_id;

	return old_value;
}

/**
* Get the h264_profile.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_profile
* \retval -1 \a encoder invalid
*/
unsigned long shcodecs_encoder_get_h264_profile(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_profile;
}

/**
* Set the h264_profile.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_profile The new value for \a h264_profile
* \returns The previous value of \a h264_profile
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_profile(SHCodecs_Encoder * encoder,
				  unsigned long h264_profile)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_profile;
	encoder->other_options_h264.avcbe_profile = h264_profile;

	return old_value;
}

/**
* Get the h264_constraint_set_flag.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_constraint_set_flag
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_constraint_set_flag(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_constraint_set_flag;
}

/**
* Set the h264_constraint_set_flag.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_constraint_set_flag The new value for \a h264_constraint_set_flag
* \returns The previous value of \a h264_constraint_set_flag
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_constraint_set_flag(SHCodecs_Encoder * encoder,
					      unsigned long
					      h264_constraint_set_flag)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_constraint_set_flag;
	encoder->other_options_h264.avcbe_constraint_set_flag =
	    h264_constraint_set_flag;

	return old_value;
}

/**
* Get the h264_level_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_level_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_level_type(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_level_type;
}

/**
* Set the h264_level_type.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_level_type The new value for \a h264_level_type
* \returns The previous value of \a h264_level_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_level_type(SHCodecs_Encoder * encoder,
				     unsigned long h264_level_type)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_level_type;
	encoder->other_options_h264.avcbe_level_type = h264_level_type;

	return old_value;
}

/**
* Get the h264_level_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_level_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_level_value(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_level_value;
}

/**
* Set the h264_level_value.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_level_value The new value for \a h264_level_value
* \returns The previous value of \a h264_level_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_level_value(SHCodecs_Encoder * encoder,
				      unsigned long h264_level_value)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_level_value;
	encoder->other_options_h264.avcbe_level_value = h264_level_value;

	return old_value;
}

/**
* Get the h264_out_vui_parameters.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_out_vui_parameters
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_out_vui_parameters(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_out_vui_parameters;
}

/**
* Set the h264_out_vui_parameters.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_out_vui_parameters The new value for \a h264_out_vui_parameters
* \returns The previous value of \a h264_out_vui_parameters
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_out_vui_parameters(SHCodecs_Encoder * encoder,
					     unsigned long
					     h264_out_vui_parameters)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_h264.avcbe_out_vui_parameters;
	encoder->other_options_h264.avcbe_out_vui_parameters =
	    h264_out_vui_parameters;

	return old_value;
}

/**
* Get the h264_chroma_qp_index_offset.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_chroma_qp_index_offset
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get_h264_chroma_qp_index_offset(SHCodecs_Encoder *
						 encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_chroma_qp_index_offset;
}

/**
* Set the h264_chroma_qp_index_offset.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_chroma_qp_index_offset The new value for \a h264_chroma_qp_index_offset
* \returns The previous value of \a h264_chroma_qp_index_offset
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_h264_chroma_qp_index_offset(SHCodecs_Encoder *
						 encoder,
						 long
						 h264_chroma_qp_index_offset)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_chroma_qp_index_offset;
	encoder->other_options_h264.avcbe_chroma_qp_index_offset =
	    h264_chroma_qp_index_offset;

	return old_value;
}

/**
* Get the h264_constrained_intra_pred.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_constrained_intra_pred
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_constrained_intra_pred(SHCodecs_Encoder *
						 encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_h264.avcbe_constrained_intra_pred;
}

/**
* Set the h264_constrained_intra_pred.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_constrained_intra_pred The new value for \a h264_constrained_intra_pred
* \returns The previous value of \a h264_constrained_intra_pred
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_constrained_intra_pred(SHCodecs_Encoder *
						 encoder,
						 unsigned long
						 h264_constrained_intra_pred)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_h264.avcbe_constrained_intra_pred;
	encoder->other_options_h264.avcbe_constrained_intra_pred =
	    h264_constrained_intra_pred;

	return old_value;
}
