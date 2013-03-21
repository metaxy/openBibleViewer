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
#include "moduleselectui.h"
#include "src/core/dbghelper.h"
#include "src/ui/dialog/moduleconfigdialog.h"
#include "src/core/moduletools.h"
#include "src/core/link/verseurl.h"
#include "src/ui/modulemodel.h"
#include <QtCore/QPointer>
#include <QtGui/QMenu>
ModuleSelectUI::ModuleSelectUI(BasicClass *parent, QWidget *parentWidget) : QObject(parentWidget), m_style(ModuleSelectUI::JustSelect)
{
    copy(parent);
    m_moduleID = -1;
    ModuleModel model(parentWidget);
    model.setSettings(m_settings);
    model.generate();

    m_moduleModel = model.itemModel();

    m_proxyModel = new RecursivProxyModel(parentWidget);
    m_proxyModel->setSourceModel(m_moduleModel);
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Module"));
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_selectionModel = new QItemSelectionModel(m_proxyModel);
}
ModuleSelectUI::~ModuleSelectUI()
{
    m_proxyModel->deleteLater();
    m_selectionModel->deleteLater();
}
void ModuleSelectUI::setMenuStyle(ModuleSelectUI::MenuStyle style)
{
    m_style = style;
}

RecursivProxyModel *ModuleSelectUI::model() const
{
    return m_proxyModel;
}
QItemSelectionModel *ModuleSelectUI::selectionModel() const
{
    return m_selectionModel;
}
QStandardItemModel *ModuleSelectUI::moduleModel() const
{
    return m_moduleModel;
}
void ModuleSelectUI::loadModuleData(const int moduleID, Actions::OpenLinkModifiers mod)
{
    if(moduleID >= 0) {
        m_moduleID = moduleID;
        Module *m = m_moduleManager->getModule(moduleID);

        const ModuleTools::ModuleClass cl = m->moduleClass();
        m_settings->getModuleSettings(moduleID)->stats_timesOpened++;
        switch(cl)
        {
            case ModuleTools::DictionaryModuleClass:
                m_actions->get(ModuleTools::dictScheme+ QString::number(moduleID), mod);
                break;
            case ModuleTools::WebPageClass:
                m_actions->get(ModuleTools::webPageScheme + QString::number(moduleID), mod);
                break;
            case ModuleTools::BookClass:
                m_actions->get(ModuleTools::bookScheme + QString::number(moduleID), mod);
                break;
            case ModuleTools::TreeBookClass:
                m_actions->get(ModuleTools::treeBookScheme + QString::number(moduleID) + ",0", mod);
                break;
            case ModuleTools::CommentaryClass:
            case ModuleTools::BibleModuleClass:
                m_actions->get(url(moduleID), mod);
                break;
             default:
                myWarning() << "module class not support in ModuleSelectUI";
        }
    }
}
VerseUrl ModuleSelectUI::url(const int moduleID)
{
    VerseUrlRange range;
    range.setModule(moduleID);
    range.setBook(VerseUrlRange::LoadCurrentBook);
    range.setChapter(VerseUrlRange::LoadCurrentChapter);
    range.setStartVerse(VerseUrlRange::LoadCurrentVerse);
    range.setOpenToTransformation(true);

    VerseUrl url(range);
    return url;
}

void ModuleSelectUI::selectModule(const int moduleID)
{
    if(moduleID == -1) {
        m_selectionModel->clearSelection();
        return;
    }

    if(m_moduleID == moduleID)
        return;

    m_moduleID = moduleID;
    const QModelIndexList list = m_proxyModel->match(m_moduleModel->invisibleRootItem()->index(),
                                                     Qt::UserRole + 1,
                                                     QString::number(moduleID));

    if(!list.isEmpty()) {
        m_selectionModel->clearSelection();
        m_selectionModel->setCurrentIndex(m_proxyModel->mapFromSource(list.first()), QItemSelectionModel::Select);
    }
}
void ModuleSelectUI::showContextMenu(const QPoint point)
{
    m_point = point;

    QMenu* contextMenu = new QMenu;
    contextMenu->setObjectName("contextMenu");

    QAction*actionSettings = new QAction(QIcon::fromTheme("configure", QIcon(":/icons/16x16/configure.png")), tr("Configure"), contextMenu);
    actionSettings->setObjectName("actionSettings");
    connect(actionSettings, SIGNAL(triggered()), this, SIGNAL(configure()));

    if(m_style == ModuleSelectUI::JustSelect) {
        contextMenu->addAction(actionSettings);
        contextMenu->exec(QCursor::pos());
    } else {
        QAction* actionOpen = new QAction(QIcon::fromTheme("document-open", QIcon(":/icons/16x16/document-open.png")), tr("Open module"), contextMenu);
        actionOpen->setObjectName("actionOpen");
        connect(actionOpen, SIGNAL(triggered()), this, SIGNAL(open()));

        QAction*actionOpenInNewTab = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("Open module in new tab"), contextMenu);
        actionOpenInNewTab->setObjectName("actionOpenInNewTab");
        connect(actionOpenInNewTab, SIGNAL(triggered()), this, SIGNAL(openInNewTab()));

        QAction*actionOpenPV = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("Open vertical parallel"), contextMenu);
        actionOpenPV->setObjectName("actionOpenPV");
        connect(actionOpenPV, SIGNAL(triggered()), this, SIGNAL(openParallelV()));


        QAction*actionOpenPH = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("Open horizontal parallel"), contextMenu);
        actionOpenPH->setObjectName("actionOpenPH");
        connect(actionOpenPH, SIGNAL(triggered()), this, SIGNAL(openParallelH()));


        contextMenu->addAction(actionOpen);
        contextMenu->addAction(actionOpenInNewTab);

        contextMenu->addSeparator();
        contextMenu->addAction(actionSettings);
        contextMenu->exec(QCursor::pos());
    }
    contextMenu->deleteLater();
}

void ModuleSelectUI::showSettingsDialog(const int moduleID)
{
    if(moduleID < 0)
        return;

    QPointer<ModuleConfigDialog> mDialog = new ModuleConfigDialog;
    mDialog->setModule(m_settings->getModuleSettings(moduleID));
    mDialog->setSettings(m_settings);
    connect(mDialog, SIGNAL(save(int)), mDialog, SLOT(close()));
    mDialog->exec();

    delete mDialog;
    m_actions->moduleChanged(moduleID);

}
void ModuleSelectUI::moduleChanged(const int moduleID)
{
    //todo: you can change the default module from ModuleSelectUI::showSettingsDialog
    // but the icon in the m_moduleModel won't change.
    const QModelIndexList list = m_proxyModel->match(m_moduleModel->invisibleRootItem()->index(), Qt::UserRole + 1, QString::number(moduleID));

    if(!list.isEmpty()) {
        m_proxyModel->setData(m_proxyModel->mapFromSource(list.first()), m_settings->getModuleSettings(moduleID)->name(false), Qt::DisplayRole);
    }
}
QPoint ModuleSelectUI::point() const
{
    return m_point;
}

