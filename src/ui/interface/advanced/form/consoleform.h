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
#ifndef CONSOLEFORM_H
#define CONSOLEFORM_H

#include <QWidget>
#include "src/ui/interface/advanced/form/webviewform.h"

namespace Ui {
class ConsoleForm;
}


class ConsoleForm : public WebViewForm
{
    Q_OBJECT
    
public:
    explicit ConsoleForm(QWidget *parent = 0);
    ~ConsoleForm();

    void init();
    void restore(const QString &key);
    void save();

    ModuleID moduleID();

    Form::FormType type() const;

private slots:
    void attach();
    void showContextMenu(QContextMenuEvent* ev);
private:
    Ui::ConsoleForm *ui;
};

#endif // CONSOLEFORM_H
