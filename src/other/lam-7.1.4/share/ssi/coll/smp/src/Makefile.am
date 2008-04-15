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
# $Id: Makefile.am,v 1.6 2003/10/22 23:19:47 adharurk Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
			-I$(top_lam_builddir)/share/include \
			-I$(top_lam_srcdir)/share/include \
			-I$(top_lam_srcdir)/share/ssi/coll/lam_basic/src

noinst_LTLIBRARIES	= libsmp.la
libsmp_la_SOURCES	= \
			lam-ssi-coll-smp.h \
			ssi_coll_smp.c \
			ssi_coll_smp_allreduce.c \
			ssi_coll_smp_allgather.c \
			ssi_coll_smp_allgatherv.c \
			ssi_coll_smp_bcast.c \
			ssi_coll_smp_barrier.c \
			ssi_coll_smp_module.c \
			ssi_coll_smp_reduce.c \
			ssi_coll_smp_reduce_scatter.c \
			ssi_coll_smp_scan.c \
			ssi_coll_smp_util.c
