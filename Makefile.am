## Process this file with automake to produce Makefile.in

DISTCHECK_CONFIGURE_FLAGS = --enable-gcc-werror

SUBDIRS = data doc include src

# pkg-config
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = shcodecs.pc

# Extra files to distribute in the source tarball.
EXTRA_DIST = shcodecs.pc.in shcodecs-uninstalled.pc.in
