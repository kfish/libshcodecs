## Process this file with automake to produce Makefile.in

INCLUDES = $(INCLTDL) \
           -I$(top_builddir) \
           -I$(top_srcdir)/include

EXTRA_DIST = Version_script.in

# Libraries to build
lib_LTLIBRARIES = libuiomux.la

libuiomux_la_SOURCES = \
	uiomux.c

libuiomux_la_LDFLAGS = -version-info @SHARED_VERSION_INFO@ @SHLIB_VERSION_ARG@
libuiomux_la_LIBADD =
