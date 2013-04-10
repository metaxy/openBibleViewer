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
#ifndef MODULEMODEL_H
#define MODULEMODEL_H
#include <QStandardItemModel>

#include "src/core/module/module.h"
#include "src/core/settings/settings.h"
/**
 * ModuleModel represents a class, which generate a tree model of all modules.
 */
class ModuleModel
{
public:
    ModuleModel(QObject *parent);
    ~ModuleModel();

    void clear();
    void setSettings(Settings *settings);
    void generate();

    QStandardItemModel* itemModel() const;
    void setShowAll(bool showAll);

    void addCategoryFilter(const ModuleTools::ModuleCategory cat);
    void addCategoryFilter(const QList<ModuleTools::ModuleCategory> cats);
private:
    QStandardItemModel *m_moduleModel;
    Settings *m_settings;
    void createModuleItem(QStandardItem *parentItem, ModuleSettings *settings);

    QList<ModuleTools::ModuleCategory> m_catFilter;
};

#endif // MODULEMODEL_H
