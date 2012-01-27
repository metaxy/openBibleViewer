#ifndef BOOKMODULE_H
#define BOOKMODULE_H
#include <QtCore/QFile>
#include "src/core/search/searchresult.h"
#include "src/core/search/searchquery.h"

#include "src/core/settings/settings.h"
#include "src/module/metainfo.h"

class BookModule
{
public:
    BookModule();
    virtual ~BookModule();
    void setSettings(Settings *settings);

    virtual MetaInfo readInfo(QFile &file) = 0;

    virtual void search(const SearchQuery &query, SearchResult *res) const = 0;
    virtual bool hasIndex() const = 0;
    virtual void buildIndex() = 0;

    virtual int moduleID() const = 0;
    virtual QString modulePath() const = 0;
    virtual QString moduleName(bool preferShortName = false) const = 0;

    virtual int loadModuleData(const int moduleID, const QString &path) = 0;

    virtual QString readAll() = 0;
protected:
    Settings *m_settings;

    QString m_path;
    int m_moduleID;
};

#endif // BOOKMODULE_H
