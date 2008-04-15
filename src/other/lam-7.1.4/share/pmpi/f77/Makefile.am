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
# $Id: Makefile.am,v 1.32 2003/11/06 00:06:00 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

AM_CPPFLAGS		= -DLAM_PROFILELIB=1

# Do we want to build the MPI profiling library, or no?

if WANT_PROFILE_BUILD
if WANT_FORTRAN
profile_library		= liblamf77pmpi.la
else
profile_library		=
endif
else
profile_library		=
endif

# Files with stubs of MPI functions that LAM has not yet implemented.
# These are here because the default action is to *not* compile them
# so that users get preprocessor/linker errors if they try to use
# functions that LAM has not yet implemented.  But for testing
# purposes, or when we eventually do implement all of these functions,
# the framework and infrastructure is all there.  These functions can
# be enabled with the --with-stubs configure option.

mpi_stub_functions	= \
			pwcallerr_f.c \
			pwlock_f.c \
			pwtest_f.c \
			pwunlock_f.c


if WANT_MPI_STUBS
mpi_stub_sources	= $(mpi_stub_functions)
else
mpi_stub_sources	=
endif


noinst_LTLIBRARIES	= $(profile_library)
nodist_liblamf77pmpi_la_SOURCES = \
			  $(mpi_stub_sources) \
			  pabort_f.c \
			  paccept_f.c \
			  paccumulate_f.c \
			  paddress_f.c \
			  pallgather_f.c \
			  pallgatherv_f.c \
			  pallocmem_f.c \
			  pallreduce_f.c \
			  palltoall_f.c \
			  palltoallv_f.c \
			  palltoallw_f.c \
			  pattrdel_f.c \
			  pattrget_f.c \
			  pattrput_f.c \
			  pbarrier_f.c \
			  pbcast_f.c \
			  pbsend_f.c \
			  pbsendinit_f.c \
			  pbufattach_f.c \
			  pbufdetach_f.c \
			  pcancel_f.c \
			  pcartcoords_f.c \
			  pcartcreate_f.c \
			  pcartdimget_f.c \
			  pcartget_f.c \
			  pcartmap_f.c \
			  pcartrank_f.c \
			  pcartshift_f.c \
			  pcartsub_f.c \
			  pccmp_f.c \
			  pccreateerr_f.c \
			  pccreate_f.c \
			  pccreatekey_f.c \
			  pcdelattr_f.c \
			  pcdisconnect_f.c \
			  pcdup_f.c \
			  pcfree_f.c \
			  pcfreekey_f.c \
			  pcgetattr_f.c \
			  pcgeterr_f.c \
			  pcgetname_f.c \
			  pcgetparent_f.c \
			  pcgroup_f.c \
			  pconnect_f.c \
			  pcrank_f.c \
			  pcrgroup_f.c \
			  pcrsize_f.c \
			  pcsetattr_f.c \
			  pcseterr_f.c \
			  pcsetname_f.c \
			  pcsize_f.c \
			  pcsplit_f.c \
			  pctestinter_f.c \
			  pdimscreate_f.c \
			  perrclass_f.c \
			  perrcreate_f.c \
			  perrfree_f.c \
			  perrget_f.c \
			  perrset_f.c \
			  perrstring_f.c \
			  pexscan_f.c \
			  pfinalize_f.c \
			  pfinalized_f.c \
			  pfreemem_f.c \
			  pgather_f.c \
			  pgatherv_f.c \
			  pgcmp_f.c \
			  pgdiff_f.c \
			  pgetaddress_f.c \
			  pgetcount_f.c \
			  pgetelem_f.c \
			  pget_f.c \
			  pgetprocname_f.c \
			  pgetversion_f.c \
			  pgexcl_f.c \
			  pgfree_f.c \
			  pgincl_f.c \
			  pginter_f.c \
			  pgrank_f.c \
			  pgraphcreate_f.c \
			  pgraphdimsget_f.c \
			  pgraphget_f.c \
			  pgraphmap_f.c \
			  pgraphnbrcount_f.c \
			  pgraphnbr_f.c \
			  pgrexcl_f.c \
			  pgrincl_f.c \
			  pgsize_f.c \
			  pgtranks_f.c \
			  pgunion_f.c \
			  pibsend_f.c \
			  piccreate_f.c \
			  picmerge_f.c \
			  pinfocreate_f.c \
			  pinfodel_f.c \
			  pinfodup_f.c \
			  pinfofree_f.c \
			  pinfoget_f.c \
			  pinfogetnkeys_f.c \
			  pinfogetnth_f.c \
			  pinfogetvlen_f.c \
			  pinfoset_f.c \
			  pinited_f.c \
			  pinit_f.c \
			  pinitthr_f.c \
			  piprobe_f.c \
			  pirecv_f.c \
			  pirsend_f.c \
			  pisend_f.c \
			  pissend_f.c \
			  pisthrmain_f.c \
			  pjoin_f.c \
			  pkeycreate_f.c \
			  pkeyfree_f.c \
			  pmpil_id_f.c \
			  pmpil_signal_f.c \
			  pmpil_trace_f.c \
			  pnamelook_f.c \
			  pnamepub_f.c \
			  pnameunpub_f.c \
			  popcreate_f.c \
			  popfree_f.c \
			  ppack_f.c \
			  ppacksize_f.c \
			  ppcontrol_f.c \
			  pportclose_f.c \
			  pportopen_f.c \
			  pprobe_f.c \
			  pput_f.c \
			  pquerythr_f.c \
			  precv_f.c \
			  precvinit_f.c \
			  preduce_f.c \
			  preducescatter_f.c \
			  preqfree_f.c \
			  prsend_f.c \
			  prsendinit_f.c \
			  pscan_f.c \
			  pscatter_f.c \
			  pscatterv_f.c \
			  psend_f.c \
			  psendinit_f.c \
			  psendrecv_f.c \
			  psendrecvrep_f.c \
			  pspawn_f.c \
			  pspawnmult_f.c \
			  pssend_f.c \
			  pssendinit_f.c \
			  pstartall_f.c \
			  pstart_f.c \
			  ptcommit_f.c \
			  ptcontig_f.c \
			  ptcreatehindex_f.c \
			  ptcreatehvector_f.c \
			  ptcreatekey_f.c \
			  ptcreatestruct_f.c \
			  ptdarray_f.c \
			  ptdelattr_f.c \
			  ptdup_f.c \
			  ptestall_f.c \
			  ptestany_f.c \
			  ptestcancel_f.c \
			  ptest_f.c \
			  ptestsome_f.c \
			  ptextent_f.c \
			  ptfree_f.c \
			  ptfreekey_f.c \
			  ptgetattr_f.c \
			  ptgetconts_f.c \
			  ptgetenvl_f.c \
			  ptgetextent_f.c \
			  ptgetname_f.c \
			  ptgettrue_f.c \
			  pthindex_f.c \
			  pthvector_f.c \
			  ptindex_f.c \
			  ptlb_f.c \
			  ptopotest_f.c \
			  ptresize_f.c \
			  ptsetattr_f.c \
			  ptsetname_f.c \
			  ptsize_f.c \
			  ptstruct_f.c \
			  ptsubarray_f.c \
			  ptub_f.c \
			  ptvector_f.c \
			  punpack_f.c \
			  pwaitall_f.c \
			  pwaitany_f.c \
			  pwait_f.c \
			  pwaitsome_f.c \
			  pwcomplete_f.c \
			  pwcreateerr_f.c \
			  pwcreate_f.c \
			  pwcreatekey_f.c \
			  pwdelattr_f.c \
			  pwfence_f.c \
			  pwfree_f.c \
			  pwfreekey_f.c \
			  pwgetattr_f.c \
			  pwgeterr_f.c \
			  pwgetname_f.c \
			  pwgroup_f.c \
			  pwpost_f.c \
			  pwsetattr_f.c \
			  pwseterr_f.c \
			  pwsetname_f.c \
			  pwstart_f.c \
			  pwtick_f.c \
			  pwtime_f.c \
			  pwwait_f.c

# Sym link in the sources from the real MPI directory

$(nodist_liblamf77pmpi_la_SOURCES) $(mpi_stub_functions):
	if test ! -r $@ ; then \
		pname=`echo $@ | cut -b '2-'` ; \
		ln -s $(top_srcdir)/share/mpi/f77/$$pname $@ ; \
	fi

# These files were created by targets above

MAINTAINERCLEANFILES = $(nodist_liblamf77pmpi_la_SOURCES) $(mpi_stub_functions)

# Don't want these targets in here

tags-recursive:
tags:
TAGS:
GTAGS:
ID:
