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
#ifndef BIBLE_H
#define BIBLE_H
#include <QtCore/QObject>
#include <QtCore/QMap>
#include "src/module/biblequote.h"
#include "src/module/zefania-bible.h"
#include "src/core/searchquery.h"
#include "src/core/searchresult.h"
#include "src/module/simplemoduleclass.h"
#include "src/core/bibledisplaysettings.h"
#include "src/module/modulemap.h"
/*!
 Bible represent a bible module(eg biblequote module or zefania xml module)
 It read the module over the module, get the raw data, format it.

 @author Paul Walger <metaxy@walger.name>
*/
class Bible : public SimpleModuleClass
{
public:
    enum BibleType {
        None = 0,
        BibleQuoteModule = 1,
        ZefaniaBibleModule = 2
    };
    Bible();
    void setSettings(Settings *settings);
    void setBibleType(const BibleType &type);
    void setModuleMap(ModuleMap *map);
    void setBibleDisplaySettings(BibleDisplaySettings *bibleDisplaySettings);

    int loadModuleData(const int &bibleID);
    int readBook(int id);
    QString readChapter(int chapterID, int verseID);
    QString readVerse(int chapterID, int startVerse, int endVerse, int markVerseID, bool saveRawDatas);
    QStringList getSearchPaths();
    QString toUniformHtml(QString string);
    QStringList toUniformHtml(QStringList string);

    SearchResult search(SearchQuery query);

    int moduleID();
    void setModuleID(const int &bible);

    BibleType bibleType();

    int bookID();
    int chapterID();
    int verseID();
    int chapterAdd();
    int booksCount();
    int chaptersCount();

    QString bibleTitle();
    QString biblePath();

    QStringList bookFullName();
    QStringList bookPath();
    QStringList chapterNames();
    QStringList chapterDataList();
    QStringList biblesRootPath();
    void setBiblesRootPath(QStringList biblesRootPath);

    SearchQuery m_lastSearchQuery;

    QList<Chapter> m_chapterData;

    Module *m_module;
    BibleQuote *m_bq;
    ZefaniaBible *m_zef;

    ModuleMap *m_map;
private:
    int m_moduleID;
    int m_bookID;
    int m_chapterID;
    int m_verseID;
    int m_chapterAdd;
    BibleType m_bibleType;
    QMap <int, int> bookCount;
    QString m_lastout;
    QString m_bibleTitle;
    QString m_biblePath;

    QStringList m_bookFullName;
    QStringList m_chapterText;
    QStringList m_bookPath;
    QStringList m_chapterNames;
    QStringList m_chapterDataList;
    QStringList m_biblesRootPath;

    SearchResult m_lastSearchResult;
    BibleDisplaySettings *m_bibleDisplaySettings;

};

#endif // BIBLE_H
