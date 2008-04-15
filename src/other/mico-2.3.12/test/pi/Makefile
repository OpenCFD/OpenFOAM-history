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

include ../../MakeVars

DIRS = poacurrent parameter-passing twoway-context twoway-request \
  oneway-context oneway-request transient-exception user-exception \
  rethrow-exception sending-received-exception requestinfo-validity \
  dii-request location-forward dii-location-forward dii-except \
  dii-client-intercept-except dsi-except pi-init servant-locator

all: prg

prg:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done

check:
	@for i in `find . -name 'run' -exec dirname \{} \;` ; \
		do echo "Testing $$i..."; CWD=`pwd`; cd $$i; ./run > log; \
		if cmp expected-output log >/dev/null; then :; else \
		echo "FAILED:"; echo "==============================="; \
		diff -u expected-output log; echo "==============================="; \
		fi; rm log; cd $$CWD; \
	done

install:
