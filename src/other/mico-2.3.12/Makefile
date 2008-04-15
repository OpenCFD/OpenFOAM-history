#
# MICO --- a free CORBA implementation
# Copyright (C) 1997 Kay Roemer & Arno Puder
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# Send comments and/or bug reports to:
#                mico@informatik.uni-frankfurt.de
#

include MakeVars

ADMDIRS = admin include
LIBDIRS = orb ir idl auxdir coss ccm
PRGDIRS = orb ir cpp idl daemon auxdir coss ccm tools
EXTRADIRS = test demo
ALLDIRS = admin orb ir daemon idl cpp auxdir coss ccm test tools demo
INSTALLDIRS = admin orb ir daemon idl cpp auxdir coss ccm tools include man demo
# Bootstrap minimal build configuration
MINLIBDIRS = orb ir idl
MINPRGDIRS = cpp idl

all: system

world .NOTPARALLEL: system extra

system:
ifeq ($(USE_PCH_DIR), yes)
	-mkdir pch
endif
	for i in $(ADMDIRS); do $(MAKE) -C $$i adm || exit 1; done
	for i in $(LIBDIRS); do $(MAKE) -C $$i lib || exit 1; done
	for i in $(PRGDIRS); do $(MAKE) -C $$i prg || exit 1; done

extra:
	for i in $(EXTRADIRS); do $(MAKE) -C $$i prg || exit 1; done

clean:
	find . -name '*~' | xargs rm -f foobarbazbummbang
	for i in $(ALLDIRS); do $(MAKE) -C $$i clean || exit 1; done
	if test -f doc/Makefile; then $(MAKE) -C doc clean || exit 1; fi
	rm -rf libs
ifeq ($(USE_PCH_DIR), yes)
	rm -rf pch
endif
	rm -f FAQ core

distclean: clean
	rm -f demo/MakeVars.install admin/mico-c++ admin/mico-shc++
	rm -f admin/mico-ld admin/mico-shld admin/mico-ar admin/mico-setup.sh
	rm -f admin/mico-setup.csh demo/siegel/diffs
	rm -f admin/mico-config
	rm -f MakeVars
	rm -f config.*
	rm -f include/mico/config.h
	rm -f include/mico/config.idl

install:
	for i in $(INSTALLDIRS); do $(MAKE) -C $$i install || exit 1; done
	if test -f doc/doc.ps; then \
		$(IDIRCMD) $(SHARED_INSTDIR)/doc/mico; \
		$(IMANCMD) doc/doc.ps $(SHARED_INSTDIR)/doc/mico/manual.ps; \
	fi
	-ldconfig

install-cd:
	for i in $(INSTALLDIRS); do $(MAKE) -C $$i install-cd || exit 1; done
	if test -f doc/doc.ps; then \
		$(IDIRCMD) $(SHARED_INSTDIR)/doc/mico; \
		$(IMANCMD) doc/doc.ps $(SHARED_INSTDIR)/doc/mico/manual.ps; \
	fi

dist:
	admin/mksrcdist

release:
	admin/mkrelease

manifest: distclean
	admin/mkmanifest

autoconf:
	autoconf
	sed -e 's/^extern "C" void exit(int);/#include <stdlib.h>/g' < configure > configure.tmp
	mv configure.tmp configure
	chmod +x configure

dep:
	for i in $(ADMDIRS); do $(MAKE) -C $$i adm || exit 1; done
	for i in $(PRGDIRS); do $(MAKE) -C $$i .depend || exit 1; done

generate:
	make -C orb generate
	make -C ir generate
	make -C daemon generate
	make -C ccm generate

bootstrap:
ifeq ($(USE_CSL2), yes)
ifeq ($(USE_CSIV2), yes)
	@echo
	@echo "Starting MICO bootstrap..."
	@echo
ifeq ($(USE_PCH_DIR), yes)
	-mkdir pch
endif
	for i in $(ADMDIRS); do $(MAKE) -C $$i adm || exit 1; done
	for i in $(MINLIBDIRS); do $(MAKE) -C $$i lib || exit 1; done
	for i in $(MINPRGDIRS); do $(MAKE) -C $$i prg || exit 1; done
	make generate
	make clean
	make all
	find . -name '*.backup' -exec rm \{} \;
	@echo
	@echo "MICO bootstrap finished successfully"
	@echo
else
	@echo
	@echo "ERROR: you have to enable MICO CSIv2 for bootstrap (--enable-csiv2 configure option)"
	@echo
endif
else
	@echo
	@echo "ERROR: you have to enable MICO CSL2 for bootstrap (--enable-csl2 configure option)"
	@echo
endif


check:
	$(MAKE) -C test check

.depend:
	for i in $(ADMDIRS); do $(MAKE) -C $$i adm || exit 1; done
	for i in $(PRGDIRS); do $(MAKE) -C $$i .depend || exit 1; done
