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
#include <QtCore/QDir>
#include <QtGui/QTextDocument>

#include "src/module/bible/biblequote.h"
#include "src/module/bible/zefania-bible.h"
#include "src/module/bible/thewordbible.h"
#include "src/module/versemodule.h"

#include "src/core/settings/modulesettings.h"
#include "src/core/settings/bibledisplaysettings.h"

#include "src/core/dbghelper.h"
#include "src/core/urlconverter.h"
#include "src/core/versereplacer.h"
#include "src/core/verseselection.h"
/*!
 Bible represent a bible module(eg. biblequote module or zefania xml module)
 It reads the module, gets the raw data and formats it.
*/
class Bible : public VerseModule
{
public:

    Bible();
    void setBibleDisplaySettings(BibleDisplaySettings *bibleDisplaySettings);
    virtual TextRanges readRanges(const Ranges &ranges, bool ignoreModuleID = false);
    virtual TextRange readRange(const Range &range, bool ignoreModuleID = false);

    int loadModuleData(const int &bibleID);
    /**
      * Load only the book without pharsing.
      */
    int readBook(const int &id);

    /**
      * Used only by BibleQuote modules.
      * @returns A list of paths, where images can be found.
      */
    QStringList getSearchPaths() const;

    void search(SearchQuery query, SearchResult *result);

    int bookID() const;
    int chapterID() const;
    int verseID() const;
    int booksCount() const;
    int chaptersCount() const;

    QString moduleTitle() const;
    QString moduleShortTitle() const;
    QString modulePath() const;

    QString bookName(const int &bookID, bool preferShort = false);

    QStringList bookPath();
    QStringList chapterNames();
    SearchQuery lastSearchQuery() const;
    bool loaded() const;

private:

    int m_bookID;
    int m_chapterID;
    int m_verseID;

    QMap <int, int> bookCount;
    QString m_moduleTitle;
    QString m_moduleShortTitle;
    QString m_modulePath;

    QStringList m_bookPath;
    QStringList m_chapterNames;

    BibleDisplaySettings *m_bibleDisplaySettings;

    BibleModule *m_bibleModule;

    bool m_loaded;

    Book m_book;

    QString toUniformHtml(QString string);

    QList<int> bookIDs() const;
};

#endif // BIBLE_H
