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
#ifndef TEXTRANGES_H
#define TEXTRANGES_H
#include <QtCore/QList>
#include <QtCore/QSet>
#include "src/core/bible/textrange.h"
class TextRanges
{
public:
    TextRanges();
    void addTextRange(const TextRange &range);
    void addTextRanges(const TextRanges &ranges);
    QList<TextRange> textRanges() const;
    QString join(const QString &seperator) const;

    QSet<int> booksIDs();
    QSet<int> chapterIDs();
private:
    QList<TextRange> m_ranges;
};

#endif // TEXTRANGES_H
