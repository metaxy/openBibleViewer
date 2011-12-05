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
#include "book.h"
#include "src/core/dbghelper.h"
Book::Book()
{
    m_id = -1;
}
Book::Book(const int bookID)
{
    m_id = bookID;
}
int Book::size() const
{
    return m_chapters.size();
}
void Book::clear()
{
    m_id = -1;
    m_chapters.clear();
}
void Book::addChapter(const Chapter &c)
{
    m_chapters.insert(c.chapterID(), c);
}
bool Book::hasChapter(const int chapterID) const
{
    return m_chapters.contains(chapterID);
}
Chapter Book::getChapter(const int chapterID)
{
    return m_chapters.value(chapterID);
}
void Book::setID(const int id)
{
    m_id = id;
}
int Book::bookID() const
{
    return m_id;
}
QHash<int, Chapter> Book::data() const
{
    return m_chapters;
}
