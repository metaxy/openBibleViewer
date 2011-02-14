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
#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QDir>
#include <QtGui/QTextDocument>

#include "src/module/bible/biblequote.h"
#include "src/module/bible/zefania-bible.h"
#include "src/module/bible/thewordbible.h"
#include "src/module/versemodule.h"

#include "src/core/settings/modulesettings.h"
#include "src/core/settings/moduledisplaysettings.h"

#include "src/core/dbghelper.h"
#include "src/core/urlconverter.h"
#include "src/core/versereplacer.h"
#include "src/core/verseselection.h"
/**
 * Bible represent a bible module(eg. biblequote module or zefania xml module)
 * It reads the module, gets the raw data and formats it.
 */
class Bible : public VerseModule
{
public:

    Bible();
    ~Bible();
    void setmoduledisplaysettings(ModuleDisplaySettings *moduledisplaysettings);
    virtual TextRanges readRanges(const Ranges &ranges, bool ignoreModuleID = false);
    virtual TextRange readRange(const Range &range, bool ignoreModuleID = false);

    int loadModuleData(const int bibleID);
    /**
      * Load only the book without pharsing.
      */
    int readBook(const int id);

    /**
      * Used only by BibleQuote modules.
      * @returns A list of paths, where images can be found.
      */
    QStringList getSearchPaths() const;

    void search(SearchQuery query, SearchResult *result);

    QString moduleTitle() const;
    QString moduleShortTitle() const;
    QString moduleUID() const;

    QStringList bookPath();
    SearchQuery lastSearchQuery() const;
    bool loaded() const;

private:
    bool m_loaded;

    int m_bookID;

    QString m_moduleTitle;
    QString m_moduleShortTitle;
    QString m_modulePath;
    QString m_moduleUID;

    /**
      * Used by BibleQuote to store where the books are
      */
    QStringList m_bookPath;

    BibleModule *m_bibleModule;

    QString toUniformHtml(QString string);
    QList<int> bookIDs() const;
};

#endif // BIBLE_H
