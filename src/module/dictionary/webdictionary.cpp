#include "webdictionary.h"

WebDictionary::WebDictionary()
{
}
WebDictionary::~WebDictionary()
{

}

void WebDictionary::loadModuleData(const int moduleID, const QString &name)
{
    m_moduleID = moduleID;
    /*m_module = m_map->module(m_moduleID);
*/
    QString fileName;
    if(name.isEmpty())
        fileName = m_modulePath;
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
                } else if(n2.nodeName() == "pharse") {
                    m_pharseScript = n2.firstChild().toCDATASection().data();
                }
                n2 = n2.nextSibling();
            }
        }
        n = n.nextSibling();
    }
    m_loaded = true;
    m_loadedModuleID = m_moduleID;
}
QString WebDictionary::getEntry(const QString &entry)
{
    if(!loaded())
        loadModuleData(m_moduleID);

    QScriptValue fun = myEngine.evaluate(m_pharseScript);
    QScriptValueList args;
    args << entry;
    QScriptValue url = fun.call(QScriptValue(), args);

    myDebug() << url.toString();
    return url.toString();
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
    myDebug() << m_name << m_shortName;
    MetaInfo ret;
    ret.setName(m_name);
    ret.setShortName(m_shortName);
    return ret;
}
bool WebDictionary::loaded()
{
    return m_loaded && m_loadedModuleID == m_moduleID;
}
