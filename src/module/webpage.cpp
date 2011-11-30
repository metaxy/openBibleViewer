/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
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
#include "webpage.h"

WebPage::WebPage()
{
    m_loaded = false;
}
void WebPage::loadModuleData(const int moduleID, const QString &name)
{
    m_moduleID = moduleID;
    m_module = m_map->module(m_moduleID);

    QString fileName;
    if(name.isEmpty())
        fileName = m_module->path();
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
                }
                n2 = n2.nextSibling();
            }
        } else if(n.nodeName() == "data") {
            QDomNode n2 = n.firstChild().firstChild();
            while(!n2.isNull()) {
                if(n2.nodeName() == "url") {
                    m_url = n2.firstChild().toText().data();
                }
                n2 = n2.nextSibling();
            }
        }
        n = n.nextSibling();
    }
    m_loaded = true;
    m_loadedModuleID = m_moduleID;
}

QUrl WebPage::getUrl()
{
    if(!loaded())
        loadModuleData(m_moduleID);
    return QUrl(m_url);
}

void WebPage::search(SearchQuery query, SearchResult *result)
{

}

MetaInfo WebPage::readInfo(const QString &name)
{
    loadModuleData(m_moduleID, name);
    myDebug() << m_name << m_shortName;
    MetaInfo ret;
    ret.setName(m_name);
    ret.setShortName(m_shortName);
    return ret;
}
bool WebPage::loaded()
{
    return m_loaded && m_loadedModuleID == m_moduleID;
}
