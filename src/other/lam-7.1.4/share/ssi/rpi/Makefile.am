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
# $Id: Makefile.am,v 1.4 2003/11/15 13:39:55 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

SUBDIRS			= base $(LAM_SSI_rpi_STATIC_SUBDIRS)
DIST_SUBDIRS		= base $(LAM_SSI_rpi_ALL_SUBDIRS)

noinst_LTLIBRARIES	= libssi_rpi.la
libssi_rpi_la_SOURCES	=
libssi_rpi_la_LIBADD	= \
			base/libssi_rpi_base.la \
			$(LAM_SSI_rpi_STATIC_LTLIBS)
libssi_rpi_la_DEPENDENCIES= $(libssi_rpi_la_LIBADD)

