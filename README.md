libshcodecs
===========

libshcodecs: A library for controlling SH-Mobile hardware codecs

Copyright (C) 2009 Renesas Technology Corp.

The [SH-Mobile][0] processor series includes a hardware video processing
unit that supports MPEG-4 and H.264 encoding and decoding.

This source archive contains:

 * src/libshcodecs: the libshcodecs shared library
 * src/examples: various examples of simple uses of libshcodecs
 * src/tools: commandline tools

libshcodecs
-----------

libshcodecs provides callback-based interfaces for encoding and decoding
video streams.

To decode video data, an application provides a callback with the following
prototype:

    int vpu4_decoded (SHCodecs_Decoder * decoder,
                      unsigned char * y_buf, int y_size,
                      unsigned char * c_buf, int c_size,
                      void * user_data);

and then supplies encoded video data to the function:

    shcodecs_decode (decoder, input_buffer, length);

The decoder will process the input buffer, and call the provided callback
function each time a frame is decoded. The output is given in two bitplanes
of YUV 4:2:0.

For a full decoder example, see src/examples/shcodecs-dec.c

To encode video data, an application provides both input and output callback
functions:

    /* SHCodecs_Encoder_Input callback for acquiring an image */
    int get_input(SHCodecs_Encoder * encoder, void *user_data);
    
    /* SHCodecs_Encoder_Output callback for writing encoded data */
    int write_output(SHCodecs_Encoder * encoder,
                     unsigned char *data, int length, void *user_data);

and then run the encoder by:

    shcodecs_encoder_run(encoder);

The encoder will retrieve raw video as needed using the input callback, and will
use the output callback you have provided whenever encoded video data is
available.

For examples of encoding from file and camera, see src/tools/shcodecs-enc.c
and src/tools/shcodecs-capenc.c.

SH-Mobile
---------

The [SH-Mobile][0] processor series includes a hardware video processing
unit that supports MPEG-4 and H.264 encoding and decoding. Some models
also include support for camera capture, JPEG processing, and DSP
instructions.

[0]: http://www.renesas.com/fmwk.jsp?cnt=sh_mobile_family_landing.jsp&fp=/products/mpumcu/sh_mobile/

License
-------

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA  02110-1301 USA

See the file COPYING for details.
