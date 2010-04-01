## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

SHCODECSDIR = ../libshcodecs
SHCODECS_LIBS = $(SHCODECSDIR)/libshcodecs.la $(VPU4_DEC_LIBS) $(VPU4_ENC_LIBS) -lm

bin_PROGRAMS = shcodecs-decode-qvga-mpeg4

shcodecs_decode_qvga_mpeg4_SOURCES = shcodecs-decode-qvga-mpeg4.c
shcodecs_decode_qvga_mpeg4_LDADD = $(SHCODECS_LIBS)
