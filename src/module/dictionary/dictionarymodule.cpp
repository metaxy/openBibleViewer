#include "dictionarymodule.h"

DictionaryModule::DictionaryModule()
{
}
DictionaryModule::~DictionaryModule()
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
