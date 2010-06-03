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
#include "moduledockwidget.h"
#include "ui_moduledockwidget.h"
#include "src/core/dbghelper.h"
#include <QModelIndexList>
#include "src/core/core.h"
ModuleDockWidget::ModuleDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::ModuleDockWidget)
{
    ui->setupUi(this);
    m_dontLoad = false;
    first = false;
}
/**
  Init the ModuleDockWidget. Init proxyModel and selectionModel.
  */
void ModuleDockWidget::init()
{
    DEBUG_FUNC_NAME

    m_proxyModel = new ModuleProxyModel(this);
    m_proxyModel->setSourceModel(m_moduleManager->m_moduleModel);
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Module"));
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_selectionModel = new QItemSelectionModel(m_proxyModel);
    connect(ui->lineEdit_filter, SIGNAL(textChanged(QString)), this, SLOT(filter(QString)));

    connect(ui->treeView_module, SIGNAL(clicked(QModelIndex)), this, SLOT(loadModuleData(QModelIndex)));

    ui->treeView_module->setSortingEnabled(true);
    ui->treeView_module->setModel(m_proxyModel);
    ui->treeView_module->setSelectionModel(m_selectionModel);
    m_proxyModel->sort(0);
}
/**
  Emits get to load the selected module.
  */
void ModuleDockWidget::loadModuleData(QModelIndex index)
{
    //DEBUG_FUNC_NAME
    QString data = index.data(Qt::UserRole + 1).toString();
    if (data.toInt() >= 0 && m_dontLoad == false) {
        emit get("bible://" + data + "/0,0,0");
    }
}
/**
  If a module is loaded not by the ModuleDock, select the module.
  */
void ModuleDockWidget::loadedModule(int id)
{
    //todo: reimplement match in moduleproxymodel
    // because it don't match modules in folders
    DEBUG_FUNC_NAME
    if(m_moduleID == id)
        return;
    m_moduleID = id;
    QModelIndexList list = m_proxyModel->match(ui->treeView_module->model()->index(0,0), Qt::UserRole + 1, QString::number(id));
    if (list.size() == 1) {
        m_selectionModel->clearSelection();
        m_selectionModel->setCurrentIndex(list.at(0), QItemSelectionModel::Select);
    }


}
void ModuleDockWidget::filter(QString string)
{
    if (first == false && string != "") {
        first = true;
        QModelIndexList list = m_proxyModel->match(ui->treeView_module->model()->index(0, 0), Qt::UserRole + 1, "-1");
        qDebug() << "list.size() = " << list.size();
        for (int i = 0; i < list.size(); ++i) {
            ui->treeView_module->setExpanded(list.at(i), true);
        }
    }
    m_proxyModel->setFilterFixedString(string);
}

ModuleDockWidget::~ModuleDockWidget()
{
    delete ui;
 //   delete m_proxyModel;
 //   delete m_selectionModel;
}

void ModuleDockWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
