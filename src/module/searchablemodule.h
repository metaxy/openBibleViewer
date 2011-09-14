#ifndef SEARCHABLEMODULE_H
#define SEARCHABLEMODULE_H

#include "src/core/search/searchquery.h"
#include "src/core/search/searchresult.h"

class SearchableModule
{
public:
    SearchableModule();

    /**
     * Searchs in the current Module. Saves the results in the second argument (*result).
     */
    virtual void search(SearchQuery query, SearchResult *result);
};

#endif // SEARCHABLEMODULE_H
