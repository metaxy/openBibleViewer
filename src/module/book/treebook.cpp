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
#include "treebook.h"

TreeBook::TreeBook()
{
}
void TreeBook::search(SearchQuery query, SearchResult *result)
{

}
int TreeBook::loadModuleData(int moduleID)
{
    m_loaded = false;
    m_module = m_map->module(moduleID);
    m_moduleID = moduleID;
    m_moduleType = m_module->moduleType();

    //ModuleSettings *m = m_settings->getModuleSettings(m_moduleID);


    if(m_module->m_treeBookModule.isNull()) {
        m_bookModule = m_module->newTreeBookModule(moduleType());
    } else {
        m_bookModule = m_module->m_treeBookModule;
    }

    if(m_bookModule == NULL) {
        myWarning() << "invalid module";
        return 1;
    }
    m_bookModule->setSettings(m_settings);

    int loaded = m_bookModule->loadModuleData(m_moduleID, m_module->path());
    if(loaded != 0) {
        myWarning() << "loading failed";
        return 1;
    }
    m_loaded = true;
    return 0;
}

void TreeBook::clearData()
{
   // return m_bookModule
}

BookTree * TreeBook::bookTree()
{
    return m_bookModule->bookTree();
}
Response *TreeBook::readChapter(int chapterID)
{
    if(!m_loaded) {
        loadModuleData(m_moduleID);
    }
    return m_bookModule->readChapter(chapterID);
}
