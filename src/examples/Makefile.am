## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

SHCODECSDIR = ../libshcodecs
SHCODECS_LIBS = $(SHCODECSDIR)/libshcodecs.la -lm

bin_PROGRAMS = shcodecsdec

shcodecsdec_SOURCES = sample_dec.c
shcodecsdec_LDADD = $(SHCODECS_LIBS)
