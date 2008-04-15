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

include $(top_srcdir)/config/Makefile.options

SUBDIRS			= ptmalloc ptmalloc2 darwin7

noinst_LTLIBRARIES	= libmemory.la
libmemory_la_SOURCES	= memory_hook.c

if LAM_WANT_PTMALLOC
libmemory_la_LIBADD	= \
			ptmalloc/libptmalloc.la
endif

if LAM_WANT_PTMALLOC2
libmemory_la_LIBADD	= \
			ptmalloc2/libptmalloc2.la
endif

if LAM_WANT_DARWIN7MALLOC
libmemory_la_LIBADD	= \
			darwin7/libdarwin7malloc.la
endif
