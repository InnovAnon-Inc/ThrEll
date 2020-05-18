AC_INIT([libthrell], [0.0], [laurence.a.maddox@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AM_PROG_AR
LT_INIT([])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/libthrell.pc
 src/threll-test.pc
])

PKG_PROG_PKG_CONFIG

AC_ARG_ENABLE([use-submodules],
    AS_HELP_STRING([--enable-use-submodules], [Enable use submodules]))
AM_CONDITIONAL([USE_SUBMODULES], [test x"$enable_use_submodules" = "xyes"])

AC_CONFIG_MACRO_DIR([m4])

#AC_CHECK_LIB([pthread],[pthread_mutex_init,pthread_mutex_lock,pthread_mutex_unlock])
#AC_CHECK_LIB([caq],[init_queue,alloc_queue,free_queue,enqueue,dequeue,isempty,isfull,gethead,dumpq])

AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([pthread.h])
#AC_CHECK_HEADERS([glitter.h io.h tscpaq.h])

#AC_CHECK_LIB([stdlib],[])
AC_CHECK_LIB([pthread],[pthread_create,pthread_join])
#AC_CHECK_LIB([tscpaq],[tscpaq_enqueue,tscpaq_dequeue])
#AC_CHECK_LIB([io],[alloc_io,free_io])
#AC_CHECK_LIB([restart],[r_close])

PKG_CHECK_MODULES([GLITTER],[glitter],,)
PKG_CHECK_MODULES([PIPE],[libpipe],,)
PKG_CHECK_MODULES([IO],[io],,)

INNOVANON_WARNINGS([])

AC_OUTPUT

