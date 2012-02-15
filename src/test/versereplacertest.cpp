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
#include "versereplacertest.h"
#include <QtTest/QtTest>

#include <QDebug>
#include "src/core/versereplacer.h"
void VerseReplacerTests::testAppend()
{
    {
        QMap<int, Verse> map = simpleMap();
        VerseReplacer r1;
        r1.setAppend(0, "Append1");
        r1.setAppend(1, "");
        r1.exec(&map);
        QVERIFY(map.value(0).data() == "Test Verse #0Append1");
        QVERIFY(map.value(1).data() == "Test Verse #1");
        QVERIFY(map.value(3).data() == "Test Verse #3");
    }
    {
        QMap<int, Verse> map = simpleMap();
        VerseReplacer r1;
        r1.setAppend(0, "Append1");
        r1.setAppend(0, "Append2");
        r1.exec(&map);
        QVERIFY(map.value(0).data() == "Test Verse #0Append1Append2");
        QVERIFY(map.value(1).data() == "Test Verse #1");
        QVERIFY(map.value(3).data() == "Test Verse #3");
    }
    {
        QMap<int, Verse> map = simpleMap();
        VerseReplacer r1;
        r1.setAppend(1, "Append1");
        r1.setAppend(1, "");
        r1.exec(&map);
        QVERIFY(map.value(0).data() == "Test Verse #0");
        QVERIFY(map.value(1).data() == "Test Verse #1Append1");
        QVERIFY(map.value(3).data() == "Test Verse #3");
    };

    {
        QMap<int, Verse> map = simpleMap();
        VerseReplacer r1;
        r1.setAppend(3, "Append1");
        r1.setAppend(3, "");
        r1.exec(&map);
        QVERIFY(map.value(0).data() == "Test Verse #0");
        QVERIFY(map.value(1).data() == "Test Verse #1");
        QVERIFY(map.value(3).data() == "Test Verse #3Append1");
    };
}
void VerseReplacerTests::testPrepend()
{
    {
        QMap<int, Verse> map = simpleMap();
        VerseReplacer r1;
        r1.setPrepend(0, "Prepend1");
        r1.setPrepend(1, "");
        r1.exec(&map);
        QVERIFY(map.value(0).data() == "Prepend1Test Verse #0");
        QVERIFY(map.value(1).data() == "Test Verse #1");
        QVERIFY(map.value(3).data() == "Test Verse #3");
    }
    {
        QMap<int, Verse> map = simpleMap();
        VerseReplacer r1;
        r1.setPrepend(0, "Prepend1");
        r1.setPrepend(0, "Prepend2");
        r1.exec(&map);
        QVERIFY(map.value(0).data() == "Prepend2Prepend1Test Verse #0");
        QVERIFY(map.value(1).data() == "Test Verse #1");
        QVERIFY(map.value(3).data() == "Test Verse #3");
    }
    {
        QMap<int, Verse> map = simpleMap();
        VerseReplacer r1;
        r1.setPrepend(1, "Prepend1");
        r1.setPrepend(1, "");
        r1.exec(&map);
        QVERIFY(map.value(0).data() == "Test Verse #0");
        QVERIFY(map.value(1).data() == "Prepend1Test Verse #1");
        QVERIFY(map.value(3).data() == "Test Verse #3");
    };
}
void VerseReplacerTests::testInsert()
{

}

QMap<int, Verse> VerseReplacerTests::simpleMap()
{
    Verse v1(0, "Test Verse #0");
    Verse v2(1, "Test Verse #1");
    Verse v3(3, "Test Verse #3");
    QMap<int,Verse> map;
    map.insert(v1.verseID(), v1);
    map.insert(v2.verseID(), v2);
    map.insert(v3.verseID(), v3);
    return map;
}

QTEST_MAIN( VerseReplacerTests )
