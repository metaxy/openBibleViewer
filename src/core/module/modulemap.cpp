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
#include "src/core/module/modulemap.h"
#include "src/core/dbghelper.h"
ModuleMap::ModuleMap()
{
}
ModuleMap::~ModuleMap()
{
    DEBUG_FUNC_NAME
    foreach(Module * m, m_data) {
        if(m != NULL) {
            delete m;
        }
    }
    m_data.clear();
}

Module* ModuleMap::module(const int id) const
{
    return m_data.value(id);
}
bool ModuleMap::contains(const int id) const
{
    return m_data.contains(id);
}
QMapIterator<int, Module*> ModuleMap::it() const
{
    return QMapIterator<int, Module*>(m_data);
}
void ModuleMap::insert(Module *module)
{
    m_data.insert(module->moduleID(), module);
}
/**
 * Returns nullptr if not found.
 */
Module * ModuleMap::moduleByUID(const QString &uid)
{
    foreach(Module * m, m_data) {
        if(m->moduleUID() == uid) {
            return m;
        }
    }
    return nullptr;
}
/**
 * @brief ModuleMap::UIDtoID
 * @param uid
 * @return -1 if not found
 */
int ModuleMap::UIDtoID(const QString &uid)
{
    Module *m = moduleByUID(uid);
    if(m) return m->moduleID();
    return -1;
}
