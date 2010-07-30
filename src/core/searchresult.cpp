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
#include "searchresult.h"

SearchResult::SearchResult()
{
}
void SearchResult::addHit(SearchHit hit)
{
    m_type.insert(m_hits.size(), hit.type());
    m_hits << hit;
}
QList<SearchHit> SearchResult::hits()
{
    return m_hits;
}
QList<SearchHit> SearchResult::hits(SearchHit::SearchHitType type)
{
    QList<SearchHit> l;
    foreach(int id, m_type.keys(type)) {
        l << m_hits.at(id);
    }
    return l;
}
