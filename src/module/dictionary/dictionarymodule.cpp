#include "dictionarymodule.h"

DictionaryModule::DictionaryModule()
{
}
void DictionaryModule::setSettings(Settings *settings)
{
    m_settings = settings;
}

void DictionaryModule::setID(int moduleID, const QString &path)
{
    m_moduleID = moduleID;
    m_modulePath = path;
}

QString DictionaryModule::getEntry(const QString &entry)
{
    return "";
}

QStringList DictionaryModule::getAllKeys()
{
    return QStringList();
}

bool DictionaryModule::hasIndex()
{
    return false;
}

int DictionaryModule::buildIndex()
{
    return -1;
}

QString DictionaryModule::indexPath() const
{
    return m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
}
