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
# $Id: Makefile.am,v 1.23 2003/11/23 13:42:23 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

AM_CPPFLAGS			= \
				-DLAM_SYSCONFDIR="\"$(sysconfdir)\"" \
				-DLAM_LIBDIR="\"$(pkglibdir)\"" \
				-DLAM_PKGLIBDIR="\"$(pkglibdir)\""

noinst_LTLIBRARIES		= libssi_lam_base.la libssi_mpi_base.la
libssi_mpi_base_la_SOURCES	= \
				ssi_mpi_init_callback.c \
				ssi_mpi_module_select.c \
				ssi_mpi_param.c

libssi_lam_base_la_SOURCES	= \
				ssi_ao.c \
				ssi_close.c \
				ssi_set_verbose.c \
				ssi_module_check.c \
				ssi_module_compare.c \
				ssi_module_find.c \
				ssi_module_registry.c \
				ssi_open.c \
				ssi_hostmap.c \
				ssi_param.c
