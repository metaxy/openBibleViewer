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
#include "verseurl.h"
#include "src/core/dbghelper.h"
#include <QtCore/QHashIterator>
#include <QtCore/QStringList>
VerseUrl::VerseUrl()
{

}
VerseUrl::VerseUrl(const VerseUrlRange &range)
{
    addRange(range);
}

void VerseUrl::setParam(const QString &name, const QString &value)
{
    //check if name or value contains / or , or =
    m_params.insert(name, value);
}

bool VerseUrl::hasParam(const QString &name) const
{
    return m_params.contains(name);
}
QString VerseUrl::getParam(const QString &name) const
{
    return m_params.value(name, "");
}
void VerseUrl::addRange(const VerseUrlRange &range)
{
    m_ranges.append(range);
}
void VerseUrl::addRanges(const QList<VerseUrlRange> &ranges)
{
    m_ranges.append(ranges);
}

QString VerseUrl::toString() const
{
    QString ret = "";
    foreach(const VerseUrlRange range, m_ranges) {
        if(!ret.isEmpty())
            ret += "|";//seperator
        else
            ret += "verse://";

        if(range.bible() == VerseUrlRange::LoadBibleByID) {
            ret += QString::number(range.bibleID());
        } else if(range.bible() == VerseUrlRange::LoadCurrentModule) {
            ret += "current";
        } else if(range.bible() == VerseUrlRange::LoadModuleByUID) {
            ret += "uid=" + range.bibleUID();
        }
        ret += ",";
        //Book
        if(range.book() == VerseUrlRange::LoadBookByID) {
            ret += QString::number(range.bookID());
        } else if(range.book() == VerseUrlRange::LoadCurrentBook) {
            ret += "current";
        } else if(range.book() == VerseUrlRange::LoadFirstBook) {
            ret += "first";
        } else if(range.book() == VerseUrlRange::LoadLastBook) {
            ret += "last";
        }

        ret += ",";

        //Chapter
        if(range.startChapter() == VerseUrlRange::LoadChapterByID) {
            ret += QString::number(range.startChapterID());
        } else if(range.startChapter() == VerseUrlRange::LoadCurrentChapter) {
            ret += "current";
        } else if(range.startChapter() == VerseUrlRange::LoadFirstChapter) {
            ret += "first";
        } else if(range.startChapter() == VerseUrlRange::LoadLastChapter) {
            ret += "last";
        }
        if((range.startChapter() != range.endChapter() && range.endChapter() != VerseUrlRange::LoadChapterNotSet) ||
                (range.startChapter() == range.endChapter() && range.startChapter() == VerseUrlRange::LoadChapterByID && range.startChapterID() != range.endChapterID() && range.endChapterID() != -1)) {
            ret += "-";
            if(range.endChapter() == VerseUrlRange::LoadChapterByID) {
                ret += QString::number(range.endChapterID());
            } else if(range.endChapter() == VerseUrlRange::LoadCurrentChapter) {
                ret += "current";
            } else if(range.endChapter() == VerseUrlRange::LoadFirstChapter) {
                ret += "first";
            } else if(range.endChapter() == VerseUrlRange::LoadLastChapter) {
                ret += "last";
            }
        }
        ret += ",";
        //Verse
        if(range.startVerse() == VerseUrlRange::LoadVerseByID) {
            ret += QString::number(range.startVerseID());
        } else if(range.startVerse() == VerseUrlRange::LoadCurrentVerse) {
            ret += "current";
        } else if(range.startVerse() == VerseUrlRange::LoadFirstVerse) {
            ret += "first";
        } else if(range.startVerse() == VerseUrlRange::LoadLastVerse) {
            ret += "last";
        }
        if((range.startVerse() != range.endVerse() && range.endVerse() != VerseUrlRange::LoadVerseNotSet) ||
                (range.startVerse() == range.endVerse() && range.startVerse() == VerseUrlRange::LoadVerseByID && range.startVerseID() != range.endVerseID() && range.endVerseID() != -1)) {
            ret += "-";
            if(range.endVerse() == VerseUrlRange::LoadVerseByID) {
                ret += QString::number(range.endVerseID());
            } else if(range.endVerse() == VerseUrlRange::LoadCurrentVerse) {
                ret += "current";
            } else if(range.endVerse() == VerseUrlRange::LoadFirstVerse) {
                ret += "first";
            } else if(range.endVerse() == VerseUrlRange::LoadLastVerse) {
                ret += "last";
            }
        }
        if(range.activeVerse() == VerseUrlRange::LoadVerseByID) {
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
bool VerseUrl::fromString(QString url)
{
    m_ranges.clear();

    //bible://bibleID,bookID,chapterID,verseID,otherStuf=otherValue
    if(!url.startsWith("verse://")) {
        return false;
    }
    url.remove(0, 8); // remove verse://
    QStringList urls = url.split("|");
    foreach(const QString nUrl, urls) {
        VerseUrlRange range;
        const QStringList params = nUrl.split(",");
        for(int i = 0; i < params.size(); ++i) {
            QString p = params.at(i);
            QString p2;
            if(i == 0) {//Bible
                if(p == "current") {
                    range.setModule(VerseUrlRange::LoadCurrentModule);
                } else if(p.startsWith("uid=")) {
                    range.setModule(p.remove(0, 4));
                } else {
                    range.setModule(p.toInt());
                }
            } else if(i == 1) {//Book
                if(p == "current") {
                    range.setBook(VerseUrlRange::LoadCurrentBook);
                } else if(p == "first") {
                    range.setBook(VerseUrlRange::LoadFirstBook);
                } else if(p == "last") {
                    range.setBook(VerseUrlRange::LoadLastBook);
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
                    range.setStartChapter(VerseUrlRange::LoadCurrentChapter);
                } else if(p == "first") {
                    range.setStartChapter(VerseUrlRange::LoadFirstChapter);
                } else if(p == "last") {
                    range.setStartChapter(VerseUrlRange::LoadLastChapter);
                } else {
                    range.setStartChapter(p.toInt());
                }
                if(p2 == "current") {
                    range.setEndChapter(VerseUrlRange::LoadCurrentChapter);
                } else if(p2 == "first") {
                    range.setEndChapter(VerseUrlRange::LoadFirstChapter);
                } else if(p2 == "last") {
                    range.setEndChapter(VerseUrlRange::LoadLastChapter);
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
                    range.setStartVerse(VerseUrlRange::LoadCurrentVerse);
                } else if(p == "first") {
                    range.setStartVerse(VerseUrlRange::LoadFirstVerse);
                } else if(p == "last") {
                    range.setStartVerse(VerseUrlRange::LoadLastVerse);
                } else {
                    range.setStartVerse(p.toInt());
                }

                if(p2 == "current") {
                    range.setEndVerse(VerseUrlRange::LoadCurrentVerse);
                } else if(p2 == "first") {
                    range.setEndVerse(VerseUrlRange::LoadFirstVerse);
                } else if(p2 == "last") {
                    range.setEndVerse(VerseUrlRange::LoadLastVerse);
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
QList<VerseUrlRange> VerseUrl::ranges() const
{
    return m_ranges;
}
void VerseUrl::clearRanges()
{
    m_ranges.clear();
}
void VerseUrl::unsetParam(const QString &name)
{
    m_params.remove(name);
}
bool VerseUrl::contains(const int &moduleID, const int &bookID, const int &chapterID, const int &verseID) const
{
    foreach(const VerseUrlRange & range, m_ranges) {
        if(range.bibleID() == moduleID && range.bookID() == bookID && range.containsChapter(chapterID) && range.containsVerse(verseID))
            return true;
    }
    return false;
}
bool VerseUrl::contains(const int &moduleID, const int &bookID, const int &chapterID) const
{
    foreach(const VerseUrlRange & range, m_ranges) {
        if(range.bibleID() == moduleID && range.bookID() == bookID && range.containsChapter(chapterID))
            return true;
    }
    return false;
}
