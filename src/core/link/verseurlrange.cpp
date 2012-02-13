/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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

    m_chapterID = -1;
    m_startVerseID = -1;
    m_endVerseID = -1;
    m_bookParam = LoadBookNotSet;
    m_chapterParam = LoadChapterNotSet;
    m_startVerseParam = LoadVerseNotSet;
    m_endVerseParam = LoadVerseNotSet;
    m_activeVerseID = -1;
    m_activeVerseParam = LoadVerseNotSet;
    m_openToTranformation = true;
}

void VerseUrlRange::setModule(const int moduleID)
{
    m_moduleID = moduleID;
    m_moduleParam = VerseUrlRange::LoadModuleByID;
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


int VerseUrlRange::moduleID() const
{
    return m_moduleID;
}

VerseUrlRange::ModuleLoadParams VerseUrlRange::module() const
{
    return m_moduleParam;
}

QString VerseUrlRange::moduleUID() const
{
    return m_moduleUID;
}

//Books

void VerseUrlRange::setBook(const int bookID)
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

void VerseUrlRange::setChapter(const int chapterID)
{
    m_chapterID = chapterID;
    m_chapterParam = VerseUrlRange::LoadChapterByID;
}

void VerseUrlRange::setChapter(const ChapterLoadParams &param)
{
    m_chapterParam = param;
}

int VerseUrlRange::chapterID() const
{
    return m_chapterID;
}

VerseUrlRange::ChapterLoadParams VerseUrlRange::chapter() const
{
    return m_chapterParam;
}

//Verse
void VerseUrlRange::setStartVerse(const int verseID)
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

void VerseUrlRange::setEndVerse(const int verseID)
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
void VerseUrlRange::setActiveVerse(const int verseID)
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

//todo: if url is invalid it returns always true
bool VerseUrlRange::containsChapter(const int chapterID) const
{
    bool ok = true;
    if(m_chapterParam == LoadChapterByID) {
        ok = ok && chapterID == m_chapterID;
    }
    return ok;
}

bool VerseUrlRange::containsVerse(const int verseID) const
{
    bool ok = true;
    if(m_startVerseParam == LoadVerseByID && m_startVerseID != -1) {
        if(m_endVerseParam == LoadVerseByID && m_endVerseID != -1) {
            ok = ok && verseID >= m_startVerseID;
        } else {
            ok = ok && verseID == m_startVerseID;
        }
    }
    if(m_endVerseParam == LoadVerseByID && m_endVerseID != -1) {
        ok = ok && verseID <= m_endVerseID;
    }
    return ok;
}
bool VerseUrlRange::openToTransformation() const
{
    return m_openToTranformation;
}
void VerseUrlRange::setOpenToTransformation(bool open)
{
    m_openToTranformation = open;
}
Range VerseUrlRange::toRange() const
{
    Range r;

    if(this->module() == VerseUrlRange::LoadModuleByID) {
        r.setModule(this->moduleID());
    }
    if(this->book() == VerseUrlRange::LoadFirstBook) {
        r.setBook(RangeEnum::FirstBook);
    } else if(this->book() == VerseUrlRange::LoadLastBook) {
        r.setBook(RangeEnum::LastBook);
    } else if(this->book() == VerseUrlRange::LoadCurrentBook) {
        r.setBook(RangeEnum::CurrentBook);
    } else {
        r.setBook(this->bookID());
    }

    if(this->chapter() == VerseUrlRange::LoadFirstChapter) {
        r.setChapter(RangeEnum::FirstChapter);
    } else if(this->chapter() == VerseUrlRange::LoadLastChapter) {
        r.setChapter(RangeEnum::LastChapter);
    } else if(this->chapter() == VerseUrlRange::LoadCurrentChapter) {
        r.setChapter(RangeEnum::CurrentChapter);
    } else {
        r.setChapter(this->chapterID());
    }

    if(this->openToTransformation()) {
        r.setStartVerse(RangeEnum::FirstVerse);
        r.setEndVerse(RangeEnum::LastVerse);
        if(this->startVerse() == this->endVerse() && this->startVerse() == VerseUrlRange::LoadVerseByID) {
            QList<int> sel;
            for(int i = this->startVerseID(); i <= this->endVerseID(); i++) {
                sel << i;
            }
            r.setSelectedVerse(sel);
        } else if(this->startVerse() == VerseUrlRange::LoadVerseByID) {
            r.setSelectedVerse(this->startVerseID());
        }
    } else {

        if(this->startVerse() == VerseUrlRange::LoadFirstVerse) {
            r.setStartVerse(RangeEnum::FirstVerse);
        } else if(this->startVerse() == VerseUrlRange::LoadLastVerse) {
            r.setStartVerse(RangeEnum::LastVerse);
        } else {
            r.setStartVerse(this->startVerseID());
        }

        if(this->endVerse() == VerseUrlRange::LoadFirstVerse) {
            r.setEndVerse(RangeEnum::FirstVerse);
        } else if(this->endVerse() == VerseUrlRange::LoadLastVerse) {
            r.setEndVerse(RangeEnum::LastVerse);
            if(this->activeVerse() == VerseUrlRange::LoadVerseByID) {
                r.setSelectedVerse(this->activeVerseID());
            }
        } else {
            r.setEndVerse(this->endVerseID());
        }
    }

    return r;
}
