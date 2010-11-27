/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger <metaxy@walger.name>
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

#ifndef SEARCHPROVIDERS_H
#define SEARCHPROVIDERS_H
#include <QtCore/QList>
#include "searchprovider.h"
class SearchProviders
{

public:
    SearchProviders();
    virtual ~SearchProviders();

    void loadProviders();
    QList<SearchProvider *> getProviders(const int &count);
    //void setUsed
};

#endif // SEARCHPROVIDERS_H
