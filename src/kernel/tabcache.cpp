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
#include "tabcache.h"
#include "../bible/zefania.h"
#include "../bible/biblequote.h"
#include <QtDebug>

tabCache::tabCache()
{
}
int tabCache::setBible(bible b_)
{
	qDebug() << "tabCache::setBible() currentTabID = "<<currentTabID;
	bibletype[currentTabID] = 1;
	qDebug() << "tabCache::setBible() bibleName = " << b_.bibleName << " bibleType" << b_.bibleType;
	bibleName[currentTabID] = b_.bibleName;
	books[currentTabID] = b_.bookFullName;
	if(b_.bibleType == 2)//zefania
	{
		zefcache[b_.currentBibleID] = b_.getZefCache();
		b_.clearZefCache();
	}
	b[currentTabID] = b_;
	qDebug() << "tabCache::setBible() end";
	return 0;
}

int tabCache::newTab(void)
{
	idlist << QString::number(idlist.size(),10);
	return 0;
}
int tabCache::removeTab(int id)
{
	idlist.removeAt(id);
	return 0;
}
int tabCache::moveTab(int from,int to)
{
	idlist.move(from,to);
	return 0;
}
int tabCache::clearAll( void )
{
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
int tabCache::getBibleType( void)
{
	return bibletype[currentTabID];
}
int tabCache::setCurrentTabId( int id)
{
	if(id < idlist.size())
	{
		QString idl = idlist.at(id);
		currentTabID = idl.toInt();
	}
	return 0;
}
int tabCache::setCurrentBook(int bookid,int ichaptercount)
{
	chaptercount[currentTabID] = ichaptercount;
	selectedBook[currentTabID] = bookid;
	return 0;
}
QString tabCache::getBibleName()
{
	return bibleName[currentTabID];
}
int tabCache::getCurrentBook()
{
	return selectedBook[currentTabID];
}
QStringList tabCache::getBooks( void )
{
	return books[currentTabID];
}
int tabCache::getChapterCount( void )
{
	return chaptercount[currentTabID];
}
bible tabCache::getBible( void )
{
	if(b[currentTabID].bibleType == 2)//Zefania
	{
		b[currentTabID].setZefCache(getZefaniaCache(b[currentTabID].currentBibleID));
	}
	return b[currentTabID];
}
QList<QDomElement> tabCache::getZefaniaCache(int bibleid)
{
	return zefcache[bibleid];
}
