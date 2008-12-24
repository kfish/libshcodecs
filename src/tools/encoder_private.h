#include <shcodecs/shcodecs_common.h>

/**
 * An opaque handle to the VPU4 for encoding.
 */
typedef void SHCodecs_Encoder;

/**
 * Signature of a callback for libshcodecs to call when it has encoded
 * YUV 4:2:0 data. To pause encoding, return 1 from this callback.
 * \param encoder The SHCodecs_Encoder* handle
 * \param data The encoded data
 * \param length Length of encoded data in bytes
 * \param user_data Arbitrary data supplied by user
 * \retval 0 Continue encoding
 * \retval 1 Pause encoding, return from shcodecs_encode()
 */
typedef int (*SHCodecs_Encoded_Callback) (SHCodecs_Encoder * encoder,
                                          unsigned char * data, int length,
                                          void * user_data);
