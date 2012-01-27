#ifndef TXTBOOK_H
#define TXTBOOK_H
#include "src/module/book/bookmodule.h"
class TxtBook : public BookModule
{
public:
    TxtBook();
    ~TxtBook();
    MetaInfo readInfo(QFile &file);

    void search(const SearchQuery &query, SearchResult *res) const;
    bool hasIndex() const;
    void buildIndex();

    int moduleID() const;
    QString modulePath() const;
    QString moduleName(bool preferShortName = false) const;

    int loadModuleData(const int moduleID, const QString &path);

    QString readAll();
};

#endif // TXTBOOK_H
