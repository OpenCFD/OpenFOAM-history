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
# $Id: Makefile.am,v 1.19 2003/11/15 13:17:39 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libs			= $(liblam)

# Because Automake sucks, we have to have a level of indirection about
# whether or not to build the binary...
lam_target		= lamd_bufferd

if WANT_TRILLIUM_INSTALL
lam_prog		= $(lam_target)
else
lam_prog		=
endif

bufferd_lib_sources	= bufferd.c
bufferd_bin_sources	= $(bufferd_lib_sources) bf_main.c

noinst_LIBRARIES	= liblamdbuffer.a
liblamdbuffer_a_SOURCES	= $(bufferd_lib_sources)

bin_PROGRAMS		= $(lam_prog)
lamd_bufferd_SOURCES    = $(bufferd_bin_sources)
lamd_bufferd_LDADD	= $(libs) $(LIBLAM_EXTRA_LIBS)
lamd_bufferd_LDFLAGS	= $(LIBLAM_EXTRA_LDFLAGS)
lamd_bufferd_DEPENDENCIES= $(libs)

CLEANFILES		= *~ $(lam_target)

EXTRA_DIST		= $(bufferd_bin_sources)
