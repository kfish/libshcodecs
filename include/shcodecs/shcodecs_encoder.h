/*
  Copyright (C) Renesas Technology Corp., 2003-2005. All rights reserved.
 */

#ifndef __SHCODECS_ENCODER_H__
#define __SHCODECS_ENCODER_H__

#include <shcodecs/shcodecs_common.h>

/** \file
 *
 * Encoder functions.
 */

/**
 * An opaque handle to the VPU4 for encoding.
 */
typedef void SHCodecs_Encoder;

/**
 * Signature of a callback for libshcodecs to call when it requires YUV 4:2:0 data.
 * To pause encoding, return 1 from this callback.
 * \param encoder The SHCodecs_Encoder* handle
 * \param user_data Arbitrary data supplied by user
 * \retval 0 Continue encoding
 * \retval 1 Pause encoding, return from shcodecs_encode()
 */
typedef int (*SHCodecs_Encoder_Input) (SHCodecs_Encoder * encoder, void * user_data);

/**
 * Signature of a callback for libshcodecs to call when it has encoded data.
 * To pause encoding, return 1 from this callback.
 * \param encoder The SHCodecs_Encoder* handle
 * \param data The encoded data
 * \param length Length of encoded data in bytes
 * \param user_data Arbitrary data supplied by user
 * \retval 0 Continue encoding
 * \retval 1 Pause encoding, return from shcodecs_encode()
 */
typedef int (*SHCodecs_Encoder_Output) (SHCodecs_Encoder * encoder,
                                        unsigned char * data, int length,
                                        void * user_data);

/**
 * Initialize the VPU4 for encoding a given video format.
 * \param width The video image width
 * \param height The video image height
 * \param format SHCodecs_Format_MPEG4 or SHCODECS_Format_H264
 * \return encoder The SHCodecs_Encoder* handle
 */
SHCodecs_Encoder *
shcodecs_encoder_init(int width, int height, SHCodecs_Format format);

/**
 * Deallocate resources used to initialize the VPU4 for encoding,
 * and reset it for future use.
 * \param encoder The SHCodecs_Encoder* handle
 */
void
shcodecs_encoder_close (SHCodecs_Encoder * encoder);

/**
 * Set the callback for libshcodecs to call when encoded data is available.
 * \param encoder The SHCodecs_Encoder* handle
 * \param encodec_cb The callback function
 * \param user_data Additional data to pass to the callback function
 */
int
shcodecs_encoder_set_output_callback (SHCodecs_Encoder * encoder,
                                      SHCodecs_Encoder_Output output_cb,
                                      void * user_data);

/**
 * Set the callback for libshcodecs to call when raw YUV data is required.
 * \param encoder The SHCodecs_Encoder* handle
 * \param get_input_cb The callback function
 * \param user_data Additional data to pass to the callback function
 */
int
shcodecs_encoder_set_input_callback (SHCodecs_Encoder * encoder,
                                     SHCodecs_Encoder_Input input_cb,
                                     void * user_data);

/**
 * Provide input data to the encoder.
 * This function MUST be called from within an SHCodecs_Encoder_Input callback,
 * otherwise the encode hardware will not see any data.
 * The byte sizes of these planes do not vary unless the image dimensions vary,
 * and can be retrieved with shcodecs_encoder_get_y_bytes() and
 * shcodecs_encoder_get_c_bytes().
 * \param encoder The SHCodecs_Encoder* handle
 * \param y_input Pointer to the Y plane of input data
 * \param c_input Pointer to the CbCr plane of input data
 * \retval 0 Success
 */
int
shcodecs_encoder_input_provide (SHCodecs_Encoder * encoder, 
                                unsigned char * y_input, unsigned char * c_input);


/**
 * Get the user space pointer to the next input buffer.
 * The byte sizes of these planes do not vary unless the image dimensions vary,
 * and can be retrieved with shcodecs_encoder_get_y_bytes() and
 * shcodecs_encoder_get_c_bytes().
 * \param encoder The SHCodecs_Encoder* handle
 * \param pp_y_input Address of the pointer to luma
 * \param pp_c_input Address of the pointer to chroma
 * \retval 0 Success
 */
int
shcodecs_encoder_get_input_buf (SHCodecs_Encoder *encoder, 
                                unsigned char **pp_y_input, unsigned char **pp_c_input);

/**
 * Get the size in bytes of a Y plane of input data.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns size in bytes of Y plane.
 * \retval -1 \a encoder invalid
 */
int
shcodecs_encoder_get_y_bytes (SHCodecs_Encoder * encoder);

/**
 * Get the size in bytes of a CbCr plane of input data.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns size in bytes of CbCr plane.
 * \retval -1 \a encoder invalid
 */
int
shcodecs_encoder_get_c_bytes (SHCodecs_Encoder * encoder);

/**
 * Run the encoder.
 * \param encoder The SHCodecs_Encoder* handle
 * \retval 0 Success
 */
int
shcodecs_encoder_run (SHCodecs_Encoder * encoder);

/**
 * Get the width in pixels of the encoded image
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The width in pixels
 * \retval -1 \a encoder invalid
 */
int shcodecs_encoder_get_width (SHCodecs_Encoder * encoder);

/**
 * Set the width in pixels of the encoded image.
 * Note that this function can only be called during intialization,
 * ie. before the first call to shcodecs_encoder_run().
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The width in pixels
 * \retval -1 \a encoder invalid
 * \retval -2 \a encoder already initialized.
 * \retval -3 \a width out of allowed range
 */
int shcodecs_encoder_set_width (SHCodecs_Encoder * encoder, int width);

/**
 * Get the height in pixels of the encoded image
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The height in pixels
 * \retval -1 \a encoder invalid
 */
int shcodecs_encoder_get_height (SHCodecs_Encoder * encoder);

/**
 * Set the height in pixels of the encoded image.
 * Note that this function can only be called during intialization,
 * ie. before the first call to shcodecs_encoder_run().
 * \param encoder The SHCodecs_Encoder* handle
 * \returns The height in pixels
 * \retval -1 \a encoder invalid
 * \retval -2 \a encoder already initialized.
 * \retval -3 \a height out of allowed range
 */
int shcodecs_encoder_set_height (SHCodecs_Encoder * encoder, int height);

/**
 * Get the number of input frames elapsed since the last output callback.
 * This is typically called by the client in the encoder output callback.
 * \param encoder The SHCodecs_Encoder* handle
 * \returns 0 when more data of the same frame has just been output.
 * \returns >0 for the number of frames since previous output callback.
 * \retval -1 \a encoder invalid
 */
int
shcodecs_encoder_get_frame_num_delta(SHCodecs_Encoder *encoder);

/**
 * Get the physical address of input data.
 * This function MUST be called from within an SHCodecs_Encoder_Input callback.
 * \param encoder The SHCodecs_Encoder* handle
 * \param y_input Pointer to the Y plane of input data
 * \param c_input Pointer to the CbCr plane of input data
 * \retval 0 Success
 * \retval -1 \a encoder invalid
 */
int
shcodecs_encoder_get_input_physical_addr (SHCodecs_Encoder * encoder, 
                                          unsigned int *addr_y, unsigned int *addr_C);


#include <shcodecs/encode_general.h>
#include <shcodecs/encode_properties.h>
#include <shcodecs/encode_h264.h>
#include <shcodecs/encode_mpeg4.h>

#endif /* __SHCODECS_ENCODER_H__ */
