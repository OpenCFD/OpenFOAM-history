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
# $Id: Makefile.am,v 1.26 2003/07/07 23:22:06 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

# Do some conditionals to see if we want to install IMPI support.  We
# do the conditional here rather than in the upper level file so that
# targets like clean, distclean, and dist still come through here,
# regardless of the current ./configure options.

# Have to do some weirdness to ensure that we get everything -- a
# double redirection on $(prog) (which gets loaded into
# $(bin_PROGRAMS), but we have to supply the sources in EXTRA_DIST.

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libmpi			= $(base)/libmpi/libmpi.la
libs			= $(libmpi) $(liblam)
AM_CPPFLAGS		= -DLAM_TCPSHORTMSGLEN=65536

lam_target		= impid

if WANT_IMPI_BUILD
lam_prog		= $(lam_target)
else
lam_prog		=
endif

bin_PROGRAMS		= $(lam_prog)
impid_SOURCES		= \
			  client.c \
			  comm.cc \
			  dance.c \
			  endian.c \
			  flow_control.cc \
			  host.cc \
			  host_ack.cc \
			  host_long.cc \
			  host_queue.cc \
			  impid.c \
			  lamnodes.c \
			  local.cc \
			  local_ack.cc \
			  local_long.cc \
			  local_queue.cc \
			  local_req.cc \
			  procs.cc \
			  packet_mgmt.cc \
			  router.cc \
			  sequence.c
impid_LDADD		= $(libs) $(LIBMPI_EXTRA_LIBS) $(LIBLAM_EXTRA_LIBS)
impid_LDFLAGS		= $(LIBMPI_EXTRA_LDFLAGS) $(LIBLAM_EXTRA_LDFLAGS)
impid_DEPENDENCIES	= $(libs)

CLEANFILES		= -r *~ $(LAM_CXX_TEMPLATE_REPOSITORY) $(lam_target)

EXTRA_DIST		= $(impid_SOURCES)
