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
 * YUV 4:2:0 data.
 * \param decoder The SHCodecs_Decoder* handle
 * \param y_buf The decoded Y plane
 * \param y_size The size in bytes of the decoded Y data
 * \param c_buf The decoded C plane
 * \param c_size The size in bytes of the decoded C data
 * \param user_data Arbitrary data supplied by user
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
 * Decode a buffer of input data. This function will call the previously
 * registered callback each time it has decoded a complete frame.
 * \param decoder The SHCodecs_Decoder* handle
 * \param data A memory buffer containing compressed video data
 * \param len The length in bytes of the data
 * \returns The number of bytes of input that were used.
 */
int
shcodecs_decode (SHCodecs_Decoder * decoder, unsigned char * data, int len);


#endif /* __SHCODECS_DECODER_H__ */
