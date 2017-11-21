#
# spec file for package openBibleViewer (Version 0.9)
#
# Copyright 2009-2017 Paul Walger <metaxy@walger.name>
#
# This file and all modifications and additions to the openBibleViewer  
# package are under the same license as the package itself. 
%define name openBibleViewer

Summary: Bible study tool
Name: %{name}
License: GPLv3
URL: https://github.com/metaxy/openBibleViewer
Version: 0.9.0
Release: 900
Group: Amusements/Teaching/Other
Source: %{name}-%{version}.tar.bz2
BuildRoot: %{_tmppath}/%{name}-%{version}-build
# see http://download.opensuse.org/distribution/leap/42.3/repo/oss/suse/x86_64/
# see http://download.opensuse.org/distribution/leap/42.2/repo/oss/suse/x86_64/
# see http://download.opensuse.org/distribution/openSUSE-current/repo/oss/suse/x86_64/

BuildRequires: libqt5-qtbase-devel >= 5.6 libqt5-qtscript-devel >= 5.6 clucene-core-devel >= 2.3.3.4
BuildRequires: desktop-file-utils cmake >= 3.3 gcc-c++ unzip 

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
%doc README LICENSE
%{_bindir}/%{name}

%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png

%changelog
* Mon Nov 20 2017 Paul Walger
- update to qt5
* Thu Jul 21 2015 Paul Walger
- update to 0.8.1
- fix broken downloads
* Sat Dec 31 2011 Paul Walger
- update to 0.7.0
* Fri Oct 23 2011 Paul Walger
- update to 0.6.0
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

