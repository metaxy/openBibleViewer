#ifndef SWORDLEXICON_H
#define SWORDLEXICON_H
#include "src/module/searchablemodule.h"
#include "src/module/dictionary/dictionarymodule.h"
#include "src/module/metainfo.h"

#ifdef BUILD_WITH_SWORD
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <swmgr.h>
#include <swmodule.h>
#include <markupfiltmgr.h>
#include <versekey.h>
using namespace::sword;

#endif

class SwordLexicon : public SearchableModule, public DictionaryModule
{
public:
    SwordLexicon();
    ~SwordLexicon();

    Response *getEntry(const QString &entry);
    QStringList getAllKeys();

    void search(SearchQuery query, SearchResult *result);

    MetaInfo readInfo(const QString &name);
    Response::ResponseType responseType() const;

private:
    bool m_loaded;
    int load(const QString &id);
    QStringList m_entryList;

#ifdef BUILD_WITH_SWORD
    SWModule *m_target;
    SWMgr *m_library;
#endif
};

#endif // SWORDLEXICON_H
