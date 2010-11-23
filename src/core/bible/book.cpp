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
#include "book.h"
#include "src/core/dbghelper.h"
Book::Book()
{
}
int Book::size()
{
    return m_chapters.size();
}
void Book::clear()
{
    m_chapters.clear();
}
void Book::addChapter(const int &chapterID, const Chapter &c)
{
    m_chapters.insert(chapterID, c);
}
bool Book::hasChapter(const int &chapterID)
{
    return m_chapters.contains(chapterID);
}
Chapter Book::getChapter(const int &chapterID)
{
    return m_chapters.value(chapterID);
}

void Book::setID(const int &id)
{
    m_id = id;
}

int Book::bookID()
{
    return m_id;
}
