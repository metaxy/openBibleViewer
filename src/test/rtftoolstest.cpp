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
#include "rtftoolstest.h"
#include <QtTest/QtTest>

#include <QDebug>
#include "src/core/rtftools.h"
void RtfToolsTests::initTestCase()
{
}
void RtfToolsTests::testFromRVF()
{
    QFile file(QString(SRC_PATH) + "/src/test/data/rtftools/test.rvf");
    if(!file.open(QFile::ReadOnly)) {
        Q_ASSERT(false);
        return;
    }
    QByteArray data = file.readAll();
    qDebug() << RtfTools::fromRVF(data);
}

void RtfToolsTests::cleanupTestCase()
{
}

QTEST_MAIN( RtfToolsTests )
