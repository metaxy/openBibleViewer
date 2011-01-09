/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "versetable.h"
#ifndef BOOKV11N_H
#define BOOKV11N_H
#include <QtCore/QStringList>
class BookV11N
{
public:
    BookV11N();
    QString name;
    QStringList shortNames;
    int bookID;
    int maxChapter;
    QList<int> maxVerse;
};

#endif // BOOKV11N_H
