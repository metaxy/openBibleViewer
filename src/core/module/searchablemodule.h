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
#ifndef SEARCHABLEMODULE_H
#define SEARCHABLEMODULE_H

#include "src/core/search/searchquery.h"
#include "src/core/search/searchresult.h"

class SearchableModule
{
public:
    SearchableModule();

    /**
     * Searchs in the current Module. Saves the results in the second argument (*result).
     */
    virtual void search(SearchQuery query, SearchResult *result);
};

#endif // SEARCHABLEMODULE_H
