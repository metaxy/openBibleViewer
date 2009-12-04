/***************************************************************************
openBibleViewer - Bible Study Tool
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
#include "dbghelper.h"
#include <QtCore/QtDebug>

WindowCache::WindowCache()
{
}
void WindowCache::setBible(Bible b)
{
    //DEBUG_FUNC_NAME
    m_bibletype[m_currentWindowID] = 1;
    m_bibleName[m_currentWindowID] = b.bibleName;
    m_books[m_currentWindowID] = b.bookFullName;
    m_b[m_currentWindowID] = b;
}

void WindowCache::newWindow()
{
    m_idList << QString::number(m_idList.size());
}
void WindowCache::removeWindow(const int &id)
{
    m_idList.removeAt(id);
}
void WindowCache::clearAll()
{
    DEBUG_FUNC_NAME
    m_idList.clear();
    m_b.clear();
    m_zefcache.clear();
    m_bibletype.clear();
    m_bibleName.clear();
    m_books.clear();
    m_chapterCount.clear();
    m_selectedBook.clear();
}
int WindowCache::getBibleType()
{
    return m_bibletype[m_currentWindowID];
}
bool WindowCache::setCurrentWindowID(const int &id)
{
    if (id < m_idList.size() && id >= 0) {
        m_currentWindowID =  m_idList.at(id).toInt();
        return false;
    }
    return true;
}
void WindowCache::setCurrentBook(const int &bookID,const int &chapterCount)
{
    m_chapterCount[m_currentWindowID] = chapterCount;
    m_selectedBook[m_currentWindowID] = bookID;
}
QString WindowCache::getBibleName()
{
    return m_bibleName[m_currentWindowID];
}
int WindowCache::getCurrentBook()
{
    return m_selectedBook[m_currentWindowID];
}
QStringList WindowCache::getBooks()
{
    return m_books[m_currentWindowID];
}
int WindowCache::getChapterCount()
{
    return m_chapterCount[m_currentWindowID];
}
Bible WindowCache::getBible()
{
    DEBUG_FUNC_NAME
    return m_b[m_currentWindowID];
}
QMap<int, QList<Chapter> > WindowCache::getZefaniaCache(const int &bibleID)
{
    DEBUG_FUNC_NAME
    //it could be that there ist no zefania Cache
    return *m_zefcache[bibleID];
}
