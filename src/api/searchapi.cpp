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

