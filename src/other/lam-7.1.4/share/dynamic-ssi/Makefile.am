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
# $Id: Makefile.am,v 1.1 2003/11/15 14:23:19 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

# This whole directory tree exists so that dynamic SSI modules can be
# build *after* liblam and libmpi have been built.  The normal
# share/ssi tree is where static modules and SSI glue are built, and
# they get sucked into liblam/libmpi.  Hence, that tree has to be
# traversed and built *before* liblam/libmpi are build.
#
# The dynamic modules, which may use symbols out of liblam/libmpi,
# need to be built *after* liblam/libmpi.  Hence, they live here
# (albiet sym linked back into the main share/ssi tree) so that they
# can be traversed/built after liblam/libmpi are built.

SUBDIRS			= boot coll crlam crmpi rpi
