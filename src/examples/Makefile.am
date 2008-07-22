## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

RSOVPU4DIR = ../librsovpu4
RSOVPU4_LIBS = $(RSOVPU4DIR)/librsovpu4.la -lm

bin_PROGRAMS = rsovpu4dec

rsovpu4dec_SOURCES = sample_dec.c
rsovpu4dec_LDADD = $(RSOVPU4_LIBS)
