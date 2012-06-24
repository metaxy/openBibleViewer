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
#include "biblelinktest.h"
#include "testtools.h"
#include <QtTest/QtTest>

#include <QDebug>
void BibleLinkTests::testFast_data()
{
    QTest::addColumn<QString>("link");
    QTest::addColumn<bool>("valid");
    QTest::newRow("simple") << "1Mose 3:3" << true;
    QTest::newRow("simple1") << "1.Mose 3:3" << true;
    QTest::newRow("simple2") << "1Mose 1:3" << true;
    
    //QTest::newRow("simple3") << "1Mose 1:3asd" << false;
    QTest::newRow("simple4") << "1Mose und wie es geth es dir" << false;
    //QTest::newRow("simple5") << "1Mose 1::" << false;
    
    QTest::newRow("simple6") << "1Mose 1:3-12" << true;
    QTest::newRow("simple7") << "1Mose 1-10" << true;
    QTest::newRow("simple8") << "1Mose 1,2" << true;
    QTest::newRow("simple9") << "1Mose 1,2-10" << true;
    
    QTest::newRow("simple10") << "Jakobus 1" << true;
    
    QTest::newRow("simple11") << "Jakobus" << false;
}
void BibleLinkTests::testFast()
{
    QFETCH(QString, link);
    QFETCH(bool, valid);
    QCOMPARE(BibleLink::fastIsBibleLink(link), valid);
    
}
void BibleLinkTests::testIsBibleLink_data()
{
    QTest::addColumn<QString>("link");
    QTest::addColumn<bool>("valid");
    QTest::newRow("one verse short") << "1B 1:2" << true;
    QTest::newRow("one verse long") << "Book 1 3:3" << true;
    QTest::newRow("one verse short 2") << "1.B 1:3" << true;
    
    //QTest::newRow("simple3") << "1Mose 1:3asd" << false;
    QTest::newRow("some text") << "1Mose und wie es geth es dir" << false;
    QTest::newRow("some text 2") << "1Mose" << false;
    
    QTest::newRow("verse range") << "1 B 1:3-8" << true;
    QTest::newRow("chapter range") << "Book 1 1-10" << true;
    QTest::newRow("comma") << "1B 1,2" << true;
    QTest::newRow("comma range") << "1B 1,2-10" << true;
    
    QTest::newRow("just one chapter") << "2B 1" << true;
    
    QTest::newRow("just the book") << "Book 2" << true;
}
void BibleLinkTests::testIsBibleLink()
{
    QFETCH(QString, link);
    QFETCH(bool, valid);
    QSharedPointer<Versification> v11n(TestTools::createTestV11n());
    BibleLink l(0, v11n);
    QCOMPARE(l.isBibleLink(link), valid);
    
    v11n.clear();
    
}

QTEST_MAIN( BibleLinkTests )
