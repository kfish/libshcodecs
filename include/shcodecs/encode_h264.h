#ifndef __SHCODECS_ENCODE_H264_H__
#define __SHCODECS_ENCODE_H264_H__

/**
* Get the h264_Ivop_quant_initial_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_Ivop_quant_initial_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_Ivop_quant_initial_value(SHCodecs_Encoder *
						   encoder);

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
						   h264_Ivop_quant_initial_value);

/**
* Get the h264_Pvop_quant_initial_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_Pvop_quant_initial_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_Pvop_quant_initial_value(SHCodecs_Encoder *
						   encoder);

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
						   h264_Pvop_quant_initial_value);

/**
* Get the h264_use_dquant.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_use_dquant
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_use_dquant(SHCodecs_Encoder * encoder);

/**
* Set the h264_use_dquant.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_use_dquant The new value for \a h264_use_dquant
* \returns The previous value of \a h264_use_dquant
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_use_dquant(SHCodecs_Encoder * encoder,
				     unsigned long h264_use_dquant);

/**
* Get the h264_clip_dquant_next_mb.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_clip_dquant_next_mb
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_clip_dquant_next_mb(SHCodecs_Encoder * encoder);

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
					      h264_clip_dquant_next_mb);

/**
* Get the h264_clip_dquant_frame.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_clip_dquant_frame
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_clip_dquant_frame(SHCodecs_Encoder * encoder);

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
					    h264_clip_dquant_frame);

/**
* Get the h264_quant_min.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_quant_min
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_quant_min(SHCodecs_Encoder * encoder);

/**
* Set the h264_quant_min.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_quant_min The new value for \a h264_quant_min
* \returns The previous value of \a h264_quant_min
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_quant_min(SHCodecs_Encoder * encoder,
				    unsigned long h264_quant_min);

/**
* Get the h264_quant_min_Ivop_under_range.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_quant_min_Ivop_under_range
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_quant_min_Ivop_under_range(SHCodecs_Encoder *
						     encoder);

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
						     h264_quant_min_Ivop_under_range);

/**
* Get the h264_quant_max.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_quant_max
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_quant_max(SHCodecs_Encoder * encoder);

/**
* Set the h264_quant_max.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_quant_max The new value for \a h264_quant_max
* \returns The previous value of \a h264_quant_max
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_quant_max(SHCodecs_Encoder * encoder,
				    unsigned long h264_quant_max);

/**
* Get the h264_ratecontrol_cpb_skipcheck_enable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_skipcheck_enable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_skipcheck_enable(SHCodecs_Encoder
							   * encoder);

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
							   h264_ratecontrol_cpb_skipcheck_enable);

/**
* Get the h264_ratecontrol_cpb_Ivop_noskip.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_Ivop_noskip
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_Ivop_noskip(SHCodecs_Encoder *
						      encoder);

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
						      h264_ratecontrol_cpb_Ivop_noskip);

/**
* Get the h264_ratecontrol_cpb_remain_zero_skip_enable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_remain_zero_skip_enable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_remain_zero_skip_enable
(SHCodecs_Encoder * encoder);

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
unsigned long h264_ratecontrol_cpb_remain_zero_skip_enable);

/**
* Get the h264_ratecontrol_cpb_buffer_unit_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_buffer_unit_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_buffer_unit_size(SHCodecs_Encoder
							   * encoder);

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
							   h264_ratecontrol_cpb_buffer_unit_size);

/**
* Get the h264_ratecontrol_cpb_buffer_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_buffer_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_buffer_mode(SHCodecs_Encoder *
						      encoder);

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
						      h264_ratecontrol_cpb_buffer_mode);

/**
* Get the h264_ratecontrol_cpb_max_size.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_max_size
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_max_size(SHCodecs_Encoder *
						   encoder);

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
						   h264_ratecontrol_cpb_max_size);

/**
* Get the h264_ratecontrol_cpb_offset.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_offset
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_offset(SHCodecs_Encoder *
						 encoder);

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
						 h264_ratecontrol_cpb_offset);

/**
* Get the h264_ratecontrol_cpb_offset_rate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_ratecontrol_cpb_offset_rate
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_ratecontrol_cpb_offset_rate(SHCodecs_Encoder *
						      encoder);

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
						      h264_ratecontrol_cpb_offset_rate);

/**
* Get the h264_intra_thr_1.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_intra_thr_1
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_intra_thr_1(SHCodecs_Encoder * encoder);

/**
* Set the h264_intra_thr_1.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_intra_thr_1 The new value for \a h264_intra_thr_1
* \returns The previous value of \a h264_intra_thr_1
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_intra_thr_1(SHCodecs_Encoder * encoder,
				      unsigned long h264_intra_thr_1);

/**
* Get the h264_intra_thr_2.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_intra_thr_2
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_intra_thr_2(SHCodecs_Encoder * encoder);

/**
* Set the h264_intra_thr_2.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_intra_thr_2 The new value for \a h264_intra_thr_2
* \returns The previous value of \a h264_intra_thr_2
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_intra_thr_2(SHCodecs_Encoder * encoder,
				      unsigned long h264_intra_thr_2);

/**
* Get the h264_sad_intra_bias.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_sad_intra_bias
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_sad_intra_bias(SHCodecs_Encoder * encoder);

/**
* Set the h264_sad_intra_bias.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_sad_intra_bias The new value for \a h264_sad_intra_bias
* \returns The previous value of \a h264_sad_intra_bias
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_sad_intra_bias(SHCodecs_Encoder * encoder,
					 unsigned long
					 h264_sad_intra_bias);

/**
* Get the h264_regularly_inserted_I_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_regularly_inserted_I_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_regularly_inserted_I_type(SHCodecs_Encoder *
						    encoder);

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
						    h264_regularly_inserted_I_type);

/**
* Get the h264_call_unit.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_call_unit
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_call_unit(SHCodecs_Encoder * encoder);

/**
* Set the h264_call_unit.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_call_unit The new value for \a h264_call_unit
* \returns The previous value of \a h264_call_unit
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_call_unit(SHCodecs_Encoder * encoder,
				    unsigned long h264_call_unit);

/**
* Get the h264_use_slice.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_use_slice
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_use_slice(SHCodecs_Encoder * encoder);

/**
* Set the h264_use_slice.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_use_slice The new value for \a h264_use_slice
* \returns The previous value of \a h264_use_slice
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_use_slice(SHCodecs_Encoder * encoder,
				    unsigned long h264_use_slice);

/**
* Get the h264_slice_size_mb.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_slice_size_mb
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_slice_size_mb(SHCodecs_Encoder * encoder);

/**
* Set the h264_slice_size_mb.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_slice_size_mb The new value for \a h264_slice_size_mb
* \returns The previous value of \a h264_slice_size_mb
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_slice_size_mb(SHCodecs_Encoder * encoder,
					unsigned long h264_slice_size_mb);

/**
* Get the h264_slice_size_bit.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_slice_size_bit
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_slice_size_bit(SHCodecs_Encoder * encoder);

/**
* Set the h264_slice_size_bit.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_slice_size_bit The new value for \a h264_slice_size_bit
* \returns The previous value of \a h264_slice_size_bit
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_slice_size_bit(SHCodecs_Encoder * encoder,
					 unsigned long
					 h264_slice_size_bit);

/**
* Get the h264_slice_type_value_pattern.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_slice_type_value_pattern
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_slice_type_value_pattern(SHCodecs_Encoder *
						   encoder);

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
						   h264_slice_type_value_pattern);

/**
* Get the h264_use_mb_partition.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_use_mb_partition
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_use_mb_partition(SHCodecs_Encoder * encoder);

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
					   h264_use_mb_partition);

/**
* Get the h264_mb_partition_vector_thr.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_mb_partition_vector_thr
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_mb_partition_vector_thr(SHCodecs_Encoder *
						  encoder);

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
						  h264_mb_partition_vector_thr);

/**
* Get the h264_deblocking_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_deblocking_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_deblocking_mode(SHCodecs_Encoder * encoder);

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
					  h264_deblocking_mode);

/**
* Get the h264_use_deblocking_filter_control.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_use_deblocking_filter_control
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_use_deblocking_filter_control(SHCodecs_Encoder *
							encoder);

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
							h264_use_deblocking_filter_control);

/**
* Get the h264_deblocking_alpha_offset.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_deblocking_alpha_offset
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get_h264_deblocking_alpha_offset(SHCodecs_Encoder *
						  encoder);

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
						  h264_deblocking_alpha_offset);

/**
* Get the h264_deblocking_beta_offset.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_deblocking_beta_offset
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get_h264_deblocking_beta_offset(SHCodecs_Encoder *
						 encoder);

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
						 h264_deblocking_beta_offset);

/**
* Get the h264_me_skip_mode.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_me_skip_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_me_skip_mode(SHCodecs_Encoder * encoder);

/**
* Set the h264_me_skip_mode.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_me_skip_mode The new value for \a h264_me_skip_mode
* \returns The previous value of \a h264_me_skip_mode
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_me_skip_mode(SHCodecs_Encoder * encoder,
				       unsigned long h264_me_skip_mode);

/**
* Get the h264_put_start_code.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_put_start_code
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_put_start_code(SHCodecs_Encoder * encoder);

/**
* Set the h264_put_start_code.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_put_start_code The new value for \a h264_put_start_code
* \returns The previous value of \a h264_put_start_code
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_put_start_code(SHCodecs_Encoder * encoder,
					 unsigned long
					 h264_put_start_code);

/**
* Get the h264_param_changeable.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_param_changeable
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_param_changeable(SHCodecs_Encoder * encoder);

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
					   h264_param_changeable);

/**
* Get the h264_changeable_max_bitrate.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_changeable_max_bitrate
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_changeable_max_bitrate(SHCodecs_Encoder *
						 encoder);

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
						 h264_changeable_max_bitrate);

/**
* Get the h264_seq_param_set_id.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_seq_param_set_id
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_seq_param_set_id(SHCodecs_Encoder * encoder);

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
					   h264_seq_param_set_id);

/**
* Get the h264_profile.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_profile
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_profile(SHCodecs_Encoder * encoder);

/**
* Set the h264_profile.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_profile The new value for \a h264_profile
* \returns The previous value of \a h264_profile
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_profile(SHCodecs_Encoder * encoder,
				  unsigned long h264_profile);

/**
* Get the h264_constraint_set_flag.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_constraint_set_flag
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_constraint_set_flag(SHCodecs_Encoder * encoder);

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
					      h264_constraint_set_flag);

/**
* Get the h264_level_type.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_level_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_level_type(SHCodecs_Encoder * encoder);

/**
* Set the h264_level_type.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_level_type The new value for \a h264_level_type
* \returns The previous value of \a h264_level_type
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_level_type(SHCodecs_Encoder * encoder,
				     unsigned long h264_level_type);

/**
* Get the h264_level_value.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_level_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_level_value(SHCodecs_Encoder * encoder);

/**
* Set the h264_level_value.
* \param encoder The SHCodecs_Encoder* handle
* \param h264_level_value The new value for \a h264_level_value
* \returns The previous value of \a h264_level_value
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_set_h264_level_value(SHCodecs_Encoder * encoder,
				      unsigned long h264_level_value);

/**
* Get the h264_out_vui_parameters.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_out_vui_parameters
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_out_vui_parameters(SHCodecs_Encoder * encoder);

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
					     h264_out_vui_parameters);

/**
* Get the h264_chroma_qp_index_offset.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_chroma_qp_index_offset
* \retval -1 \a encoder invalid
*/
long
shcodecs_encoder_get_h264_chroma_qp_index_offset(SHCodecs_Encoder *
						 encoder);

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
						 h264_chroma_qp_index_offset);

/**
* Get the h264_constrained_intra_pred.
* \param encoder The SHCodecs_Encoder* handle
* \returns The h264_constrained_intra_pred
* \retval -1 \a encoder invalid
*/
unsigned long
shcodecs_encoder_get_h264_constrained_intra_pred(SHCodecs_Encoder *
						 encoder);

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
						 h264_constrained_intra_pred);


#endif				/* __SHCODECS_ENCODE_H264_H__ */
