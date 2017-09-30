LIBTHRELL_LIBADD = ../ThIpe/src/.libs/libthipe.la $(LIBTHIPE_LIBADD)
#LIBTHRELL_LIBADD += -lrestart -ltscpaq -lcpaq -lpthread

lib_LTLIBRARIES = libthrell.la
include_HEADERS = threll.h
libthrell_la_LIBADD = $(LIBTHRELL_LIBADD)

bin_PROGRAMS = threll-test
threll_test_LDADD = libthrell.la $(LIBTHRELL_LIBADD)

AM_CPPFLAGS = -D__STDC_INT64__

SUBDIRS = ../glitter
AM_CPPFLAGS += -I ../glitter/src

SUBDIRS += ../TSCPAQ
AM_CPPFLAGS += -I ../TSCPAQ/src

SUBDIRS += ThIpe
AM_CPPFLAGS += -I ../ThIpe/src
