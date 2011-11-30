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
#include "modulemodel.h"

ModuleModel::ModuleModel(QObject *parent)
{
    m_moduleModel = new QStandardItemModel(parent);
    m_settings = NULL;
    m_showAll = false;
    //m_moduleModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Module"));
    QStyle *style = QApplication::style();
    m_folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    m_folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);

    m_bibleQuoteIcon = QIcon::fromTheme("text-x-generic", QIcon(":/icons/16x16/text-x-generic.png"));
    m_bibleZefaniaIcon = QIcon::fromTheme("text-xml", QIcon(":/icons/16x16/text-xml.png"));
}
ModuleModel::~ModuleModel()
{
}
void ModuleModel::clear()
{
    m_moduleModel->clear();
}

void ModuleModel::setSettings(Settings *settings)
{
    m_settings = settings;
}

void ModuleModel::generate()
{
    DEBUG_FUNC_NAME
    QStandardItem *parentItem = m_moduleModel->invisibleRootItem();

    ModuleSettings *rootModuleSettings = m_settings->getModuleSettings(-1);//it's the invisble root item
    if(rootModuleSettings != NULL) {
        foreach(ModuleSettings * s, rootModuleSettings->children()) {
            loadModule(parentItem, s);
        }
    }
}

QStandardItemModel* ModuleModel::itemModel() const
{
    return m_moduleModel;
}
void ModuleModel::loadModule(QStandardItem *parentItem, ModuleSettings *settings)
{
    QStandardItem *item = NULL;

    if(settings->moduleType == OBVCore::BibleQuoteModule || settings->moduleType == OBVCore::ZefaniaBibleModule || settings->moduleType == OBVCore::TheWordBibleModule || settings->moduleType == OBVCore::SwordBibleModule) {
        item = new QStandardItem;
        item->setText(settings->moduleName);
        item->setData(QString::number(settings->moduleID));
        item->setToolTip(Module::moduleTypeName(settings->moduleType) + " - " + settings->modulePath + " (" + QString::number(settings->moduleID) + ")");

        if(settings->moduleType == OBVCore::BibleQuoteModule || settings->moduleType == OBVCore::TheWordBibleModule || settings->moduleType == OBVCore::SwordBibleModule) {
            item->setIcon(m_bibleQuoteIcon);
        } else if(settings->moduleType == OBVCore::ZefaniaBibleModule) {
            item->setIcon(m_bibleZefaniaIcon);
        }
        parentItem->appendRow(item);
    } else if(settings->moduleType == OBVCore::ZefaniaLexModule || settings->moduleType == OBVCore::BibleQuoteDictModule) {
        if(m_showAll) {
            item = new QStandardItem;
            item->setText(settings->moduleName);
            item->setData(QString::number(settings->moduleID));
            item->setIcon(m_bibleZefaniaIcon);
            item->setToolTip(Module::moduleTypeName(settings->moduleType) + " - " + settings->modulePath + " (" + QString::number(settings->moduleID) + ")");
            parentItem->appendRow(item);
        }
    } else if(settings->moduleType == OBVCore::WebPageModule || settings->moduleType == OBVCore::WebDictionaryModule) {
        if(m_showAll) {
            item = new QStandardItem;
            item->setText(settings->moduleName);
            item->setData(QString::number(settings->moduleID));
            item->setToolTip(Module::moduleTypeName(settings->moduleType) + " - " + settings->modulePath + " (" + QString::number(settings->moduleID) + ")");
            parentItem->appendRow(item);
        }
    } else if(settings->moduleType == OBVCore::FolderModule) {
        item = new QStandardItem;
        item->setText(settings->moduleName);
        item->setData(QString::number(settings->moduleID));
        item->setIcon(m_folderIcon);
        parentItem->appendRow(item);
    }

    //recursive
    foreach(ModuleSettings * s, settings->children()) {
        loadModule(item, s);
    }

}
void ModuleModel::setShowAll(bool showAll)
{
    m_showAll = showAll;
}
