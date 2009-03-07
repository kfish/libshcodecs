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

        int initialized; /* Is avcbe_encode_init() done? */

        /* Internal encode error tracking */
        long error_return_function;	/* ID of the API function when error ocuured *//* add at Version2 */
        long error_return_code;	/* return_value of the API function when error ocuured *//* add at Version2 */

	/* Encoder internals */
	long frame_number_to_encode;
	long frm; /* Current frame */
	long frame_no_increment;	/* Increment value of Frame number to be encoded for 
					   m4vse_encode_picture function */
	long frame_skip_num; /* Number of frames skipped */
	long frame_counter; /* The number of encoded frames */
	long set_intra;	/* Forced intra-mode flag for m4vse_encode_picture function */
	long output_type;	/* Header insertion flag for m4vse_encode_picture function */

	/* encoding parameters */
	avcbe_encoding_property encoding_property;
	avcbe_encoding_property paramR;	/* for stream-1 */
	M4IPH_VPU4_INIT_OPTION vpu4_param;	/* parameters of VPU4 */
	OTHER_API_ENC_PARAM other_API_enc_param;

        /* MPEG-4 specific internals */
	avcbe_other_options_mpeg4 other_options_mpeg4;	/* parameters to control details */

        /* H.264 specific internals */
	avcbe_other_options_h264 other_options_h264;	/* parameters to control details */
	unsigned long mb_num_of_picture;	/* total number of mb in picture */
	unsigned long slice_mb_counter;	/* counter of mb_num_of_picture */
	long SPS_PPS_header_bytes;	/* bytes of SPS and PPS */
	unsigned char ref_frame_num;	/* »²¾È¥Õ¥ì¡¼¥à¿ô¡Ê1 or 2) (H.264¤Î¤ß¡Ë */
	long output_filler_enable;	/* enable flag to put Filler Data for CPB Buffer Over *//* 050519 */
	long output_filler_data;	/* for FillerData(CPB  Buffer) *//* add at Version2 */

};

/* Internal prototypes of functions using SHCodecs_Encoder */

int h264_encode_init  (SHCodecs_Encoder * encoder, long stream_type);
int h264_encode_run (SHCodecs_Encoder * encoder, long stream_type);

int mpeg4_encode_init (SHCodecs_Encoder * encoder, long stream_type);
int mpeg4_encode_run (SHCodecs_Encoder * encoder, long stream_type);

/* XXX: Move back out of encoder_private.h to appli code */
int select_inputfile_set_param(SHCodecs_Encoder * encoder,
			       APPLI_INFO * appli_info);

/* Other internal functions */
void m4iph_sleep_time_init(void);

#endif				/* __ENCODER_PRIVATE_H__ */
