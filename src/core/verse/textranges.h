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
#ifndef TEXTRANGES_H
#define TEXTRANGES_H
#include <QtCore/QList>
#include <QtCore/QSet>
#include "src/core/verse/textrange.h"
#include "src/core/link/verseurlrange.h"
#include "src/core/dbghelper.h"
#include "src/core/verse/ranges.h"

/**
  * TextRanges is a list of TextRange[s].
  * With some additional functionality.
  */
class TextRanges
{
public:

    TextRanges();
    TextRanges(const TextRanges &r);
    void addTextRange(const TextRange &range);
    void addTextRanges(const TextRanges &ranges);
    QList<TextRange> textRanges() const;
    QString join(const QString &seperator) const;

    void setSource(const Ranges &url);
    Ranges source() const;

    Verse getVerse(const int bookID, const int chapterID, const int verseID);
    int verseCount() const;
    bool isEmpty();

    int minBookID() const;
    int maxBookID() const;
    QSet<int> bookIDs() const;

    int minChapterID() const;
    int maxChapterID() const;
    QSet<int> chapterIDs() const;

    int minChapterID(const int bookID) const;
    int maxChapterID(const int bookID) const;
    QSet<int> chapterIDs(const int bookID) const;

    int minVerseID() const;
    int maxVerseID() const;
    QSet<int> verseIDs() const;

    int minVerseID(const int bookID, const int chapterID) const;
    int maxVerseID(const int bookID, const int chapterID) const;
    QSet<int> verseIDs(const int bookID, const int chapterID) const;

    bool contains(const int bookID, const int chapterID) const;
    QList<VerseUrlRange> toBibleUrlRanges() const;
    QList<VerseUrlRange> toBibleUrlRanges(int verseTableID) const;

    void setVerseTableID(int verseTableID);

    TextRange::TextRangeError error() const;
private:
    QList<TextRange> m_ranges;

    Ranges m_source;
};

#endif // TEXTRANGES_H
