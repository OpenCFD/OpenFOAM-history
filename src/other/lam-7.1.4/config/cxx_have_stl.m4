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
dnl $Id: cxx_have_stl.m4,v 1.4 2002/10/09 20:55:10 brbarret Exp $
dnl

define([LSC_CXX_HAVE_STL],[
#
# Arguments: prefix (optional)
#
# Dependencies: None
#
# See if the C++ compiler has STL
#
# Set prefix_CXX_STL and LSC_CXX_STL to 1 if so, 0 if not
# AC_DEFINE prefix_CXX_STL to 1 if so, 0 if not
#

# Figure out what prefix to use
lsc_prefix="$1"
if test "$lsc_prefix" = ""; then
    lsc_prefix="LSC"
fi

# Do the test
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_CHECK_HEADER(map, LSC_CXX_STL=1, LSC_CXX_STL=0)

# Set the result
lsc_name="${lsc_prefix}_CXX_STL"
lsc_str="${lsc_name}=$LSC_CXX_STL"
eval $lsc_str
AC_DEFINE_UNQUOTED($lsc_name, $LSC_CXX_STL, 
    [Whether we have the C++ standard library or not])

# Clean up
AC_LANG_RESTORE
unset lsc_prefix lsc_str lsc_name])dnl

