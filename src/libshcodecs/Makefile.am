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

libshcodecs_la_CFLAGS = -v -Wall -O2 -I $(srcdir) -fPIC -fno-common \
	-DNDEBUG -D_LIT -DSH=1 -DVPU4=1 \
	-DAZASM=1 -DCACHE_PURGE=1 -DCOEFF_INTERMEM=1 -DFPGA=1 \
	-DVPU_CBP_USED=1 -DVPU2_V20=1 -DSLEEP_ONCE=1 -DVPU=1 \
	-DBS_FLUSH=1 -DENCOCE_ENABLE=1 -DTEL_WITHOUT_DISPLAY=1 \
	-DSMAP=1 -DRC_TM5_ENABLE=1 -DVPU2=1 -DVPU3=1 -DFPGA2=1 \
	-DVPU_FILEOUT=1 -DTEL_TRACE_LEVEL2=1 -DSH_X=1 -DLINUX_VPU=1CFLAGS \
	-DVPU3IP -DVPU4IP -DMPEG4SP -DVPU4S
libshcodecs_la_LDFLAGS = -version-info @SHARED_VERSION_INFO@ @SHLIB_VERSION_ARG@
libshcodecs_la_LIBADD = -lstdc++ $(VPU4_DEC_LIBS) $(VPU4_ENC_LIBS) -lm
