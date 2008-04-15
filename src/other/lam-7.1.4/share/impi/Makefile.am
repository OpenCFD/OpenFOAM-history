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
# $Id: Makefile.am,v 1.20 2003/07/07 23:22:07 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

if WANT_IMPI_BUILD
impi_lib		= liblamimpi.la
else
impi_lib		=
endif

noinst_LTLIBRARIES	= $(impi_lib)
liblamimpi_la_SOURCES	= \
			  datasyncack.c \
			  iabort.c \
			  iattr.c \
			  ifinalize.c \
			  ifree_dtype.c \
			  ihelpers.c \
			  iinit.c \
			  ilisend.c \
			  ilisendinit.c \
			  ilsend.c \
			  imk_dtype.c \
			  ireduce.c \
			  lamgiappe.c \
			  regdatasync.c \
			  regsyncack.c \
			  regdatasyncinit.c \
			  regsyncackinit.c
