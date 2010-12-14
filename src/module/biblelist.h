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
#ifndef BIBLELIST_H
#define BIBLELIST_H
#include "src/module/bible.h"
#include <QtCore/QPoint>
#include <utility>
/**
  BibleList represents a list of bible to display them in one window. It is a some kind of parallel view
*/
class BibleList
{
public:
    BibleList();
    ~BibleList();
    QHash<int, Bible *> m_bibles;
    QMap<int, QPoint> m_biblePoints;
    void clear();
    Bible * bible(const int &id = -1) const;
    void addBible(Bible* b, const QPoint &p);

    int readBook(int id);
    QString readChapter(const int &chapterID, const int &verseID) const;
    std::pair<QString, TextRanges> readRanges(const Ranges &ranges) const;

    int m_currentBible;

    bool hasTopBar() const;

private:
    QString title(Bible *b, const QString &active, const int &bibleListID) const;
    int countInCol(const int &col) const;

};

#endif // BIBLELIST_H
