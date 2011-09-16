/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "src/module/dictionary/biblequote-dict.h"
#include "src/module/dictionary/zefania-lex.h"
#include "src/module/dictionary/webdictionary.h"
Dictionary::Dictionary()
{
    m_dictionaryModule = 0;
    m_moduleType = OBVCore::NoneType;
    m_moduleID = -1;
}
int Dictionary::loadModuleData(const int moduleID)
{
    DEBUG_FUNC_NAME
    m_module = m_map->module(moduleID);
    if(moduleID < 0 || m_module == NULL) {
        myWarning() << "invalid moduleID = " << moduleID;
        return 1;
    }
    m_moduleID = moduleID;
    const QString path = m_module->path();

    if(m_module->m_dictionaryModule) {
        m_dictionaryModule = m_module->m_dictionaryModule;
    } else {
        //Module delete them
        if(m_moduleType == OBVCore::ZefaniaLexModule) {
            m_dictionaryModule = new ZefaniaLex();
        } else if(m_moduleType == OBVCore::BibleQuoteDictModule) {
            m_dictionaryModule = new BibleQuoteDict();
        } else if(m_moduleType == OBVCore::WebDictionaryModule) {
            m_dictionaryModule = new WebDictionary();
        }  else {
            return 1;
        }

        m_dictionaryModule->setSettings(m_settings);
        m_module->m_dictionaryModule = m_dictionaryModule;
        m_dictionaryModule->setID(m_moduleID, path);
    }
    return 0;
}
QString Dictionary::getEntry(const QString &string) const
{
    return m_dictionaryModule->getEntry(string);
}
QStringList Dictionary::getAllKeys() const
{
    return m_dictionaryModule->getAllKeys();
}
