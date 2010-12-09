#include "bibleurlrange.h"

BibleUrlRange::BibleUrlRange()
{
}
void BibleUrlRange::setBible(const int &bibleID)
{
    m_bibleID = bibleID;
}

void BibleUrlRange::setBible(const BibleLoadParams &param)
{
    m_bibleParam = param;
}
int BibleUrlRange::bibleID()
{
    return m_bibleID;
}
BibleUrlRange::BibleLoadParams BibleUrlRange::bible()
{
    return m_bibleParam;
}


void BibleUrlRange::setStartBook(const int &bookID)
{
    m_startBookID = bookID;
}

void BibleUrlRange::setStartBook(const BookLoadParams &param)
{
    m_startBookParam = param;
}
int BibleUrlRange::startBookID()
{
    return m_startBookID;
}
BibleUrlRange::BookLoadParams BibleUrlRange::startBook()
{
    return m_startBookParam;
}

void BibleUrlRange::setEndBookID(const int &bookID)
{
    m_endBookID = bookID;
}

void BibleUrlRange::setEndBook(const BookLoadParams &param)
{
    m_endBookParam = param;
}
int BibleUrlRange::endBookID()
{
    return m_endBookID;
}
BibleUrlRange::BookLoadParams BibleUrlRange::endBook()
{
    return m_endBookParam;
}

//Chapter
void BibleUrlRange::setStartChapter(const int &chapterID)
{
    m_startChapterID = chapterID;
}

void BibleUrlRange::setStartChapter(const ChapterLoadParams &param)
{
    m_startChapterParam = param;
}
int BibleUrlRange::startChapterID()
{
    return m_startChapterID;
}
BibleUrlRange::ChapterLoadParams BibleUrlRange::startChapter()
{
    return m_startChapterParam;
}

void BibleUrlRange::setEndChapterID(const int chapterID)
{
    m_endChapterID = chapterID;
}

void BibleUrlRange::setEndChapter(const ChapterLoadParams &param)
{
    m_endChapterParam = param;
}
int BibleUrlRange::endChapterID()
{
    return m_endChapterID;
}
BibleUrlRange::ChapterLoadParams BibleUrlRange::endChapter()
{
    return m_endChapterParam;
}
//Verse
void BibleUrlRange::setStartVerse(const int &verseID)
{
    m_startVerseID = verseID;
}

void BibleUrlRange::setStartVerse(const VerseLoadParams &param)
{
    m_startVerseParam = param;
}
int BibleUrlRange::startVerseID()
{
    return m_startVerseID;
}
BibleUrlRange::VerseLoadParams BibleUrlRange::startVerse()
{
    return m_startVerseParam;
}

void BibleUrlRange::setEndVerseID(const int &verseID)
{
    m_endVerseID = verseID;
}

void BibleUrlRange::setEndVerse(const VerseLoadParams &param)
{
    m_endVerseParam = param;
}
int BibleUrlRange::endVerseID()
{
    return m_endVerseID;
}
BibleUrlRange::VerseLoadParams BibleUrlRange::endVerse()
{
    return m_endVerseParam;
}
