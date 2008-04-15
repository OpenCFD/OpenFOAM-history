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
# $Id: Makefile.am,v 1.7 2003/11/15 13:18:03 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libs			= $(liblam)

# Because Automake sucks, we have to have a level of indirection about
# whether or not to build the binary...
lam_target		= lamd_versiond

if WANT_TRILLIUM_INSTALL
lam_prog		= $(lam_target)
else
lam_prog		=
endif

versiond_lib_sources	= versiond.c
versiond_bin_sources	= \
			$(versiond_lib_sources) \
			v_main.c

noinst_LIBRARIES	= liblamdversion.a
liblamdversion_a_SOURCES= $(versiond_lib_sources)

bin_PROGRAMS		= $(lam_prog)
lamd_versiond_SOURCES	= $(versiond_bin_sources)
lamd_versiond_LDADD	= $(libs) $(LIBLAM_EXTRA_LIBS)
lamd_versiond_LDFLAGS	= $(LIBLAM_EXTRA_LDFLAGS)
lamd_versiond_DEPENDENCIES= $(libs)

CLEANFILES		= *~ $(lam_target)

EXTRA_DIST		= $(versiond_bin_sources)
