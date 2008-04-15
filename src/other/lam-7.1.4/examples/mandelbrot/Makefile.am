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
# $Id: Makefile.am,v 1.9 2003/08/18 22:45:36 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

# The programs to be built, and the compiler to build them

COMPILE			= mpicc $(CFLAGS)
LINK			= $(COMPILE) $(LDFLAGS) -o $@
noinst_PROGRAMS		= master slave

# List the .h file so that it gets snarfed into the distribution

master_SOURCES		= master.c mandel.h
slave_SOURCES		= slave.c mandel.h

# Also snarf these files

EXTRA_DIST		= README
