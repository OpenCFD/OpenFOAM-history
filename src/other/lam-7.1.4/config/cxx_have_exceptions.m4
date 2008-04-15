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
dnl $Id: cxx_have_exceptions.m4,v 1.4 2003/02/04 17:47:20 jsquyres Exp $
dnl

define([LAM_CXX_HAVE_EXCEPTIONS],[
#
# Arguments: None
#
# Depdencies: None
#
# Check to see if the C++ compiler can handle exceptions 
#
# Sets LAM_CXX_EXCEPTIONS to 1 if compiler has exceptions, 0 if not
#

AC_MSG_CHECKING([for throw/catch])
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE(, int i=1; throw(i);, 
    LAM_CXX_EXCEPTIONS=1, LAM_CXX_EXCPTIONS=0)
if test "$LAM_CXX_EXCEPTIONS" = "1"; then
    AC_MSG_RESULT([yes])
else
    AC_MSG_RESULT([no])
fi

# Clean up
AC_LANG_RESTORE])dnl
