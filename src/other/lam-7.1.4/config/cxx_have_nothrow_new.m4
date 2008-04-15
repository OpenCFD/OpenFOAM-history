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
dnl $Id: cxx_have_nothrow_new.m4,v 1.5 2003/02/04 17:47:20 jsquyres Exp $
dnl

define([LAM_CXX_HAVE_NOTHROW_NEW],[
#
# Arguments: None
#
# Depdendencies: None
#
# Check to see if the C++ compiler has std::nothrow support for new
# and new[]
#
# Defines LAM_CXX_NOTHROW_NEW_ARG to be either "(std::nothrow)" or ""
# Defines LAM_CXX_NOTHROW_NEW to be either 1 or 0
# Sets LAM_CXX_NOTHROW_NEW to be 1 or 0
#

# Do the rest
if test "$?CXXFLAGS" = 0; then
    CXXFLAGS=""
fi
AC_MSG_CHECKING([whether $CXX has new(std::nothrow)])
rm -f conftest.cc conftest.o
cat > conftest.cc <<EOF
#include <new>

int main(int argc, char* argv[]) {
  int *baz;
  unsigned int foo = 3;
  baz = new(std::nothrow) int;
  baz = new(std::nothrow) int[[foo - 1]];
}
EOF
echo configure:__oline__: $CXX $CXXFLAGS conftest.cc -o conftest >&5 
$CXX $CXXFLAGS conftest.cc -o conftest >&5 2>&5
if test -f conftest; then
    lsc_result=1
    lsc_define="(std::nothrow)"
    AC_MSG_RESULT([yes])
else
    echo configure:__oline__: here is the program that failed: >&5
    cat conftest.cc >&5
    lsc_result=0
    lsc_define=""
    AC_MSG_RESULT([no])
fi

# Set the result
LAM_CXX_NOTHROW_NEW="$lsc_result"
AC_DEFINE_UNQUOTED(LAM_CXX_NOTHROW_NEW, $lsc_result, 
    [Whether we have nothrow::new or not])
AC_DEFINE_UNQUOTED(LAM_CXX_NOTHROW_NEW_ARG, $lsc_define,
    [Whether nothrow::new requires an argument or not])

# Clean up
rm -f conftest*
unset lsc_result lsc_define])dnl

