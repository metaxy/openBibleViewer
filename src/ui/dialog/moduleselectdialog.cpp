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
#include "moduleselectdialog.h"
#include "ui_moduleselectdialog.h"

ModuleSelectDialog::ModuleSelectDialog(BasicClass *basic, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModuleSelectDialog)
{
    ui->setupUi(this);
    copy(basic);

    ui->treeView->clearSelection();
    m_moduleUI = new ModuleSelectUI(this, this);

    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), m_moduleUI, SLOT(showContextMenu(QPoint)));

    connect(m_moduleUI, SIGNAL(configure()), this, SLOT(configure()));

    ui->treeView->setSortingEnabled(true);
    ui->treeView->setModel(m_moduleUI->model());
    ui->treeView->setSelectionModel(m_moduleUI->selectionModel());

    m_moduleUI->model()->sort(0);
    m_moduleUI->setMenuStyle(ModuleSelectUI::JustSelect);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(close()));
}

ModuleSelectDialog::~ModuleSelectDialog()
{
    delete ui;
}
void ModuleSelectDialog::ok()
{
    const QModelIndexList list = m_moduleUI->selectionModel()->selectedIndexes();
    if(list.size() > 0)
        emit moduleSelected(list.first().data(Qt::UserRole + 1).toInt());
}
void ModuleSelectDialog::configure()
{
    m_moduleUI->showSettingsDialog(ui->treeView->indexAt(m_moduleUI->point()).data(Qt::UserRole + 1).toInt());
}
