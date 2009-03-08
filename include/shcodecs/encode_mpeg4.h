#ifndef __SHCODECS_ENCODE_MPEG4_H__
#define __SHCODECS_ENCODE_MPEG4_H__

/**
* Get the mpeg4_out_vos.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_out_vos
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_out_vos(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_out_vos.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_out_vos The new value for \a mpeg4_out_vos
* \returns The previous value of \a mpeg4_out_vos
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_out_vos(SHCodecs_Encoder * encoder,
				   unsigned long mpeg4_out_vos);

/**
* Get the mpeg4_out_gov.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_out_gov
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_out_gov(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_out_gov.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_out_gov The new value for \a mpeg4_out_gov
* \returns The previous value of \a mpeg4_out_gov
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_out_gov(SHCodecs_Encoder * encoder,
				   unsigned long mpeg4_out_gov);

/**
* Get the mpeg4_aspect_ratio_info_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_aspect_ratio_info_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_aspect_ratio_info_type(SHCodecs_Encoder *
						  encoder);

/**
* Set the mpeg4_aspect_ratio_info_type.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_aspect_ratio_info_type The new value for \a mpeg4_aspect_ratio_info_type
* \returns The previous value of \a mpeg4_aspect_ratio_info_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_aspect_ratio_info_type(SHCodecs_Encoder *
						  encoder,
						  unsigned long
						  mpeg4_aspect_ratio_info_type);

/**
* Get the mpeg4_aspect_ratio_info_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_aspect_ratio_info_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_aspect_ratio_info_value(SHCodecs_Encoder *
						   encoder);

/**
* Set the mpeg4_aspect_ratio_info_value.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_aspect_ratio_info_value The new value for \a mpeg4_aspect_ratio_info_value
* \returns The previous value of \a mpeg4_aspect_ratio_info_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_aspect_ratio_info_value(SHCodecs_Encoder *
						   encoder,
						   unsigned long
						   mpeg4_aspect_ratio_info_value);

/**
* Get the mpeg4_vos_profile_level_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_vos_profile_level_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_vos_profile_level_type(SHCodecs_Encoder *
						  encoder);

/**
* Set the mpeg4_vos_profile_level_type.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_vos_profile_level_type The new value for \a mpeg4_vos_profile_level_type
* \returns The previous value of \a mpeg4_vos_profile_level_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_vos_profile_level_type(SHCodecs_Encoder *
						  encoder,
						  unsigned long
						  mpeg4_vos_profile_level_type);

/**
* Get the mpeg4_vos_profile_level_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_vos_profile_level_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_vos_profile_level_value(SHCodecs_Encoder *
						   encoder);

/**
* Set the mpeg4_vos_profile_level_value.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_vos_profile_level_value The new value for \a mpeg4_vos_profile_level_value
* \returns The previous value of \a mpeg4_vos_profile_level_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_vos_profile_level_value(SHCodecs_Encoder *
						   encoder,
						   unsigned long
						   mpeg4_vos_profile_level_value);

/**
* Get the mpeg4_out_visual_object_identifier.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_out_visual_object_identifier
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_out_visual_object_identifier(SHCodecs_Encoder *
							encoder);

/**
* Set the mpeg4_out_visual_object_identifier.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_out_visual_object_identifier The new value for \a mpeg4_out_visual_object_identifier
* \returns The previous value of \a mpeg4_out_visual_object_identifier
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_out_visual_object_identifier(SHCodecs_Encoder *
							encoder,
							unsigned long
							mpeg4_out_visual_object_identifier);

/**
* Get the mpeg4_visual_object_verid.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_visual_object_verid
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_visual_object_verid(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_visual_object_verid.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_visual_object_verid The new value for \a mpeg4_visual_object_verid
* \returns The previous value of \a mpeg4_visual_object_verid
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_visual_object_verid(SHCodecs_Encoder * encoder,
					       unsigned long
					       mpeg4_visual_object_verid);

/**
* Get the mpeg4_visual_object_priority.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_visual_object_priority
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_visual_object_priority(SHCodecs_Encoder *
						  encoder);

/**
* Set the mpeg4_visual_object_priority.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_visual_object_priority The new value for \a mpeg4_visual_object_priority
* \returns The previous value of \a mpeg4_visual_object_priority
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_visual_object_priority(SHCodecs_Encoder *
						  encoder,
						  unsigned long
						  mpeg4_visual_object_priority);

/**
* Get the mpeg4_video_object_type_indication.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_video_object_type_indication
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_video_object_type_indication(SHCodecs_Encoder *
							encoder);

/**
* Set the mpeg4_video_object_type_indication.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_video_object_type_indication The new value for \a mpeg4_video_object_type_indication
* \returns The previous value of \a mpeg4_video_object_type_indication
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_video_object_type_indication(SHCodecs_Encoder *
							encoder,
							unsigned long
							mpeg4_video_object_type_indication);

/**
* Get the mpeg4_out_object_layer_identifier.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_out_object_layer_identifier
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_out_object_layer_identifier(SHCodecs_Encoder *
						       encoder);

/**
* Set the mpeg4_out_object_layer_identifier.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_out_object_layer_identifier The new value for \a mpeg4_out_object_layer_identifier
* \returns The previous value of \a mpeg4_out_object_layer_identifier
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_out_object_layer_identifier(SHCodecs_Encoder *
						       encoder,
						       unsigned long
						       mpeg4_out_object_layer_identifier);

/**
* Get the mpeg4_video_object_layer_verid.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_video_object_layer_verid
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_video_object_layer_verid(SHCodecs_Encoder *
						    encoder);

/**
* Set the mpeg4_video_object_layer_verid.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_video_object_layer_verid The new value for \a mpeg4_video_object_layer_verid
* \returns The previous value of \a mpeg4_video_object_layer_verid
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_video_object_layer_verid(SHCodecs_Encoder *
						    encoder,
						    unsigned long
						    mpeg4_video_object_layer_verid);

/**
* Get the mpeg4_video_object_layer_priority.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_video_object_layer_priority
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_video_object_layer_priority(SHCodecs_Encoder *
						       encoder);

/**
* Set the mpeg4_video_object_layer_priority.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_video_object_layer_priority The new value for \a mpeg4_video_object_layer_priority
* \returns The previous value of \a mpeg4_video_object_layer_priority
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_video_object_layer_priority(SHCodecs_Encoder *
						       encoder,
						       unsigned long
						       mpeg4_video_object_layer_priority);

/**
* Get the mpeg4_error_resilience_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_error_resilience_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_error_resilience_mode(SHCodecs_Encoder *
						 encoder);

/**
* Set the mpeg4_error_resilience_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_error_resilience_mode The new value for \a mpeg4_error_resilience_mode
* \returns The previous value of \a mpeg4_error_resilience_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_error_resilience_mode(SHCodecs_Encoder *
						 encoder,
						 unsigned long
						 mpeg4_error_resilience_mode);

/**
* Get the mpeg4_video_packet_size_mb.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_video_packet_size_mb
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_video_packet_size_mb(SHCodecs_Encoder *
						encoder);

/**
* Set the mpeg4_video_packet_size_mb.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_video_packet_size_mb The new value for \a mpeg4_video_packet_size_mb
* \returns The previous value of \a mpeg4_video_packet_size_mb
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_video_packet_size_mb(SHCodecs_Encoder * encoder,
						unsigned long
						mpeg4_video_packet_size_mb);

/**
* Get the mpeg4_video_packet_size_bit.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_video_packet_size_bit
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_video_packet_size_bit(SHCodecs_Encoder *
						 encoder);

/**
* Set the mpeg4_video_packet_size_bit.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_video_packet_size_bit The new value for \a mpeg4_video_packet_size_bit
* \returns The previous value of \a mpeg4_video_packet_size_bit
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_video_packet_size_bit(SHCodecs_Encoder *
						 encoder,
						 unsigned long
						 mpeg4_video_packet_size_bit);

/**
* Get the mpeg4_video_packet_header_extention.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_video_packet_header_extention
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_video_packet_header_extention(SHCodecs_Encoder *
							 encoder);

/**
* Set the mpeg4_video_packet_header_extention.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_video_packet_header_extention The new value for \a mpeg4_video_packet_header_extention
* \returns The previous value of \a mpeg4_video_packet_header_extention
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_video_packet_header_extention(SHCodecs_Encoder *
							 encoder,
							 unsigned long
							 mpeg4_video_packet_header_extention);

/**
* Get the mpeg4_data_partitioned.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_data_partitioned
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_data_partitioned(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_data_partitioned.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_data_partitioned The new value for \a mpeg4_data_partitioned
* \returns The previous value of \a mpeg4_data_partitioned
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_data_partitioned(SHCodecs_Encoder * encoder,
					    unsigned long
					    mpeg4_data_partitioned);

/**
* Get the mpeg4_reversible_vlc.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_reversible_vlc
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_reversible_vlc(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_reversible_vlc.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_reversible_vlc The new value for \a mpeg4_reversible_vlc
* \returns The previous value of \a mpeg4_reversible_vlc
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_reversible_vlc(SHCodecs_Encoder * encoder,
					  unsigned long
					  mpeg4_reversible_vlc);

/**
* Get the mpeg4_high_quality.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_high_quality
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_high_quality(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_high_quality.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_high_quality The new value for \a mpeg4_high_quality
* \returns The previous value of \a mpeg4_high_quality
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_high_quality(SHCodecs_Encoder * encoder,
					unsigned long mpeg4_high_quality);

/**
* Get the mpeg4_param_changeable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_param_changeable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_param_changeable(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_param_changeable.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_param_changeable The new value for \a mpeg4_param_changeable
* \returns The previous value of \a mpeg4_param_changeable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_param_changeable(SHCodecs_Encoder * encoder,
					    unsigned long
					    mpeg4_param_changeable);

/**
* Get the mpeg4_changeable_max_bitrate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_changeable_max_bitrate
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_changeable_max_bitrate(SHCodecs_Encoder *
						  encoder);

/**
* Set the mpeg4_changeable_max_bitrate.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_changeable_max_bitrate The new value for \a mpeg4_changeable_max_bitrate
* \returns The previous value of \a mpeg4_changeable_max_bitrate
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_changeable_max_bitrate(SHCodecs_Encoder *
						  encoder,
						  unsigned long
						  mpeg4_changeable_max_bitrate);

/**
* Get the mpeg4_Ivop_quant_initial_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_Ivop_quant_initial_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_Ivop_quant_initial_value(SHCodecs_Encoder *
						    encoder);

/**
* Set the mpeg4_Ivop_quant_initial_value.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_Ivop_quant_initial_value The new value for \a mpeg4_Ivop_quant_initial_value
* \returns The previous value of \a mpeg4_Ivop_quant_initial_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_Ivop_quant_initial_value(SHCodecs_Encoder *
						    encoder,
						    unsigned long
						    mpeg4_Ivop_quant_initial_value);

/**
* Get the mpeg4_Pvop_quant_initial_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_Pvop_quant_initial_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_Pvop_quant_initial_value(SHCodecs_Encoder *
						    encoder);

/**
* Set the mpeg4_Pvop_quant_initial_value.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_Pvop_quant_initial_value The new value for \a mpeg4_Pvop_quant_initial_value
* \returns The previous value of \a mpeg4_Pvop_quant_initial_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_Pvop_quant_initial_value(SHCodecs_Encoder *
						    encoder,
						    unsigned long
						    mpeg4_Pvop_quant_initial_value);

/**
* Get the mpeg4_use_dquant.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_use_dquant
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_use_dquant(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_use_dquant.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_use_dquant The new value for \a mpeg4_use_dquant
* \returns The previous value of \a mpeg4_use_dquant
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_use_dquant(SHCodecs_Encoder * encoder,
				      unsigned long mpeg4_use_dquant);

/**
* Get the mpeg4_clip_dquant_frame.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_clip_dquant_frame
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_clip_dquant_frame(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_clip_dquant_frame.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_clip_dquant_frame The new value for \a mpeg4_clip_dquant_frame
* \returns The previous value of \a mpeg4_clip_dquant_frame
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_clip_dquant_frame(SHCodecs_Encoder * encoder,
					     unsigned long
					     mpeg4_clip_dquant_frame);

/**
* Get the mpeg4_quant_min.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_quant_min
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_quant_min(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_quant_min.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_quant_min The new value for \a mpeg4_quant_min
* \returns The previous value of \a mpeg4_quant_min
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_quant_min(SHCodecs_Encoder * encoder,
				     unsigned long mpeg4_quant_min);

/**
* Get the mpeg4_quant_min_Ivop_under_range.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_quant_min_Ivop_under_range
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_quant_min_Ivop_under_range(SHCodecs_Encoder *
						      encoder);

/**
* Set the mpeg4_quant_min_Ivop_under_range.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_quant_min_Ivop_under_range The new value for \a mpeg4_quant_min_Ivop_under_range
* \returns The previous value of \a mpeg4_quant_min_Ivop_under_range
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_quant_min_Ivop_under_range(SHCodecs_Encoder *
						      encoder,
						      unsigned long
						      mpeg4_quant_min_Ivop_under_range);

/**
* Get the mpeg4_quant_max.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_quant_max
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_quant_max(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_quant_max.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_quant_max The new value for \a mpeg4_quant_max
* \returns The previous value of \a mpeg4_quant_max
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_quant_max(SHCodecs_Encoder * encoder,
				     unsigned long mpeg4_quant_max);

/**
* Get the mpeg4_ratecontrol_vbv_skipcheck_enable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_skipcheck_enable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_skipcheck_enable
(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_ratecontrol_vbv_skipcheck_enable.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_ratecontrol_vbv_skipcheck_enable The new value for \a mpeg4_ratecontrol_vbv_skipcheck_enable
* \returns The previous value of \a mpeg4_ratecontrol_vbv_skipcheck_enable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_ratecontrol_vbv_skipcheck_enable
(SHCodecs_Encoder * encoder, unsigned long mpeg4_ratecontrol_vbv_skipcheck_enable);

/**
* Get the mpeg4_ratecontrol_vbv_Ivop_noskip.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_Ivop_noskip
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_Ivop_noskip(SHCodecs_Encoder *
						       encoder);

/**
* Set the mpeg4_ratecontrol_vbv_Ivop_noskip.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_ratecontrol_vbv_Ivop_noskip The new value for \a mpeg4_ratecontrol_vbv_Ivop_noskip
* \returns The previous value of \a mpeg4_ratecontrol_vbv_Ivop_noskip
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_ratecontrol_vbv_Ivop_noskip(SHCodecs_Encoder *
						       encoder,
						       unsigned long
						       mpeg4_ratecontrol_vbv_Ivop_noskip);

/**
* Get the mpeg4_ratecontrol_vbv_remain_zero_skip_enable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_remain_zero_skip_enable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_remain_zero_skip_enable
(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_ratecontrol_vbv_remain_zero_skip_enable.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_ratecontrol_vbv_remain_zero_skip_enable The new value for \a mpeg4_ratecontrol_vbv_remain_zero_skip_enable
* \returns The previous value of \a mpeg4_ratecontrol_vbv_remain_zero_skip_enable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_ratecontrol_vbv_remain_zero_skip_enable
(SHCodecs_Encoder * encoder,
unsigned long mpeg4_ratecontrol_vbv_remain_zero_skip_enable);

/**
* Get the mpeg4_ratecontrol_vbv_buffer_unit_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_buffer_unit_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_buffer_unit_size
(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_ratecontrol_vbv_buffer_unit_size.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_ratecontrol_vbv_buffer_unit_size The new value for \a mpeg4_ratecontrol_vbv_buffer_unit_size
* \returns The previous value of \a mpeg4_ratecontrol_vbv_buffer_unit_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_ratecontrol_vbv_buffer_unit_size
(SHCodecs_Encoder * encoder, unsigned long mpeg4_ratecontrol_vbv_buffer_unit_size);

/**
* Get the mpeg4_ratecontrol_vbv_buffer_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_buffer_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_buffer_mode(SHCodecs_Encoder *
						       encoder);

/**
* Set the mpeg4_ratecontrol_vbv_buffer_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_ratecontrol_vbv_buffer_mode The new value for \a mpeg4_ratecontrol_vbv_buffer_mode
* \returns The previous value of \a mpeg4_ratecontrol_vbv_buffer_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_ratecontrol_vbv_buffer_mode(SHCodecs_Encoder *
						       encoder,
						       unsigned long
						       mpeg4_ratecontrol_vbv_buffer_mode);

/**
* Get the mpeg4_ratecontrol_vbv_max_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_max_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_max_size(SHCodecs_Encoder *
						    encoder);

/**
* Set the mpeg4_ratecontrol_vbv_max_size.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_ratecontrol_vbv_max_size The new value for \a mpeg4_ratecontrol_vbv_max_size
* \returns The previous value of \a mpeg4_ratecontrol_vbv_max_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_ratecontrol_vbv_max_size(SHCodecs_Encoder *
						    encoder,
						    unsigned long
						    mpeg4_ratecontrol_vbv_max_size);

/**
* Get the mpeg4_ratecontrol_vbv_offset.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_offset
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_offset(SHCodecs_Encoder *
						  encoder);

/**
* Set the mpeg4_ratecontrol_vbv_offset.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_ratecontrol_vbv_offset The new value for \a mpeg4_ratecontrol_vbv_offset
* \returns The previous value of \a mpeg4_ratecontrol_vbv_offset
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_ratecontrol_vbv_offset(SHCodecs_Encoder *
						  encoder,
						  unsigned long
						  mpeg4_ratecontrol_vbv_offset);

/**
* Get the mpeg4_ratecontrol_vbv_offset_rate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_offset_rate
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_offset_rate(SHCodecs_Encoder *
						       encoder);

/**
* Set the mpeg4_ratecontrol_vbv_offset_rate.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_ratecontrol_vbv_offset_rate The new value for \a mpeg4_ratecontrol_vbv_offset_rate
* \returns The previous value of \a mpeg4_ratecontrol_vbv_offset_rate
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_ratecontrol_vbv_offset_rate(SHCodecs_Encoder *
						       encoder,
						       unsigned long
						       mpeg4_ratecontrol_vbv_offset_rate);

/**
* Get the mpeg4_quant_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_quant_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_quant_type(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_quant_type.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_quant_type The new value for \a mpeg4_quant_type
* \returns The previous value of \a mpeg4_quant_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_quant_type(SHCodecs_Encoder * encoder,
				      unsigned long mpeg4_quant_type);

/**
* Get the mpeg4_use_AC_prediction.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_use_AC_prediction
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_use_AC_prediction(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_use_AC_prediction.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_use_AC_prediction The new value for \a mpeg4_use_AC_prediction
* \returns The previous value of \a mpeg4_use_AC_prediction
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_use_AC_prediction(SHCodecs_Encoder * encoder,
					     unsigned long
					     mpeg4_use_AC_prediction);

/**
* Get the mpeg4_vop_min_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_vop_min_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_vop_min_mode(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_vop_min_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_vop_min_mode The new value for \a mpeg4_vop_min_mode
* \returns The previous value of \a mpeg4_vop_min_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_vop_min_mode(SHCodecs_Encoder * encoder,
					unsigned long mpeg4_vop_min_mode);

/**
* Get the mpeg4_vop_min_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_vop_min_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_vop_min_size(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_vop_min_size.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_vop_min_size The new value for \a mpeg4_vop_min_size
* \returns The previous value of \a mpeg4_vop_min_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_vop_min_size(SHCodecs_Encoder * encoder,
					unsigned long mpeg4_vop_min_size);

/**
* Get the mpeg4_intra_thr.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_intra_thr
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_intra_thr(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_intra_thr.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_intra_thr The new value for \a mpeg4_intra_thr
* \returns The previous value of \a mpeg4_intra_thr
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_intra_thr(SHCodecs_Encoder * encoder,
				     unsigned long mpeg4_intra_thr);

/**
* Get the mpeg4_b_vop_num.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_b_vop_num
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_b_vop_num(SHCodecs_Encoder * encoder);

/**
* Set the mpeg4_b_vop_num.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_b_vop_num The new value for \a mpeg4_b_vop_num
* \returns The previous value of \a mpeg4_b_vop_num
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_b_vop_num(SHCodecs_Encoder * encoder,
				     unsigned long mpeg4_b_vop_num);


#endif				/* __SHCODECS_ENCODE_MPEG4_H__ */
