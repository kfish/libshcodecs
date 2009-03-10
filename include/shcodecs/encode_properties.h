#ifndef __SHCODECS_ENCODE_PROPERTIES_H__
#define __SHCODECS_ENCODE_PROPERTIES_H__

/** \file
 *
 * Accessors for encoder property parameters
 */

/**
 * Get the stream_type.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The stream_type
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_stream_type(SHCodecs_Encoder * encoder);

/**
 * Set the stream_type.
 * \param encoder The SHCodecs_Encoder* handle
 * \param stream_type The new value for \a stream_type
 * \returns The previous value of \a stream_type
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_stream_type(SHCodecs_Encoder * encoder,
				 long stream_type);

/**
 * Get the bitrate.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The bitrate
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_bitrate(SHCodecs_Encoder * encoder);

/**
 * Set the bitrate.
 * \param encoder The SHCodecs_Encoder* handle
 * \param bitrate The new value for \a bitrate
 * \returns The previous value of \a bitrate
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_bitrate(SHCodecs_Encoder * encoder, long bitrate);

/**
 * Get the xpic_size.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The xpic_size
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_xpic_size(SHCodecs_Encoder * encoder);

/**
 * Set the xpic_size.
 * \param encoder The SHCodecs_Encoder* handle
 * \param xpic_size The new value for \a xpic_size
 * \returns The previous value of \a xpic_size
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_xpic_size(SHCodecs_Encoder * encoder, long xpic_size);

/**
 * Get the ypic_size.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The ypic_size
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_ypic_size(SHCodecs_Encoder * encoder);

/**
 * Set the ypic_size.
 * \param encoder The SHCodecs_Encoder* handle
 * \param ypic_size The new value for \a ypic_size
 * \returns The previous value of \a ypic_size
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_ypic_size(SHCodecs_Encoder * encoder, long ypic_size);

/**
 * Get the frame_rate.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The frame_rate
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_frame_rate(SHCodecs_Encoder * encoder);

/**
 * Set the frame_rate.
 * \param encoder The SHCodecs_Encoder* handle
 * \param frame_rate The new value for \a frame_rate
 * \returns The previous value of \a frame_rate
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_frame_rate(SHCodecs_Encoder * encoder,
				long frame_rate);

/**
 * Get the I_vop_interval.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The I_vop_interval
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_I_vop_interval(SHCodecs_Encoder * encoder);

/**
 * Set the I_vop_interval.
 * \param encoder The SHCodecs_Encoder* handle
 * \param I_vop_interval The new value for \a I_vop_interval
 * \returns The previous value of \a I_vop_interval
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_I_vop_interval(SHCodecs_Encoder * encoder,
				    long I_vop_interval);

/**
 * Get the mv_mode.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The mv_mode
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_mv_mode(SHCodecs_Encoder * encoder);

/**
 * Set the mv_mode.
 * \param encoder The SHCodecs_Encoder* handle
 * \param mv_mode The new value for \a mv_mode
 * \returns The previous value of \a mv_mode
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_mv_mode(SHCodecs_Encoder * encoder, long mv_mode);

/**
 * Get the fcode_forward.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The fcode_forward
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_fcode_forward(SHCodecs_Encoder * encoder);

/**
 * Set the fcode_forward.
 * \param encoder The SHCodecs_Encoder* handle
 * \param fcode_forward The new value for \a fcode_forward
 * \returns The previous value of \a fcode_forward
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_fcode_forward(SHCodecs_Encoder * encoder,
				   long fcode_forward);

/**
 * Get the search_mode.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The search_mode
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_search_mode(SHCodecs_Encoder * encoder);

/**
 * Set the search_mode.
 * \param encoder The SHCodecs_Encoder* handle
 * \param search_mode The new value for \a search_mode
 * \returns The previous value of \a search_mode
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_search_mode(SHCodecs_Encoder * encoder,
				 long search_mode);

/**
 * Get the search_time_fixed.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The search_time_fixed
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_search_time_fixed(SHCodecs_Encoder * encoder);

/**
 * Set the search_time_fixed.
 * \param encoder The SHCodecs_Encoder* handle
 * \param search_time_fixed The new value for \a search_time_fixed
 * \returns The previous value of \a search_time_fixed
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_search_time_fixed(SHCodecs_Encoder * encoder,
				       long search_time_fixed);

/**
 * Get the ratecontrol_skip_enable.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The ratecontrol_skip_enable
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_get_ratecontrol_skip_enable(SHCodecs_Encoder * encoder);

/**
 * Set the ratecontrol_skip_enable.
 * \param encoder The SHCodecs_Encoder* handle
 * \param ratecontrol_skip_enable The new value for \a ratecontrol_skip_enable
 * \returns The previous value of \a ratecontrol_skip_enable
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_ratecontrol_skip_enable(SHCodecs_Encoder * encoder,
					     long ratecontrol_skip_enable);

/**
 * Get the ratecontrol_use_prevquant.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The ratecontrol_use_prevquant
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_get_ratecontrol_use_prevquant(SHCodecs_Encoder * encoder);

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
					       ratecontrol_use_prevquant);

/**
 * Get the ratecontrol_respect_type.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The ratecontrol_respect_type
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_get_ratecontrol_respect_type(SHCodecs_Encoder * encoder);

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
					      ratecontrol_respect_type);

/**
 * Get the ratecontrol_intra_thr_changeable.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The ratecontrol_intra_thr_changeable
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_get_ratecontrol_intra_thr_changeable(SHCodecs_Encoder *
						      encoder);

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
						      ratecontrol_intra_thr_changeable);

/**
 * Get the control_bitrate_length.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The control_bitrate_length
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_get_control_bitrate_length(SHCodecs_Encoder * encoder);

/**
 * Set the control_bitrate_length.
 * \param encoder The SHCodecs_Encoder* handle
 * \param control_bitrate_length The new value for \a control_bitrate_length
 * \returns The previous value of \a control_bitrate_length
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_control_bitrate_length(SHCodecs_Encoder * encoder,
					    long control_bitrate_length);

/**
 * Get the intra_macroblock_refresh_cycle.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The intra_macroblock_refresh_cycle
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_get_intra_macroblock_refresh_cycle(SHCodecs_Encoder *
						    encoder);

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
						    intra_macroblock_refresh_cycle);

/**
 * Get the video_format.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The video_format
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_video_format(SHCodecs_Encoder * encoder);

/**
 * Set the video_format.
 * \param encoder The SHCodecs_Encoder* handle
 * \param video_format The new value for \a video_format
 * \returns The previous value of \a video_format
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_video_format(SHCodecs_Encoder * encoder,
				  long video_format);

/**
 * Get the frame_num_resolution.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The frame_num_resolution
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_frame_num_resolution(SHCodecs_Encoder * encoder);

/**
 * Set the frame_num_resolution.
 * \param encoder The SHCodecs_Encoder* handle
 * \param frame_num_resolution The new value for \a frame_num_resolution
 * \returns The previous value of \a frame_num_resolution
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_frame_num_resolution(SHCodecs_Encoder * encoder,
					  long frame_num_resolution);

/**
 * Get the noise_reduction.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The noise_reduction
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_noise_reduction(SHCodecs_Encoder * encoder);

/**
 * Set the noise_reduction.
 * \param encoder The SHCodecs_Encoder* handle
 * \param noise_reduction The new value for \a noise_reduction
 * \returns The previous value of \a noise_reduction
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_noise_reduction(SHCodecs_Encoder * encoder,
				     long noise_reduction);

/**
 * Get the reaction_param_coeff.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The reaction_param_coeff
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_reaction_param_coeff(SHCodecs_Encoder * encoder);

/**
 * Set the reaction_param_coeff.
 * \param encoder The SHCodecs_Encoder* handle
 * \param reaction_param_coeff The new value for \a reaction_param_coeff
 * \returns The previous value of \a reaction_param_coeff
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_reaction_param_coeff(SHCodecs_Encoder * encoder,
					  long reaction_param_coeff);

/**
 * Get the weightedQ_mode.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The weightedQ_mode
 * \retval -1 \a encoder invalid
 */
long shcodecs_encoder_get_weightedQ_mode(SHCodecs_Encoder * encoder);

/**
 * Set the weightedQ_mode.
 * \param encoder The SHCodecs_Encoder* handle
 * \param weightedQ_mode The new value for \a weightedQ_mode
 * \returns The previous value of \a weightedQ_mode
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_weightedQ_mode(SHCodecs_Encoder * encoder,
				    long weightedQ_mode);


#endif				/* __SHCODECS_ENCODE_PROPERTIES_H__ */
