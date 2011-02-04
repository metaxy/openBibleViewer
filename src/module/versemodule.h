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
#include "versetable.h"
#ifndef VERSEMODULE_H
#define VERSEMODULE_H
#include "src/core/verse/ranges.h"
#include "src/core/verse/range.h"
#include "src/core/verse/textrange.h"
#include "src/core/verse/textranges.h"
#include "src/core/verse/versification.h"
#include "src/module/simplemoduleclass.h"
#include "src/core/settings/moduledisplaysettings.h"
/**
  * VerseModule is an abstract class for modules classes like Bible or StudyNotes which are based on a versification.
  * That means it has books, chapters and verse.
  */
class VerseModule : public SimpleModuleClass
{
public:
    VerseModule();
    virtual ~VerseModule();
    void setmoduledisplaysettings(ModuleDisplaySettings *moduledisplaysettings);
    virtual TextRanges readRanges(const Ranges &ranges, bool ignoreModuleID = false);
    virtual TextRange readRange(const Range &range, bool ignoreModuleID = false);

    void setLastTextRanges(TextRanges *textRanges);
    /**
      * Returns the last computed TextRanges.
      */
    TextRanges *lastTextRanges() const;
    /**
      * Returns the Versification of the Module.
      * Every VerseModule must have a versification.
      */
    Versification *versification() const;

    /**
      * Searchs in the current VerseModule. Saves the results in the second argument (*result).
      */
    virtual void search(SearchQuery query, SearchResult *result);
    SearchQuery lastSearchQuery() const;
protected:
    SearchQuery m_lastSearchQuery;
    Versification *m_versification;
    TextRanges *m_lastTextRanges;
    ModuleDisplaySettings *m_moduledisplaysettings;
};

#endif // VERSEMODULE_H