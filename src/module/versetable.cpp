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
#include "versetable.h"
#include "src/core/dbghelper.h"

VerseTable::VerseTable()
{
    m_currentModule = 0;
    m_lastTextRanges = NULL;
    m_lastUrl = NULL;
}
VerseTable::~VerseTable()
{
    DEBUG_FUNC_NAME
    foreach(VerseModule * m, m_modules) {
        myDebug() << m;
        if(m) {
            delete m;
            m = NULL;
        }
    }
}

void VerseTable::setCurrentVerseTableID(const int verseTableID)
{
    //myDebug() << "before currentModule = " << m_currentModule << " new verseTable = " << verseTableID;
    m_currentModule = verseTableID;
    setLastTextRanges(m_lastTextRanges);
}

int VerseTable::currentVerseTableID() const
{
    return m_currentModule;
}

void VerseTable::addModule(VerseModule* m, const QPoint &p)
{
    //DEBUG_FUNC_NAME;
    //if it contains already a module with point p
    //then delete the old and insert the new
    //myDebug() << "p = " << p << " m = " << m;
    if(m_points.values().contains(p)) {
        const int id = m_points.key(p, -1);
        if(m_modules.contains(id) && m_modules.value(id) != NULL) {
            delete m_modules.value(id);
            m_modules.remove(id);
            m_points.remove(id);
        }
        m_points.insert(m_currentModule, p);
        m_modules.insert(m_currentModule, m);
    } else {
        const int id = m_points.size();
        m_currentModule = id;
        m_points.insert(id, p);
        m_modules.insert(id, m);
    }
    myDebug() << "points = " << m_points << " modules = " << m_modules;
    setLastTextRanges(m_lastTextRanges);
}

VerseModule * VerseTable::verseModule(const int id) const
{
    //myDebug() << "return verse module = " << id;
    if(id == -1) {
        if(m_modules.contains(m_currentModule)) {
            return m_modules.value(m_currentModule);
        }
    } else {
        if(m_modules.contains(id)) {
            return m_modules.value(id);
        }
    }
    return 0;
}
void VerseTable::clear()
{
    //DEBUG_FUNC_NAME
    m_points.clear();
    foreach(VerseModule * b, m_modules) {
        if(b) {
            delete b;
            b = 0;
        }
    }
    m_modules.clear();
    m_currentModule = 0;
}

std::pair<QString, TextRanges> VerseTable::readRanges(const Ranges &ranges) const
{
    //DEBUG_FUNC_NAME
    //myDebug() << "points = " << m_points << " modules = " << m_modules;
    if(m_modules.size() == 1) {
        std::pair<QString, TextRanges> ret;
        VerseModule *b = m_modules.value(m_currentModule, NULL);
        //myDebug() << b;
        if(b) {
            ret.second = b->readRanges(ranges);
            ret.second.setVerseTableID(0);
            if(ret.second.textRanges().size() == 1)
                ret.first = ret.second.join("");
            else {
                foreach(const TextRange & range, ret.second.textRanges()) {
                    //todo: use in future range.title()
                    ret.first += "<span class='rangeTitle'> " + b->versification()->bookName(range.bookID()) + " " + QString::number(range.chapterID() + 1) + "</span>\n";
                    ret.first += range.join("");
                }
            }
        }
        ret.second.setSource(ranges);
        return ret;
    } else if(m_modules.size() > 1) {
        std::pair<QString, TextRanges> ret;
        QHash<int, TextRanges> data;
        QHashIterator<int, VerseModule *> i(m_modules);
        TextRanges def;
        VerseModule *defModule = 0;
        TextRanges tret;

        while(i.hasNext()) {
            i.next();
            myDebug() << i.value()->moduleID();
            TextRanges r = i.value()->readRanges(ranges, true);
            r.setVerseTableID(i.key());

            data.insert(i.key(), r);
            def = r;
            defModule = i.value();
            tret.addTextRanges(r);
        }
        ret.second = tret;

        int maxRow = 0;
        int maxCol = 0;
        foreach(const QPoint & p, m_points) {
            maxRow = qMax(maxRow, p.x());
            maxCol = qMax(maxCol, p.y());
        }
        QString out;

        if(maxCol >= 1) {
            out += "<table id='topTable'><tr>\n";
            for(int i = 0; i <= maxCol; i++) {
                int id = m_points.key(QPoint(0, i), -1);
                if(countInCol(i) > 1) {
                    out += "<td class=\"rowTitle\"></td>\n";
                }
                bool countInColEq = (countInCol(i) == 1);
                if(id != -1 && countInColEq) {
                    VerseModule *m = m_modules.value(id);
                    QString active = "";
                    if(id == m_currentModule)
                        active = " active";
                    out += title(m, active, id);
                } else if(countInColEq) {
                    for(int j = 0; j <= maxRow; j++) {
                        id = m_points.key(QPoint(j, i), -1);
                        if(id != -1) {
                            VerseModule *m = m_modules.value(id);
                            QString active = "";
                            if(id == m_currentModule)
                                active = " active";
                            out += title(m, active, id);
                            break;
                        }
                    }
                } else {
                    out += "<td></td>\n";
                }

            }
            out += "</tr></table>\n";
        }
        out += "<table class='verseTable'>\n";
        //for all verse
        out += "<tbody>";
        int countTextRange = 0;
        foreach(const TextRange & textRange, def.textRanges()) {
            int rMaxCol = 0;
            for(int i = 0; i <= maxCol; i++) {
                rMaxCol++;
                if(countInCol(i) > 1) {
                    rMaxCol++;
                }
            }
            if(def.textRanges().size() != 1)
                out += "<tr><td align='center' class='rangeTitle' colspan='" + QString::number(rMaxCol) + "'>" + defModule->versification()->bookName(textRange.bookID()) + " " + QString::number(textRange.chapterID() + 1) + "</td></tr>\n";

            QMapIterator<int, Verse> mapIt(textRange.verseMap());
            while(mapIt.hasNext()) {
                mapIt.next();
                for(int i = 0; i <= maxRow; i++) {
                    out += "<tr>\n";
                    for(int j = 0; j <= maxCol; j++) {
                        const int id = m_points.key(QPoint(i, j));
                        if(!data.contains(id)) {
                            out += "<td></td>\n";
                            continue;
                        }
                        const TextRange r = data.value(id).textRanges().at(countTextRange);
                        VerseModule *m = m_modules.value(id);
                        if(countInCol(j) > 1) {
                            QString active = " rowTitle";
                            if(id == m_currentModule)
                                active += " active";
                            out += title(m, active, id);
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
        ret.second.setSource(ranges);
        return ret;
    } else {
        std::pair<QString, TextRanges> ret;
        ret.second.setSource(ranges);
        return ret;
    }
}
int VerseTable::countInCol(const int col) const
{
    QList<QPoint> list = m_points.values();
    int count = 0;
    foreach(const QPoint & p, list) {
        if(p.y() == col)
            count++;
    }
    return count;
}

QString VerseTable::title(VerseModule *module, const QString &active, const int verseTableID) const
{
    return "<td class='verseTableTitle" + active + "' titleOf='" +
           QString::number(module->moduleID()) + "' verseTableID='" +
           QString::number(verseTableID) + "'><a href='#' onclick=\"Module.activateModule('" +
           QString::number(verseTableID) + "');\" class='verseTableTitleLink'>" + module->moduleShortTitle() + "</a></td>";
}

bool VerseTable::hasTopBar() const
{
    int maxCol = 0;
    foreach(const QPoint & p, m_points) {
        maxCol = qMax(maxCol, p.y());
    }
    return maxCol >= 1;
}
void VerseTable::setLastTextRanges(TextRanges *textRanges)
{
    m_lastTextRanges = textRanges;
    foreach(VerseModule * b, m_modules) {
        b->setLastTextRanges(textRanges);
    }
}
TextRanges *VerseTable::lastTextRanges()
{
    return m_lastTextRanges;
}
void VerseTable::setLastUrl(VerseUrl *verseUrl)
{
    m_lastUrl = verseUrl;
}
VerseUrl *VerseTable::lastVerseUrl()
{
    return m_lastUrl;
}
