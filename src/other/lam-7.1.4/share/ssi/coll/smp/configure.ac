# -*- shell-script -*-
#
# Copyright (c) 2001-2003 The Trustees of Indiana University.  
#                         All rights reserved.
# Copyright (c) 1998-2001 University of Notre Dame. 
#                         All rights reserved.
# Copyright (c) 1994-1998 The Ohio State University.  
#                         All rights reserved.
# 
# This file is part of the LAM/MPI software package.  For license
# information, see the LICENSE file in the top level directory of the
# LAM/MPI source distribution.
#
# $Id: ssi_configure.ac,v 1.7 2003/11/15 15:55:25 jsquyres Exp $
#


############################################################################
# Initialization, version number, and other random setup/init stuff
############################################################################

# Init autoconf

AC_INIT(src/ssi_coll_smp.c)
AC_PREREQ(2.57)
AC_CONFIG_AUX_DIR(config)

# Establish the top-level LAM directory

top_lam_srcdir='$(top_srcdir)/../../../..'
top_lam_builddir='$(top_builddir)/../../../..'

AC_SUBST(top_lam_srcdir)
AC_SUBST(top_lam_builddir)

# Get the version of coll smp that we are installing.

LAM_GET_VERSION($srcdir/../../../../config, $srcdir/VERSION,
  LAM_SSI_COLL_SMP)

AC_DEFINE_UNQUOTED(LAM_SSI_COLL_SMP_MAJOR_VERSION, 
    $LAM_SSI_COLL_SMP_MAJOR_VERSION, 
    [Major LAM SSI coll smp version])
AC_DEFINE_UNQUOTED(LAM_SSI_COLL_SMP_MINOR_VERSION, 
    $LAM_SSI_COLL_SMP_MINOR_VERSION, 
    [Minor LAM SSI coll smp version])
AC_DEFINE_UNQUOTED(LAM_SSI_COLL_SMP_RELEASE_VERSION, 
    $LAM_SSI_COLL_SMP_RELEASE_VERSION, 
    [Release LAM SSI coll smp version])
AC_DEFINE_UNQUOTED(LAM_SSI_COLL_SMP_ALPHA_VERSION, 
    $LAM_SSI_COLL_SMP_ALPHA_VERSION, 
    [Alpha LAM SSI coll smp version])
AC_DEFINE_UNQUOTED(LAM_SSI_COLL_SMP_BETA_VERSION, 
    $LAM_SSI_COLL_SMP_BETA_VERSION, 
    [Beta LAM SSI coll smp version])
AC_DEFINE_UNQUOTED(LAM_SSI_COLL_SMP_CVS_VERSION, 
    $LAM_SSI_COLL_SMP_CVS_VERSION, 
    [CVS LAM SSI coll smp version])
AC_DEFINE_UNQUOTED(LAM_SSI_COLL_SMP_VERSION, 
    "$LAM_SSI_COLL_SMP_VERSION", 
    [Overall LAM SSI coll smp version number])

#
# Start it up
#

LAM_CONFIGURE_SETUP
lam_show_title "Configuring SSI coll smp module version $LAM_SSI_COLL_SMP_VERSION"
lam_show_subtitle "Initialization, setup" 

#
# Init automake
# The third argument to AM_INIT_AUTOMAKE surpresses the PACKAGE and
# VERSION macors
#

AM_INIT_AUTOMAKE(coll-smp, $LAM_SSI_COLL_SMP_VERSION, 'no')

# Setup the top-level SSI module config.h file

AH_TOP([/* -*- c -*-
 *
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
 *                         All rights reserved.
 * Copyright (c) 1998-2001 University of Notre Dame. 
 *                         All rights reserved.
 * Copyright (c) 1994-1998 The Ohio State University.  
 *                         All rights reserved.
 * 
 * This file is part of the LAM/MPI software package.  For license
 * information, see the LICENSE file in the top level directory of the
 * LAM/MPI source distribution.
 *
 * LAM/MPI configuation header file.
 * SSI coll: smp module
 */

#ifndef LAM_SSI_COLL_SMP_CONFIG_H
#define LAM_SSI_COLL_SMP_CONFIG_H
])
AH_BOTTOM([#endif /* _LAM_SSI_COLL_SMP_CONFIG_H */])


#
# Do the normal basics of setup:
# - set CLEANFILES
# - figure out the host type
# - set the default prefix
#
LAM_BASIC_SETUP


#
# Check to see if the user wants this coll to be the default
#

AC_MSG_CHECKING([if want the smp coll to be the default])
want_default=0
result=no
AC_ARG_WITH(coll,
    AC_HELP_STRING([--with-coll=name],
		   [if name is "smp", the smp coll will be the default]))
if test "$with_coll" = "smp"; then
    want_default=1
    result=yes
fi
AC_DEFINE_UNQUOTED(LAM_SSI_COLL_SMP_DEFAULT, $want_default,
    [Whether the smp coll is the default coll or not])
AC_MSG_RESULT([$result])

#
# Check and see if the user wants this module built as a run-time
# loadable module.  Acceptable combinations:
#
# --with-modules
# --with-modules=[.+,]*MODULE_TYPE[.+,]*
# --with-modules=[.+,]*MODULE_TYPE-MODULE_NAME[.+,]*
# --without-modules
#

AC_MSG_CHECKING([if want module to be run-time loadable])
AC_ARG_WITH(modules,
    AC_HELP_STRING([--with-modules=name],
		   [if name is "coll-smp", then smp will be compiled as a loadable module (if supported on this platform).  This directly implies "--enable-shared=coll-smp --disable-static=coll-smp"]))

# Manual conversion of $kind to its generic name (e.g., crmpi->cr,
# crlam->cr).

case "coll" in
crmpi)
    generic_type="cr"
    ;;
crlam)
    generic_type="cr"
    ;;
*)
    generic_type="coll"
;;
esac

BUILD_LOADABLE_MODULE=0
msg=no
if test "$with_modules" = "yes" -o \
    "$with_modules" = "coll" -o "$with_modules" = "$generic_type"; then
    BUILD_LOADABLE_MODULE=1
    msg=yes
elif test "$with_modules" != "no"; then
    ifs_save="$IFS"
    IFS="${IFS}$PATH_SEPARATOR,"
    for module in $with_modules; do
	if test "$module" = "coll-smp" -o \
	    "$module" = "coll" -o "$module" = "$generic_type"; then
	    BUILD_LOADABLE_MODULE=1
	    msg=yes
	fi
    done
    IFS="$ifs_save"
fi
AC_MSG_RESULT([$msg])
unset msg


#
# Part one of libtool magic
#

AM_ENABLE_STATIC
AM_DISABLE_SHARED


#
# Now, if we're building the run-time loadable module, bypass those
# defaults and reset them to shared=yes, static=no.
#

if test "$BUILD_LOADABLE_MODULE" = "1"; then
    enable_shared=yes
    enable_static=no
fi

###########################################################################
# Check for compilers and preprocessors
############################################################################

##################################
# Compiler characteristics
##################################

LAM_SETUP_CC


##################################
# coll smp module specific setup
##################################

if test "$LAM_WANT_DIST" = "no"; then
    true
else
    true
fi

#
# This is needed for VPATH builds, so that it will -I the appropriate
# include directory (don't know why automake doesn't do this
# automatically).  We delayed doing it until now just so that
# '-I$(top_srcdir)' doesn't show up in any of the configure output --
# purely aesthetic.
#

CPPFLAGS='-I$(top_lam_srcdir)/share/include'" $CPPFLAGS"

#
# Delayed the substitution of these until now because they may have
# been modified throughout the course of this script.
#

AC_SUBST(CPPFLAGS)


############################################################################
# libtool magic
############################################################################

lam_show_subtitle "GNU libtool setup" 

AM_PROG_LIBTOOL
AM_CONDITIONAL(LAM_BUILD_LOADABLE_MODULE, test "$BUILD_LOADABLE_MODULE" = "1")


############################################################################
# Party on
############################################################################

lam_show_subtitle "Final output" 

AM_CONFIG_HEADER([src/lam-ssi-coll-smp-config.h])
AC_CONFIG_FILES([Makefile config/Makefile src/Makefile])
AC_OUTPUT
