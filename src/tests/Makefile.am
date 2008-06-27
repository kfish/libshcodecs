## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) \
           -I$(top_srcdir)/include

UIOMUXDIR = ../libuiomux
UIOMUX_LIBS = $(UIOMUXDIR)/libuiomux.la

# Test programs

TESTS_ENVIRONMENT = $(VALGRIND_ENVIRONMENT)

test: check

basic_tests = noop double-open

noinst_PROGRAMS = $(basic_tests)
noinst_HEADERS = uiomux_tests.h

TESTS = $(basic_tests)

noop_SOURCES = noop.c
noop_LDADD = $(UIOMUX_LIBS)

double_open_SOURCES = double-open.c
double_open_LDADD = $(UIOMUX_LIBS)
