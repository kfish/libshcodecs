## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

RSOVPU4DIR = ../librsovpu4
RSOVPU4_LIBS = $(RSOVPU4DIR)/librsovpu4.la -lm

AM_CFLAGS = -v -Wall -O2 -I $(SRC_PATH) -fPIC -fno-common -m4-nofpu
AM_CFLAGS += -DVPU4 -DDISABLE_INT2 -D_LIT -DANNEX_B -D_DEBUG

AM_CFLAGS = -v -Wall -O2 -I $(SRC_PATH) -fPIC -fno-common -m4-nofpu
AM_CFLAGS += -DNDEBUG -D_LIT -DSH=1 -DVPU4=1^M
AM_CFLAGS += -DAZASM=1 -DCACHE_PURGE=1 -DCOEFF_INTERMEM=1 -DFPGA=1
AM_CFLAGS += -DVPU_CBP_USED=1 -DVPU2_V20=1 -DSLEEP_ONCE=1 -DVPU=1
AM_CFLAGS += -DBS_FLUSH=1 -DENCOCE_ENABLE=1 -DTEL_WITHOUT_DISPLAY=1
AM_CFLAGS += -DSMAP=1 -DRC_TM5_ENABLE=1 -DVPU2=1 -DVPU3=1 -DFPGA2=1
AM_CFLAGS += -DVPU_FILEOUT=1 -DTEL_TRACE_LEVEL2=1 -DSH_X=1 -DLINUX_VPU=1CFLAGS
AM_CFLAGS += -DVPU3IP -DVPU4IP -DMPEG4SP -DVPU4S


bin_PROGRAMS = rsovpu4capenc

noinst_HEADERS = \
	avcbd.h \
	avcbd_inner_typedef.h \
	avcbd_optionaldata.h \
	avcbe_global.h \
	avcbe.h \
	avcbe_inner.h \
	avcbencsmp.h \
	ControlFileUtil.h \
	m4driverif.h \
	m4iph_vpu4.h \
	m4vse_api_sub.h \
	QuantMatrix.h \
	sh7722_vpu.h \
	VPU4EncDef.h

rsovpu4capenc_SOURCES =  \
	avcbeinputuser.c \
	avcbencsmp_common.c \
	avcbencsmp_h264.c \
	avcbencsmp_mpeg4.c \
	ControlFileUtil.c \
	m4driverif.c \
	QuantMatrix.c

rsovpu4capenc_LDADD = -lstdc++ $(RSOVPU4_LIBS) $(VPU4_ENC_LIBS)
