#include <stdlib.h>

#include "encoder_private.h"

/**
* Get the _stream_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _stream_type
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__stream_type(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_stream_type;
}

/**
* Set the _stream_type.
* \param encoder The SHCodecs_Encoder* handle
* \param _stream_type The new value for \a _stream_type
* \returns The previous value of \a _stream_type
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__stream_type(SHCodecs_Encoder * encoder,
				  long _stream_type)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_stream_type;
	encoder->encoding_property.avcbe_stream_type = _stream_type;

	return old_value;
}

/**
* Get the _bitrate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _bitrate
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__bitrate(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_bitrate;
}

/**
* Set the _bitrate.
* \param encoder The SHCodecs_Encoder* handle
* \param _bitrate The new value for \a _bitrate
* \returns The previous value of \a _bitrate
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__bitrate(SHCodecs_Encoder * encoder, long _bitrate)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_bitrate;
	encoder->encoding_property.avcbe_bitrate = _bitrate;

	return old_value;
}

/**
* Get the _xpic_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _xpic_size
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__xpic_size(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_xpic_size;
}

/**
* Set the _xpic_size.
* \param encoder The SHCodecs_Encoder* handle
* \param _xpic_size The new value for \a _xpic_size
* \returns The previous value of \a _xpic_size
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__xpic_size(SHCodecs_Encoder * encoder,
				long _xpic_size)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_xpic_size;
	encoder->encoding_property.avcbe_xpic_size = _xpic_size;

	return old_value;
}

/**
* Get the _ypic_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _ypic_size
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__ypic_size(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_ypic_size;
}

/**
* Set the _ypic_size.
* \param encoder The SHCodecs_Encoder* handle
* \param _ypic_size The new value for \a _ypic_size
* \returns The previous value of \a _ypic_size
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__ypic_size(SHCodecs_Encoder * encoder,
				long _ypic_size)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_ypic_size;
	encoder->encoding_property.avcbe_ypic_size = _ypic_size;

	return old_value;
}

/**
* Get the _frame_rate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _frame_rate
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__frame_rate(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_frame_rate;
}

/**
* Set the _frame_rate.
* \param encoder The SHCodecs_Encoder* handle
* \param _frame_rate The new value for \a _frame_rate
* \returns The previous value of \a _frame_rate
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__frame_rate(SHCodecs_Encoder * encoder,
				 long _frame_rate)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_frame_rate;
	encoder->encoding_property.avcbe_frame_rate = _frame_rate;

	return old_value;
}

/**
* Get the _I_vop_interval.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _I_vop_interval
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__I_vop_interval(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_I_vop_interval;
}

/**
* Set the _I_vop_interval.
* \param encoder The SHCodecs_Encoder* handle
* \param _I_vop_interval The new value for \a _I_vop_interval
* \returns The previous value of \a _I_vop_interval
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__I_vop_interval(SHCodecs_Encoder * encoder,
				     long _I_vop_interval)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_I_vop_interval;
	encoder->encoding_property.avcbe_I_vop_interval = _I_vop_interval;

	return old_value;
}

/**
* Get the _mv_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _mv_mode
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__mv_mode(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_mv_mode;
}

/**
* Set the _mv_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param _mv_mode The new value for \a _mv_mode
* \returns The previous value of \a _mv_mode
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__mv_mode(SHCodecs_Encoder * encoder, long _mv_mode)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_mv_mode;
	encoder->encoding_property.avcbe_mv_mode = _mv_mode;

	return old_value;
}

/**
* Get the _fcode_forward.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _fcode_forward
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__fcode_forward(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_fcode_forward;
}

/**
* Set the _fcode_forward.
* \param encoder The SHCodecs_Encoder* handle
* \param _fcode_forward The new value for \a _fcode_forward
* \returns The previous value of \a _fcode_forward
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__fcode_forward(SHCodecs_Encoder * encoder,
				    long _fcode_forward)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_fcode_forward;
	encoder->encoding_property.avcbe_fcode_forward = _fcode_forward;

	return old_value;
}

/**
* Get the _search_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _search_mode
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__search_mode(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_search_mode;
}

/**
* Set the _search_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param _search_mode The new value for \a _search_mode
* \returns The previous value of \a _search_mode
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__search_mode(SHCodecs_Encoder * encoder,
				  long _search_mode)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_search_mode;
	encoder->encoding_property.avcbe_search_mode = _search_mode;

	return old_value;
}

/**
* Get the _search_time_fixed.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _search_time_fixed
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__search_time_fixed(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_search_time_fixed;
}

/**
* Set the _search_time_fixed.
* \param encoder The SHCodecs_Encoder* handle
* \param _search_time_fixed The new value for \a _search_time_fixed
* \returns The previous value of \a _search_time_fixed
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__search_time_fixed(SHCodecs_Encoder * encoder,
					long _search_time_fixed)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_search_time_fixed;
	encoder->encoding_property.avcbe_search_time_fixed =
	    _search_time_fixed;

	return old_value;
}

/**
* Get the _ratecontrol_skip_enable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _ratecontrol_skip_enable
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__ratecontrol_skip_enable(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_ratecontrol_skip_enable;
}

/**
* Set the _ratecontrol_skip_enable.
* \param encoder The SHCodecs_Encoder* handle
* \param _ratecontrol_skip_enable The new value for \a _ratecontrol_skip_enable
* \returns The previous value of \a _ratecontrol_skip_enable
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__ratecontrol_skip_enable(SHCodecs_Encoder * encoder,
					      long
					      _ratecontrol_skip_enable)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->encoding_property.avcbe_ratecontrol_skip_enable;
	encoder->encoding_property.avcbe_ratecontrol_skip_enable =
	    _ratecontrol_skip_enable;

	return old_value;
}

/**
* Get the _ratecontrol_use_prevquant.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _ratecontrol_use_prevquant
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__ratecontrol_use_prevquant(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_ratecontrol_use_prevquant;
}

/**
* Set the _ratecontrol_use_prevquant.
* \param encoder The SHCodecs_Encoder* handle
* \param _ratecontrol_use_prevquant The new value for \a _ratecontrol_use_prevquant
* \returns The previous value of \a _ratecontrol_use_prevquant
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__ratecontrol_use_prevquant(SHCodecs_Encoder * encoder,
						long
						_ratecontrol_use_prevquant)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->encoding_property.avcbe_ratecontrol_use_prevquant;
	encoder->encoding_property.avcbe_ratecontrol_use_prevquant =
	    _ratecontrol_use_prevquant;

	return old_value;
}

/**
* Get the _ratecontrol_respect_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _ratecontrol_respect_type
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__ratecontrol_respect_type(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_ratecontrol_respect_type;
}

/**
* Set the _ratecontrol_respect_type.
* \param encoder The SHCodecs_Encoder* handle
* \param _ratecontrol_respect_type The new value for \a _ratecontrol_respect_type
* \returns The previous value of \a _ratecontrol_respect_type
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__ratecontrol_respect_type(SHCodecs_Encoder * encoder,
					       long
					       _ratecontrol_respect_type)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->encoding_property.avcbe_ratecontrol_respect_type;
	encoder->encoding_property.avcbe_ratecontrol_respect_type =
	    _ratecontrol_respect_type;

	return old_value;
}

/**
* Get the _ratecontrol_intra_thr_changeable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _ratecontrol_intra_thr_changeable
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__ratecontrol_intra_thr_changeable(SHCodecs_Encoder *
						       encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.
	    avcbe_ratecontrol_intra_thr_changeable;
}

/**
* Set the _ratecontrol_intra_thr_changeable.
* \param encoder The SHCodecs_Encoder* handle
* \param _ratecontrol_intra_thr_changeable The new value for \a _ratecontrol_intra_thr_changeable
* \returns The previous value of \a _ratecontrol_intra_thr_changeable
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__ratecontrol_intra_thr_changeable(SHCodecs_Encoder *
						       encoder,
						       long
						       _ratecontrol_intra_thr_changeable)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->encoding_property.
	    avcbe_ratecontrol_intra_thr_changeable;
	encoder->encoding_property.avcbe_ratecontrol_intra_thr_changeable =
	    _ratecontrol_intra_thr_changeable;

	return old_value;
}

/**
* Get the _control_bitrate_length.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _control_bitrate_length
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__control_bitrate_length(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_control_bitrate_length;
}

/**
* Set the _control_bitrate_length.
* \param encoder The SHCodecs_Encoder* handle
* \param _control_bitrate_length The new value for \a _control_bitrate_length
* \returns The previous value of \a _control_bitrate_length
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__control_bitrate_length(SHCodecs_Encoder * encoder,
					     long _control_bitrate_length)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->encoding_property.avcbe_control_bitrate_length;
	encoder->encoding_property.avcbe_control_bitrate_length =
	    _control_bitrate_length;

	return old_value;
}

/**
* Get the _intra_macroblock_refresh_cycle.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _intra_macroblock_refresh_cycle
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__intra_macroblock_refresh_cycle(SHCodecs_Encoder *
						     encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.
	    avcbe_intra_macroblock_refresh_cycle;
}

/**
* Set the _intra_macroblock_refresh_cycle.
* \param encoder The SHCodecs_Encoder* handle
* \param _intra_macroblock_refresh_cycle The new value for \a _intra_macroblock_refresh_cycle
* \returns The previous value of \a _intra_macroblock_refresh_cycle
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__intra_macroblock_refresh_cycle(SHCodecs_Encoder *
						     encoder,
						     long
						     _intra_macroblock_refresh_cycle)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->encoding_property.
	    avcbe_intra_macroblock_refresh_cycle;
	encoder->encoding_property.avcbe_intra_macroblock_refresh_cycle =
	    _intra_macroblock_refresh_cycle;

	return old_value;
}

/**
* Get the _video_format.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _video_format
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__video_format(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_video_format;
}

/**
* Set the _video_format.
* \param encoder The SHCodecs_Encoder* handle
* \param _video_format The new value for \a _video_format
* \returns The previous value of \a _video_format
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__video_format(SHCodecs_Encoder * encoder,
				   long _video_format)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_video_format;
	encoder->encoding_property.avcbe_video_format = _video_format;

	return old_value;
}

/**
* Get the _frame_num_resolution.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _frame_num_resolution
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__frame_num_resolution(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_frame_num_resolution;
}

/**
* Set the _frame_num_resolution.
* \param encoder The SHCodecs_Encoder* handle
* \param _frame_num_resolution The new value for \a _frame_num_resolution
* \returns The previous value of \a _frame_num_resolution
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__frame_num_resolution(SHCodecs_Encoder * encoder,
					   long _frame_num_resolution)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_frame_num_resolution;
	encoder->encoding_property.avcbe_frame_num_resolution =
	    _frame_num_resolution;

	return old_value;
}

/**
* Get the _noise_reduction.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _noise_reduction
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__noise_reduction(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_noise_reduction;
}

/**
* Set the _noise_reduction.
* \param encoder The SHCodecs_Encoder* handle
* \param _noise_reduction The new value for \a _noise_reduction
* \returns The previous value of \a _noise_reduction
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__noise_reduction(SHCodecs_Encoder * encoder,
				      long _noise_reduction)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_noise_reduction;
	encoder->encoding_property.avcbe_noise_reduction =
	    _noise_reduction;

	return old_value;
}

/**
* Get the _reaction_param_coeff.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _reaction_param_coeff
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__reaction_param_coeff(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_reaction_param_coeff;
}

/**
* Set the _reaction_param_coeff.
* \param encoder The SHCodecs_Encoder* handle
* \param _reaction_param_coeff The new value for \a _reaction_param_coeff
* \returns The previous value of \a _reaction_param_coeff
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__reaction_param_coeff(SHCodecs_Encoder * encoder,
					   long _reaction_param_coeff)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_reaction_param_coeff;
	encoder->encoding_property.avcbe_reaction_param_coeff =
	    _reaction_param_coeff;

	return old_value;
}

/**
* Get the _weightedQ_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _weightedQ_mode
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__weightedQ_mode(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->encoding_property.avcbe_weightedQ_mode;
}

/**
* Set the _weightedQ_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param _weightedQ_mode The new value for \a _weightedQ_mode
* \returns The previous value of \a _weightedQ_mode
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__weightedQ_mode(SHCodecs_Encoder * encoder,
				     long _weightedQ_mode)
{
	long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->encoding_property.avcbe_weightedQ_mode;
	encoder->encoding_property.avcbe_weightedQ_mode = _weightedQ_mode;

	return old_value;
}
