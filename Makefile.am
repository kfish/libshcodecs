## Process this file with automake to produce Makefile.in

DISTCHECK_CONFIGURE_FLAGS = --enable-gcc-werror

SUBDIRS = doc include src

# pkg-config
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = rsovpu4.pc

# Extra files to distribute in the source tarball.
EXTRA_DIST = rsovpu4.pc.in rsovpu4-uninstalled.pc.in
