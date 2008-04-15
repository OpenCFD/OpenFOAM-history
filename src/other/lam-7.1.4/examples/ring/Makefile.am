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
# $Id: Makefile.am,v 1.7 2003/05/30 04:34:05 brbarret Exp $
#

include $(top_srcdir)/config/Makefile.options

# The programs to be built, and the compiler to build them

COMPILE			= mpicc $(CFLAGS)
LINK			= $(COMPILE) $(LDFLAGS) -o $@

CXXCOMPILE		= mpic++ $(CXXFLAGS) $(USER_CXXFLAGS)
CXXLINK			= $(CXXCOMPILE) $(CXXLDFLAGS) $(USER_CXXLDFLAGS) -o $@

F77COMPILE		= mpif77 $(FFLAGS) $(USER_FFLAGS)
F77LINK			= $(F77COMPILE) $(F77LDFLAGS) $(USER_F77LDFLAGS) -o $@

if WANT_MPI2CPP
cxx_program		= cxxring
else
cxx_program		=
endif

if WANT_FORTRAN
f_program		= fring
else
f_program		=
endif

noinst_PROGRAMS		= ring $(cxx_program) $(f_program)
EXTRA_PROGRAMS		= cxxring

ring_SOURCES		= ring.c
cxxring_SOURCES		= cxxring.cc
fring_SOURCES		= fring.f

# Also snarf these files

EXTRA_DIST		= README
