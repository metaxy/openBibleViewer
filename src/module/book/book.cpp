#include "book.h"

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

}

Response* Book::getAll()
{
    return 0;
}
