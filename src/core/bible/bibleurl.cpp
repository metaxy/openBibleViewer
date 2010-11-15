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
    m_bibleParam = BibleUrl::LoadBibleByID;
    m_bookParam = BibleUrl::LoadBookByID;
    m_chapterParam = BibleUrl::LoadChapterByID;
    m_verseParam = BibleUrl::LoadVerseByID;

    m_bibleID = -1;
    m_bookID = -1;
    m_chapterID = -1;
    m_verseID = -1;
}
void BibleUrl::setBibleID(const int &bibleID)
{
    m_bibleID = bibleID;
}

void BibleUrl::setBible(const BibleLoadParams &param)
{
    m_bibleParam = param;
}
int BibleUrl::bibleID()
{
    return m_bibleID;
}
BibleUrl::BibleLoadParams BibleUrl::bible()
{
    return m_bibleParam;
}

void BibleUrl::setBookID(const int &bookID)
{
    m_bookID = bookID;
}

void BibleUrl::setBook(const BookLoadParams &param)
{
    m_bookParam = param;
}
int BibleUrl::bookID()
{
    return m_bookID;
}
BibleUrl::BookLoadParams BibleUrl::book()
{
    return m_bookParam;
}

void BibleUrl::setChapterID(const int &chapterID)
{
    m_chapterID = chapterID;
}

void BibleUrl::setChapter(const ChapterLoadParams &param)
{
    m_chapterParam = param;
}
int BibleUrl::chapterID()
{
    return m_chapterID;
}
BibleUrl::ChapterLoadParams BibleUrl::chapter()
{
    return m_chapterParam;
}

void BibleUrl::setVerseID(const int &verseID)
{
    m_verseID = verseID;
}

void BibleUrl::setVerse(const VerseLoadParams &param)
{
    m_verseParam = param;
}
int BibleUrl::verseID()
{
    return m_verseID;
}
BibleUrl::VerseLoadParams BibleUrl::verse()
{
    return m_verseParam;
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

QString BibleUrl::toString()
{
    QString ret = "bible://";

    if(m_bibleParam == BibleUrl::LoadBibleByID) {
        ret += QString::number(m_bibleID);
    } else if(m_bibleParam == BibleUrl::LoadCurrentBible) {
        ret += "current";
    } else if(m_bibleParam == BibleUrl::ReloadActive) {
        ret += "reloadActive";
        return ret;
    }

    ret += "/";

    if(m_bookParam == BibleUrl::LoadBookByID) {
        ret += QString::number(m_bookID);
    } else if(m_bookParam == BibleUrl::LoadCurrentBook) {
        ret += "current";
    } else if(m_bookParam == BibleUrl::LoadFirstBook) {
        ret += "first";
    }
    ret += ",";


    if(m_chapterParam == BibleUrl::LoadChapterByID) {
        ret += QString::number(m_chapterID);
    } else if(m_chapterParam == BibleUrl::LoadCurrentChapter) {
        ret += "current";
    } else if(m_chapterParam == BibleUrl::LoadFirstChapter) {
        ret += "first";
    }
    ret += ",";

    if(m_verseParam == BibleUrl::LoadVerseByID) {
        ret += QString::number(m_verseID);
    } else if(m_verseParam == BibleUrl::LoadCurrentVerse) {
        ret += "current";
    } else if(m_verseParam == BibleUrl::LoadFirstVerse) {
        ret += "first";
    }
    if(!m_params.isEmpty()) {
        QHashIterator<QString, QString> i(m_params);
        while(i.hasNext()) {
            i.next();
            ret += "," + i.key() + "=" + i.value();
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
