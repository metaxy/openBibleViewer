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
#include "src/core/bible/bibleurl.h"
#include <QtCore/QPoint>
#include <utility>
/**
  BibleList represents a list of bible to display them in one window. It is a some kind of parallel view
*/
class BibleList
{
public:
    BibleList();
    /**
      * Deletes all bibles of the BibleList
      */
    ~BibleList();

    void clear();
    /**
      Returns the bible at id.
      \param id The bibleList internal id, not the moduleID. If id == -1 then current bible;
      */
    Bible * bible(const int &id = -1) const;
    void addBible(Bible* b, const QPoint &p);
    void setCurrentBibleListID(const int &bibleListID);
    int currentBibleListID() const;

    int readBook(int id);
    QString readChapter(const int &chapterID, const int &verseID) const;
    //todo: should be const, but uses
    std::pair<QString, TextRanges> readRanges(const Ranges &ranges) const;

    /**
      * Retruns true if a top bar is shown.
      */
    bool hasTopBar() const;

    QHash<int, Bible *> m_bibles;
    QMap<int, QPoint> m_biblePoints;

    void setLastTextRanges(TextRanges *textRanges);
    void setLastUrl(BibleUrl *url);
    TextRanges *lastTextRanges();
    BibleUrl *lastBibleUrl();

private:
    QString title(Bible *b, const QString &active, const int &bibleListID) const;
    int countInCol(const int &col) const;
    int m_currentBible;
    TextRanges *m_lastTextRanges;
    BibleUrl *m_lastUrl;

};

#endif // BIBLELIST_H
