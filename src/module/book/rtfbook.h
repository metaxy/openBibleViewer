#ifndef RTFBOOK_H
#define RTFBOOK_H
#include "src/module/book/bookmodule.h"
class RTFBook : public BookModule
{

public:
    RTFBook();

    MetaInfo readInfo(const QString &name);

    void search(const SearchQuery &query, SearchResult *res) const;
    bool hasIndex() const;
    void buildIndex();

    int moduleID() const;
    QString modulePath() const;
    QString moduleName(bool preferShortName = false) const;

    int loadModuleData(const int moduleID, const QString &path);

    QString readAll();
};

#endif // RTFBOOK_H
