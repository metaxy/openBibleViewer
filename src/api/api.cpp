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
#include "api.h"

Api::Api(QObject *parent) :
    QObject(parent)
{
    m_moduleApi = NULL;
    m_notesApi = NULL;
    m_searchApi = NULL;
}
void Api::init()
{
    m_moduleApi = new ModuleApi(this);
    setAll(m_moduleApi);

    m_notesApi = new NotesApi(this);
    setAll(m_notesApi);
    m_notesApi->init();

    m_searchApi = new SearchApi(this);
    setAll(m_searchApi);

}
Api::~Api()
{
    if(m_moduleApi != NULL) {
        delete m_moduleApi;
        m_moduleApi = NULL;
    }
    if(m_notesApi != NULL) {
        delete m_moduleApi;
        m_moduleApi = NULL;
    }
    if(m_searchApi != NULL) {
        delete m_searchApi;
        m_searchApi = NULL;
    }
}

ModuleApi* Api::moduleApi() const
{
    return m_moduleApi;
}
NotesApi* Api::notesApi() const
{
    return m_notesApi;
}
SearchApi* Api::searchApi() const
{
    return m_searchApi;
}
