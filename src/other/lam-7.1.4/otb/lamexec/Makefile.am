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
# $Id: Makefile.am,v 1.23 2003/11/24 19:35:02 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libltdl			= $(base)/libltdl/libltdlc.la
libs			= $(liblam) $(libltdl)

# This is ugly, but we need it so that people can change things at
# "make" time (e.g., "make sysconfdir=/foo/bar all") as documented in
# autoconf.

AM_CPPFLAGS		= -DLAM_SYSCONFDIR="\"$(sysconfdir)\""

bin_PROGRAMS		= lamexec
lamexec_SOURCES		= lamexec.c
lamexec_LDADD		= $(libs) $(LIBLAM_EXTRA_LIBS)
lamexec_LDFLAGS		= $(LIBLAM_EXTRA_LDFLAGS)
lamexec_DEPENDENCIES	= $(libs)
