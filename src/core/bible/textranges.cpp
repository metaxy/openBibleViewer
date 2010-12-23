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
#include "textranges.h"

TextRanges::TextRanges()
{
}
QString TextRanges::join(const QString &seperator) const
{
    QString out;
    foreach(const TextRange & r, m_ranges) {
        out += r.join(seperator);
    }
    return out;
}
void TextRanges::addTextRange(const TextRange &range)
{
    m_ranges.append(range);
}
void TextRanges::addTextRanges(const TextRanges &ranges)
{
    m_ranges.append(ranges.textRanges());
}

QList<TextRange> TextRanges::textRanges() const
{
    return m_ranges;
}
QSet<int> TextRanges::booksIDs()
{
    QSet<int> ret;
    foreach(TextRange range, m_ranges) {
        ret.insert(range.bookID());
    }
    return ret;
}
QSet<int> TextRanges::chapterIDs()
{
    QSet<int> ret;
    foreach(TextRange range, m_ranges) {
        ret.insert(range.chapterID());
    }

    return ret;
}
Verse TextRanges::getVerse(const int &bookID, const int &chapterID, const int &verseID)
{
    foreach(TextRange range, m_ranges) {
        if(range.bookID() == bookID && range.chapterID() == chapterID && range.hasVerse(verseID))
            return range.getVerse(verseID);
    }
    return Verse();
}
int TextRanges::verseCount() const
{
    int count = 0;
    foreach(TextRange range, m_ranges) {
        count += range.verseList().size();
    }
    return count;
}
bool TextRanges::isEmpty()
{
    return m_ranges.isEmpty();
}
