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
# $Id: Makefile.am,v 1.29 2004/01/12 22:36:13 pkambadu Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libs			= $(liblam)

# This is ugly, but we need it so that people can change things at
# "make" time (e.g., "make sysconfdir=/foo/bar all") as documented in
# autoconf.

AM_CPPFLAGS		= -DLAM_SYSCONFDIR="\"$(sysconfdir)\""

bin_PROGRAMS		= lamwipe
lamwipe_SOURCES		= wipe.c
lamwipe_LDADD		= $(libs) $(LIBLAM_EXTRA_LIBS)
lamwipe_LDFLAGS		= $(LIBLAM_EXTRA_LDFLAGS)
lamwipe_DEPENDENCIES	= $(libs)

# Per bug 476, the "wipe" command is now "lamwipe".  So that we don't
# break backwards compatability, we provide sym link names to the old
# names (i.e., lamwipe -> wipe).  However, we provide a
# --disable-deprecated-executable-names option that will disable this
# behavior.  Someday, we'll change the default and the option will
# become --enable-deprecated-executable-names.

if WANT_DEPRECATED_EXECUTABLE_NAMES
install-exec-hook:
	(cd $(DESTDIR)$(bindir); rm -f wipe$(EXEEXT); $(LN_S) lamwipe$(EXEEXT) wipe$(EXEEXT))

uninstall-local:
	rm -f $(DESTDIR)$(bindir)/wipe$(EXEEXT)
endif
