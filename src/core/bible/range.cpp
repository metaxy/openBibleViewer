#include "range.h"

Range::Range()
{
}

void Range::setStartBook(const int &bookID)
{
    m_startBookID = bookID;
}

void Range::setStartBook(RangeEnum::BookRange book)
{
    m_startBook = book;
}

void Range::setEndBook(const int &bookID)
{
    m_endBookID = bookID;
}

void Range::setEndBook(RangeEnum::BookRange book)
{
    m_endBook = book;
}

void Range::setStartChapter(const int &chapterID)
{
    m_startChapterID = chapterID;
}

void Range::setStartChapter(RangeEnum::ChapterRange chapter)
{
    m_startChapter = chapter;
}

void Range::setEndChapter(const int &chapterID)
{
    m_endChapterID = chapterID;
}

void Range::setEndChapter(RangeEnum::ChapterRange chapter)
{
    m_endChapter = chapter;
}

void Range::setStartVerse(const int &verseID)
{
    m_startVerseID = verseID;
}

void Range::setStartVerse(RangeEnum::VerseRange verse)
{
    m_startVerse = verse;
}

void Range::setEndVerse(const int &verseID)
{
    m_endVerseID = verseID;
}

void Range::setEndVerse(RangeEnum::VerseRange verse)
{
    m_endVerse = verse;
}
int Range::startBookID()
{
    return m_startBookID;
}

RangeEnum::BookRange Range::startBook()
{
    return m_startBook;
}

int Range::endBookID()
{
    return m_endBookID;
}

RangeEnum::BookRange Range::endBook()
{
    return m_endBook;
}

int Range::startChaperID()
{
    return m_startChapterID;
}

RangeEnum::ChapterRange Range::startChapter()
{
    return m_startChapter;
}

int Range::endChapterID()
{
    return m_endChapterID;
}

RangeEnum::ChapterRange Range::endChapter()
{
    return m_endChapter;
}

int Range::startVerseID()
{
    return m_startVerseID;
}

RangeEnum::VerseRange Range::startVerse()
{
    return m_startVerse;
}

int Range::endVerseID()
{
    return m_endVerseID;
}

RangeEnum::VerseRange Range::endVerse()
{
    return m_endVerse;
}
