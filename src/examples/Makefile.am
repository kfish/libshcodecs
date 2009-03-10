## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

SHCODECSDIR = ../libshcodecs
SHCODECS_LIBS = $(SHCODECSDIR)/libshcodecs.la -lm

bin_PROGRAMS = shcodecs-dec

shcodecs_dec_SOURCES = shcodecs-dec.c
shcodecs_dec_LDADD = $(SHCODECS_LIBS)
