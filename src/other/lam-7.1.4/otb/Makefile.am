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
# $Id: Makefile.am,v 1.20 2003/12/19 02:56:14 pcharapa Exp $
#

include $(top_srcdir)/config/Makefile.options

# These directories are only listed separately because they will only
# have binaries compiled if --with-trillium was specified during the
# configuration of LAM/MPI.  The Makefile.am's in each subdirectory
# have a conditional defined such that they will compile their binary
# [or not] depending on the args to configure.  

# The decision as to whether we compile or not is in the subdirectory
# files so that we can come through....

SUBDIRS			= bfctl bfstate doom fctl filedr fstate ipcdr \
			  kdump lamclean lamexec lamgrow lamhalt lamnodes \
			  lamshrink lamtrace loadgo mpimsg mpirun \
			  mpitask state sys tping lamcheckpoint lamrestart
