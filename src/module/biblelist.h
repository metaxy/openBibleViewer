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
#include <QPoint>
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
    Bible * bible(const int &id = -1);
    void addBible(Bible* b, const QPoint &p);

    int readBook(int id);
    QString readChapter(int chapterID, int verseID);
    QString readVerse(int chapterID, int startVerse, int endVerse, int markVerseID, bool saveRawDatas);
    int m_currentBible;

    bool hasTopBar();

private:
    QString title(Bible *b, const QString &active, const int &bibleListID);
    int countInCol(const int &col);

};

#endif // BIBLELIST_H
