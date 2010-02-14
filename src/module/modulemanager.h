/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H
#include "src/module/module.h"
#include "src/core/settings.h"
#include "src/module/bible.h"
#include "src/module/strong.h"
#include <QtCore/QList>
#include <QtGui/QTreeWidgetItem>
class ModuleManager
{
public:
    ModuleManager();
    void setSettings(Settings *settings);
    int loadAllModules();
    QList<Module> m_moduleList;
    QList<QTreeWidgetItem *> m_bibleItems;
    Settings *m_settings;
    Bible m_bible;
    Strong m_strong;
    bool bibleLoaded();
    bool strongLoaded();
    QString notePos2Text(const QString &pos);

};

#endif // MODULEMANAGER_H
