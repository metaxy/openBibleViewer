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
#ifndef BOOKMODULE_H
#define BOOKMODULE_H
#include "src/core/search/searchresult.h"

#include "src/core/module/simplemodule.h"
#include "src/core/module/response/response.h"
class BookModule : public SimpleModule
{
public:
    BookModule();
    virtual ~BookModule();

    virtual void search(const SearchQuery &query, SearchResult *res) const = 0;
    virtual bool hasIndex() const = 0;
    virtual void buildIndex() = 0;

    virtual int moduleID() const = 0;
    virtual QString modulePath() const = 0;
    virtual QString moduleName(bool preferShortName = false) const = 0;

    virtual int loadModuleData(const int moduleID, const QString &path) = 0;

    virtual Response* readAll() = 0;
protected:

    QString m_path;
    int m_moduleID;
};

#endif // BOOKMODULE_H
