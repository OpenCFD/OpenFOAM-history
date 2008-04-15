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
# $Id: Makefile.am,v 1.28 2004/01/12 22:36:12 pkambadu Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
bootdir			= $(base)/boot
incdirs			= -I$(bootdir)
libs			= $(liblam)

# This is ugly, but we need it so that people can change things at
# "make" time (e.g., "make sysconfdir=/foo/bar all") as documented in
# autoconf.

AM_CPPFLAGS		= -DLAM_SYSCONFDIR="\"$(sysconfdir)\""

bin_PROGRAMS		= recon
recon_SOURCES		= recon.c
recon_LDADD		= $(libs) $(LIBLAM_EXTRA_LIBS)
recon_LDFLAGS		= $(LIBLAM_EXTRA_LDFLAGS)
recon_DEPENDENCIES	= $(libs)
