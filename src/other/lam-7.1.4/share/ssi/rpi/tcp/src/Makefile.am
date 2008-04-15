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
# $Id: Makefile.am,v 1.6 2003/07/07 23:22:14 jsquyres Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
			-DLAM_TCPSHORTMSGLEN=@TCPSHORT@ \
			-I$(top_lam_builddir)/share/include \
			-I$(top_lam_srcdir)/share/include

noinst_LTLIBRARIES	= libtcp.la
libtcp_la_SOURCES	= \
			lam-ssi-rpi-tcp.h \
			rpi_tcp.h \
			rpi_tcp_macros.h \
			rpi_tcp_proc.h \
			rpi_tcp_req.h \
			ssi_rpi_tcp.c \
			ssi_rpi_tcp_module.c \
			ssi_rpi_tcp_low.c \
			tcp_low.h 
