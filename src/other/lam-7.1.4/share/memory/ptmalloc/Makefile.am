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
# $Id: Makefile.am,v 1.5 2003/07/07 23:22:13 jsquyres Exp $
#
# Original ptmalloc package notices:
# 
# Copyright (c) 1999 Wolfram Gloger
#
# See COPYRIGHT file included in this directory for additional notices
# about the original ptmalloc package.
#

include $(top_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
			-I$(top_srcdir)/src \
			-I$(top_lam_builddir)/share/include \
			-I$(top_lam_srcdir)/share/include \
			-D_USE_TSD_DATA_HACK \
			-DMALLOC_HOOKS -DMALLOC_DEBUG=0 \
			-DHAVE_USR_INCLUDE_MALLOC_H=0

# ptmalloc requires us to include the copyright notice in the
# software.  So install it in the same place that we install ROMIO's
# copyright notices.

docdir			= $(datadir)/lam/doc
doc_DATA		= ptmalloc-COPYRIGHT

if LAM_WANT_PTMALLOC
noinst_LTLIBRARIES	= libptmalloc.la
endif

libptmalloc_la_SOURCES = \
			munmap_wrapper.c \
			ptmalloc.c \
			ptmalloc.h \
			ptmalloc_hook.c \
			thread-m.h

EXTRA_DIST		= \
			README \
			README.LAM \
			ChangeLog \
			lran2.h \
			t-test.h \
			t-test1.c \
			t-test2.c \
			$(doc_DATA)

