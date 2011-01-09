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
#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H
#include "searchhit.h"
#include "src/core/search/searchquery.h"
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGui/QListWidget>
/*!
 Searchresult represents the results for a search

 @author Paul Walger <metaxy@walger.name>
*/
class SearchResult
{
public:
    SearchResult();
    void addHit(SearchHit hit);
    QList<SearchHit> hits();
    QList<SearchHit> hits(SearchHit::SearchHitType type);
    SearchQuery searchQuery;
    void sort();

private:
    QList<SearchHit> m_hits;
    QMap<int, int> m_type;


};

#endif // SEARCHRESULT_H
