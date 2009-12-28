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
#ifndef WINDOWCACHE_H
#define WINDOWCACHE_H
#include "src/module/bible.h"
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QCache>
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
    void setBible(Bible b);
    void setCurrentBook(const int &bookid, const int &chapterCount);

    QString getBibleName();
    int getBibleType();
    int getCurrentBook();
    int getChapterCount();
    QStringList getBooks();

    QMap<int, QList<Chapter> > getZefaniaCache(const int &bibleID);
    Bible getBible();
private:
    QCache<int, QMap<int, QList<Chapter> > > m_zefcache;
    QMap<int, int> m_bibletype;
    QMap<int, QString> m_bibleName;
    QMap<int, QStringList> m_books;
    QMap<int, int> m_chapterCount;
    QMap<int, int> m_selectedBook;
    QStringList m_idList;
    int m_currentWindowID;
    QMap<int, Bible> m_b;

};

#endif // WINDOWCACHE_H
