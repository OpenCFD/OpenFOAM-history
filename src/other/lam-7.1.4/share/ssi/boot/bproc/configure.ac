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

AC_INIT(src/ssi_boot_bproc.c)
AC_PREREQ(2.57)
AC_CONFIG_AUX_DIR(config)

# Establish the top-level LAM directory

top_lam_srcdir='$(top_srcdir)/../../../..'
top_lam_builddir='$(top_builddir)/../../../..'

AC_SUBST(top_lam_srcdir)
AC_SUBST(top_lam_builddir)

# Get the version of boot bproc that we are installing.

LAM_GET_VERSION($srcdir/../../../../config, $srcdir/VERSION,
  LAM_SSI_BOOT_BPROC)

AC_DEFINE_UNQUOTED(LAM_SSI_BOOT_BPROC_MAJOR_VERSION, 
    $LAM_SSI_BOOT_BPROC_MAJOR_VERSION, 
    [Major LAM SSI boot bproc version])
AC_DEFINE_UNQUOTED(LAM_SSI_BOOT_BPROC_MINOR_VERSION, 
    $LAM_SSI_BOOT_BPROC_MINOR_VERSION, 
    [Minor LAM SSI boot bproc version])
AC_DEFINE_UNQUOTED(LAM_SSI_BOOT_BPROC_RELEASE_VERSION, 
    $LAM_SSI_BOOT_BPROC_RELEASE_VERSION, 
    [Release LAM SSI boot bproc version])
AC_DEFINE_UNQUOTED(LAM_SSI_BOOT_BPROC_ALPHA_VERSION, 
    $LAM_SSI_BOOT_BPROC_ALPHA_VERSION, 
    [Alpha LAM SSI boot bproc version])
AC_DEFINE_UNQUOTED(LAM_SSI_BOOT_BPROC_BETA_VERSION, 
    $LAM_SSI_BOOT_BPROC_BETA_VERSION, 
    [Beta LAM SSI boot bproc version])
AC_DEFINE_UNQUOTED(LAM_SSI_BOOT_BPROC_CVS_VERSION, 
    $LAM_SSI_BOOT_BPROC_CVS_VERSION, 
    [CVS LAM SSI boot bproc version])
AC_DEFINE_UNQUOTED(LAM_SSI_BOOT_BPROC_VERSION, 
    "$LAM_SSI_BOOT_BPROC_VERSION", 
    [Overall LAM SSI boot bproc version number])

#
# Start it up
#

LAM_CONFIGURE_SETUP
lam_show_title "Configuring SSI boot bproc module version $LAM_SSI_BOOT_BPROC_VERSION"
lam_show_subtitle "Initialization, setup" 

#
# Init automake
# The third argument to AM_INIT_AUTOMAKE surpresses the PACKAGE and
# VERSION macors
#

AM_INIT_AUTOMAKE(boot-bproc, $LAM_SSI_BOOT_BPROC_VERSION, 'no')

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
 * SSI boot: bproc module
 */

#ifndef LAM_SSI_BOOT_BPROC_CONFIG_H
#define LAM_SSI_BOOT_BPROC_CONFIG_H
])
AH_BOTTOM([#endif /* _LAM_SSI_BOOT_BPROC_CONFIG_H */])


#
# Do the normal basics of setup:
# - set CLEANFILES
# - figure out the host type
# - set the default prefix
#
LAM_BASIC_SETUP


#
# Check to see if the user wants this boot to be the default
#

AC_MSG_CHECKING([if want the bproc boot to be the default])
want_default=0
result=no
AC_ARG_WITH(boot,
    AC_HELP_STRING([--with-boot=name],
		   [if name is "bproc", the bproc boot will be the default]))
if test "$with_boot" = "bproc"; then
    want_default=1
    result=yes
fi
AC_DEFINE_UNQUOTED(LAM_SSI_BOOT_BPROC_DEFAULT, $want_default,
    [Whether the bproc boot is the default boot or not])
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
		   [if name is "boot-bproc", then bproc will be compiled as a loadable module (if supported on this platform).  This directly implies "--enable-shared=boot-bproc --disable-static=boot-bproc"]))

# Manual conversion of $kind to its generic name (e.g., crmpi->cr,
# crlam->cr).

case "boot" in
crmpi)
    generic_type="cr"
    ;;
crlam)
    generic_type="cr"
    ;;
*)
    generic_type="boot"
;;
esac

BUILD_LOADABLE_MODULE=0
msg=no
if test "$with_modules" = "yes" -o \
    "$with_modules" = "boot" -o "$with_modules" = "$generic_type"; then
    BUILD_LOADABLE_MODULE=1
    msg=yes
elif test "$with_modules" != "no"; then
    ifs_save="$IFS"
    IFS="${IFS}$PATH_SEPARATOR,"
    for module in $with_modules; do
	if test "$module" = "boot-bproc" -o \
	    "$module" = "boot" -o "$module" = "$generic_type"; then
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
# boot bproc module specific setup
##################################

if test "$LAM_WANT_DIST" = "no"; then
    lam_show_subtitle "SSI boot bproc-specific setup"
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

AM_CONFIG_HEADER([src/lam-ssi-boot-bproc-config.h])
AC_CONFIG_FILES([Makefile config/Makefile src/Makefile])
AC_OUTPUT
