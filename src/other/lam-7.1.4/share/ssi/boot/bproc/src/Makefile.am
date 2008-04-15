# -*- makefile -*-
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
# $Id: Makefile.am,v 1.5 2003/07/07 23:22:10 jsquyres Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
			-I$(top_lam_builddir)/share/include \
			-I$(top_lam_srcdir)/share/include \
			-DLAM_SYSCONFDIR="\"$(sysconfdir)\""

sysconf_DATA		= lam-ssi-boot-bproc-helpfile
EXTRA_DIST		= $(sysconf_DATA)

noinst_LTLIBRARIES	= libbproc.la
libbproc_la_SOURCES	= \
			lam-ssi-boot-bproc.h \
			ssi_boot_bproc.c \
			ssi_boot_bproc_lamnode.h \
			ssi_boot_bproc_module.c
