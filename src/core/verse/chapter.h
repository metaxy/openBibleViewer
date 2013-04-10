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
#ifndef CHAPTER_H
#define CHAPTER_H

#include <QString>
#include <QStringList>
#include <QMap>
#include "src/core/verse/verse.h"
/**
  * Chapter represents a verse chapter.
 */
class Chapter
{
public:
    Chapter();
    Chapter(const int chapterID);
    void addVerse(const Verse &verse);
    QMap<int, Verse> data() const;
    int verseCount() const;
    void setChapterID(const int chapterID);
    int chapterID() const;
private:
    QMap<int, Verse> m_data;
    int m_verseCount;
    int m_chapterID;

};

#endif // CHAPTER_H
