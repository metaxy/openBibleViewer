/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#ifndef VERSETABLE_H
#define VERSETABLE_H
#include "src/core/module/textrangesversemodule.h"
#include "src/core/verse/textranges.h"
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
     * \param id The bibleList internal id, not the moduleID. If id == -1 then current bible.
     */
    TextRangesVerseModule * verseModule(const int id = -1) const;
    void addModule(TextRangesVerseModule* module, const QPoint &p);
    void setActiveItem(const int verseTableID);
    int activeItem() const;

    std::pair<QString, TextRanges> readRanges(const Ranges &ranges) const;

    /**
     * Retruns true if a top bar is shown.
     */
    bool hasTopBar() const;

    QHash<int, TextRangesVerseModule *> m_modules;
    QMap<int, QPoint> m_points;

    void setLastTextRanges(TextRanges *textRanges);

    TextRanges *lastTextRanges();

    /*void setLastUrl(const VerseUrl &url);
    VerseUrl lastVerseUrl();*/

    void clearData();

    bool contains(const int moduleID);
private:
    inline QString title(TextRangesVerseModule *module, const QString &active, const int bibleListID) const;
    int countInCol(const int col) const;
    int m_activeItem;
    TextRanges *m_lastTextRanges;
   /* VerseUrl m_lastUrl;*/

};

#endif // VERSETABLE_H
