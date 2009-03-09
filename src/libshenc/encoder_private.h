#ifndef __ENCODER_PRIVATE_H__
#define __ENCODER_PRIVATE_H__

#include <shcodecs/shcodecs_common.h>

#include "avcbe.h"
#include "avcbe_inner.h"
#include "m4iph_vpu4.h"

#include "encoder_common.h"

/**
 * An opaque handle to the VPU4 for encoding.
 */
typedef struct _SHCodecs_Encoder SHCodecs_Encoder;

/**
 * Signature of a callback for libshcodecs to call when it requires YUV 4:2:0 data.
 * To pause encoding, return 1 from this callback.
 * \param encoder The SHCodecs_Encoder* handle
 * \param user_data Arbitrary data supplied by user
 * \retval 0 Continue encoding
 * \retval 1 Pause encoding, return from shcodecs_encode()
 */
typedef int (*SHCodecs_Encoder_Input) (SHCodecs_Encoder * encoder, void *user_data);

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

typedef struct {		/* add at Version2 */
	long weightdQ_enable;
	TAVCBE_WEIGHTEDQ_CENTER weightedQ_info_center;	/* API´Ø¿ôavcbe_set_weightedQ()¤ËÅÏ¤¹¤¿¤á¤Î¹½Â¤ÂÎ(1) */
	TAVCBE_WEIGHTEDQ_RECT weightedQ_info_rect;	/* API´Ø¿ôavcbe_set_weightedQ()¤ËÅÏ¤¹¤¿¤á¤Î¹½Â¤ÂÎ(2) */
	TAVCBE_WEIGHTEDQ_USER weightedQ_info_user;	/* API´Ø¿ôavcbe_set_weightedQ()¤ËÅÏ¤¹¤¿¤á¤Î¹½Â¤ÂÎ(3) */
	char weightedQ_table_filepath[256];	/* ½Å¤ßÉÕ¤±¥Æ¡¼¥Ö¥ë¥Õ¥¡¥¤¥ë¤Î¥Ñ¥¹Ì¾ */

	/* Table to set encoding parameter (for H.264 bitstream) */
	avcbe_vui_main_param vui_main_param;	/* the parameter of the avcbe_set_VUI_parameters function */

	char out_buffering_period_SEI;	/* whether output buffering_period SEI message (1:output, 2:NOT outputj */
	char out_pic_timing_SEI;	/* whether output picture_timing SEI message (1:output, 2:NOT outputj */
	char out_pan_scan_rect_SEI;	/* whether output filler_payload SEI message (1:output, 2:NOT outputj */
	char out_filler_payload_SEI;	/* whether output picture_timing SEI message (1:output, 2:NOT outputj */
	char out_recovery_point_SEI;	/* whether output recovery_point SEI message (1:output, 2:NOT outputj */
	char out_dec_ref_pic_marking_repetition_SEI;	/* whether output dec_ref_pic_marking_repetition SEI message (1:output, 2:NOT outputj */

	avcbe_sei_buffering_period_param sei_buffering_period_param;	/* the parameter of the avcbe_put_SEI_parameters function (1) */
	avcbe_sei_pic_timing_param sei_pic_timing_param;	/* the parameter of the avcbe_put_SEI_parameters function (2) */
	avcbe_sei_pan_scan_rect_param sei_pan_scan_rect_param;	/* the parameter of the avcbe_put_SEI_parameters function (3) */
	avcbe_sei_filler_payload_param sei_filler_payload_param;	/* the parameter of the avcbe_put_SEI_parameters function (4) */
	avcbe_sei_recovery_point_param sei_recovery_point_param;	/* the parameter of the avcbe_put_SEI_parameters function (5) */
} OTHER_API_ENC_PARAM;

struct _SHCodecs_Encoder {
	int width;
	int height;

	SHCodecs_Format format;

	SHCodecs_Encoder_Input input;
	void *input_user_data;

	SHCodecs_Encoder_Output output;
	void *output_user_data;

        /* Internal encode error tracking */
        long error_return_function;	/* ID of the API function when error ocuured *//* add at Version2 */
        long error_return_code;	/* return_value of the API function when error ocuured *//* add at Version2 */

	/* Internal */
        int initialized; /* Is avcbe_encode_init() done? */
	int y_bytes; /* Bytes used by Y input plane; CbCr plane uses y_bytes/2 */
        unsigned long * addr_y; /* VPU address to write next Y plane; updated by encoder backends */
        unsigned long * addr_c; /* VPU address to write next C plane; updated by encoder backends */

	avcbe_stream_info *stream_info;
	long frm; /* Current frame */
	long frame_skip_num; /* Number of frames skipped */
	long frame_counter; /* The number of encoded frames */
	long set_intra;	/* Forced intra-mode flag for m4vse_encode_picture function */
	long output_type;	/* Header insertion flag for m4vse_encode_picture function */

	/* Working values */
	TAVCBE_FMEM LDEC_ARRY[3];	/* Always set the head of STREAMAREA section! */
	TAVCBE_FMEM CAPTF_ARRY[3];

	unsigned long *my_frame_memory_capt[19];
	unsigned long *my_frame_memory_ldec1;
	unsigned long *my_frame_memory_ldec2;
	unsigned long *my_frame_memory_ldec3;

	/* General encoder internals (general_accessors.c) */
	long frame_number_to_encode;
	long frame_no_increment;	/* Increment value of Frame number to be encoded for 
					   m4vse_encode_picture function */
	/* encoding parameters */
	avcbe_encoding_property encoding_property;
	avcbe_encoding_property paramR;	/* for stream-1 */
	M4IPH_VPU4_INIT_OPTION vpu4_param;	/* parameters of VPU4 */
	OTHER_API_ENC_PARAM other_API_enc_param;

        /* MPEG-4 specific internals */
	avcbe_other_options_mpeg4 other_options_mpeg4;	/* parameters to control details */

        /* H.264 specific internals */
	long slice_total_size;
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

/* Other internal functions */
void m4iph_sleep_time_init(void);

#endif				/* __ENCODER_PRIVATE_H__ */
