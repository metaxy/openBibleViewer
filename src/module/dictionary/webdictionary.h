#ifndef WEBDICTIONARY_H
#define WEBDICTIONARY_H
#include "src/module/dictionary/dictionarymodule.h"
#include "src/module/simplemoduleclass.h"
#include "src/core/settings/moduledisplaysettings.h"
#include "src/module/searchablemodule.h"
#include <QtScript/QScriptEngine>
#include <QtCore/QUrl>

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

    QString pharseUrl(QUrl url);

private:
    bool m_loaded;

    QString m_pharseScript;
    QString m_pharseOutScript;

    QString m_name;
    QString m_shortName;
    QString m_desc;
    QString m_type;

    QString m_url;

    bool loaded();
    int m_loadedModuleID;

   QScriptEngine myEngine;
};

#endif // WEBDICTIONARY_H
