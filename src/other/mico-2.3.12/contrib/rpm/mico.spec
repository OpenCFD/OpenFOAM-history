%define version	2.3.11
%define release	0
%define name	mico
%define libname_orig	lib%{name}
%define major	2.3.11
%define libname	%mklibname %{name} %{major}

%define with_micoccm 0

Summary:	Object Request Broker (ORB) (CORBA 2.3)
Name:		%{name}
Version:	%{version}
Release:	%{release}
License:	GPL/LGPL
Group:		System/Libraries
Source0:	http://www.mico.org/mico-%{version}.tar.gz
URL:		http://www.mico.org/
BuildRequires:	gcc >= 3.2
BuildRequires:	glibc-devel libopenssl-devel >= 0.9.7a
BuildRequires:	bison >= 1.75 flex >= 2.5.3
BuildRequires:	XFree86-devel
BuildRequires:	libgtk+1.2-devel
BuildRequires:	libqt3-devel
BuildRequires:	tcl == 8.3.3
BuildRequires:	postgresql-devel >= 7.2.2
Provides:	corba
BuildRoot:	%{_tmppath}/%{libname}-buildroot
Requires:	%{libname} = %{version}

%description
MICO (MICO Is CORBA) is a freely available and fully compliant implementation
of the CORBA 2.3 standard Object Request Broker Architecture.

%package -n %{libname}
Summary:	Object Request Broker (ORB) (CORBA 2.3)
Group:		System/Libraries
Provides:	%{libname_orig} = %{version}-%{release}

%description -n %{libname}
This package contains the libraries needed to run programs dynamically
linked with %{name}.

%package -n %{libname}-devel
Summary:	Header files and libraries needed for %{name} development
Group:		Development/C++
Requires:	%{libname} = %{version}-%{release}
Requires:	gcc >= 3.2
Requires:	glibc-devel libopenssl-devel postgresql-devel
Provides:	%{libname_orig}-devel = %{version}-%{release}

%description -n %{libname}-devel
This package includes the header files and libraries needed for
developing programs using %{name}.

%if %with_micoccm

%package -n %{name}-ccm
Summary:	CORBA Component Model (CORBA 3) for MICO
Group:		System/Libraries
Requires:	%{libname}-ccm = %{version}

%description -n %{name}-ccm
Mico-CCM is an implementation of the CORBA Component Model specified by
CORBA 3.

%package -n %{libname}-ccm
Summary:	CORBA Component Model (CORBA 3) for MICO
Group:		System/Libraries
Provides:	 %{libname_orig}-ccm = %{version}-%{release}

%description -n %{libname}-ccm
This package contains the libraries needed to run programs dynamically
linked with %{name}-ccm.

%package -n %{libname}-ccm-devel
Summary:	Header files and libraries needed for %{name}-ccm development
Group:		Development/C++
Requires:	%{libname}-ccm = %{version}-%{release}
Requires:	%{libname}-devel = %{version}-%{release}
Provides:	%{libname_orig}-ccm-devel = %{version}-%{release}

%description -n %{libname}-ccm-devel
This package includes the header files and libraries needed for
developing programs using %{name}-ccm.

%endif # %with_micoccm

%package -n %{name}-doc
Summary:	Developer documentation and examples for %{name}
Group:		Books/Other
Requires:	%{name} = %{version}

%description -n %{name}-doc
This package includes developer documentation including examples.

%package -n %{libname}-gtk
Summary:	Libraries for GTK applications with %{name}
Group:		System/Libraries
Requires:	%{libname} = %{version}-%{release}
Provides:	%{libname_orig}-gtk = %{version}-%{release}

%description -n %{libname}-gtk
This package includes libraries to support GTK applications built with %{name}. 

%package -n %{libname}-gtk-devel
Summary:	Header files and libraries needed for %{name}-gtk development
Group:		Development/C++
Requires:	%{libname}-gtk = %{version}-%{release}
Requires:	%{libname}-devel = %{version}-%{release}
Requires:	libgtk+1.2-devel
Provides:	%{libname_orig}-gtk-devel = %{version}-%{release}

%description -n %{libname}-gtk-devel
This package includes the header files and libraries needed for
developing programs using %{libname}-gtk.

%package -n %{libname}-qt
Summary:	Libraries for QT applications with %{name}
Group:		System/Libraries
Requires:	%{libname} = %{version}-%{release}
Provides:	%{libname_orig}-qt = %{version}-%{release}

%description -n %{libname}-qt
This package includes libraries to support QT applications built with %{name}. 

%package -n %{libname}-qt-devel
Summary:	Header files and libraries needed for %{name}-qt development
Group:		Development/C++
Requires:	%{libname}-qt = %{version}-%{release}
Requires:	%{libname}-devel = %{version}-%{release}
Requires:	libqt3-devel
Provides:	%{libname_orig}-qt-devel = %{version}-%{release}

%description -n %{libname}-qt-devel
This package includes the header files and libraries needed for
developing programs using %{libname}-qt.

%package -n %{libname}-tcl
Summary:	Libraries for TCL applications with %{name}
Group:		System/Libraries
Requires:	%{libname} = %{version}-%{release}
Provides:	%{libname_orig}-tcl = %{version}-%{release}

%description -n %{libname}-tcl
This package includes libraries to support TCL applications built with %{name}. 

%package -n %{libname}-tcl-devel
Summary:	Header files and libraries needed for %{name}-tcl development
Group:		Development/C++
Requires:	%{libname}-tcl = %{version}-%{release}
Requires:	%{libname}-devel = %{version}-%{release}
Provides:	%{libname_orig}-tcl-devel = %{version}-%{release}

%description -n %{libname}-tcl-devel
This package includes the header files and libraries needed for
developing programs using %{libname}-tcl.

%package -n %{libname}-x11
Summary:	Libraries for X11 applications with %{name}
Group:		System/Libraries
Requires:	%{libname} = %{version}-%{release}
Provides:	%{libname_orig}-x11 = %{version}-%{release}

%description -n %{libname}-x11
This package includes libraries to support X11 applications built with %{name}. 

%package -n %{libname}-x11-devel
Summary:	Header files and libraries needed for %{name}-x11 development
Group:		Development/C++
Requires:	%{libname}-x11 = %{version}-%{release}
Requires:	%{libname}-devel = %{version}-%{release}
Requires:	XFree86-devel
Provides:	%{libname_orig}-x11-devel = %{version}-%{release}

%description -n %{libname}-x11-devel
This package includes the header files and libraries needed for
developing programs using %{libname}-x11.

%prep 
%setup -q -n mico

%build
%if %with_micoccm
%configure --enable-threads --enable-ccm --enable-csl2 --enable-csiv2 \
	--with-x --with-gtk --with-qt=/usr/lib/qt3 --with-tcl --with-pgsql
%else
%configure --enable-threads --enable-csl2 --enable-csiv2 \
	--with-x --with-gtk --with-qt=/usr/lib/qt3 --with-tcl --with-pgsql
%endif # %with_micoccm
%make

%install
[ -d %buildroot ] && rm -rf %buildroot
mkdir -p %buildroot%{_usr}
mkdir -p %buildroot%{_datadir}
make install-cd INSTDIR=%buildroot%{_usr} SHARED_INSTDIR=%buildroot%{_datadir}

rm -rf packagedoc
mv %buildroot%{_usr}/doc/mico/examples packagedoc

rm -f %buildroot%{_libdir}/*sh.in
rm -f %buildroot%{_includedir}/*.in

find %buildroot%{_libdir}* -name '*.so' | sed "s#^%{buildroot}##" | grep -Ev '(ccm|gtk|qt|tcl|micox)' > files.libmico
find %buildroot%{_libdir}* -name '*.a' -o -name '*.sh' -o -name '*.csh' | sed "s#^%{buildroot}##" | grep -Ev '(ccm|gtk|qt|tcl|micox)' > files.libmico-devel
find %buildroot%{_includedir}* -type d | sed "s#^%{buildroot}#%dir #" | grep -v CCM >> files.libmico-devel
find %buildroot%{_includedir}* -type f | sed "s#^%{buildroot}##" | grep -v CCM >> files.libmico-devel

%if %with_micoccm

find %buildroot%{_libdir}* -name '*.so' | sed "s#^%{buildroot}##" | grep ccm > files.libmico-ccm
find %buildroot%{_libdir}* -name '*.a' -o -name '*.sh' -o -name '*.csh' | sed "s#^%{buildroot}##" | grep ccm > files.libmico-ccm-devel
find %buildroot%{_includedir}* -type f | sed "s#^%{buildroot}##" | grep CCM >> files.libmico-ccm-devel

%else
rm -f %buildroot%{_includedir}/mico/CCM*
%endif # %with_micoccm

find %buildroot%{_libdir}* -name '*.so' | sed "s#^%{buildroot}##" | grep gtk > files.libmico-gtk
find %buildroot%{_libdir}* -name '*.a' -o -name '*.sh' -o -name '*.csh' | sed "s#^%{buildroot}##" | grep gtk > files.libmico-gtk-devel

find %buildroot%{_libdir}* -name '*.so' | sed "s#^%{buildroot}##" | grep qt > files.libmico-qt
find %buildroot%{_libdir}* -name '*.a' -o -name '*.sh' -o -name '*.csh' | sed "s#^%{buildroot}##" | grep qt > files.libmico-qt-devel

find %buildroot%{_libdir}* -name '*.so' | sed "s#^%{buildroot}##" | grep tcl > files.libmico-tcl
find %buildroot%{_libdir}* -name '*.a' -o -name '*.sh' -o -name '*.csh' | sed "s#^%{buildroot}##" | grep tcl > files.libmico-tcl-devel

find %buildroot%{_libdir}* -name '*.so' | sed "s#^%{buildroot}##" | grep micox > files.libmico-x11
find %buildroot%{_libdir}* -name '*.a' -o -name '*.sh' -o -name '*.csh' | sed "s#^%{buildroot}##" | grep micox > files.libmico-x11-devel

%clean
[ -z %buildroot ] || rm -rf %buildroot

%post -n %{libname} -p /sbin/ldconfig
%postun -n %{libname} -p /sbin/ldconfig

%if %with_micoccm
%post -n %{libname}-ccm -p /sbin/ldconfig
%postun -n %{libname}-ccm -p /sbin/ldconfig
%endif # %with_micoccm

%post -n %{libname}-gtk -p /sbin/ldconfig
%postun -n %{libname}-gtk -p /sbin/ldconfig

%post -n %{libname}-qt -p /sbin/ldconfig
%postun -n %{libname}-qt -p /sbin/ldconfig

%post -n %{libname}-tcl -p /sbin/ldconfig
%postun -n %{libname}-tcl -p /sbin/ldconfig

%post -n %{libname}-x11 -p /sbin/ldconfig
%postun -n %{libname}-x11 -p /sbin/ldconfig

%files
%defattr (-,root,root)
%doc BUGS CHANGES* CONVERT LICENSE
%{_bindir}/containmentd
%{_bindir}/eventd
%{_bindir}/idl
%{_bindir}/imr
%{_bindir}/iordump
%{_bindir}/ird
%{_bindir}/micod
%{_bindir}/mico-shld
%{_bindir}/noded
%{_bindir}/nsadmin
%{_bindir}/nsd
%{_bindir}/propertyd
%{_bindir}/proxyd
%{_bindir}/randomd
%{_bindir}/referenced
%{_bindir}/timed
%{_bindir}/traderd
%{_bindir}/traversald
%attr(644,root,man) %{_mandir}/man1/*
%attr(644,root,man) %{_mandir}/man5/*
%attr(644,root,man) %{_mandir}/man8/*

%files -n %{libname} -f files.libmico
%defattr (-,root,root)

%files -n %{libname}-devel -f files.libmico-devel
%defattr(-,root,root)
%doc README* 
%{_bindir}/mico-ar
%{_bindir}/mico-c++
%{_bindir}/mico-cpp
%{_bindir}/mico-config
%{_bindir}/mico-ld
%{_bindir}/mico-shc++
%{_bindir}/mkdepend

%if %with_micoccm

%files -n %{name}-ccm
%defattr(-,root,root)
%{_bindir}/ccmload
%{_bindir}/componentserver
%{_bindir}/mico-ccmd

%files -n %{libname}-ccm -f files.libmico-ccm
%defattr(-,root,root)

%files -n %{libname}-ccm-devel -f files.libmico-ccm-devel
%defattr(-,root,root)
%{_bindir}/mico-ccm

%endif # %with_micoccm

%files -n %{libname}-gtk -f files.libmico-gtk
%defattr(-,root,root)

%files -n %{libname}-gtk-devel -f files.libmico-gtk-devel
%defattr(-,root,root)

%files -n %{libname}-qt -f files.libmico-qt
%defattr(-,root,root)

%files -n %{libname}-qt-devel -f files.libmico-qt-devel
%defattr(-,root,root)

%files -n %{libname}-tcl -f files.libmico-tcl
%defattr(-,root,root)

%files -n %{libname}-tcl-devel -f files.libmico-tcl-devel
%defattr(-,root,root)

%files -n %{libname}-x11 -f files.libmico-x11
%defattr(-,root,root)

%files -n %{libname}-x11-devel -f files.libmico-x11-devel
%defattr(-,root,root)

%files -n %{name}-doc
%defattr(-,root,root)
%doc packagedoc/*

%changelog
* Thu Sep 25 2003 Dirk O. Siebnich <dok@dok-net.net> 2.3.11-0
- simplified and submitted for inclusion in contrib/rpm of Mico sources.
