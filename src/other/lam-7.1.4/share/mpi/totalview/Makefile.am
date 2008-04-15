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
# $Id: Makefile.am,v 1.2 2003/07/07 23:22:08 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

noinst_LTLIBRARIES	= libmpi_totalview.la

# This is not quite in the Automake spirit, but we have to do it.
# Since the totalview portion of the library must be built with -g, we
# must eliminate the CFLAGS that are passed in here by default (which
# may already have debugging and/or optimization flags).  We use
# post-processed forms of the CFLAGS in the library targets down
# below.

CFLAGS			= $(CFLAGS_WITHOUT_OPTFLAGS) $(TOTALVIEW_DEBUG_FLAGS)

libmpi_totalview_la_SOURCES = types.c
