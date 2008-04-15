# -*- makefile -*-
#
# Copyright (c) 2001-2005 The Trustees of Indiana University.  
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

include $(top_srcdir)/config/Makefile.options

dist_trillium_man	= \
			  bfctl.1 \
			  bfstate.1 \
			  doom.1 \
			  fctl.1 \
			  fstate.1 \
			  loadgo.1 \
			  state.1

no_dist_trillium_man	= \
			  sweep.1

dist_mpi_man		= \
			  hboot.1 \
			  hcc.1 \
			  hf77.1 \
			  introu.1 \
			  lamd.1 \
			  lamboot.1 \
                          lamcheckpoint.1 \
			  lamclean.1 \
			  lamexec.1 \
                          lamhalt.1 \
			  lamgrow.1 \
			  laminfo.1 \
			  lamnodes.1 \
                          lamrestart.1 \
			  lamshrink.1 \
			  lamtrace.1 \
			  lamwipe.1 \
			  mpicc.1 \
			  mpiexec.1 \
			  mpif77.1 \
			  mpimsg.1 \
			  mpirun.1 \
			  mpitask.1 \
			  recon.1 \
			  tkill.1 \
			  tping.1 \
			  wipe.1

no_dist_mpi_man		= \
			  hcp.1 \
			  lamd.1 \
			  mpic++.1

if CASE_SENSITIVE_FS
cs_mans			= mpiCC.1
else
cs_mans			=
endif

if WANT_TRILLIUM_INSTALL
install_trill_man	= $(dist_trillium_man) $(no_dist_trillium_man)
else
install_trill_man	=
endif

man_MANS		= \
			$(dist_mpi_man) \
			$(no_dist_mpi_man) \
			$(install_trill_man) \
			$(cs_mans)

EXTRA_DIST		= $(dist_mpi_man) $(dist_trillium_man)

clean-local:
	$(RM) -f *~ $(no_dist_trillium_man) $(no_dist_mpi_man) $(cs_mans)

m1dir			= $(top_srcdir)/man/man1
m7dir			= $(top_srcdir)/man/man7

hcp.1: $(m1dir)/hcc.1
	cp -f $(m1dir)/hcc.1 hcp.1
mpic++.1: $(m1dir)/mpicc.1
	cp -f $(m1dir)/mpicc.1 mpic++.1
lamd.1: $(m7dir)/lam.7
	rm -f lamd.1
	sed -e 's/LAM 7/LAM 1/' $(m7dir)/lam.7 > lamd.1
sweep.1: $(m1dir)/bfctl.1
	cp -f $(m1dir)/bfctl.1 sweep.1

# mpic++.1 is not in the $(top_srcdir) -- it's in the build dir (see
# the corresponding rule above where it was created).  So no $(m1dir)
# in this rule.

mpiCC.1: mpic++.1
	cp -f mpic++.1 mpiCC.1

