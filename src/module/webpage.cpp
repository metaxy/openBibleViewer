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
    if (!file.open(QIODevice::ReadOnly))
        return;

    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();
    myDebug() << "reading";

    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        myDebug() << n.nodeName();
        if(n.nodeName() == "meta") {
            QDomNode n2 = n.firstChild();
            while(!n2.isNull()) {
                myDebug() << n2.nodeName();
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
}

QUrl WebPage::getUrl()
{
    if(!m_loaded)
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
