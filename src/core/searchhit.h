#ifndef SEARCHHIT_H
#define SEARCHHIT_H
#include <QtCore/QString>
class SearchHit
{
public:
    SearchHit(const int &bibleID, const int &bookID, const int &chapterID, const int &verseID, const QString &text);
    int bookID();
    int chapterID();
    int verseID();
    int bibleID();
    QString text();
private:
    int m_bookID;
    int m_chapterID;
    int m_verseID;
    int m_bibleID;
    QString m_text;
};

#endif // SEARCHHIT_H
