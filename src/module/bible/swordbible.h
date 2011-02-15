/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#ifndef SWORDBIBLE_H
#define SWORDBIBLE_H
#include "src/core/dbghelper.h"
#include "src/module/bible/biblemodule.h"

#include "CLucene.h"
#include "CLucene/_clucene-config.h"

class SwordBible : public BibleModule
{
public:
    SwordBible();
    void setSettings(Settings *settings);
    void loadBibleData(const int id, const QString &uid);
    int readBook(const int id);

    QString readInfo(QFile &file);
    QString readInfo(const QString &fileName);
    void search(const SearchQuery &query, SearchResult *res) const;
    bool hasIndex() const;
    void buildIndex();

    int moduleID() const;
    QString modulePath() const;
    QString moduleName(bool preferShortName = false) const;
    QString uid() const;
private:
    int m_moduleID;
    QString m_modulePath;
    QString indexPath() const;
};

#endif // SWORDBIBLE_H
