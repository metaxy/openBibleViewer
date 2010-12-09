#include "bibleurlrange.h"

BibleUrlRange::BibleUrlRange()
{
    m_bibleID = -1;
    m_startBookID = -1;
    m_endBookID = -1;
    m_startChapterID = -1;
    m_endChapterID = -1;
    m_startVerseID = -1;
    m_endVerseID = -1;

}
void BibleUrlRange::setBible(const int &bibleID)
{
    m_bibleID = bibleID;
    m_bibleParam = BibleUrlRange::LoadBibleByID;
}

void BibleUrlRange::setBible(const BibleLoadParams &param)
{
    m_bibleParam = param;
}
int BibleUrlRange::bibleID() const
{
    return m_bibleID;
}
BibleUrlRange::BibleLoadParams BibleUrlRange::bible() const
{
    return m_bibleParam;
}
//Books
void BibleUrlRange::setBook(const int &bookID)
{
    setStartBook(bookID);
    setEndBook(bookID);
}

void BibleUrlRange::setBook(const BookLoadParams &param)
{
    setStartBook(param);
    setEndBook(param);
}

void BibleUrlRange::setStartBook(const int &bookID)
{
    m_startBookID = bookID;
    m_startBookParam = BibleUrlRange::LoadBookByID;
}

void BibleUrlRange::setStartBook(const BookLoadParams &param)
{
    m_startBookParam = param;
}
int BibleUrlRange::startBookID() const
{
    return m_startBookID;
}
BibleUrlRange::BookLoadParams BibleUrlRange::startBook() const
{
    return m_startBookParam;
}

void BibleUrlRange::setEndBook(const int &bookID)
{
    m_endBookID = bookID;
    m_endBookParam = BibleUrlRange::LoadBookByID;
}

void BibleUrlRange::setEndBook(const BookLoadParams &param)
{
    m_endBookParam = param;
}
int BibleUrlRange::endBookID() const
{
    return m_endBookID;
}
BibleUrlRange::BookLoadParams BibleUrlRange::endBook() const
{
    return m_endBookParam;
}

//Chapter
void BibleUrlRange::setChapter(const int &chapterID)
{
    setStartChapter(chapterID);
    setEndChapter(chapterID);
}

void BibleUrlRange::setChapter(const ChapterLoadParams &param)
{
    setStartChapter(param);
    setEndChapter(param);
}

void BibleUrlRange::setStartChapter(const int &chapterID)
{
    m_startChapterID = chapterID;
    m_startChapterParam = BibleUrlRange::LoadChapterByID;
}

void BibleUrlRange::setStartChapter(const ChapterLoadParams &param)
{
    m_startChapterParam = param;
}
int BibleUrlRange::startChapterID() const
{
    return m_startChapterID;
}
BibleUrlRange::ChapterLoadParams BibleUrlRange::startChapter() const
{
    return m_startChapterParam;
}

void BibleUrlRange::setEndChapter(const int &chapterID)
{
    m_endChapterID = chapterID;
    m_endChapterParam = BibleUrlRange::LoadChapterByID;
}

void BibleUrlRange::setEndChapter(const ChapterLoadParams &param)
{
    m_endChapterParam = param;
}
int BibleUrlRange::endChapterID() const
{
    return m_endChapterID;
}
BibleUrlRange::ChapterLoadParams BibleUrlRange::endChapter() const
{
    return m_endChapterParam;
}
//Verse
void BibleUrlRange::setStartVerse(const int &verseID)
{
    m_startVerseID = verseID;
    m_startVerseParam = BibleUrlRange::LoadVerseByID;
}

void BibleUrlRange::setStartVerse(const VerseLoadParams &param)
{
    m_startVerseParam = param;
}
int BibleUrlRange::startVerseID() const
{
    return m_startVerseID;
}
BibleUrlRange::VerseLoadParams BibleUrlRange::startVerse() const
{
    return m_startVerseParam;
}

void BibleUrlRange::setEndVerse(const int &verseID)
{
    m_endVerseID = verseID;
    m_endVerseParam = BibleUrlRange::LoadVerseByID;
}

void BibleUrlRange::setEndVerse(const VerseLoadParams &param)
{
    m_endVerseParam = param;

}
int BibleUrlRange::endVerseID() const
{
    return m_endVerseID;
}
BibleUrlRange::VerseLoadParams BibleUrlRange::endVerse() const
{
    return m_endVerseParam;
}
void BibleUrlRange::setWholeChapter()
{
    setStartVerse(BibleUrlRange::LoadFirstVerse);
    setEndVerse(BibleUrlRange::LoadLastVerse);
}
