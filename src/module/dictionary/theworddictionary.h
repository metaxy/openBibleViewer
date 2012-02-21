#ifndef THEWORDDICTIONARY_H
#define THEWORDDICTIONARY_H
#include "src/module/dictionary/dictionarymodule.h"
#include "src/module/metainfo.h"
#include <QtSql/QSqlDatabase>
class TheWordDictionary : public DictionaryModule
{
public:
    TheWordDictionary();

    MetaInfo readInfo(const QString &name);
    Response* getEntry(const QString &entry);

    QStringList getAllKeys();

    Response::ResponseType responseType() const;
private:
    int loadModuleData(const int moduleID);
    QSqlDatabase m_db;
};

#endif // THEWORDDICTIONARY_H
