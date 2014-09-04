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
#include "commentary.h"

Commentary::Commentary() : m_loaded(false)
{
}
int Commentary::loadModuleData(const int moduleID)
{
    myDebug() << moduleID;
    m_loaded = false;
    m_module = m_map->module(moduleID);
    m_moduleID = moduleID;
    m_moduleType = m_module->moduleType();

    if(m_module->m_commentaryModule.isNull()) {
        m_commentaryModule = m_module->newCommentaryModule(moduleType());
    } else {
        m_commentaryModule = m_module->m_commentaryModule;
    }

    if(m_commentaryModule == nullptr) {
        myWarning() << "invalid module";
        return 1;
    }
    m_commentaryModule->setSettings(m_settings);

    if(m_commentaryModule->loadModuleData(m_moduleID, m_module->path()) != 0) {
        myWarning() << "loading failed";
        return 1;
    }

    m_versification = m_commentaryModule->versification();
    m_loadedModuleID = m_moduleID;
    m_loaded = true;

    return 0;
}

Response* Commentary::readRanges(const Ranges &ranges, bool ignoreModuleID)
{
    Range r = ranges.getList().first();

    if(!loaded() || (m_moduleID != r.moduleID() && !ignoreModuleID)) {//nothing loaded here yet
        loadModuleData(r.moduleID());
    }

    if(!loaded()) {
        myWarning() << "failed reading because module not laoded";
        return nullptr;
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
/**
 * @brief Commentary::readStart shows the "Welcome" screen
 * @param moduleID if moduleID == ModuleIDNotSet then it loads the current module start screen
 * @return Response
 */
Response* Commentary::readStart(int moduleID)
{
    if(!loaded()) {
        if(moduleID == ModuleIDNotSet) moduleID = m_moduleID;

        loadModuleData(moduleID);
    }

    if(!loaded()) {
        myWarning() << "failed reading because module not loaded";
        return nullptr;
    }

    return m_commentaryModule->readStart();
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
