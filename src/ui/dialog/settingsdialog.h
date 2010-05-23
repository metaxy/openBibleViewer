/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui/QDialog>
#include "src/core/settings.h"
namespace Ui
{
class SettingsDialog;
}
/*!
 SettingsDialog represents a dialog to modife the settings

*/
class SettingsDialog : public QDialog
{
    Q_OBJECT
public slots:
    void save();
    void addModuleFile();
    void addModuleDir();
    void removeModule();
    void editModule();
    void reset();
    void saveModule(ModuleSettings);
    void downloadModule();
    void addModules(QStringList files, QStringList names);
signals:
    int settingsChanged(Settings settings);
public:
    explicit SettingsDialog(QWidget *parent = 0);
    virtual ~SettingsDialog();
    int setSettings(Settings settings);
    void setCurrentTab(int tabID);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Settings m_set;
    Settings m_backupSet;

    void generateModuleTree();
    QStringList m_encodings;
    QStringList m_langCode;
    Ui::SettingsDialog *m_ui;
};

#endif // SETTINGSDIALOG_H
