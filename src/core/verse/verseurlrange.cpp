/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "verseurlrange.h"

VerseUrlRange::VerseUrlRange()
{
    m_moduleID = -1;
    m_moduleUID = "";
    m_bookID = -1;

    m_startChapterID = -1;
    m_endChapterID = -1;
    m_startVerseID = -1;
    m_endVerseID = -1;
    m_bookParam = LoadBookNotSet;
    m_startChapterParam = LoadChapterNotSet;
    m_endChapterParam = LoadChapterNotSet;
    m_startVerseParam = LoadVerseNotSet;
    m_endVerseParam = LoadVerseNotSet;
    m_activeVerseID = -1;
    m_activeVerseParam = LoadVerseNotSet;


}
void VerseUrlRange::setModule(const int &bibleID)
{
    m_moduleID = bibleID;
    m_moduleParam = VerseUrlRange::LoadBibleByID;
}

void VerseUrlRange::setModule(const ModuleLoadParams &param)
{
    m_moduleParam = param;
}
void VerseUrlRange::setModule(const QString &bibleUID)
{
    m_moduleUID = bibleUID;
    m_moduleParam = VerseUrlRange::LoadModuleByUID;
}
int VerseUrlRange::bibleID() const
{
    return m_moduleID;
}
VerseUrlRange::ModuleLoadParams VerseUrlRange::bible() const
{
    return m_moduleParam;
}
QString VerseUrlRange::bibleUID() const
{
    return m_moduleUID;
}
//Books

void VerseUrlRange::setBook(const int &bookID)
{
    m_bookID = bookID;
    m_bookParam = VerseUrlRange::LoadBookByID;
}

void VerseUrlRange::setBook(const BookLoadParams &param)
{
    m_bookParam = param;
}
int VerseUrlRange::bookID() const
{
    return m_bookID;
}
VerseUrlRange::BookLoadParams VerseUrlRange::book() const
{
    return m_bookParam;
}

//Chapter
void VerseUrlRange::setChapter(const int &chapterID)
{
    setStartChapter(chapterID);
    setEndChapter(chapterID);
}

void VerseUrlRange::setChapter(const ChapterLoadParams &param)
{
    setStartChapter(param);
    setEndChapter(param);
}

void VerseUrlRange::setStartChapter(const int &chapterID)
{
    m_startChapterID = chapterID;
    m_startChapterParam = VerseUrlRange::LoadChapterByID;
}

void VerseUrlRange::setStartChapter(const ChapterLoadParams &param)
{
    m_startChapterParam = param;
}
int VerseUrlRange::startChapterID() const
{
    return m_startChapterID;
}
VerseUrlRange::ChapterLoadParams VerseUrlRange::startChapter() const
{
    return m_startChapterParam;
}

void VerseUrlRange::setEndChapter(const int &chapterID)
{
    m_endChapterID = chapterID;
    m_endChapterParam = VerseUrlRange::LoadChapterByID;
}

void VerseUrlRange::setEndChapter(const ChapterLoadParams &param)
{
    m_endChapterParam = param;
}
int VerseUrlRange::endChapterID() const
{
    return m_endChapterID;
}
VerseUrlRange::ChapterLoadParams VerseUrlRange::endChapter() const
{
    return m_endChapterParam;
}
//Verse
void VerseUrlRange::setStartVerse(const int &verseID)
{
    m_startVerseID = verseID;
    m_startVerseParam = VerseUrlRange::LoadVerseByID;
}

void VerseUrlRange::setStartVerse(const VerseLoadParams &param)
{
    m_startVerseParam = param;
}
int VerseUrlRange::startVerseID() const
{
    return m_startVerseID;
}
VerseUrlRange::VerseLoadParams VerseUrlRange::startVerse() const
{
    return m_startVerseParam;
}

void VerseUrlRange::setEndVerse(const int &verseID)
{
    m_endVerseID = verseID;
    m_endVerseParam = VerseUrlRange::LoadVerseByID;
}

void VerseUrlRange::setEndVerse(const VerseLoadParams &param)
{
    m_endVerseParam = param;
}
int VerseUrlRange::endVerseID() const
{
    return m_endVerseID;
}
VerseUrlRange::VerseLoadParams VerseUrlRange::endVerse() const
{
    return m_endVerseParam;
}
void VerseUrlRange::setWholeChapter()
{
    setStartVerse(VerseUrlRange::LoadFirstVerse);
    setEndVerse(VerseUrlRange::LoadLastVerse);
}
//active Verse
void VerseUrlRange::setActiveVerse(const int &verseID)
{
    m_activeVerseID = verseID;
    m_activeVerseParam = VerseUrlRange::LoadVerseByID;
}
int VerseUrlRange::activeVerseID() const
{
    return m_activeVerseID;
}
VerseUrlRange::VerseLoadParams VerseUrlRange::activeVerse() const
{
    return m_activeVerseParam;
}
bool VerseUrlRange::containsChapter(const int &chapterID) const
{
    return (chapterID >= m_startChapterID && chapterID <= m_endChapterID);
}

bool VerseUrlRange::containsVerse(const int &verseID) const
{
    return (verseID >= m_startVerseID && verseID <= m_endVerseID);
}
