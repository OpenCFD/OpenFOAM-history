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
# $Id: Makefile.am,v 1.31 2003/11/15 13:22:05 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

# The ordering is very important here.  libmpi, liblam, and
# ssi-dynamic must be the last three, and ssi-dynamic must be the last
# one.  This is because ssi-dynamic uses the libraries that are
# generated in liblam and libmpi.

SUBDIRS			= include args boot etc freq kreq impi memory mpi \
			  $(LIBLTDL_SUBDIR) nreq pmpi rreq ssi \
			  threads $(TOTALVIEW_DLL_DIR) trillium tstdio \
			  liblam libmpi dynamic-ssi
