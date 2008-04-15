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
# $Id: Makefile.am,v 1.14 2003/07/07 23:22:13 jsquyres Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
			-DCRTCP_DO_FAST=0 \
			-DLAM_CRTCPSHORTMSGLEN=@CRTCPSHORT@ \
			-I$(top_lam_builddir)/share/include \
			-I$(top_lam_srcdir)/share/include

noinst_LTLIBRARIES	= libcrtcp.la
libcrtcp_la_SOURCES	= \
			lam-ssi-rpi-crtcp.h \
			rpi_crtcp.h \
			rpi_crtcp_bookmarks.h \
			rpi_crtcp_macros.h \
			rpi_crtcp_proc.h \
			rpi_crtcp_req.h \
			ssi_rpi_crtcp.c \
			ssi_rpi_crtcp_bookmarks_alltoall.c \
			ssi_rpi_crtcp_cr.c \
			ssi_rpi_crtcp_module.c \
			ssi_rpi_crtcp_low.c \
			crtcp_low.h 
