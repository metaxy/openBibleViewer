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
    DEBUG_FUNC_NAME
    foreach(Bible * b, m_bibles) {
        b->readBook(id);
    }
    return 0;
}
void BibleList::addBible(Bible* b, QPoint p)
{
    DEBUG_FUNC_NAME
    int id = m_biblePoints.size();
    m_currentBible = id;
    m_biblePoints[id] = p;
    m_bibles[id] = b;

}
Bible * BibleList::bible(const int &id)
{
    if(id == -1) {
        if(m_bibles.contains(m_currentBible)) {
            return m_bibles.value(m_currentBible);
        }
    } else {
        if(m_bibles.contains(id)) {
            return m_bibles.value(id);
        }
    }
    return 0;
}
void BibleList::clear()
{
    m_biblePoints.clear();
    m_bibles.clear();
    m_currentBible = 0;
}

QString BibleList::readChapter(int chapterID, int verseID)
{
    DEBUG_FUNC_NAME
    if(m_bibles.size() == 1) {
        return m_bibles[m_currentBible]->readChapter(chapterID, verseID);
    } else {
        foreach(Bible * b, m_bibles) {
            b->readChapter(chapterID, verseID);
        }
        int maxRow = 0;
        int maxCol = 0;
        foreach(QPoint p, m_biblePoints) {
            maxRow = qMax(maxRow, p.x());
            maxCol = qMax(maxCol, p.y());
        }
        QString out;


        if(maxCol >= 1) {
            out += "<table id='topTable'><tr>\n";
            for(int i = 0; i <= maxCol; i++) {
                int id = m_biblePoints.key(QPoint(0, i), -1);
                if(countInCol(i) > 1) {
                    out += "<td class=\"rowTitle\"></td>\n";
                }
                bool countInColEq = (countInCol(i) == 1);
                if(id != -1 && countInColEq) {
                    Bible *b = m_bibles.value(id);
                    QString active = "";
                    if(id == m_currentBible)
                        active = " active";
                    out += title(b, active, id);
                } else if(countInColEq) {
                    for(int j = 0; j <= maxRow; j++) {
                        id = m_biblePoints.key(QPoint(j, i), -1);
                        if(id != -1) {
                            Bible *b = m_bibles.value(id);
                            QString active = "";
                            if(id == m_currentBible)
                                active = " active";
                            out += title(b, active, id);
                            break;
                        }
                    }
                } else {
                    out += "<td></td>\n";
                }

            }
            out += "</tr></table>\n";
        }
        out += "<table class='biblelist'>\n";
        //for all verse
        out += "<tbody>";
        for(int verse = 0; verse < bible()->chapterDataList().size(); verse++) {
            for(int i = 0; i <= maxRow; i++) {
                out += "<tr>";
                for(int j = 0; j <= maxCol; j++) {
                    int id = m_biblePoints.key(QPoint(i, j), -1);
                    Bible *b;
                    if(id != -1) {
                        b = m_bibles.value(id);
                    } else {
                        out += "<td></td>\n";
                        continue;
                    }
                    if(countInCol(j) > 1) {
                        QString active = " rowTitle";
                        if(id == m_currentBible)
                            active = " active";
                        out += title(b, active, id);
                    }

                    out += "<td >" + b->chapterDataList().at(verse) + "</td>\n";
                }
                out += "</tr>\n";
            }
        }
        out += "</tbody>\n";
        out += "</table>\n";
        return out;
    }
}
int BibleList::countInCol(const int &col)
{
    QList<QPoint> list = m_biblePoints.values();
    int count = 0;
    foreach(QPoint p, list) {
        if(p.y() == col)
            count++;
    }
    return count;
}

QString BibleList::title(Bible *b, const QString &active, const int &bibleListID)
{
    return "<td class='bibleListTitle" + active + "' titleOf='" + QString::number(b->moduleID()) + "' bibleListID='" + QString::number(bibleListID) + "'><a href='#' onclick=\"Bible.activateBible('" + QString::number(bibleListID) + "');\"  class='bibleListTitleLink'>" + b->bibleShortTitle() + "</a></td>";
}
