#ifndef TREEBOOK_H
#define TREEBOOK_H
#include "src/module/simplemoduleclass.h"
#include "src/core/settings/moduledisplaysettings.h"
#include "src/module/searchablemodule.h"
#include "src/module/response/response.h"
#include "src/core/books/booktree.h"
#include "src/module/book/treebookmodule.h"
class TreeBook : public SimpleModuleClass, public SearchableModule
{
public:
    TreeBook();
    void search(SearchQuery query, SearchResult *result);
    void clearData();
    BookTree *bookTree();
    Response *readChapter(int chapterID);
    int loadModuleData(int moduleID);
protected:
    ModuleDisplaySettings *m_moduleDisplaySettings;


    bool m_loaded;

    QString m_moduleTitle;
    QString m_moduleShortTitle;
    QString m_moduleUID;
    QSharedPointer<TreeBookModule> m_bookModule;
};

#endif // TREEBOOK_H
