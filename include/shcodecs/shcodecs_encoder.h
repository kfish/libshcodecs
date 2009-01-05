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
 * Signature of a callback for libshcodecs to call when it has encoded
 * YUV 4:2:0 data. To pause encoding, return 1 from this callback.
 * \param encoder The SHCodecs_Encoder* handle
 * \param data The encoded data
 * \param length Length of encoded data in bytes
 * \param user_data Arbitrary data supplied by user
 * \retval 0 Continue encoding
 * \retval 1 Pause encoding, return from shcodecs_encode()
 */
typedef int (*SHCodecs_Encoded_Callback) (SHCodecs_Encoder * encoder,
                                          unsigned char * data, int length,
                                          void * user_data);

#if 0
#endif

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
shcodecs_encoder_set_encoded_callback (SHCodecs_Encoder * encoder,
                                       SHCodecs_Encoded_Callback encoded_cb,
                                       void * user_data);

/**
 * Encode a buffer of input data. This function will call the previously
 * registered callback each time it has encoded a complete frame. If that
 * callback returns 1, encoding is paused and shcodecs_encode() will
 * return immediately. The encode state will be retained between successive
 * calls.
 * \param encoder The SHCodecs_Encoder* handle
 * \param y_buf The encoded Y plane
 * \param y_size The size in bytes of the encoded Y data
 * \param c_buf The encoded C plane
 * \param c_size The size in bytes of the encoded C data
 * \retval 0 Success
 */
int
shcodecs_encode (SHCodecs_Encoder * encoder,
                 unsigned char * y_buf, int y_size,
                 unsigned char * c_buf, int c_size);

#endif /* __SHCODECS_ENCODER_H__ */
