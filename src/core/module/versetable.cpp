/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#include "src/core/module/response/textrangesresponse.h"
VerseTable::VerseTable()
{
    m_activeItem = 0;
    m_lastTextRanges = NULL;
}
VerseTable::~VerseTable()
{
    DEBUG_FUNC_NAME
    //deletes all VerseModules
    foreach(VerseModule * m, m_modules) {
        if(m) {
            delete m;
            m = nullptr;
        }
    }
}

void VerseTable::setActiveItem(const int verseTableID)
{
    m_activeItem = verseTableID;
    setLastTextRanges(m_lastTextRanges);
}

int VerseTable::activeItem() const
{
    return m_activeItem;
}

void VerseTable::addModule(TextRangesVerseModule* module, const QPoint &p)
{
    //if it contains already a module at point p
    //then delete the old and insert the new
    myDebug() << "before points = " << m_points << " modules = " << m_modules;
    if(m_points.values().contains(p)) {
        myDebug() << "already contains";
        const int id = m_points.key(p, -1);
        if(m_modules.contains(id) && m_modules.value(id) != NULL) {
            delete m_modules.value(id);
            m_modules.remove(id);
            m_points.remove(id);
        }
        //id or m_activeItem ???
        m_points.insert(id, p);
        m_modules.insert(id, module);
    } else {
        myDebug() << "new";

        int id = 0;
        foreach(int i, m_points.keys()) {
            id = qMax(id, i);
        }
        id++;

        m_activeItem = id;
        m_points.insert(id, p);
        m_modules.insert(id, module);
    }
    myDebug() << "after points = " << m_points << " modules = " << m_modules;
    setLastTextRanges(m_lastTextRanges);
}

TextRangesVerseModule * VerseTable::verseModule(const int id) const
{
    if(id == -1) {
        if(m_modules.contains(m_activeItem)) {
            return m_modules.value(m_activeItem);
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
    m_activeItem = 0;
}

std::pair<QString, TextRanges> VerseTable::readRanges(const Ranges &ranges) const
{
    //DEBUG_FUNC_NAME
    //myDebug() << "points = " << m_points << " modules = " << m_modules;
    if(m_modules.size() == 1) {
        std::pair<QString, TextRanges> ret;
        TextRangesVerseModule *b = m_modules.value(m_activeItem, NULL);
        if(b) {
            TextRangesResponse *res = (TextRangesResponse*)b->readRanges(ranges);
            ret.second = res->ranges();
            delete res;
            ret.second.setVerseTableID(0);
            if(ret.second.textRanges().size() == 1) {
                ret.first = ret.second.join("");
            } else {
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
        QHashIterator<int, TextRangesVerseModule *> i(m_modules);
        TextRanges def;
        TextRangesVerseModule *defModule = 0;
        TextRanges tret;

        while(i.hasNext()) {
            i.next();
            myDebug() << i.value()->moduleID();
            TextRangesResponse *res = (TextRangesResponse*) i.value()->readRanges(ranges, true);
            TextRanges r = res->ranges();
            delete res;
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
                    TextRangesVerseModule *m = m_modules.value(id);
                    QString active = "";
                    if(id == m_activeItem)
                        active = " active";
                    out += title(m, active, id);
                } else if(countInColEq) {
                    for(int j = 0; j <= maxRow; j++) {
                        id = m_points.key(QPoint(j, i), -1);
                        if(id != -1) {
                            TextRangesVerseModule *m = m_modules.value(id);
                            QString active = "";
                            if(id == m_activeItem)
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
                        TextRangesVerseModule *m = m_modules.value(id);
                        if(countInCol(j) > 1) {
                            QString active = " rowTitle";
                            if(id == m_activeItem)
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
    const QList<QPoint> list = m_points.values();
    int count = 0;
    foreach(const QPoint & p, list) {
        if(p.y() == col)
            count++;
    }
    return count;
}

QString VerseTable::title(TextRangesVerseModule *module, const QString &active, const int verseTableID) const
{
    return "<td class='verseTableTitle" + active + "' titleOf='" +
           QString::number(module->moduleID()) + "' verseTableID='" +
           QString::number(verseTableID) + "'><a href='#' onclick=\"Module.activateModule('" +
           QString::number(verseTableID) + "');\" class='verseTableTitleLink'>" + module->moduleShortTitle() + "</a><a href='#' onclick=\"Module.deleteModule('" +
            QString::number(verseTableID) + "');\" class='verseTableDeleteModule'><img src='qrc:/icons/16x16/kdenlive-deleffect.png' /></a></td>";
}

bool VerseTable::hasTopBar() const
{
    int maxCol = 0;
    foreach(const QPoint & p, m_points) {
        maxCol = qMax(maxCol, p.y());
    }
    return maxCol >= 1;
}

void VerseTable::clearData()
{
    foreach(TextRangesVerseModule *m, m_modules) {
        m->clearData();
    }
}
bool VerseTable::contains(const int moduleID)
{
    foreach(TextRangesVerseModule *m, m_modules) {
        if(m->moduleID() == moduleID)
            return true;
    }
    return false;
}
void VerseTable::setLastTextRanges(TextRanges *textRanges)
{
    if(textRanges == NULL)
        return;

    m_lastTextRanges = textRanges;

    foreach(TextRangesVerseModule * b, m_modules) {
        b->setLastTextRanges(textRanges);
    }
}
TextRanges *VerseTable::lastTextRanges()
{
    return m_lastTextRanges;
}
QPoint VerseTable::topRight() const
{
    myDebug() << "m_points" << m_points;
    int max = 0;
    foreach(const QPoint & p, m_points) {
        max = qMax(max, p.y());
    }
    myDebug() << "QPoint(0, max + 1)" << QPoint(0, max + 1);
    return QPoint(0, max + 1);
}

QPoint VerseTable::bottomLeft() const
{
    myDebug() << "m_points" << m_points;
    int max = 0;
    foreach(const QPoint & p, m_points) {
        max = qMax(max, p.x());
    }
    myDebug() << "QPoint(max + 1, 0)" << QPoint(max + 1, 0);
    return QPoint(max + 1, 0);

}
void VerseTable::removeModule(const int id)
{
    if(m_modules.contains(id) && m_modules.value(id) != NULL) {
        delete m_modules.value(id);
        m_modules.remove(id);
        m_points.remove(id);
        if(m_points.size() >= 1) {
             setActiveItem(m_points.keys().first());
        }
        removeUnneeded();
        //currently i think the reload is the problem
    }

}
void VerseTable::removeCol(int id)
{

    QMutableMapIterator<int, QPoint> i(m_points);
     while (i.hasNext()) {
         i.next();
         if(i.value().x() >= id) {
             i.value().setX(i.value().x() - 1);
         }
     }
}

void VerseTable::removeRow(int id)
{
    QMutableMapIterator<int, QPoint> i(m_points);
     while (i.hasNext()) {
         i.next();
         if(i.value().y() >= id) {
             i.value().setY(i.value().y() - 1);
         }
     }
}

void VerseTable::removeUnneededCols() //x
{
    myDebug() << "before m_points" << m_points;
    int max = 0;
    foreach(const QPoint & p, m_points) {
        max = qMax(max, p.x());
    }
    for(int i = 0; i<= max; i++) {
        bool contains = false;
        foreach(const QPoint & p, m_points) {
            if(p.x() == i) contains = true;
        }
        if(!contains) removeCol(i);
    }
    myDebug() << "after m_points" << m_points;
}
void VerseTable::removeUnneededRows() //y
{
    myDebug() << "before m_points" << m_points;
    int max = 0;
    foreach(const QPoint & p, m_points) {
        max = qMax(max, p.y());
    }
    for(int i = 0; i<= max; i++) {
        bool contains = false;
        foreach(const QPoint & p, m_points) {
            if(p.y() == i) contains = true;
        }
        if(!contains) removeRow(i);
    }
    myDebug() << "after m_points" << m_points;
}
void VerseTable::removeUnneeded()
{
    removeUnneededCols();
    removeUnneededRows();
}
