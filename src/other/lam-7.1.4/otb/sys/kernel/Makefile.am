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
# $Id: Makefile.am,v 1.20 2003/11/15 13:17:56 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libs			= $(liblam)
AM_CPPFLAGS		= -DLAM_BINDIR="\"$(bindir)\""

# Because Automake sucks, we have to have a level of indirection about
# whether or not to build the binary...
lam_target		= lamd_kernel

if WANT_TRILLIUM_INSTALL
lam_prog		= $(lam_target)
else
lam_prog		=
endif

kernel_lib_sources	= \
			  kernelio.c \
			  kinner.c \
			  kkill.c \
			  kouter.c
kernel_bin_sources	= \
			$(kernel_lib_sources) \
			k_main.c \
			kiproc.c

noinst_LIBRARIES	= liblamdkernel.a
liblamdkernel_a_SOURCES	= $(kernel_lib_sources)

bin_PROGRAMS		= $(lam_prog)
lamd_kernel_SOURCES	= $(kernel_bin_sources)
lamd_kernel_LDADD	= $(libs) $(LIBLAM_EXTRA_LIBS)
lamd_kernel_LDFLAGS	= $(LIBLAM_EXTRA_LDFLAGS)
lamd_kernel_DEPENDENCIES= $(libs)

CLEANFILES		= *~ $(lam_target)

EXTRA_DIST		= $(kernel_bin_sources)
