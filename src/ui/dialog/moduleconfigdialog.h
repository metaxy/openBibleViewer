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
#ifndef MODULECONFIGDIALOG_H
#define MODULECONFIGDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QFileDialog>
#include "src/core/settings/modulesettings.h"
#include "src/core/dbghelper.h"
#include "src/module/bible/zefania-bible.h"
#include "src/module/module.h"

namespace Ui
{
class ModuleConfigDialog;
}
/**
 * ModuleConfigDialog represents a dialog to change module settings
 */
class ModuleConfigDialog : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(ModuleConfigDialog)
signals:
    void save(ModuleSettings config);
public:
    explicit ModuleConfigDialog(QWidget *parent = 0);
    virtual ~ModuleConfigDialog();
    void setModule(ModuleSettings config);
protected:
    virtual void changeEvent(QEvent *e);

private slots:
    void bsave();
    void moduleTypeChanged(int id);
    void fileSelect();
private:
    Ui::ModuleConfigDialog *m_ui;
    ModuleSettings m_moduleSettings;
    QStringList m_encodings;
};

#endif // MODULECONFIGDIALOG_H
