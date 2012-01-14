#ifndef SWORDLEXICON_H
#define SWORDLEXICON_H
#include "src/module/searchablemodule.h"
#include "src/module/dictionary/dictionarymodule.h"

class SwordLexicon : public SearchableModule, public DictionaryModule
{
public:
    SwordLexicon();
    ~SwordLexicon();

    QString getEntry(const QString &entry);
    QStringList getAllKeys();

    void search(SearchQuery query, SearchResult *result);

    MetaInfo readInfo(const QString &name);
};

#endif // SWORDLEXICON_H
