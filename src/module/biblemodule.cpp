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
#include "biblemodule.h"
#include <QtCore/QString>
#include <QtCore/QFile>
#include "src/core/search/searchquery.h"
#include "src/core/search/searchresult.h"
#include "src/core/dbghelper.h"
BibleModule::BibleModule()
{
}
void BibleModule::setSettings(Settings *settings)
{
    //DEBUG_FUNC_NAME
    m_settings = settings;
}
int BibleModule::readBook(const int &id)
{
    myWarning() << "calling BibleModule";
    return 0;
}

void BibleModule::loadBibleData(const int &bibleID, const QString &path)
{
    myWarning() << "calling BibleModule";
}

QString BibleModule::readInfo(QFile &file)
{
    myWarning() << "calling BibleModule";
    return "";
}

void BibleModule::search(const SearchQuery &query, SearchResult *res) const
{
    myWarning() << "calling BibleModule";
}

bool BibleModule::hasIndex() const
{
    myWarning() << "calling BibleModule";
    return false;
}
void BibleModule::buildIndex()
{
    myWarning() << "calling BibleModule";
}
int BibleModule::moduleID() const
{
    myWarning() << "calling BibleModule";
    return 0;
}
QString BibleModule::modulePath() const
{
    myWarning() << "calling BibleModule";
    return "";
}
QString BibleModule::moduleName(bool preferShortName) const
{
    myWarning() << "calling BibleModule";
    return "";
}
QMap<int, int> BibleModule::bookCount() const
{
    myWarning() << "calling BibleModule";
    return QMap<int, int> ();
}
BookNames BibleModule::getBookNames()
{
    myWarning() << "calling BibleModule";
    BookNames m;
    return m;
}

Book BibleModule::book() const
{
    myWarning() << "calling BibleModule";
    Book b;
    return b;
}
