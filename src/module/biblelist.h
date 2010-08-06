#ifndef BIBLELIST_H
#define BIBLELIST_H
#include "src/module/bible.h"
#include <QPoint>
/**
  BibleList represents a list of bible to display them in one window. It is a some kind of parallel view
*/
class BibleList
{
public:
    BibleList();
    QHash<int, Bible *> m_bibles;
    QMap<int, QPoint> m_biblePoints;
    void clear();
    Bible * bible(const int &id = -1);
    void addBible(Bible* b, QPoint p);

    int readBook(int id);
    QString readChapter(int chapterID, int verseID);
    QString readVerse(int chapterID, int startVerse, int endVerse, int markVerseID, bool saveRawDatas);
    int m_currentBible;

private:
    QString title(Bible *b, const QString &active, const int &bibleListID);
    int countInCol(const int &col);

};

#endif // BIBLELIST_H
