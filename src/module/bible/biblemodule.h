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
#ifndef BIBLEMODULE_H
#define BIBLEMODULE_H
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QMap>

#include "src/core/search/searchresult.h"
#include "src/core/search/searchquery.h"

#include "src/core/settings/settings.h"

#include "src/core/verse/versebook.h"
#include "src/core/verse/textrange.h"
#include "src/core/dbghelper.h"
#include "src/module/metainfo.h"
#include "src/module/simplemodule.h"
#include <utility>
/**
  * This is an abstract class for bible modules, which are used in the Bible Class. It represents the data.
  */
class BibleModule : public SimpleModule
{
public:
    BibleModule();
    virtual ~BibleModule();

    virtual int loadBibleData(const int moduleID, const QString &path) = 0;

    virtual TextRange rawTextRange(int bookID, int chapterID, int startVerse, int endVerse) = 0;
    virtual std::pair<int, int> minMaxVerse(int bookID, int chapterID) = 0;

    virtual void search(const SearchQuery &query, SearchResult *res) const = 0;
    virtual bool hasIndex() const = 0;
    virtual void buildIndex() = 0;

    virtual int moduleID() const = 0;
    virtual QString modulePath() const = 0;
    virtual QString moduleName(bool preferShortName = false) const = 0;

    virtual QSharedPointer<Versification> versification() const;
    virtual QString uid() const = 0;
    virtual void clearData() = 0;
protected:
    QSharedPointer<Versification> m_versification;
};

#endif // BIBLEMODULE_H
