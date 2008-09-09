## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

RSOVPU4DIR = ../librsovpu4
RSOVPU4_LIBS = $(RSOVPU4DIR)/librsovpu4.la -lm

#AM_CFLAGS = -v -Wall -O2 -I $(SRC_PATH) -fPIC -fno-common -m4-nofpu

bin_PROGRAMS = rsovpu4enc rsovpu4capenc rsovpu4cap

noinst_HEADERS = \
	capture.h \
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
	VPU4EncDef.h \
	veu_colorspace.h

rsovpu4enc_SOURCES =  \
	capture.c \
	avcbeinputuser.c \
	avcbencsmp_common.c \
	avcbencsmp_h264.c \
	avcbencsmp_mpeg4.c \
	ControlFileUtil.c \
	m4driverif.c \
	QuantMatrix.c \
	veu_colorspace.c

rsovpu4enc_CFLAGS = -v -Wall -O2 -I $(srcdir) -fPIC -fno-common \
	-DNDEBUG -D_LIT -DSH=1 -DVPU4=1 \
	-DAZASM=1 -DCACHE_PURGE=1 -DCOEFF_INTERMEM=1 -DFPGA=1 \
	-DVPU_CBP_USED=1 -DVPU2_V20=1 -DSLEEP_ONCE=1 -DVPU=1 \
	-DBS_FLUSH=1 -DENCOCE_ENABLE=1 -DTEL_WITHOUT_DISPLAY=1 \
	-DSMAP=1 -DRC_TM5_ENABLE=1 -DVPU2=1 -DVPU3=1 -DFPGA2=1 \
	-DVPU_FILEOUT=1 -DTEL_TRACE_LEVEL2=1 -DSH_X=1 -DLINUX_VPU=1CFLAGS \
	-DVPU3IP -DVPU4IP -DMPEG4SP -DVPU4S

rsovpu4enc_LDADD = -lstdc++ $(RSOVPU4_LIBS) $(VPU4_ENC_LIBS)

rsovpu4capenc_SOURCES =  \
	capture.c \
	avcbeinputuser.c \
	avcbencsmp_common.c \
	avcbencsmp_h264.c \
	avcbencsmp_mpeg4-capture.c \
	ControlFileUtil.c \
	m4driverif.c \
	QuantMatrix.c \
	veu_colorspace.c

rsovpu4capenc_CFLAGS = -v -Wall -O2 -I $(srcdir) -fPIC -fno-common \
	-DNDEBUG -D_LIT -DSH=1 -DVPU4=1 \
	-DAZASM=1 -DCACHE_PURGE=1 -DCOEFF_INTERMEM=1 -DFPGA=1 \
	-DVPU_CBP_USED=1 -DVPU2_V20=1 -DSLEEP_ONCE=1 -DVPU=1 \
	-DBS_FLUSH=1 -DENCOCE_ENABLE=1 -DTEL_WITHOUT_DISPLAY=1 \
	-DSMAP=1 -DRC_TM5_ENABLE=1 -DVPU2=1 -DVPU3=1 -DFPGA2=1 \
	-DVPU_FILEOUT=1 -DTEL_TRACE_LEVEL2=1 -DSH_X=1 -DLINUX_VPU=1CFLAGS \
	-DVPU3IP -DVPU4IP -DMPEG4SP -DVPU4S

rsovpu4capenc_LDADD = -lstdc++ $(RSOVPU4_LIBS) $(VPU4_ENC_LIBS)

rsovpu4cap_SOURCES = capture.c rsovpu4cap.c

