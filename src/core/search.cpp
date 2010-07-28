#include "search.h"

Search::Search()
{
}
SearchResult* Search::search(SearchQuery query)
{
    SearchResult *result = new SearchResult();
    result->searchQuery = query;
    m_moduleManager->bible()->search(query,result);
    if(query.searchInNotes) {
        m_notes->search(query,result);
    }
    return result;
}
