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
#include "versification_zefania.h"

Versification_Zefania::Versification_Zefania(const QStringList &fullNames, const QStringList &shortNames, const QStringList &bookIDs)
{
    for(int i = 0; i < fullNames.size(); i++) {
        BookV11N book;
        book.name = fullNames.at(i);
        book.shortNames = (QStringList() << shortNames.at(i));
        book.bookID = bookIDs.at(i).toInt();
        m_books[book.bookID] = book;
    }
    setFlags(Versification::ReturnAll);
}
bool Versification_Zefania::filter(const int bookID, const VersificationFilterFlags flags) const
{
    if(flags.testFlag(Versification::ReturnAll) || (flags.testFlag(Versification::ReturnOT) && flags.testFlag(Versification::ReturnNT)))
        return true;
    else if(flags.testFlag(Versification::ReturnOT)) {
        return (bookID >= 0 && bookID <= 38);
    } else if(flags.testFlag(Versification::ReturnNT)) {
        return (bookID >= 39);
    }
    return true;
}
void Versification_Zefania::setMaxChapter(const int bookID, const int maxChapter)
{
    BookV11N book = m_books.value(bookID);
    book.maxChapter = maxChapter;
    m_books[bookID] = book;
}
