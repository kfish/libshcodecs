/*
  Copyright (C) Renesas Technology Corp., 2003-2005. All rights reserved.
 */

#ifndef __RSOVPU4_DECODER_H__
#define __RSOVPU4_DECODER_H__

typedef void RSOVPU4_Decoder;

typedef enum {
    RSOVPU4_Format_NONE = 0,
    RSOVPU4_Format_MPEG4 = 1,
    RSOVPU4_Format_H264  = 2
} RSOVPU4_Format;

#define	RSOVPU4_MIN_FX		48
#define	RSOVPU4_MAX_FX		320
#define	RSOVPU4_MIN_FY		48
#define	RSOVPU4_MAX_FY		240

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
 * init ()
 */
RSOVPU4_Decoder *
rsovpu4_decoder_init(int width, int height, RSOVPU4_Format format);

/**
 * close ()
 */
void
rsovpu4_decoder_close (RSOVPU4_Decoder * decoder);

/**
 * Set the callback for librsovpu4 to call when decoded data is available
 */
int
rsovpu4_decoder_set_decoded_callback (RSOVPU4_Decoder * decoder,
                                      RSOVPU4_Decoded_Callback * decoded_cb,
                                      void * user_data);

/**
 * Query the optimal input length in bytes
 */
int
rsovpu4_decoder_preferred_length (RSOVPU4_Decoder * decoder);

/**
 * Decode
 */
int
rsovpu4_decode (RSOVPU4_Decoder * decoder, unsigned char * data, int len);


#endif /* __RSOVPU4_DECODER_H__ */
