lib_LTLIBRARIES = libthrell.la
libthrell_la_SOURCES = threll.c
include_HEADERS	= threll.h

libthrell_la_CFLAGS = -Wextra -pedantic -ansi -Werror -Wdouble-promotion \
                   -Wformat=2 -Wswitch-default -Wswitch-enum -Wuninitialized \
                   -Wstrict-aliasing=3 -Wstrict-overflow=5 \
                   -Wsuggest-attribute=pure -Wsuggest-attribute=const \
                   -Wsuggest-attribute=noreturn -Wsuggest-attribute=format \
                   -Warray-bounds=2 -Wtrampolines -Wfloat-equal \
                   -Wdeclaration-after-statement \
                   -Wundef -Wshadow -Wunsafe-loop-optimizations \
                   -Wbad-function-cast -Wcast-qual -Wcast-align \
                   -Wwrite-strings -Wconversion -Wjump-misses-init \
                   -Wlogical-op -Waggregate-return \
                   -Wno-aggressive-loop-optimizations -Wstrict-prototypes \
                   -Wmissing-prototypes -Wmissing-declarations -Wpacked \
                   -Wredundant-decls -Wnested-externs -Winline \
                   -Wvector-operation-performance -Wdisabled-optimization \
                   -Wstack-protector -Wunsuffixed-float-constants \
                   -Winit-self
libthrell_la_LDFLAGS = -lcaq -lpthread

bin_PROGRAMS = threll-test
threll_test_SOURCES = main.c
threll_test_LDFLAGS = -lcaq -lpthread
threll_test_LDADD = libthrell.la

threll_test_CFLAGS = -Wextra -pedantic -ansi -Werror -Wdouble-promotion \
                   -Wformat=2 -Wswitch-default -Wswitch-enum -Wuninitialized \
                   -Wstrict-aliasing=1 -Wstrict-overflow=5 \
                   -Wsuggest-attribute=pure -Wsuggest-attribute=const \
                   -Wsuggest-attribute=noreturn -Wsuggest-attribute=format \
                   -Warray-bounds=2 -Wtrampolines -Wfloat-equal \
                   -Wtraditional-conversion -Wdeclaration-after-statement \
                   -Wundef -Wshadow -Wunsafe-loop-optimizations \
                   -Wbad-function-cast -Wcast-qual -Wcast-align \
                   -Wwrite-strings -Wconversion -Wjump-misses-init \
                   -Wlogical-op -Waggregate-return \
                   -Wno-aggressive-loop-optimizations -Wstrict-prototypes \
                   -Wmissing-prototypes -Wmissing-declarations -Wpacked \
                   -Wredundant-decls -Wnested-externs -Winline \
                   -Wvector-operation-performance -Wdisabled-optimization \
                   -Wstack-protector -Wunsuffixed-float-constants \
                   -Winit-self

