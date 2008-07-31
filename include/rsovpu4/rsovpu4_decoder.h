/*
  Copyright (C) Renesas Technology Corp., 2003-2005. All rights reserved.
 */

#ifndef __RSOVPU4_DECODER_H__
#define __RSOVPU4_DECODER_H__

/** \file
 *
 * Decoder functions.
 */

/**
 * An opaque handle to the VPU4 for decoding.
 */
typedef void RSOVPU4_Decoder;

/**
 * Video formats which can be decoded by the VPU4
 */
typedef enum {
    RSOVPU4_Format_NONE = 0,
    RSOVPU4_Format_MPEG4 = 1,
    RSOVPU4_Format_H264  = 2
} RSOVPU4_Format;

/** Minimum frame width */
#define	RSOVPU4_MIN_FX		48

/** Maximum frame width */
#define	RSOVPU4_MAX_FX		640

/** Minimum frame height */
#define	RSOVPU4_MIN_FY		48

/** Maximum frame height */
#define	RSOVPU4_MAX_FY		480

/**
 * Signature of a callback for librsovpu4 to call when it has decoded
 * YUV 4:2:0 data.
 * \param decoder The RSOVPU4_Decoder* handle
 * \param y_buf The decoded Y plane
 * \param y_size The size in bytes of the decoded Y data
 * \param c_buf The decoded C plane
 * \param c_size The size in bytes of the decoded C data
 * \param user_data Arbitrary data supplied by user
 */
typedef int (*RSOVPU4_Decoded_Callback) (RSOVPU4_Decoder * decoder,
                                         unsigned char * y_buf, int y_size,
                                         unsigned char * c_buf, int c_size,
                                         void * user_data);
/**
 * Initialize the VPU4 for decoding a given video format.
 * \param width The video image width
 * \param height The video image height
 * \param format RSOVPU4_Format_MPEG4 or RSOVPU4_Format_H264
 * \return decoder The RSOVPU4_Decoder* handle
 */
RSOVPU4_Decoder *
rsovpu4_decoder_init(int width, int height, RSOVPU4_Format format);

/**
 * Deallocate resources used to initialize the VPU4, and
 * reset it for future use.
 * \param decoder The RSOVPU4_Decoder* handle
 */
void
rsovpu4_decoder_close (RSOVPU4_Decoder * decoder);

/**
 * Set the callback for librsovpu4 to call when decoded data is available.
 * \param decoder The RSOVPU4_Decoder* handle
 * \param decodec_cb The callback function
 * \param user_data Additional data to pass to the callback function
 */
int
rsovpu4_decoder_set_decoded_callback (RSOVPU4_Decoder * decoder,
                                      RSOVPU4_Decoded_Callback decoded_cb,
                                      void * user_data);

/**
 * Query the optimal input length in bytes.
 * \param decoder The RSOVPU4_Decoder* handle
 * \returns The optimal input length in bytes.
 */
int
rsovpu4_decoder_preferred_length (RSOVPU4_Decoder * decoder);

/**
 * Decode a buffer of input data. This function will call the previously
 * registered callback each time it has decoded a complete frame.
 * \param decoder The RSOVPU4_Decoder* handle
 * \param data A memory buffer containing comperssed video data
 * \param len The length in bytes of the data
 */
int
rsovpu4_decode (RSOVPU4_Decoder * decoder, unsigned char * data, int len);


#endif /* __RSOVPU4_DECODER_H__ */
