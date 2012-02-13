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
#include "book.h"
#include "src/module/response/htmlresponse.h"
Book::Book()
{
}
Book::~Book()
{

}

void Book::setModuleDisplaySettings(ModuleDisplaySettings *moduleDisplaySettings)
{
    m_moduleDisplaySettings = moduleDisplaySettings;
}
void Book::search(SearchQuery query, SearchResult *result)
{

}

void Book::clearData()
{
    m_loaded = false;
    m_bookModule.clear();
}

Response* Book::getAll()
{
    if(!m_loaded) {
        loadModuleData(m_moduleID);
    }

    if(m_loaded)
        return new HtmlResponse(m_bookModule->readAll());
    else
        return NULL;
}

int Book::loadModuleData(int moduleID)
{
    m_loaded = false;
    m_module = m_map->module(moduleID);

    //not valid module
    if(moduleID < 0 || m_module == NULL) {
        return 1;
    }

    m_moduleType = m_module->moduleType();
    if(moduleType() == ModuleTools::NoneType || m_module->moduleClass() != ModuleTools::BookClass) {
        return 2;
    }
    m_moduleID = moduleID;


    if(m_module->m_bookModule.isNull()) {
        m_bookModule = m_module->newBookModule(moduleType());
    } else {
        m_bookModule = m_module->m_bookModule;
    }

    if(m_bookModule == NULL) {
        return 1;
    }
    ModuleSettings *m = m_settings->getModuleSettings(m_moduleID);
    m_bookModule->setSettings(m_settings);
    int loaded = m_bookModule->loadModuleData(m_moduleID, m_module->path());

    if(loaded != 0) {
        m_loaded = false;
        return 1;
    }

 //   m_moduleUID = m_bookModule->uid();

    if(m->moduleName.isEmpty())
        m_moduleTitle = m_bookModule->moduleName(false);
    else
        m_moduleTitle = m->moduleName;

    m_moduleShortTitle = m_bookModule->moduleName(true);

    m_loaded = true;

    return 0;
}
