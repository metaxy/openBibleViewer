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
#include "reftext.h"
const QString sep_book_chaper = " ";
const QString sep_chapter_verse = " ";
const QString sep_two_verse = ".";
const QString sep_verse_range = "-";
const QString sep_next = " ";
RefText::RefText()
{
    m_showModuleName = true;
}
QString RefText::toString(const VerseUrl &url)
{
    QString ret = "";
    foreach(const VerseUrlRange & range, url.ranges()) {
        if(!ret.isEmpty())
            ret += sep_next;
        ret += toString(range.moduleID(), range.bookID(), range.chapterID(), range.startVerseID(), range.endVerseID());
    }
    if(!url.ranges().isEmpty()) {
        const VerseUrlRange r = url.ranges().first();
        //todo: wir haben nicht immer einen short name
        ret += " (" + m_settings->getModuleSettings(r.moduleID())->name(true) + ")";
    }
    return ret;
}
QString RefText::toString(const Ranges &ranges)
{
    QString ret = "";
    foreach(const Range & range, ranges.getList()) {
        if(!ret.isEmpty())
            ret += sep_next;
        ret += toString(range.moduleID(), range.bookID(), range.chapterID(), range.startVerseID(), range.endVerseID());
    }
    if(!ranges.getList().isEmpty()) {
        const Range r = ranges.getList().first();
        //todo: wir haben nicht immer einen short name
        ret += " (" + m_settings->getModuleSettings(r.moduleID())->name(true) + ")";
    }
    return ret;
}
void RefText::setShowModuleName(bool show)
{
    m_showModuleName = show;
}
QString RefText::toString(int moduleID, int bookID, int chapterID, int startVerseID, int endVerseID)
{
    QString ret;
    ret += m_settings->getModuleSettings(moduleID)->v11n->bookName(bookID) + sep_book_chaper +
           QString::number(chapterID + 1);
    if(startVerseID == endVerseID) {
        ret += sep_chapter_verse + QString::number(startVerseID + 1);
    } else if(startVerseID + 1 == endVerseID) {
        ret += sep_chapter_verse + QString::number(startVerseID + 1) + sep_two_verse + QString::number(endVerseID + 1);
    } else {
        ret += sep_chapter_verse + QString::number(startVerseID + 1) + sep_verse_range + QString::number(endVerseID + 1);
    }
    return ret;
}
