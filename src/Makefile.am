lib_LTLIBRARIES = libthrell.la
libthrell_la_SOURCES = threll.c
include_HEADERS	= threll.h
#libthrell_la_CFLAGS =
libthrell_la_LDFLAGS = -lcaq -lpthread

bin_PROGRAMS = threll-test
threll_test_SOURCES = main.c
threll_test_LDFLAGS = -lcaq -lpthread
threll_test_LDADD = libthrell.la
#threll_test_CFLAGS =
