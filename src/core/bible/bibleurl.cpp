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
#include "bibleurl.h"
#include "src/core/dbghelper.h"
#include <QtCore/QHashIterator>
#include <QtCore/QStringList>
BibleUrl::BibleUrl()
{

}
BibleUrl::BibleUrl(const BibleUrlRange &range)
{
    addRange(range);
}

void BibleUrl::setParam(const QString &name, const QString &value)
{
    //check if name or value contains / or , or =
    m_params.insert(name, value);
}

bool BibleUrl::hasParam(const QString &name) const
{
    return m_params.contains(name);
}
QString BibleUrl::getParam(const QString &name) const
{
    return m_params.value(name, "");
}
void BibleUrl::addRange(const BibleUrlRange &range)
{
    m_ranges.append(range);
}
void BibleUrl::addRanges(const QList<BibleUrlRange> &ranges)
{
    m_ranges.append(ranges);
}

QString BibleUrl::toString() const
{
    QString ret = "";
    int c = 0;
    foreach(const BibleUrlRange range, m_ranges) {
        if(!ret.isEmpty())
            ret += "|";//seperator
        else
            ret += "bible://";

        if(range.bible() == BibleUrlRange::LoadBibleByID) {
            ret += QString::number(range.bibleID());
        } else if(range.bible() == BibleUrlRange::LoadCurrentBible) {
            ret += "current";
        } else if(range.bible() == BibleUrlRange::LoadBibleByUID) {
            ret += "uid=" + range.bibleUID();
        }
        ret += ",";
        //Book
        if(range.book() == BibleUrlRange::LoadBookByID) {
            ret += QString::number(range.bookID());
        } else if(range.book() == BibleUrlRange::LoadCurrentBook) {
            ret += "current";
        } else if(range.book() == BibleUrlRange::LoadFirstBook) {
            ret += "first";
        } else if(range.book() == BibleUrlRange::LoadLastBook) {
            ret += "last";
        }

        ret += ",";

        //Chapter
        if(range.startChapter() == BibleUrlRange::LoadChapterByID) {
            ret += QString::number(range.startChapterID());
        } else if(range.startChapter() == BibleUrlRange::LoadCurrentChapter) {
            ret += "current";
        } else if(range.startChapter() == BibleUrlRange::LoadFirstChapter) {
            ret += "first";
        } else if(range.startChapter() == BibleUrlRange::LoadLastChapter) {
            ret += "last";
        }
        if((range.startChapter() != range.endChapter() && range.endChapter() != BibleUrlRange::LoadChapterNotSet) ||
                (range.startChapter() == range.endChapter() && range.startChapter() == BibleUrlRange::LoadChapterByID && range.startChapterID() != range.endChapterID() && range.endChapterID() != -1)) {
            ret += "-";
            if(range.endChapter() == BibleUrlRange::LoadChapterByID) {
                ret += QString::number(range.endChapterID());
            } else if(range.endChapter() == BibleUrlRange::LoadCurrentChapter) {
                ret += "current";
            } else if(range.endChapter() == BibleUrlRange::LoadFirstChapter) {
                ret += "first";
            } else if(range.endChapter() == BibleUrlRange::LoadLastChapter) {
                ret += "last";
            }
        }
        ret += ",";
        //Verse
        if(range.startVerse() == BibleUrlRange::LoadVerseByID) {
            ret += QString::number(range.startVerseID());
        } else if(range.startVerse() == BibleUrlRange::LoadCurrentVerse) {
            ret += "current";
        } else if(range.startVerse() == BibleUrlRange::LoadFirstVerse) {
            ret += "first";
        } else if(range.startVerse() == BibleUrlRange::LoadLastVerse) {
            ret += "last";
        }
        if((range.startVerse() != range.endVerse() && range.endVerse() != BibleUrlRange::LoadVerseNotSet) ||
                (range.startVerse() == range.endVerse() && range.startVerse() == BibleUrlRange::LoadVerseByID && range.startVerseID() != range.endVerseID() && range.endVerseID() != -1)) {
            ret += "-";
            if(range.endVerse() == BibleUrlRange::LoadVerseByID) {
                ret += QString::number(range.endVerseID());
            } else if(range.endVerse() == BibleUrlRange::LoadCurrentVerse) {
                ret += "current";
            } else if(range.endVerse() == BibleUrlRange::LoadFirstVerse) {
                ret += "first";
            } else if(range.endVerse() == BibleUrlRange::LoadLastVerse) {
                ret += "last";
            }
        }
        if(range.activeVerse() == BibleUrlRange::LoadVerseByID) {
            ret += ",active=" + QString::number(range.activeVerseID());
        }
        if(!m_params.isEmpty()) {
            QHashIterator<QString, QString> i(m_params);
            while(i.hasNext()) {
                i.next();
                ret += "," + i.key() + "=" + i.value();
            }
        }

    }

    return ret;
}
//todo: reads currently only one range
bool BibleUrl::fromString(QString url)
{
    m_ranges.clear();

    //bible://bibleID,bookID,chapterID,verseID,otherStuf=otherValue
    if(!url.startsWith("bible://")) {
        return false;
    }
    url.remove(0, 8); // remove bible://
    QStringList urls = url.split("|");
    foreach(const QString nUrl, urls) {
        BibleUrlRange range;
        const QStringList params = nUrl.split(",");
        for(int i = 0; i < params.size(); ++i) {
            QString p = params.at(i);
            QString p2;
            if(i == 0) {//Bible
                if(p == "current") {
                    range.setBible(BibleUrlRange::LoadCurrentBible);
                } else if(p.startsWith("uid=")) {
                    range.setBible(p.remove(0,4));
                } else {
                    range.setBible(p.toInt());
                }
            } else if(i == 1) {//Book
                if(p == "current") {
                    range.setBook(BibleUrlRange::LoadCurrentBook);
                } else if(p == "first") {
                    range.setBook(BibleUrlRange::LoadFirstBook);
                } else if(p == "last") {
                    range.setBook(BibleUrlRange::LoadLastBook);
                } else {
                    range.setBook(p.toInt());
                }

            } else if(i == 2) {
                if(p.contains("-")) {
                    QStringList tmp = p.split("-");
                    p = tmp.first();
                    p2 = tmp.last();
                }
                if(p == "current") {
                    range.setStartChapter(BibleUrlRange::LoadCurrentChapter);
                } else if(p == "first") {
                    range.setStartChapter(BibleUrlRange::LoadFirstChapter);
                } else if(p == "last") {
                    range.setStartChapter(BibleUrlRange::LoadLastChapter);
                } else {
                    range.setStartChapter(p.toInt());
                }
                if(p2 == "current") {
                    range.setEndChapter(BibleUrlRange::LoadCurrentChapter);
                } else if(p2 == "first") {
                    range.setEndChapter(BibleUrlRange::LoadFirstChapter);
                } else if(p2 == "last") {
                    range.setEndChapter(BibleUrlRange::LoadLastChapter);
                } else if(!p2.isEmpty()) {
                    range.setEndChapter(p2.toInt());
                }
            } else if(i == 3) {
                if(p.contains("-")) {
                    QStringList tmp = p.split("-");
                    p = tmp.first();
                    p2 = tmp.last();
                }
                if(p == "current") {
                    range.setStartVerse(BibleUrlRange::LoadCurrentVerse);
                } else if(p == "first") {
                    range.setStartVerse(BibleUrlRange::LoadFirstVerse);
                } else if(p == "last") {
                    range.setStartVerse(BibleUrlRange::LoadLastVerse);
                } else {
                    range.setStartVerse(p.toInt());
                }

                if(p2 == "current") {
                    range.setEndVerse(BibleUrlRange::LoadCurrentVerse);
                } else if(p2 == "first") {
                    range.setEndVerse(BibleUrlRange::LoadFirstVerse);
                } else if(p2 == "last") {
                    range.setEndVerse(BibleUrlRange::LoadLastVerse);
                } else if(!p2.isEmpty()) {
                    range.setEndVerse(p2.toInt());
                }
            } else {
                if(p.contains("=")) {
                    const QStringList s = p.split("=");
                    const QString key = s.first();
                    const QString value = s.last();
                    if(key == "active") {
                        range.setActiveVerse(value.toInt());
                    } else {
                        m_params.insert(key, value);
                    }
                } else {
                    m_params.insert(p, "true");
                }
            }
        }
        m_ranges.append(range);
    }
    return true;
}
QList<BibleUrlRange> BibleUrl::ranges() const
{
    return m_ranges;
}
void BibleUrl::clearRanges()
{
    m_ranges.clear();
}
void BibleUrl::unsetParam(const QString &name)
{
    m_params.remove(name);
}
bool BibleUrl::contains(const int &moduleID, const int &bookID, const int &chapterID, const int &verseID) const
{
    foreach(const BibleUrlRange &range, m_ranges) {
        if(range.bibleID() == moduleID && range.bookID() == bookID && range.containsChapter(chapterID) && range.containsVerse(verseID))
            return true;
    }
    return false;
}
bool BibleUrl::contains(const int &moduleID, const int &bookID, const int &chapterID) const
{
    foreach(const BibleUrlRange &range, m_ranges) {
        if(range.bibleID() == moduleID && range.bookID() == bookID && range.containsChapter(chapterID))
            return true;
    }
    return false;
}
