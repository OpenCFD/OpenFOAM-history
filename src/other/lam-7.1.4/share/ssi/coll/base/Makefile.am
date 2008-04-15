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
# $Id: Makefile.am,v 1.10 2003/11/23 15:10:54 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

AM_CPPFLAGS                     = \
			        -I$(top_srcdir)/share/ssi/coll/lam_basic/src

noinst_LTLIBRARIES		= libssi_coll_base.la
libssi_coll_base_la_SOURCES	= \
				ssi_coll_checkpoint.c \
				ssi_coll_close.c \
				ssi_coll_init_comm.c \
				ssi_coll_finalize_comm.c \
				ssi_coll_get_param.c \
				ssi_coll_open.c \
				ssi_coll_query.c

distclean-local:
	-rm -f lam-ssi-coll-modules.h
