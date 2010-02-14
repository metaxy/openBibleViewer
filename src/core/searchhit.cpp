/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#include "searchhit.h"

SearchHit::SearchHit(const int &bibleID, const int &bookID, const int &chapterID, const int &verseID, const QString &text)
{
    m_bibleID = bibleID;
    m_bookID = bookID;
    m_chapterID = chapterID;
    m_verseID = verseID;
    m_text = text;

}
int SearchHit::bibleID()
{
    return m_bibleID;
}
int SearchHit::bookID()
{
    return m_bookID;
}
int SearchHit::chapterID()
{
    return m_chapterID;
}
int SearchHit::verseID()
{
    return m_verseID;
}
QString SearchHit::text()
{
    return m_text;
}
