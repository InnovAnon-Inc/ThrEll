lib_LTLIBRARIES = libthrell.la
libthrell_la_SOURCES = threll.c
include_HEADERS = threll.h
libthrell_la_LIBADD = -lthipe -lrestart -ltscpaq -lcpaq -lpthread

bin_PROGRAMS = threll-test
threll_test_SOURCES = main.c
threll_test_LDADD = libthrell.la -lthipe -lrestart -ltscpaq -lcpaq -lpthread

AM_CPPFLAGS = -D__STDC_INT64__
