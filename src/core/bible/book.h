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
#ifndef BOOK_H
#define BOOK_H
#include <QtCore/QHash>
#include "src/core/bible/chapter.h"
class Book
{
public:
    Book();
    QHash<int, Chapter> m_chapters;
    int size();
    void clear();
    void addChapter(const int &chapterID, const Chapter &c);
    bool hasChapter(const int &chapterID);
    Chapter getChapter(const int &chapterID);
    void setID(const int &id);
    int id();
private:
    int m_id;
};

#endif // BOOK_H
