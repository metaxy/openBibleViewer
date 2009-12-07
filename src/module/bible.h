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
#ifndef BIBLE_H
#define BIBLE_H

#include <QtXml/QDomElement>
#include <QtCore/QMap>
#include "biblequote.h"
#include "zefania-bible.h"
#include "../core/stelle.h"
#include "../core/searchquery.h"
class Bible
{
public:
    enum bibleType {
        None = 0,
        BibleQuote = 1,
        ZefaniaBible = 2,
        ZefaniaStrong = 3
    };
    Bible();
    void setBibleType(int type);
    void setSettings(Settings *settings);
    QMap<int, QList<Chapter> > getZefCache();
    void clearZefCache();
    void setZefCache(QMap<int, QList<Chapter> > cache);

    int loadBibleData(int bibleID, QString path);
    int readBook(int id);
    QString readChapter(int chapterID, int verseID);
    QString readVerse(int chapterID, int startVerse, int endVerse, int markVerseID, bool saveRawDatas);
    QStringList getSearchPaths();

    struct stelle search(struct searchQuery query);

    int currentBibleID, currentBookID, currentChapterID, chapterAdd;
    struct stelle st;
    QString bibleName, lastout, path, currentBiblePath, lastSearchString;
    QStringList bookFullName, chapterText, bookPath, chapterNames, chapterDataList;
    QMap <int, int> bookCount;
    QList<Chapter> chapterData;
    int bibleType;
    biblequote bq;
    zefaniaBible zef;
private:

    Settings *m_settings;

};

#endif // BIBLE_H
