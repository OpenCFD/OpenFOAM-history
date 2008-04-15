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
# $Id: Makefile.am,v 1.2 2004/03/09 16:56:14 pcharapa Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

AM_CPPFLAGS		= \
                        -DLAM_BUILDING=1 \
                        -I$(top_lam_builddir)/share/include \
                        -I$(top_lam_srcdir)/share/include \
                        -DLAM_SYSCONFDIR="\"$(sysconfdir)\""

sysconf_DATA		= lam-ssi-crlam-self-helpfile
EXTRA_DIST		= $(sysconf_DATA)

noinst_LTLIBRARIES      = libself.la
libself_la_SOURCES      = \
			  lam-ssi-crlam-self.h \
			  ssi_crlam_self.c \
			  ssi_crlam_self_module.c
