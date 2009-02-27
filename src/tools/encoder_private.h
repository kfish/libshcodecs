#ifndef __ENCODER_PRIVATE_H__
#define __ENCODER_PRIVATE_H__

#include <shcodecs/shcodecs_common.h>

#include "avcbe.h"
#include "avcbencsmp.h"		/* XXX: for APPLI_INFO */

/**
 * An opaque handle to the VPU4 for encoding.
 */
typedef struct _SHCodecs_Encoder SHCodecs_Encoder;

/**
 * Signature of a callback for libshcodecs to call when it requires YUV 4:2:0 data.
 * To pause encoding, return 1 from this callback.
 * \param encoder The SHCodecs_Encoder* handle
 * \param addr_y Address in which to write Y data
 * \param addr_c Address in which to write CbCr data
 * \param user_data Arbitrary data supplied by user
 * \retval 0 Continue encoding
 * \retval 1 Pause encoding, return from shcodecs_encode()
 */
typedef int (*SHCodecs_Encoder_Input) (SHCodecs_Encoder * encoder,
				       unsigned long *addr_y,
				       unsigned long *addr_c,
				       void *user_data);

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
					unsigned char *data, int length,
					void *user_data);

struct _SHCodecs_Encoder {
	int width;
	int height;
	SHCodecs_Format format;

	SHCodecs_Encoder_Input input;
	void *input_user_data;

	SHCodecs_Encoder_Output output;
	void *output_user_data;

	/* Encoder internals */
	avcbe_encoding_property paramR;	/* for stream-1 */
	M4IPH_VPU4_INIT_OPTION vpu4_param;	/* parameters of VPU4 */
	OTHER_API_ENC_PARAM other_API_enc_param;
	avcbe_other_options_mpeg4 other_options_mpeg4;	/* parameters to control details */
};

/* Internal prototypes of functions using SHCodecs_Encoder */

int encode_1file_h264(SHCodecs_Encoder * encoder, APPLI_INFO * appli_info,
		      long stream_type);
long init_for_encoder_h264(SHCodecs_Encoder * encoder,
			   APPLI_INFO * appli_info, long stream_type,
			   avcbe_stream_info ** context);
long encode_picture_unit(SHCodecs_Encoder * encoder,
			 APPLI_INFO * appli_info, long stream_type,
			 avcbe_stream_info * context);
long encode_nal_unit(SHCodecs_Encoder * encoder, APPLI_INFO * appli_info,
		     long stream_type, avcbe_stream_info * context);
long output_SEI_parameters(SHCodecs_Encoder * encoder,
			   APPLI_INFO * appli_info,
			   avcbe_stream_info * context,
			   TAVCBE_STREAM_BUFF * stream_buff_info);

int encode_1file_mpeg4(SHCodecs_Encoder * encoder, APPLI_INFO * appli_info,
		       long stream_type);
long init_for_encoder_mpeg4(SHCodecs_Encoder * encoder,
			    APPLI_INFO * appli_info, long stream_type,
			    avcbe_stream_info ** context);
long encode_picture_for_mpeg4(SHCodecs_Encoder * encoder,
			      APPLI_INFO * appli_info, long stream_type,
			      avcbe_stream_info * context);

/* XXX: Move back out of encoder_private.h to appli code */
int select_inputfile_set_param(SHCodecs_Encoder * encoder,
			       APPLI_INFO * appli_info);

/* Other internal functions */
void m4iph_sleep_time_init(void);

#endif				/* __ENCODER_PRIVATE_H__ */
