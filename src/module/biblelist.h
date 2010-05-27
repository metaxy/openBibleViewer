#ifndef BIBLELIST_H
#define BIBLELIST_H
#include "src/module/bible.h"
class BibleList
{
public:
    BibleList();
    QList<Bible *> m_bibleList;
    Bible * bible();
    void addBible(Bible* b);

    int readBook(int id);
    QString readChapter(int chapterID, int verseID);
    QString readVerse(int chapterID, int startVerse, int endVerse, int markVerseID, bool saveRawDatas);

    int bookID();
    int chapterID();
    int verseID();

private:
    int m_currentBible;
    int m_bookID;
    int m_chapterID;
    int m_verseID;

};

#endif // BIBLELIST_H
