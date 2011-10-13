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
#ifndef RANGES_H
#define RANGES_H
#include "src/core/verse/range.h"
#include "src/core/link/verseurl.h"
#include <QtCore/QList>
class Ranges
{
public:
    Ranges();
    void addRange(Range range);
    void addRanges(Ranges range);
    QList<Range> getList() const;

    void setSource(const VerseUrl &url);
    VerseUrl source() const;

private:
    QList<Range> m_ranges;
    VerseUrl m_source;
};

#endif // RANGES_H
