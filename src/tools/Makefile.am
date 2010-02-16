## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

SHCODECSDIR = ../libshcodecs
SHCODECS_LIBS = $(SHCODECSDIR)/libshcodecs.la -lm

bin_PROGRAMS = shcodecs-dec shcodecs-enc shcodecs-cap shcodecs-play shcodecs-record

noinst_PROGRAMS = shcodecs-enc-benchmark

noinst_HEADERS = \
	avcbencsmp.h \
	capture.h \
	framerate.h \
	ControlFileUtil.h \
	veu_colorspace.h

shcodecs_dec_SOURCES = shcodecs-dec.c
shcodecs_dec_LDADD = $(SHCODECS_LIBS)

shcodecs_play_SOURCES = shcodecs-play.c framerate.c veu_colorspace.c
shcodecs_play_LDADD = -lrt -lpthread $(SHCODECS_LIBS)

shcodecs_record_SOURCES = \
	shcodecs-record.c \
	capture.c \
	framerate.c \
	veu_colorspace.c \
	ControlFileUtil.c \
	avcbeinputuser.c

shcodecs_record_LDADD = -lrt -lpthread $(SHCODECS_LIBS)


shcodecs_enc_SOURCES =  \
	shcodecs-enc.c \
	capture.c \
	framerate.c \
	veu_colorspace.c \
	ControlFileUtil.c \
	avcbeinputuser.c

shcodecs_enc_LDADD = -lrt $(SHCODECS_LIBS)

shcodecs_enc_benchmark_SOURCES =  \
	shcodecs-enc-benchmark.c \
	capture.c \
	framerate.c \
	veu_colorspace.c \
	ControlFileUtil.c \
	avcbeinputuser.c

shcodecs_enc_benchmark_LDADD = -lrt $(SHCODECS_LIBS)

shcodecs_cap_SOURCES =  \
	shcodecs-cap.c \
	capture.c \
	framerate.c \
	veu_colorspace.c \
	ControlFileUtil.c \
	avcbeinputuser.c

shcodecs_cap_LDADD = -lrt $(SHCODECS_LIBS)

