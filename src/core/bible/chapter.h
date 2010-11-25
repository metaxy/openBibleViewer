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
#ifndef CHAPTER_H
#define CHAPTER_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include "src/core/bible/verse.h"
/**
 Chapter represents a bible chapter
*/
class Chapter
{
public:
    Chapter();
    Chapter(const int &chapterID);
    void addVerse(const int &verseID, const Verse &verse);
    QHash<int, Verse> getData();
    int verseCount();
    void setChapterID(const int &chapterID);
private:
    QHash<int, Verse> m_data;
    QStringList m_verseNumber;
    int m_verseCount;
    int m_chapterID;

};

#endif // CHAPTER_H
