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
# $Id: Makefile.am,v 1.2 2004/02/19 23:31:36 adharurk Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
			-I$(top_lam_builddir)/share/include \
			-I$(top_lam_srcdir)/share/include \
			-I$(top_lam_srcdir)/share/ssi/coll/lam_basic/src

noinst_LTLIBRARIES	= libshmem.la
libshmem_la_SOURCES	= \
			lam-ssi-coll-shmem.h \
			ssi_coll_shmem.c \
			ssi_coll_shmem_allgather.c \
			ssi_coll_shmem_alltoall.c \
                        ssi_coll_shmem_barrier.c \
			ssi_coll_shmem_bcast.c \
			ssi_coll_shmem_gather.c \
			ssi_coll_shmem_module.c \
			ssi_coll_shmem_reduce.c \
			ssi_coll_shmem_scatter.c \
			ssi_coll_shmem_util.c



