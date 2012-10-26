/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#ifndef VERSEMODULE_H
#define VERSEMODULE_H
#include "src/core/verse/ranges.h"
#include "src/core/module/simplemoduleclass.h"

struct CompiledRange {
    int moduleID;
    int bookID;
    int chapterID;
    int startVerse;
    int endVerse;

    int error;
};
/**
 * VerseModule is an abstract class for modules classes like Bible or StudyNotes which are based on a versification.
 * That means it has books, chapters and verse.
 * Meta-Module.
 */
class VerseModule : public SimpleModuleClass, public SearchableModule
{
public:
    VerseModule();
    virtual ~VerseModule();
    void setModuleDisplaySettings(ModuleDisplaySettings *moduleDisplaySettings);
    virtual Response* readRanges(const Ranges &ranges, bool ignoreModuleID = false) = 0;
    /**
     * Returns the Versification of the module.
     * Every VerseModule must have a versification.
     */
    QSharedPointer<Versification> versification() const;
    virtual void search(SearchQuery query, SearchResult *result) = 0;
    virtual void clearData() = 0;
protected:
    QSharedPointer<Versification> m_versification;
    ModuleDisplaySettings *m_moduleDisplaySettings;

    QList<int> bookIDs() const;
    virtual int currentBook() = 0;
    virtual int currentChapter() = 0;
    virtual std::pair<int, int> minMaxVerse(const int bookID, const int chapterID) = 0;
    CompiledRange toCompiledRange(const Range &range);
};

#endif // VERSEMODULE_H
