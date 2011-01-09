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
#include "verse.h"
Verse::Verse()
{
    m_verseID = -1;
    m_text = "";
}

Verse::Verse(const int &verseID, const QString &text)
{
    m_verseID = verseID;
    m_text = text;
}
int Verse::verseID() const
{
    return m_verseID;
}

QString Verse::data() const
{
    return m_text;
}
void Verse::append(const QString &str)
{
    m_text.append(str);
}

void Verse::prepend(const QString &str)
{
    m_text.prepend(str);
}
void Verse::insert(int position, const QString & str)
{
    m_text.insert(position, str);
}
