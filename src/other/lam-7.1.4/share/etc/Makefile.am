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
# $Id: Makefile.am,v 1.31 2003/10/10 17:41:09 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
			-DLAM_SYSCONFDIR="\"$(sysconfdir)\"" \
			-DLAM_PREFIX="\"$(prefix)\""

noinst_LTLIBRARIES	= liblametc.la
liblametc_la_SOURCES	= \
			  T.c \
			  all_array.c \
			  all_hash.c \
			  all_list.c \
			  all_queue.c \
			  all_shash.c \
			  all_slist.c \
			  all_squeue.c \
			  atos.c \
			  blktype.c \
			  bootsockio.c \
			  combine.c \
			  f2cstring.c \
			  fail.c \
			  few.c \
			  fork.c \
			  getinetaddr.c \
			  getworkdir.c \
			  get_ncpus.c \
			  ieee.c \
			  itoa.c \
			  kill.c \
			  kiproc.c \
			  lam_gethostby.c \
			  lam_gethostname.c \
			  lam_getpid.c \
			  lamcommdebug.c \
			  lamdebug.c \
			  lamlog.c \
			  lamunix.c \
			  ldtimer.c \
			  mempool.c \
			  memucmp.c \
			  microsleep.c \
			  mpiformout.c \
			  mpitr_com.c \
			  mpitr_dtprint.c \
			  mpitr_dtype.c \
			  mrw.c \
			  nmsgconv.c \
			  nodespin.c \
			  panic.c \
			  pathsearch.c \
			  prime.c \
			  pty.c \
			  registry.c \
			  rev.c \
			  show_help.c \
			  show_version.c \
			  snprintf.c \
			  socket.c \
			  srfd.c \
			  stoi.c \
			  strncpy.c \
			  terror.c \
			  trdrain_mpi.c \
			  ttime.c \
			  vctable.c \
			  vsnprintf.c
