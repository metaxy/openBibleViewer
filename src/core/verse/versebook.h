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
#ifndef VERSEBOOK_H
#define VERSEBOOK_H
#include <QtCore/QHash>
#include "src/core/verse/chapter.h"
class VerseBook
{
public:
    VerseBook();
    VerseBook(const int bookID);
    int size() const;
    void clear();
    void addChapter(const Chapter &c);

    bool hasChapter(const int chapterID) const;
    Chapter getChapter(const int chapterID);
    void setID(const int id);
    int bookID() const;
    QHash<int, Chapter> data() const;
private:
    int m_id;
    QHash<int, Chapter> m_chapters;
};

#endif // VERSEBOOK_H
