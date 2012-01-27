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


    int loadModuleData(int moduleID);
    bool m_loaded;

    QString m_moduleTitle;
    QString m_moduleShortTitle;
    QString m_moduleUID;
    QSharedPointer<BookModule> m_bookModule;
};

#endif // BOOK_H
