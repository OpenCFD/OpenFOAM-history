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
# $Id: Makefile.am,v 1.13 2003/11/23 15:10:55 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

noinst_LTLIBRARIES		= libssi_rpi_base.la
libssi_rpi_base_la_SOURCES	= \
				ssi_rpi_cbuf.c \
				ssi_rpi_close.c \
				ssi_rpi_fault.c \
				ssi_rpi_init.c \
				ssi_rpi_mem.c \
				ssi_rpi_open.c \
				ssi_rpi_query.c \
				ssi_rpi_util.c

distclean-local:
	-rm -f lam-ssi-rpi-modules.h
