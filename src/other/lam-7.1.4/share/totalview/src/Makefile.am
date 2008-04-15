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
# $Id: Makefile.am,v 1.7 2003/07/07 23:22:15 jsquyres Exp $
#

AM_CPPFLAGS		= -I$(top_lam_srcdir)/share/include \
			  -I$(top_lam_builddir)/share/include \
			  -DLAM_BUILDING=1
pkglib_LTLIBRARIES	= liblam_totalview.la
liblam_totalview_la_SOURCES= dll_lam.c mpi_interface.h lam_dll_defs.h
liblam_totalview_la_LDFLAGS = -module
