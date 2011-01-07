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
#ifndef BIBLEMODULE_H
#define BIBLEMODULE_H
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QMap>

#include "src/core/search/searchresult.h"
#include "src/core/search/searchquery.h"

#include "src/core/settings/settings.h"

#include "src/core/bible/booknames.h"
#include "src/core/bible/book.h"

#include "src/core/dbghelper.h"
/**
  * This is an abstract class for bible modules, which are used in the Bible Class.
  * All this virtual methods have to be implemented in the bible module.
  */
class BibleModule
{
public:
    BibleModule();
    virtual ~BibleModule() {}
    virtual void setSettings(Settings *settings);

    virtual void loadBibleData(const int &moduleID, const QString &path);
    virtual QString readInfo(QFile &file);
    virtual int readBook(const int &id);

    virtual void search(const SearchQuery &query, SearchResult *res) const;
    virtual bool hasIndex() const;
    virtual void buildIndex();

    virtual int moduleID() const;
    virtual QString modulePath() const;
    virtual QString moduleName(bool preferShortName = false) const;

    virtual Versification *versification() const;
    virtual Book book() const;
protected:
    Versification *m_versification;
    Settings *m_settings;
};

#endif // BIBLEMODULE_H
