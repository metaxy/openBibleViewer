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
#include "src/module/moduletools.h"
#include "src/core/link/verseurl.h"
#include "src/ui/dialog/moduleconfigdialog.h"
#include <QtCore/QModelIndexList>
#include <QtCore/QPointer>
#include <QtGui/QMenu>

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
    model.setSettings(m_settings);
    model.generate();

    m_proxyModel = new RecursivProxyModel(this);
    m_proxyModel->setSourceModel(model.itemModel());
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Module"));
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_selectionModel = new QItemSelectionModel(m_proxyModel);

    connect(ui->lineEdit_filter, SIGNAL(textChanged(QString)), this, SLOT(filter(QString)));

    connect(ui->treeView_module, SIGNAL(activated(QModelIndex)), this, SLOT(loadModuleData(QModelIndex)));
    connect(ui->treeView_module, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));

    ui->treeView_module->setSortingEnabled(true);
    ui->treeView_module->setModel(m_proxyModel);
    ui->treeView_module->setSelectionModel(m_selectionModel);

    m_proxyModel->sort(0);
    connect(m_actions, SIGNAL(_setCurrentModule(int)), this, SLOT(loadedModule(int)));
    connect(m_actions, SIGNAL(_moduleChanged(int)), this, SLOT(moduleChanged(int)));
}
/**
  * Emits get to load the selected module.
  */
void ModuleDockWidget::loadModuleData(QModelIndex index, Actions::OpenLinkModifiers mod)
{
    const int moduleID = index.data(Qt::UserRole + 1).toInt();
    if(m_dontLoad == false && moduleID >= 0) {

        Module *m = m_moduleManager->getModule(moduleID);
        //const ModuleTools::ModuleType type = m->moduleType();
        const ModuleTools::ModuleClass cl = m->moduleClass();
        if(cl == ModuleTools::DictionaryModuleClass) {
            m_actions->get("dict:/" + QString::number(moduleID), mod);
            m_settings->getModuleSettings(moduleID)->stats_timesOpend++;
        } else if(cl == ModuleTools::WebPageClass) {
            m_actions->get("webpage:/" + QString::number(moduleID), mod);
            m_settings->getModuleSettings(moduleID)->stats_timesOpend++;
        } else if(cl == ModuleTools::BibleModuleClass) {
            myDebug() << "bible";
            m_moduleID = moduleID;
            VerseUrlRange range;
            range.setModule(moduleID);
            range.setBook(VerseUrlRange::LoadCurrentBook);
            range.setChapter(VerseUrlRange::LoadCurrentChapter);
            range.setStartVerse(VerseUrlRange::LoadCurrentVerse);
            range.setOpenToTransformation(true);

            VerseUrl url(range);
            m_actions->get(url, mod);
            m_settings->getModuleSettings(moduleID)->stats_timesOpend++;
        }
    }
}
/**
  * If a module is loaded not through the ModuleDock, select the module.
  */
void ModuleDockWidget::loadedModule(const int id)
{
    DEBUG_FUNC_NAME
    if(id == -1) {
        m_selectionModel->clearSelection();
        return;
    }

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
void ModuleDockWidget::contextMenu(QPoint point)
{
    m_point = point;
    QPointer<QMenu> contextMenu = new QMenu(ui->treeView_module);
    contextMenu->setObjectName("contextMenu");

    QAction *actionOpen = new QAction(QIcon::fromTheme("document-open", QIcon(":/icons/16x16/document-open.png")), tr("Open module"), contextMenu);
    actionOpen->setObjectName("actionOpen");
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));

    QAction *actionOpenInNewTab = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("Open module in new tab"), contextMenu);
    actionOpenInNewTab->setObjectName("actionPaste");
    connect(actionOpenInNewTab, SIGNAL(triggered()), this, SLOT(openInNewTab()));

    QAction *actionSettings = new QAction(QIcon::fromTheme("configure", QIcon(":/icons/16x16/configure.png")), tr("Configure"), contextMenu);
    actionSettings->setObjectName("actionNew");
    connect(actionSettings, SIGNAL(triggered()), this, SLOT(configure()));



    contextMenu->addAction(actionOpen);
    contextMenu->addAction(actionOpenInNewTab);

    contextMenu->addSeparator();
    contextMenu->addAction(actionSettings);

    contextMenu->exec(QCursor::pos());
    delete contextMenu;
}
void ModuleDockWidget::open()
{
    loadModuleData(ui->treeView_module->indexAt(m_point));
}
void ModuleDockWidget::openInNewTab()
{
    loadModuleData(ui->treeView_module->indexAt(m_point), Actions::OpenInNewWindow);
}
void ModuleDockWidget::configure()
{
    const int moduleID = ui->treeView_module->indexAt(m_point).data(Qt::UserRole + 1).toInt();
    if(m_dontLoad == false && moduleID >= 0) {
        QPointer<ModuleConfigDialog> mDialog = new ModuleConfigDialog(this);
        mDialog->setModule(m_settings->getModuleSettings(moduleID));
        connect(mDialog, SIGNAL(save(ModuleSettings)), mDialog, SLOT(close()));
        mDialog->exec();
        delete mDialog;
        m_actions->moduleChanged(moduleID);
    }
}
void ModuleDockWidget::moduleChanged(const int moduleID)
{
    const QModelIndexList list = m_proxyModel->match(m_moduleManager->m_moduleModel->invisibleRootItem()->index(), Qt::UserRole + 1, QString::number(moduleID));

    if(list.size() == 1) {
        m_proxyModel->setData(m_proxyModel->mapFromSource(list.first()), m_settings->getModuleSettings(moduleID)->name(false), Qt::DisplayRole);
    }
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
