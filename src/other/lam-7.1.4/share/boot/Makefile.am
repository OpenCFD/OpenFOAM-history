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
# $Id: Makefile.am,v 1.17 2003/04/04 04:44:06 vsahay Exp $
#

include $(top_srcdir)/config/Makefile.options

AM_CPPFLAGS			= \
				-DLAM_SYSCONFDIR="\"$(sysconfdir)\""

noinst_LTLIBRARIES	= liblamboot.la
liblamboot_la_SOURCES	= \
			  app_mgmt.c \
			  asc_parse.c \
			  asc_run.c \
			  asc_schedule.c \
			  asc_workdir.c \
			  bhostparse.c \
			  deallocate.c \
			  hbootparse.c \
			  lamnet.c \
			  proc_schema.c
