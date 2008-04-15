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
# $Id: Makefile.am,v 1.20 2003/11/23 15:10:54 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

AM_CPPFLAGS			= -DLAM_SYSCONFDIR="\"$(sysconfdir)\""

noinst_LTLIBRARIES		= libssi_boot_base.la
libssi_boot_base_la_SOURCES	= \
				ssi_boot_alg_linear.c \
				ssi_boot_alg_linear_windowed.c \
				ssi_boot_alg_thread.c \
				ssi_boot_alg_tree.c \
				ssi_boot_append_path.c \
				ssi_boot_client.c \
				ssi_boot_close.c \
				ssi_boot_cmd_line.c \
				ssi_boot_do_args.c \
				ssi_boot_find_boot_schema.c \
				ssi_boot_find_hostname.c \
				ssi_boot_find_prefix.c \
				ssi_boot_ioexecvp.c \
				ssi_boot_lamgrow.c \
				ssi_boot_open.c \
				ssi_boot_select.c \
				ssi_boot_server.c

distclean-local:
	-rm -f lam-ssi-boot-modules.h

