## Process this file with automake to produce Makefile.in

INCLUDES = $(INCLTDL) \
           -I$(top_builddir) \
           -I$(top_srcdir)/include

EXTRA_DIST = Version_script.in

# Libraries to build
lib_LTLIBRARIES = librsovpu4.la

noinst_HEADERS = \
	avcbd.h \
	avcbd_inner_typedef.h \
	avcbd_optionaldata.h \
	m4driverif.h \
	m4iph_vpu4.h \
	m4vsd_h263dec.h \
	Mpeg4vdec.h \
	sample_dec.h \
	sh7722_vpu.h

librsovpu4_la_SOURCES = \
	m4driverif.c \
	rsovpu4_decoder.c

librsovpu4_la_LDFLAGS = -version-info @SHARED_VERSION_INFO@ @SHLIB_VERSION_ARG@
librsovpu4_la_LIBADD = $(VPU4_DEC_LIBS) -lm
