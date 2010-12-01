/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
#include "range.h"

Range::Range()
{
    m_bookID = -1;
    m_chapterID = -1;
    m_startVerseID = -1;
    m_endVerseID = -1;
}

void Range::setBook(const int &bookID)
{
    m_bookID = bookID;
    m_book = RangeEnum::BookByID;
}

void Range::setBook(RangeEnum::BookRange book)
{
    m_book = book;
}

void Range::setChapter(const int &chapterID)
{
    m_chapterID = chapterID;
    m_chapter = RangeEnum::ChapterByID;
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
int Range::bookID() const
{
    return m_bookID;
}

RangeEnum::BookRange Range::book() const
{
    return m_book;
}


int Range::chapterID() const
{
    return m_chapterID;
}

RangeEnum::ChapterRange Range::chapter() const
{
    return m_chapter;
}


int Range::startVerseID() const
{
    return m_startVerseID;
}

RangeEnum::VerseRange Range::startVerse() const
{
    return m_startVerse;
}

int Range::endVerseID() const
{
    return m_endVerseID;
}

RangeEnum::VerseRange Range::endVerse() const
{
    return m_endVerse;
}
