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
# $Id: Makefile.am,v 1.1 2003/12/02 16:28:38 pcharapa Exp $
#

include $(top_lam_srcdir)/config/Makefile.options

INCLUDES                = \
                        -DLAM_BUILDING=1 \
                        -I$(top_lam_builddir)/share/include \
                        -I$(top_lam_srcdir)/share/include \
                        -DLAM_SYSCONFDIR="\"$(sysconfdir)\"" \
			$(LIBCR_INCLUDE)

sysconf_DATA		= lam-ssi-crmpi-self-helpfile
EXTRA_DIST		= $(sysconf_DATA)

noinst_LTLIBRARIES      = libself.la
libself_la_SOURCES      = \
			  lam-ssi-crmpi-self.h \
			  ssi_crmpi_self.c \
			  ssi_crmpi_self_module.c
