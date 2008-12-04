/*
  Copyright (C) Renesas Technology Corp., 2003-2005. All rights reserved.
 */

#ifndef __SHCODECS_DECODER_H__
#define __SHCODECS_DECODER_H__

/** \file
 *
 * Decoder functions.
 */

/**
 * An opaque handle to the VPU4 for decoding.
 */
typedef void SHCodecs_Decoder;

/**
 * Video formats which can be decoded by the VPU4
 */
typedef enum {
    SHCodecs_Format_NONE = 0,
    SHCodecs_Format_MPEG4 = 1,
    SHCodecs_Format_H264  = 2
} SHCodecs_Format;

/** Minimum frame width */
#define	SHCODECS_MIN_FX		48

/** Maximum frame width */
#define	SHCODECS_MAX_FX		640

/** Minimum frame height */
#define	SHCODECS_MIN_FY		48

/** Maximum frame height */
#define	SHCODECS_MAX_FY		480

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
 * \retval 0 Success
 */
int
shcodecs_decoder_set_frame_by_frame (SHCodecs_Decoder * decoder,
                                     int frame_by_frame);

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
 * \param decoder The SHCodecs_Decoder* handle
 * \returns The number of final frames extracted
 */
int
shcodecs_decode_finalize (SHCodecs_Decoder * decoder);

#endif /* __SHCODECS_DECODER_H__ */
