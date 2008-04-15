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

all: .depend Cart.jar client

DIR_PREFIX=../../../

include ../../../../MakeVars

.SUFFIXES: .idl

%.h %.cc: %.idl
	$(IDL) -I. $(IDL_INCL) -DprimaryKey=_primaryKey \
               --ifdef-prefix=$(subst /,_,$(<D)) $<
	mv $(*F).cc $(<D)
	mv $(*F).h $(<D)

CXXFLAGS  := $(COS_CXXFLAGS) $(CXXFLAGS)
LDLIBS    := $(COS_LDLIBS) $(LDLIBS)
LDFLAGS   := $(COS_LDFLAGS) $(LDFLAGS)
DEPS      := $(COS_DEPS) $(DEPS)

IDL_INCL="-I$(MICO_HOME)/include/mico -I$(MICO_HOME)/include"

IDL_FILES= examples/Cart.idl \
           examples/CartHome.idl \
           examples/BookException.idl \
           examples/BookEx.idl \
           org/omg/boxedRMI/CORBA/seq1_WStringValue.idl \
           javax/ejb/EJBHome.idl \
           javax/ejb/EJBMetaData.idl \
           javax/ejb/HomeHandle.idl \
           javax/ejb/RemoveException.idl \
           javax/ejb/RemoveEx.idl \
           javax/ejb/Handle.idl \
           javax/ejb/EJBObject.idl \
           javax/ejb/CreateException.idl \
           javax/ejb/CreateEx.idl \
           java/lang/Throwable.idl \
           java/lang/ThrowableEx.idl \
           java/lang/Exception.idl \
           java/lang/Ex.idl \
           java/lang/Object.idl

examples/Cart.idl: examples/Cart.class
	rmic -classpath $(JBOSS_HOME)/client/jboss-j2ee.jar:. \
             -idl -noValueMethods examples.Cart

examples/CartHome.idl: examples/CartHome.class
	rmic -classpath $(JBOSS_HOME)/client/jboss-j2ee.jar:. \
             -idl -noValueMethods examples.CartHome

Cart.jar: examples/Cart.class examples/CartHome.class examples/CartBean.class \
          examples/BookException.class examples/IdVerifier.class \
          META-INF/ejb-jar.xml META-INF/jboss.xml
	jar cf Cart.jar $+

client: client.o $(subst .idl,.o,$(IDL_FILES)) $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $(subst .idl,.o,$(IDL_FILES)) $< -o $@

client.o: client.cc $(subst .idl,.h,$(IDL_FILES))

examples/BookException.h: examples/BookException.idl

examples/BookEx.h: examples/BookEx.idl

clean:
	rm -f .depend core client *.o *.jar `find . -name '*~' -print`
	rm -f examples/*.idl examples/*.cc examples/*.h examples/*.o
	rm -f examples/corba/*.java examples/corba/*.class examples/corba/*.o
	rm -f examples/corba/*.h examples/corba/*.cc examples/*.class 
	rm -rf java javax org

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.3.x/cart-ejb/META-INF
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.3.x/cart-ejb/examples
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.3.x/cart-ejb
	$(ISRCCMD) client.cc $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.3.x/cart-ejb
	$(ISCRIPTCMD) run-client $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.3.x/cart-ejb
	$(ISRCCMD) META-INF/ejb-jar.xml $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.3.x/cart-ejb/META-INF
	$(ISRCCMD) META-INF/jboss.xml $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.3.x/cart-ejb/META-INF
	$(ISRCCMD) examples/BookException.java $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.3.x/cart-ejb/examples
	$(ISRCCMD) examples/Cart.java $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.3.x/cart-ejb/examples
	$(ISRCCMD) examples/CartBean.java $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.3.x/cart-ejb/examples
	$(ISRCCMD) examples/CartHome.java $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.3.x/cart-ejb/examples
	$(ISRCCMD) examples/IdVerifier.java $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.3.x/cart-ejb/examples

