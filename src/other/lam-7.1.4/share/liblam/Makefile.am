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
# $Id: Makefile.am,v 1.2 2003/11/26 23:44:45 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

# Note that mpi/liblamextra.la gets built into both liblam.la and
# in both libraries, and avoids a dependency from liblam -> libmpi
# (dependencies the other way are ok).

lib_LTLIBRARIES		= liblam.la
liblam_la_SOURCES	= 
liblam_la_LIBADD	= \
			$(top_builddir)/share/args/liblamargs.la \
			$(top_builddir)/share/boot/liblamboot.la \
			$(top_builddir)/share/etc/liblametc.la \
			$(top_builddir)/share/freq/liblamfreq.la \
			$(top_builddir)/share/kreq/liblamkreq.la \
			$(top_builddir)/share/libltdl/libltdlc.la \
			$(top_builddir)/share/mpi/liblamextra.la \
			$(top_builddir)/share/nreq/liblamnreq.la \
			$(top_builddir)/share/rreq/liblamrreq.la \
			$(top_builddir)/share/ssi/libssi_lam.la \
			$(top_builddir)/share/threads/liblamthreads.la \
			$(top_builddir)/share/trillium/liblamtrillium.la \
			$(top_builddir)/share/tstdio/liblamtstdio.la
liblam_la_LDFLAGS	= $(EXTRA_LDFLAGS)
