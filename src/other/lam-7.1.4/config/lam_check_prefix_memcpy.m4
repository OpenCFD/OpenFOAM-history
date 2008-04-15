dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
dnl $Id: lam_check_prefix_memcpy.m4,v 1.1 2002/12/16 16:11:45 jsquyres Exp $
dnl

define(LAM_CHECK_PREFIX_MEMCPY,[

# glibc's memcpy has weird copying issues when the size being copied
# is not divisble by 4 on x86 platforms.  See if we're linking against
# glibc, and if so, enable a workaround that first copies (size % 8)
# "prefix" bytes over before memcpy'ing over the rest of the block.
# Also provide a --without option to turn this behavior off because we
# don't know when/if glibc will fix this behavior, and/or if it really
# happens on all platforms.

AC_MSG_CHECKING([for glibc])
AC_TRY_RUN([
#include <stdio.h>

int main(int argc, char* argv[]) 
{
  FILE *fp = fopen("configtest.out", "w");
#ifdef __GLIBC__
  fprintf(fp, "1\n");
#else
  fprintf(fp, "0\n");
#endif
  fclose(fp);
  return 0;
}])
have_glibc=`cat configtest.out`
if test "$have_glibc" = "1"; then
    AC_MSG_RESULT([yes])
else
    AC_MSG_RESULT([no])
fi
rm -f configtest.out

# If we have glibc, then assume we want the prefix memcpy

AC_MSG_CHECKING([whether to use prefix memcpy()])
if test "$have_glibc" = "1"; then
    want_prefix_memcpy=1
    prefix_memcpy_msg1="yes"
    prefix_memcpy_msg2="(default on glibc systems: yes)"
else
    want_prefix_memcpy=0
    prefix_memcpy_msg1="no"
    prefix_memcpy_msg2="(default on non-glibc systems: no)"
fi

# The --without (or --with) argument will trump the previous test

AC_ARG_WITH(prefix-memcpy, 
    AC_HELP_STRING([--with(out)-prefix-memcpy],
                   [use a "prefix" memcpy() (default: yes glibc systems, no on other systems)]))

if test "$with_prefix_memcpy" = "yes"; then
    prefix_memcpy_msg1="yes"
    want_prefix_memcpy=1
elif test "$with_prefix_memcpy" = "no"; then
    prefix_memcpy_msg1="no"
    want_prefix_memcpy=0
fi
AC_DEFINE_UNQUOTED([LAM_WANT_PREFIX_MEMCPY], $want_prefix_memcpy,
    [Whether to use a "prefix" memcpy or not])
AM_CONDITIONAL(WANT_PREFIX_MEMCPY, test "$want_prefix_memcpy" = "1")
AC_MSG_RESULT([$prefix_memcpy_msg1 $prefix_memcpy_msg2])

unset have_glibc want_prefix_memcpy prefix_memcpy_msg1 prefix_memcpy_msg2])dnl
