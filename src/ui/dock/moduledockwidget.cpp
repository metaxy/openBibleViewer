/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "src/core/obvcore.h"
#include "src/core/link/verseurl.h"
ModuleDockWidget::ModuleDockWidget(QWidget *parent) :
    DockWidget(parent),
    ui(new Ui::ModuleDockWidget)
{
    ui->setupUi(this);
    m_dontLoad = false;
    first = false;

    m_proxyModel = NULL;
    m_selectionModel = NULL;
}
/**
  * Init the ModuleDockWidget. Init proxyModel and selectionModel.
  */
void ModuleDockWidget::init()
{
    ui->treeView_module->clearSelection();

    if(m_proxyModel != NULL) {
        m_proxyModel->deleteLater();
        m_proxyModel = NULL;
    }
    if(m_selectionModel != NULL) {
        m_selectionModel->deleteLater();
        m_selectionModel = NULL;
    }
    m_moduleID = -1;

    ModuleModel model(this);
    model.setShowAll(true);
    model.setSettings(m_settings);
    model.generate();

    m_proxyModel = new RecursivProxyModel(this);
    m_proxyModel->setSourceModel(model.itemModel());
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Module"));
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_selectionModel = new QItemSelectionModel(m_proxyModel);

    connect(ui->lineEdit_filter, SIGNAL(textChanged(QString)), this, SLOT(filter(QString)));

    connect(ui->treeView_module, SIGNAL(activated(QModelIndex)), this, SLOT(loadModuleData(QModelIndex)));

    ui->treeView_module->setSortingEnabled(true);
    ui->treeView_module->setModel(m_proxyModel);
    ui->treeView_module->setSelectionModel(m_selectionModel);

    m_proxyModel->sort(0);
    connect(m_actions, SIGNAL(_setCurrentModule(int)), this, SLOT(loadedModule(int)));
}
/**
  * Emits get to load the selected module.
  */
void ModuleDockWidget::loadModuleData(QModelIndex index)
{
    const int moduleID = index.data(Qt::UserRole + 1).toString().toInt();
    if(m_dontLoad == false && moduleID >= 0) {

        Module *m = m_moduleManager->getModule(moduleID);
        //const OBVCore::ModuleType type = m->moduleType();
        const OBVCore::ModuleClass cl = m->moduleClass();
        if(cl == OBVCore::DictionaryModuleClass) {
            m_actions->get("dict:/"+QString::number(moduleID));
        } else if(cl == OBVCore::WebPageClass) {
            m_actions->get("webpage:/"+QString::number(moduleID));
        } else if(cl == OBVCore::BibleModuleClass) {
            myDebug() << "bible";
            m_moduleID = moduleID;
            VerseUrl url;
            VerseUrlRange range;
            range.setModule(moduleID);
            range.setBook(VerseUrlRange::LoadFirstBook);
            range.setChapter(VerseUrlRange::LoadFirstChapter);
            range.setWholeChapter();
            url.addRange(range);
            m_actions->get(url);
        }
    }
}
/**
  * If a module is loaded not through the ModuleDock, select the module.
  */
void ModuleDockWidget::loadedModule(int id)
{
    if(m_moduleID == id)
        return;
    m_moduleID = id;
    const QModelIndexList list = m_proxyModel->match(m_moduleManager->m_moduleModel->invisibleRootItem()->index(), Qt::UserRole + 1, QString::number(id));

    if(list.size() == 1) {
        m_selectionModel->clearSelection();
        m_selectionModel->setCurrentIndex(m_proxyModel->mapFromSource(list.at(0)), QItemSelectionModel::Select);
    }


}
void ModuleDockWidget::filter(QString string)
{
    if(first == false && !string.isEmpty()) {
        first = true;
        const QModelIndexList list = m_proxyModel->match(m_moduleManager->m_moduleModel->invisibleRootItem()->index(), Qt::UserRole + 1, "-1");
        for(int i = 0; i < list.size(); ++i) {
            ui->treeView_module->setExpanded(list.at(i), true);
        }
    }
    m_proxyModel->setFilterFixedString(string);
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
