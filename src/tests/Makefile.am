## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

RSOVPU4DIR = ../librsovpu4
RSOVPU4_LIBS = $(RSOVPU4DIR)/librsovpu4.la $(VPU4_LIBS)

# Test programs

TESTS_ENVIRONMENT = $(VALGRIND_ENVIRONMENT)

test: check

basic_tests = noop

noinst_PROGRAMS = $(basic_tests)
noinst_HEADERS = rsovpu4_tests.h

TESTS = $(basic_tests)

noop_SOURCES = noop.c
noop_LDADD = $(RSOVPU4_LIBS)
