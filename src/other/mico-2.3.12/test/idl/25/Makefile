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


all: stage2.idl
	@if cmp stage1.idl stage2.idl >/dev/null; then :; \
	else \
	  echo ===== pragma test failed ====================; \
	  diff -c stage1.idl stage2.idl; \
	fi

stage2.idl : stage1.idl
	$(IDL) --no-codegen-c++ --codegen-idl --emit-repoids \
               --name=stage2 stage1.idl

stage1.idl : pragma.idl
	$(IDL) --no-codegen-c++ --codegen-idl --emit-repoids \
               --name=stage1 pragma.idl

clean:
	rm -f stage?.idl core *~


