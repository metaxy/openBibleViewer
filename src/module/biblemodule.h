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
    //virtual ~BibleModule();
    virtual void setSettings(Settings *settings); //1

    virtual void loadBibleData(const int &bibleID, const QString &path);//2
    virtual QString readInfo(QFile &file);//3
    virtual int readBook(const int &id);//4

    virtual void search(const SearchQuery &query, SearchResult *res);//5
    virtual bool hasIndex() const;//6
    virtual void buildIndex();//7

    virtual int bibleID() const ;//8
    virtual QString biblePath() const ;//9
    virtual QString bibleName(bool preferShortName = false) const ;//10
    virtual QMap<int, int> bookCount() const ;//11

    virtual BookNames getBookNames();//12
    virtual Book book() const;//13
    Settings *m_settings;
};

#endif // BIBLEMODULE_H
