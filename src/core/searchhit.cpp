#include "searchhit.h"

SearchHit::SearchHit(const int &bibleID, const int &bookID, const int &chapterID, const int &verseID, const QString &text)
{
    m_bibleID = bibleID;
    m_bookID = bookID;
    m_chapterID = chapterID;
    m_verseID = verseID;
    m_text = text;

}
int SearchHit::bibleID()
{
    return m_bibleID;
}
int SearchHit::bookID()
{
    return m_bookID;
}
int SearchHit::chapterID()
{
    return m_chapterID;
}
int SearchHit::verseID()
{
    return m_verseID;
}
QString SearchHit::text()
{
    return m_text;
}
