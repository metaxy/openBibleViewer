/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger <metaxy@walger.name>
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

#include "src/ui/mainwindow.h"
bool removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if(dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
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

//CONFIG
//#define _PORTABLE_VERSION
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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
#ifdef _PORTABLE_VERSION
    homeDataPath = QApplication::applicationDirPath() + "/";
    settings = new QSettings(homeDataPath + "openBibleViewer.ini", QSettings::IniFormat);
#else
    homeDataPath = QDir(QString(getenv("APPDATA"))).absolutePath() + "/openbible/";
    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "openBible", "openBibleViewer");
#endif

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
    QStringList avLang;
    avLang <<  "en" << "de" << "ru" << "cs";
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
    myappTranslator.load(":/data/obv_" + lang);
    a.installTranslator(&myappTranslator);

    MainWindow w;
    w.setTranslator(&myappTranslator, &qtTranslator);
    w.init(homeDataPath, settings);

    w.show();
    a.exec();
    return 0;
}
