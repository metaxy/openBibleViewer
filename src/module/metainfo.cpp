#include "metainfo.h"

MetaInfo::MetaInfo() : m_name(), m_shortName()
{
}
QString MetaInfo::name() const
{
    return m_name;
}

QString MetaInfo::shortName() const
{
    return m_shortName;
}
QString MetaInfo::uid() const
{
    return m_uid;
}

void MetaInfo::setName(const QString &name)
{
    m_name = name;
}

void MetaInfo::setShortName(const QString &shortName)
{
    m_shortName = shortName;
}
void MetaInfo::setUID(const QString &uid)
{
    m_uid = uid;
}
