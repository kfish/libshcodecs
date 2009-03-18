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

/**
* Get the stream_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The stream_type
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_stream_type(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_stream_type;
}

/**
* Set the stream_type.
* \param encoder The SHCodecs_Encoder* handle
* \param stream_type The new value for \a stream_type
* \returns The previous value of \a stream_type
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_stream_type(SHCodecs_Encoder * encoder,
				 long stream_type)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_stream_type;
	encoder->encoding_property.avcbe_stream_type = stream_type;

	return old_value;
}

/**
* Get the bitrate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The bitrate
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_bitrate(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_bitrate;
}

/**
* Set the bitrate.
* \param encoder The SHCodecs_Encoder* handle
* \param bitrate The new value for \a bitrate
* \returns The previous value of \a bitrate
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_set_bitrate(SHCodecs_Encoder * encoder, long bitrate)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_bitrate;
	encoder->encoding_property.avcbe_bitrate = bitrate;

	return old_value;
}

/**
* Get the xpic_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The xpic_size
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_xpic_size(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_xpic_size;
}

/**
* Set the xpic_size.
* \param encoder The SHCodecs_Encoder* handle
* \param xpic_size The new value for \a xpic_size
* \returns The previous value of \a xpic_size
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_xpic_size(SHCodecs_Encoder * encoder, long xpic_size)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_xpic_size;
	encoder->encoding_property.avcbe_xpic_size = xpic_size;

	return old_value;
}

/**
* Get the ypic_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The ypic_size
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_ypic_size(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_ypic_size;
}

/**
* Set the ypic_size.
* \param encoder The SHCodecs_Encoder* handle
* \param ypic_size The new value for \a ypic_size
* \returns The previous value of \a ypic_size
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_ypic_size(SHCodecs_Encoder * encoder, long ypic_size)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_ypic_size;
	encoder->encoding_property.avcbe_ypic_size = ypic_size;

	return old_value;
}

/**
* Get the frame_rate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The frame_rate
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_frame_rate(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_frame_rate;
}

/**
* Set the frame_rate.
* \param encoder The SHCodecs_Encoder* handle
* \param frame_rate The new value for \a frame_rate
* \returns The previous value of \a frame_rate
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_frame_rate(SHCodecs_Encoder * encoder,
				long frame_rate)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_frame_rate;
	encoder->encoding_property.avcbe_frame_rate = frame_rate;

	return old_value;
}

/**
* Get the I_vop_interval.
* \param encoder The SHCodecs_Encoder* handle
* \returns The I_vop_interval
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_I_vop_interval(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_I_vop_interval;
}

/**
* Set the I_vop_interval.
* \param encoder The SHCodecs_Encoder* handle
* \param I_vop_interval The new value for \a I_vop_interval
* \returns The previous value of \a I_vop_interval
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_I_vop_interval(SHCodecs_Encoder * encoder,
				    long I_vop_interval)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_I_vop_interval;
	encoder->encoding_property.avcbe_I_vop_interval = I_vop_interval;

	return old_value;
}

/**
* Get the mv_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mv_mode
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_mv_mode(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_mv_mode;
}

/**
* Set the mv_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param mv_mode The new value for \a mv_mode
* \returns The previous value of \a mv_mode
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_set_mv_mode(SHCodecs_Encoder * encoder, long mv_mode)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_mv_mode;
	encoder->encoding_property.avcbe_mv_mode = mv_mode;

	return old_value;
}

/**
* Get the fcode_forward.
* \param encoder The SHCodecs_Encoder* handle
* \returns The fcode_forward
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_fcode_forward(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_fcode_forward;
}

/**
* Set the fcode_forward.
* \param encoder The SHCodecs_Encoder* handle
* \param fcode_forward The new value for \a fcode_forward
* \returns The previous value of \a fcode_forward
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_fcode_forward(SHCodecs_Encoder * encoder,
				   long fcode_forward)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_fcode_forward;
	encoder->encoding_property.avcbe_fcode_forward = fcode_forward;

	return old_value;
}

/**
* Get the search_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The search_mode
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_search_mode(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_search_mode;
}

/**
* Set the search_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param search_mode The new value for \a search_mode
* \returns The previous value of \a search_mode
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_search_mode(SHCodecs_Encoder * encoder,
				 long search_mode)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_search_mode;
	encoder->encoding_property.avcbe_search_mode = search_mode;

	return old_value;
}

/**
* Get the search_time_fixed.
* \param encoder The SHCodecs_Encoder* handle
* \returns The search_time_fixed
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_search_time_fixed(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_search_time_fixed;
}

/**
* Set the search_time_fixed.
* \param encoder The SHCodecs_Encoder* handle
* \param search_time_fixed The new value for \a search_time_fixed
* \returns The previous value of \a search_time_fixed
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_search_time_fixed(SHCodecs_Encoder * encoder,
				       long search_time_fixed)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_search_time_fixed;
	encoder->encoding_property.avcbe_search_time_fixed =
	    search_time_fixed;

	return old_value;
}

/**
* Get the ratecontrol_skip_enable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The ratecontrol_skip_enable
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get_ratecontrol_skip_enable(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_ratecontrol_skip_enable;
}

/**
* Set the ratecontrol_skip_enable.
* \param encoder The SHCodecs_Encoder* handle
* \param ratecontrol_skip_enable The new value for \a ratecontrol_skip_enable
* \returns The previous value of \a ratecontrol_skip_enable
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_ratecontrol_skip_enable(SHCodecs_Encoder * encoder,
					     long ratecontrol_skip_enable)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->encoding_property.avcbe_ratecontrol_skip_enable;
	encoder->encoding_property.avcbe_ratecontrol_skip_enable =
	    ratecontrol_skip_enable;

	return old_value;
}

/**
* Get the ratecontrol_use_prevquant.
* \param encoder The SHCodecs_Encoder* handle
* \returns The ratecontrol_use_prevquant
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get_ratecontrol_use_prevquant(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_ratecontrol_use_prevquant;
}

/**
* Set the ratecontrol_use_prevquant.
* \param encoder The SHCodecs_Encoder* handle
* \param ratecontrol_use_prevquant The new value for \a ratecontrol_use_prevquant
* \returns The previous value of \a ratecontrol_use_prevquant
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_ratecontrol_use_prevquant(SHCodecs_Encoder * encoder,
					       long
					       ratecontrol_use_prevquant)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->encoding_property.avcbe_ratecontrol_use_prevquant;
	encoder->encoding_property.avcbe_ratecontrol_use_prevquant =
	    ratecontrol_use_prevquant;

	return old_value;
}

/**
* Get the ratecontrol_respect_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The ratecontrol_respect_type
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get_ratecontrol_respect_type(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_ratecontrol_respect_type;
}

/**
* Set the ratecontrol_respect_type.
* \param encoder The SHCodecs_Encoder* handle
* \param ratecontrol_respect_type The new value for \a ratecontrol_respect_type
* \returns The previous value of \a ratecontrol_respect_type
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_ratecontrol_respect_type(SHCodecs_Encoder * encoder,
					      long
					      ratecontrol_respect_type)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->encoding_property.avcbe_ratecontrol_respect_type;
	encoder->encoding_property.avcbe_ratecontrol_respect_type =
	    ratecontrol_respect_type;

	return old_value;
}

/**
* Get the ratecontrol_intra_thr_changeable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The ratecontrol_intra_thr_changeable
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get_ratecontrol_intra_thr_changeable(SHCodecs_Encoder *
						      encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.
	    avcbe_ratecontrol_intra_thr_changeable;
}

/**
* Set the ratecontrol_intra_thr_changeable.
* \param encoder The SHCodecs_Encoder* handle
* \param ratecontrol_intra_thr_changeable The new value for \a ratecontrol_intra_thr_changeable
* \returns The previous value of \a ratecontrol_intra_thr_changeable
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_ratecontrol_intra_thr_changeable(SHCodecs_Encoder *
						      encoder,
						      long
						      ratecontrol_intra_thr_changeable)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->encoding_property.
	    avcbe_ratecontrol_intra_thr_changeable;
	encoder->encoding_property.avcbe_ratecontrol_intra_thr_changeable =
	    ratecontrol_intra_thr_changeable;

	return old_value;
}

/**
* Get the control_bitrate_length.
* \param encoder The SHCodecs_Encoder* handle
* \returns The control_bitrate_length
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get_control_bitrate_length(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_control_bitrate_length;
}

/**
* Set the control_bitrate_length.
* \param encoder The SHCodecs_Encoder* handle
* \param control_bitrate_length The new value for \a control_bitrate_length
* \returns The previous value of \a control_bitrate_length
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_control_bitrate_length(SHCodecs_Encoder * encoder,
					    long control_bitrate_length)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->encoding_property.avcbe_control_bitrate_length;
	encoder->encoding_property.avcbe_control_bitrate_length =
	    control_bitrate_length;

	return old_value;
}

/**
* Get the intra_macroblock_refresh_cycle.
* \param encoder The SHCodecs_Encoder* handle
* \returns The intra_macroblock_refresh_cycle
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get_intra_macroblock_refresh_cycle(SHCodecs_Encoder *
						    encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.
	    avcbe_intra_macroblock_refresh_cycle;
}

/**
* Set the intra_macroblock_refresh_cycle.
* \param encoder The SHCodecs_Encoder* handle
* \param intra_macroblock_refresh_cycle The new value for \a intra_macroblock_refresh_cycle
* \returns The previous value of \a intra_macroblock_refresh_cycle
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_intra_macroblock_refresh_cycle(SHCodecs_Encoder *
						    encoder,
						    long
						    intra_macroblock_refresh_cycle)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->encoding_property.
	    avcbe_intra_macroblock_refresh_cycle;
	encoder->encoding_property.avcbe_intra_macroblock_refresh_cycle =
	    intra_macroblock_refresh_cycle;

	return old_value;
}

/**
* Get the video_format.
* \param encoder The SHCodecs_Encoder* handle
* \returns The video_format
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_video_format(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_video_format;
}

/**
* Set the video_format.
* \param encoder The SHCodecs_Encoder* handle
* \param video_format The new value for \a video_format
* \returns The previous value of \a video_format
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_video_format(SHCodecs_Encoder * encoder,
				  long video_format)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_video_format;
	encoder->encoding_property.avcbe_video_format = video_format;

	return old_value;
}

/**
* Get the frame_num_resolution.
* \param encoder The SHCodecs_Encoder* handle
* \returns The frame_num_resolution
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_frame_num_resolution(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_frame_num_resolution;
}

/**
* Set the frame_num_resolution.
* \param encoder The SHCodecs_Encoder* handle
* \param frame_num_resolution The new value for \a frame_num_resolution
* \returns The previous value of \a frame_num_resolution
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_frame_num_resolution(SHCodecs_Encoder * encoder,
					  long frame_num_resolution)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_frame_num_resolution;
	encoder->encoding_property.avcbe_frame_num_resolution =
	    frame_num_resolution;

	return old_value;
}

/**
* Get the noise_reduction.
* \param encoder The SHCodecs_Encoder* handle
* \returns The noise_reduction
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_noise_reduction(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_noise_reduction;
}

/**
* Set the noise_reduction.
* \param encoder The SHCodecs_Encoder* handle
* \param noise_reduction The new value for \a noise_reduction
* \returns The previous value of \a noise_reduction
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_noise_reduction(SHCodecs_Encoder * encoder,
				     long noise_reduction)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_noise_reduction;
	encoder->encoding_property.avcbe_noise_reduction = noise_reduction;

	return old_value;
}

/**
* Get the reaction_param_coeff.
* \param encoder The SHCodecs_Encoder* handle
* \returns The reaction_param_coeff
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_reaction_param_coeff(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_reaction_param_coeff;
}

/**
* Set the reaction_param_coeff.
* \param encoder The SHCodecs_Encoder* handle
* \param reaction_param_coeff The new value for \a reaction_param_coeff
* \returns The previous value of \a reaction_param_coeff
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_reaction_param_coeff(SHCodecs_Encoder * encoder,
					  long reaction_param_coeff)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_reaction_param_coeff;
	encoder->encoding_property.avcbe_reaction_param_coeff =
	    reaction_param_coeff;

	return old_value;
}

/**
* Get the weightedQ_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The weightedQ_mode
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get_weightedQ_mode(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_weightedQ_mode;
}

/**
* Set the weightedQ_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param weightedQ_mode The new value for \a weightedQ_mode
* \returns The previous value of \a weightedQ_mode
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set_weightedQ_mode(SHCodecs_Encoder * encoder,
				    long weightedQ_mode)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_weightedQ_mode;
	encoder->encoding_property.avcbe_weightedQ_mode = weightedQ_mode;

	return old_value;
}
