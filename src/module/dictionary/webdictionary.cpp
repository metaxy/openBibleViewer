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
#include "webdictionary.h"
#include "src/module/response/urlresponse.h"
WebDictionary::WebDictionary()
{
}
WebDictionary::~WebDictionary()
{

}

void WebDictionary::loadModuleData(const int moduleID, const QString &name)
{
    m_moduleID = moduleID;
    QString fileName;
    if(name.isEmpty())
        fileName = m_modulePath;
    else
        fileName = name;

    myDebug() << fileName;
    QDomDocument doc;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return;

    if(!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        if(n.nodeName() == "meta") {
            QDomNode n2 = n.firstChild();
            while(!n2.isNull()) {
                if(n2.nodeName() == "name") {
                    m_name = n2.firstChild().toText().data();

                } else if(n2.nodeName() == "shortName") {
                    m_shortName = n2.firstChild().toText().data();
                } else if(n2.nodeName() == "desc") {
                    m_desc = n2.firstChild().toText().data();
                }  else if(n2.nodeName() == "type") {
                    m_type = n2.firstChild().toText().data();
                }
                n2 = n2.nextSibling();
            }
        } else if(n.nodeName() == "data") {
            QDomNode n2 = n.firstChild().firstChild();
            while(!n2.isNull()) {
                if(n2.nodeName() == "url") {
                    m_url = n2.firstChild().toText().data();
                } else if(n2.nodeName() == "pharseIn") {
                    m_pharseScript = n2.firstChild().toCDATASection().data();
                } else if(n2.nodeName() == "pharseOut") {
                    m_pharseOutScript = n2.firstChild().toCDATASection().data();
                }
                n2 = n2.nextSibling();
            }
        }
        n = n.nextSibling();
    }
    m_loaded = true;
    m_loadedModuleID = m_moduleID;
}
Response* WebDictionary::getEntry(const QString &entry)
{
    if(!loaded())
        loadModuleData(m_moduleID);

    QScriptValue fun = myEngine.evaluate(m_pharseScript);
    QScriptValueList args;
    args << entry;
    QScriptValue url = fun.call(QScriptValue(), args);
    return new UrlResponse(url.toString());
}

QStringList WebDictionary::getAllKeys()
{
    return QStringList();
}

void WebDictionary::search(SearchQuery query, SearchResult *result)
{

}

MetaInfo WebDictionary::readInfo(const QString &name)
{
    loadModuleData(m_moduleID, name);
    MetaInfo ret;
    ret.setName(m_name);
    ret.setShortName(m_shortName);

    if(m_type == "strong-dictionary") {
        ret.setDefaultModule(ModuleTools::DefaultStrongDictModule);
        ret.setContent(ModuleTools::StrongsContent);
    } else if(m_type == "rmac-dictionary") {
        ret.setDefaultModule(ModuleTools::DefaultRMACDictModule);
        ret.setContent(ModuleTools::RMACContent);
    } else if(m_type == "gram-dictionary") {
        ret.setDefaultModule(ModuleTools::DefaultGramDictModule);
        ret.setContent(ModuleTools::GramContent);
    } else if(m_type == "dictionary") {
        ret.setDefaultModule(ModuleTools::DefaultDictModule);
        ret.setContent(ModuleTools::DictionaryContent);
    }
    return ret;
}
bool WebDictionary::loaded()
{
    return m_loaded && m_loadedModuleID == m_moduleID;
}
QString WebDictionary::pharseUrl(const QUrl &url)
{
    if(!loaded())
        loadModuleData(m_moduleID);

    QScriptValue fun = myEngine.evaluate(m_pharseOutScript);
    QScriptValueList args;
    args << url.toString();
    QScriptValue n = fun.call(QScriptValue(), args);
    myDebug() << n.toString();
    return n.toString();
}
Response::ResponseType WebDictionary::responseType() const
{
    return Response::UrlReponse;
}
