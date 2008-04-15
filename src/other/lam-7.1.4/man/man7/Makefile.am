# -*- makefile -*-
#
# Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
# $Id: Makefile.am,v 1.16 2003/05/31 03:20:35 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

dist_mans		= \
			lam.7 \
			lamssi.7 \
			lamssi_boot.7 \
			lamssi_coll.7 \
			lamssi_cr.7 \
			lamssi_rpi.7 \
			libmpi.7

no_dist_mans		= \
			mpi.7

if CASE_SENSITIVE_FS
cs_mans			= \
			LAM.7 \
			MPI.7
else
cs_mans			=
endif

man_MANS		= $(dist_mans) $(no_dist_mans) $(cs_mans)

EXTRA_DIST		= $(dist_mans)

clean-local:
	$(RM) -f *~ $(no_dist_mans) $(cs_mans)

msdir			= $(top_srcdir)/man/mans
m7dir			= $(top_srcdir)/man/man7

libmpi.7: $(msdir)/mpi.share
	rm -f libmpi.7
	sed -e 's/TOPIC/libmpi/' $(msdir)/mpi.share > libmpi.7
LAM.7: $(m7dir)/lam.7
	cp -f $(m7dir)/lam.7 LAM.7
MPI.7: mpi.7
	cp -f mpi.7 MPI.7
mpi.7: $(msdir)/mpi.share
	rm -f mpi.7
	sed -e 's/TOPIC/mpi/' $(msdir)/mpi.share > mpi.7
