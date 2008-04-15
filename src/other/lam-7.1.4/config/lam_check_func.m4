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
dnl $Id: lam_check_func.m4,v 1.6 2002/10/09 20:55:11 brbarret Exp $
dnl

define(LAM_CHECK_FUNC,[
# Determine if function taking void args is available. 
# First arg is function, 2nd arg is config var to define, 3rd arg is options
# to add to link line.
LIBS_save="$LIBS"
LIBS="$LIBS $3"
AC_CHECK_FUNC($1, FOUND_FUNC=1, FOUND_FUNC=0)
LIBS="$LIBS_save"
AH_TEMPLATE([$2], [Whether we have $2 or not])
if test "$FOUND_FUNC" = "1"; then
    AC_DEFINE_UNQUOTED($2, 1)
else
    AC_DEFINE_UNQUOTED($2, 0)
fi
unset LIBS_save FOUND_FUNC])dnl
