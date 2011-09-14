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
#ifndef SEARCHHIT_H
#define SEARCHHIT_H
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QMap>
/**
 * SearchHit represents a hit of a search.
 */
class SearchHit
{
public:
    SearchHit();
    enum SearchHitType {
        BibleHit = 0,
        NoteHit = 1
    };
    enum SearchHitKey {
        ModuleID = 0,
        BookID = 1,
        ChapterID = 2,
        VerseID = 3,
        VerseText = 4,
        NoteID = 5,
        NoteText = 6,
        Score = 7
    };
    SearchHitType type() const;
    void setType(SearchHitType type);
    QVariant value(SearchHitKey v) const;
    void setValue(SearchHitKey k, QVariant value);



private:
    SearchHitType m_type;
    QMap<int, QVariant> m_map;

};

#endif // SEARCHHIT_H
