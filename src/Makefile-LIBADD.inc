#include $(top_srcdir)/ThIpe/src/Makefile-LIBADD.inc
include ../ThIpe/src/Makefile-LIBADD.inc

#if USE_SUBMODULES
#LIBTHRELL_LIBADD = ../ThIpe/src/.libs/libthipe.la $(LIBTHIPE_LIBADD)
#else
LIBTHRELL_LIBADD = -lthipe $(LIBTHIPE_LIBADD)
#endif
