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
# $Id: Makefile.am,v 1.23 2003/11/24 18:49:09 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libltdl			= $(base)/libltdl/libltdlc.la
libs			= $(liblam) $(libltdl)
AM_CPPFLAGS		= -DLAM_BUILDING=1

bin_PROGRAMS		= lamgrow
lamgrow_SOURCES		= lamgrow.c
lamgrow_LDADD		= $(libs) $(LIBLAM_EXTRA_LIBS)
lamgrow_LDFLAGS		= $(LIBLAM_EXTRA_LDFLAGS)
lamgrow_DEPENDENCIES	= $(libs)
