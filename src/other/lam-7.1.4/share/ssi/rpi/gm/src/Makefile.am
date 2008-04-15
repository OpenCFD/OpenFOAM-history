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
# $Id: Makefile.am,v 1.22 2003/11/15 16:00:09 jsquyres Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
			-DLAM_SSI_RPI_GM_TINYMSGLEN=@GMTINY@ \
			-DLAM_SSI_RPI_GM_SHORTMSGLEN=@GMSHORT@ \
			-DLAM_SSI_RPI_GM_MAX_PORT_NUM=@GMMAXPORT@ \
			-DLAM_SSI_RPI_GM_PORT=@GMPORT@ \
			-I$(top_lam_builddir)/share/include \
			-I$(top_lam_srcdir)/share/include

sysconf_DATA		= lam-ssi-rpi-gm-helpfile
EXTRA_DIST		= \
			rpi_gm_cr.h \
			ssi_rpi_gm_cr.c \
			ssi_rpi_gm_long.c \
			ssi_rpi_gm_long_get.c \
			$(sysconf_DATA)

if USE_LONG_GET_PROTOCOL
long_protocol_source	= ssi_rpi_gm_long_get.c
else
long_protocol_source	= ssi_rpi_gm_long.c
endif

if WANT_CHECKPOINT
checkpoint_source	= rpi_gm_cr.h ssi_rpi_gm_cr.c
else
checkpoint_source	=
endif

noinst_LTLIBRARIES	= libgm.la
libgm_la_SOURCES	= \
			$(long_protocol_source) \
			$(checkpoint_source) \
			lam-ssi-rpi-gm.h \
			lam-ssi-rpi-gm-config.h \
			lam-ssi-rpi-gm.h \
			rpi_gm.h \
			rpi_gm_ack.h \
			rpi_gm_actions.h \
			rpi_gm_cbuf.h \
			rpi_gm_dreg.h \
			rpi_gm_interval.h \
			rpi_gm_long.h \
			rpi_gm_proc.h \
			rpi_gm_recv_events.h \
			rpi_gm_recv_queue.h \
			rpi_gm_req.h \
			rpi_gm_reqlist.h \
			rpi_gm_send_queue.h \
			rpi_gm_tiny.h \
			rpi_gm_unexpected.h \
			rpi_gm_util.h \
			ssi_rpi_gm_ack.c \
			ssi_rpi_gm_actions.c \
			ssi_rpi_gm_bookmarks_alltoall.c \
			ssi_rpi_gm_dreg.c \
			ssi_rpi_gm_fast.c \
			ssi_rpi_gm_interval.c \
			ssi_rpi_gm_module.c \
			ssi_rpi_gm_recv_events.c \
			ssi_rpi_gm_recv_queue.c \
			ssi_rpi_gm_reqlist.c \
			ssi_rpi_gm_send_queue.c \
			ssi_rpi_gm.c \
			ssi_rpi_gm_tiny.c \
			ssi_rpi_gm_unexpected.c \
			ssi_rpi_gm_util.c
