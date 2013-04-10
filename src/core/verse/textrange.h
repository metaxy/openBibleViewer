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
#ifndef TEXTRANGE_H
#define TEXTRANGE_H
#include <QString>
#include <QList>
#include <QMap>
#include "src/core/verse/verse.h"
/**
 * @brief Eigenlich its ein Kapitel mit ein paar Versen.
 */
class TextRange
{
public:
    enum TextRangeError {
        NoError = 0,
        FatalError = 1, //no such module or it cannot be loaded
        NotFoundError = 2 // book not available
    };

    TextRange();
    QString join(const QString &seperator) const;
    void addVerse(const Verse &verse);
    void addVerse(const QList<Verse> &verse);
    QList<Verse> verseList() const;
    QMap<int, Verse> verseMap() const;

    Verse getVerse(const int verseID) const;
    bool hasVerse(const int verseID) const;

    void setBookID(const int bookID);
    void setChapterID(const int chapterID);
    void setModuleID(const int id);
    void setVerseTableID(int verseTableID);

    int bookID() const;
    int chapterID() const;
    int moduleID() const;
    int verseTableID() const;


    void setError(const TextRange::TextRangeError error);
    TextRange::TextRangeError error() const;

    void filter(const int startVerse, const int endVerse);

private:
    QMap<int, Verse> m_verse;

    int m_bookID;
    int m_chapterID;
    int m_moduleID;
    int m_verseTableID;
    TextRange::TextRangeError m_error;
};

#endif // TEXTRANGE_H
