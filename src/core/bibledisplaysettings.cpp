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
#include "bibledisplaysettings.h"

BibleDisplaySettings::BibleDisplaySettings()
{
}

void BibleDisplaySettings::setLoadNotes(bool loadNotes)
{
    m_loadNotes = loadNotes;
}
bool BibleDisplaySettings::loadNotes()
{
    return m_loadNotes;
}

void BibleDisplaySettings::setShowMarks(bool showMarks)
{
    if(showMarks)
        m_loadNotes = true;
    m_showMarks = showMarks;
}
bool BibleDisplaySettings::showMarks()
{
    return m_showMarks;
}

bool BibleDisplaySettings::showNotes()
{
    return m_showNotes;
}
void BibleDisplaySettings::setShowNotes(bool showNotes)
{
    if(showNotes)
        m_loadNotes = true;
    m_showNotes = showNotes;
}
