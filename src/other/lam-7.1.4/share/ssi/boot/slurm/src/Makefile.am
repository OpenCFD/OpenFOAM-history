# -*- makefile -*-
#
# Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
# $Id: Makefile.am,v 1.1 2003/12/08 21:55:34 jsquyres Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
			-I$(top_lam_builddir)/share/include \
			-I$(top_lam_srcdir)/share/include \
			-DLAM_SYSCONFDIR="\"$(sysconfdir)\""

sysconf_DATA		= lam-ssi-boot-slurm-helpfile
EXTRA_DIST		= $(sysconf_DATA)

noinst_LTLIBRARIES	= libslurm.la
libslurm_la_SOURCES	= \
			lam-ssi-boot-slurm.h \
			ssi_boot_slurm.c \
			ssi_boot_slurm_hostlist.c \
			ssi_boot_slurm_module.c
