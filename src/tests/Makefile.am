## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

SHCODECSDIR = ../libshcodecs
SHCODECS_LIBS = $(SHCODECSDIR)/libshcodecs.la $(VPU4_DEC_LIBS) $(VPU4_ENC_LIBS) -lm

# Test programs

TESTS_ENVIRONMENT = $(VALGRIND_ENVIRONMENT)

test: check

basic_tests = noop

noinst_PROGRAMS = $(basic_tests)
noinst_HEADERS = shcodecs_tests.h

TESTS = $(basic_tests)

noop_SOURCES = noop.c
noop_LDADD = $(SHCODECS_LIBS)
