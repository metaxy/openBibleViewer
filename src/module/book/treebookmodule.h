#ifndef TREEBOOKMODULE_H
#define TREEBOOKMODULE_H
#include "src/core/search/searchresult.h"
#include "src/core/search/searchquery.h"

#include "src/core/settings/settings.h"
#include "src/module/metainfo.h"
#include "src/core/books/booktree.h"
#include "src/module/response/response.h"
#include <QtCore/QFile>
class TreeBookModule
{
public:
    TreeBookModule();
    virtual ~TreeBookModule();
    void setSettings(Settings *settings);

    virtual MetaInfo readInfo(const QString &name) = 0;

    virtual void search(const SearchQuery &query, SearchResult *res) const = 0;
    virtual bool hasIndex() const = 0;
    virtual void buildIndex() = 0;

    virtual int moduleID() const = 0;
    virtual QString modulePath() const = 0;
    virtual QString moduleName(bool preferShortName = false) const = 0;

    virtual int loadModuleData(const int moduleID, const QString &path) = 0;

    virtual Response* readChapter(const int chapterID) = 0;
    virtual BookTree * bookTree() = 0;
protected:
    Settings *m_settings;

    QString m_path;
    int m_moduleID;
    BookTree *m_tree;
};

#endif // TREEBOOKMODULE_H
