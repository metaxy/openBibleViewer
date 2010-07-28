#ifndef SEARCH_H
#define SEARCH_H
#include "src/core/basicclass.h"
#include "src/core/searchresult.h"
#include "src/core/searchquery.h"
class Search : public BasicClass
{
public:
    Search();
    SearchResult* search(SearchQuery query);
};

#endif // SEARCH_H
