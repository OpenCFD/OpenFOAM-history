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
# $Id: Makefile.am,v 1.25 2003/11/24 18:49:10 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

# other required directories

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libltdl			= $(base)/libltdl/libltdlc.la

# lamd pseudo daemons -- convenience libraries (static, regardless of
# what the user chose -- they are only built so that they can be
# incorporated into the lamd; they are not intended to be separate
# libraries, hence, it doesn't make sense to give them the option to
# be shared libraries).

bforward		= ../bforward/liblamdbforward.a
buffer			= ../bufferd/liblamdbuffer.a
dli			= ../dli_inet/liblamddli.a
dlo			= ../dlo_inet/liblamddlo.a
echo			= ../echod/liblamdecho.a
file			= ../filed/liblamdfile.a
flat			= ../flatd/liblamdflat.a
halt			= ../haltd/liblamdhalt.a
io			= ../iod/liblamdio.a
kenya			= ../kenyad/liblamdkenya.a
kernel			= ../kernel/liblamdkernel.a
load			= ../loadd/liblamdload.a
router			= ../router/liblamdrouter.a
trace			= ../traced/liblamdtrace.a
version			= ../versiond/liblamdversion.a
libs			= $(bforward) $(buffer) $(dli) $(dlo) $(echo) $(file) $(flat) $(halt) $(io) $(kenya) $(kernel) $(load) $(router) $(trace) $(liblam) $(version) $(libltdl)

bin_PROGRAMS		= lamd
lamd_SOURCES		= \
			  couter.c \
			  di_wait.int.c \
			  do_wait.int.c \
			  iod.int.c \
			  kenyad.int.c \
			  kexit.c \
			  kiproc.c \
			  lamd_main.c \
			  nrecv.c \
			  nsend.c \
			  rreq.c \
			  stub.c \
			  trillium.c
lamd_LDADD		= $(libs) $(LIBLAM_EXTRA_LIBS)
lamd_LDFLAGS		= $(LIBLAM_EXTRA_LDFLAGS)
lamd_DEPENDENCIES	= $(libs)
