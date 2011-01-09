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
#include "verseselection.h"

VerseSelection::VerseSelection()
{
    startVerse = 0;
    endVerse = 0;
    selectedText = "";
    shortestStringInStartVerse = "";
    shortestStringInEndVerse = "";
    repeat = 0;
    moduleID = -1;
    chapterID = -1;
    bookID = -1;
    repeat = -1;
    longestString = "";
    m_readyForMarks = false;
}
bool VerseSelection::canBeUsedForMarks()
{
    return m_readyForMarks;
}
void VerseSelection::setCanBeUsedForMarks(bool newValue)
{
    m_readyForMarks = newValue;
}
QString VerseSelection::typeToString()
{
    if(type == VerseSelection::RepeatOfLongestString) {
        return "RepeatOfLongestString";
    } else if(type == VerseSelection::ShortestString) {
        return "ShortestString";
    }
    return QString();
}
QString VerseSelection::typeToString(const int &t)
{
    if(t == VerseSelection::RepeatOfLongestString) {
        return "RepeatOfLongestString";
    } else if(t == VerseSelection::ShortestString) {
        return "ShortestString";
    }
    return QString();
}
VerseSelection::SelectionPosInTextType VerseSelection::typeFromString(const QString &t)
{
    if(t == "RepeatOfLongestString") {
        return VerseSelection::RepeatOfLongestString;
    } else if(t == "ShortestString") {
        return VerseSelection::ShortestString;
    }
    return VerseSelection::ShortestString;//default
}
