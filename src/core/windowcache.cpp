/***************************************************************************
openBibleViewer - Free Bibel Viewer
Copyright (C) 2009 Paul Walger
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
#include "windowcache.h"
#include "../module/zefania-bible.h"
#include "../module/biblequote.h"
#include <QtCore/QtDebug>

windowCache::windowCache()
{
}
int windowCache::setBible(bible b_)
{
    qDebug() << "windowCache::setBible() currentWindowID = " << m_currentWindowID;
    m_bibletype[m_currentWindowID] = 1;
    qDebug() << "windowCache::setBible() bibleName = " << b_.bibleName << " bibleType" << b_.bibleType;
    m_bibleName[m_currentWindowID] = b_.bibleName;
    m_books[m_currentWindowID] = b_.bookFullName;
    /*if(b_.bibleType == 2)//zefania
    {
        qDebug() << "windowCache::setBible() size of zefCache" << zefcache[b_.currentBibleID] .size();
        zefcache[b_.currentBibleID] = b_.getZefCache();
        b_.clearZefCache();
    }*/
    m_b[m_currentWindowID] = b_;
    qDebug() << "windowCache::setBible() end";
    return 0;
}

int windowCache::newWindow(void)
{
    m_idList << QString::number(m_idList.size());
    return 0;
}
int windowCache::removeWindow(int id)
{
    m_idList.removeAt(id);
    return 0;
}
int windowCache::clearAll(void)
{
    qDebug() << "windowCache::clearAll";
    m_idList.clear();
    m_b.clear();
    m_zefcache.clear();
    m_bibletype.clear();
    m_bibleName.clear();
    m_books.clear();
    m_chaptercount.clear();
    m_selectedBook.clear();
    return 0;
}
int windowCache::getBibleType(void)
{
    return m_bibletype[m_currentWindowID];
}
int windowCache::setCurrentWindowID(int id)
{
    if (id < m_idList.size() && id >= 0) {
        m_currentWindowID =  m_idList.at(id).toInt();
        return 0;
    }
    return 1;
}
int windowCache::setCurrentBook(int bookID, int chapterCount)
{
    m_chaptercount[m_currentWindowID] = chapterCount;
    m_selectedBook[m_currentWindowID] = bookID;
    return 0;
}
QString windowCache::getBibleName()
{
    return m_bibleName[m_currentWindowID];
}
int windowCache::getCurrentBook()
{
    return m_selectedBook[m_currentWindowID];
}
QStringList windowCache::getBooks(void)
{
    return m_books[m_currentWindowID];
}
int windowCache::getChapterCount(void)
{
    return m_chaptercount[m_currentWindowID];
}
bible windowCache::getBible(void)
{
    qDebug() << "bible windowCache::getBible";
    /*if(b[currentWindowID].bibleType == 2)//Zefania
    {
        qDebug() << "bible windowCache::zefania";
        b[currentWindowID].setZefCache(getZefaniaCache(b[currentWindowID].currentBibleID));
    }*/
    return m_b[m_currentWindowID];
}
QMap<int, QList<chapter> > windowCache::getZefaniaCache(int bibleID)
{
    qDebug() << "windowCache::getZefaniaCache bibleID = " << bibleID << "zefcache[bibleID].size() = " << m_zefcache[bibleID].size();
    return m_zefcache[bibleID];
}
