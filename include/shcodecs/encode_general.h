#ifndef __SHCODECS_ENCODE_GENERAL_H__
#define __SHCODECS_ENCODE_GENERAL_H__

/*
 * Get the frame_number_to_encode
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The frame_number_to_encode
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_get_frame_number_to_encode (SHCodecs_Encoder * encoder);

/*
 * Set the frame_number_to_encode
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The previous value of \a frame_number_to_encode
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_frame_number_to_encode (SHCodecs_Encoder * encoder, long frame_number_to_encode);

/*
 * Get the frame_no_increment
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The frame_no_increment
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_get_frame_no_increment (SHCodecs_Encoder * encoder);

/*
 * Set the frame_no_increment
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The previous value of \a frame_no_increment
 * \retval -1 \a encoder invalid
 */
long
shcodecs_encoder_set_frame_no_increment (SHCodecs_Encoder * encoder, long frame_no_increment);

#endif				/* __SHCODECS_ENCODE_GENERAL_H__ */
