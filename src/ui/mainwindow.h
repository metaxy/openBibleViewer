/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QSettings>
#include <QtCore/QTranslator>
#include <QtGui/QToolBar>
#include "src/module/modulemanager.h"
#include "src/core/bibledisplay.h"
#include "src/core/settings.h"
#include "src/core/notes.h"
#include "src/core/session.h"
#include "src/ui/interface/interface.h"
#include "src/ui/interface/simple/simpleinterface.h"
#include "src/ui/interface/advanced/advancedinterface.h"
#include "src/ui/interface/study/studyinterface.h"
namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow, public BasicClass
{
    Q_OBJECT
signals:
    void settingsChanged(Settings oldSettings, Settings newSettings);
public slots:
    void showSettingsDialog_Module();
    void showSettingsDialog_General();
    void saveSettings(Settings set);
    void reloadInterface();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init(const QString &homeDataPath, QSettings * settingsFile);
    QSettings *m_settingsFile;
    QSettings *m_moduleCacheFile;
    QString VERSION, BUILD;
    QString m_homeDataPath;
    void setSettings(Settings settings);
    void writeSettings();
    void showSettingsDialog(int tabID);
    void loadLanguage(QString language);
    void setTranslator(QTranslator *my, QTranslator *qt);
    QTranslator *myappTranslator;
    QTranslator *qtTranslator;


protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
signals:
    void closing();
private:
    Ui::MainWindow *ui;
    Interface *m_interface;
    QList<QToolBar *> m_toolBarList;
    QMenuBar *m_menuBar;
    void loadInterface();
    void deleteInterface();

    Session *m_session;
    void loadSimpleInterface();
    void loadAdvancedInterface();
    void loadStudyInterface();
    void loadDefaultSettings();
    void loadSettings();
    void restoreSession();
    int m_reloadLang;

};

#endif // MAINWINDOW_H
