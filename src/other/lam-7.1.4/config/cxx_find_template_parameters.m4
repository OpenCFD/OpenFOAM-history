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
dnl $Id: cxx_find_template_parameters.m4,v 1.1 2003/02/22 22:08:48 vsahay Exp $
dnl

define(LAM_CXX_FIND_TEMPLATE_PARAMETERS,[
#
# Arguments: none
#
# Dependencies: None
#
# Get the C++ compiler template parameters.
#
# Adds to CXXFLAGS

AC_MSG_CHECKING([for C++ compiler template parameters])
if test "$BASECXX" = "KCC"; then                              
  new_flags="--one_instantiation_per_object"
  CXXFLAGS="$CXXFLAGS $new_flags" 
else
  new_flags="none needed"
fi
AC_MSG_RESULT([$new_flags])

#
# Clean up
#
unset new_flags
])
