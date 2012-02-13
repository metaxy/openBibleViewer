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
#ifndef BOOK_H
#define BOOK_H
#include "src/module/simplemoduleclass.h"
#include "src/core/settings/moduledisplaysettings.h"
#include "src/module/searchablemodule.h"
#include "src/module/response/response.h"

class Book : public SimpleModuleClass, public SearchableModule
{
public:
    Book();
    ~Book();
    void setModuleDisplaySettings(ModuleDisplaySettings *moduleDisplaySettings);
    void search(SearchQuery query, SearchResult *result);
    void clearData();

    Response* getAll();

protected:
    ModuleDisplaySettings *m_moduleDisplaySettings;


    int loadModuleData(int moduleID);
    bool m_loaded;

    QString m_moduleTitle;
    QString m_moduleShortTitle;
    QString m_moduleUID;
    QSharedPointer<BookModule> m_bookModule;
};

#endif // BOOK_H
