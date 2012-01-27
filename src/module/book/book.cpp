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
