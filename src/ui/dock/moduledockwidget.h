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
#ifndef MODULEDOCKWIDGET_H
#define MODULEDOCKWIDGET_H

#include <QDockWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QSortFilterProxyModel>
#include <QItemSelectionModel>
#include "dockwidget.h"
#include "moduleproxymodel.h"
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
    // bool eventFilter(QObject *obj, QEvent *event);
signals:
    void get(QString);
private:
    Ui::ModuleDockWidget *ui;
    bool m_dontLoad;
    int m_moduleID;
    ModuleProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
    bool first;
    //  void iterateTreeWidget(QTreeWidgetItem *parent = 0);
//   void iterateTreeWidgetHide(QTreeWidgetItem *parent = 0);

};

#endif // MODULEDOCKWIDGET_H
