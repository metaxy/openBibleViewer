/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger <metaxy@walger.name>
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
  Thx at http://www.oxygen-icons.org/ for the icons
*****************************************************************************/
#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QSettings>
#include <QtCore/QLocale>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QLibraryInfo>
#include <QtCore/QtDebug>
#include <QtCore/QFSFileEngine>
#include <QtCore/QDir>
#include <stdlib.h>

#include "ui/main/mainwindow.h"

//CONFIG
//#define _PORTABLE_VERSION
//#define KOXML_USE_QDOM //WARNING QDOM USE VERY MUCH RAM

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
#else
    homeDataPath = QDir(QString(getenv("APPDATA"))).absolutePath() + "/openbible/";
#endif
    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                             "openBible", "openBibleViewer");
#endif

#ifdef Q_WS_WIN
    QString lang = settings->value("general/language", "en").toString();
#else
    QString lang = settings->value("general/language", QLocale::system().name()).toString();
#endif
    QStringList avLang;
    avLang <<  "en" << "de" << "ru";
    qDebug() << " avLang = " << avLang << " lang = " << lang;
    if (avLang.lastIndexOf(lang) == -1) {
        lang = lang.remove(lang.lastIndexOf("_"), lang.size());
        if (avLang.lastIndexOf(lang) == -1) {
            lang = avLang.at(0);
        }
    }

    QString themePath = settings->value("theme/path", homeDataPath + "stylesheet.css").toString();
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + lang, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    qDebug() << "main::main lang = " << lang;

    myappTranslator.load(":/data/obv_" + lang);

    a.installTranslator(&myappTranslator);

    MainWindow w;
    w.init(homeDataPath);
    w.setTranslator(&myappTranslator, &qtTranslator);
    w.loadModules();


    /*  QFile file(themePath);
      if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
          QTextStream in(&file);
          QString out;
          while (!in.atEnd()) {
              QString line = in.readLine();
              out += line;
          }
          w.setStyleSheet(out);//load from file
          qDebug() << "main::main() load stylesheet fileName = " << file.fileName();
      }*/
    w.show();
    return a.exec();
}
