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
#ifndef BIBLELIST_H
#define BIBLELIST_H
#include "src/module/versemodule.h"
#include "src/core/link/verseurl.h"

#include <QtCore/QPoint>
#include <utility>
/**
 * VerseTable represents a list of VerseModules to display them in one window. It is a some kind of a parallel view.
 */
class VerseTable
{
public:
    VerseTable();
    /**
     * Deletes all bibles of the VerseTable
     */
    ~VerseTable();

    void clear();
    /**
     * Returns the bible at id.
     * \param id The bibleList internal id, not the moduleID. If id == -1 then current bible;
     */
    VerseModule * verseModule(const int id = -1) const;
    void addModule(VerseModule* b, const QPoint &p);
    void setCurrentVerseTableID(const int verseTableID);
    int currentVerseTableID() const;

    std::pair<QString, TextRanges> readRanges(const Ranges &ranges) const;

    /**
     * Retruns true if a top bar is shown.
     */
    bool hasTopBar() const;

    QHash<int, VerseModule *> m_modules;
    QMap<int, QPoint> m_points;

    void setLastTextRanges(TextRanges *textRanges);
    void setLastUrl(const VerseUrl &url);
    TextRanges *lastTextRanges();
    VerseUrl lastVerseUrl();

    void clearData();
private:
    inline QString title(VerseModule *module, const QString &active, const int bibleListID) const;
    int countInCol(const int col) const;
    int m_currentModule;
    TextRanges *m_lastTextRanges;
    VerseUrl m_lastUrl;

};

#endif // BIBLELIST_H
