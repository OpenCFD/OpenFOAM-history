# -*- makefile -*-
#
# Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
# $Id: Makefile.am,v 1.1 2003/11/11 18:47:23 vsahay Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

# This file is only here so that "make dist" grabs all the extra
# config-level files that are necessary to build a LAM distribution
# tarball.  Nothing gets built in this directory.

# Seems to be an automake bug -- depcomp is not automatically included
# in distribution tarballs.

EXTRA_DIST		= \
			depcomp \
			lam_rpi_ib_params.m4 \
			lam_rpi_ib_setup.m4
