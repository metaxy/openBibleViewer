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
    qDebug() << "windowCache::setBible() currentTabID = " << currentTabID;
    bibletype[currentTabID] = 1;
    qDebug() << "windowCache::setBible() bibleName = " << b_.bibleName << " bibleType" << b_.bibleType;
    bibleName[currentTabID] = b_.bibleName;
    books[currentTabID] = b_.bookFullName;
    /*if(b_.bibleType == 2)//zefania
    {
        qDebug() << "windowCache::setBible() size of zefCache" << zefcache[b_.currentBibleID] .size();
        zefcache[b_.currentBibleID] = b_.getZefCache();
        b_.clearZefCache();
    }*/
    b[currentTabID] = b_;
    qDebug() << "windowCache::setBible() end";
    return 0;
}

int windowCache::newWindow(void)
{
    idlist << QString::number(idlist.size(), 10);
    return 0;
}
int windowCache::removeTab(int id)
{
    idlist.removeAt(id);
    return 0;
}
/*int windowCache::moveTab(int from, int to)
{
    idlist.move(from, to);
    return 0;
}*/
int windowCache::clearAll(void)
{
    qDebug() << "windowCache::clearAll";
    idlist.clear();
    b.clear();
    zefcache.clear();
    bibletype.clear();
    bibleName.clear();
    books.clear();
    chaptercount.clear();
    selectedBook.clear();
    return 0;
}
int windowCache::getBibleType(void)
{
    return bibletype[currentTabID];
}
int windowCache::setCurrentTabId(int id)
{
    if (id < idlist.size() && id >= 0) {
        QString idl = idlist.at(id);
        currentTabID = idl.toInt();
        return 0;
    }
    return 1;
}
int windowCache::setCurrentBook(int bookid, int ichaptercount)
{
    chaptercount[currentTabID] = ichaptercount;
    selectedBook[currentTabID] = bookid;
    return 0;
}
QString windowCache::getBibleName()
{
    return bibleName[currentTabID];
}
int windowCache::getCurrentBook()
{
    return selectedBook[currentTabID];
}
QStringList windowCache::getBooks(void)
{
    return books[currentTabID];
}
int windowCache::getChapterCount(void)
{
    return chaptercount[currentTabID];
}
bible windowCache::getBible(void)
{
    qDebug() << "bible windowCache::getBible";
    /*if(b[currentTabID].bibleType == 2)//Zefania
    {
        qDebug() << "bible windowCache::zefania";
        b[currentTabID].setZefCache(getZefaniaCache(b[currentTabID].currentBibleID));
    }*/
    return b[currentTabID];
}
QMap<int, QList<chapter> > windowCache::getZefaniaCache(int bibleID)
{
    qDebug() << "windowCache::getZefaniaCache bibleID = " << bibleID << "zefcache[bibleID].size() = " << zefcache[bibleID].size();
    return zefcache[bibleID];
}
