## Process this file with automake to produce Makefile.in

INCLUDES = $(INCLTDL) \
           -I$(top_builddir) \
           -I$(top_srcdir)/include

EXTRA_DIST = Version_script.in

lib_LTLIBRARIES = libshcodecs.la

noinst_HEADERS = \
	avcbd.h \
	avcbd_inner_typedef.h \
	avcbd_optionaldata.h \
	avcbe_global.h \
	avcbe.h \
	avcbe_inner.h \
	encoder_common.h \
	encoder_private.h \
	m4driverif.h \
	m4iph_vpu4.h \
	m4vsd_h263dec.h \
	m4vse_api_sub.h \
	QuantMatrix.h \
	Mpeg4vdec.h \
	decoder_private.h \
	sh7722_vpu.h

libshcodecs_la_SOURCES = \
	m4driverif.c \
	shcodecs_decoder.c \
	shcodecs_encoder.c \
	encoder_common.c \
	general_accessors.c \
	h264_accessors.c \
	mpeg4_accessors.c \
	property_accessors.c \
	h264_encode.c \
	mpeg4_encode.c \
	QuantMatrix.c

libshcodecs_la_CFLAGS = -DSH -D_LIT -DVPU4=1 -DVPU3IP -DVPU4IP -DANNEX_B
libshcodecs_la_LDFLAGS = -version-info @SHARED_VERSION_INFO@ @SHLIB_VERSION_ARG@
libshcodecs_la_LIBADD = -lstdc++ $(VPU4_DEC_LIBS) $(VPU4_ENC_LIBS) -lm
