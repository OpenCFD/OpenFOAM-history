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
dnl $Id: lam_bproc_setup.m4,v 1.9 2003/06/25 19:28:14 jsquyres Exp $
dnl

define([LAM_BPROC_SETUP],[
#
# Arguments: None
#
# Dependencies: None
#
# Make a best effort to see if we are on a BPROC system.  Also allow
# user to specify location to the bproc prefix, which will work just as
# well :).
#

LAM_HAVE_BPROC=0
BPROC_LDFLAGS=""

#
AC_ARG_WITH(bproc, 
    AC_HELP_STRING([--with-bproc=DIR],
		   [directory where the bproc software was installed]))


#
# Case 1: --without-bproc specified - overrides autodetect
#
if test "$with_bproc" = "no"; then
    LAM_HAVE_BPROC=0

#
# Case 2: --with-bproc specified - look in generic places for bproc libs
#
elif test "$with_bproc" = "yes"; then
    # See if we can find the bproc libraries...
    LIBS_save="$LIBS"
    AC_CHECK_LIB(bproc, bproc_numnodes, 
	   LAM_HAVE_BPROC=1,
	   AC_MSG_ERROR([*** Connot find working libbproc.]))
    LIBS="$LIBS_save"

#
# Case 3: nothing specified - if it is there, go with it.  If not, don't :)
#
elif test -z "$with_bproc"; then
    LIBS_save="$LIBS"
    AC_CHECK_LIB(bproc, bproc_numnodes, LAM_HAVE_BPROC=1)
    LIBS="$LIBS_save"

#
# Case 4: --with-bproc=<foo> specified - try where they said to find it
#
else
    BPROC_DIR=$with_bproc

    if test -n "$BPROC_DIR"; then
	# Make the tests work...
	OLDLDFLAGS="$LDFLAGS"
	OLDCPPFLAGS="$CPPFLAGS"
	BPROC_LDFLAGS="-L$BPROC_DIR/lib"
	LDFLAGS="$LDFLAGS $BPROC_LDFLAGS"
	CPPFLAGS="$CPPFLAGS -I$BPROC_DIR/include"
	LIBS_save="$LIBS"

	AC_CHECK_LIB(bproc, bproc_numnodes, LAM_HAVE_BPROC=1,
		AC_MSG_ERROR([*** Cannot find working libbproc.]))

	# Since we are going to add the -L and -l to LIBLAM_EXTRA_LIBS,
	# we reset this to the start ...
	LDFLAGS="$OLDLDFLAGS"
	CPPFLAGS="$OLDCPPFLAGS"
	LIBS="$LIBS_save"

    else	
	AC_CHECK_LIB(bproc, bproc_numnodes, LAM_HAVE_BPROC=1,
	    AC_MSG_ERROR([*** Cannot find working libbproc.]))
    fi
fi

AC_MSG_CHECKING([if want BPROC support])

if test "$LAM_HAVE_BPROC" = "1"; then

    AC_MSG_RESULT([yes])
#
# Ok, we have bproc support.  Add proper things to the various
# compiler flags..
#
    LIBLAM_EXTRA_LIBS="$LIBLAM_EXTRA_LIBS $BPROC_LDFLAGS -lbproc"
    WRAPPER_EXTRA_LIBS="$WRAPPER_EXTRA_LIBS -lbproc"

#
# Tell the RSH test (coming up soon) not to bother
#
    RSH_COMMAND="/bin/false"

#
# Do we have the pre-beonss api?
#
    LDFLAGS_orig="$LDFLAGS"
    LIBS_orig="$LIBS"
    LDFLAGS="$BPROC_LDFLAGS $LDFLAGS"
    LIBS="$LIBS -lbproc"
    AC_CHECK_LIB(bproc, bproc_getnodebyname, 
        LAM_HAVE_BPROC3_API=1, LAM_HAVE_BPROC3_API=0)
    LDFLAGS="$LDFLAGS_orig"
    LIBS="$LIBS_orig"
    AC_MSG_CHECKING([for pre-beonss libbproc])
    if test "$LAM_HAVE_BPROC3_API" = "1" ; then
      AC_MSG_RESULT([yes])
    else
      AC_MSG_RESULT([no])
    fi

else
    AC_MSG_RESULT([no])
    LAM_HAVE_BPROC3_API=0
fi

AC_DEFINE_UNQUOTED(LAM_HAVE_BPROC, $LAM_HAVE_BPROC, 
    [Whether we have bproc support or not])

AC_DEFINE_UNQUOTED(LAM_HAVE_BPROC3_API, $LAM_HAVE_BPROC3_API,
    [Whether we have orignal bproc api or are missing gethost functions])

# Clean up
unset BPROC_LDFLAGS])dnl
