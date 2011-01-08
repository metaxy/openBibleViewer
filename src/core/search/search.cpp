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
#include "search.h"

Search::Search()
{
}
SearchResult* Search::search(SearchQuery query)
{
    SearchResult *result = new SearchResult();
    result->searchQuery = query;
    m_moduleManager->verseModule()->search(query, result);
    if(query.searchInNotes) {
        m_notes->search(query, result);
    }
    result->sort();//todo: move maybe to the display section, to avoid double sorting if there is the possibilty to change sort settings
    return result;
}
