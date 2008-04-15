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
dnl $Id: cxx_find_exception_flags.m4,v 1.5 2003/02/04 17:47:20 jsquyres Exp $
dnl

define([LAM_CXX_FIND_EXCEPTION_FLAGS],[
#
# Arguments: none
#
# Dependencies: none
#
# Get the exception handling flags for the C++ compiler.  Leaves
# CXXFLAGS undisturbed.
# Provides --with-exflags command line argument for configure as well.
#
# Sets LAM_CXX_EXCEPTION_CXXFLAGS and LAM_CXX_EXCEPTION_LDFLAGS as
# appropriate.
# Must call AC_SUBST manually
#

# Command line flags
AC_ARG_WITH(exflags,   
  AC_HELP_STRING([--with-exflags],
     [Specify flags necessary to enable exceptions]), 
  lam_force_exflags="$withval")

lam_CXXFLAGS_SAVE="$CXXFLAGS"
AC_MSG_CHECKING([for compiler exception flags])
if test "$lam_force_exflags" = "no"; then
    lam_exflags=
elif test "$lam_force_exflags" != ""; then
    lam_exflags="$lam_force_exflags"
elif test "$GXX" = "yes"; then
    CXXFLAGS="$CXXFLAGS -fexceptions"

    AC_LANG_SAVE
    AC_LANG_CPLUSPLUS
    AC_TRY_COMPILE(, [try { int i = 0; } catch(...) { int j = 2; }], lam_happy=1, lam_happy=0)

    if test "$lam_happy" = "1"; then
	lam_exflags="-fexceptions";
    else
	CXXFLAGS="$CXXFLAGS_SAVE -fhandle-exceptions"
	AC_TRY_COMPILE(, [try { int i = 0; } catch(...) { int j = 2; }], lam_happy=1, lam_happy=0)
	if test "$lam_happy" = "1"; then
	    lam_exflags="-fhandle-exceptions";
	fi
    fi
    AC_LANG_RESTORE
elif test "`basename $CXX`" = "KCC" -o "`basename $LAMHCP`" = "KCC"; then
    lam_exflags="--exceptions"
fi
CXXFLAGS="$lam_CXXFLAGS_SAVE"

# Save the result
LAM_CXX_EXCEPTIONS_CXXFLAGS="$lam_exflags"
LAM_CXX_EXCEPTIONS_LDFLAGS="$lam_exflags"
if test "$lam_exflags" = ""; then
    AC_MSG_RESULT([none necessary])
else
    AC_MSG_RESULT([$lam_exflags])
fi

# Clean up
unset lam_force_exflags lam_CXXFLAGS_SAVE lam_exflags lam_happy])dnl

