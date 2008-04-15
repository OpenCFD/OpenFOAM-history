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
# $Id: Makefile.am,v 1.39 2003/11/15 16:25:10 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

# Note that the vast majority of these files are not required to be
# installed for proper MPI functionality.  Only mpi.h and mpif.h need
# be installed.

impi			= \
			  hostack_mgmt.h \
			  impi.h \
			  impi-defs.h \
			  impid.h \
			  impid-cc.h \
			  lamdebug-cc.h \
			  longbuf_mgmt.h \
			  packet_mgmt.h \
			  req_mgmt.h 

trillium		= \
			  FREQF.h \
			  MPISYSF.h \
			  NETF.h \
			  all_array.h \
			  all_hash.h \
			  all_list.h \
			  all_opt.h \
			  all_queue.h \
			  all_slist.h \
			  app_mgmt.h \
			  app_schema.h \
			  args.h \
			  bfreq.h \
			  blktype.h \
			  boot.h \
			  dl_inet.h \
			  ereq.h \
			  etc_misc.h \
			  events.h \
			  flatreq.h \
			  freq.h \
			  hreq.h \
			  kio.h \
			  kreq.h \
			  ksignal.h \
			  ktrace.h \
			  lam.h \
			  lam_build_info.h \
			  lam_ksignal.h \
			  lam_ltdl.h \
			  lam_network.h \
			  lamdebug.h \
			  laminternal.h \
			  lammpithreads.h \
			  lamnet.h \
			  lamthreads.h \
			  lam-totalview.h \
			  lam-ssi.h \
			  lam-ssi-boot.h \
			  lam-ssi-coll.h \
			  lam-ssi-cr.h \
			  lam-ssi-rpi.h \
			  lamwrap.h \
			  lreq.h \
			  malloc_wrapper.h \
			  mpi_apps.h \
			  mpi_types.h \
			  mpisys.h \
			  mpitrace.h \
			  ndi.h \
			  net.h \
			  pdi.h \
			  portable.h \
			  preq.h \
			  priority.h \
			  proc_schema.h \
			  rpisys.h \
			  rreq.h \
			  sfh.h \
			  t_types.h \
			  terror.h \
			  trreq.h \
			  tstdio.h \
			  typical.h \
			  vctable.h \
			  vreq.h

# Do some conditionals to see if we want to install the full set of
# Trillium header files.  This is not required for correct MPI
# functionality -- it is only required for developers who want to use
# the underlying Trillium interface.

if WANT_TRILLIUM_INSTALL
trillium_files		= $(trillium)
else
trillium_files		=
endif

# We really don't need to install the Fortran header file if we don't
# want fortran - what are we going to do?  We can't link anyway....
if WANT_FORTRAN
fortran_files		= mpif.h
else
fortran_files		=
endif

# We really don't need to install the C++ header files if we don't
# want C++ - what are we going to do?  We can't link anyway....
if WANT_MPI2CPP
mpi2cpp_files		= mpicxx.h
else
mpi2cpp_files		=
endif

nodist_include_HEADERS	= lam_config.h


include_HEADERS		= $(trillium_files) \
			  $(fortran_files) \
			  $(mpi2cpp_files) \
			  lam_config_bottom.h \
			  mpi.h

SUBDIRS = mpi2cxx

EXTRA_DIST		= $(trillium) \
			  $(impi) \
			  mpif.h \
			  mpicxx.h

# Add a hook to run *after* the file lam_config.h has been installed
# out to the target location.  It changes the pesky PACKAGE_* macros
# that autoconf automatically generates (and there is no way of
# turning off) into LAM_PACKAGE_* in order to make <mpi.h> safe to
# include with other files.

install-data-hook:
	sed -e 's/define PACKAGE/define LAM_PACKAGE/' \
		$(DESTDIR)$(includedir)/lam_config.h \
		> $(DESTDIR)$(includedir)/lam_config.h.install
	$(CP) $(DESTDIR)$(includedir)/lam_config.h.install \
		$(DESTDIR)$(includedir)/lam_config.h
	$(RM) -f $(DESTDIR)$(includedir)/lam_config.h.install
	chmod 0644 $(DESTDIR)$(includedir)/lam_config.h
