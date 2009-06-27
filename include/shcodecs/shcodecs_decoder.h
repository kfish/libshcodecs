/*
  Copyright (C) Renesas Technology Corp., 2003-2005. All rights reserved.
 */

#ifndef __SHCODECS_DECODER_H__
#define __SHCODECS_DECODER_H__

#include <shcodecs/shcodecs_common.h>

/** \file
 *
 * Decoder functions.
 */

/**
 * An opaque handle to the VPU4 for decoding.
 */
typedef void SHCodecs_Decoder;

/**
 * Signature of a callback for libshcodecs to call when it has decoded
 * YUV 4:2:0 data. To pause decoding, return 1 from this callback.
 * \param decoder The SHCodecs_Decoder* handle
 * \param y_buf The decoded Y plane
 * \param y_size The size in bytes of the decoded Y data
 * \param c_buf The decoded C plane
 * \param c_size The size in bytes of the decoded C data
 * \param user_data Arbitrary data supplied by user
 * \retval 0 Continue decoding
 * \retval 1 Pause decoding, return from shcodecs_decode()
 */
typedef int (*SHCodecs_Decoded_Callback) (SHCodecs_Decoder * decoder,
                                         unsigned char * y_buf, int y_size,
                                         unsigned char * c_buf, int c_size,
                                         void * user_data);
/**
 * Initialize the VPU4 for decoding a given video format.
 * \param width The video image width
 * \param height The video image height
 * \param format SHCodecs_Format_MPEG4 or SHCODECS_Format_H264
 * \return decoder The SHCodecs_Decoder* handle
 */
SHCodecs_Decoder *
shcodecs_decoder_init(int width, int height, SHCodecs_Format format);

/**
 * Deallocate resources used to initialize the VPU4, and
 * reset it for future use.
 * \param decoder The SHCodecs_Decoder* handle
 */
void
shcodecs_decoder_close (SHCodecs_Decoder * decoder);

/**
 * Set the callback for libshcodecs to call when decoded data is available.
 * \param decoder The SHCodecs_Decoder* handle
 * \param decodec_cb The callback function
 * \param user_data Additional data to pass to the callback function
 */
int
shcodecs_decoder_set_decoded_callback (SHCodecs_Decoder * decoder,
                                      SHCodecs_Decoded_Callback decoded_cb,
                                      void * user_data);

/**
 * Set the data input mode for frame-by-frame input, or for continuous
 * data streaming input. If the calling application does its own packet
 * demuxing and passes individiual packets to shcodecs, then set this
 * function. Otherwise, arbitrary data can be streamed in and shcodecs
 * will scan for packet boundaries.
 * \param decoder The SHCodecs_Decoder* handle
 * \param frame_by_frame Flag: Input data is passed in one frame blocks
 * if set to a non-zero value.
 * \retval 0 Success
 */
int
shcodecs_decoder_set_frame_by_frame (SHCodecs_Decoder * decoder,
                                     int frame_by_frame);

/**
 * Set the data output mode to use physical addresses.
 * If the calling application is interfacing to other IP blocks such as
 * the VEU, then set this function. Otherwise, decoded frame output will
 * be mapped to userspace addresses, usable by normal applications.
 * \param decoder The SHCodecs_Decoder* handle
 * \param use_physical Flag: Physical addresses will be reported for
 * output frame data if set to a non-zero value.
 * \retval 0 Success
 */
int
shcodecs_decoder_set_use_physical (SHCodecs_Decoder * decoder,
                                   int use_physical);

/**
 * Decode a buffer of input data. This function will call the previously
 * registered callback each time it has decoded a complete frame. If that
 * callback returns 1, decoding is paused and shcodecs_decode() will
 * return immediately. The decode state will be retained between successive
 * calls.
 * \param decoder The SHCodecs_Decoder* handle
 * \param data A memory buffer containing compressed video data
 * \param len The length in bytes of the data
 * \returns The number of bytes of input that were used. Note that this
 * may be zero even if frames were decoded, in the case that the decoder
 * was previously paused and is being resumed.
 */
int
shcodecs_decode (SHCodecs_Decoder * decoder, unsigned char * data, int len);

/**
 * Finalize decoding of a stream. This will flush any partial decode state,
 * which may result in a final frame being extracted. The previously
 * registered callback will be called if a complete frame is decoded.
 * Note that previously generated frames will be referenced, so do not
 * free any of the image buffers you were using for shcodecs_decode()
 * before calling this function.
 *
 * finalize() generates the final frames after all input data has been read
 * in. The reason the call exists is that the encoded data order is not
 * necessarily the same as presentation order, so the frames are re-ordered.
 * finalize() signals to the decoder that the re-ordering buffer should be
 * flushed.
 *
 * The encoded data for most video frames is a description of the difference
 * between this and previous video images, which is why it needs to
 * reference the previously generated frames (both during normal decode and
 * during finalization).
 *
 * \param decoder The SHCodecs_Decoder* handle
 * \returns The number of final frames extracted by this call
 */
int
shcodecs_decoder_finalize (SHCodecs_Decoder * decoder);

/**
 * Retrieve the count of decoded frames.
 * \param decoder The SHCodecs_Decoder* handle
 * \returns The total number of frames decoded by \a decoder
 */
int
shcodecs_decoder_get_frame_count (SHCodecs_Decoder * decoder);

#endif /* __SHCODECS_DECODER_H__ */
