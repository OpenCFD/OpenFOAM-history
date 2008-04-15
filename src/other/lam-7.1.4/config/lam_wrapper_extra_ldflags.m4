dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
dnl $Id: lam_wrapper_extra_ldflags.m4,v 1.5 2003/10/12 02:37:16 brbarret Exp $
dnl

define([LAM_WRAPPER_EXTRA_LDFLAGS], [
#
# Arguments: None
#
# Dependencies: None
#
# Defines:
#  LAM_WANT_WRAPPER_EXTRA_LDFLAGS
#
# Adds to:
#  EXTRA_MPICC_LDFLAGS
#  EXTRA_MPICXX_LDFLAGS
#  EXTRA_MPIF77_LDFLAGS
#

# Here's the --with option that allows the user to select this
# behavior

WANT_WRAPPER_EXTRA_LDFLAGS=no
AC_ARG_WITH(wrapper-extra-ldflags,
    AC_HELP_STRING([--with-wrapper-extra-ldflags],
		   [Add extra LDFLAGS to the wrapper compiler underlying command line, extracted from libtool]),
    [WANT_WRAPPER_EXTRA_LDFLAGS="$withval"])
AC_MSG_CHECKING([if want extra wrapper compiler LDFLAGS])

# If we want the behavior.  This is only relevant when building shared
# libraries.

if test "$enable_shared" = "no"; then
    AC_MSG_RESULT([no (shared libs disabled)])
elif test "$WANT_WRAPPER_EXTRA_LDFLAGS" = "yes"; then
    AC_MSG_RESULT([yes])
    AC_MSG_CHECKING([for extra wrapper compiler LDFLAGS])

    # Make a directory to play in

    rm -f conftest.$$
    mkdir conftest.$$
    cd conftest.$$

    # Annoying things to get a usable $libdir

    prefix_save="$prefix"
    exec_prefix_save="$exec_prefix"

    test "x$prefix" = xNONE && prefix="$ac_default_prefix"
    test "x$exec_prefix" = xNONE && exec_prefix="$prefix"
    my_libdir="`eval echo $libdir`"

    prefix="$prefix_save"
    exec_prefix="$exec_prefix_save"

    # Make a small shared library with libtool.  Note that the -rpath
    # option to libtool seems to be what triggers making the shared
    # library.

    cat > foo.c <<EOF
int foo(void) { return 1; }
EOF
    cat > conftest.c <<EOF
extern int foo(void);
int main(int argc, char* argv[]) { return foo(); }
EOF

    ../libtool --mode=compile $CC foo.c -o foo.lo -c > /dev/null 2>&1
    ../libtool --mode=link $CC foo.lo -o libfoo.la -rpath $my_libdir > /dev/null 2>&1

    # Now try linking something against it, and see what extra flags
    # libtool adds.

    ../libtool --mode=compile $CC conftest.c -o conftest.lo -c > /dev/null 2>&1
    libtool_flags="`../libtool --mode=link $CC conftest.lo -o conftest libfoo.la | egrep \^$CC`"
    wrapper_extra_ldflags=
    for flag in $libtool_flags; do
	case "$flag" in
	$CC)
	    ;;
	*conftest.lo)
	    ;;
	*conftest.o)
	    ;;
	-o)
	    ;;
	*conftest)
	    ;;
	*libfoo.*)
	    ;;
	*)
	    wrapper_extra_ldflags="$wrapper_extra_ldflags $flag"
	    ;;
	esac
    done

    # Got the results -- back out of the sandbox directory and whack
    # it

    cd ..
    rm -rf conftest.$$

    # We put them in the LIBS flags so that they're added at the end
    # of the compile/link command.

    WRAPPER_EXTRA_LIBS="$WRAPPER_EXTRA_LIBS $wrapper_extra_ldflags"
    AC_MSG_RESULT([$wrapper_extra_ldflags])
    unset prefix_save exec_prefix_save my_libdir libtool_flags wrapper_extra_ldflags
else
    AC_MSG_RESULT([no])
fi])
