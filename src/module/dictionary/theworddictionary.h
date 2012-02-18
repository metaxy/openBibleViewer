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
    bool m_loaded;
    int loadModuleData(const int moduleID);
    QSqlDatabase m_db;
        QString toValidRTF(QString data);
};

#endif // THEWORDDICTIONARY_H
