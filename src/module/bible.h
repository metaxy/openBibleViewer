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
#include <QtCore/QObject>
#include <QtXml/QDomElement>
#include <QtCore/QMap>
#include "src/module/biblequote.h"
#include "src/module/zefania-bible.h"
#include "src/core/searchquery.h"
#include "src/core/searchresult.h"
#include "src/module/simplemoduleclass.h"
#include "src/core/bibledisplaysettings.h"
/*!
 Bible represent a bible module(eg biblequote module)

 @author Paul Walger <metaxy@walger.name>
*/
class Bible : public SimpleModuleClass
{
public:
    enum m_bibleTypes {
        None = 0,
        BibleQuoteModule = 1,
        ZefaniaBibleModule = 2
    };
    Bible();
    void setSettings(Settings *settings);
    void setBibleType(const int &type);
    QMap<int, QList<Chapter> > getSoftCache();
    void clearSoftCache();
    void setSoftCache(QMap<int, QList<Chapter> > cache);

    void setBibleDisplaySettings(BibleDisplaySettings bibleDisplaySettings);
    BibleDisplaySettings m_bibleDisplaySettings;

    int loadBibleData(const int &bibleID, const QString &path);
    int readBook(int id);
    QString readChapter(int chapterID, int verseID);
    QString readVerse(int chapterID, int startVerse, int endVerse, int markVerseID, bool saveRawDatas);
    QStringList getSearchPaths();
    QString toUniformHtml(QString string);
    QStringList toUniformHtml(QStringList string);

    SearchResult search(SearchQuery query);

    int bibleID();
    void setBibleID(const int &bible);
    int bookID();
    int chapterID();
    int chapterAdd();
    int booksCount();
    int chaptersCount();

    QString bibleTitle, lastout, m_biblePath;
    SearchQuery lastSearchQuery;
    SearchResult lastSearchResult;
    QStringList bookFullName, chapterText, bookPath, chapterNames, chapterDataList;
    QStringList biblesIniPath;
    QMap <int, int> bookCount;
    QList<Chapter> chapterData;
    int m_bibleType;
    int m_verseID;
    BibleQuote bq;
    ZefaniaBible zef;
private:
    int m_bibleID;
    int m_bookID;
    int m_chapterID;
    int m_chapterAdd;

};

#endif // BIBLE_H
