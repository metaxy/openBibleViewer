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
#include "versification_biblequote.h"

Versification_BibleQuote::Versification_BibleQuote(const QStringList &fullNames, const QList<QStringList> &shortNames, const QMap<int, int> &bookCount)
{
    for(int i = 0; i < fullNames.size(); i++) {
        BookV11N book;
        book.name = fullNames.at(i);
        if(i < shortNames.size())
            book.shortNames = shortNames.at(i);
        else
            book.shortNames = QStringList();
        book.bookID = i;
        book.maxChapter = bookCount.value(i);
        m_books[book.bookID] = book;
    }
    setFlags(Versification::ReturnAll);
}
bool Versification_BibleQuote::filter(const int bookID, const Versification::VersificationFilterFlags flags) const
{
    return true;
}
