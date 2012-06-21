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
QTEST_MAIN( BibleLinkTests )
