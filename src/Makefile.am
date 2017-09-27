lib_LTLIBRARIES = libthrell.la
libthrell_la_SOURCES = threll.c
include_HEADERS = threll.h
libthrell_la_LIBADD = -lthipe -lrestart -ltscpaq -lcpaq -lpthread

bin_PROGRAMS = threll
threll_SOURCES = main.c
threll_LDADD = libthrell.la -lthipe -lrestart -ltscpaq -lcpaq -lpthread

AM_CPPFLAGS += -D__STDC_INT64__
