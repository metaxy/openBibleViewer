#include "searchablemodule.h"
#include "src/core/dbghelper.h"
SearchableModule::SearchableModule()
{
}
void SearchableModule::search(SearchQuery /*query*/, SearchResult */*result*/)
{
    myWarning() << "calling searchable module";
}
