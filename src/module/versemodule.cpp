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
    m_lastTextRanges = NULL;
}

VerseModule::~VerseModule()
{

}

void VerseModule::setLastTextRanges(TextRanges *textRanges)
{
    //DEBUG_FUNC_NAME;
    //myDebug() << m_moduleID;
    //myDebug() << textRanges->source().source().toString();
    m_lastTextRanges = textRanges;
}

TextRanges *VerseModule::lastTextRanges() const
{
    return m_lastTextRanges;
}

QSharedPointer<Versification> VerseModule::versification() const
{
    return m_versification;
}

SearchQuery VerseModule::lastSearchQuery() const
{
    return m_lastSearchQuery;
}

void VerseModule::setModuleDisplaySettings(ModuleDisplaySettings *moduleDisplaySettings)
{
    m_moduleDisplaySettings = moduleDisplaySettings;
}
