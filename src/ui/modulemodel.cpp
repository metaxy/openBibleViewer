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
#include "modulemodel.h"

ModuleModel::ModuleModel(QObject *parent) :  m_moduleModel(new QStandardItemModel(parent)), m_settings(nullptr)
{
    //m_moduleModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Module"));
}
ModuleModel::~ModuleModel()
{
}
void ModuleModel::clear()
{
    if(m_moduleModel != nullptr)
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
    if(rootModuleSettings != nullptr) {
        foreach(ModuleSettings * s, rootModuleSettings->children()) {
            createModuleItem(parentItem, s);
        }
    }
}

QStandardItemModel* ModuleModel::itemModel() const
{
    return m_moduleModel;
}
bool ModuleModel::createModuleItem(QStandardItem *parentItem, ModuleSettings *settings)
{
    ModuleTools::ModuleCategory cat = ModuleTools::getCategory(settings->moduleType);
    if(cat != ModuleTools::FolderCategory &&
            !m_catFilter.isEmpty() &&
            !m_catFilter.contains(cat)) return false;
    QStandardItem *item = new QStandardItem(settings->name(false));
    item->setData(QString::number(settings->moduleID));
    item->setToolTip(ModuleTools::moduleTypeName(settings->moduleType) + " - " + settings->modulePath + " (" + QString::number(settings->moduleID) + ")");
    item->setIcon(settings->icon());


    bool atLeastOne = false;
    bool noneChildren = (cat != ModuleTools::FolderCategory);
    foreach(ModuleSettings * s, settings->children()) {
        noneChildren = false;
        bool a = createModuleItem(item, s);
        atLeastOne = atLeastOne || a;
    }

    if(!atLeastOne && !noneChildren) {
        delete item;
        return false;
    }
    parentItem->appendRow(item);
    return true;
}

