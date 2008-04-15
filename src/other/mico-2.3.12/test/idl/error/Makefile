#
# MICO --- a CORBA 2.0 implementation
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

include ../../../MakeVars


all: check


check:
	@for i in case*.idl; do \
	  echo Testing $$i...; \
	  if $(IDL) $$i 2>&1 | tail -1 | cmp expected-stderr-$$i - >/dev/null;  then :;\
	  else \
	    echo FAILED; \
	  fi; \
	done

generate:
	@for i in case*.idl; do \
	  echo Generating $$i...; \
	  $(IDL) $$i 2>&1 | tail -1 > expected-stderr-$$i; \
	done

