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
#ifndef SEARCHHIT_H
#define SEARCHHIT_H
#include <QtCore/QString>
/*!
 SearcHit represent a hit in a bible search

 @author Paul Walger <metaxy@walger.name>
*/
class SearchHit
{
public:
    SearchHit(const int &bibleID, const int &bookID, const int &chapterID, const int &verseID, const QString &text);
    int bookID();
    int chapterID();
    int verseID();
    int bibleID();
    QString text();
private:
    int m_bookID;
    int m_chapterID;
    int m_verseID;
    int m_bibleID;
    QString m_text;
};

#endif // SEARCHHIT_H
