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
# $Id: Makefile.am,v 1.10 2003/09/21 23:08:53 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options


if WANT_MPI2CPP
mpi2cpp_lib		= liblammpi++.la
else
mpi2pp_lib		=
endif

# VPS: The following CPPFILE stuff added until ROMIO integrated into LAM
# LAM_BUILDING is 0 and -I so that mpio.h can be referenced.
if WANT_CPPFILE
AM_CPPFLAGS		= -I$(top_srcdir)/romio/include
else
AM_CPPFLAGS		=
endif


lib_LTLIBRARIES		= $(mpi2cpp_lib)

always_sources		= \
			mpicxx.cc \
			intercepts.cc
profile_only_sources	= \
                        pmpicxx.cc \
                        op.cc \
                        comm.cc \
                        intracomm.cc \
                        topology.cc \
                        intercomm.cc \
			info.cc \
			win.cc \
			request.cc \
                        group.cc \
                        datatype.cc \
                        status.cc \
                        errhandler.cc \
                        exception.cc \
                        functions.cc

#VPS: Added so that file.cc which requires ROMIO references is compiled only
# if MPI::File required 

if WANT_CPPFILE
mpi_file_sources		= file.cc
else
mpi_file_sources		=
endif

if WANT_PROFILE_BUILD
liblammpi___la_SOURCES	= $(always_sources) $(profile_only_sources) \
			  $(mpi_file_sources)
else
liblammpi___la_SOURCES	= $(always_sources)
endif

EXTRA_DIST		= $(always_sources) $(profile_only_sources)

clean-local:
	test -z "$(LAM_CXX_TEMPLATE_REPOSITORY)" || $(RM) -rf $(LAM_CXX_TEMPLATE_REPOSITORY)
