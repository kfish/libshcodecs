## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

SHCODECSDIR = ../libshcodecs
SHCODECS_LIBS = $(SHCODECSDIR)/libshcodecs.la $(VPU4_DEC_LIBS) $(VPU4_ENC_LIBS) -lm

bin_PROGRAMS = shcodecs-dec shcodecs-enc shcodecs-encdec shcodecs-cap shcodecs-play shcodecs-record

noinst_PROGRAMS = shcodecs-enc-benchmark

noinst_HEADERS = \
	avcbencsmp.h \
	capture.h \
	display.h \
	framerate.h \
	thrqueue.h \
	ControlFileUtil.h

shcodecs_dec_SOURCES = shcodecs-dec.c
shcodecs_dec_LDADD = -lrt $(SHCODECS_LIBS)

shcodecs_play_SOURCES = shcodecs-play.c framerate.c display.c
shcodecs_play_CFLAGS = $(SHVEU_CFLAGS) $(UIOMUX_CFLAGS)
shcodecs_play_LDADD = $(SHVEU_LIBS) $(UIOMUX_LIBS) -lrt -lpthread $(SHCODECS_LIBS)

shcodecs_record_SOURCES = \
	shcodecs-record.c \
	capture.c \
	display.c \
	framerate.c \
	thrqueue.c \
	ControlFileUtil.c \
	avcbeinputuser.c

shcodecs_record_CFLAGS = $(SHVEU_CFLAGS) $(UIOMUX_CFLAGS)
shcodecs_record_LDADD = $(SHVEU_LIBS) $(UIOMUX_LIBS) -lrt -lpthread $(SHCODECS_LIBS)


shcodecs_enc_SOURCES =  \
	shcodecs-enc.c \
	framerate.c \
	ControlFileUtil.c \
	avcbeinputuser.c

shcodecs_enc_CFLAGS = $(UIOMUX_CFLAGS)
shcodecs_enc_LDADD = $(UIOMUX_LIBS) -lrt $(SHCODECS_LIBS)

shcodecs_encdec_SOURCES =  \
	shcodecs-encdec.c \
	framerate.c \
	ControlFileUtil.c \
	avcbeinputuser.c

shcodecs_encdec_CFLAGS = $(UIOMUX_CFLAGS)
shcodecs_encdec_LDADD = $(UIOMUX_LIBS) -lrt $(SHCODECS_LIBS)

shcodecs_enc_benchmark_SOURCES =  \
	shcodecs-enc-benchmark.c \
	capture.c \
	framerate.c \
	ControlFileUtil.c \
	avcbeinputuser.c

shcodecs_enc_benchmark_CFLAGS = $(SHVEU_CFLAGS) $(UIOMUX_CFLAGS)
shcodecs_enc_benchmark_LDADD = $(SHVEU_LIBS) $(UIOMUX_LIBS) -lrt $(SHCODECS_LIBS)

shcodecs_cap_SOURCES =  \
	shcodecs-cap.c \
	capture.c \
	framerate.c \
	ControlFileUtil.c \
	avcbeinputuser.c

shcodecs_cap_CFLAGS = $(SHVEU_CFLAGS) $(UIOMUX_CFLAGS)
shcodecs_cap_LDADD = $(SHVEU_LIBS) $(UIOMUX_LIBS)  $(SHCODECS_LIBS)


