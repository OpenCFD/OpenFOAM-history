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
# $Id: Makefile.am,v 1.5 2003/07/07 23:22:13 jsquyres Exp $
#
# Original ptmalloc package notices:
# 
# Makefile for ptmalloc, version 2
# by Wolfram Gloger 1996-1999, 2001, 2002
#
# See COPYRIGHT file included in this directory for additional notices
# about the original ptmalloc package.
#

include $(top_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
			-I$(top_srcdir)/src \
			-I$(top_lam_builddir)/share/include \
			-I$(top_lam_srcdir)/share/include \
			-DUSE_TSD_DATA_HACK \
			-DMALLOC_DEBUG=0 \
			-DUSE_NO_SPINLOCKS=1

# ptmalloc requires us to include the copyright notice in the
# software.  So install it in the same place that we install ROMIO's
# copyright notices.

docdir			= $(datadir)/lam/doc
doc_DATA		= ptmalloc2-COPYRIGHT

if LAM_WANT_PTMALLOC2
noinst_LTLIBRARIES	= libptmalloc2.la
endif

libptmalloc2_la_SOURCES = \
			malloc.c \
			malloc.h \
			thread-m.h \
			thread-st.h \
			lam_ptmalloc2_hook.c

# these are included directly and shouldn't be built solo
EXTRA_libptmalloc2_la_SOURCES = \
			arena.c \
			hooks.c

EXTRA_DIST		= \
			ptmalloc2-COPYRIGHT \
			ChangeLog \
			README \
			README.LAM \
			ChangeLog \
			lran2.h \
			t-test.h \
			t-test1.c \
			t-test2.c \
			tst-mallocstate.c \
			$(doc_DATA)
