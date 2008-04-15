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
# $Id: Makefile.am,v 1.5 2003/10/22 23:24:59 adharurk Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
			-I$(top_lam_builddir)/share/include \
			-I$(top_lam_srcdir)/share/include

noinst_LTLIBRARIES	= liblam_basic.la
liblam_basic_la_SOURCES	= \
			lam-ssi-coll-lam-basic.h \
			ssi_coll_lam_basic.c \
			ssi_coll_lam_basic_util.c \
			ssi_coll_lam_basic_allgather.c \
			ssi_coll_lam_basic_allgatherv.c \
			ssi_coll_lam_basic_allreduce.c \
			ssi_coll_lam_basic_alltoall.c \
			ssi_coll_lam_basic_alltoallv.c \
			ssi_coll_lam_basic_alltoallw.c \
			ssi_coll_lam_basic_barrier.c \
			ssi_coll_lam_basic_bcast.c \
			ssi_coll_lam_basic_gather.c \
			ssi_coll_lam_basic_gatherv.c \
			ssi_coll_lam_basic_module.c \
			ssi_coll_lam_basic_reduce.c \
			ssi_coll_lam_basic_reduce_scatter.c \
			ssi_coll_lam_basic_scan.c \
			ssi_coll_lam_basic_exscan.c \
			ssi_coll_lam_basic_scatter.c \
			ssi_coll_lam_basic_scatterv.c
