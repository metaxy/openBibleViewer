/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "src/module/versemodule.h"

/**
 * Bible represent a bible module(eg. biblequote module or zefania xml module)
 * It reads the module, gets the raw data and formats it.
 * Meta-Module
 */
class Bible : public VerseModule
{
public:

    Bible();
    ~Bible();
    virtual TextRanges readRanges(const Ranges &ranges, bool ignoreModuleID = false);
    /**
      * ignoreModuleID is used in VerseTable
      */
    virtual TextRange readRange(const Range &range, bool ignoreModuleID = false);

    QStringList getSearchPaths() const;
    /**
      * Unsafe. Could be deleted.
      */
    BibleModule *bibleModule() const;

    void search(SearchQuery query, SearchResult *result);

    QString moduleTitle() const;
    QString moduleShortTitle() const;
    QString moduleUID() const;

    SearchQuery lastSearchQuery() const;

    void clearData();

private:
    int loadModuleData(const int bibleID);
    bool loaded() const;

    QString toUniformHtml(const QString &string);
    QList<int> bookIDs() const;

    bool m_loaded;
    int m_bookID;

    QString m_moduleTitle;
    QString m_moduleShortTitle;
    QString m_moduleUID;
    QSharedPointer<BibleModule> m_bibleModule;

};

#endif // BIBLE_H
