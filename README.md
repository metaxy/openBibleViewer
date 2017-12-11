
General introduction:
====================
openBibleViewer is a bible study application based on C++11, Qt5 and CLucene >= 2.3.3.4.

[![Build Status](https://travis-ci.org/metaxy/openBibleViewer.svg?branch=master)](https://travis-ci.org/metaxy/openBibleViewer)
[![Build status](https://ci.appveyor.com/api/projects/status/3h89o1atjc6ndoh3?svg=true)](https://ci.appveyor.com/project/metaxy/openbibleviewer)

If you found a bug, please report it on <https://github.com/metaxy/openBibleViewer/issues>.

Read the file INSTALL how to install and compile this version of openBibleViewer.

Screenshoots:
===============
![Simple Interace](https://metaxy.github.io/openBibleViewer/images/german.png)

![German Greek Interface](https://metaxy.github.io/openBibleViewer/images/greek-german.png)

![Greek Interface](https://metaxy.github.io/openBibleViewer/images/greek.png)

![Windows Version](https://metaxy.github.io/openBibleViewer/images/windows.png)

Features:
=================
+ Quick startup time (less then 2 seconds)
+ Simple interface
+ Strong numbers and Robinsons Morphological Analysis Codes available
+ Compare different translations
+ Blazing fast fulltext search
+ Make notes to every verse
+ Marking verses in different colors
+ Make bookmarks
+ Print documents
+ Export as plain text and as HTML
+ Portable version for Windows
+ 180 ZenfaniaXML Modules available
+ 200 BibleQuote Modules available


File Formats:
================
openBibleViewer can read files in the following formats:
+ [Zefania XML](http://sourceforge.net/projects/zefania-sharp/files/)
+ [BibleQuote](http://jesuschrist.ru/software/)
+ [Sword](http://www.crosswire.org/sword/index.jsp)
+ [TheWord](http://www.theword.net/)
+ PDF
+ RTF
+ Plaintext
+ [ESword](http://www.e-sword.net/)
+ [MySword](http://www.mysword.info/)



# Installation

## Linux
### openSUSE
[Installation Guide](https://software.opensuse.org/download.html?project=home%3Ametaxy:stable&package=openBibleViewer)

Or direkt: 
 + [openSUSE Tumbleweed](https://software.opensuse.org/ymp/home:metaxy:stable/openSUSE_Tumbleweed/openBibleViewer.ymp)
 + [openSUSE Leap 42.3](https://software.opensuse.org/ymp/home:metaxy:stable/openSUSE_Leap_42.3/openBibleViewer.ymp)
 + [openSUSE Leap 42.2](https://software.opensuse.org/ymp/home:metaxy:stable/openSUSE_Leap_42.2/openBibleViewer.ymp)
 + [openSUSE Leap 42.1](https://software.opensuse.org/ymp/home:metaxy:stable/openSUSE_Leap_42.1/openBibleViewer.ymp)
 + [openSUSE Factory](https://software.opensuse.org/ymp/home:metaxy:stable/openSUSE_Factory/openBibleViewer.ymp)
### Debian
[Installation Guide](https://software.opensuse.org/download.html?project=home%3Ametaxy:stable&package=openbibleviewer)

Debian 9.0

```
echo 'deb http://download.opensuse.org/repositories/home:/metaxy:/stable/Debian_9.0/ /' > /etc/apt/sources.list.d/openbibleviewer.list 
apt-get update
apt-get install openbibleviewer
```

Debian 8.0
```bash
echo 'deb http://download.opensuse.org/repositories/home:/metaxy:/stable/Debian_8.0/ /' > /etc/apt/sources.list.d/openbibleviewer.list 
apt-get update
apt-get install openbibleviewer    
```

### Ubuntu, Kubuntu, Xubuntu
[Installation Guide](https://software.opensuse.org/download.html?project=home%3Ametaxy:stable&package=openbibleviewer)

Which Version do i have? Run the following command and look out for the release line.
```
lsb_release -a
```

Ubuntu 17.10
```bash
sudo sh -c "echo 'deb http://download.opensuse.org/repositories/home:/metaxy:/stable/xUbuntu_17.10/ /' > /etc/apt/sources.list.d/openbibleviewer.list"
wget -nv https://download.opensuse.org/repositories/home:metaxy:stable/xUbuntu_17.10/Release.key -O Release.key
sudo apt-key add - < Release.key
sudo apt-get update
sudo apt-get install openbibleviewer
```

Uuntu 17.04
```bash
sudo sh -c "echo 'deb http://download.opensuse.org/repositories/home:/metaxy:/stable/xUbuntu_17.04/ /' > /etc/apt/sources.list.d/openbibleviewer.list"
wget -nv https://download.opensuse.org/repositories/home:metaxy:stable/xUbuntu_17.04/Release.key -O Release.key
sudo apt-key add - < Release.key
sudo apt-get update
sudo apt-get install openbibleviewer
```
 
Uuntu 16.10
```bash
sudo sh -c "echo 'deb http://download.opensuse.org/repositories/home:/metaxy:/stable/xUbuntu_16.10/ /' > /etc/apt/sources.list.d/openbibleviewer.list"
wget -nv https://download.opensuse.org/repositories/home:metaxy:stable/xUbuntu_16.10/Release.key -O Release.key
sudo apt-key add - < Release.key
sudo apt-get update
sudo apt-get install openbibleviewer
```

Uuntu 16.04
```bash
sudo sh -c "echo 'deb http://download.opensuse.org/repositories/home:/metaxy:/stable/xUbuntu_16.04/ /' > /etc/apt/sources.list.d/openbibleviewer.list"
wget -nv https://download.opensuse.org/repositories/home:metaxy:stable/xUbuntu_16.04/Release.key -O Release.key
sudo apt-key add - < Release.key
sudo apt-get update
sudo apt-get install openbibleviewer
```

    
Uuntu 14.04
```bash
sudo sh -c "echo 'deb http://download.opensuse.org/repositories/home:/metaxy:/stable/xUbuntu_14.04/ /' > /etc/apt/sources.list.d/openbibleviewer.list"
wget -nv https://download.opensuse.org/repositories/home:metaxy:stable/xUbuntu_14.04/Release.key -O Release.key
sudo apt-key add - < Release.key
sudo apt-get update
sudo apt-get install openbibleviewer
```

### Fedora
[Installation Guide](https://software.opensuse.org/download.html?project=home%3Ametaxy:stable&package=openBibleViewer)

Or direct:

For Fedora Raehide:
```bash
sudo dnf config-manager --add-repo https://download.opensuse.org/repositories/home:metaxy:stable/Fedora_Rawhide/home:metaxy:stable.repo
sudo dnf install openBibleViewer
```

For Fedora 27
```bash
dnf config-manager --add-repo https://download.opensuse.org/repositories/home:metaxy:stable/Fedora_27/home:metaxy:stable.repo
dnf install openBibleViewer
```
 
## macOS
Currently the builds for macOS are broken. See Issue [#27](https://github.com/metaxy/openBibleViewer/issues/27)
You can download openBibleViewer for macOS [here](https://bintray.com/metaxy/openBibleViewer/osx/view#files/bin)

## Windows
Currently the builds for Windows are broken. See Issue [#30](https://github.com/metaxy/openBibleViewer/issues/30)


## Compile on your own
See [INSTALL](https://github.com/metaxy/openBibleViewer/blob/master/INSTALL) for a compile guide




