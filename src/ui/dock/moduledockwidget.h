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
#ifndef MODULEDOCKWIDGET_H
#define MODULEDOCKWIDGET_H

#include <QDockWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QSortFilterProxyModel>
#include <QItemSelectionModel>
#include "dockwidget.h"
#include "src/ui/recursivproxymodel.h"
namespace Ui
{
class ModuleDockWidget;
}
/*!
 ModuleDockWidget represents a dock widget from which you can load modules

*/
class ModuleDockWidget : public DockWidget
{
    Q_OBJECT
private slots:
    void loadModuleData(QModelIndex index);
    void filter(QString string);
public:
    ModuleDockWidget(QWidget *parent = 0);
    ~ModuleDockWidget();
    void init();
    void loadedModule(int id);
protected:
    void changeEvent(QEvent *e);
private:
    Ui::ModuleDockWidget *ui;
    bool m_dontLoad;
    int m_moduleID;
    RecursivProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
    bool first;

};

#endif // MODULEDOCKWIDGET_H
