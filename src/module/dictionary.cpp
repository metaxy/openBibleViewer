#include "dictionary.h"
#include "src/core/dbghelper.h"
Dictionary::Dictionary()
{
    m_zefaniaLex = 0;
    m_bibleQuoteDict = 0;
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
    case Module::ZefaniaLexModule: {
        if(m_module->m_zefaniaLex) {
            m_zefaniaLex = m_module->m_zefaniaLex;
        } else {
            m_zefaniaLex = new ZefaniaLex();
            m_zefaniaLex->setSettings(m_settings);
            m_module->m_zefaniaLex = m_zefaniaLex;
        }
        m_zefaniaLex->m_modulePath = path;
        break;
    }
    case Module::BibleQuoteDictModule: {
        if(m_module->m_bibleQuoteDict) {
            m_bibleQuoteDict = m_module->m_bibleQuoteDict;
        } else {
            m_bibleQuoteDict = new BibleQuoteDict();
            m_bibleQuoteDict->setSettings(m_settings);
            m_bibleQuoteDict->setID(m_moduleID, path);
        }
        break;
    }
    default:
        return 1;
    }
    return 0;
}
QString Dictionary::getEntry(const QString &string)
{
    switch(m_moduleType) {
    case Module::ZefaniaLexModule: {
        return m_zefaniaLex->getEntry(string);
        break;
    }
    case Module::BibleQuoteDictModule: {
        return m_bibleQuoteDict->getEntry(string);
        break;
    }
    default:
        return QString();
    }
    return QString();
}
