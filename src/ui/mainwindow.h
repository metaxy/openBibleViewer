/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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


#include <QtCore/QSettings>

#include <QtCore/QLibraryInfo>
#include <QtCore/QDir>
#include <QtCore/QTranslator>

#include <QtGui/QToolBar>
#include <QtGui/QMessageBox>
#include <QtGui/QMainWindow>

#include "config.h"
#include "src/core/version.h"

#include "src/core/settings/settings.h"
#include "src/core/settings/session.h"

#include "src/ui/interface/interface.h"

#include "src/core/context.h"
namespace Ui
{
class MainWindow;
}
/**
  * MainWindow is the first BasicClass.
  */
class MainWindow : public QMainWindow, public BasicClass
{
    Q_OBJECT
signals:
    void settingsChanged(Settings oldSettings, Settings newSettings, bool modifedModuleSettings);
public slots:
    void showSettingsDialog_Module();
    void showSettingsDialog_General();
    void saveSettings(Settings set, bool modifedModuleSettings);
    void reloadInterface();
public:
    MainWindow(Context *parent);
    ~MainWindow();
    void init(const QString &homeDataPath, QSettings * settingsFile, bool firstStart);
    QSettings *m_settingsFile;

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

    void loadSimpleInterface();
    void loadAdvancedInterface();
    void loadStudyInterface();
    void loadDefaultSettings();
    void loadSettings();
    void restoreSession();
    int m_reloadLang;

    Context *m_context;

};

#endif // MAINWINDOW_H
