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
#include "moduledockwidget.h"
#include "ui_moduledockwidget.h"
#include "src/core/dbghelper.h"
#include <QtCore/QPointer>
#include <QtGui/QMenu>

ModuleDockWidget::ModuleDockWidget(QWidget *parent) :
    DockWidget(parent),
    ui(new Ui::ModuleDockWidget),
    m_dontLoad(false),
    m_first(false),
    m_moduleUI(NULL)
{
    ui->setupUi(this);
}
/**
  * Init the ModuleDockWidget. Init proxyModel and selectionModel.
  */
void ModuleDockWidget::init()
{
    ui->treeView_module->clearSelection();
    if(m_moduleUI != NULL) {
        delete m_moduleUI;
        m_moduleUI = NULL;
    }
    m_moduleUI = new ModuleSelectUI(this, this);

    connect(ui->lineEdit_filter, SIGNAL(textChanged(QString)), this, SLOT(filter(QString)));

    connect(ui->treeView_module, SIGNAL(activated(QModelIndex)), this, SLOT(loadModuleData(QModelIndex)));
    connect(ui->treeView_module, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));

    connect(m_moduleUI, SIGNAL(configure()), this, SLOT(configure()));
    connect(m_moduleUI, SIGNAL(open()), this, SLOT(open()));
    connect(m_moduleUI, SIGNAL(openInNewTab()), this, SLOT(openInNewTab()));


    ui->treeView_module->setSortingEnabled(true);
    ui->treeView_module->setModel(m_moduleUI->model());
    ui->treeView_module->setSelectionModel(m_moduleUI->selectionModel());

    m_moduleUI->model()->sort(0);
    connect(m_actions, SIGNAL(_setCurrentModule(int)), this, SLOT(loadedModule(int)));
    connect(m_actions, SIGNAL(_moduleChanged(int)), this, SLOT(moduleChanged(int)));
}
/**
  * Emits get to load the selected module.
  */
void ModuleDockWidget::loadModuleData(QModelIndex index, Actions::OpenLinkModifiers mod)
{
    if(m_dontLoad)
        return;
    const int moduleID = index.data(Qt::UserRole + 1).toInt();
    m_moduleUI->loadModuleData(moduleID, mod);
}
/**
  * If a module is loaded not through the ModuleDock, select the module.
  */
void ModuleDockWidget::loadedModule(const int id)
{
    DEBUG_FUNC_NAME
    m_moduleUI->selectModule(id);
}
void ModuleDockWidget::filter(const QString &string)
{
    //first time: expand all
    if(m_first == false && !string.isEmpty()) {
        m_first = true;
        const QModelIndexList list = m_moduleUI->model()->match(m_moduleUI->moduleModel()->invisibleRootItem()->index(), Qt::UserRole + 1, "-1");
        foreach(const QModelIndex &index, list) {
            ui->treeView_module->setExpanded(index, true);
        }
    }
    m_moduleUI->model()->setFilterFixedString(string);
}
void ModuleDockWidget::contextMenu(const QPoint &point)
{
   m_moduleUI->showContextMenu(point);
}
void ModuleDockWidget::open()
{
    loadModuleData(ui->treeView_module->indexAt(m_moduleUI->point()));
}
void ModuleDockWidget::openInNewTab()
{
    loadModuleData(ui->treeView_module->indexAt(m_moduleUI->point()), Actions::OpenInNewWindow);
}
void ModuleDockWidget::configure()
{
    if(m_dontLoad)
        return;
    const int moduleID = ui->treeView_module->indexAt(m_moduleUI->point()).data(Qt::UserRole + 1).toInt();
   m_moduleUI->showSettingsDialog(moduleID);
}
void ModuleDockWidget::moduleChanged(const int moduleID)
{
    m_moduleUI->moduleChanged(moduleID);
}

ModuleDockWidget::~ModuleDockWidget()
{
    delete ui;
}

void ModuleDockWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
