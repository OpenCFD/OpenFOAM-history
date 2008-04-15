dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2001-2004 The Trustees of Indiana University.  
dnl                         All rights reserved.
dnl Copyright (c) 1998-2001 University of Notre Dame. 
dnl                         All rights reserved.
dnl Copyright (c) 1994-1998 The Ohio State University.  
dnl                         All rights reserved.
dnl 
dnl This file is part of the LAM/MPI software package.  For license
dnl information, see the LICENSE file in the top level directory of the
dnl LAM/MPI source distribution.
dnl
dnl

AC_DEFUN([LAM_PTMALLOC_SETUP],[
#
# Call the top-level LAM threads setup stuff
#
OLD_CPPFLAGS="$CPPFLAGS"
OLD_LDFLAGS="$LDFLAGS"
OLD_LIBS="$LIBS"

CPPFLAGS="$CPPFLAGS $THREADCPPFLAGS"
LDFLAGS="$LDFLAGS $THREADLDFLAGS"
LIBS="$LIBS $THREADLIBS"

if test "`echo $host | grep apple-darwin`" != "" ; then
    AC_MSG_WARN([*** Using ptmalloc with OS X will result in failure.])
    AC_MSG_ERROR([*** Aborting to save you the effort])
fi

#
# See if we can use syscall interface
#
use_syscall=0
AC_CHECK_HEADER([syscall.h], [have_syscall_h=yes], [have_syscall_h=no])
if test "$have_syscall_h" = "yes"; then
    AC_CHECK_FUNCS([syscall])
fi
AC_DEFINE_UNQUOTED([LAM_USE_SYSCALL], $use_syscall,
	[Whether we can get munmap through syscall() or not])

#
# See if we have sbrk prototyped
#
AC_CHECK_DECL([sbrk], [have_decl_sbrk=1], [have_decl_sbrk=0])
AC_DEFINE_UNQUOTED(LAM_HAVE_DECL_SBRK, $have_decl_sbrk,
	[Whether we have a declaration for sbrk() or not])

CPPFLAGS="$OLD_CPPFLAGS"
LDFLAGS="$OLD_LDFLAGS"
LIBS="$OLD_LIBS"
])dnl

AC_DEFUN([LAM_DARWIN_MALLOC_SETUP],[
case "$host" in
  *apple-darwin*)
    WRAPPER_EXTRA_LDFLAGS="-Wl,-u -Wl,_lam_darwin_malloc_linker_hack -Wl,-multiply_defined,suppress -Wl,-force_flat_namespace -Wl,-flat_namespace $WRAPPER_EXTRA_LDFLAGS"
    LDFLAGS="-Wl,-multiply_defined,suppress $LDFLAGS"
    ;;
  *)
    AC_MSG_ERROR([Trying to use Darwin malloc while not on a Darwin system.])
    ;;
esac
])dnl

AC_DEFUN([LAM_MEMORY_SETUP],[
case "$host" in
    *apple-darwin*)
        default_type="darwin"
        ;;
    *solaris* | *-aix* | *cygwin* | *bsd*)
        default_type="none"
        ;;
    *)
        default_type="ptmalloc2"
        ;;
esac

AC_ARG_WITH(memory-manager,
    AC_HELP_STRING([--with-memory-manager=TYPE],
		   [Use TYPE for intercepting memory management calls to control memory pinning (TYPE is one of darwin,ptmalloc,ptmalloc2,external,none)]),
    [WANT_MEMORY="$withval"], [WANT_MEMORY="$default_type"])

AC_MSG_CHECKING([for memory management type])
if test "$WANT_MEMORY" = "darwin" ; then
    AC_MSG_RESULT([Darwin / Mac OS X])
    LAM_DARWIN_MALLOC_SETUP
    LAM_WANT_DARWIN7MALLOC=1
    LAM_WANT_PTMALLOC=0
    LAM_WANT_PTMALLOC2=0
    LAM_WANT_EXTERNALMALLOC=0
elif test "$WANT_MEMORY" = "ptmalloc" ; then
    AC_MSG_RESULT([ptmalloc])
    LAM_PTMALLOC_SETUP
    LAM_WANT_DARWIN7MALLOC=0
    LAM_WANT_PTMALLOC=1
    LAM_WANT_PTMALLOC2=0
    LAM_WANT_EXTERNALMALLOC=0
elif test "$WANT_MEMORY" = "ptmalloc2" ; then
    AC_MSG_RESULT([ptmalloc2])
    LAM_PTMALLOC_SETUP
    LAM_WANT_DARWIN7MALLOC=0
    LAM_WANT_PTMALLOC=0
    LAM_WANT_PTMALLOC2=1
    LAM_WANT_EXTERNALMALLOC=0
elif test "$WANT_MEMORY" = "external" ; then
    AC_MSG_RESULT([external])
    LAM_PTMALLOC_SETUP
    LAM_WANT_DARWIN7MALLOC=0
    LAM_WANT_PTMALLOC=0
    LAM_WANT_PTMALLOC2=0
    LAM_WANT_EXTERNALMALLOC=1
else
    AC_MSG_RESULT([none])
    LAM_WANT_DARWIN7MALLOC=0
    LAM_WANT_PTMALLOC=0
    LAM_WANT_PTMALLOC2=0
    LAM_WANT_EXTERNALMALLOC=0
fi

AC_DEFINE_UNQUOTED([LAM_WANT_PTMALLOC], $LAM_WANT_PTMALLOC,
    [Do we want ptmalloc support])
AM_CONDITIONAL(LAM_WANT_PTMALLOC, test "$LAM_WANT_PTMALLOC" = "1")

AC_DEFINE_UNQUOTED([LAM_WANT_PTMALLOC2], $LAM_WANT_PTMALLOC2,
    [Do we want ptmalloc2 support])
AM_CONDITIONAL(LAM_WANT_PTMALLOC2, test "$LAM_WANT_PTMALLOC2" = "1")

AC_DEFINE_UNQUOTED([LAM_WANT_DARWIN7MALLOC], $LAM_WANT_DARWIN7MALLOC,
    [Do we want darwin7malloc support])
AM_CONDITIONAL(LAM_WANT_DARWIN7MALLOC, test "$LAM_WANT_DARWIN7MALLOC" = "1")

AC_DEFINE_UNQUOTED([LAM_WANT_EXTERNALMALLOC], $LAM_WANT_EXTERNALMALLOC,
    [Do we want external malloc support])

])dnl
