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
#ifndef TEXTRANGE_H
#define TEXTRANGE_H
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QMap>
#include "src/core/bible/verse.h"
class TextRange
{
public:
    TextRange();
    QString join(const QString &seperator) const;
    void addVerse(const Verse &verse);
    void addVerse(const QList<Verse> &verse);
    void setTitle(const QString &title);
    QList<Verse> verseList() const;
    QMap<int, Verse> verseMap() const;
    QString title() const;

    Verse getVerse(const int &verseID) const;
    bool hasVerse(const int &verseID) const;

    void setBookID(const int &bookID);
    void setChapterID(const int &chapterID);
    void setModuleID(const int &id);

    int bookID() const;
    int chapterID() const;
    int moduleID() const;
private:
    QMap<int, Verse> m_verse;
    QString m_title;

    int m_bookID;
    int m_chapterID;
    int m_moduleID;
};

#endif // TEXTRANGE_H
