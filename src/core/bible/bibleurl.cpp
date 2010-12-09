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

void BibleUrl::setParam(const QString &name, const QString &value)
{
    //check if name or value contains / or , or =
    m_params.insert(name, value);
}

bool BibleUrl::hasParam(const QString &name)
{
    return m_params.contains(name);
}
QString BibleUrl::getParam(const QString &name)
{
    return m_params.value(name, "");
}
void BibleUrl::addRange(const BibleUrlRange &range)
{
    m_ranges.append(range);
}

QString BibleUrl::toString()
{
    QString ret = ;
    foreach (const BibleUrlRange range, m_ranges) {
        if(!ret.isEmpty())
            ret += "|";
        ret += "bible://";

        if(range.bible() == BibleUrlRange::LoadBibleByID) {
            ret += QString::number(range.bibleID());
        } else if(range.bible() == BibleUrlRange::LoadCurrentBible) {
            ret += "current";
        } else if(range.bible() == BibleUrlRange::ReloadActive) {
            ret += "reloadActive";
            return ret;
        }

        ret += "/";
        //Book
        if(range.startBook() == BibleUrlRange::LoadBookByID) {
            ret += QString::number(range.startBookID());
        } else if(range.startBook() == BibleUrlRange::LoadCurrentBook) {
            ret += "current";
        } else if(range.startBook() == BibleUrlRange::LoadFirstBook) {
            ret += "first";
        }
        if(range.startBook() != range.endBook() ||
          (range.startBook() == range.endBook() && range.startBook() == BibleUrl::LoadBookByID && range.startBookID() != range.endBookID())) {
            ret += "-";
            if(range.endBook() == BibleUrlRange::LoadBookByID) {
                ret += QString::number(range.endBookID());
            } else if(range.endBook() == BibleUrlRange::LoadCurrentBook) {
                ret += "current";
            } else if(range.endBook() == BibleUrlRange::LoadFirstBook) {
                ret += "first";
            }
        }
        ret += ",";

        //Chapter
        if(range.startChapter() == BibleUrlRange::LoadChapterByID) {
            ret += QString::number(range.startChapterID());
        } else if(range.startChapter() == BibleUrlRange::LoadCurrentChapter) {
            ret += "current";
        } else if(range.startChapter() == BibleUrlRange::LoadFirstChapter) {
            ret += "first";
        }
        if(range.startChapter() != range.endChapter() ||
          (range.startChapter() == range.endChapter() && range.startChapter() == BibleUrl::LoadChapterByID && range.startChapterID() != range.endChapterID())) {
            ret += "-";
            if(range.endChapter() == BibleUrlRange::LoadChapterByID) {
                ret += QString::number(range.endChapterID());
            } else if(range.endChapter() == BibleUrlRange::LoadCurrentChapter) {
                ret += "current";
            } else if(range.endChapter() == BibleUrlRange::LoadFirstChapter) {
                ret += "first";
            }
        }
        ret += ",";
        //Verse
        if(range.startBook() == BibleUrlRange::LoadBookByID) {
            ret += QString::number(range.startBookID());
        } else if(range.startBook() == BibleUrlRange::LoadCurrentBook) {
            ret += "current";
        } else if(range.startBook() == BibleUrlRange::LoadFirstBook) {
            ret += "first";
        }
        if(range.startBook() != range.endBook() ||
          (range.startBook() == range.endBook() && range.startBook() == BibleUrl::LoadBookByID && range.startBookID() != range.endBookID())) {
            ret += "-";
            if(range.endBook() == BibleUrlRange::LoadBookByID) {
                ret += QString::number(range.endBookID());
            } else if(range.endBook() == BibleUrlRange::LoadCurrentBook) {
                ret += "current";
            } else if(range.endBook() == BibleUrlRange::LoadFirstBook) {
                ret += "first";
            }
        }
        ret += ",";

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
bool BibleUrl::fromString(QString url)
{
    //bible://bibleID/bookID,chapterID,verseID,otherStuf=otherValue
    if(!url.startsWith("bible://")) {
        return false;
    }
    url.remove(0, 8); // remove bible://

    const QStringList slash = url.split("/");
    if(slash.size() != 2) {//there should be only two
        return false;
    }
    const QString bibleID = slash.at(0);
    if(bibleID == "current") {
        m_bibleParam = BibleUrl::LoadCurrentBible;
        m_bibleID = -1;
    } else if(bibleID == "reloadActive") {
        m_bibleParam = BibleUrl::ReloadActive;
        m_bibleID = -1;
        return true;
    } else {
        m_bibleID = bibleID.toInt();
        m_bibleParam = BibleUrl::LoadBibleByID;
    }
    const QString other = slash.at(1);
    const QStringList params = other.split(",");
    for(int i = 0; i < params.size(); ++i) {
        const QString p = params.at(i);
        if(i == 0) {
            if(p == "current") {
                m_bookParam = BibleUrl::LoadCurrentBook;
            } else if(p == "first") {
                m_bookParam = BibleUrl::LoadFirstBook;
            } else {
                m_bookID = p.toInt();
                m_bookParam = BibleUrl::LoadBookByID;
            }
        } else if(i == 1) {
            if(p == "current") {
                m_chapterParam = BibleUrl::LoadCurrentChapter;
            } else if(p == "first") {
                m_chapterParam = BibleUrl::LoadFirstChapter;
            } else {
                m_chapterID = p.toInt();
                m_chapterParam = BibleUrl::LoadChapterByID;
            }
        } else if(i == 2) {
            if(p == "current") {
                m_verseParam = BibleUrl::LoadCurrentVerse;
            } else if(p == "first") {
                m_verseParam = BibleUrl::LoadFirstVerse;
            } else {
                m_verseID = p.toInt();
                m_verseParam = BibleUrl::LoadVerseByID;
            }
        } else {
            if(p.contains("=")) {
                const QStringList s = p.split("=");
                const QString key = s.first();
                const QString value = s.last();
                m_params.insert(key, value);
            } else {
                m_params.insert(p, "true");
            }
        }
    }


    return true;
}
