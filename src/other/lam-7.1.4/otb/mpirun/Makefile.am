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
# $Id: Makefile.am,v 1.35 2003/11/24 18:49:09 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libltdl			= $(base)/libltdl/libltdlc.la
libs			= $(liblam) $(libltdl)

# This is ugly, but we need it so that people can change things at
# "make" time (e.g., "make sysconfdir=/foo/bar all") as documented in
# autoconf.

AM_CPPFLAGS		= \
			-DLAM_SYSCONFDIR="\"$(sysconfdir)\"" \
			-DLAM_BINDIR="\"$(bindir)\"" \
			$(LIBCR_INCLUDE)

# This is not quite in the Automake spirit, but we have to do it.
# Since the totalview portion of the library must be built with -g, we
# must eliminate the CFLAGS that are passed in here by default (which
# may already have debugging and/or optimization flags).  We use
# post-processed forms of the CFLAGS in the library targets down
# below.

CFLAGS			= $(CFLAGS_WITHOUT_OPTFLAGS) $(TOTALVIEW_DEBUG_FLAGS)

bin_PROGRAMS		= mpirun
mpirun_SOURCES		= mpirun.c totalview.c 
mpirun_LDADD		= $(libs) $(LIBCR_LIBS) $(LIBLAM_EXTRA_LIBS)
mpirun_LDFLAGS		= $(LIBCR_LDFLAGS) $(LIBLAM_EXTRA_LDFLAGS)
mpirun_DEPENDENCIES	= $(libs)

# We only install the impirun sym link if IMPI support was included.

if WANT_IMPI_BUILD
impi_install_target	= install-impirun
else
impi_install_target	=
endif

# Hence, we only install the target specified (or not specified, as
# the case may be) by $(impi_install_target).

install-exec-hook: $(impi_install_target)

# We can alwasy remove sym links, regardless of whether IMPI was
# installed or not.  This seems to be a bit more conservative/safer.

uninstall-local: uninstall-impirun

install-impirun:
	(cd $(DESTDIR)$(bindir); rm -f impiruni$(EXEEXT); $(LN_S) mpirun$(EXEEXT) impirun$(EXEEXT))

uninstall-impirun:
	rm -f $(DESTDIR)$(bindir)/impirun$(EXEEXT)
