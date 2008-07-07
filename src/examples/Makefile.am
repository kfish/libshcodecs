## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

RSOVPU4DIR = ../librsovpu4
RSOVPU4_LIBS = $(RSOVPU4DIR)/librsovpu4.la

bin_PROGRAMS = vpu4dec

vpu4dec_SOURCES = sample-dec.c
vpu4dec_LDADD = $(RSOVPU4_LIBS)
