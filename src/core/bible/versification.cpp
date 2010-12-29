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
#include "versification.h"
#include "src/core/dbghelper.h"
Versification::Versification()
{
}
Versification::~Versification()
{
}
BookNames Versification::toBookNames(VersificationFilterFlags filter)
{
    //DEBUG_FUNC_NAME
    QHash<int, QString> bookFullName;
    QHash<int, QStringList> bookShortName;
    QList<int> bookIDs;

    QStringList bNames = getBookNames(filter);
    QList<QStringList> bShortNames = multipleBookShortNames(filter);

    //myDebug() << bNames;
    for(int i = 0; i < bNames.size(); i++) {
        bookFullName.insert(i, bNames.at(i));
        bookShortName.insert(i, bShortNames.at(i));
        bookIDs.append(i);
    }

    BookNames ret;
    ret.m_bookFullName = bookFullName;
    ret.m_bookShortName = bookShortName;
    ret.m_bookIDs = bookIDs;
    return ret;
}
QMap<int, int> Versification::toBookCount(VersificationFilterFlags filter)
{
    //DEBUG_FUNC_NAME
    QMap<int, int> ret;
    QList<int> mChapter = maxChapter(filter);
    for(int i = 0; i < mChapter.size(); i++) {
        ret.insert(i, mChapter.at(i));
    }
    return ret;
}
