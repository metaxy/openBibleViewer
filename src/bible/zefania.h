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
#ifndef ZEFANIA_H
#define ZEFANIA_H
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include "../core/config.h"
#include "../core/stelle.h"
#include "../core/chapter.h"

class zefania
{
private:
	struct settings_s zefset;
	QString formatfromini(QString input);
public:
	zefania();
	int setSettings( struct settings_s *settings );
	void readBook(int id);
	void loadBibleData(int id,QString path);
	QString readInfo(QFile &file);
	QList<QDomElement> cache_books_data;
	int currentBookID,fontsize,bibletype,book_ccount;
	struct stelle st;
	struct stelle search(QString searchstring,bool regexp,bool whole,bool casesen);
	int bibleid;
	QString currentBiblePath,lastout,chaptersign,versesign,biblepath,bibleName,lastSearch;
	QStringList bibles,biblesPath,
	biblesIniPath,
	bookPath,
	bookFullName,
	bookShortName,
	bookCount,
	chapterText;
	QList<chapter> chapterData;

};

#endif // ZEFANIA_H
