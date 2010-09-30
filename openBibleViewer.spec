#
# spec file for package openBibleViewer (Version 0.5)
#
# Copyright 2009-2010 Paul Walger http://paul.walger.eu
#
# This file and all modifications and additions to the openBibleViewer  
# package are under the same license as the package itself. 
%define name openBibleViewer

Summary: Bible study tool
Name: %{name}
License: GPLv3
URL: http://openbv.uucyc.name
Version: 0.5
Release: 128
Group: Amusements/Teaching/Other
Source: %{name}-%{version}.tar.bz2
Packager: Paul Walger
BuildRoot: %{_tmppath}/%{name}-%{version}-build

%if 0%{?fedora_version}
BuildRequires: qt-devel >= 4.6 gcc-c++ clucene-core-devel
%endif

%if 0%{?mandriva_version}
BuildRequires: libqt4-devel >= 4.6 gcc-c++ clucene-devel
%endif

%if 0%{?suse_version}
BuildRequires: libqt4-devel libQtWebKit-devel >= 4.6 gcc-c++ clucene-core-devel clucene-core
%endif

BuildRequires: desktop-file-utils cmake

%description
This program allows one to work with the bible and study it.
%prep
%setup
%build
cmake -DCMAKE_INSTALL_PREFIX='%{_prefix}' 
make
%install

make install

%__install -m 644 src/icons/124x124/%{name}.png %{buildroot}%{_datadir}/pixmaps/%{name}.png
%__install -m 644 %{name}.desktop %{buildroot}%{_datadir}/applications


%clean
%{__rm} -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%doc README INSTALL LICENSE
%{_bindir}/%{name}

%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/%{name}
%changelog
* Sat Sep 25 2010 Paul Walger
- update to 0.5
* Sat Apr 17 2010 Paul Walger
- update to 0.4.1
* Sat Apr 10 2010 Paul Walger
- update to 0.4
* Sun Mar 14 2010 Paul Walger
- update to 0.3.1
* Sun Feb 14 2010 Paul Walger
- update to 0.3
* Sun Oct 18 2009 Paul Walger
- add .desktop file,icon and translations
* Thu Sep 17 2009 Paul Walger
- works with Mandriva and Fedora
* Wed Apr 29 2009 Paul Walger
- Update to 0.2
* Sat Mar 14 2009 Paul Walger
- Inital Spec file

