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
#include "windowcache.h"
#include "dbghelper.h"

WindowCache::WindowCache()
{
    m_reload = false;
}
void WindowCache::setBibleList(BibleList *b)
{
    m_b[m_currentWindowID] = b;
}
void WindowCache::newWindow()
{
    m_currentWindowID = m_idList.size();
    m_idList << QString::number(m_idList.size());
}
void WindowCache::removeWindow(const int &id)
{
    DEBUG_FUNC_NAME
    BibleList *list = m_b[id];
    if(list) {
        delete list;
        list = 0;
        m_b.remove(id);
    }

    m_idList.removeAt(id);
}
void WindowCache::clearAll()
{
    DEBUG_FUNC_NAME
    foreach(BibleList *list, m_b) {
        if(list) {
            delete list;
        }
    }
    m_idList.clear();
    m_b.clear();
}

bool WindowCache::setCurrentWindowID(const int &id)
{
    if(id < m_idList.size() && id >= 0) {
        m_currentWindowID = m_idList.at(id).toInt();
        return true;
    }
    myWarning() << "failed to set current window id to " << id;
    return false;
}
BibleList* WindowCache::getBibleList()
{
    if(m_b.contains(m_currentWindowID))
        return m_b[m_currentWindowID];
    else {
        myWarning() << "no biblelist at " << m_currentWindowID;
        return 0;
    }
}
int WindowCache::currentWindowID()
{
    return m_currentWindowID;
}

