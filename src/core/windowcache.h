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
#ifndef WINDOWCACHE_H
#define WINDOWCACHE_H
#include "src/module/bible.h"
#include "src/module/biblelist.h"
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
/*!
 WindowCache represents the cache for bible data

 @author Paul Walger <metaxy@walger.name>
*/
class WindowCache
{
public:
    WindowCache();

    void newWindow();
    void removeWindow(const int &id);
    void clearAll();

    bool setCurrentWindowID(const int &id);
    void setBibleList(BibleList *b);

    BibleList* getBibleList();
    bool m_reload;
    int currentWindowID();
private:
    QStringList m_idList;
    int m_currentWindowID;
    QMap<int, BibleList*> m_b;

};

#endif // WINDOWCACHE_H
