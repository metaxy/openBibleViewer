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
#include <QtCore/QMap>
#include "src/core/dbghelper.h"
bool searchResultLessThanByVerse(const SearchHit &hit1, const SearchHit &hit2)
{
    if(hit1.type() == SearchHit::BibleHit && hit2.type() == SearchHit::BibleHit) {
        if(hit1.value(SearchHit::BibleID) != hit2.value(SearchHit::BibleID)) {
            return hit1.value(SearchHit::BibleID).toInt() < hit2.value(SearchHit::BibleID).toInt();
        }
        if(hit1.value(SearchHit::BookID) != hit2.value(SearchHit::BookID)) {
            return hit1.value(SearchHit::BookID).toInt() < hit2.value(SearchHit::BookID).toInt();
        }
        if(hit1.value(SearchHit::ChapterID) != hit2.value(SearchHit::ChapterID)) {
            return hit1.value(SearchHit::ChapterID).toInt() < hit2.value(SearchHit::ChapterID).toInt();
        }
        if(hit1.value(SearchHit::VerseID) != hit2.value(SearchHit::VerseID)) {
            return hit1.value(SearchHit::VerseID).toInt() < hit2.value(SearchHit::VerseID).toInt();
        }
        return false;
    } else {
        return hit1.type() == SearchHit::BibleHit;
    }
}

SearchResult::SearchResult()
{
}
/**
  Adds a new hit to the search result.
*/
void SearchResult::addHit(SearchHit hit)
{
    m_hits << hit;
}
/**
  Returns all hits.
  */
QList<SearchHit> SearchResult::hits()
{
    return m_hits;
}
/**
  Returns all hits with the given type.
  */
QList<SearchHit> SearchResult::hits(SearchHit::SearchHitType type)
{
    QList<SearchHit> l;
    foreach(SearchHit h, m_hits) {
        if(h.type() == type)
            l << h;
    }
    return l;
}
/**
  Sort by verse.
  */
void SearchResult::sort()
{
    DEBUG_FUNC_NAME
    qSort(m_hits.begin(),m_hits.end(),searchResultLessThanByVerse);
}
