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
    m_b[m_windowName] = b;
}
/**
  Generate a new window name
  @return the window name
  */
int WindowCache::newWindow()
{
    m_windowName = m_nameList.size();//Generate a new window
    m_nameList << m_windowName;
    return m_windowName;
}
void WindowCache::removeWindow(const int &name)
{
    BibleList *list = m_b[name];
    if(list) {
        delete list;
        list = 0;
        m_b.remove(name);
    }
    m_nameList.removeOne(name);

}
/**
  Deletes all BibleLists and clears internal data
  */
void WindowCache::clearAll()
{
    foreach(BibleList * list, m_b) {
        if(list) {
            delete list;
        }
    }
    m_nameList.clear();
    m_b.clear();
    m_windowName = 0;
}

bool WindowCache::setCurrentWindowID(const int &id)
{
    if(id < m_nameList.size() && id >= 0) {
        m_windowName = m_nameList.at(id);
        return true;
    }
    myWarning() << "failed to set current window id to " << id;
    return false;
}
bool WindowCache::setCurrentWindowName(const int &name)
{
    if(name >= 0) {
        m_windowName = name;
        return true;
    }
    myWarning() << "failed to set current window name to " << name;
    return false;
}
BibleList* WindowCache::getBibleList() const
{
    if(m_b.contains(m_windowName))
        return m_b.value(m_windowName);
    else {
        myWarning() << "no biblelist at " << m_windowName;
        return 0;
    }
}
/**
  @returns current window ID
  */
int WindowCache::currentWindowID() const
{
    return m_nameList.value(m_windowName);
}
/**
  @returns all window names.
  */
QList<int> WindowCache::nameList() const
{
    return m_nameList;
}

