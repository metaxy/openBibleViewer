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
#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H
#include "searchhit.h"
#include "src/core/search/searchquery.h"
#include <QList>
#include <QString>
#include <QListWidget>
/**
 * SearchResult represents the results for a search. It contains all the all the hits (SearchHit) and the query (SearchQuery)
 */
class SearchResult
{

public:
    SearchResult();

    void addHit(const SearchHit &hit);
    SearchQuery searchQuery;
    void sort();

    QList<SearchHit> hits();
    QList<SearchHit> hits(SearchHit::SearchHitType type);
private:
    QList<SearchHit> m_hits;
    QMap<int, int> m_type;


};

#endif // SEARCHRESULT_H
