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
#include "anyoption.h"

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
    AnyOption *opt = new AnyOption();
    opt->addUsage( "" );
    opt->addUsage( "Usage: " );
    opt->addUsage( "" );
    opt->addUsage( " -h  --help                 Prints this help " );
    opt->addUsage( " --configPath 	        Path to config files " );
    opt->addUsage( "" );
    opt->setFlag(  "help", 'h' );
    opt->setOption(  "configPath");
    opt->processCommandArgs( argc, argv );

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


    if( opt->getValue( "configPath" ) != NULL ) {
        homeDataPath = QString::fromLocal8Bit(opt->getValue( "configPath" ));
    } else {
        #if !defined(Q_WS_WIN)
            homeDataPath = QFSFileEngine::homePath() + "/.openbible/";
        #else
            //a protable version is needed only for windows
            #ifdef OBV_PORTABLE_VERSION
                homeDataPath = QApplication::applicationDirPath() + "/";
            #else
                homeDataPath = QDir(QString(getenv("APPDATA"))).absolutePath() + "/openbible/";
            #endif
        #endif

    }

    settings = new QSettings(homeDataPath + "openBibleViewer.ini", QSettings::IniFormat);
    myDebug() << homeDataPath;
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
    delete opt;

    MainWindow w;
    w.setTranslator(&myappTranslator, &qtTranslator);
    w.init(homeDataPath, settings);

    w.show();
    a.exec();

    return 0;
}
