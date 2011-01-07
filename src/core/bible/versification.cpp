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
QStringList Versification::bookNames(VersificationFilterFlags filter) const
{
    QStringList ret;
    foreach(const BookV11N &book, m_books) {
        if(!filter(book.bookID))
            continue;
        ret.append(book.name);
    }
    return ret;
}
QList<QStringList> Versification::multipleBookShortNames(VersificationFilterFlags filter) const
{
    QList<QStringList> ret;
    foreach(const BookV11N &book, m_books) {
        if(!filter(book.bookID))
            continue;
        ret.append(book.shortNames);
    }
    return ret;
}
QStringList Versification::bookShortNames(VersificationFilterFlags filter) const
{
    QList<QStringList> mul = multipleBookShortNames(filter);
    QStringList ret;
    foreach(const QStringList & a, mul) {
        if(a.isEmpty())
            ret.append("");
        else
            ret.append(a.first());
    }
    return ret;
}
QList<int> Versification::maxChapter(VersificationFilterFlags filter) const
{
    QList<int> ret;
    foreach(const BookV11N &book, m_books) {
        if(!filter(book.bookID))
            continue;
        ret.append(book.maxChapter);
    }
    return ret;
}
QList<QList<int> > Versification::maxVerse(VersificationFilterFlags filter) const
{
    QList<QList<int> > ret;
    foreach(const BookV11N &book, m_books) {
        if(!filter(book.bookID))
            continue;
        ret.append(book.maxVerse);
    }
    return ret;
}
int Versification::bookCount(VersificationFilterFlags filter) const
{
    return bookNames(filter).size();
}
bool Versification::filter(const int &bookID, VersificationFilterFlags filter) const
{
    if(filter.testFlag(Versification::ReturnAll) || (filter.testFlag(Versification::ReturnOT) && filter.testFlag(Versification::ReturnNT)))
        return true;
    return true;
}
void Versification::setFlags(VersificationFilterFlags filter)
{
    m_filter = filter;
}

QStringList Versification::bookNames() const
{
    return bookNames(m_filter);
}
QList<QStringList> Versification::multipleBookShortNames() const
{
    return multipleBookShortNames(m_filter);
}
QStringList Versification::bookShortNames() const
{
    return bookShortNames(m_filter);
}
QList<int> Versification::maxChapter() const
{
    return maxChapter(m_filter);
}
QList< QList<int> > Versification::maxVerse() const
{
    return maxVerse(m_filter);
}
int Versification::bookCount()
{
    return bookCount(m_filter);
}
QString Versification::bookName(const int &bookID, bool preferShort) const
{
    const QStringList shortNames = m_books.value(bookID).shortNames;
    if(!shortNames.isEmpty()) {
        if(preferShort) {
            return shortNames.first();
        } else {
            return m_books.value(bookID,shortNames.first()).name;
        }
    } else {
        return m_books.value(bookID).name;
    }
}
QMap<int, BookV11N> Versification::data() const
{
    return m_books;
}
