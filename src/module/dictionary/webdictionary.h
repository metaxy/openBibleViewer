#ifndef WEBDICTIONARY_H
#define WEBDICTIONARY_H
#include "src/module/dictionary/dictionarymodule.h"
#include "src/module/simplemoduleclass.h"
#include "src/core/settings/moduledisplaysettings.h"
#include "src/module/searchablemodule.h"
#include <QtScript/QScriptEngine>

class WebDictionary : public SearchableModule, public DictionaryModule
{
public:
    WebDictionary();
    ~WebDictionary();
    void loadModuleData(const int moduleID, const QString &name = "");

    QString getEntry(const QString &entry);
    QStringList getAllKeys();

    void search(SearchQuery query, SearchResult *result);

    MetaInfo readInfo(const QString &name);

private:
    bool m_loaded;

    QString m_pharseScript;

    QString m_name;
    QString m_shortName;
    QString m_desc;

    QString m_url;

    bool loaded();
    int m_loadedModuleID;

   QScriptEngine myEngine;
};

#endif // WEBDICTIONARY_H
