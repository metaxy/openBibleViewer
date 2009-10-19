#
# spec file for package openBibleViewer (Version 0.2)
#
# Copyright 2009 Paul Walger http://paul.walger.eu
#
# This file and all modifications and additions to the pristine  
# package are under the same license as the package itself. 
%define name openBibleViewer

Summary: Bible study tool
Name: %{name}
License: GPLv3
URL: http://openbv.uucyc.name
Version: 0.2
Release: 1
Group: Amusements/Teaching/Other
Source: %{name}-%{version}.tar.bz2
Packager: Paul Walger
BuildRoot: %{_tmppath}/%{name}-%{version}-build
%if 0%{?fedora_version}
BuildRequires: qt-devel >= 4.4.3 gcc-c++
%else
BuildRequires: libqt4-devel >= 4.4.3 gcc-c++
%endif

%description
This program allows one to work with the Bible and study it
%prep
%setup
%build
%if 0%{?fedora_version}
qmake-qt4 -makefile %{name}.pro 'target.path = %{buildroot}%{_bindir}'
qmake-qt4 'target.path = %{buildroot}%{_bindir}'
%else
qmake -makefile %{name}.pro 'target.path = %{buildroot}%{_bindir}'
qmake 'target.path = %{buildroot}%{_bindir}' 
%endif

make
%install
%__mkdir -p %{buildroot}/%{_bindir} %{buildroot}/usr/share/{%{name}/{fonts,translation},applications,pixmaps} %{buildroot}/%{_docdir}/%{name}
make install DESTDIR=$RPM_BUILD_ROOT $INSTALL_TARGET
%__install -m 644 src/icons/124x124/%{name}.png %{buildroot}%{_datadir}/pixmaps/%{name}.png
%__install -m 644 %{name}.desktop %{buildroot}%{_datadir}/applications  
%__install -m 644 src/*.qm %{buildroot}/usr/share/%{name}/translation  
%if 0%{?fedora_version}
%suse_update_desktop_file %{name} Teaching
%endif
%clean
%{__rm} -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc README INSTALL LICENSE
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/%{name}
%changelog
* Sun Oct 18 2009 Paul Walger
- add .desktop file,icon and translations
* Thu Sep 17 2009 Paul Walger
- works with Mandriva and Fedora
* Wed Apr 29 2009 Paul Walger
- Update to 0.2
* Sat Mar 14 2009 Paul Walger
- Inital Spec file

