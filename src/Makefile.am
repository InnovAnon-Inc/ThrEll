AM_CPPFLAGS = -D__STDC_INT64__

lib_LTLIBRARIES =
bin_PROGRAMS    =

lib_LTLIBRARIES += libthrell.la
include_HEADERS = threll.h
libthrell_la_CPPFLAGS = $(GLITTER_CFLAGS) $(IO_CFLAGS) $(PIPE_CFLAGS)
libthrell_la_LIBADD   = $(GLITTER_LIBS)   $(IO_LIBS)   $(PIPE_LIBS) -lpthread

bin_PROGRAMS += threll-test
threll_test_CPPFLAGS =          $(GLITTER_CFLAGS) $(IO_CFLAGS) $(PIPE_CFLAGS)
threll_test_LDADD    = -lthrell $(GLITTER_LIBS)   $(IO_LIBS)   $(PIPE_LIBS)
EXTRA_threll_test_DEPENDENCIES = libthrell.la

