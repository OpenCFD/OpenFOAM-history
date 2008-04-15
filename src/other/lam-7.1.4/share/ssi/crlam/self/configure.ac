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

AC_INIT(src/ssi_crlam_self.c)
AC_PREREQ(2.57)
AC_CONFIG_AUX_DIR(config)

# Establish the top-level LAM directory

top_lam_srcdir='$(top_srcdir)/../../../..'
top_lam_builddir='$(top_builddir)/../../../..'

AC_SUBST(top_lam_srcdir)
AC_SUBST(top_lam_builddir)

# Get the version of crlam self that we are installing.

LAM_GET_VERSION($srcdir/../../../../config, $srcdir/VERSION,
  LAM_SSI_CRLAM_SELF)

AC_DEFINE_UNQUOTED(LAM_SSI_CRLAM_SELF_MAJOR_VERSION, 
    $LAM_SSI_CRLAM_SELF_MAJOR_VERSION, 
    [Major LAM SSI crlam self version])
AC_DEFINE_UNQUOTED(LAM_SSI_CRLAM_SELF_MINOR_VERSION, 
    $LAM_SSI_CRLAM_SELF_MINOR_VERSION, 
    [Minor LAM SSI crlam self version])
AC_DEFINE_UNQUOTED(LAM_SSI_CRLAM_SELF_RELEASE_VERSION, 
    $LAM_SSI_CRLAM_SELF_RELEASE_VERSION, 
    [Release LAM SSI crlam self version])
AC_DEFINE_UNQUOTED(LAM_SSI_CRLAM_SELF_ALPHA_VERSION, 
    $LAM_SSI_CRLAM_SELF_ALPHA_VERSION, 
    [Alpha LAM SSI crlam self version])
AC_DEFINE_UNQUOTED(LAM_SSI_CRLAM_SELF_BETA_VERSION, 
    $LAM_SSI_CRLAM_SELF_BETA_VERSION, 
    [Beta LAM SSI crlam self version])
AC_DEFINE_UNQUOTED(LAM_SSI_CRLAM_SELF_CVS_VERSION, 
    $LAM_SSI_CRLAM_SELF_CVS_VERSION, 
    [CVS LAM SSI crlam self version])
AC_DEFINE_UNQUOTED(LAM_SSI_CRLAM_SELF_VERSION, 
    "$LAM_SSI_CRLAM_SELF_VERSION", 
    [Overall LAM SSI crlam self version number])

#
# Start it up
#

LAM_CONFIGURE_SETUP
lam_show_title "Configuring SSI crlam self module version $LAM_SSI_CRLAM_SELF_VERSION"
lam_show_subtitle "Initialization, setup" 

#
# Init automake
# The third argument to AM_INIT_AUTOMAKE surpresses the PACKAGE and
# VERSION macors
#

AM_INIT_AUTOMAKE(crlam-self, $LAM_SSI_CRLAM_SELF_VERSION, 'no')

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
 * SSI crlam: self module
 */

#ifndef LAM_SSI_CRLAM_SELF_CONFIG_H
#define LAM_SSI_CRLAM_SELF_CONFIG_H
])
AH_BOTTOM([#endif /* _LAM_SSI_CRLAM_SELF_CONFIG_H */])


#
# Do the normal basics of setup:
# - set CLEANFILES
# - figure out the host type
# - set the default prefix
#
LAM_BASIC_SETUP


#
# Check to see if the user wants this crlam to be the default
#

AC_MSG_CHECKING([if want the self crlam to be the default])
want_default=0
result=no
AC_ARG_WITH(cr,
    AC_HELP_STRING([--with-cr=name],
		   [if name is "self", the self crlam will be the default]))
if test "$with_cr" = "self"; then
    want_default=1
    result=yes
fi
AC_DEFINE_UNQUOTED(LAM_SSI_CRLAM_SELF_DEFAULT, $want_default,
    [Whether the self crlam is the default cr or not])
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
		   [if name is "crlam-self", then self will be compiled as a loadable module (if supported on this platform).  This directly implies "--enable-shared=crlam-self --disable-static=crlam-self"]))

# Manual conversion of $kind to its generic name (e.g., crmpi->cr,
# crlam->cr).

case "crlam" in
crmpi)
    generic_type="cr"
    ;;
crlam)
    generic_type="cr"
    ;;
*)
    generic_type="crlam"
;;
esac

BUILD_LOADABLE_MODULE=0
msg=no
if test "$with_modules" = "yes" -o \
    "$with_modules" = "crlam" -o "$with_modules" = "$generic_type"; then
    BUILD_LOADABLE_MODULE=1
    msg=yes
elif test "$with_modules" != "no"; then
    ifs_save="$IFS"
    IFS="${IFS}$PATH_SEPARATOR,"
    for module in $with_modules; do
	if test "$module" = "crlam-self" -o \
	    "$module" = "crlam" -o "$module" = "$generic_type"; then
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
LAM_SETUP_CXX

##################################
# crlam self module specific setup
##################################

if test "$LAM_WANT_DIST" = "no"; then
    lam_show_subtitle "SSI crlam self-specific setup"
SSI_CONFIGURE_STUB
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

AM_CONFIG_HEADER([src/lam-ssi-crlam-self-config.h])
AC_CONFIG_FILES([Makefile config/Makefile src/Makefile])
AC_OUTPUT
