/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
/****************************************************************************
  Icons from: http://www.oxygen-icons.org/
*****************************************************************************/
#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QSettings>
#include <QtCore/QLocale>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QLibraryInfo>
#include <QtCore/QFSFileEngine>
#include <QtCore/QDir>
#include <stdlib.h>
#include "src/core/dbghelper.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#include <stdio.h>
#include <QWindowsVistaStyle>
#include <QCleanlooksStyle>
#endif

#include "src/ui/mainwindow.h"
#include "config.h"

#ifdef BUILD_WITH_SWORD
/*#include <swmgr.h>
#include <swmodule.h>
#include <markupfiltmgr.h>
#include <versekey.h>
#include <listkey.h>
using sword::SWMgr;
using sword::VerseKey;
using sword::ListKey;
using sword::SWModule;
using sword::SW_POSITION;
using sword::FMT_PLAIN;
using sword::MarkupFilterMgr;
using namespace::sword;*/
#endif

bool removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if(dir.exists(dirName)) {
        foreach(const QFileInfo & info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if(info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            } else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if(!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef BUILD_WITH_SWORD

#endif

#ifdef Q_OS_WIN32
    //todo: would it be better if we use QSysInfo::windowsVersion() instead?
    //get the windows version
    OSVERSIONINFO osvi;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);
    //6 is vista and 6.1 windows 7
    if(osvi.dwMajorVersion >= 6)
        a.setStyle(new QWindowsVistaStyle);
    else
        a.setStyle(new QCleanlooksStyle);
#endif

    QSettings *settings;
    QString homeDataPath;

#ifdef Q_WS_MAC
    homeDataPath = QFSFileEngine::homePath() + "/.openbible/";
    settings = new QSettings(homeDataPath + "openBibleViewer.ini", QSettings::IniFormat);
#endif

#ifdef Q_WS_X11
    homeDataPath = QFSFileEngine::homePath() + "/.openbible/";
    settings = new QSettings(homeDataPath + "openBibleViewer.ini", QSettings::IniFormat);
#endif

#ifdef Q_WS_WIN
    //a protable version is needed only for windows
#ifdef _PORTABLE_VERSION
    homeDataPath = QApplication::applicationDirPath() + "/";
    settings = new QSettings(homeDataPath + "openBibleViewer.ini", QSettings::IniFormat);
#else
    homeDataPath = QDir(QString(getenv("APPDATA"))).absolutePath() + "/openbible/";
    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "openBible", "openBibleViewer");
#endif

#endif

#if !defined(Q_WS_MAC) && !defined(Q_WS_X11)  && !defined(Q_WS_WIN) //all other os
    homeDataPath = QFSFileEngine::homePath() + "/.openbible/";
    settings = new QSettings(homeDataPath + "openBibleViewer.ini", QSettings::IniFormat);
#endif

    QDir dir(homeDataPath);
    if(!dir.exists(homeDataPath)) {
        dir.mkpath(homeDataPath);
    }
    //remove cache dir if update from 0.4 or 0.3
    const QString v = settings->value("general/version", "0.4").toString();
    if(v.startsWith("0.4")) {
        removeDir(homeDataPath + "cache/");
    }

    QString lang = settings->value("general/language", QLocale::system().name()).toString();
    const QString av(_AV_LANG);
    const QStringList avLang = av.split(";");
    //myDebug() << avLang;
    if(avLang.lastIndexOf(lang) == -1) {
        lang = lang.remove(lang.lastIndexOf("_"), lang.size());
        if(avLang.lastIndexOf(lang) == -1) {
            lang = avLang.at(0);
        }
    }

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + lang, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    //myDebug() << lang;
    bool loaded = myappTranslator.load(":/data/obv_" + lang);
    //myDebug() << loaded;
    if(!loaded)
        myappTranslator.load(lang);

    a.installTranslator(&myappTranslator);

    MainWindow w;
    w.setTranslator(&myappTranslator, &qtTranslator);
    w.init(homeDataPath, settings);

    w.show();
    a.exec();
    return 0;
}
