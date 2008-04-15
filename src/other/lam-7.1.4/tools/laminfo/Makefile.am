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
# $Id: Makefile.am,v 1.10 2004/01/12 22:36:12 pkambadu Exp $
#

include $(top_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
			-DLAM_PREFIX="\"$(prefix)\"" \
			-DLAM_BINDIR="\"$(bindir)\"" \
			-DLAM_LIBDIR="\"$(libdir)\"" \
			-DLAM_INCDIR="\"$(includedir)\"" \
			-DLAM_PKGLIBDIR="\"$(pkglibdir)\"" \
			-DLAM_SYSCONFDIR="\"$(sysconfdir)\""

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libmpi			= $(base)/libmpi/libmpi.la
libs			= $(libmpi) $(liblam) 

bin_PROGRAMS		= laminfo
laminfo_SOURCES		= laminfo.cc
laminfo_LDADD		= $(libs) $(LIBMPI_EXTRA_LIBS) $(LIBLAM_EXTRA_LIBS)
laminfo_LDFLAGS		= $(LIBMPI_EXTRA_LDFLAGS) $(LIBLAM_EXTRA_LDFLAGS)
laminfo_DEPENDENCIES	= $(libs)

clean-local:
	test -z "$(LAM_CXX_TEMPLATE_REPOSITORY)" || $(RM) -rf $(LAM_CXX_TEMPLATE_REPOSITORY)
