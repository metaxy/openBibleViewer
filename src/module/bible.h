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
#include "src/core/search/searchquery.h"
#include "src/core/search/searchresult.h"
#include "src/module/simplemoduleclass.h"
#include "src/core/settings/bibledisplaysettings.h"
#include "src/core/bible/booknames.h"
/*!
 Bible represent a bible module(eg. biblequote module or zefania xml module)
 It reads the module, gets the raw data and formats it.
*/
class Bible : public SimpleModuleClass
{
public:

    Bible();
    void setBibleDisplaySettings(BibleDisplaySettings *bibleDisplaySettings);

    int loadModuleData(const int &bibleID);
    int readBook(int id);
    QString readChapter(int chapterID, int verseID);
    QString readVerse(int chapterID, int startVerse, int endVerse, int markVerseID, bool saveRawDatas);
    QStringList getSearchPaths();
    QString toUniformHtml(QString string);

    void search(SearchQuery query, SearchResult *result);

    Module::ModuleType bibleType();

    int bookID();
    int chapterID();
    int verseID();
    int booksCount();
    int chaptersCount();

    QString bibleTitle();
    QString bibleShortTitle();
    QString biblePath();

    QString bookName(const int &bookID, bool preferShort = false);
    QHash<int, QString> bookNames(bool preferShort = false);

    QHash<int, QString> bookFullNames();
    QHash<int, QStringList> bookShortNames();
    QList<int> bookIDs();
    QStringList bookPath();
    QStringList chapterNames();
    QStringList chapterDataList();
    SearchQuery lastSearchQuery();

    bool loaded();
    Chapter rawChapter();
private:
    int m_bookID;
    int m_chapterID;
    int m_verseID;

    QMap <int, int> bookCount;
    QString m_lastout;
    QString m_bibleTitle;
    QString m_bibleShortTitle;
    QString m_biblePath;

    QStringList m_chapterText;
    QStringList m_bookPath;
    QStringList m_chapterNames;
    QStringList m_chapterDataList;
    BookNames m_names;

    BibleDisplaySettings *m_bibleDisplaySettings;

    Module *m_module;
    BibleModule *m_bibleModule;
    /*BibleQuote *m_bibleModule;
    ZefaniaBible *m_bibleModule;*/
    bool m_loaded;
    SearchQuery m_lastSearchQuery;
    QString textTitle;
    Book m_book;

    Chapter m_rawChapter;
};

#endif // BIBLE_H
