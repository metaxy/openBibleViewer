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
#ifndef V11N_TEST_H
#define V11N_TEST_H

#include <QObject>
#include <QtCore/QMap>
#include "src/core/link/biblelink.h"
class V11nTests : public QObject
{
    Q_OBJECT
private slots:
    void testBooks();
    void testCreate();
public:
    static Versification* createTestV11n();
    static QString bookName1();
    static QString bookName2();
    
    static QStringList bookShortNames1();
    static QStringList bookShortNames2();
    
    static int bookID1();
    static int bookID2();
    
    static int maxChapter1();
    static int maxChapter2();
    
    static QList<int> maxVerse1();
    static QList<int> maxVerse2();
    
};

#endif
