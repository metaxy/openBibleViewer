#ifndef RANGE_H
#define RANGE_H
#include "src/core/bible/rangeenum.h"
class Range
{
public:
    Range();

    void setBook(const int &bookID);
    void setBook(RangeEnum::BookRange book);


    void setChapter(const int &chapterID);
    void setChapter(RangeEnum::ChapterRange chapter);

    void setStartVerse(const int &verseID);
    void setStartVerse(RangeEnum::VerseRange verse);
    void setEndVerse(const int &verseID);
    void setEndVerse(RangeEnum::VerseRange verse);

    int bookID();
    RangeEnum::BookRange book();

    int chapterID();
    RangeEnum::ChapterRange chapter();

    int startVerseID();
    RangeEnum::VerseRange startVerse();
    int endVerseID();
    RangeEnum::VerseRange endVerse();

private:
    int m_startVerseID;
    RangeEnum::VerseRange m_startVerse;
    int m_endVerseID;
    RangeEnum::VerseRange m_endVerse;

    int m_chapterID;
    RangeEnum::ChapterRange m_chapter;


    int m_bookID;
    RangeEnum::BookRange m_book;

};

#endif // RANGE_H
