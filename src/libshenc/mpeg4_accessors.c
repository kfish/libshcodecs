#include <stdlib.h>

#include "encoder_private.h"

/**
* Get the mpeg4_out_vos.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_out_vos
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_out_vos(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_out_vos;
}

/**
* Set the mpeg4_out_vos.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_out_vos The new value for \a mpeg4_out_vos
* \returns The previous value of \a mpeg4_out_vos
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_out_vos(SHCodecs_Encoder * encoder,
				   unsigned long mpeg4_out_vos)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_out_vos;
	encoder->other_options_mpeg4.avcbe_out_vos = mpeg4_out_vos;

	return old_value;
}

/**
* Get the mpeg4_out_gov.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_out_gov
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_out_gov(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_out_gov;
}

/**
* Set the mpeg4_out_gov.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_out_gov The new value for \a mpeg4_out_gov
* \returns The previous value of \a mpeg4_out_gov
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_out_gov(SHCodecs_Encoder * encoder,
				   unsigned long mpeg4_out_gov)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_out_gov;
	encoder->other_options_mpeg4.avcbe_out_gov = mpeg4_out_gov;

	return old_value;
}

/**
* Get the mpeg4_aspect_ratio_info_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_aspect_ratio_info_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_aspect_ratio_info_type(SHCodecs_Encoder *
						  encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_aspect_ratio_info_type;
}

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
						  mpeg4_aspect_ratio_info_type)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_aspect_ratio_info_type;
	encoder->other_options_mpeg4.avcbe_aspect_ratio_info_type =
	    mpeg4_aspect_ratio_info_type;

	return old_value;
}

/**
* Get the mpeg4_aspect_ratio_info_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_aspect_ratio_info_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_aspect_ratio_info_value(SHCodecs_Encoder *
						   encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_aspect_ratio_info_value;
}

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
						   mpeg4_aspect_ratio_info_value)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_aspect_ratio_info_value;
	encoder->other_options_mpeg4.avcbe_aspect_ratio_info_value =
	    mpeg4_aspect_ratio_info_value;

	return old_value;
}

/**
* Get the mpeg4_vos_profile_level_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_vos_profile_level_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_vos_profile_level_type(SHCodecs_Encoder *
						  encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_vos_profile_level_type;
}

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
						  mpeg4_vos_profile_level_type)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_vos_profile_level_type;
	encoder->other_options_mpeg4.avcbe_vos_profile_level_type =
	    mpeg4_vos_profile_level_type;

	return old_value;
}

/**
* Get the mpeg4_vos_profile_level_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_vos_profile_level_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_vos_profile_level_value(SHCodecs_Encoder *
						   encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_vos_profile_level_value;
}

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
						   mpeg4_vos_profile_level_value)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_vos_profile_level_value;
	encoder->other_options_mpeg4.avcbe_vos_profile_level_value =
	    mpeg4_vos_profile_level_value;

	return old_value;
}

/**
* Get the mpeg4_out_visual_object_identifier.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_out_visual_object_identifier
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_out_visual_object_identifier(SHCodecs_Encoder *
							encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.
	    avcbe_out_visual_object_identifier;
}

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
							mpeg4_out_visual_object_identifier)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.
	    avcbe_out_visual_object_identifier;
	encoder->other_options_mpeg4.avcbe_out_visual_object_identifier =
	    mpeg4_out_visual_object_identifier;

	return old_value;
}

/**
* Get the mpeg4_visual_object_verid.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_visual_object_verid
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_visual_object_verid(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_visual_object_verid;
}

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
					       mpeg4_visual_object_verid)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_visual_object_verid;
	encoder->other_options_mpeg4.avcbe_visual_object_verid =
	    mpeg4_visual_object_verid;

	return old_value;
}

/**
* Get the mpeg4_visual_object_priority.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_visual_object_priority
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_visual_object_priority(SHCodecs_Encoder *
						  encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_visual_object_priority;
}

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
						  mpeg4_visual_object_priority)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_visual_object_priority;
	encoder->other_options_mpeg4.avcbe_visual_object_priority =
	    mpeg4_visual_object_priority;

	return old_value;
}

/**
* Get the mpeg4_video_object_type_indication.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_video_object_type_indication
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_video_object_type_indication(SHCodecs_Encoder *
							encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.
	    avcbe_video_object_type_indication;
}

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
							mpeg4_video_object_type_indication)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.
	    avcbe_video_object_type_indication;
	encoder->other_options_mpeg4.avcbe_video_object_type_indication =
	    mpeg4_video_object_type_indication;

	return old_value;
}

/**
* Get the mpeg4_out_object_layer_identifier.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_out_object_layer_identifier
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_out_object_layer_identifier(SHCodecs_Encoder *
						       encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.
	    avcbe_out_object_layer_identifier;
}

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
						       mpeg4_out_object_layer_identifier)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_out_object_layer_identifier;
	encoder->other_options_mpeg4.avcbe_out_object_layer_identifier =
	    mpeg4_out_object_layer_identifier;

	return old_value;
}

/**
* Get the mpeg4_video_object_layer_verid.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_video_object_layer_verid
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_video_object_layer_verid(SHCodecs_Encoder *
						    encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_video_object_layer_verid;
}

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
						    mpeg4_video_object_layer_verid)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_video_object_layer_verid;
	encoder->other_options_mpeg4.avcbe_video_object_layer_verid =
	    mpeg4_video_object_layer_verid;

	return old_value;
}

/**
* Get the mpeg4_video_object_layer_priority.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_video_object_layer_priority
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_video_object_layer_priority(SHCodecs_Encoder *
						       encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.
	    avcbe_video_object_layer_priority;
}

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
						       mpeg4_video_object_layer_priority)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_video_object_layer_priority;
	encoder->other_options_mpeg4.avcbe_video_object_layer_priority =
	    mpeg4_video_object_layer_priority;

	return old_value;
}

/**
* Get the mpeg4_error_resilience_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_error_resilience_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_error_resilience_mode(SHCodecs_Encoder *
						 encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_error_resilience_mode;
}

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
						 mpeg4_error_resilience_mode)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_error_resilience_mode;
	encoder->other_options_mpeg4.avcbe_error_resilience_mode =
	    mpeg4_error_resilience_mode;

	return old_value;
}

/**
* Get the mpeg4_video_packet_size_mb.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_video_packet_size_mb
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_video_packet_size_mb(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_video_packet_size_mb;
}

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
						mpeg4_video_packet_size_mb)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_video_packet_size_mb;
	encoder->other_options_mpeg4.avcbe_video_packet_size_mb =
	    mpeg4_video_packet_size_mb;

	return old_value;
}

/**
* Get the mpeg4_video_packet_size_bit.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_video_packet_size_bit
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_video_packet_size_bit(SHCodecs_Encoder *
						 encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_video_packet_size_bit;
}

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
						 mpeg4_video_packet_size_bit)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_video_packet_size_bit;
	encoder->other_options_mpeg4.avcbe_video_packet_size_bit =
	    mpeg4_video_packet_size_bit;

	return old_value;
}

/**
* Get the mpeg4_video_packet_header_extention.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_video_packet_header_extention
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_video_packet_header_extention(SHCodecs_Encoder *
							 encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.
	    avcbe_video_packet_header_extention;
}

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
							 mpeg4_video_packet_header_extention)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.
	    avcbe_video_packet_header_extention;
	encoder->other_options_mpeg4.avcbe_video_packet_header_extention =
	    mpeg4_video_packet_header_extention;

	return old_value;
}

/**
* Get the mpeg4_data_partitioned.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_data_partitioned
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_data_partitioned(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_data_partitioned;
}

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
					    mpeg4_data_partitioned)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_data_partitioned;
	encoder->other_options_mpeg4.avcbe_data_partitioned =
	    mpeg4_data_partitioned;

	return old_value;
}

/**
* Get the mpeg4_reversible_vlc.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_reversible_vlc
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_reversible_vlc(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_reversible_vlc;
}

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
					  mpeg4_reversible_vlc)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_reversible_vlc;
	encoder->other_options_mpeg4.avcbe_reversible_vlc =
	    mpeg4_reversible_vlc;

	return old_value;
}

/**
* Get the mpeg4_high_quality.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_high_quality
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_high_quality(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_high_quality;
}

/**
* Set the mpeg4_high_quality.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_high_quality The new value for \a mpeg4_high_quality
* \returns The previous value of \a mpeg4_high_quality
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_high_quality(SHCodecs_Encoder * encoder,
					unsigned long mpeg4_high_quality)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_high_quality;
	encoder->other_options_mpeg4.avcbe_high_quality =
	    mpeg4_high_quality;

	return old_value;
}

/**
* Get the mpeg4_param_changeable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_param_changeable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_param_changeable(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_param_changeable;
}

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
					    mpeg4_param_changeable)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_param_changeable;
	encoder->other_options_mpeg4.avcbe_param_changeable =
	    mpeg4_param_changeable;

	return old_value;
}

/**
* Get the mpeg4_changeable_max_bitrate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_changeable_max_bitrate
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_changeable_max_bitrate(SHCodecs_Encoder *
						  encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_changeable_max_bitrate;
}

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
						  mpeg4_changeable_max_bitrate)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_changeable_max_bitrate;
	encoder->other_options_mpeg4.avcbe_changeable_max_bitrate =
	    mpeg4_changeable_max_bitrate;

	return old_value;
}

/**
* Get the mpeg4_Ivop_quant_initial_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_Ivop_quant_initial_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_Ivop_quant_initial_value(SHCodecs_Encoder *
						    encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_Ivop_quant_initial_value;
}

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
						    mpeg4_Ivop_quant_initial_value)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_Ivop_quant_initial_value;
	encoder->other_options_mpeg4.avcbe_Ivop_quant_initial_value =
	    mpeg4_Ivop_quant_initial_value;

	return old_value;
}

/**
* Get the mpeg4_Pvop_quant_initial_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_Pvop_quant_initial_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_Pvop_quant_initial_value(SHCodecs_Encoder *
						    encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_Pvop_quant_initial_value;
}

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
						    mpeg4_Pvop_quant_initial_value)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_Pvop_quant_initial_value;
	encoder->other_options_mpeg4.avcbe_Pvop_quant_initial_value =
	    mpeg4_Pvop_quant_initial_value;

	return old_value;
}

/**
* Get the mpeg4_use_dquant.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_use_dquant
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_use_dquant(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_use_dquant;
}

/**
* Set the mpeg4_use_dquant.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_use_dquant The new value for \a mpeg4_use_dquant
* \returns The previous value of \a mpeg4_use_dquant
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_use_dquant(SHCodecs_Encoder * encoder,
				      unsigned long mpeg4_use_dquant)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_use_dquant;
	encoder->other_options_mpeg4.avcbe_use_dquant = mpeg4_use_dquant;

	return old_value;
}

/**
* Get the mpeg4_clip_dquant_frame.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_clip_dquant_frame
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_clip_dquant_frame(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_clip_dquant_frame;
}

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
					     mpeg4_clip_dquant_frame)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_clip_dquant_frame;
	encoder->other_options_mpeg4.avcbe_clip_dquant_frame =
	    mpeg4_clip_dquant_frame;

	return old_value;
}

/**
* Get the mpeg4_quant_min.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_quant_min
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_quant_min(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_quant_min;
}

/**
* Set the mpeg4_quant_min.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_quant_min The new value for \a mpeg4_quant_min
* \returns The previous value of \a mpeg4_quant_min
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_quant_min(SHCodecs_Encoder * encoder,
				     unsigned long mpeg4_quant_min)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_quant_min;
	encoder->other_options_mpeg4.avcbe_quant_min = mpeg4_quant_min;

	return old_value;
}

/**
* Get the mpeg4_quant_min_Ivop_under_range.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_quant_min_Ivop_under_range
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_quant_min_Ivop_under_range(SHCodecs_Encoder *
						      encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.
	    avcbe_quant_min_Ivop_under_range;
}

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
						      mpeg4_quant_min_Ivop_under_range)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_quant_min_Ivop_under_range;
	encoder->other_options_mpeg4.avcbe_quant_min_Ivop_under_range =
	    mpeg4_quant_min_Ivop_under_range;

	return old_value;
}

/**
* Get the mpeg4_quant_max.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_quant_max
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_quant_max(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_quant_max;
}

/**
* Set the mpeg4_quant_max.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_quant_max The new value for \a mpeg4_quant_max
* \returns The previous value of \a mpeg4_quant_max
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_quant_max(SHCodecs_Encoder * encoder,
				     unsigned long mpeg4_quant_max)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_quant_max;
	encoder->other_options_mpeg4.avcbe_quant_max = mpeg4_quant_max;

	return old_value;
}

/**
* Get the mpeg4_ratecontrol_vbv_skipcheck_enable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_skipcheck_enable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_skipcheck_enable
(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.
	    avcbe_ratecontrol_vbv_skipcheck_enable;
}

/**
* Set the mpeg4_ratecontrol_vbv_skipcheck_enable.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_ratecontrol_vbv_skipcheck_enable The new value for \a mpeg4_ratecontrol_vbv_skipcheck_enable
* \returns The previous value of \a mpeg4_ratecontrol_vbv_skipcheck_enable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_ratecontrol_vbv_skipcheck_enable
(SHCodecs_Encoder * encoder, unsigned long mpeg4_ratecontrol_vbv_skipcheck_enable)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.
	    avcbe_ratecontrol_vbv_skipcheck_enable;
	encoder->other_options_mpeg4.
	    avcbe_ratecontrol_vbv_skipcheck_enable =
	    mpeg4_ratecontrol_vbv_skipcheck_enable;

	return old_value;
}

/**
* Get the mpeg4_ratecontrol_vbv_Ivop_noskip.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_Ivop_noskip
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_Ivop_noskip(SHCodecs_Encoder *
						       encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.
	    avcbe_ratecontrol_vbv_Ivop_noskip;
}

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
						       mpeg4_ratecontrol_vbv_Ivop_noskip)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_Ivop_noskip;
	encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_Ivop_noskip =
	    mpeg4_ratecontrol_vbv_Ivop_noskip;

	return old_value;
}

/**
* Get the mpeg4_ratecontrol_vbv_remain_zero_skip_enable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_remain_zero_skip_enable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_remain_zero_skip_enable
(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.
	    avcbe_ratecontrol_vbv_remain_zero_skip_enable;
}

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
unsigned long mpeg4_ratecontrol_vbv_remain_zero_skip_enable)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.
	    avcbe_ratecontrol_vbv_remain_zero_skip_enable;
	encoder->other_options_mpeg4.
	    avcbe_ratecontrol_vbv_remain_zero_skip_enable =
	    mpeg4_ratecontrol_vbv_remain_zero_skip_enable;

	return old_value;
}

/**
* Get the mpeg4_ratecontrol_vbv_buffer_unit_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_buffer_unit_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_buffer_unit_size
(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.
	    avcbe_ratecontrol_vbv_buffer_unit_size;
}

/**
* Set the mpeg4_ratecontrol_vbv_buffer_unit_size.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_ratecontrol_vbv_buffer_unit_size The new value for \a mpeg4_ratecontrol_vbv_buffer_unit_size
* \returns The previous value of \a mpeg4_ratecontrol_vbv_buffer_unit_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_ratecontrol_vbv_buffer_unit_size
(SHCodecs_Encoder * encoder, unsigned long mpeg4_ratecontrol_vbv_buffer_unit_size)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.
	    avcbe_ratecontrol_vbv_buffer_unit_size;
	encoder->other_options_mpeg4.
	    avcbe_ratecontrol_vbv_buffer_unit_size =
	    mpeg4_ratecontrol_vbv_buffer_unit_size;

	return old_value;
}

/**
* Get the mpeg4_ratecontrol_vbv_buffer_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_buffer_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_buffer_mode(SHCodecs_Encoder *
						       encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.
	    avcbe_ratecontrol_vbv_buffer_mode;
}

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
						       mpeg4_ratecontrol_vbv_buffer_mode)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_buffer_mode;
	encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_buffer_mode =
	    mpeg4_ratecontrol_vbv_buffer_mode;

	return old_value;
}

/**
* Get the mpeg4_ratecontrol_vbv_max_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_max_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_max_size(SHCodecs_Encoder *
						    encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_max_size;
}

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
						    mpeg4_ratecontrol_vbv_max_size)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_max_size;
	encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_max_size =
	    mpeg4_ratecontrol_vbv_max_size;

	return old_value;
}

/**
* Get the mpeg4_ratecontrol_vbv_offset.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_offset
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_offset(SHCodecs_Encoder *
						  encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_offset;
}

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
						  mpeg4_ratecontrol_vbv_offset)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_offset;
	encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_offset =
	    mpeg4_ratecontrol_vbv_offset;

	return old_value;
}

/**
* Get the mpeg4_ratecontrol_vbv_offset_rate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_ratecontrol_vbv_offset_rate
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_ratecontrol_vbv_offset_rate(SHCodecs_Encoder *
						       encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.
	    avcbe_ratecontrol_vbv_offset_rate;
}

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
						       mpeg4_ratecontrol_vbv_offset_rate)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value =
	    encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_offset_rate;
	encoder->other_options_mpeg4.avcbe_ratecontrol_vbv_offset_rate =
	    mpeg4_ratecontrol_vbv_offset_rate;

	return old_value;
}

/**
* Get the mpeg4_quant_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_quant_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_quant_type(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_quant_type;
}

/**
* Set the mpeg4_quant_type.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_quant_type The new value for \a mpeg4_quant_type
* \returns The previous value of \a mpeg4_quant_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_quant_type(SHCodecs_Encoder * encoder,
				      unsigned long mpeg4_quant_type)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_quant_type;
	encoder->other_options_mpeg4.avcbe_quant_type = mpeg4_quant_type;

	return old_value;
}

/**
* Get the mpeg4_use_AC_prediction.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_use_AC_prediction
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_use_AC_prediction(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_use_AC_prediction;
}

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
					     mpeg4_use_AC_prediction)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_use_AC_prediction;
	encoder->other_options_mpeg4.avcbe_use_AC_prediction =
	    mpeg4_use_AC_prediction;

	return old_value;
}

/**
* Get the mpeg4_vop_min_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_vop_min_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_vop_min_mode(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_vop_min_mode;
}

/**
* Set the mpeg4_vop_min_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_vop_min_mode The new value for \a mpeg4_vop_min_mode
* \returns The previous value of \a mpeg4_vop_min_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_vop_min_mode(SHCodecs_Encoder * encoder,
					unsigned long mpeg4_vop_min_mode)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_vop_min_mode;
	encoder->other_options_mpeg4.avcbe_vop_min_mode =
	    mpeg4_vop_min_mode;

	return old_value;
}

/**
* Get the mpeg4_vop_min_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_vop_min_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_vop_min_size(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_vop_min_size;
}

/**
* Set the mpeg4_vop_min_size.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_vop_min_size The new value for \a mpeg4_vop_min_size
* \returns The previous value of \a mpeg4_vop_min_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_vop_min_size(SHCodecs_Encoder * encoder,
					unsigned long mpeg4_vop_min_size)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_vop_min_size;
	encoder->other_options_mpeg4.avcbe_vop_min_size =
	    mpeg4_vop_min_size;

	return old_value;
}

/**
* Get the mpeg4_intra_thr.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_intra_thr
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_intra_thr(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_intra_thr;
}

/**
* Set the mpeg4_intra_thr.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_intra_thr The new value for \a mpeg4_intra_thr
* \returns The previous value of \a mpeg4_intra_thr
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_intra_thr(SHCodecs_Encoder * encoder,
				     unsigned long mpeg4_intra_thr)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_intra_thr;
	encoder->other_options_mpeg4.avcbe_intra_thr = mpeg4_intra_thr;

	return old_value;
}

/**
* Get the mpeg4_b_vop_num.
* \param encoder The SHCodecs_Encoder* handle
* \returns The mpeg4_b_vop_num
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_mpeg4_b_vop_num(SHCodecs_Encoder * encoder)
{
	if (encoder == NULL)
		return -1;

	return encoder->other_options_mpeg4.avcbe_b_vop_num;
}

/**
* Set the mpeg4_b_vop_num.
* \param encoder The SHCodecs_Encoder* handle
* \param mpeg4_b_vop_num The new value for \a mpeg4_b_vop_num
* \returns The previous value of \a mpeg4_b_vop_num
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_mpeg4_b_vop_num(SHCodecs_Encoder * encoder,
				     unsigned long mpeg4_b_vop_num)
{
	unsigned long old_value;

	if (encoder == NULL)
		return -1;

	old_value = encoder->other_options_mpeg4.avcbe_b_vop_num;
	encoder->other_options_mpeg4.avcbe_b_vop_num = mpeg4_b_vop_num;

	return old_value;
}
