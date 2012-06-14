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
#ifndef TREEBOOKMODULE_H
#define TREEBOOKMODULE_H
#include "src/core/search/searchresult.h"

#include "src/core/books/booktree.h"
#include "src/module/response/response.h"
#include "src/module/simplemodule.h"
class TreeBookModule : public SimpleModule
{
public:
    TreeBookModule();
    virtual ~TreeBookModule();

    virtual void search(const SearchQuery &query, SearchResult *res) const = 0;
    virtual bool hasIndex() const = 0;
    virtual void buildIndex() = 0;

    virtual int moduleID() const = 0;
    virtual QString modulePath() const = 0;
    virtual QString moduleName(bool preferShortName = false) const = 0;

    virtual int loadModuleData(const int moduleID, const QString &path) = 0;

    virtual Response* readChapter(const int chapterID) = 0;
    virtual BookTree * bookTree() = 0;
protected:

    QString m_path;
    int m_moduleID;
    BookTree *m_tree;
};

#endif // TREEBOOKMODULE_H
