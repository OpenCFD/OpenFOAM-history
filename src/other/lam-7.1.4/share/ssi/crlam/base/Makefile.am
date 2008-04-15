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
# $Id: Makefile.am,v 1.8 2003/11/23 15:10:54 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

AM_CPPFLAGS			= -DLAM_SYSCONFDIR="\"$(sysconfdir)\""

noinst_LTLIBRARIES              = libssi_crlam_base.la
libssi_crlam_base_la_SOURCES       = \
				  ssi_crlam.c \
				  ssi_crlam_close.c \
				  ssi_crlam_open.c 

distclean-local:
	-rm -f lam-ssi-crlam-modules.h
