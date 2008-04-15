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
dnl $Id: cxx_have_true_false.m4,v 1.5 2002/10/09 20:55:10 brbarret Exp $
dnl

define([LSC_CXX_HAVE_TRUE_FALSE],[
#
# Arguments: prefix (optional)
#
# Dependencies: None
#
# Check to see if the C++ compiler has "true" and "false" defined.  <sigh>
#
# Defines LSC_CXX_TRUE_FALSE to be 1 or 0
#

# Figure out what prefix to use
lsc_prefix="$1"
if test "$lsc_prefix" = ""; then
    lsc_prefix="LSC"
fi

# Do the test
if test "$?CXXFLAGS" = 0; then
    CXXFLAGS=""
fi
AC_MSG_CHECKING([for true/false])
rm -f conftest.cc conftest.o
cat > conftest.cc <<EOF
#include <stdio.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
  int foo;
  foo = (int) true;
  foo = (int) false;
  printf("so foo is used and the compiler wont complain: %d", (int) foo);
  return 0;
}
EOF
echo configure:__oline__: $CXX $CXXFLAGS conftest.cc -o conftest >&5 
$CXX $CXXFLAGS conftest.cc -o conftest >&5 2>&5
if test -f conftest; then
    lsc_result=1
    AC_MSG_RESULT([yes])
else
    lsc_result=0
    echo configure:__oline__: here is the program that failed: >&5
    cat conftest.cc >&5
    AC_MSG_RESULT([no])
fi

# Set the result
lsc_name="${lsc_prefix}_CXX_TRUE_FALSE"
lsc_str="${lsc_name}=$lsc_result"
eval $lsc_str
AC_DEFINE_UNQUOTED($lsc_name, $lsc_result, 
    [Whether we have the C++ keywords "true" and "false" or not])

# Clean up
rm -f conftest*
unset lsc_prefix lsc_name lsc_str lsc_result])dnl

