#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H
#include "searchhit.h"
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGui/QListWidget>

class SearchResult
{
public:
    SearchResult();
    void addHit(SearchHit hit);
    void addHit(const int &bibleID, const int &bookID, const int &chapterID, const int &verseID, const QString &text);
    QList<SearchHit> hits();

private:
    QList<SearchHit> m_hits;

};

#endif // SEARCHRESULT_H
