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
# $Id: Makefile.am,v 6.20 2003/11/24 19:35:03 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libltdl			= $(base)/libltdl/libltdlc.la
libs			= $(liblam) $(libltdl)

bin_PROGRAMS		= lamtrace
lamtrace_SOURCES	= lamtrace.c 
lamtrace_LDADD		= $(libs) $(LIBLAM_EXTRA_LIBS)
lamtrace_LDFLAGS	= $(LIBLAM_EXTRA_LDFLAGS)
lamtrace_DEPENDENCIES	= $(libs)
