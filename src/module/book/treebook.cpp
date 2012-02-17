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
