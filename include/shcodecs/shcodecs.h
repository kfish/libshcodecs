
#ifndef __SHCODECS_H__
#define __SHCODECS_H__

#ifdef __cplusplus
extern "C" {
#endif

/** \mainpage
 *
 * \section intro SHCodecs: A library for accessing the VPU4
 *
 * This is the documentation for the SHCodecs C API.
 *
 * Features:
 *  - Simple callback-based encoding and decoding
 * 
 * \subsection contents Contents
 * 
 * - \link shcodecs.h shcodecs.h \endlink:
 * Documentation of the SHCodecs C API
 *
 * - \link configuration Configuration \endlink:
 * Customizing libshcodecs to only read or write.
 *
 * - \link building Building \endlink:
 * \section API
 * 
 * The API is modelled on a conventional open/read/write/close
 * file descriptor interface.
 * 
 */

/** \defgroup configuration Configuration
 * \section configure ./configure
 *
 * It is possible to customize the functionality of libshcodecs
 * by using various ./configure flags when building it from source.
 *
 * For general information about using ./configure, see the file
 * \link install INSTALL \endlink
 *
 */

/** \defgroup install Installation
 * \section install INSTALL
 *
 * \include INSTALL
 */

/** \defgroup building Building against libshcodecs
 *
 *
 * \section autoconf Using GNU autoconf
 *
 * If you are using GNU autoconf, you do not need to call pkg-config
 * directly. Use the following macro to determine if libshcodecs is
 * available:
 *
 <pre>
 PKG_CHECK_MODULES(SHCODECS, shcodecs >= 0.5.0,
                   HAVE_SHCODECS="yes", HAVE_SHCODECS="no")
 if test "x$HAVE_SHCODECS" = "xyes" ; then
   AC_SUBST(SHCODECS_CFLAGS)
   AC_SUBST(SHCODECS_LIBS)
 fi
 </pre>
 *
 * If libshcodecs is found, HAVE_SHCODECS will be set to "yes", and
 * the autoconf variables SHCODECS_CFLAGS and SHCODECS_LIBS will
 * be set appropriately.
 *
 * \section pkg-config Determining compiler options with pkg-config
 *
 * If you are not using GNU autoconf in your project, you can use the
 * pkg-config tool directly to determine the correct compiler options.
 *
 <pre>
 SHCODECS_CFLAGS=`pkg-config --cflags shcodecs`

 SHCODECS_LIBS=`pkg-config --libs shcodecs`
 </pre>
 *
 */

/** \file
 * The libshcodecs C API.
 *
 */

#include <shcodecs/shcodecs_decoder.h>
#include <shcodecs/shcodecs_encoder.h>

#ifdef __cplusplus
}
#endif

#endif /* __SHCODECS_H__ */

