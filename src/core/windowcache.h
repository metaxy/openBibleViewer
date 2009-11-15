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
#include "../module/bible.h"
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
class windowCache
{
public:
    windowCache();

    QMap<int, bible> m_b;

    QMap<int, QMap<int, QList<chapter> > > m_zefcache;
    QMap<int, bool> m_zefcacheset;
    QMap<int, int> m_bibletype;
    QMap<int, QString> m_bibleName;
    QMap<int, QStringList> m_books;
    QMap<int, int> m_chaptercount;
    QMap<int, int> m_selectedBook;
    QStringList m_idList;
    int m_currentWindowID;

    int newWindow(void);
    int removeWindow(int id);
    int clearAll(void);

    int setCurrentWindowID(int id);
    int setBible(bible b_);
    int setCurrentBook(int bookid, int chaptercount);

    QString getBibleName();
    int getBibleType(void);
    int getCurrentBook(void);
    QStringList getBooks(void);
    int getChapterCount(void);
    QMap<int, QList<chapter> > getZefaniaCache(int bibleID);
    bible getBible(void);

};

#endif // WINDOWCACHE_H
