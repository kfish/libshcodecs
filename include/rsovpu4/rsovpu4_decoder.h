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
