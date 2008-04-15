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
# $Id: Makefile.am,v 1.24 2004/01/12 22:26:25 pkambadu Exp $
#

include $(top_srcdir)/config/Makefile.options

base			= $(top_builddir)/share
liblam			= $(base)/liblam/liblam.la
libs			= $(liblam)
bootdir			= $(base)/boot
incdirs			= -I$(bootdir)

# This is ugly, but we need it so that people can change things at
# "make" time (e.g., "make sysconfdir=/foo/bar all") as documented in
# autoconf.

AM_CPPFLAGS		= \
			-DLAM_PREFIX="\"$(prefix)\"" \
			-DLAM_INCDIR="\"$(includedir)\"" \
			-DLAM_LIBDIR="\"$(libdir)\""

bin_PROGRAMS		= mpicc mpic++ mpif77

# We unfortunately can't make a library for the common sources because
# making a library of C++ sources is not standardized.  :-( We also
# can't just list the same .cc files in multiple targets, because some
# C++ compilers will actually compile them multiple times (i.e., once
# for each target).  This results in a timestamp for wrap.o being
# later than already-finished targets (e.g., when wrap.o may end up
# with a timestamp before mpif77, but after mpicc).  So we have to sym
# link the common sources to unique names for each wrapper compiler
# and let them effectively be compiled once for each wrapper.

EXTRA_DIST		= wrap.cc wrap_engine.cc

mpicc_SOURCES		= mpicc.cc wrap_cc.cc wrap_engine_cc.cc
mpicc_LDADD		= $(libs) $(LIBLAM_EXTRA_LIBS)
mpicc_LDFLAGS		= $(LIBLAM_EXTRA_LDFLAGS)
mpicc_DEPENDENCIES	= $(libs)

mpic___SOURCES		= mpicxx.cc wrap_cxx.cc wrap_engine_cxx.cc
mpic___LDADD		= $(libs) $(LIBLAM_EXTRA_LIBS)
mpic___LDFLAGS		= $(LIBLAM_EXTRA_LDFLAGS)
mpic___DEPENDENCIES	= $(libs)

mpif77_SOURCES		= mpif77.cc wrap_f77.cc wrap_engine_f77.cc
mpif77_LDADD		= $(libs) $(LIBLAM_EXTRA_LIBS)
mpif77_LDFLAGS		= $(LIBLAM_EXTRA_LDFLAGS)
mpif77_DEPENDENCIES	= $(libs)

# Make the sym links

wrap_cc.cc:
	$(LN_S) $(top_srcdir)/tools/wrappers/wrap.cc wrap_cc.cc
wrap_engine_cc.cc:
	$(LN_S) $(top_srcdir)/tools/wrappers/wrap_engine.cc wrap_engine_cc.cc
wrap_cxx.cc:
	$(LN_S) $(top_srcdir)/tools/wrappers/wrap.cc wrap_cxx.cc
wrap_engine_cxx.cc:
	$(LN_S) $(top_srcdir)/tools/wrappers/wrap_engine.cc wrap_engine_cxx.cc
wrap_f77.cc:
	$(LN_S) $(top_srcdir)/tools/wrappers/wrap.cc wrap_f77.cc
wrap_engine_f77.cc:
	$(LN_S) $(top_srcdir)/tools/wrappers/wrap_engine.cc wrap_engine_f77.cc

# Since this is C++ and we use templates (the STL), also ditch the
# template repository directory

clean-local:
	test -z "$(LAM_CXX_TEMPLATE_REPOSITORY)" || $(RM) -rf $(LAM_CXX_TEMPLATE_REPOSITORY)
	rm -f wrap_cc.cc wrap_engine_cc.cc
	rm -f wrap_cxx.cc wrap_engine_cxx.cc
	rm -f wrap_f77.cc wrap_engine_f77.cc


# Per bug 476, the "wipe" command is now "lamwipe".  So that we don't
# break backwards compatability, we provide sym link names to the old
# names (i.e., lamwipe -> wipe).  However, we provide a
# --disable-deprecated-executable-names option that will disable this
# behavior.  Someday, we'll change the default and the option will
# become --enable-deprecated-executable-names.

if WANT_DEPRECATED_EXECUTABLE_NAMES
if CASE_SENSITIVE_FS
install-exec-hook:
	(cd $(DESTDIR)$(bindir); rm -f hcc$(EXEEXT); $(LN_S) mpicc$(EXEEXT) hcc$(EXEEXT))
	(cd $(DESTDIR)$(bindir); rm -f hcp$(EXEEXT); $(LN_S) mpiCC$(EXEEXT) hcp$(EXEEXT))
	(cd $(DESTDIR)$(bindir); rm -f hf77$(EXEEXT); $(LN_S) mpif77$(EXEEXT) hf77$(EXEEXT))
	(cd $(DESTDIR)$(bindir); rm -f mpiCC$(EXEEXT); $(LN_S) mpic++$(EXEEXT) mpiCC$(EXEEXT))
else
install-exec-hook:
	(cd $(DESTDIR)$(bindir); rm -f hcc$(EXEEXT); $(LN_S) mpicc$(EXEEXT) hcc$(EXEEXT))
	(cd $(DESTDIR)$(bindir); rm -f hcp$(EXEEXT); $(LN_S) mpic++$(EXEEXT) hcp$(EXEEXT))
	(cd $(DESTDIR)$(bindir); rm -f hf77$(EXEEXT); $(LN_S) mpif77$(EXEEXT) hf77$(EXEEXT))
endif

#
# mpiCC might be a symlink we created, so be nice....
#
if CASE_SENSITIVE_FS
uninstall-local:
	rm -f $(DESTDIR)$(bindir)/hcc$(EXEEXT) \
		$(DESTDIR)$(bindir)/hcp$(EXEEXT) \
		$(DESTDIR)$(bindir)/hf77$(EXEEXT) \
		$(DESTDIR)$(bindir)/mpiCC$(EXEEXT)
else
uninstall-local:
	rm -f $(DESTDIR)$(bindir)/hcc$(EXEEXT) \
		$(DESTDIR)$(bindir)/hcp$(EXEEXT) \
		$(DESTDIR)$(bindir)/hf77$(EXEEXT)
endif
#
# If deprecated names are not enabled, then we still need the install-eec-hook 
# and uninstall-local targets. Else "make install" fails. So, we are adding dummy
# install-exec-hook and uninstall-local targets
else
install-exec-hook:
uninstall-local:
endif
