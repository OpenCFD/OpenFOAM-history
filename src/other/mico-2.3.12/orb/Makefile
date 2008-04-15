#
#  MICO --- an Open Source CORBA implementation
#  Copyright (c) 1997-2001 by The Mico Team
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Library General Public
#  License as published by the Free Software Foundation; either
#  version 2 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Library General Public License for more details.
#
#  You should have received a copy of the GNU Library General Public
#  License along with this library; if not, write to the Free
#  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#  For more information, visit the MICO Home Page at
#  http://www.mico.org/
#

include ../MakeVars

CXXFLAGS := -I../include $(PCHFLAGS) $(CXXFLAGS)

ifeq ($(OSTYPE), windows)
ifeq ($(HAVE_SHARED), yes)
CXXFLAGS := -DBUILD_MICO_DLL $(CXXFLAGS)
endif
endif

PLATFORM_LIBS =
ifeq ($(HAVE_CYGWIN), yes)
ifeq ($(USE_CSL2), yes)
PLATFORM_LIBS := $(PLATFORM_LIBS) -lssl -lcrypto
else
ifeq ($(USE_CSIV2), yes)
PLATFORM_LIBS := $(PLATFORM_LIBS) -lssl -lcrypto
else
ifeq ($(HAVE_SSL), yes)
PLATFORM_LIBS := $(PLATFORM_LIBS) -lssl -lcrypto
endif
endif
endif
endif

ifeq ($(HAVE_ORB_EXCEPTS), no)
CXXFLAGS := $(CXXFLAGS) $(NOEHFLAGS) -DMICO_CONF_NO_EXCEPTIONS
else
ifeq ($(HAVE_EXCEPTIONS), yes)
NOPICFLAGS := $(NOPICFLAGS) $(EHOPTFLAGS)
else
NOPICFLAGS := $(NOPICFLAGS) $(NOEHFLAGS)
endif
ifeq ($(HAVE_SHARED_EXCEPTS), yes)
PICFLAGS := $(PICFLAGS) $(EHOPTFLAGS)
else
PICFLAGS := $(PICFLAGS) $(NOEHFLAGS)
endif
endif

EH_SRCS = except2.cc
UNI_SRCS = uni_base64.cc uni_unicode.cc uni_fromuni.cc uni_touni.cc
PI_SRC = pi.o pi_impl.o typecode_seq.o
MT_SRCS = os-thread.cc operation.cc message.cc \
  mt_dispatcher.cc mt_manager.cc
TIME_SRC = timebase.cc

###ras
CSL2_SRCS = security/security.o \
	  security/securitylevel1.o \
	  security/securitylevel1_impl.o \
	  security/securitylevel2.o \
	  security/securitylevel2_impl.o \
	  security/audit_impl.o \
	  security/SecurityAdmin.o \
	  security/SecurityAdmin_impl.o \
	  security/AuditClientIntercept.o \
	  security/AuditOptions.o \
	  security/AuditServIntercept.o \
	  security/ODMConfig.o \
	  security/DomainManager.o \
	  security/odm.o \
	  security/odm_impl.o \
	  security/DomainManager_impl.o \
	  security/AccessConfig.o \
	  security/RightsConfig.o \
	  security/AuditConfig.o

#karel
CSIV2_SRCS = security/csi_base.cc security/csi.cc security/csiiop.cc \
  security/gssup.cc security/csiv2.cc security/csiv2_impl.cc

ifeq ($(HAVE_MEMCHECK), yes)
SRCS = memcheck.cc
ifeq ($(USE_MEMTRACE), yes)
SRCS := $(SRCS) memtrace.cc
endif
endif

ifeq ($(HAVE_FINAL), no)
ifeq ($(OSTYPE), windows)
SRCS := $(SRCS) os-windows.cc
else
SRCS := $(SRCS) os-unix.cc
endif
endif

ifeq ($(HAVE_FINAL), yes)
SRCS := orb_all.cc $(SRCS) $(EH_SRCS) $(UNI_SRCS)
STATIC_OBJS = $(SRCS:.cc=.o)
else
SRCS := $(SRCS) dii.cc typecode.cc any.cc codec.cc buffer.cc context.cc \
  except.cc dispatch.cc string.cc object.cc address.cc ior.cc \
  orb.cc dsi.cc transport.cc transport/tcp.cc transport/udp.cc transport/unix.cc \
  iop.cc util.cc basic_seq.cc fast_array.cc \
  ssl.cc fixed.cc intercept.cc codeset.cc queue.cc static.cc \
  current.cc policy_impl.cc service_info.cc ioptypes.cc ssliop.cc \
  value.cc valuetype.cc valuetype_impl.cc dynany_impl.cc \
  policy2.cc tckind.cc orb_excepts.cc policy.cc poa.cc poa_base.cc \
  poa_impl.cc dynany.cc $(UNI_SRCS) $(EH_SRCS) $(PI_SRC) $(TIME_SRC) \
  ir.cc ir_base.cc imr.cc mtdebug.cc reflection.cc

ifeq ($(USE_WIRELESS), yes)
SRCS := $(SRCS) watm.cc
endif

ifeq ($(HAVE_BLUETOOTH), yes)
SRCS := $(SRCS) ltp_address.cc ltp_ior.cc transport/ltp.cc
endif

ifeq ($(HAVE_THREADS), yes)
SRCS := $(SRCS) $(MT_SRCS)
endif
STATIC_OBJS = $(SRCS:.cc=.o)
endif

ifeq ($(USE_CSL2), yes)
ifeq ($(HAVE_FINAL), no)
SRCS := $(SRCS) $(CSL2_SRCS)
else
SRCS := $(SRCS) security/securitylevel2_impl.cc security/audit_impl.cc \
	security/odm_impl.cc security/DomainManager_impl.cc security/AccessConfig.cc \
	security/RightsConfig.cc security/AuditConfig.cc
endif
endif

ifeq ($(HAVE_FINAL), no)
ifeq ($(USE_CSIV2), yes)
SRCS := $(SRCS) $(CSIV2_SRCS)
#SRCS := $(CSIV2_SRCS) $(SRCS)
endif
endif

SHARED_OBJS = $(STATIC_OBJS:.o=.pic.o)
EH_STATIC_OBJS=$(EH_SRCS:.cc=.o)
EH_SHARED_OBJS=$(EH_SRCS:.cc=.pic.o)

REG_FILES = ../admin/code_set_registry.txt ../admin/mico_code_set_registry.txt

#
# Rules
#

all: lib prg

prg:

ifeq ($(OSTYPE), windows)
ifeq ($(HAVE_SHARED), yes)
lib: .depend libmico$(VERSION).a mico$(NODOTVERSION).dll
else
lib: .depend libmico$(VERSION).a
endif

else

ifeq ($(HAVE_SHARED), yes)
ifeq ($(HAVE_STATIC), yes)
lib: .depend libmico$(VERSION).$(SOEXT) libmico$(VERSION).a
else
lib: .depend libmico$(VERSION).$(SOEXT)
endif
else
lib: .depend libmico$(VERSION).a
endif
endif

$(EH_STATIC_OBJS):
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(NOPICFLAGS) $(EHOPTFLAGS) -c $(@:.o=.cc) -o $@

$(EH_SHARED_OBJS):
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(PICFLAGS) $(EHOPTFLAGS) -c $(@:.pic.o=.cc) -o $@


codeset.cc: ../include/mico/code_set_registry.h

../include/mico/code_set_registry.h: $(REG_FILES)
	../admin/mkcsreg $(REG_FILES) > $@

# shared

ifeq ($(HAVE_SHARED), yes)
libmico$(VERSION).$(SOEXT): $(SHARED_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SHARED_OBJS) $(LDFLAGS) $(CONFLIBS)
endif
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(SHARED_OBJS) $(LDFLAGS) $(CONFLIBS)
endif
	$(RM) $@
	$(LDSO) -o libmico$(VERSION) $(SHARED_OBJS) $(PLATFORM_LIBS)
	$(IDIRCMD) ../libs
	cd ../libs; $(RM) $@; $(LN) ../orb/$@ .
endif

# static

libmico$(VERSION).a: $(STATIC_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(STATIC_OBJS) $(LDFLAGS) $(CONFLIBS)
endif
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(STATIC_OBJS) $(LDFLAGS) $(CONFLIBS)
endif
	$(AR) $(ARFLAGS) $@ $(STATIC_OBJS)
	$(RANLIB) $@
	$(IDIRCMD) ../libs
	cd ../libs; $(RM) $@; $(LN) ../orb/$@ .

# mingw dll

mico$(NODOTVERSION).dll: $(STATIC_OBJS)
	$(CXX) -shared -o $@ $(STATIC_OBJS) $(CONFLIBS)
	cd ../libs; $(RM) $@; $(LN) ../orb/$@ .

generate: orbcore-generate security-generate

orbcore-generate:
	../admin/mkcsreg $(REG_FILES) > ../include/mico/code_set_registry.h
	$(IDL) --no-poa --no-boa --name current --windows-dll mico --pseudo \
		-B.. --mico-core ../include/mico/current.idl
	mv current.h ../include/mico/current.h
	$(IDL) --name policy --windows-dll mico -B.. \
		--no-poa --no-boa --pseudo --mico-core \
		../include/mico/policy.idl
	patch < policy.cc.diffs
	$(RM) policy.*.orig
	mv policy.h ../include/mico/policy.h
	$(IDL) --name policy2 --any --windows-dll mico --no-poa --no-boa \
		-B.. --pseudo --mico-core \
		../include/mico/policy2.idl
	mv policy2.h ../include/mico/policy2.h
	$(IDL) --any --name poa --windows-dll mico --no-poa --no-boa \
		-B.. --pseudo --mico-core \
		../include/mico/poa.idl
	patch < poa.h.diffs
	patch < poa.cc.diffs
	$(RM) poa.*.orig
	mv poa.h ../include/mico/poa.h
	$(IDL) --name service_info --windows-dll mico --pseudo --mico-core \
		-B.. ../include/mico/service_info.idl
	mv service_info.h ../include/mico/service_info.h
	$(IDL) --name orb_excepts --windows-dll mico --pseudo --mico-core \
		-B.. ../include/mico/orb_excepts.idl
	mv orb_excepts.h ../include/mico/orb_excepts.h
	$(IDL) --name dynany --windows-dll mico --pseudo --mico-core --any \
		-B.. --no-poa --no-boa -I../include ../include/mico/dynany.idl
	mv dynany.h ../include/mico/dynany.h
	$(IDL) --name ioptypes --windows-dll mico --pseudo --mico-core \
		-B.. --any ../include/mico/ioptypes.idl
	mv ioptypes.h ../include/mico/ioptypes.h
	$(IDL) --name ssliop --windows-dll mico --pseudo -B.. \
		--hh-prefix mico \
		../include/mico/ssliop.idl
	mv ssliop.h ../include/mico/ssliop.h
	$(IDL) --name valuetype --windows-dll mico --pseudo --any \
		--mico-core -B.. ../include/mico/valuetype.idl
	mv valuetype.h ../include/mico/valuetype.h
	$(IDL) --name tckind --windows-dll mico --pseudo \
		--mico-core -B.. ../include/mico/tckind.idl
	mv tckind.h ../include/mico/tckind.h
	$(IDL) --name pi --windows-dll mico --mico-core --any --pseudo \
		-B.. ../include/mico/pi.idl
	mv pi.h ../include/mico/pi.h
	$(IDL) --name typecode_seq --windows-dll mico --mico-core --any \
		--pseudo -B.. ../include/mico/typecode_seq.idl
	mv typecode_seq.h ../include/mico/typecode_seq.h
	$(IDL)  --name timebase --windows-dll mico --mico-core -B.. \
		../include/mico/timebase.idl
	mv timebase.h ../include/mico/timebase.h
	$(IDL)  --name reflection --windows-dll mico --hh-prefix mico/ \
		-B.. --any ../include/mico/reflection.idl
	mv reflection.h ../include/mico/reflection.h

security-generate:
	make -C security generate

csiv2-generate:
	cd security
#	$(IDL) --name ioptypes --windows-dll mico -B.. --pseudo --mico-core \
#		--any ../include/mico/ioptypes.idl
#	mv ioptypes.h ../include/mico/ioptypes.h
	$(IDL) --windows-dll mico -B.. --no-poa --no-boa \
		--pseudo --any --mico-core ../include/mico/security/csi_base.idl
	mv csi_base.h ../include/mico/security/csi_base.h
	$(IDL) --windows-dll mico -B.. --no-poa --no-boa \
		--pseudo --any --mico-core ../include/mico/security/csi.idl
	mv csi.h ../include/mico/security/csi.h
	$(IDL) --windows-dll mico -B.. --no-poa --no-boa \
		--pseudo --any --mico-core ../include/mico/security/csiiop.idl
	mv csiiop.h ../include/mico/security/csiiop.h
	$(IDL) --windows-dll mico -B.. --no-poa --no-boa \
		--pseudo --any --mico-core ../include/mico/security/gssup.idl
	mv gssup.h ../include/mico/security/gssup.h
	$(IDL) --windows-dll mico -B.. --no-poa --no-boa \
		--pseudo --any --mico-core ../include/mico/security/csiv2.idl
	mv csiv2.h ../include/mico/security/csiv2.h
	cd ..

diffs:
	mkdir orig
	cd orig; $(IDL) --name policy --windows-dll mico --no-poa --no-boa \
	  -B../.. --pseudo --mico-core \
	  ../../include/mico/policy.idl
	$(RM) policy.cc.diffs
	diff -c orig/policy.cc policy.cc > policy.cc.diffs || true
	cd ../include/mico; diff -c ../../orb/orig/policy.h policy.h \
	  >> ../../orb/policy.cc.diffs || true
	$(RM) -r orig
	mkdir orig
	cd orig; $(IDL) --any --name poa --windows-dll mico --no-poa --no-boa \
	  -B../.. --pseudo --mico-core \
	  ../../include/mico/poa.idl
	$(RM) poa.cc.diffs
	diff -c orig/poa.cc poa.cc > poa.cc.diffs || true
	$(RM) -r orig

clean:
	$(RM) *.rpo *.ii *.ti *.a *.o *~ *.$(SOEXT) *.exe .depend
	(cd transport; $(RM) *.rpo *.ii *.ti *.a *.o *~ .depend)
	(cd os-thread; $(RM) *.rpo *.ii *.ti *.a *.o *~ .depend)
	(cd security; $(RM) *.rpo *.ii *.ti *.a *.o *~ .depend)


install:
	$(IDIRCMD) $(INSTDIR)/lib
ifeq ($(HAVE_STATIC), yes)
	$(ILIBCMD) libmico$(VERSION).a $(INSTDIR)/lib
endif
ifeq ($(HAVE_SHARED), yes)
	$(ILIBCMD) libmico$(VERSION).$(SOEXT) $(INSTDIR)/lib
endif

install-cd: install

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) $(CPPFLAGS) *.cc >> .depend
	$(MKDEPEND) $(CXXFLAGS) $(CPPFLAGS) transport/*.cc >> .depend
	$(MKDEPEND) $(CXXFLAGS) $(CPPFLAGS) os-thread/*.cc >> .depend
	$(MKDEPEND) $(CXXFLAGS) $(CPPFLAGS) security/*.cc >> .depend


