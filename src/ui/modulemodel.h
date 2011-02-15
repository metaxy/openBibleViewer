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
#ifndef MODULEMODEL_H
#define MODULEMODEL_H
#include <QtGui/QStandardItemModel>
#include <QtGui/QStyle>
#include <QtGui/QApplication>
#include "src/module/module.h"
#include "src/core/settings/settings.h"
class ModuleModel
{
public:
    ModuleModel(QObject *parent);
    ~ModuleModel();

    void clear();
    void setSettings(Settings *settings);
    void generate();
    QStandardItemModel* itemModel() const;
private:
    QStandardItemModel *m_moduleModel;
    Settings *m_settings;
    void loadModule(QStandardItem *parentItem, ModuleSettings *settings);

    QIcon m_bibleQuoteIcon;
    QIcon m_folderIcon;
    QIcon m_bibleZefaniaIcon;
};

#endif // MODULEMODEL_H
