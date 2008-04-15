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
# $Id: Makefile.am,v 1.3 2003/05/30 04:34:05 brbarret Exp $
#

include $(top_srcdir)/config/Makefile.options

# The programs to be built, and the compiler to build them
CXXCOMPILE		= mpic++ $(CXXFLAGS) $(USER_CXXFLAGS)
CXXLINK			= $(CXXCOMPILE) $(CXXLDFLAGS) $(USER_CXXLDFLAGS) -o $@

if WANT_MPI2CPP
program			= cxx_topology
else
program			=
endif

noinst_PROGRAMS		= $(program)
EXTRA_PROGRAMS		= cxx_topology

cxx_topology_SOURCES	= cxx_topology.cc
