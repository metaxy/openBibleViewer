#ifndef METAINFO_H
#define METAINFO_H
#include <QtCore/QString>
class MetaInfo
{
public:
    MetaInfo();
    QString name() const;
    QString shortName() const;
    QString uid() const;

    void setName(const QString &name);
    void setShortName(const QString &shortName);
    void setUID(const QString &uid);

private:
    QString m_name;
    QString m_shortName;
    QString m_uid;
};

#endif // METAINFO_H
