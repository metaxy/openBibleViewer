#ifndef BIBLEMODULE_H
#define BIBLEMODULE_H
#include <QtCore/QFile>
#include "src/core/search/searchresult.h"
#include "src/core/search/searchquery.h"
#include "src/core/settings/settings.h"
#include <QtCore/QMap>
#include "src/core/bible/booknames.h"
#include "src/core/bible/book.h"
class BibleModule
{
public:
    BibleModule();
    virtual ~BibleModule() {}
    virtual void setSettings(Settings *settings);

    virtual void loadBibleData(const int &bibleID, const QString &path);
    virtual QString readInfo(QFile &file);
    virtual int readBook(const int &id);

    virtual void search(const SearchQuery &query, SearchResult *res) const;
    virtual bool hasIndex() const;
    virtual void buildIndex();

    virtual int bibleID() const ;
    virtual QString biblePath() const ;
    virtual QString bibleName(bool preferShortName = false) const ;
    virtual QMap<int, int> bookCount() const ;//11

    virtual BookNames getBookNames();
    virtual Book book() const;
    Settings *m_settings;
};

#endif // BIBLEMODULE_H
