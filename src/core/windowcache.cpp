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
#include "dbghelper.h"
#include <QtCore/QtDebug>

WindowCache::WindowCache()
{
}
void WindowCache::setBible(Bible b)
{
   // DEBUG_FUNC_NAME
    m_softCache.insert(b.bibleID(), b.getSoftCache());
    b.clearSoftCache();
    m_b[m_currentWindowID] = b;
    m_bibletype[m_currentWindowID] = b.m_bibleType;
    //m_bibleName[m_currentWindowID] = b.bibleName;
    //m_books[m_currentWindowID] = b.bookFullName;
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
    //DEBUG_FUNC_NAME
    m_idList.clear();
    m_b.clear();
    m_softCache.clear();
    m_bibletype.clear();

}
int WindowCache::getBibleType()
{
    return m_bibletype[m_currentWindowID];
}

bool WindowCache::setCurrentWindowID(const int &id)
{
    if (id < m_idList.size() && id >= 0) {
        m_currentWindowID = m_idList.at(id).toInt();
        return true;
    }
    return false;
}
Bible WindowCache::getBible()
{
    //DEBUG_FUNC_NAME
    Bible b = m_b[m_currentWindowID];
    b.setSoftCache(getSoftCache(b.bibleID()));
    return b;
}
QMap<int, QList<Chapter> > WindowCache::getSoftCache(const int &bibleID)
{
    //DEBUG_FUNC_NAME
    //it could be that there ist no zefania Cache
    return m_softCache[bibleID];
}
