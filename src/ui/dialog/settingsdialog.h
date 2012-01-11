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
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>
#include <QtGui/QProgressDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>

#include "src/core/settings/settings.h"
#include "src/core/settings/modulesettings.h"
#include "src/core/dbghelper.h"

#include "src/module/modulemanager.h"
#include "src/module/bible/bible.h"
#include "src/module/module.h"

#include "src/ui/modulemodel.h"

#include "src/ui/dialog/moduleconfigdialog.h"
#include "src/ui/dialog/moduledownloaddialog.h"
#include "config.h"

namespace Ui
{
class SettingsDialog;
}
/**
 * SettingsDialog represents a dialog to modify the settings.
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);
    virtual ~SettingsDialog();
    int setSettings(Settings settings);
    void setCurrentTab(int tabID);
protected:
    virtual void changeEvent(QEvent *e);
signals:
    int settingsChanged(Settings settings, bool modifedModuleSettings);
private slots:
    void save();
    void addModuleFile();
    void addModuleDir();
    void removeModule();
    void editModule();
    void reset();
    void downloadModule();
    void addModules(QStringList files, QStringList names, int parentID = -1);
    void addModules(QMap<QString, QString> data);

    void importSwordModules();

    void addVirtualFolder();
private:
    Settings m_set;
    Settings m_backupSet;

    QStringList m_encodings;
    QStringList m_langCode;
    Ui::SettingsDialog *m_ui;
    bool m_modifedModuleSettings;

    void generateModuleTree();
    static QStringList scan(const QString &path, const int level);
    int quiteAddModule(const QString &path, int parentID = -1, const QString &name = "");
    void saveModule(QModelIndex parentIndex, ModuleSettings *parentSettings);
};

#endif // SETTINGSDIALOG_H
