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
# $Id: Makefile.am,v 1.4 2003/12/24 23:44:52 vsahay Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
			-DLAM_SSI_RPI_IB_TINYMSGLEN=@IBTINY@ \
			-DLAM_SSI_RPI_IB_PORT=@IBPORT@ \
			-DLAM_SSI_RPI_IB_HCA_ID="@IBHCAID@" \
			-DLAM_SSI_RPI_IB_NUM_ENVELOPES=@IBNUMENVELOPES@ \
			-I$(top_lam_builddir)/share/include \
			-I$(top_lam_srcdir)/share/include

sysconf_DATA		= lam-ssi-rpi-ib-helpfile
EXTRA_DIST		= \
			ssi_rpi_ib_fast.c \
			$(sysconf_DATA)

noinst_LTLIBRARIES	= libib.la
libib_la_SOURCES	= \
			lam-ssi-rpi-ib.h \
			lam-ssi-rpi-ib-config.h \
			lam-ssi-rpi-ib.h \
			rpi_ib.h \
			rpi_ib_ack.h \
			rpi_ib_actions.h \
			rpi_ib_api_map.h \
			rpi_ib_bitmap.h \
			rpi_ib_cbuf.h \
			rpi_ib_dreg.h \
			rpi_ib_fast.h \
			rpi_ib_hashmap.h \
			rpi_ib_interval.h \
			rpi_ib_long.h \
			rpi_ib_proc.h \
			rpi_ib_recv_events.h \
			rpi_ib_recv_queue.h \
			rpi_ib_req.h \
			rpi_ib_reqlist.h \
			rpi_ib_send_queue.h \
			rpi_ib_tiny.h \
			rpi_ib_unexpected.h \
			rpi_ib_util.h \
			ssi_rpi_ib_ack.c \
			ssi_rpi_ib_actions.c \
			ssi_rpi_ib_bitmap.c \
			ssi_rpi_ib_dreg.c \
			ssi_rpi_ib_hashmap.c \
			ssi_rpi_ib_interval.c \
			ssi_rpi_ib_long.c \
			ssi_rpi_ib_module.c \
			ssi_rpi_ib_recv_events.c \
			ssi_rpi_ib_recv_queue.c \
			ssi_rpi_ib_reqlist.c \
			ssi_rpi_ib_send_queue.c \
			ssi_rpi_ib.c \
			ssi_rpi_ib_tiny.c \
			ssi_rpi_ib_unexpected.c \
			ssi_rpi_ib_util.c


