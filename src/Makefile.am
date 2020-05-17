AM_CPPFLAGS = -D__STDC_INT64__

#include Makefile-LIBADD.inc
dist_bin_SCRIPTS = libthrell_config threll_test_config

lib_LTLIBRARIES =
bin_PROGRAMS    =

lib_LTLIBRARIES += libthrell.la
include_HEADERS = threll.h
libthrell_la_LIBADD = @LIBTHRELL_LIBADD@

bin_PROGRAMS += threll-test
#threll_test_LDADD = libthrell.la $(LIBTHRELL_LIBADD)
threll_test_LDADD = @THRELL_TEST_LIBADD@
EXTRA_threll_test_DEPENDENCIES = libthrell.la
