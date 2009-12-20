#include "searchresult.h"

SearchResult::SearchResult()
{
}
void SearchResult::addHit(SearchHit hit)
{
    m_hits << hit;
}
void SearchResult::addHit(const int &bibleID, const int &bookID, const int &chapterID, const int &verseID, const QString &text)
{
    SearchHit hit(bibleID, bookID, chapterID, verseID, text);
    m_hits << hit;
}
QList<SearchHit> SearchResult::hits()
{
    return m_hits;
}
