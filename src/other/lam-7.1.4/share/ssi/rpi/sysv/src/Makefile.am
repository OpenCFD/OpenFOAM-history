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

# Somewhat abuse the SSI RPI system here -- this RPI is tied to the
# TCP RPI.  Hence, we need to -I where its .h files live.

AM_CPPFLAGS		= \
			-DLAM_TCPSHORTMSGLEN=@TCPSHORT@ \
			-DLAM_SHMSHORTMSGLEN=@SHMSHORT@ \
			-DLAM_SHMPOOLSIZE=@SHMPOOL@ \
			-DLAM_SHMMAXALLOC=@SHMALLOC@ \
			-I$(top_lam_builddir)/share/include \
			-I$(top_lam_srcdir)/share/include \
			-I$(top_lam_srcdir)/share/ssi/rpi/tcp/src

noinst_LTLIBRARIES	= libsysv.la
libsysv_la_SOURCES	= \
			lam-ssi-rpi-sysv.h \
			rpi_sysv.h \
			rpi_sysv_macros.h \
			rpi_sysv_proc.h \
			rpi_sysv_req.h \
			ssi_rpi_sysv.c \
			ssi_rpi_sysv_common.c \
			ssi_rpi_sysv_low.c \
			ssi_rpi_sysv_module.c \
			ssi_rpi_sysv_shm.c \
			sysv_common.h \
			sysv_low.h \
			sysv_shmem.h
