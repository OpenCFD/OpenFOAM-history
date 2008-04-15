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
# $Id: Makefile.am,v 1.28 2003/10/22 23:44:33 adharurk Exp $
#

include $(top_srcdir)/config/Makefile.options

# This kinda sucks.  The "ar" command in IRIX 6.5 has a upper limit
# on the total number of characters in its arguments.  If we include
# all these fortran files in making libmpi.a, with all the
# subdirectory names involved (share/mpi and share/rpi), we will just
# barely exceed the limit, and ar will barf.  So unfortunately, of
# the three components in libmpi.a (the C API, the Fortran API, and
# the RPI), we have to split off one of them.  The Fortran one makes
# the most sense, since we can still get a clean link of MPI programs
# with an additional -llamf77mpi at the beginning of the -l arguments
# (splitting the RPI is not nearly as clean -- trust me!).

# Hence, this library will get installed and not be a "noinst".

# Note that this is *not* true for the profiling interface; since the
# RPI does not have to be linked in to the profiling library, there's
# no extra subdirectories, so it doesn't run into the same character
# limit with ar.  Ugh.

if WANT_FORTRAN
fortran_lib		= liblamf77mpi.la
else
fortran_lib		=
endif


# Files with stubs of MPI functions that LAM has not yet implemented.
# These are here because the default action is to *not* compile them
# so that users get preprocessor/linker errors if they try to use
# functions that LAM has not yet implemented.  But for testing
# purposes, or when we eventually do implement all of these functions,
# the framework and infrastructure is all there.  These functions can
# be enabled with the --with-stubs configure option.

mpi_stub_functions	= \
			wcallerr_f.c \
			wlock_f.c \
			wtest_f.c \
			wunlock_f.c

if WANT_MPI_STUBS
mpi_stub_sources	= $(mpi_stub_functions)
else
mpi_stub_sources	=
endif


lib_LTLIBRARIES		= $(fortran_lib)
liblamf77mpi_la_SOURCES	= \
			  $(mpi_stub_sources) \
			  abort_f.c \
			  accept_f.c \
			  accumulate_f.c \
			  address_f.c \
			  allgather_f.c \
			  allgatherv_f.c \
			  allocmem_f.c \
			  allreduce_f.c \
			  alltoall_f.c \
			  alltoallv_f.c \
			  alltoallw_f.c \
			  attrdel_f.c \
			  attrget_f.c \
			  attrput_f.c \
			  barrier_f.c \
			  bcast_f.c \
			  bsend_f.c \
			  bsendinit_f.c \
			  bufattach_f.c \
			  bufdetach_f.c \
			  cancel_f.c \
			  cartcoords_f.c \
			  cartcreate_f.c \
			  cartdimget_f.c \
			  cartget_f.c \
			  cartmap_f.c \
			  cartrank_f.c \
			  cartshift_f.c \
			  cartsub_f.c \
			  ccmp_f.c \
			  ccreateerr_f.c \
			  ccreate_f.c \
			  ccreatekey_f.c \
			  cdelattr_f.c \
			  cdisconnect_f.c \
			  cdup_f.c \
			  cfree_f.c \
			  cfreekey_f.c \
			  cgetattr_f.c \
			  cgeterr_f.c \
			  cgetname_f.c \
			  cgetparent_f.c \
			  cgroup_f.c \
			  connect_f.c \
			  crank_f.c \
			  crgroup_f.c \
			  crsize_f.c \
			  csetattr_f.c \
			  cseterr_f.c \
			  csetname_f.c \
			  csize_f.c \
			  csplit_f.c \
			  ctestinter_f.c \
			  dimscreate_f.c \
			  dupfn_f.c \
			  errclass_f.c \
			  errcreate_f.c \
			  errfree_f.c \
			  errget_f.c \
			  errset_f.c \
			  errstring_f.c \
			  exscan_f.c \
			  finalize_f.c \
			  finalized_f.c \
			  freemem_f.c \
			  gather_f.c \
			  gatherv_f.c \
			  gcmp_f.c \
			  gdiff_f.c \
			  getaddress_f.c \
			  getcount_f.c \
			  getelem_f.c \
			  get_f.c \
			  getprocname_f.c \
			  getversion_f.c \
			  gexcl_f.c \
			  gfree_f.c \
			  gincl_f.c \
			  ginter_f.c \
			  grank_f.c \
			  graphcreate_f.c \
			  graphdimsget_f.c \
			  graphget_f.c \
			  graphmap_f.c \
			  graphnbrcount_f.c \
			  graphnbr_f.c \
			  grexcl_f.c \
			  grincl_f.c \
			  gsize_f.c \
			  gtranks_f.c \
			  gunion_f.c \
			  ibsend_f.c \
			  iccreate_f.c \
			  icmerge_f.c \
			  infocreate_f.c \
			  infodel_f.c \
			  infodup_f.c \
			  infofree_f.c \
			  infoget_f.c \
			  infogetnkeys_f.c \
			  infogetnth_f.c \
			  infogetvlen_f.c \
			  infoset_f.c \
			  inited_f.c \
			  init_f.c \
			  initthr_f.c \
			  iprobe_f.c \
			  irecv_f.c \
			  irsend_f.c \
			  isend_f.c \
			  issend_f.c \
			  isthrmain_f.c \
			  join_f.c \
			  keycreate_f.c \
			  keyfree_f.c \
			  mpil_id_f.c \
			  mpil_signal_f.c \
			  mpil_trace_f.c \
			  namelook_f.c \
			  namepub_f.c \
			  nameunpub_f.c \
			  opcreate_f.c \
			  opfree_f.c \
			  pack_f.c \
			  packsize_f.c \
			  pcontrol_f.c \
			  portclose_f.c \
			  portopen_f.c \
			  probe_f.c \
			  put_f.c \
			  querythr_f.c \
			  recv_f.c \
			  recvinit_f.c \
			  reduce_f.c \
			  reducescatter_f.c \
			  reqfree_f.c \
			  rsend_f.c \
			  rsendinit_f.c \
			  scan_f.c \
			  scatter_f.c \
			  scatterv_f.c \
			  send_f.c \
			  sendinit_f.c \
			  sendrecv_f.c \
			  sendrecvrep_f.c \
			  spawn_f.c \
			  spawnmult_f.c \
			  ssend_f.c \
			  ssendinit_f.c \
			  startall_f.c \
			  start_f.c \
			  tcommit_f.c \
			  tcontig_f.c \
			  tcreatehindex_f.c \
			  tcreatehvector_f.c \
			  tcreatekey_f.c \
			  tcreatestruct_f.c \
			  tdarray_f.c \
			  tdelattr_f.c \
			  tdup_f.c \
			  testall_f.c \
			  testany_f.c \
			  testcancel_f.c \
			  test_f.c \
			  testsome_f.c \
			  textent_f.c \
			  tfree_f.c \
			  tfreekey_f.c \
			  tgetattr_f.c \
			  tgetconts_f.c \
			  tgetenvl_f.c \
			  tgetextent_f.c \
			  tgetname_f.c \
			  tgettrue_f.c \
			  thindex_f.c \
			  thvector_f.c \
			  tindex_f.c \
			  tlb_f.c \
			  topotest_f.c \
			  tresize_f.c \
			  tsetattr_f.c \
			  tsetname_f.c \
			  tsize_f.c \
			  tstruct_f.c \
			  tsubarray_f.c \
			  tub_f.c \
			  tvector_f.c \
			  unpack_f.c \
			  waitall_f.c \
			  waitany_f.c \
			  wait_f.c \
			  waitsome_f.c \
			  wcomplete_f.c \
			  wcreateerr_f.c \
			  wcreate_f.c \
			  wcreatekey_f.c \
			  wdelattr_f.c \
			  wfence_f.c \
			  wfree_f.c \
			  wfreekey_f.c \
			  wgetattr_f.c \
			  wgeterr_f.c \
			  wgetname_f.c \
			  wgroup_f.c \
			  wpost_f.c \
			  wsetattr_f.c \
			  wseterr_f.c \
			  wsetname_f.c \
			  wstart_f.c \
			  wtick_f.c \
			  wtime_f.c \
			  wwait_f.c \
			  xbuoy_f.c \
			  xcoloron_f.c \
			  xcoloroff_f.c
