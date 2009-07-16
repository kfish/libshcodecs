## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

SHCODECSDIR = ../libshcodecs
SHCODECS_LIBS = $(SHCODECSDIR)/libshcodecs.la -lm

bin_PROGRAMS = shcodecs-dec shcodecs-enc shcodecs-capenc shcodecs-cap shcodecs-play

noinst_HEADERS = \
	avcbencsmp.h \
	capture.h \
	ControlFileUtil.h \
	veu_colorspace.h

shcodecs_dec_SOURCES = shcodecs-dec.c
shcodecs_dec_LDADD = $(SHCODECS_LIBS)

shcodecs_play_SOURCES = shcodecs-play.c veu_colorspace.c
shcodecs_play_LDADD = -lrt -lpthread $(SHCODECS_LIBS)

shcodecs_enc_SOURCES =  \
	shcodecs-enc.c \
	capture.c \
	veu_colorspace.c \
	ControlFileUtil.c \
	avcbeinputuser.c \
	avcbencsmp_common.c

shcodecs_enc_LDADD = $(SHCODECS_LIBS)

shcodecs_capenc_SOURCES =  \
	shcodecs-capenc.c \
	capture.c \
	veu_colorspace.c \
	ControlFileUtil.c \
	avcbeinputuser.c \
	avcbencsmp_common.c

shcodecs_capenc_LDADD = $(SHCODECS_LIBS)


shcodecs_cap_SOURCES = capture.c shcodecs-cap.c

