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
#include "versebook.h"
#include "src/core/dbghelper.h"
VerseBook::VerseBook()
{
    m_id = -1;
}
VerseBook::VerseBook(const int bookID)
{
    m_id = bookID;
}
int VerseBook::size() const
{
    return m_chapters.size();
}
void VerseBook::clear()
{
    m_id = -1;
    m_chapters.clear();
}
void VerseBook::addChapter(const Chapter &c)
{
    m_chapters.insert(c.chapterID(), c);
}
bool VerseBook::hasChapter(const int chapterID) const
{
    return m_chapters.contains(chapterID);
}
Chapter VerseBook::getChapter(const int chapterID)
{
    return m_chapters.value(chapterID);
}
void VerseBook::setID(const int id)
{
    m_id = id;
}
int VerseBook::bookID() const
{
    return m_id;
}
QHash<int, Chapter> VerseBook::data() const
{
    return m_chapters;
}
