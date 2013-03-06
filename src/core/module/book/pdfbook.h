#ifndef PDFBOOK_H
#define PDFBOOK_H
#include "src/core/module/book/bookmodule.h"
class PDFBook : public BookModule
{
public:
    PDFBook();
    MetaInfo readInfo(const QString &fileName);

    void search(const SearchQuery &query, SearchResult *res) const;
    bool hasIndex() const;
    void buildIndex();

    int moduleID() const;
    QString modulePath() const;
    QString moduleName(bool preferShortName = false) const;

    int loadModuleData(const int moduleID, const QString &path);

    Response* readAll();
};

#endif // PDFBOOK_H
