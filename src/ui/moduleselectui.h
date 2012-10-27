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
#ifndef MODULESELECTUI_H
#define MODULESELECTUI_H

#include <QTreeWidgetItem>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
#include "src/core/basicclass.h"
#include "src/ui/recursivproxymodel.h"
class ModuleSelectUI : public QObject, public BasicClass
{
    Q_OBJECT
public:
    ModuleSelectUI(BasicClass *parent, QWidget *oarentWidget);
    ~ModuleSelectUI();
    void loadModuleData(const int moduleID, Actions::OpenLinkModifiers mod = Actions::NoModifer);    void selectModule(const int moduleID);

    void showSettingsDialog(const int moduleID);

    RecursivProxyModel *model() const;
    QItemSelectionModel *selectionModel() const;
    QStandardItemModel *moduleModel() const;

    void moduleChanged(const int moduleID);

    QPoint point() const;

    QAction* actionOpen() const;
    QAction* actionOpenInNewTab() const;
    QAction* actionSettings() const;
public slots:
    void showContextMenu(const QPoint point);
signals:
    void open();
    void openInNewTab();
    void configure();
private:
    int m_moduleID;
    RecursivProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
    QStandardItemModel *m_moduleModel;
    QPoint m_point;


    QMenu *m_contextMenu;
    QAction *m_actionOpen;
    QAction *m_actionOpenInNewTab;
    QAction *m_actionSettings;
    VerseUrl url(const int moduleID);

};

#endif // MODULESELECTUI_H
