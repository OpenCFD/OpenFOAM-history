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
# $Id: Makefile.am,v 1.7 2003/11/15 13:58:53 jsquyres Exp $
#

# Use the top-level LAM Makefile.options

include $(top_lam_srcdir)/config/Makefile.options

SUBDIRS			= src
DIST_SUBDIRS		= config src

EXTRA_DIST		= VERSION

# According to the SSI spec, we have to make the output library here
# in the top-level directory, and it has to be named
# liblam_ssi_rpi_gm.la

if LAM_BUILD_LOADABLE_MODULE
module_noinst	=
module_install	= ssi_rpi_gm.la
else
module_noinst	= liblam_ssi_rpi_gm.la
module_install	=
endif

lamssiexecdir			= $(libdir)/lam
lamssiexec_LTLIBRARIES		= $(module_install)
ssi_rpi_gm_la_SOURCES		=
ssi_rpi_gm_la_LIBADD		= \
				src/libgm.la \
				$(top_lam_builddir)/share/libmpi/libmpi.la \
				$(top_lam_builddir)/share/liblam/liblam.la
ssi_rpi_gm_la_LDFLAGS		= -module -avoid-version

noinst_LTLIBRARIES		= $(module_noinst)
liblam_ssi_rpi_gm_la_SOURCES	=
liblam_ssi_rpi_gm_la_LIBADD	= src/libgm.la
liblam_ssi_rpi_gm_la_LDFLAGS	= -module -avoid-version
