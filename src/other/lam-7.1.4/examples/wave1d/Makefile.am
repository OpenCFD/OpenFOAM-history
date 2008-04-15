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
# $Id: Makefile.am,v 1.9 2003/08/18 22:45:38 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

# The programs to be built, and the compiler to build them

F77COMPILE		= mpif77 $(FFLAGS)
F77LINK			= $(F77COMPILE) -o $@

if WANT_FORTRAN
program			= wave
else
program			=
endif

noinst_PROGRAMS		= $(program) 
wave_SOURCES		= wave.f

# Also snarf these files

EXTRA_DIST		= README
