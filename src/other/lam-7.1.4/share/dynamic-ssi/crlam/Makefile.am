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
# $Id: Makefile.am,v 1.1 2003/11/15 14:11:02 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

SUBDIRS			= $(LAM_SSI_crlam_DYNAMIC_SUBDIRS)
DISTCLEANFILES		= $(SUBDIRS)

# Every directory under here is a sym link to something in the main
# share/ssi tree.  Hence, we don't want to distribute anything under
# here.

DIST_SUBDIRS		=
