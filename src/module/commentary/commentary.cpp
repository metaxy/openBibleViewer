#include "commentary.h"

Commentary::Commentary()
{
    m_loaded = false;
}
Response* Commentary::readRanges(const Ranges &ranges, bool ignoreModuleID)
{
    DEBUG_FUNC_NAME
    Range r = ranges.getList().first();
    myDebug() << m_moduleID << r.moduleID() << ignoreModuleID;
    if(!loaded() || (m_moduleID != r.moduleID() && !ignoreModuleID)) {
        loadModuleData(r.moduleID());
    }
    if(!loaded()) {
        myWarning() << "failed reading because module not laoded";
        return NULL;
    }
    CompiledRange range = this->toCompiledRange(r);
    if(r.endVerse() == RangeEnum::NoneVerse || r.startVerse() == RangeEnum::NoneVerse) {
        return m_commentaryModule->readChapter(range.bookID, range.chapterID);
    } else if(r.chapter() == RangeEnum::NoneChapter) {
        return m_commentaryModule->readBook(range.bookID);
    } else {
        return m_commentaryModule->readVerseRange(range.bookID, range.chapterID, range.startVerse, range.endVerse);
    }


}

void Commentary::search(SearchQuery query, SearchResult *result)
{
    m_commentaryModule->search(query, result);
}

void Commentary::clearData()
{

}

bool Commentary::loaded()
{
    return m_loaded && (m_moduleID == m_loadedModuleID);
}
int Commentary::currentBook()
{
    return m_commentaryModule->currentBook();
}

int Commentary::currentChapter()
{
    return m_commentaryModule->currentChapter();
}

std::pair<int, int> Commentary::minMaxVerse(const int bookID, const int chapterID)
{
    return m_commentaryModule->minMaxVerse(bookID, chapterID);
}
int Commentary::loadModuleData(const int moduleID)
{
    myDebug() << moduleID;
    m_loaded = false;
    m_module = m_map->module(moduleID);
    m_moduleID = moduleID;
    m_moduleType = m_module->moduleType();

    //ModuleSettings *m = m_settings->getModuleSettings(m_moduleID);


    if(m_module->m_commentaryModule.isNull()) {
        m_commentaryModule = m_module->newCommentaryModule(moduleType());

    } else {
        m_commentaryModule = m_module->m_commentaryModule;
    }

    if(m_commentaryModule == NULL) {
        myWarning() << "invalid module";
        return 1;
    }
    m_commentaryModule->setSettings(m_settings);

    int loaded = m_commentaryModule->loadModuleData(m_moduleID, m_module->path());
    m_versification = m_commentaryModule->versification();
    if(loaded != 0) {
        myWarning() << "loading failed";
        return 1;
    }
    m_loadedModuleID = m_moduleID;
    m_loaded = true;
    return 0;
}
