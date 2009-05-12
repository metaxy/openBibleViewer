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
#ifndef BIBLE_H
#define BIBLE_H

#include <QDomElement>
#include "biblequote.h"
#include "zefania.h"
#include "../kernel/stelle.h"

class bible
{
	public:
		bible();
		void setBibleType(int type);
		int loadBibleData(int bibleID,QString path);
		int readBook(int id);
		void setSettings( struct settings_s *settings );

		QList<QDomElement> getZefCache();
		void clearZefCache();
		void setZefCache(QList<QDomElement> cache);

		QString readChapter(int chapterID, int verseID);
                int currentBibleID,currentBookID,currentChapterID,chapterAdd;
		struct stelle search(QString searchstring,bool regexp,bool whole,bool casesen);
		struct stelle st;
                QString bibleName,lastout,path,currentBiblePath,lastSearchString;
                QStringList bookCount,bookFullName,chapterText,bookPath,chapterNames,chapterDataList;
		QList<chapter> chapterData;
		int bibleType;
	private:

		biblequote bq;
		zefania zef;
		struct settings_s settings;

};

#endif // BIBLE_H
