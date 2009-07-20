#
# spec file for package openBibleViewer (Version 0.2b1)
#
# Copyright 2009 Paul Walger http://paul.walger.eu
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.
%define name    openBibleViewer
%define is_mandrake %(test -e /etc/mandrake-release && echo 1 || echo 0)
%define is_suse %(test -e /etc/SuSE-release && echo 1 || echo 0)
%define is_fedora %(test -e /etc/fedora-release && echo 1 || echo 0)
%define qmake qmake
%define lrelease lrelease

%if %is_fedora
%define distr %(cat /etc/fedora-release)
%define qmake qmake-qt4
%define lrelease lrelease-qt4
%endif
%if %is_suse
%define distr %(head -1 /etc/SuSE-release)
%endif
%if %is_mandrake
%define distr %(cat /etc/mandrake-release)
%endif

Summary: Tool to study the Bible
Name: %{name}
License: GPLv3
URL: http://openbv.uucyc.name
Version: 0.2b1
Release: 1
Group: Amusements/Teaching/Other
Source: %{name}-%{version}.tar.bz2
Packager: Paul Walger
Distribution: %{distr}
BuildRoot: %{_tmppath}/%{name}-%{version}-build
BuildRequires: libqt4-devel >= 4.4.3
%description
This program allows one to work with the Bible and study it
%prep
%setup
%build
qmake -makefile %{name}.pro 'target.path = %{_tmppath}/%{name}-%{version}-build%{_bindir}'
qmake 'target.path = %{_tmppath}/%{name}-%{version}-build%{_bindir}'
make
%install
make install DESTDIR=$RPM_BUILD_ROOT $INSTALL_TARGET
%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc README INSTALL
%{_bindir}/%{name}
%changelog
* Wed Apr 29 2009 Paul Walger
- Update to 0.2
* Sat Mar 14 2009 Paul Walger
- Inital SPec file

