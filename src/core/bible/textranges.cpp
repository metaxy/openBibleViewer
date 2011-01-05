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


int TextRanges::minBookID() const
{
    int ret = -1;
    foreach(const TextRange &r, m_ranges) {
        if((r.bookID() < ret || ret == -1))
            ret = r.bookID();
    }
    return ret;
}
int TextRanges::maxBookID() const
{
    int ret = -1;
    foreach(const TextRange &r, m_ranges) {
        if((r.bookID() > ret) )
            ret = r.bookID();
    }
    return ret;
}

QSet<int> TextRanges::bookIDs() const
{
    QSet<int> ret;
    foreach(TextRange range, m_ranges) {
        ret.insert(range.bookID());
    }
    return ret;
}
int TextRanges::minChapterID() const
{
    return minChapterID(minBookID());
}
int TextRanges::maxChapterID() const
{
    return maxChapterID(minBookID());
}
QSet<int> TextRanges::chapterIDs() const
{
    return chapterIDs(minBookID());
}
int TextRanges::minChapterID(const int &bookID) const
{
    int ret = -1;
    foreach(const TextRange &r, m_ranges) {
        if((r.chapterID() < ret || ret == -1) && r.bookID() == bookID)
            ret = r.chapterID();
    }
    return ret;
}
int TextRanges::maxChapterID(const int &bookID) const
{
    int ret = -1;
    foreach(const TextRange &r, m_ranges) {
        if((r.chapterID() > ret) && r.bookID() == bookID)
            ret = r.chapterID();
    }
    return ret;
}
QSet<int> TextRanges::chapterIDs(const int &bookID) const
{
    QSet<int> ret;
    foreach(const TextRange &range, m_ranges) {
        if(bookID == range.bookID())
            ret.insert(range.chapterID());
    }
    return ret;
}

int TextRanges::minVerseID() const
{
    const int bookID = minBookID();
    return minVerseID(bookID, minChapterID(bookID));
}
int TextRanges::maxVerseID() const
{
    const int bookID = minBookID();
    return maxVerseID(bookID, minChapterID(bookID));
}
QSet<int>TextRanges::verseIDs() const
{
    const int bookID = minBookID();
    return verseIDs(bookID, minChapterID(bookID));
}
int TextRanges::minVerseID(const int &bookID, const int &chapterID) const
{
    int ret = -1;
    foreach(const TextRange &r, m_ranges) {
        if(r.bookID() == bookID && r.chapterID() == chapterID) {
            QList<Verse> verseList = r.verseList();
            int min = -1;
            foreach(const Verse &verse, verseList) {
                if(verse.verseID() < min || min == -1)
                    min = verse.verseID();
            }
            if(ret == -1 || min < ret)
                ret = min;
        }
    }
    return ret;
}
int TextRanges::maxVerseID(const int &bookID, const int &chapterID) const
{
    int ret = -1;
    foreach(const TextRange &r, m_ranges) {
        if(r.bookID() == bookID && r.chapterID() == chapterID) {
            QList<Verse> verseList = r.verseList();
            int max = -1;
            foreach(const Verse &verse, verseList) {
                if(verse.verseID() > max)
                    max = verse.verseID();
            }
            if(max > ret)
                ret = max;
        }
    }
    return ret;
}
QSet<int> TextRanges::verseIDs(const int &bookID, const int &chapterID) const
{
    QSet<int> ret;
    foreach(const TextRange &range, m_ranges) {
        if(bookID == range.bookID() && chapterID == range.chapterID()) {
            QSet<int> ids;
            foreach(const Verse &verse, range.verseList()) {
                ids.insert(verse.verseID());
            }
            ret.unite(ids);
        }
    }
    return ret;
}

bool TextRanges::contains(const int &bookID, const int &chapterID) const
{
    foreach(const TextRange &r, m_ranges) {
        if(r.bookID() == bookID && r.chapterID() == chapterID)
            return true;
    }
    return false;
}
