#include "search.h"

Search::Search()
{
}
SearchResult* Search::search(SearchQuery query)
{
    SearchResult *result = new SearchResult();
    result->searchQuery = query;
    m_moduleManager->bible()->search(query, result);
    if(query.searchInNotes) {
        m_notes->search(query, result);
    }
    result->sort();//todo: move maybe to the display section, to avoid double sorting if there is the possibilty to change sort settings
    return result;
}
