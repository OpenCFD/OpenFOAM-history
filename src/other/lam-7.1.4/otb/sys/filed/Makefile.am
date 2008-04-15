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
# $Id: Makefile.am,v 1.20 2003/11/15 13:17:47 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libs			= $(liblam)

# Because Automake sucks, we have to have a level of indirection about
# whether or not to build the binary...
lam_target		= lamd_filed

if WANT_TRILLIUM_INSTALL
lam_prog		= $(lam_target)
else
lam_prog		=
endif

filed_lib_sources	= \
			  fdesc.c \
			  fface.c \
			  filed.c \
			  filed.h \
			  fqclose.c \
			  fqf77.c \
			  fqlseek.c \
			  fqopen.c \
			  fqopenfd.c \
			  fqpathfind.c \
			  fqread.c \
			  fqsimplex.c \
			  fqstate.c \
			  fqwrite.c
filed_bin_sources	= \
			$(filed_lib_sources) \
			f_main.c

noinst_LIBRARIES	= liblamdfile.a
liblamdfile_a_SOURCES	= $(filed_lib_sources)

bin_PROGRAMS		= $(lam_prog)
lamd_filed_SOURCES	= $(filed_bin_sources)
lamd_filed_LDADD	= $(libs) $(LIBLAM_EXTRA_LIBS)
lamd_filed_LDFLAGS	= $(LIBLAM_EXTRA_LDFLAGS)
lamd_filed_DEPENDENCIES	= $(libs)

CLEANFILES		= *~ $(lam_target)

EXTRA_DIST		= $(filed_bin_sources)
