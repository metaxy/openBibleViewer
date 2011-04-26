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
Version: 0.5.82
Release: 5
Group: Amusements/Teaching/Other
Source: %{name}-%{version}.tar.bz2
BuildRoot: %{_tmppath}/%{name}-%{version}-build

%if 0%{?fedora_version}

%if 0%{?fedora_version} < 14
BuildRequires: qt-devel >= 4.6 gcc-c++ clucene-core-devel zlib
%endif

%if 0%{?fedora_version} >= 14
BuildRequires: qt-devel >= 4.6 gcc-c++ clucene-core-devel zlib qt-webkit-devel >= 4.6
%endif

%endif

%if 0%{?mandriva_version}
BuildRequires: libqt4-devel >= 4.6 gcc-c++ clucene-devel libz
%endif

%if 0%{?suse_version}
BuildRequires: libqt4-devel libQtWebKit-devel >= 4.6 gcc-c++ clucene-core-devel clucene-core libz
%endif

BuildRequires: desktop-file-utils cmake

%description
This program allows one to work with the bible and study it.
%prep
%setup -q

%build
cmake -DCMAKE_INSTALL_PREFIX="%{_prefix}" -DCMAKE_BUILD_TYPE='Release'
make
%install

make DESTDIR=%{buildroot} install

%clean
%{__rm} -rf %{buildroot}

%files
%defattr(-,root,root,-)
%doc README INSTALL LICENSE
%{_bindir}/%{name}

%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png

%changelog
* Mon Nov 15 2010 Paul Walger
- update to 0.5.4
* Mon Nov 8 2010 Paul Walger
- update to 0.5.3
* Sat Oct 26 2010 Paul Walger
- update to 0.5.2
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

