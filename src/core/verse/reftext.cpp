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
#include "reftext.h"
//Heb{sep_book_chapter} 1:2
const QString sep_book_chaper = " ";
//Heb 1{sep_chapter_verse}2
const QString sep_chapter_verse = ":";
//Heb 1:2{sep_two_verse}3
const QString sep_two_verse = ".";
//Heb 1:2{sep_verse_range}19
const QString sep_verse_range = "-";
//Heb 1:2-19{sep_next}Joh 1:12
const QString sep_next = " ";
//Heb 1:2-19 {sep_same_book}2:18
const QString sep_same_book = "";

RefText::RefText()
{
    m_showModuleName = false;
}
void RefText::setSettings(Settings *settings)
{
    m_settings = settings;
}

QString RefText::toString(const VerseUrl &url)
{
    QString ret = "";
    int prevBook = -2;
    foreach(VerseUrlRange range, url.ranges()) {
        if(!ret.isEmpty())
            ret += sep_next;
        //fix
        if(range.endVerse() == VerseUrlRange::LoadVerseNotSet)
            range.setEndVerse(range.startVerseID());

        ret += toString(range.moduleID(), range.bookID(), range.chapterID(), range.startVerseID(), range.endVerseID(), prevBook);
        prevBook = range.bookID();
    }
    if(!url.ranges().isEmpty() && m_showModuleName) {
        const VerseUrlRange r = url.ranges().first();
        if(r.module() == VerseUrlRange::LoadModuleByID)
            ret += " (" + m_settings->getModuleSettings(r.moduleID())->name(true) + ")";
        //todo: load by uid
    }
    return ret;
}
QString RefText::toString(const Ranges &ranges)
{
    QString ret = "";
    int prevBook = -2;
    foreach(const Range & range, ranges.getList()) {
        if(!ret.isEmpty())
            ret += sep_next;
        ret += toString(range.moduleID(), range.bookID(), range.chapterID(), range.startVerseID(), range.endVerseID(), prevBook);
        prevBook = range.bookID();
    }
    if(!ranges.getList().isEmpty() && m_showModuleName) {
        const Range r = ranges.getList().first();
        if(r.moduleID() != -1)
            ret += " (" + m_settings->getModuleSettings(r.moduleID())->name(true) + ")";
    }
    return ret;
}
void RefText::setShowModuleName(bool show)
{
    m_showModuleName = show;
}
QString RefText::toString(int moduleID, int bookID, int chapterID, int startVerseID, int endVerseID, int prevBook)
{
    QString ret;
    if(bookID != prevBook) {
        QSharedPointer<Versification> v11n;
        if(moduleID == -1) {
            v11n = m_settings->defaultVersification;
        } else {
            v11n = m_settings->getModuleSettings(moduleID)->getV11n();
        }
        if(v11n == NULL)
            v11n = m_settings->defaultVersification;

        if(v11n != NULL) {
            ret += v11n->bookName(bookID, true) + sep_book_chaper +
                   QString::number(chapterID + 1);
        } else {
            ret += QString::number(bookID) + sep_book_chaper +
                   QString::number(chapterID + 1);
        }
    } else {
        ret += sep_same_book + QString::number(chapterID + 1);
    }
    //verse
    if(startVerseID == endVerseID) {
        ret += sep_chapter_verse + QString::number(startVerseID + 1);
    } else if(startVerseID + 1 == endVerseID) {
        ret += sep_chapter_verse + QString::number(startVerseID + 1) + sep_two_verse + QString::number(endVerseID + 1);
    } else {
        ret += sep_chapter_verse + QString::number(startVerseID + 1) + sep_verse_range + QString::number(endVerseID + 1);
    }
    return ret;
}
