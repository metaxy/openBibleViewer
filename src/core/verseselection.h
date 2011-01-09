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
#ifndef VERSESELECTION_H
#define VERSESELECTION_H
#include <QtCore/QString>
class VerseSelection
{
public:
    VerseSelection();
    enum SelectionPosInTextType {
        ShortestString,
        RepeatOfLongestString,
        NotSet
    };

    SelectionPosInTextType type;

    int startVerse;
    int endVerse;

    int moduleID;
    int bookID;
    int chapterID;

    QString selectedText;
    QString shortestStringInStartVerse;
    QString shortestStringInEndVerse;

    QString longestString;
    int repeat;

    bool canBeUsedForMarks();
    void setCanBeUsedForMarks(bool newValue);

    QString typeToString();
    static QString typeToString(const int &t);

    static VerseSelection::SelectionPosInTextType typeFromString(const QString &t);
private:
    bool m_readyForMarks;
};

#endif // VERSESELECTION_H
