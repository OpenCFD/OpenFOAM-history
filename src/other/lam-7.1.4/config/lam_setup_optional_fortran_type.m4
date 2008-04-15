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
dnl $Id: lam_get_sizeof_fortran_type.m4,v 1.7 2003/02/17 13:50:01 jsquyres Exp $
dnl

AC_DEFUN([LAM_SETUP_OPTIONAL_FORTRAN_TYPE],[
f_type="$1"
f_kind="$2"
f_suffix="$3"

# First, get the size of the fortran type.  If the type is not
# supported, then LAM_SIZEOF_FORTRAN_<type> will be 0.

LAM_GET_SIZEOF_FORTRAN_TYPE_NOFAIL($f_type, LAM_SIZEOF_FORTRAN_$f_suffix)

# Check to see if the size is > 0, and if so, find a corresponding C
# type for it

str="test \$LAM_SIZEOF_FORTRAN_$f_suffix != 0"
eval $str
if test "$?" = "0"; then

    AC_MSG_CHECKING([for corresponding C type for $f_type])

    if test "$f_kind" = "integer"; then 
	c_types="char short int long long_long"
    else
	c_types="float double long_double"
    fi

    found=0
    result="<not found> (default to int)"
    for t in $c_types; do
	real_t="`echo $t | sed -e 's/_/ /g'`" 
	caps_t="`echo $t | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`"
	str="test \$LAM_SIZEOF_FORTRAN_$f_suffix = \$LAM_SIZEOF_$caps_t"
	eval $str
	if test "$found" = "0" -a "$?" = "0"; then
	    result="$real_t"
	    found=1
	fi
    done
    AC_MSG_RESULT([$result])
    if test "$found" = "0"; then
      result=int
# Set the sizeof corresponding FORTRAN type to be 0 
      eval "LAM_SIZEOF_FORTRAN_$f_suffix=0"
    	AC_MSG_WARN([Could not find corresponding C type!])
      AC_MSG_WARN([***$f_suffix will not be supported]) 
    fi

    eval "LAM_FORTRAN_C_TYPE_$f_suffix=\"$result\""
fi
])dnl
