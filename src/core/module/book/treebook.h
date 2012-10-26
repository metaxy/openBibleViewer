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
#ifndef TREEBOOK_H
#define TREEBOOK_H
#include "src/core/module/simplemoduleclass.h"
class TreeBook : public SimpleModuleClass, public SearchableModule
{
public:
    TreeBook();
    void search(SearchQuery query, SearchResult *result);
    void clearData();
    BookTree *bookTree();
    Response *readChapter(int chapterID);
    int loadModuleData(int moduleID);
protected:
    ModuleDisplaySettings *m_moduleDisplaySettings;


    bool m_loaded;

    QString m_moduleTitle;
    QString m_moduleShortTitle;
    QString m_moduleUID;
    QSharedPointer<TreeBookModule> m_bookModule;
};

#endif // TREEBOOK_H
