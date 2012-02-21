/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#include "src/module/response/stringresponse.h"
#include "src/module/response/urlresponse.h"
Dictionary::Dictionary()
{
    m_moduleType = ModuleTools::NoneType;
    m_moduleID = -1;
}
Dictionary::~Dictionary()
{
    DEBUG_FUNC_NAME
    m_dictionaryModule.clear();
}

int Dictionary::loadModuleData(const int moduleID)
{
    DEBUG_FUNC_NAME
    myDebug() << moduleID;
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
        m_dictionaryModule = m_module->newDictionaryModule(m_module->moduleType());
        if(m_dictionaryModule.isNull())
            return 1;
        m_dictionaryModule->setSettings(m_settings);
        m_dictionaryModule->setID(m_moduleID, path);
    }
    return 0;
}
Response* Dictionary::getEntry(const QString &string) const
{
    if(string.contains(" ") && m_dictionaryModule->responseType() == Response::StringReponse) {
        QString ret;
        const QStringList parts = string.split(" ");
        foreach(const QString &key, parts) {
            StringResponse *r = (StringResponse*) m_dictionaryModule->getEntry(key);
            r->prepend("<div class='dictEntry'>");
            r->append("</div>");
            ret += r->data();
            delete r;
        }
        return new StringResponse(ret);

    }  else {
        Response *res = m_dictionaryModule->getEntry(string);
        if(res->type() == Response::StringReponse) {
            StringResponse *r = (StringResponse*) res;
            r->prepend("<div class='dictEntry'>");
            r->append("</div>");
            return r;
        } else {
            return res;
        }
    }
}
QStringList Dictionary::getAllKeys() const
{
    return m_dictionaryModule->getAllKeys();
}
DictionaryModule * Dictionary::module() const
{
    return m_dictionaryModule.data();
}
QString Dictionary::moduleUID() const
{
    return QString();
}
