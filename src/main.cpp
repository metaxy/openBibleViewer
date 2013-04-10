/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#include <QApplication>
#include <QTranslator>
#include <QSettings>
#include <QLocale>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QLibraryInfo>
#include <QFSFileEngine>
#include <QDir>
#include <stdlib.h>
#include "src/core/dbghelper.h"
#include "src/ui/qtwin.h"
#ifdef Q_WS_WIN
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>
#include <QWindowsVistaStyle>
#include <QCleanlooksStyle>
#endif

#include "src/ui/mainwindow.h"
#include "src/ui/context.h"
#include "config.h"
#include "anyoption.h"

#ifdef Q_WS_WIN
PCHAR*
    CommandLineToArgvA(
        PCHAR CmdLine,
        int* _argc
        )
    {
        PCHAR* argv;
        PCHAR  _argv;
        ULONG   len;
        ULONG   argc;
        CHAR   a;
        ULONG   i, j;

        BOOLEAN  in_QM;
        BOOLEAN  in_TEXT;
        BOOLEAN  in_SPACE;

        len = strlen(CmdLine);
        i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

        argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
            i + (len+2)*sizeof(CHAR));

        _argv = (PCHAR)(((PUCHAR)argv)+i);

        argc = 0;
        argv[argc] = _argv;
        in_QM = FALSE;
        in_TEXT = FALSE;
        in_SPACE = TRUE;
        i = 0;
        j = 0;

        while( a = CmdLine[i] ) {
            if(in_QM) {
                if(a == '\"') {
                    in_QM = FALSE;
                } else {
                    _argv[j] = a;
                    j++;
                }
            } else {
                switch(a) {
                case '\"':
                    in_QM = TRUE;
                    in_TEXT = TRUE;
                    if(in_SPACE) {
                        argv[argc] = _argv+j;
                        argc++;
                    }
                    in_SPACE = FALSE;
                    break;
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    if(in_TEXT) {
                        _argv[j] = '\0';
                        j++;
                    }
                    in_TEXT = FALSE;
                    in_SPACE = TRUE;
                    break;
                default:
                    in_TEXT = TRUE;
                    if(in_SPACE) {
                        argv[argc] = _argv+j;
                        argc++;
                    }
                    _argv[j] = a;
                    j++;
                    in_SPACE = FALSE;
                    break;
                }
            }
            i++;
        }
        _argv[j] = '\0';
        argv[argc] = NULL;

        (*_argc) = argc;
        return argv;
    }
	
#endif


#ifdef Q_WS_WIN
int WINAPI WinMain(HINSTANCE v1, HINSTANCE v2, LPSTR v3, int v4)
{
    int argc;
    PCHAR *argv;
    argv = CommandLineToArgvA(v3, &argc);
#else
int main(int argc, char *argv[])
{
#endif
    QApplication a(argc, argv);
    AnyOption *opt = new AnyOption();
    const QString s = "openBibleViewer " + QString(OBV_VERSION_NUMBER);
    opt->addUsage(s.toStdString().c_str());
    opt->addUsage("Usage: ");
    opt->addUsage("");
    opt->addUsage(" -h  --help              Prints this help ");
    opt->addUsage(" --configPath 	        Path to config files ");
    opt->addUsage("");
    opt->setFlag("help", 'h');
    opt->setOption("configPath");
    opt->processCommandArgs(argc, argv);
    if(opt->getValue("help") != NULL) {
        opt->printUsage();
        return 0;
    }

#ifdef Q_WS_WIN
    bool win7 = false;

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

    if(osvi.dwMajorVersion >= 6.1) {
        win7 = true;
    }

    if (QtWin::isCompositionEnabled()) {
        myDebug() << "add css files " << win7;
        if(win7) {
            QFile file(":/data/style/win7_transparent.css");
            if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                a.setStyleSheet(in.readAll());
            }
        }

        QFile file2("stylesheet.css");
        if(file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in2(&file2);
            a.setStyleSheet(in2.readAll());
        }
    }
#endif

    QSettings *settings;
    QString homeDataPath;


    if(opt->getValue("configPath") != NULL) {
        homeDataPath = QString::fromLocal8Bit(opt->getValue("configPath"));
    } else {
    #ifdef Q_WS_WIN
        //a protable version is needed only for windows
        #ifdef OBV_PORTABLE_VERSION
                homeDataPath = QApplication::applicationDirPath() + "/";
        #else
                homeDataPath = QDir(QString(getenv("APPDATA"))).absolutePath() + "/openbible/";
        #endif
    #else
         homeDataPath = QFSFileEngine::homePath() + "/.openbible/";
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
        ModuleTools::removeDir(homeDataPath + "cache/");
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
    bool loaded = myappTranslator.load(":/data/obv_" + lang);

    if(!loaded)
        myappTranslator.load(lang);

    a.installTranslator(&myappTranslator);
    delete opt;

    Context *context = new Context(&a);
    context->setHomePath(homeDataPath);
    context->setSettings(settings);
    context->setTranslator(&myappTranslator, &qtTranslator);
    context->init();
    context->showWindow();

    a.exec();

    delete context;
    return 0;
}
