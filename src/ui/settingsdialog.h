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
#include "../core/config.h"
namespace Ui
{
class settingsDialog;
}

class settingsDialog : public QDialog
{
    Q_OBJECT
public slots:
    int bsave();
    void addModuleFile();
    void addModuleDir();
    void removeModule();
    void editModule();
    void reset();
    void saveModule(struct moduleConfig);
    void downloadModule();
    void addModules(QStringList files, QStringList names);
signals:
    int save(struct settings_s settings);
public:
    explicit settingsDialog(QWidget *parent = 0);
    virtual ~settingsDialog();
    int setSettings(struct settings_s settings);

protected:
    virtual void changeEvent(QEvent *e);

private:
    struct settings_s m_set;
    struct settings_s m_backupSet;
    void generateModuleTree();
    QStringList m_encodings;
    QStringList m_langCode;
    Ui::settingsDialog *m_ui;
};

#endif // SETTINGSDIALOG_H
