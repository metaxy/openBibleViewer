#include "commentary.h"

Commentary::Commentary()
{
}
Response* Commentary::readRanges(const Ranges &ranges, bool ignoreModuleID = false)
{

}

void Commentary::search(SearchQuery query, SearchResult *result)
{
    m_module->search(query, result);
}

void Commentary::clearData()
{

}

bool Commentary::loaded()
{
    return true;
}
int Commentary::currentBook()
{
    return m_module->currentBook();
}

int Commentary::currentChapter()
{
    return m_module->currentChapter();
}

std::pair<int, int> Commentary::minMaxVerse(const int bookID, const int chapterID)
{
    return m_module->minMaxVerse(bookID, chapterID);
}
