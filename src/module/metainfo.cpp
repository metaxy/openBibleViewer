#include "metainfo.h"

MetaInfo::MetaInfo() : m_name(), m_shortName()
{
}
QString MetaInfo::name()
{
    return m_name;
}

QString MetaInfo::shortName()
{
    return m_shortName;
}

void MetaInfo::setName(const QString &name)
{
    m_name = name;
}

void MetaInfo::setShortName(const QString &shortName)
{
    m_shortName = shortName;
}
