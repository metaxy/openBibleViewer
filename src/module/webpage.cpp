#include "webpage.h"

WebPage::WebPage()
{
    m_loaded = false;
}
void WebPage::loadData()
{
    const QString fileName = m_module->path();
    QDomDocument doc;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;

    if (!doc.setContent(&file)) {
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
}

QUrl WebPage::getUrl()
{
    if(!m_loaded)
        loadData();
    return QUrl(m_url);
}

void WebPage::search(SearchQuery query, SearchResult *result)
{

}
