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
# $Id: Makefile.am,v 1.10 2002/10/09 20:55:31 brbarret Exp $
#

include $(top_srcdir)/config/Makefile.options

# The programs to be built, and the compiler to build them

COMPILE			= mpicc $(CFLAGS)
LINK			= $(COMPILE) -o $@

if WANT_ROMIO
program			= simple
else
program 		=
endif

noinst_PROGRAMS		= $(program)
simple_SOURCES		= simple.c

# Also snarf these files

EXTRA_DIST		= README 
