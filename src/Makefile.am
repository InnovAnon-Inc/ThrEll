lib_LTLIBRARIES = libthrell.la
libthrell_la_SOURCES = threll.c
include_HEADERS	= threll.h

libthrell_la_CFLAGS = -g -O0 -Wextra -pedantic -ansi
libthrell_la_LDFLAGS = -lcaq -lpthread

bin_PROGRAMS = threll-test
threll_test_SOURCES = main.c
threll_test_LDFLAGS = -lcaq -lpthread
threll_test_LDADD = libthrell.la

threll_test_CFLAGS = -g -O0 -Wextra -pedantic -ansi -Wconversion -Winit-self -Wold-style-cast -Woverloaded-virtual -Wuninitialized -Wmissing-declarations -Winit-self -Wcast-qual  -Wsign-promo -Wcast-align -Wstrict-null-sentinel -Wstrict-overflow 
