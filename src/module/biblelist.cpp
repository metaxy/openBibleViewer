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
#include "biblelist.h"
#include "src/core/dbghelper.h"
BibleList::BibleList()
{
    m_currentBible = 0;
}
int BibleList::readBook(int id)
{
    foreach(Bible* b,m_bibleList) {
        b->readBook(id);
    }
}
void BibleList::addBible(Bible* b)
{
    m_currentBible = m_bibleList.size();
    m_bibleList << b;

}
Bible * BibleList::bible()
{
    myDebug() << m_currentBible << m_bibleList.size();
    if(m_currentBible < m_bibleList.size() && m_bibleList.size() > 0) {
        return m_bibleList.at(m_currentBible);
    }
    return 0;
}

QString BibleList::readChapter(int chapterID, int verseID)
{
    QStringList a;
    if(m_bibleList.size() == 1) {
        return m_bibleList.at(0)->readChapter(chapterID,verseID);
    } else {
        foreach(Bible* b,m_bibleList) {
            a << b->readChapter(chapterID,verseID);
        }
    }
    QString out = "<table>";
    foreach(QString b,a) {
        out += "<tr>"+b+"</tr>";
    }
    out +="</table";
    return out;
}

int BibleList::bookID()
{
    return m_bookID;
}
int BibleList::chapterID()
{
    return m_chapterID;
}
int BibleList::verseID()
{
    return m_verseID;
}
