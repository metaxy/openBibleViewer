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

#ifndef RANGE_H
#define RANGE_H
#include "src/core/verse/rangeenum.h"

#include <QtCore/QList>
class Range
{
public:
    Range();

    void setModule(const int moduleID);

    void setBook(const int bookID);
    void setBook(RangeEnum::BookRange book);

    void setChapter(const int chapterID);
    void setChapter(RangeEnum::ChapterRange chapter);

    void setStartVerse(const int verseID);
    void setStartVerse(RangeEnum::VerseRange verse);

    void setEndVerse(const int verseID);
    void setEndVerse(RangeEnum::VerseRange verse);

    void setSelectedVerse(const QList<int> &verseIDs);
    void setSelectedVerse(const int verseID);


    //void setSelectedVerse(RangeEnum::VerseRange verse);
    int moduleID() const;

    int bookID() const;
    RangeEnum::BookRange book() const;

    int chapterID() const;
    RangeEnum::ChapterRange chapter() const;

    int startVerseID() const;
    RangeEnum::VerseRange startVerse() const;
    int endVerseID() const;
    RangeEnum::VerseRange endVerse() const;

    QList<int> selectedVerse() const;

private:
    int m_startVerseID;
    RangeEnum::VerseRange m_startVerse;
    int m_endVerseID;
    RangeEnum::VerseRange m_endVerse;

    int m_chapterID;
    RangeEnum::ChapterRange m_chapter;

    int m_bookID;
    RangeEnum::BookRange m_book;
    QList<int> m_selectedVerse;

    int m_moduleID;



};

#endif // RANGE_H
