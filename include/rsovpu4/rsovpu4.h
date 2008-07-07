
#ifndef __RSOVPU4_H__
#define __RSOVPU4_H__

#ifdef __cplusplus
extern "C" {
#endif

/** \mainpage
 *
 * \section intro RSOVPU4: A library for accessing the VPU4
 *
 * This is the documentation for the RSOVPU4 C API.
 *
 * Features:
 *  - Simple callback-based decoding
 * 
 * \subsection contents Contents
 * 
 * - \link rsovpu4.h rsovpu4.h \endlink:
 * Documentation of the UIOMux C API
 *
 * - \link configuration Configuration \endlink:
 * Customizing librsovpu4 to only read or write.
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
 * It is possible to customize the functionality of librsovpu4
 * by using various ./configure flags when
 * building it from source.
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

/** \defgroup building Building against librsovpu4
 *
 *
 * \section autoconf Using GNU autoconf
 *
 * If you are using GNU autoconf, you do not need to call pkg-config
 * directly. Use the following macro to determine if librsovpu4 is
 * available:
 *
 <pre>
 PKG_CHECK_MODULES(RSOVPU4, rsovpu4 >= 0.5.0,
                   HAVE_RSOVPU4="yes", HAVE_RSOVPU4="no")
 if test "x$HAVE_RSOVPU4" = "xyes" ; then
   AC_SUBST(RSOVPU4_CFLAGS)
   AC_SUBST(RSOVPU4_LIBS)
 fi
 </pre>
 *
 * If librsovpu4 is found, HAVE_RSOVPU4 will be set to "yes", and
 * the autoconf variables RSOVPU4_CFLAGS and RSOVPU4_LIBS will
 * be set appropriately.
 *
 * \section pkg-config Determining compiler options with pkg-config
 *
 * If you are not using GNU autoconf in your project, you can use the
 * pkg-config tool directly to determine the correct compiler options.
 *
 <pre>
 RSOVPU4_CFLAGS=`pkg-config --cflags rsovpu4`

 RSOVPU4_LIBS=`pkg-config --libs rsovpu4`
 </pre>
 *
 */

/** \file
 * The librsovpu4 C API.
 *
 */

#include <rsovpu4/rsovpu4_decoder.h>

#ifdef __cplusplus
}
#endif

#endif /* __RSOVPU4_H__ */

