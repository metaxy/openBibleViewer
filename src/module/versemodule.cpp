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
#include "versemodule.h"

VerseModule::VerseModule()
{
    m_versification = 0;
    m_lastTextRanges = 0;
}
VerseModule::~VerseModule()
{

}

void VerseModule::setLastTextRanges(TextRanges *textRanges)
{
    m_lastTextRanges = textRanges;
}
TextRanges *VerseModule::lastTextRanges() const
{
    return m_lastTextRanges;
}

Versification *VerseModule::versification() const
{
    return m_versification;
}
TextRanges VerseModule::readRanges(const Ranges &ranges, bool ignoreModuleID)
{
    myWarning() << "calling VerseModule";
    return TextRanges();
}

TextRange VerseModule::readRange(const Range &range, bool ignoreModuleID)
{
    myWarning() << "calling VerseModule";
    return TextRange();
}

SearchQuery VerseModule::lastSearchQuery() const
{
    return m_lastSearchQuery;
}
void VerseModule::search(SearchQuery query, SearchResult *result)
{
    myWarning() << "calling VerseModule";
}
void VerseModule::setmoduledisplaysettings(ModuleDisplaySettings *moduledisplaysettings)
{
    m_moduledisplaysettings = moduledisplaysettings;
}