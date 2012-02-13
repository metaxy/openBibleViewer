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
#include "versemodule.h"

VerseModule::VerseModule()
{
}

VerseModule::~VerseModule()
{

}

QSharedPointer<Versification> VerseModule::versification() const
{
    return m_versification;
}

void VerseModule::setModuleDisplaySettings(ModuleDisplaySettings *moduleDisplaySettings)
{
    m_moduleDisplaySettings = moduleDisplaySettings;
}
CompiledRange VerseModule::toCompiledRange(const Range &range)
{
    CompiledRange ret;
    int bookID = -1;
    if(range.book() == RangeEnum::BookByID) {
        bookID = range.bookID();
    } else if(range.book() == RangeEnum::FirstBook) {
        foreach(int id, bookIDs()) {
            if(id < bookID || bookID == -1) {
                bookID = id;
            }
        }
        if(bookID == -1)
            bookID = 0;
    } else if(range.book() == RangeEnum::LastBook) {
        foreach(int id, bookIDs()) {
            if(id > bookID || bookID == -1) {
                bookID = id;
            }
        }
        if(bookID == -1)
            bookID = 0;
    } else if(range.book() == RangeEnum::CurrentBook) {
        bookID = this->currentBook();
        if(bookID == -1) {
            foreach(int id, bookIDs()) {
                if(id < bookID || bookID == -1) {
                    bookID = id;
                }
            }
            if(bookID == -1)
                bookID = 0;
        }

    }

    if(bookID < 0) {
        myWarning() << "index out of range index bookID = " << bookID;
        ret.error = 1;
        return ret;
    }

    int chapterID = -1;
    if(range.chapter() == RangeEnum::ChapterByID) {
        chapterID = range.chapterID();
    } else if(range.chapter() == RangeEnum::FirstChapter) {
        chapterID = 0;
    } else if(range.chapter() == RangeEnum::LastChapter) {
        chapterID = m_versification->maxChapter().value(bookID) - 1;
    } else if(range.chapter() == RangeEnum::CurrentChapter) {
        chapterID = this->currentChapter();
        if(chapterID == -1) {
            chapterID = 0;
        }
    }
    std::pair<int, int> minMax = this->minMaxVerse(bookID, chapterID);

    int startVerse = 0;
    int endVerse = 0;

    if(range.startVerse() == RangeEnum::VerseByID) {
        startVerse = range.startVerseID();
    } else if(range.startVerse() == RangeEnum::FirstVerse) {
        startVerse = minMax.first;
    } else if(range.startVerse() == RangeEnum::LastVerse) {
        startVerse = minMax.second;
    }

    if(range.endVerse() == RangeEnum::VerseByID) {
        endVerse = range.endVerseID();
    } else if(range.endVerse() == RangeEnum::FirstVerse) {
        endVerse = minMax.first;
    } else if(range.endVerse() == RangeEnum::LastVerse) {
        endVerse = minMax.second;
    }
    if(endVerse == -1) {
        endVerse = startVerse;
    }
    ret.bookID = bookID,
    ret.chapterID = chapterID;
    ret.startVerse = startVerse;
    ret.endVerse = endVerse;

    return ret;
}

QList<int> VerseModule::bookIDs() const
{
    return m_versification->bookNames().keys();
}
