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
# $Id: Makefile.am,v 1.1 2004/02/17 16:39:55 adharurk Exp $
#

# Use the top-level LAM Makefile.options

include $(top_lam_srcdir)/config/Makefile.options

SUBDIRS                 = src
DIST_SUBDIRS            = config $(SUBDIRS)

EXTRA_DIST              = VERSION

# According to the SSI spec, we have to make the output library here
# in the top-level directory, and it has to be named
# liblam_ssi_coll_shmem.la

if LAM_BUILD_LOADABLE_MODULE
module_noinst	=
module_install	= ssi_coll_shmem.la
else
module_noinst	= liblam_ssi_coll_shmem.la
module_install	=
endif

lamssiexecdir			= $(libdir)/lam
lamssiexec_LTLIBRARIES		= $(module_install)
ssi_coll_shmem_la_SOURCES	=
ssi_coll_shmem_la_LIBADD	= \
				src/libshmem.la \
				$(top_lam_builddir)/share/libmpi/libmpi.la \
				$(top_lam_builddir)/share/liblam/liblam.la
ssi_coll_shmem_la_LDFLAGS	= -module -avoid-version

noinst_LTLIBRARIES		= $(module_noinst)
liblam_ssi_coll_shmem_la_SOURCES=
liblam_ssi_coll_shmem_la_LIBADD= src/libshmem.la
liblam_ssi_coll_shmem_la_LDFLAGS= -module -avoid-version
