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
# $Id: Makefile.am,v 1.2 2004/03/09 16:56:09 pcharapa Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libltdl = $(base)/libltdl/libltdlc.la
libs			= $(liblam) $(libltdl)

AM_CPPFLAGS		= \
			-DLAM_SYSCONFDIR="\"$(sysconfdir)\"" \
			-DLAM_BINDIR="\"$(bindir)\"" \
			$(LIBCR_INCLUDE)

bin_PROGRAMS		= lamcheckpoint
lamcheckpoint_SOURCES	= lamcheckpoint.c
lamcheckpoint_LDADD		= $(libs) $(LIBLAM_EXTRA_LIBS)
lamcheckpoint_LDFLAGS	= $(LIBLAM_EXTRA_LDFLAGS)
lamcheckpoint_DEPENDENCIES	= $(libs)
