## Process this file with automake to produce Makefile.in

INCLUDES = $(INCLTDL) \
           -I$(top_builddir) \
           -I$(top_srcdir)/include

EXTRA_DIST = Version_script.in

AM_CFLAGS = -DVPU4 -DDISABLE_INT2 -D_LIT -DANNEX_B
#AM_CFLAGS = -DANNEX_B

# Libraries to build
lib_LTLIBRARIES = libshcodecs.la

noinst_HEADERS = \
	avcbd.h \
	avcbd_inner_typedef.h \
	avcbd_optionaldata.h \
	m4driverif.h \
	m4iph_vpu4.h \
	m4vsd_h263dec.h \
	Mpeg4vdec.h \
	decoder_private.h \
	sh7722_vpu.h

libshcodecs_la_SOURCES = \
	m4driverif.c \
	shcodecs_decoder.c

libshcodecs_la_LDFLAGS = -version-info @SHARED_VERSION_INFO@ @SHLIB_VERSION_ARG@
libshcodecs_la_LIBADD = $(VPU4_DEC_LIBS) -lm
