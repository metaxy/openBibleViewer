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
#include "../core/searchresult.h"
#include "../core/notes.h"
/*!
 Bible represent a bible module(eg biblequote module)

 @author Paul Walger <metaxy@walger.name>
*/
class Bible
{
public:
    enum bibleType {
        None = 0,
        BibleQuoteModule = 1,
        ZefaniaBibleModule = 2,
        ZefaniaStrongModule = 3
    };
    Bible();
    void setBibleType(const int &type);
    void setSettings(Settings *settings);
    QMap<int, QList<Chapter> > getZefCache();
    void clearZefCache();
    void setZefCache(QMap<int, QList<Chapter> > cache);
    void setNotes(Notes*n);

    int loadBibleData(const int &bibleID, const QString &path);
    int readBook(int id);
    QString readChapter(int chapterID, int verseID);
    QString readVerse(int chapterID, int startVerse, int endVerse, int markVerseID, bool saveRawDatas);
    QStringList getSearchPaths();
    QString toUniformHtml(QString string);
    QStringList toUniformHtml(QStringList string);

    SearchResult search(SearchQuery query);

    int currentBibleID, currentBookID, currentChapterID, chapterAdd;
    struct stelle st;
    QString bibleName, lastout, path, currentBiblePath;
    SearchQuery lastSearchQuery;
    SearchResult lastSearchResult;
    QStringList bookFullName, chapterText, bookPath, chapterNames, chapterDataList;
    QStringList biblesIniPath;
    QMap <int, int> bookCount;
    QList<Chapter> chapterData;
    int bibleType;
    BibleQuote bq;
    ZefaniaBible zef;
private:
    Settings *m_settings;
    Notes *m_notes;

};

#endif // BIBLE_H
