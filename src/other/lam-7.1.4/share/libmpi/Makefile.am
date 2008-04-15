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
# $Id: Makefile.am,v 1.1 2003/11/15 14:09:55 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

if WANT_IMPI_BUILD
impi_lib		= $(top_builddir)/share/impi/liblamimpi.la
else
impi_lib		=
endif

if WANT_PROFILE_BUILD
pmpi_lib		= $(top_builddir)/share/pmpi/libpmpi.la
else
pmpi_lib		=
endif

# Note that mpi/liblamextra.la gets built into both liblam.la and
# in both libraries, and avoids a dependency from liblam -> libmpi
# (dependencies the other way are ok).

lib_LTLIBRARIES		= libmpi.la

# SSI and MPI libraries get munged together.

libmpi_la_SOURCES	=
libmpi_la_LIBADD	= \
			$(top_builddir)/share/mpi/libmpi.la \
			$(top_builddir)/share/mpi/libmpiextra.la \
			$(top_builddir)/share/ssi/libssi_mpi.la \
			$(top_builddir)/share/memory/libmemory.la \
			$(impi_lib) $(pmpi_lib)
libmpi_la_LDFLAGS	= $(EXTRA_LDFLAGS)
