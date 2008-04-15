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
# $Id: Makefile.am,v 1.19 2003/11/15 13:11:20 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

# Do some conditionals to see if we want to install the full set of
# Trillium binary files.  This is not required for correct MPI
# functionality -- it is only required for developers who want to use
# the underlying Trillium interface.  We do the conditional here
# rather than in the upper level file so that targets like clean,
# distclean, and dist still come through here, regardless of the
# current ./configure options.

# Have to do some weirdness to ensure that we get everything -- a
# double redirection on $(prog) (which gets loaded into
# $(bin_PROGRAMS), but we have to supply the sources in EXTRA_DIST and
# a special rule for clean-local.

lam_target		= loadgo

if WANT_TRILLIUM_INSTALL
lam_prog		= $(lam_target)
else
lam_prog		=
endif

liblam			= $(top_builddir)/share/liblam/liblam.la
libs			= $(liblam)

bin_PROGRAMS		= $(lam_prog)
loadgo_SOURCES		= loadgo.c 
loadgo_LDADD		= $(libs) $(LIBLAM_EXTRA_LIBS)
loadgo_LDFLAGS		= $(LIBLAM_EXTRA_LDFLAGS)
loadgo_DEPENDENCIES	= $(libs)

CLEANFILES		= *~ $(lam_target)

EXTRA_DIST		= $(loadgo_SOURCES)
