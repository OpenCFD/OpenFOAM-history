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

all: .depend client ConverterApp.ear

DIR_PREFIX=../
include ../../MakeVars

CXXFLAGS  := $(COS_CXXFLAGS) $(CXXFLAGS)
LDLIBS    := $(COS_LDLIBS) $(LDLIBS)
LDFLAGS   := $(COS_LDFLAGS) $(LDFLAGS)
DEPS      := $(COS_DEPS) $(DEPS)


client: libejbcpp.a Converter.o ConverterHome.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) client.o Converter.o ConverterHome.o -L. -lejbcpp $(LDLIBS) -o $@

ConverterHome.h ConverterHome.cc: ConverterHome.idl
	$(IDL) $(IDLFLAGS) -DprimaryKey=primKey ConverterHome.idl

Converter.h Converter.cc: Converter.idl
	$(IDL) $(IDLFLAGS) -DprimaryKey=primKey Converter.idl

libejbcpp.a: Converter.idl ConverterHome.idl
	./gen-libejbcpp

Converter.idl: Converter.class
	rmic -classpath $(J2EE_HOME)/lib/j2ee.jar:. \
             -idl -noValueMethods Converter

ConverterHome.idl: ConverterHome.class
	rmic -classpath $(J2EE_HOME)/lib/j2ee.jar:. \
             -idl -noValueMethods ConverterHome

ConverterApp.ear: ConverterEJB.jar
	packager -enterpriseArchive ConverterEJB.jar ConverterApp ConverterApp.ear
	packager -setRuntime ConverterApp.ear converter-ri.xml

ConverterEJB.jar: Converter.class ConverterHome.class ConverterBean.class
	packager -ejbJar `pwd` Converter.class:ConverterHome.class:ConverterBean.class converter-jar.xml ConverterEJB.jar

clean:
	rm -f .depend *.o core client *~
	rm -rf *.a *.h *.idl ConverterHome.cc Converter.cc
	rm -rf *.class *.jar *.ear java javax org
