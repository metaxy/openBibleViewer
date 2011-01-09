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
#include "chapter.h"
Chapter::Chapter()
{
    m_chapterID = -1;
    m_verseCount = 0;
}
Chapter::Chapter(const int &chapterID)
{
    m_chapterID = chapterID;
    m_verseCount = 0;
}
void Chapter::addVerse(const int &verseID, const Verse &verse)
{
    m_verseCount++;
    m_data.insert(verseID, verse);
}
QHash<int, Verse> Chapter::getData() const
{
    return m_data;
}
int Chapter::verseCount() const
{
    return m_verseCount;
}
void Chapter::setChapterID(const int &chapterID)
{
    m_chapterID = chapterID;
}
int Chapter::chapterID() const
{
    return m_chapterID;
}
