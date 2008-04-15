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
# $Id: Makefile.am,v 1.3 2003/07/07 23:22:12 jsquyres Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
                        -DLAM_BUILDING=1 \
                        -I$(top_lam_builddir)/share/include \
                        -I$(top_lam_srcdir)/share/include \
                        -DLAM_SYSCONFDIR="\"$(sysconfdir)\"" \
			$(LIBCR_INCLUDE)

noinst_LTLIBRARIES      = libblcr.la
libblcr_la_SOURCES      = \
			  lam-ssi-crlam-blcr.h \
			  ssi_crlam_blcr.c \
			  ssi_crlam_blcr_module.c
