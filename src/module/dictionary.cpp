#include "dictionary.h"
#include "src/core/dbghelper.h"
Dictionary::Dictionary()
{
    m_zefaniaStrong = 0;
    m_moduleType = Module::NoneType;
    m_moduleID = -1;
}
int Dictionary::loadModuleData(const int &moduleID)
{
    DEBUG_FUNC_NAME
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
            m_zefaniaStrong->setSettings(m_settings);
            m_module->m_zefaniaStrong = m_zefaniaStrong;
        }
        m_zefaniaStrong->m_modulePath = path;
        break;
    }

    default:
        return 1;
    }
    return 0;
}
QString Dictionary::getEntry(const QString &string)
{
    DEBUG_FUNC_NAME
    myDebug() << string << m_moduleType;
    switch(m_moduleType) {
    case Module::ZefaniaStrongModule: {
        return m_zefaniaStrong->getStrong(string);
        break;
    }

    default:
        return QString();
    }
}
