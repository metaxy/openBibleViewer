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
#ifndef STRONGDOCKWIDGET_H
#define STRONGDOCKWIDGET_H

#include <QtGui/QDockWidget>
#include <QtCore/QUrl>
#include "dockwidget.h"
#include "src/module/strong.h"
namespace Ui
{
class StrongDockWidget;
}
/*!
 StrongDockWidget represents a dock widget to show a strong.
*/
class StrongDockWidget : public DockWidget
{
    Q_OBJECT
private slots:
    void loadModule(int id);
    void search();
    void get_(QUrl url);
public:
    StrongDockWidget(QWidget *parent = 0);
    ~StrongDockWidget();
    void init();
    void showStrong(QString id);
protected:
    void changeEvent(QEvent *e);
signals:
    void get(QUrl);
private:
    Ui::StrongDockWidget *ui;
    Strong m_strong;
    QStringList strongModuleTitle;
    QList<int> strongModuleID;
};

#endif // STRONGDOCKWIDGET_H
