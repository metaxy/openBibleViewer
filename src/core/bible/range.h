#ifndef RANGE_H
#define RANGE_H
#include "src/core/bible/rangeenum.h"
class Range
{
public:
    Range();

    void setStartBook(const int &bookID);
    void setStartBook(RangeEnum::BookRange book);
    void setEndBook(const int &bookID);
    void setEndBook(RangeEnum::BookRange book);
    //void setBook(const int &startID, const int &endID);


    void setStartChapter(const int &chapterID);
    void setStartChapter(RangeEnum::ChapterRange chapter);
    void setEndChapter(const int &chapterID);
    void setEndChapter(RangeEnum::ChapterRange chapter);


    void setStartVerse(const int &verseID);
    void setStartVerse(RangeEnum::VerseRange verse);
    void setEndVerse(const int &verseID);
    void setEndVerse(RangeEnum::VerseRange verse);

    int startBookID();
    RangeEnum::BookRange startBook();
    int endBookID();
    RangeEnum::BookRange endBook();

    int startChaperID();
    RangeEnum::ChapterRange startChapter();
    int endChapterID();
    RangeEnum::ChapterRange endChapter();

    int startVerseID();
    RangeEnum::VerseRange startVerse();
    int endVerseID();
    RangeEnum::VerseRange endVerse();

private:
    int m_startVerseID;
    RangeEnum::VerseRange m_startVerse;
    int m_endVerseID;
    RangeEnum::VerseRange m_endVerse;

    int m_startChapterID;
    RangeEnum::ChapterRange m_startChapter;
    int m_endChapterID;
    RangeEnum::ChapterRange m_endChapter;

    int m_startBookID;
    RangeEnum::BookRange m_startBook;
    int m_endBookID;
    RangeEnum::BookRange m_endBook;
};

#endif // RANGE_H
