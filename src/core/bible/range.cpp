#include "range.h"

Range::Range()
{
}

void Range::setBook(const int &bookID)
{
    m_bookID = bookID;
}

void Range::setBook(RangeEnum::BookRange book)
{
    m_book = book;
}

void Range::setChapter(const int &chapterID)
{
    m_chapterID = chapterID;
}

void Range::setChapter(RangeEnum::ChapterRange chapter)
{
    m_chapter = chapter;
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
int Range::bookID()
{
    return m_bookID;
}

RangeEnum::BookRange Range::book()
{
    return m_book;
}


int Range::chaperID()
{
    return m_chapterID;
}

RangeEnum::ChapterRange Range::chapter()
{
    return m_chapter;
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
