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
#ifndef QUICKJUMPDOCKWIDGET_H
#define QUICKJUMPDOCKWIDGET_H

#include <QDockWidget>
#include "dockwidget.h"
namespace Ui
{
class QuickJumpDockWidget;
}
/*!
 ModuleDockWidget represents a dock widget from which you can jump fast to certain position in the module

*/
class QuickJumpDockWidget : public DockWidget
{
    Q_OBJECT
private slots:
    void goToPos();
public:
    QuickJumpDockWidget(QWidget *parent = 0);
    ~QuickJumpDockWidget();

protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void get(QString url);
private:
    Ui::QuickJumpDockWidget *ui;
};

#endif // QUICKJUMPDOCKWIDGET_H
