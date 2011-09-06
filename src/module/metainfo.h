#ifndef METAINFO_H
#define METAINFO_H
#include <QtCore/QString>
class MetaInfo
{
public:
    MetaInfo();
    QString name();
    QString shortName();

    void setName(const QString &name);
    void setShortName(const QString &shortName);

private:
    QString m_name;
    QString m_shortName;
};

#endif // METAINFO_H
