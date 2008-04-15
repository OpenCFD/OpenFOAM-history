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
# $Id: Makefile.am,v 1.5 2003/02/10 02:54:40 vsahay Exp $
#

include $(top_srcdir)/config/Makefile.options

# Override so that these include files get installed into
# $prefix/include/mpi2cxx.

pkgincludedir		= $(includedir)/mpi2cxx

common_h		= \
			mpi2cxx_map.h \
			mpi2cxx_list.h \
			constants.h \
			functions.h \
			datatype.h \
			exception.h \
			op.h \
			status.h \
			request.h \
			group.h \
			comm.h \
			errhandler.h \
			intracomm.h \
			info.h \
			win.h \
			topology.h \
			intercomm.h \
			datatype_inln.h \
			functions_inln.h \
			request_inln.h \
			comm_inln.h \
			intracomm_inln.h \
			info_inln.h \
			win_inln.h \
			topology_inln.h \
			intercomm_inln.h \
			group_inln.h \
			op_inln.h \
			errhandler_inln.h \
			status_inln.h

profile_h		= \
			pmpicxx.h \
			pcomm.h \
			pdatatype.h \
			perrhandler.h \
			pexception.h \
			pgroup.h \
			pgroup_inln.h \
			pintercomm.h \
			pintracomm.h \
			pop.h \
			pop_inln.h \
			prequest.h \
			prequest_inln.h \
			pinfo.h \
			pwin.h \
			pstatus.h \
			pstatus_inln.h \
			ptopology.h


if WANT_CPPFILE
mpi_file_headers	= \
			file.h \
			file_inln.h

pmpi_file_header	= pfile.h
else
mpi_file_headers	=
pmpi_file_header	=
endif

if WANT_PROFILE_BUILD
pro_headers		= $(profile_h) $(pmpi_file_header)
else
pro_headers		=
endif

pkginclude_HEADERS	= $(common_h) $(mpi_file_headers) $(pro_headers)
EXTRA_HEADERS		= $(common_h) $(mpi_file_headers) $(profile_h) \
			$(pmpi_file_header)	
EXTRA_DIST		= $(profile_h) $(pmpi_file_header)
