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
#include "zefaniabibletest.h"
#include <QtTest/QtTest>

#include <QDebug>
#include "src/core/moduletools.h"

void ZefaniaBibleTests::init()
{
    
}
void ZefaniaBibleTests::testScan()
{
	//const QStringList resAll = ZefaniaBible::scan(QString(SRC_PATH) + "/src/test/data/moduletools/structure");

}

QTEST_MAIN( ZefaniaBibleTests )
