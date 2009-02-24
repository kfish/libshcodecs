/*
  Copyright (C) Renesas Technology Corp., 2003-2005. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include "m4driverif.h"
#include "m4iph_vpu4.h"	/* SuperH MEPG-4&H.264 Video Driver Library Header */  

#include "encoder_private.h"

/**
 * Initialize the VPU4 for encoding a given video format.
 * \param width The video image width
 * \param height The video image height
 * \param format SHCodecs_Format_MPEG4 or SHCODECS_Format_H264
 * \return encoder The SHCodecs_Encoder* handle
 */
SHCodecs_Encoder *
shcodecs_encoder_init(int width, int height, SHCodecs_Format format)
{
        SHCodecs_Encoder * encoder;

        encoder = malloc (sizeof (SHCodecs_Encoder));

        m4iph_vpu_open();
	m4iph_sdr_open();

	encoder->width = width;
	encoder->height = height;
	encoder->format = format;

        encoder->input = NULL;
        encoder->output = NULL;

	m4iph_sleep_time_init();

        return encoder;
}

/**
 * Deallocate resources used to initialize the VPU4 for encoding,
 * and reset it for future use.
 * \param encoder The SHCodecs_Encoder* handle
 */
void
shcodecs_encoder_close (SHCodecs_Encoder * encoder)
{
	m4iph_sdr_close();
	m4iph_vpu_close();

  if (encoder) free (encoder);

  return;
}

/**
 * Set the callback for libshcodecs to call when raw YUV data is required.
 * \param encoder The SHCodecs_Encoder* handle
 * \param get_input_cb The callback function
 * \param user_data Additional data to pass to the callback function
 */
int
shcodecs_encoder_set_input_callback (SHCodecs_Encoder * encoder,
                                     SHCodecs_Encoder_Input input_cb,
                                     void * user_data)
{
  encoder->input = input_cb;
  encoder->input_user_data = user_data;

  return 0;
}

/**
 * Set the callback for libshcodecs to call when encoded data is available.
 * \param encoder The SHCodecs_Encoder* handle
 * \param encodec_cb The callback function
 * \param user_data Additional data to pass to the callback function
 */
int
shcodecs_encoder_set_output_callback (SHCodecs_Encoder * encoder,
                                      SHCodecs_Encoder_Output output_cb,
                                      void * user_data)
{
  encoder->output = output_cb;
  encoder->output_user_data = user_data;

  return 0;
}

/**
 * Run the encoder.
 * \param encoder The SHCodecs_Encoder* handle
 * \retval 0 Success
 */
int
shcodecs_encoder_run (SHCodecs_Encoder * encoder, APPLI_INFO * ainfo)
{
#if 1
	if (encoder->format == AVCBE_H264 ) {
		return encode_1file_h264(encoder, ainfo->case_no, ainfo, AVCBE_H264);   
	} else {
		return encode_1file_mpeg4(encoder, ainfo->case_no, ainfo, encoder->format);
	}
#else
        return 0;
#endif
}
