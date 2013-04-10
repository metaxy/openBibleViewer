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
#ifndef SEARCHQUERY_H
#define SEARCHQUERY_H
#include <QString>
/**
 * SearchQuery represents a search query. It consists mainly of a range and a query string.
 */
class SearchQuery
{
public:
    SearchQuery();
    enum Type {
        Simple = 0,
        Complex = 1
    };
    enum Range {
        Whole,
        OT,
        NT,
        Custom
    };

    int queryType;
    QString searchText;
    bool searchInNotes;
    Range range;
};

#endif // SEARCHQUERY_H
