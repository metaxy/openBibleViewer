#ifndef WEBPAGE_H
#define WEBPAGE_H
#include "src/module/simplemoduleclass.h"
#include "src/core/settings/moduledisplaysettings.h"
#include "src/module/searchablemodule.h"
#include <QtCore/QUrl>
class WebPage : public SimpleModuleClass, public SearchableModule
{
public:
    WebPage();

    void loadModuleData(const int moduleID, const QString &name = "");
    QUrl getUrl();
    void search(SearchQuery query, SearchResult *result);

    MetaInfo readInfo(const QString &name);
private:
    QString m_name;
    QString m_shortName;
    QString m_desc;

    QString m_url;

    bool m_loaded;

    bool loaded();
    int m_loadedModuleID;
};

#endif // WEBPAGE_H
