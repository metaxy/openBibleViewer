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
#ifndef CHAPTER_H
#define CHAPTER_H

#include <QtCore/QString>
#include <QtCore/QStringList>
/*!
 Chapter represents a bible chapter

 @author Paul Walger <metaxy@walger.name>
*/
struct Chapter {
    QStringList data;
    QStringList verseNumber;
    QString chapterName;
    QString bookName;
    int verseCount;
    int chapterID;

};

#endif // CHAPTER_H
