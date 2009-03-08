#ifndef __SHCODECS_ENCODE_PROPERTIES_H__
#define __SHCODECS_ENCODE_PROPERTIES_H__

/**
* Get the _stream_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _stream_type
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__stream_type(SHCodecs_Encoder * encoder);

/**
* Set the _stream_type.
* \param encoder The SHCodecs_Encoder* handle
* \param _stream_type The new value for \a _stream_type
* \returns The previous value of \a _stream_type
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__stream_type(SHCodecs_Encoder * encoder,
				  long _stream_type);

/**
* Get the _bitrate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _bitrate
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__bitrate(SHCodecs_Encoder * encoder);

/**
* Set the _bitrate.
* \param encoder The SHCodecs_Encoder* handle
* \param _bitrate The new value for \a _bitrate
* \returns The previous value of \a _bitrate
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__bitrate(SHCodecs_Encoder * encoder, long _bitrate);

/**
* Get the _xpic_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _xpic_size
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__xpic_size(SHCodecs_Encoder * encoder);

/**
* Set the _xpic_size.
* \param encoder The SHCodecs_Encoder* handle
* \param _xpic_size The new value for \a _xpic_size
* \returns The previous value of \a _xpic_size
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__xpic_size(SHCodecs_Encoder * encoder,
				long _xpic_size);

/**
* Get the _ypic_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _ypic_size
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__ypic_size(SHCodecs_Encoder * encoder);

/**
* Set the _ypic_size.
* \param encoder The SHCodecs_Encoder* handle
* \param _ypic_size The new value for \a _ypic_size
* \returns The previous value of \a _ypic_size
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__ypic_size(SHCodecs_Encoder * encoder,
				long _ypic_size);

/**
* Get the _frame_rate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _frame_rate
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__frame_rate(SHCodecs_Encoder * encoder);

/**
* Set the _frame_rate.
* \param encoder The SHCodecs_Encoder* handle
* \param _frame_rate The new value for \a _frame_rate
* \returns The previous value of \a _frame_rate
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__frame_rate(SHCodecs_Encoder * encoder,
				 long _frame_rate);

/**
* Get the _I_vop_interval.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _I_vop_interval
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__I_vop_interval(SHCodecs_Encoder * encoder);

/**
* Set the _I_vop_interval.
* \param encoder The SHCodecs_Encoder* handle
* \param _I_vop_interval The new value for \a _I_vop_interval
* \returns The previous value of \a _I_vop_interval
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__I_vop_interval(SHCodecs_Encoder * encoder,
				     long _I_vop_interval);

/**
* Get the _mv_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _mv_mode
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__mv_mode(SHCodecs_Encoder * encoder);

/**
* Set the _mv_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param _mv_mode The new value for \a _mv_mode
* \returns The previous value of \a _mv_mode
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__mv_mode(SHCodecs_Encoder * encoder, long _mv_mode);

/**
* Get the _fcode_forward.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _fcode_forward
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__fcode_forward(SHCodecs_Encoder * encoder);

/**
* Set the _fcode_forward.
* \param encoder The SHCodecs_Encoder* handle
* \param _fcode_forward The new value for \a _fcode_forward
* \returns The previous value of \a _fcode_forward
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__fcode_forward(SHCodecs_Encoder * encoder,
				    long _fcode_forward);

/**
* Get the _search_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _search_mode
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__search_mode(SHCodecs_Encoder * encoder);

/**
* Set the _search_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param _search_mode The new value for \a _search_mode
* \returns The previous value of \a _search_mode
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__search_mode(SHCodecs_Encoder * encoder,
				  long _search_mode);

/**
* Get the _search_time_fixed.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _search_time_fixed
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__search_time_fixed(SHCodecs_Encoder * encoder);

/**
* Set the _search_time_fixed.
* \param encoder The SHCodecs_Encoder* handle
* \param _search_time_fixed The new value for \a _search_time_fixed
* \returns The previous value of \a _search_time_fixed
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__search_time_fixed(SHCodecs_Encoder * encoder,
					long _search_time_fixed);

/**
* Get the _ratecontrol_skip_enable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _ratecontrol_skip_enable
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__ratecontrol_skip_enable(SHCodecs_Encoder * encoder);

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
					      _ratecontrol_skip_enable);

/**
* Get the _ratecontrol_use_prevquant.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _ratecontrol_use_prevquant
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__ratecontrol_use_prevquant(SHCodecs_Encoder *
						encoder);

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
						_ratecontrol_use_prevquant);

/**
* Get the _ratecontrol_respect_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _ratecontrol_respect_type
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__ratecontrol_respect_type(SHCodecs_Encoder * encoder);

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
					       _ratecontrol_respect_type);

/**
* Get the _ratecontrol_intra_thr_changeable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _ratecontrol_intra_thr_changeable
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__ratecontrol_intra_thr_changeable(SHCodecs_Encoder *
						       encoder);

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
						       _ratecontrol_intra_thr_changeable);

/**
* Get the _control_bitrate_length.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _control_bitrate_length
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__control_bitrate_length(SHCodecs_Encoder * encoder);

/**
* Set the _control_bitrate_length.
* \param encoder The SHCodecs_Encoder* handle
* \param _control_bitrate_length The new value for \a _control_bitrate_length
* \returns The previous value of \a _control_bitrate_length
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__control_bitrate_length(SHCodecs_Encoder * encoder,
					     long _control_bitrate_length);

/**
* Get the _intra_macroblock_refresh_cycle.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _intra_macroblock_refresh_cycle
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__intra_macroblock_refresh_cycle(SHCodecs_Encoder *
						     encoder);

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
						     _intra_macroblock_refresh_cycle);

/**
* Get the _video_format.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _video_format
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__video_format(SHCodecs_Encoder * encoder);

/**
* Set the _video_format.
* \param encoder The SHCodecs_Encoder* handle
* \param _video_format The new value for \a _video_format
* \returns The previous value of \a _video_format
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__video_format(SHCodecs_Encoder * encoder,
				   long _video_format);

/**
* Get the _frame_num_resolution.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _frame_num_resolution
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__frame_num_resolution(SHCodecs_Encoder * encoder);

/**
* Set the _frame_num_resolution.
* \param encoder The SHCodecs_Encoder* handle
* \param _frame_num_resolution The new value for \a _frame_num_resolution
* \returns The previous value of \a _frame_num_resolution
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__frame_num_resolution(SHCodecs_Encoder * encoder,
					   long _frame_num_resolution);

/**
* Get the _noise_reduction.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _noise_reduction
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__noise_reduction(SHCodecs_Encoder * encoder);

/**
* Set the _noise_reduction.
* \param encoder The SHCodecs_Encoder* handle
* \param _noise_reduction The new value for \a _noise_reduction
* \returns The previous value of \a _noise_reduction
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__noise_reduction(SHCodecs_Encoder * encoder,
				      long _noise_reduction);

/**
* Get the _reaction_param_coeff.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _reaction_param_coeff
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get__reaction_param_coeff(SHCodecs_Encoder * encoder);

/**
* Set the _reaction_param_coeff.
* \param encoder The SHCodecs_Encoder* handle
* \param _reaction_param_coeff The new value for \a _reaction_param_coeff
* \returns The previous value of \a _reaction_param_coeff
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__reaction_param_coeff(SHCodecs_Encoder * encoder,
					   long _reaction_param_coeff);

/**
* Get the _weightedQ_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The _weightedQ_mode
* \retval -1 \a encoder invalid
*/
long shcodecs_encoder_get__weightedQ_mode(SHCodecs_Encoder * encoder);

/**
* Set the _weightedQ_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param _weightedQ_mode The new value for \a _weightedQ_mode
* \returns The previous value of \a _weightedQ_mode
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_set__weightedQ_mode(SHCodecs_Encoder * encoder,
				     long _weightedQ_mode);


#endif				/* __SHCODECS_ENCODE_PROPERTIES_H__ */
