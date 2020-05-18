# Install the pkg-config file.
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = src/libthrell.pc src/threll-test.pc

DISTCLEANFILES = $(pkgconfig_DATA)
EXTRA_DIST = src/libthrell.pc.in src/threll-test.pc.in

SUBDIRS = src
dist_doc_DATA = README.md
ACLOCAL_AMFLAGS = -I m4

