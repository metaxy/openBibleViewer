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
    foreach(Bible* b,m_bibles) {
        b->readBook(id);
    }
    return 0;
}
void BibleList::addBible(Bible* b, QPoint p)
{
    int id = m_biblePoints.size();
    m_currentBible = id;
    m_biblePoints[id] = p;
    m_bibles[id] = b;

}
Bible * BibleList::bible()
{
    if(m_bibles.contains(m_currentBible)) {
        return m_bibles.value(m_currentBible);
    }
    return 0;
}

QString BibleList::readChapter(int chapterID, int verseID)
{
    QStringList a;
    if(m_bibles.size() == 1) {
        return m_bibles[m_currentBible]->readChapter(chapterID,verseID);
    } else {
        foreach(Bible* b,m_bibles) {
            b->readChapter(chapterID,verseID);
        }
        int maxRow = 0;
        int maxCol = 0;
        foreach(QPoint p, m_biblePoints) {
            maxRow = qMax(maxRow,p.y());
            maxCol = qMax(maxCol,p.x());
        }
        QString out = "<table>";
        //for all verse
        for(int verse = 0; verse < bible()->chapterDataList().size();verse++) {
            for(int i = 0; i < maxRow; i++) {
                out += "<tr>";
                for(int j = 0; j < maxCol;j++) {
                    out += "<td>";
                    int id = m_biblePoints.key(QPoint(i,j));
                    Bible *b = m_bibles.value(id);
                    out += b->chapterDataList().at(verse);
                    out += "</td>";
                }
                out += "</tr>";
            }
        }
        out += "</table>";
        return out;
     }
}
