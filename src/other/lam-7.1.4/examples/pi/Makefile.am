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
# $Id: Makefile.am,v 1.9 2003/05/30 04:34:05 brbarret Exp $
#

include $(top_srcdir)/config/Makefile.options

# The programs to be built, and the compiler to build them

COMPILE			= mpicc $(CFLAGS)
LINK			= $(COMPILE) $(LDFLAGS) -o $@
F77COMPILE		= mpif77 $(FFLAGS)
F77LINK			= $(F77COMPILE) -o $@
CXXCOMPILE		= mpic++ $(CXXFLAGS) $(USER_CXXFLAGS)
CXXLINK			= $(CXXCOMPILE) $(CXXLDFLAGS) $(USER_CXXLDFLAGS) -o $@

if WANT_MPI2CPP
cxx_prog		= cxxpi
else
cxx_prog		=
endif

if WANT_FORTRAN
fortran_prog		= fpi
else
fortran_prog		=
endif

noinst_PROGRAMS		= cpi $(fortran_prog) $(cxx_prog)
EXTRA_PROGRAMS		= fpi cxxpi

# List the .h file so that it gets snarfed into the distribution

cpi_SOURCES		= cpi.c
cpi_LDADD		= -lm
fpi_SOURCES		= fpi.f
fpi_LDADD		= -lm
cxxpi_SOURCES		= cxxpi.cc
cxxpi_LDADD		= -lm

# Also snarf these files

EXTRA_DIST		= README
