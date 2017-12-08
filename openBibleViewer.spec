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

BuildRequires: clucene-core-devel >= 2.3.3.4
BuildRequires: desktop-file-utils cmake >= 3.3 gcc-c++ unzip zlib-devel

%if 0%{?is_opensuse}
BuildRequires: libqt5-qtbase-devel >= 5.5 libqt5-qtscript-devel >= 5.5 libQt5WebKitWidgets-devel >= 5.5 
# BuildRequires: libqt5-qtwebengine-devel >= 5.5
%endif

%if 0%{?fedora}
BuildRequires: qt5 qt5-qtwebkit-devel qt5-qtscript-devel >= 5.5
# BuildRequires: qt5-qtwebengine-devel >= 5.5
%endif


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
%doc README.md LICENSE
%{_bindir}/%{name}

%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png

%changelog
* Mon Nov 20 2017 Paul Walger
- update to qt5

