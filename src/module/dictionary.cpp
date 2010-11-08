/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
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
    if(moduleID < 0 || m_module == NULL) {
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
        m_zefaniaLex->setID(m_moduleID, path);
        break;
    }
    case Module::BibleQuoteDictModule: {
        if(m_module->m_bibleQuoteDict) {
            m_bibleQuoteDict = m_module->m_bibleQuoteDict;
        } else {
            m_bibleQuoteDict = new BibleQuoteDict();
            m_bibleQuoteDict->setSettings(m_settings);
            m_module->m_bibleQuoteDict = m_bibleQuoteDict;
        }
        m_bibleQuoteDict->setID(m_moduleID, path);
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

}
QStringList Dictionary::getAllKeys()
{
    //DEBUG_FUNC_NAME
    switch(m_moduleType) {
    case Module::ZefaniaLexModule: {
        return m_zefaniaLex->getAllKeys();
    }
    case Module::BibleQuoteDictModule: {
        return m_bibleQuoteDict->getAllKeys();
    }
    default:
        return QStringList();
    }

}
