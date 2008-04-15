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
# $Id: Makefile.am,v 6.72 2003/07/05 18:30:01 jsquyres Exp $
#

include $(top_srcdir)/config/Makefile.options

# Are we building ROMIO?

if WANT_ROMIO
romio_all_target	= romio-all
romio_install_target	= romio-install
romio_examples_target	= romio-examples
else
romio_all_target	=
romio_install_target	=
romio_examples_target	=
endif

# Subdirectory list

BASE_SUBDIRS		= config doc share otb tools etc man
SUBDIRS			= $(BASE_SUBDIRS)
DIST_SUBDIRS		= $(BASE_SUBDIRS) examples

EXTRA_DIST		= \
			AUTHORS \
			HISTORY \
			INSTALL \
			LICENSE \
			README \
			VERSION

# 'cause this is just so annoying!!!  This may not work anywhere
# outside iu.edu.  :-)

TAR			= gtar

# We used to have a depend target, but we now use the Automake 1.5 
# dependency system.  Since some people (Brian) aren't used to this,
# print out a reminder message

depend:
	@ echo "Dependency analysis now done automagically by"
	@ echo "GNU Automake.  No need to run 'make depend'"

# For building all the examples

lamexamples: $(romio_examples_target)
	@ (cd examples; $(MAKE) all)
	@ $(MAKE) examples-done

# Supplement various targets so that we can build ROMIO as well.  Note
# that we only build and install ROMIO if ROMIO was selected in
# configure, but we *always* [try to] clean, distclean, and uninstall
# ROMIO.  This counts for the corner case where a user could do:
#
# ./configure --with-romio
# make all install
# ./configure --without-romio
# make uninstall distclean
#
# If we didn't always traverse into the romio/ directory for uninstall
# and clean, ROMIO will still be left installed, which would not be
# social for us to do.  This scenario doesn't account for all cases
# (mainly because ROMIO still has its own configure script) but it
# does handle the majority of cases.  For example, the following
# scenario won't be handled correctly:
#
# ./configure --with-romio
# make all install
# make distclean
# ./configure --without-romio
# make uninstall
#
# ROMIO will still left installed after this becuase ROMIO's
# ./configure won't have been triggered again, and therefore there
# will be no Makefile (et al.) in the romio/ directory to travese and
# do the uninstall.  
#
# Someday, when ROMIO is fully integrated into LAM, this won't be an
# issue.

# Need to have a separate $(MAKE) invocation for the "all-done" target
# so that make won't perform the "all-done" target before the
# $(romio_all_target).  This is the only way to force/guarantee the
# ordering.

all-local: $(romio_all_target)
	@ $(MAKE) all-done

install-exec-local: $(romio_install_target)

# The presence of the .lam_built file triggers whether the all-done
# target generates the "LAM has been built..." message.

clean-local: romio-clean
	@ rm -f .lam_built

distclean-local: romio-distclean

uninstall-local: romio-uninstall

# Be sure to set LAMHOME properly so that hcc/hcp/hf77 use the right
# libraries/include files (to avoid conflicts if the user may already
# have a LAM installed, and already have LAMHOME set somewhere else).

# Another bug in ROMIO -- we have to go set the unix permissions on
# directories under $LAMHOME/include to be 755 again.  ROMIO does a
# chmod 644 $LAMHOME/include/* -- ick!  The ROMIO authors have been
# notified of this.  13 June 1999/JMS.

# Automakeizm -- check to see if we already built ROMIO.  "make
# install" implicitly invokes "make all" first (in a roundabout way).

romio-all:
	@ echo "Making all in romio"
	@ (if test ! -f romio/.lam_built; then cd romio; $(MAKE) && touch .lam_built; fi)

romio-install:
	@ echo "Making install in romio"
	@ if test ! -f romio/.lam_built ; then \
	    $(MAKE) romio-all && $(MAKE) romio-install-redirect ; \
	  else \
	    $(MAKE) romio-install-redirect; \
	  fi

romio-install-redirect:
	@ (if test -d romio; then \
	     cd romio ; \
	     if test -f Makefile; then \
	       ( $(MAKE) install PREFIX=$(DESTDIR)$(prefix) \
			DATADIR=$(DESTDIR)$(datadir) \
			LIBDIR=$(DESTDIR)$(libdir) \
			INCLUDEDIR=$(DESTDIR)$(includedir) \
			MANDIR=$(DESTDIR)$(mandir); ) \
	     fi ; \
	   fi)

romio-examples:
	@ echo "Making lamexamples in romio"
	@ (if test -d romio; then \
	     cd romio; \
	     if test -f Makefile; then \
		if test -f .lam_built ; then \
		    ( cd test; \
		      $(MAKE) CC=mpicc F77=mpif77 LIBS= USER_CFLAGS="$(CFLAGS)" USER_FFLAGS="$(FFLAGS) $(ROMIO_FLAGS)" ) ; \
		else \
		    echo "*** Error: it does not appear that ROMIO has been built yet." ; \
		    echo "*** I cannot tell if LAM was built yet or not, so I will abort." ; \
		    echo "*** Please build ROMIO the normal LAM build interface" ; \
		    echo "*** and then attempt to build the ROMIO again." ; \
		fi; \
	     fi; \
	   fi )

romio-clean:
	@ echo "Making clean in romio"
	@ (if test -d romio; then \
	     cd romio; \
	     rm -f .lam_built; \
	     if test -f Makefile ; then \
		$(MAKE) clean; \
	     fi; \
	   fi )

romio-distclean:
	@ echo "Making distclean in romio"
	@ (if test -d romio; then \
	     cd romio; \
	     rm -f .lam_built config.command; \
	     if test -f Makefile ; \
		then $(MAKE) cleanall; \
	     fi ; \
	   fi)

romio-uninstall:
	@ echo "Making uninstall in romio"
	@ if test -f romio/.lam_built ; then \
	    $(MAKE) romio-uninstall-redirect; \
	  fi

# This is just so ugly; it's painful.

romio-uninstall-redirect:
	@ ( cd romio; $(MAKE) uninstall PREFIX=$(DESTDIR)$(prefix) \
			DATADIR=$(DESTDIR)$(datadir) \
			LIBDIR=$(DESTDIR)$(libdir) \
			INCLUDEDIR=$(DESTDIR)$(includedir) \
			MANDIR=$(DESTDIR)$(mandir); )
	   if test -f $(DESTDIR)$(libdir)/liblammpio.a; then \
	     rm -f $(DESTDIR)$(libdir)/liblammpio.a; \
	   fi

#
# For printing friendly messages at the end :-)
#

examples-done:
	@echo
	@echo "==========================================================================="
	@echo
	@echo "The examples for LAM/MPI $(LAM_VERSION) have been built."
	@echo
	@echo "Look in the subdirectories of examples/ for each of"
	@echo "the example programs.  Each subdirectory has a README file"
	@echo "that explains how to run it, etc."
	@echo
	@echo "==========================================================================="

all-done: .lam_built
.lam_built:
	@ echo
	@ echo "==========================================================================="
	@ echo
	@ echo "LAM/MPI $(LAM_VERSION) has been built."
	@ echo
	@ echo "You now need to install it with \"make install\""
	@ echo
	@ echo "==========================================================================="
	@ touch .lam_built

install-exec-hook:
	@echo
	@echo "==========================================================================="
	@echo
	@echo "Installation of LAM/MPI $(LAM_VERSION) is complete."
	@echo
	@echo "Be sure to visit the LAM/MPI web page: http://www.lam-mpi.org/"
	@echo
	@echo "The FAQ and LAM mailing list archives (both accessable from the web page)"
	@echo "contain much information about getting started with LAM/MPI, as well as"
	@echo "solutions to common problems."
	@echo
	@echo "==========================================================================="
	@echo

#
# For making the distribution.  LAM maintainers only.  It won't work
# for you.  :-)
#
# Requires the "sowing" package from ftp://ftp.mcs.anl.gov/ to make
# the man pages.
#

dist-hook:
	csh -f config/distscript.csh $(DESTDIR)$(distdir)
