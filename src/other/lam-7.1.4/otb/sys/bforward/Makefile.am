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
# $Id: Makefile.am,v 1.18 2003/11/15 13:17:33 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libs			= $(liblam)

# Because Automake sucks, we have to have a level of indirection about
# whether or not to build the binary...
lam_target		= lamd_bforward

if WANT_TRILLIUM_INSTALL
lam_prog		= $(lam_target)
else
lam_prog		=
endif

bforward_lib_sources 	= bforward.c
bforward_bin_sources	= $(bforward_lib_sources) bw_main.c

noinst_LIBRARIES	= liblamdbforward.a
liblamdbforward_a_SOURCES= $(bforward_lib_sources)

bin_PROGRAMS		= $(lam_prog)
lamd_bforward_SOURCES	= $(bforward_bin_sources)
lamd_bforward_LDADD	= $(libs) $(LIBLAM_EXTRA_LIBS)
lamd_bforward_LDFLAGS	= $(LIBLAM_EXTRA_LDFLAGS)
lamd_bforward_DEPENDENCIES= $(libs)

CLEANFILES		= *~ $(lam_target)

EXTRA_DIST		= $(bforward_bin_sources)
