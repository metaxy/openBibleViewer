#ifndef BOOK_H
#define BOOK_H
#include "src/module/simplemoduleclass.h"
#include "src/core/settings/moduledisplaysettings.h"
#include "src/module/searchablemodule.h"
#include "src/module/response/response.h"

class Book : public SimpleModuleClass, public SearchableModule
{
public:
    Book();
    ~Book();
    void setModuleDisplaySettings(ModuleDisplaySettings *moduleDisplaySettings);
    void search(SearchQuery query, SearchResult *result);
    void clearData();

    Response* getAll();

protected:
    ModuleDisplaySettings *m_moduleDisplaySettings;
};

#endif // BOOK_H
