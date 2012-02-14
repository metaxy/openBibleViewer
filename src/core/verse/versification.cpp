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
#include "versification.h"
#include "src/core/dbghelper.h"
Versification::Versification()
{
    //DEBUG_FUNC_NAME
}
Versification::~Versification()
{
}


QHash<int, QString> Versification::bookNames(VersificationFilterFlags flags) const
{
    QHash<int, QString> ret;
    foreach(const BookV11N & book, m_books) {
        if(!filter(book.bookID, flags))
            continue;
        ret[book.bookID] = book.name;
    }
    return ret;
}
QHash<int, QStringList> Versification::multipleBookShortNames(VersificationFilterFlags flags) const
{
    QHash<int, QStringList> ret;
    foreach(const BookV11N & book, m_books) {
        if(!filter(book.bookID, flags))
            continue;
        ret[book.bookID] = book.shortNames;
    }
    return ret;
}
QHash<int, QString> Versification::bookShortNames(VersificationFilterFlags flags) const
{
    QHash<int, QStringList> mul = multipleBookShortNames(flags);
    QHash<int, QString> ret;

    QHashIterator<int, QStringList> i(mul);
    while(i.hasNext()) {
        i.next();
        if(i.value().isEmpty())
            ret[i.key()] = "";
        else
            ret[i.key()] = i.value().first();
    }

    return ret;
}
QList<int> Versification::bookIDs(VersificationFilterFlags flags) const
{
    QList<int> ret;
    foreach(const BookV11N & book, m_books) {
        if(!filter(book.bookID, flags))
            continue;
        ret << book.bookID;
    }
    return ret;
}
QHash<int, int> Versification::maxChapter(VersificationFilterFlags flags) const
{
    QHash<int, int> ret;
    foreach(const BookV11N & book, m_books) {
        if(!filter(book.bookID, flags))
            continue;
        ret[book.bookID] = book.maxChapter;
    }
    return ret;
}
QHash<int, QList<int> > Versification::maxVerse(VersificationFilterFlags flags) const
{
    QHash<int, QList<int> > ret;
    foreach(const BookV11N & book, m_books) {
        if(!filter(book.bookID, flags))
            continue;
        ret[book.bookID] = book.maxVerse;
    }
    return ret;
}
int Versification::bookCount(VersificationFilterFlags flags) const
{
    return bookNames(flags).size();
}
bool Versification::filter(const int bookID, VersificationFilterFlags flags) const
{
    if(flags.testFlag(Versification::ReturnAll) || (flags.testFlag(Versification::ReturnOT) && flags.testFlag(Versification::ReturnNT)))
        return true;
    return true;
}
void Versification::setFlags(VersificationFilterFlags flags)
{
    m_filter = flags;
}

QHash<int, QString> Versification::bookNames() const
{
    return bookNames(m_filter);
}
QHash<int, QStringList> Versification::multipleBookShortNames() const
{
    return multipleBookShortNames(m_filter);
}
QHash<int, QString> Versification::bookShortNames() const
{
    return bookShortNames(m_filter);
}
QList<int> Versification::bookIDs() const
{
    return bookIDs(m_filter);
}
QHash<int, int> Versification::maxChapter() const
{
    return maxChapter(m_filter);
}
QHash<int, QList<int> >Versification::maxVerse() const
{
    return maxVerse(m_filter);
}
int Versification::bookCount() const
{
    return bookCount(m_filter);
}
QString Versification::bookName(const int bookID, bool preferShort) const
{
    const QStringList shortNames = m_books.value(bookID).shortNames;
    if(!shortNames.isEmpty()) {
        if(preferShort) {
            return shortNames.first();
        } else {
            if(!m_books.value(bookID).name.isEmpty())
                return m_books.value(bookID).name;
            else
                return shortNames.first();
        }
    } else {
        return m_books.value(bookID).name;
    }
}
QMap<int, BookV11N> Versification::data() const
{
    return m_books;
}
