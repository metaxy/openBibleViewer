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
#include "booknames.h"

BookNames::BookNames()
{
    m_bookFullName = QHash<int, QString>();
    m_bookShortName = QHash<int, QStringList>();
    m_bookIDs = QList<int>();
}
QString BookNames::bookName(const int &bookID, bool preferShort) const
{
    const QStringList shortNames = m_bookShortName.value(bookID);
    if(!shortNames.isEmpty()) {
        if(preferShort) {
            return shortNames.first();
        } else {
            return m_bookFullName.value(bookID, shortNames.first());
        }
    } else {
        return m_bookFullName.value(bookID);
    }
}
