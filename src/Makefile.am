AM_CPPFLAGS = -D__STDC_INT64__

if USE_SUBMODULES
SUBDIRS = ../glitter
AM_CPPFLAGS += -I $(top_srcdir)/glitter/src

SUBDIRS += ../TSCPAQ
AM_CPPFLAGS += -I $(top_srcdir)/TSCPAQ/src

SUBDIRS += ThIpe
AM_CPPFLAGS += -I $(top_srcdir)/ThIpe/src
endif

include Makefile-LIBADD.inc

lib_LTLIBRARIES = libthrell.la
include_HEADERS = threll.h
libthrell_la_LIBADD = $(LIBTHRELL_LIBADD)

bin_PROGRAMS = threll-test
threll_test_LDADD = libthrell.la $(LIBTHRELL_LIBADD)
