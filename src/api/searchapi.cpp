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
#include "searchapi.h"
#include "src/core/module/bible/bible.h"
SearchApi::SearchApi(QObject *parent) :
    QObject(parent)
{
}
QVariantList SearchApi::search(const int moduleID, const QString &queryString)
{
    DEBUG_FUNC_NAME
    SearchQuery query;
    query.searchText = queryString;
    query.queryType = SearchQuery::Simple;
    query.range = SearchQuery::Whole;

    SearchResult *result = new SearchResult();
    result->searchQuery = query;
    QVariantList res;

    Bible *module = (Bible*) m_moduleManager->newTextRangesVerseModule(moduleID);
    if(module != nullptr) {
        module->search(query, result);

        result->sort();
        foreach(const SearchHit &h, result->hits()) {
            QVariantMap m;
            QMapIterator<int, QVariant> i(h.m_map);
            while (i.hasNext()) {
                 i.next();
                 m[QString::number(i.key())] = i.value();
             }

            res << QVariant(m);
        }

        delete result;
        delete module;
    }
    return res;
}

QString SearchApi::name() const
{
    return "Search";
}


QString SearchApi::connectorJS() const
{
    return "";
}



