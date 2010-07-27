#include "dictionary.h"

Dictionary::Dictionary()
{
}
int Dictionary::loadModuleData(const int &moduleID)
{
    m_module = m_map->m_map.value(moduleID);
    if(moduleID < 0 || !m_module) {
        myWarning() << "invalid moduleID = " << moduleID;
        return 1;
    }
    m_moduleID = moduleID;
    const QString path = m_module->m_path;

    switch(m_moduleType) {
    case Module::ZefaniaStrongModule: {
        if(m_module->m_zefaniaStrong) {
            m_zefaniaStrong = m_module->m_zefaniaStrong;
        } else {
            m_zefaniaStrong = new ZefaniaStrong();
            m_module->m_zefaniaStrong = m_zefaniaStrong;
        }
        ModuleSettings m = m_settings->getModuleSettings(m_moduleID);


        break;
    }

    default:
        return 1;
    }
    m_loaded = true;
    return 0;
}
QString Dictionary::getEntryFor(const QString &string)
{
    return QString();
}
