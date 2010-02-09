#include "session.h"

Session::Session()
{
}
void Session::setData(QString key, QVariant value)
{
    m_data.insert(key, value);
}
QVariant Session::getData(QString key, QVariant defaultValue = QVariant())
{
    return m_data.value(key, defaultValue);
}
QVariant Session::getData(QString key)
{
    return m_data.value(key);
}
bool Session::isSet(QString key)
{
    if (m_data[key].isNull())
        return false;
    return true;
}

QMapIterator<QString, QVariant> Session::getInterator()
{
    QMapIterator<QString, QVariant> i(m_data);
    return i;
}

