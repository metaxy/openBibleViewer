#ifndef METAINFO_H
#define METAINFO_H
#include <QtCore/QString>
#include "src/core/obvcore.h"
class MetaInfo
{
public:
    MetaInfo();
    QString name() const;
    QString shortName() const;
    QString uid() const;
    OBVCore::DefaultModule defaultModule() const;

    void setName(const QString &name);
    void setShortName(const QString &shortName);
    void setUID(const QString &uid);
    void setDefaultModule(const OBVCore::DefaultModule d);

    QString publisher;
    QString contributors;
    QString date;
    QString type;
    QString format;
    QString identifier;
    QString source;
    QString language;
    QString coverage;
    QString rights;
    QString subject;
    QString description;
    QString creator;

private:
    QString m_name;
    QString m_shortName;
    QString m_uid;
    OBVCore::DefaultModule m_defaultModule;
};

#endif // METAINFO_H
