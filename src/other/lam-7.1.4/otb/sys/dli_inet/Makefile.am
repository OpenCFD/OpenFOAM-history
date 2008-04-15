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
# $Id: Makefile.am,v 1.22 2003/11/24 18:49:10 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libltdl			= $(base)/libltdl/libltdlc.la
libs			= $(liblam) $(libltdl)

# Because Automake sucks, we have to have a level of indirection about
# whether or not to build the binary...
lam_target		= lamd_dli_inet

if WANT_TRILLIUM_INSTALL
lam_prog		= $(lam_target)
else
lam_prog		=
endif

dli_inet_lib_sources	= \
			di_bootagent.c \
			di_bootcmd.c \
			dli_inet.c 
dli_inet_bin_sources	= \
			$(dli_inet_lib_sources) \
			di_main.c \
			di_wait.ext.c

noinst_LIBRARIES	= liblamddli.a
liblamddli_a_SOURCES	= $(dli_inet_lib_sources)

bin_PROGRAMS		= $(lam_prog)
lamd_dli_inet_SOURCES	= $(dli_inet_bin_sources)
lamd_dli_inet_LDADD	= $(libs) $(LIBLAM_EXTRA_LIBS)
lamd_dli_inet_LDFLAGS	= $(LIBLAM_EXTRA_LDFLAGS)
lamd_dli_inet_DEPENDENCIES= $(libs)

CLEANFILES		= *~ $(lam_target)

EXTRA_DIST		= $(dli_inet_bin_sources)
