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
# $Id: Makefile.am,v 1.7 2003/07/07 23:22:14 jsquyres Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

# figure out what we need for assembly
if LAM_ASSEMBLY_POWERPC_DARWIN
darwin_sync_assembly=ssi_rpi_usysv_syncwrite_powerpc_darwin.s
else
darwin_sync_assembly=
endif

if LAM_ASSEMBLY_POWERPC_LINUX
linux_sync_assembly=ssi_rpi_usysv_syncwrite_powerpc_linux.s
else
linux_sync_assembly=
endif

if LAM_ASSEMBLY_POWERPC_AIX
aix_sync_assembly=ssi_rpi_usysv_syncwrite_powerpc_aix.s
else
aix_sync_assembly=
endif


if LAM_ASSEMBLY_POWERPC_AIX64
aix64_sync_assembly=ssi_rpi_usysv_syncwrite_powerpc_aix64.s
else
aix64_sync_assembly=
endif


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

noinst_LTLIBRARIES	= libusysv.la
libusysv_la_SOURCES	= \
			$(darwin_sync_assembly) \
			$(linux_sync_assembly) \
			$(aix_sync_assembly) \
			$(aix64_sync_assembly) \
			lam-ssi-rpi-usysv.h \
			rpi_usysv.h \
			rpi_usysv_macros.h \
			rpi_usysv_proc.h \
			rpi_usysv_req.h \
			ssi_rpi_usysv.c \
			ssi_rpi_usysv_common.c \
			ssi_rpi_usysv_low.c \
			ssi_rpi_usysv_module.c \
			ssi_rpi_usysv_shm.c \
			usysv_common.h \
			usysv_low.h \
			usysv_shmem.h \
			usysv_syncwrite.h \
			usysv_syncwrite_powerpc.h

EXTRA_DIST		= \
			ssi_rpi_usysv_syncwrite_powerpc_darwin.s \
			ssi_rpi_usysv_syncwrite_powerpc_linux.s \
			ssi_rpi_usysv_syncwrite_powerpc_aix.s \
			ssi_rpi_usysv_syncwrite_powerpc_aix64.s
