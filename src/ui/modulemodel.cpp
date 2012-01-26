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
    //m_moduleModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Module"));
    QStyle *style = QApplication::style();

    m_folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    m_folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);

    m_bibleIcon = QIcon::fromTheme("page-2sides", QIcon(":/icons/16x16/page-2sides.png"));
    m_dictionayIcon = QIcon::fromTheme("text-x-texinfo", QIcon(":/icons/16x16/text-x-texinfo.png"));
    m_bookIcon = QIcon::fromTheme("text-x-generic", QIcon(":/icons/16x16/text-x-generic.png"));
    //m_webIcon = QIcon::fromTheme("text-xml", QIcon(":/icons/16x16/text-html.png"));
}
ModuleModel::~ModuleModel()
{
}
void ModuleModel::clear()
{
    if(m_moduleModel != NULL)
        m_moduleModel->clear();
    m_catFilter.clear();
}
void ModuleModel::addCategoryFilter(const ModuleTools::ModuleCategory cat)
{
    m_catFilter.clear();
    m_catFilter.append(cat);
}

void ModuleModel::addCategoryFilter(const QList<ModuleTools::ModuleCategory> cats)
{
    m_catFilter = cats;
}

void ModuleModel::setSettings(Settings *settings)
{
    m_settings = settings;
}

void ModuleModel::generate()
{
    QStandardItem *parentItem = m_moduleModel->invisibleRootItem();

    ModuleSettings *rootModuleSettings = m_settings->getModuleSettings(-1);//it's the invisble root item
    if(rootModuleSettings != NULL) {
        foreach(ModuleSettings * s, rootModuleSettings->children()) {
            createModuleItem(parentItem, s);
        }
    }
}

QStandardItemModel* ModuleModel::itemModel() const
{
    return m_moduleModel;
}
void ModuleModel::createModuleItem(QStandardItem *parentItem, ModuleSettings *settings)
{
    QStandardItem *item = NULL;
    bool show = true;
    ModuleTools::ModuleCategory cat = ModuleTools::getCategory(settings->moduleType);
    if(!m_catFilter.isEmpty()) {
        show = false;
        if(m_catFilter.contains(cat)) {
            show = true;
        }
    }

    if(show == true) {
        item = new QStandardItem(settings->name(false));
        item->setData(QString::number(settings->moduleID));
        item->setToolTip(ModuleTools::moduleTypeName(settings->moduleType) + " - " + settings->modulePath + " (" + QString::number(settings->moduleID) + ")");

        if(cat == ModuleTools::BibleCategory) {
            item->setIcon(m_bibleIcon);
        } else if(cat == ModuleTools::DictionaryCategory) {
            item->setIcon(m_dictionayIcon);
        } else if(cat == ModuleTools::BookCategory) {
            item->setIcon(m_bookIcon);
        } else if(cat == ModuleTools::FolderCategory) {
            item->setIcon(m_folderIcon);
        }
       parentItem->appendRow(item);
    }

    //recursive
    foreach(ModuleSettings * s, settings->children()) {
        createModuleItem(item, s);
    }

}

