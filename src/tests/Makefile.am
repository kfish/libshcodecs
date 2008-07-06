## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

UIOMUXDIR = ../librsovpu4
UIOMUX_LIBS = $(UIOMUXDIR)/librsovpu4.la

# Test programs

TESTS_ENVIRONMENT = $(VALGRIND_ENVIRONMENT)

test: check

basic_tests = noop

noinst_PROGRAMS = $(basic_tests)
noinst_HEADERS = rsovpu4_tests.h

TESTS = $(basic_tests)

noop_SOURCES = noop.c
noop_LDADD = $(UIOMUX_LIBS)
