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
dnl $Id: lam_get_libtool_linker_flags.m4,v 1.1 2003/11/15 12:43:51 jsquyres Exp $
dnl

AC_DEFUN([LAM_CHECK_LINKER_FLAGS],[
#
# libtool has been created by this point
# Try to see if it will add any additional flags for dependant libraries
#

AC_MSG_CHECKING([for libtool-supplied linker flags])

libtool="$1"
extra_flags="$2"

# Get the directory where libtool lives

start="`pwd`"
d="`dirname $libtool`"
cd "$d"
libtool="`pwd`/libtool"
cd "$start"
unset d start

# Make a playground to work in

mkdir conftest.$$
cd conftest.$$

cat > foo.c <<EOF
int foo(void) { return 0; }
EOF

lam_check_linker_flags_work() {
    LAM_LOG_MSG([$cmd], [yes])
    eval $cmd >&5 2>&5
    if test -n "[$]1"; then
	output="`eval $cmd 2>/dev/null`"
    fi
    status="$?"
    LAM_LOG_MSG([\$? = $status], [yes])
    if test "$status" != "0"; then
	AC_MSG_RESULT([libtool error!])
	AC_MSG_ERROR([Cannot continue])
    fi
}

#
# First make a sample library with the current LDFLAGS and LIBS
#

cmd="$libtool --mode=compile $CC $CFLAGS -c foo.c -o foo.o"
lam_check_linker_flags_work
cmd="$libtool --mode=link $CC $CFLAGS foo.lo $LDFLAGS $LIBS -o libfoo.la"
lam_check_linker_flags_work

#
# Now fake linking to it and capture the output from libtool
#

cmd="$libtool --dry-run --mode=link $CC bar.lo libfoo.la -o bar $extra_flags"
lam_check_linker_flags_work yes

eval "set $output"
extra_ldflags=
extra_libs=
while test -n "[$]1"; do
    flag="[$]1"
    LAM_LOG_MSG([examining flag: $flag])
    case "[$]1" in
    $CC) ;;
    bar*) ;;
    -I*) ;;
    -lfoo) ;;
    -o) ;;
    */.libs/*) ;;
    ./*) ;;
    -L/lib) ;;
    -L/lib64) ;;
    -L/usr/lib) ;;
    -L/usr/lib64) ;;
    -L${libdir}) ;;
    -L${prefix}/lib) ;;
    -L${exec_prefix}/lib) ;;
    -l*)
	extra_libs="$extra_libs [$]1"
        LAM_LOG_MSG([added to EXTRA_LIBS: [$]1])
        ;;
    *.so)
	extra_libs="$extra_libs [$]1"
        LAM_LOG_MSG([added to EXTRA_LIBS: [$]1])
        ;;
    *.a)
	extra_libs="$extra_libs [$]1"
        LAM_LOG_MSG([added to EXTRA_LIBS: [$]1])
        ;;
    -L*)
	extra_ldflags="$extra_ldflags [$]1"
        LAM_LOG_MSG([added to EXTRA_LDFLAGS: [$]1])
        ;;
    -R*)
	extra_ldflags="$extra_ldflags [$]1"
        LAM_LOG_MSG([added to EXTRA_LDFLAGS: [$]1])
        ;;
    *)

        # Double check that $CC isn't a multi-word token, such that
        # the $CC) above wouldn't have caught individual words coming
        # through this loop.

        found=
        for word in $CC; do
            if test "$flag" = "$word"; then
                found=1
            fi
        done

        if test -z "$found"; then
            extra_ldflags="$extra_ldflags [$]1"
            LAM_LOG_MSG([added to EXTRA_LDFLAGS: [$]1])
        fi
	;;
    esac
    shift
done

if test -n "$extra_ldflags" -o -n "$extra_libs"; then
    AC_MSG_RESULT([found])
    if test -n "$extra_ldflags"; then
        echo "extra LDFLAGS:$extra_ldflags"
    else
        echo "extra LDFLAGS: (none)"
    fi

    if test -n "$extra_libs"; then
        echo "extra LIBS:$extra_libs"
    else
        echo "extra LIBS: (none)"
    fi
else
    AC_MSG_RESULT([no extra flags])
fi

cd ..
rm -rf conftest.$$])dnl
