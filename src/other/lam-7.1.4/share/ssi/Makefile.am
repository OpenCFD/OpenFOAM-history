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
# $Id: Makefile.am,v 1.11 2003/11/15 13:30:11 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

SUBDIRS			= base boot coll crlam crmpi rpi

# Make 2 libraries: one that will be part of liblam.la, and one that
# will be part of libmpi.la.

noinst_LTLIBRARIES	= libssi_lam.la libssi_mpi.la

# LAM SSI library

libssi_lam_la_SOURCES	=
libssi_lam_la_LIBADD	= \
			base/libssi_lam_base.la \
			boot/libssi_boot.la \
			crlam/libssi_crlam.la
libssi_lam_la_DEPENDENCIES= $(libssi_lam_la_LIBADD)

# MPI SSI library

libssi_mpi_la_SOURCES=
libssi_mpi_la_LIBADD	= \
			base/libssi_mpi_base.la \
			coll/libssi_coll.la \
			rpi/libssi_rpi.la \
			crmpi/libssi_crmpi.la
libssi_mpi_la_DEPENDENCIES= $(libssi_mpi_la_LIBADD)
