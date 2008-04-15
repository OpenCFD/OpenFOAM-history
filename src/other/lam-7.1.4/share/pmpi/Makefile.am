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
# $Id: Makefile.am,v 1.45 2003/11/06 00:05:58 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

AM_CPPFLAGS		= -DLAM_PROFILELIB=1
SUBDIRS			= f77

# Do we want to build the MPI profiling library, or no?

if WANT_PROFILE_BUILD
profile_library		= libpmpi.la
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
			pwcallerr.c \
			pwlock.c \
			pwtest.c \
			pwunlock.c 

if WANT_MPI_STUBS
mpi_stub_sources	= $(mpi_stub_functions)
else
mpi_stub_sources	=
endif

# No need for RPI stuff here; that can always be obtained from libmpi.a

noinst_LTLIBRARIES	= $(profile_library)
libpmpi_la_LIBADD	= $(F77_LTPLIB) 

nodist_libpmpi_la_SOURCES = \
			  $(mpi_stub_sources) \
			  pabort.c \
			  paccept.c \
			  paccumulate.c \
			  paddress.c \
			  pallgather.c \
			  pallgatherv.c \
			  pallreduce.c \
			  pallocmem.c \
			  palltoall.c \
			  palltoallv.c \
			  palltoallw.c \
			  pattrdel.c \
			  pattrget.c \
			  pattrput.c \
			  pbarrier.c \
			  pbcast.c \
			  pbsend.c \
			  pbsendinit.c \
			  pbufattach.c \
			  pbufdetach.c \
			  pcancel.c \
			  pcartcoords.c \
			  pcartcreate.c \
			  pcartdimget.c \
			  pcartget.c \
			  pcartmap.c \
			  pcartrank.c \
			  pcartshift.c \
			  pcartsub.c \
			  pccmp.c \
			  pccreate.c \
			  pccreateerr.c \
			  pccreatekey.c \
			  pcdelattr.c \
			  pcdisconnect.c \
			  pcdup.c \
			  pcfree.c \
			  pcfreekey.c \
			  pcgetattr.c \
			  pcgeterr.c \
			  pcgetname.c \
			  pcgetparent.c \
			  pcgroup.c \
			  pconnect.c \
			  pcrank.c \
			  pcrgroup.c \
			  pcrsize.c \
			  pcsetattr.c \
			  pcseterr.c \
			  pcsetname.c \
			  pcsize.c \
			  pcsplit.c \
			  pctestinter.c \
			  pdimscreate.c \
			  perrclass.c \
			  perrcreate.c \
			  perrfree.c \
			  perrget.c \
			  perrset.c \
			  perrstring.c \
			  pexscan.c \
			  pfinalize.c \
			  pfinalized.c \
			  pfreemem.c \
			  pgather.c \
			  pgatherv.c \
			  pgcmp.c \
			  pgdiff.c \
			  pgetaddress.c \
			  pget.c \
			  pgetcount.c \
			  pgetelem.c \
			  pgetprocname.c \
			  pgetversion.c \
			  pgexcl.c \
			  pgfree.c \
			  pgincl.c \
			  pginter.c \
			  pgrank.c \
			  pgraphcreate.c \
			  pgraphdimsget.c \
			  pgraphget.c \
			  pgraphmap.c \
			  pgraphnbr.c \
			  pgraphnbrcount.c \
			  pgrexcl.c \
			  pgrincl.c \
			  pgsize.c \
			  pgtranks.c \
			  pgunion.c \
			  phandles.c \
			  pibsend.c \
			  piccreate.c \
			  picmerge.c \
			  pinfocreate.c \
			  pinfodel.c \
			  pinfodup.c \
			  pinfofree.c \
			  pinfoget.c \
			  pinfogetnkeys.c \
			  pinfogetnth.c \
			  pinfogetvlen.c \
			  pinfoset.c \
			  pinit.c \
			  pinited.c \
			  pinitthr.c \
			  piprobe.c \
			  pirecv.c \
			  pirsend.c \
			  pisend.c \
			  pissend.c \
			  pisthrmain.c \
			  pjoin.c \
			  pkeycreate.c \
			  pkeyfree.c \
			  pmpil_id.c \
			  pmpil_signal.c \
			  pmpil_trace.c \
			  pnamelook.c \
			  pnamepub.c \
			  pnameunpub.c \
			  popcreate.c \
			  popfree.c \
			  ppack.c \
			  ppacksize.c \
			  ppcontrol.c \
			  pportclose.c \
			  pportopen.c \
			  pprobe.c \
			  pput.c \
			  pquerythr.c \
			  precv.c \
			  precvinit.c \
			  preduce.c \
			  preducescatter.c \
			  preqfree.c \
			  prsend.c \
			  prsendinit.c \
			  pscan.c \
			  pscatter.c \
			  pscatterv.c \
			  psend.c \
			  psendinit.c \
			  psendrecv.c \
			  psendrecvrep.c \
			  pspawn.c \
			  pspawnmult.c \
			  pssend.c \
			  pssendinit.c \
			  pstartall.c \
			  pstart.c \
			  ptcommit.c \
			  ptcontig.c \
			  ptcreatehindex.c \
			  ptcreatehvector.c \
			  ptcreatekey.c \
			  ptcreatestruct.c \
			  ptdarray.c \
			  ptdelattr.c \
			  ptdup.c \
			  ptestall.c \
			  ptestany.c \
			  ptest.c \
			  ptestcancel.c \
			  ptestsome.c \
			  ptextent.c \
			  ptfree.c \
			  ptfreekey.c \
			  ptgetattr.c \
			  ptgetconts.c \
			  ptgetenvl.c \
			  ptgetextent.c \
			  ptgetname.c \
			  ptgettrue.c \
			  pthindex.c \
			  pthvector.c \
			  ptindex.c \
			  ptlb.c \
			  ptopotest.c \
			  ptresize.c \
			  ptsetattr.c \
			  ptsetname.c \
			  ptsize.c \
			  ptstruct.c \
			  ptsubarray.c \
			  ptub.c \
			  ptvector.c \
			  punpack.c \
			  pwaitall.c \
			  pwaitany.c \
			  pwait.c \
			  pwaitsome.c \
			  pwcomplete.c \
			  pwcreate.c \
			  pwcreateerr.c \
			  pwcreatekey.c \
			  pwdelattr.c \
			  pwfence.c \
			  pwfree.c \
			  pwfreekey.c \
			  pwgetattr.c \
			  pwgeterr.c \
			  pwgetname.c \
			  pwgroup.c \
			  pwpost.c \
			  pwsetattr.c \
			  pwseterr.c \
			  pwsetname.c \
			  pwstart.c \
			  pwtick.c \
			  pwtime.c \
			  pwwait.c

# Sym link in the sources from the real MPI directory

$(nodist_libpmpi_la_SOURCES) $(mpi_stub_functions):
	if test ! -r $@ ; then \
		pname=`echo $@ | cut -b '2-'` ; \
		ln -s $(top_srcdir)/share/mpi/$$pname $@ ; \
	fi

# These files were created by targets above

MAINTAINERCLEANFILES = $(nodist_libpmpi_la_SOURCES) $(mpi_stub_functions)

# Don't want these targets in here

tags-recursive:
tags:
TAGS:
GTAGS:
ID:
