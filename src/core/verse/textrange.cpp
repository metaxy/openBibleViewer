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
#include "textrange.h"

TextRange::TextRange()
{
    m_verseTableID = 0;//the default versetableid is 0
    m_bookID = -1;
    m_chapterID = -1;
    m_moduleID = -1;
    m_error = TextRange::NoError;
}
QString TextRange::join(const QString &seperator) const
{
    QString out;
    QMapIterator<int, Verse> i(m_verse);
    while(i.hasNext()) {
        i.next();
        out += i.value().data() + (i.hasNext() ? seperator : "");
    }
    return out;
}

void TextRange::addVerse(const Verse &verse)
{
    m_verse.insert(verse.verseID(), verse);
}

void TextRange::addVerse(const QList<Verse> &verse)
{
    foreach(const Verse & v, verse) {
        addVerse(v);
    }
}

void TextRange::setTitle(const QString &title)
{
    m_title = title;
}

QList<Verse> TextRange::verseList() const
{
    return m_verse.values();
}
QMap<int, Verse> TextRange::verseMap() const
{
    return m_verse;
}
QString TextRange::title() const
{
    return m_title;
}
Verse TextRange::getVerse(const int verseID) const
{
    return m_verse.value(verseID);
}
bool TextRange::hasVerse(const int verseID) const
{
    return m_verse.contains(verseID);
}
void TextRange::setBookID(const int bookID)
{
    m_bookID = bookID;
}
void TextRange::setChapterID(const int chapterID)
{
    m_chapterID = chapterID;
}
int TextRange::bookID() const
{
    return m_bookID;
}
int TextRange::chapterID() const
{
    return m_chapterID;
}
int TextRange::moduleID() const
{
    return m_moduleID;
}
void TextRange::setModuleID(const int id)
{
    m_moduleID = id;
}
int TextRange::verseTableID() const
{
    return m_verseTableID;
}
void TextRange::setVerseTableID(int verseTableID)
{
    m_verseTableID = verseTableID;
}
void TextRange::setError(const TextRange::TextRangeError error)
{
    m_error = error;
}

TextRange::TextRangeError TextRange::error() const
{
    return m_error;
}
