/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
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
#include "v11ntest.h"
#include <QtTest/QtTest>
#include <QDebug>
#include "src/core/verse/versification/versification_kjv.h"
#include "src/core/verse/versification/versification_cache.h"

void V11nTests::testBooks()
{
    Versification *test = createTestV11n();
    QHash<int, QString> bookNames;
    bookNames[bookID1()] = bookName1();
    bookNames[bookID2()] = bookName2();
    QCOMPARE(bookNames, test->bookNames());
    
     QHash<int, QStringList> bookShortNames;
    bookShortNames[bookID1()] =  bookShortNames1();
    bookShortNames[bookID2()] = bookShortNames2();
    QCOMPARE(bookShortNames, test->multipleBookShortNames());
    /*
    QHash<int, QStringList> multipleBookShortNames(VersificationFilterFlags filter) const;
    QHash<int, QString> bookShortNames(VersificationFilterFlags filter) const;
    QList<int> bookIDs(VersificationFilterFlags filter) const;
    QHash<int, int> maxChapter(VersificationFilterFlags filter) const;
    QHash<int, QList<int> > maxVerse(VersificationFilterFlags filter) const;
    int bookCount(VersificationFilterFlags filter) const;
    */
    delete test;
}

void V11nTests::testCreate()
{
    Versification_KJV *v11n = new Versification_KJV();
    delete v11n;
    
    Versification* v11n2 = createTestV11n();
    delete v11n2;
}

Versification* V11nTests::createTestV11n()
{
    QMap<int, BookV11N> d;
    
    {
        BookV11N b1;
        b1.bookID = bookID1();
        b1.name = bookName1();
        b1.shortNames = bookShortNames1();
        b1.maxChapter = maxChapter1();
        b1.maxVerse = maxVerse1();
        d[b1.bookID] = b1;
    }
    {
        BookV11N b1;
        b1.bookID = bookID2();
        b1.name = bookName2();
        b1.shortNames = bookShortNames2();
        b1.maxChapter = maxChapter2();
        b1.maxVerse = maxVerse2();
        d[b1.bookID] = b1;
    }
    return new Versification_Cache(d);
    
}
QString V11nTests::bookName1()
{
    return "Book 1";
}
QString V11nTests::bookName2()
{
    return "Book 2";
}
    
QStringList V11nTests::bookShortNames1()
{
    QStringList l;
    l << "1B" << "1.B" << "1 B";
    return l;
}
QStringList V11nTests::bookShortNames2()
{
    QStringList l;
    l << "2B" << "2.B" << "2 B";
    return l;
}
    
int V11nTests::bookID1()
{
    return 0;
}
int V11nTests::bookID2()
{
    return 40;
}
    
int V11nTests::maxChapter1()
{
    return 1;
}
int V11nTests::maxChapter2()
{
    return 1;
}
    
QList<int> V11nTests::maxVerse1()
{
    QList<int> maxVerse;
    maxVerse << 10;
    return maxVerse;
}
QList<int> V11nTests::maxVerse2()
{
    QList<int> maxVerse;
    maxVerse << 20;
    return maxVerse;
}

QTEST_MAIN( V11nTests )
