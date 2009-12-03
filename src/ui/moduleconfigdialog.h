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
#ifndef MODULECONFIGDIALOG_H
#define MODULECONFIGDIALOG_H

#include <QtGui/QDialog>
#include "../core/moduleconfig.h"
namespace Ui
{
class moduleConfigDialog;
}

class moduleConfigDialog : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(moduleConfigDialog)
public:
    explicit moduleConfigDialog(QWidget *parent = 0);
    virtual ~moduleConfigDialog();
    void setModule(struct moduleConfig config);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::moduleConfigDialog *m_ui;
    struct moduleConfig c;
    QStringList encodings;
signals:
    void save(struct moduleConfig config);
private slots:
    void bsave();
    void moduleTypeChanged(int id);
    void fileSelect();
};

#endif // MODULECONFIGDIALOG_H
