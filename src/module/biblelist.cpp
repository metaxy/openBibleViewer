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
BibleList::~BibleList()
{
    foreach(Bible * b, m_bibles) {
        if(b) {
            delete b;
            b = 0;
        }
    }
}

int BibleList::readBook(int id)
{
    foreach(Bible * b, m_bibles) {
        b->readBook(id);
    }
    return 0;
}
void BibleList::setCurrentBibleListID(const int &bibleListID)
{
    DEBUG_FUNC_NAME;
    m_currentBible = bibleListID;
    setLastTextRanges(m_lastTextRanges);
}
int BibleList::currentBibleListID() const
{
    return m_currentBible;
}

void BibleList::addBible(Bible* b, const QPoint &p)
{
    DEBUG_FUNC_NAME;
    const int id = m_biblePoints.size();
    m_currentBible = id;
    m_biblePoints.insert(id, p);
    m_bibles.insert(id, b);
    setLastTextRanges(m_lastTextRanges);

}

Bible * BibleList::bible(const int &id) const
{
    if(id == -1) {
        if(m_bibles.contains(m_currentBible)) {
            //myDebug() << "return bible with " << m_currentBible;
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
    foreach(Bible * b, m_bibles) {
        if(b) {
            delete b;
            b = 0;
        }
    }
    m_bibles.clear();
    m_currentBible = 0;
}

QString BibleList::readChapter(const int &chapterID, const int &verseID) const
{
    //DEBUG_FUNC_NAME
    Ranges ranges;
    Range f;
    myDebug() << "bookID = " << bible()->bookID() << " chapterID = " << chapterID;
    f.setBook(bible()->bookID());
    f.setChapter(chapterID);
    f.setStartVerse(RangeEnum::FirstVerse);
    f.setEndVerse(RangeEnum::LastVerse);
    f.setSelectedVerse(verseID);
    ranges.addRange(f);
    return readRanges(ranges).first;
}
std::pair<QString, TextRanges> BibleList::readRanges(const Ranges &ranges) const
{
    //DEBUG_FUNC_NAME

    if(m_bibles.size() == 1) {
        std::pair<QString, TextRanges> ret;
        ret.second = m_bibles[m_currentBible]->readRanges(ranges);
        ret.first = ret.second.join("");
        return ret;
    } else if(m_bibles.size() > 1) {
        std::pair<QString, TextRanges> ret;

        //myDebug() << "oh a real biblelist";
        QHash<int, TextRanges> data;
        QHashIterator<int, Bible *> i(m_bibles);
        TextRanges def;
        TextRanges tret;

        while(i.hasNext()) {
            i.next();
            myDebug() << "read = " << i.value()->moduleID();
            const TextRanges r = i.value()->readRanges(ranges, true);
            data.insert(i.key(), r);
            def = r;
            tret.addTextRanges(r);
        }
        ret.second = tret;

        int maxRow = 0;
        int maxCol = 0;
        foreach(const QPoint & p, m_biblePoints) {
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
        int countTextRange = 0;
        foreach(const TextRange & textRange, def.textRanges()) {
            //todo: its a new range
            QMapIterator<int, Verse> mapIt(textRange.verseMap());
            while(mapIt.hasNext()) {
                mapIt.next();
                for(int i = 0; i <= maxRow; i++) {
                    out += "<tr>\n";
                    for(int j = 0; j <= maxCol; j++) {
                        const int id = m_biblePoints.key(QPoint(i, j));
                        if(!data.contains(id)) {
                            out += "<td></td>\n";
                            continue;
                        }
                        const TextRange r = data.value(id).textRanges().at(countTextRange);
                        Bible *b = m_bibles.value(id);
                        if(countInCol(j) > 1) {
                            QString active = " rowTitle";
                            if(id == m_currentBible)
                                active += " active";
                            out += title(b, active, id);
                        }
                        //todo: what todo if there ist no verse ?
                        out += "<td >" + r.getVerse(mapIt.key()).data() + "</td>\n";
                    }
                    out += "</tr>\n";
                }
            }
            countTextRange++;
        }

        out += "</tbody>\n</table>\n";
        ret.first = out;
        return ret;
    } else {
        std::pair<QString, TextRanges> ret;
        return ret;
    }
}
int BibleList::countInCol(const int &col) const
{
    QList<QPoint> list = m_biblePoints.values();
    int count = 0;
    foreach(const QPoint & p, list) {
        if(p.y() == col)
            count++;
    }
    return count;
}

QString BibleList::title(Bible *b, const QString &active, const int &bibleListID) const
{
    return "<td class='bibleListTitle" + active + "' titleOf='" +
           QString::number(b->moduleID()) + "' bibleListID='" +
           QString::number(bibleListID) + "'><a href='#' onclick=\"Bible.activateBible('" +
           QString::number(bibleListID) + "');\" class='bibleListTitleLink'>" + b->bibleShortTitle() + "</a></td>";
}

bool BibleList::hasTopBar() const
{
    int maxCol = 0;
    foreach(const QPoint & p, m_biblePoints) {
        maxCol = qMax(maxCol, p.y());
    }
    return maxCol >= 1;
}
void BibleList::setLastTextRanges(TextRanges *textRanges)
{
    m_lastTextRanges = textRanges;
    foreach(Bible * b, m_bibles) {
        b->setLastTextRanges(textRanges);
    }
}
TextRanges *BibleList::lastTextRanges()
{
    return m_lastTextRanges;
}
void BibleList::setLastUrl(BibleUrl *bibleUrl)
{
    m_lastUrl = bibleUrl;
}
BibleUrl *BibleList::lastBibleUrl()
{
    return m_lastUrl;
}
