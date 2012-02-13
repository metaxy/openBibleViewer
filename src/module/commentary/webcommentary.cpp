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
#include "webcommentary.h"
#include "src/module/response/urlresponse.h"

WebCommentary::WebCommentary()
{
}
Response* WebCommentary::readRanges(const Ranges &ranges, bool ignoreModuleID)
{
    DEBUG_FUNC_NAME
    CompiledRange range = this->toCompiledRange(ranges.getList().first());

    if(!loaded())
        loadModuleData(m_moduleID);

    QScriptValue fun = myEngine.evaluate(m_pharseVerseScript);
    QScriptValueList args;
    args << range.bookID << m_books.value(range.bookID).key << range.chapterID << range.startVerse << range.endVerse;
    QScriptValue url = fun.call(QScriptValue(), args);
    return new UrlResponse(url.toString());
}
int WebCommentary::currentBook()
{
    return 0;
}
int WebCommentary::currentChapter()
{
    return 0;
}
std::pair<int, int> WebCommentary::minMaxVerse(const int bookID, const int chapterID)
{
    std::pair<int, int> ret;
    ret.first = 0;
    ret.second = 0;
    return ret;
}

void WebCommentary::loadModuleData(const int moduleID, const QString &name)
{
    DEBUG_FUNC_NAME
    ModuleSettings *settings = m_settings->getModuleSettings(m_moduleID);

    if(!settings)
        return;

    m_moduleID = moduleID;
    QString fileName;

    if(name.isEmpty())
        fileName = settings->modulePath;
    else
        fileName = name;
    m_modulePath = fileName;

    QDomDocument doc;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        myWarning() << "failed to read" << fileName;
        return;
    }
    QString error;
    int line, col;
    if(!doc.setContent(&file, &error, &line, &col)) {
        myDebug() << "fail to pharse content of" << fileName << error << line << col;
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
                } else if(n2.nodeName() == "versification") {
                    m_v11nName = n2.firstChild().toText().data();
                }
                n2 = n2.nextSibling();
            }
        } else if(n.nodeName() == "data") {
            QDomNode n2 = n.firstChild().firstChild();
            while(!n2.isNull()) {
                if(n2.nodeName() == "url") {
                    m_url = n2.firstChild().toText().data();
                } else if(n2.nodeName() == "pharseInBook") {
                    m_pharseBookScript = n2.firstChild().toCDATASection().data();
                } else if(n2.nodeName() == "pharseInChapter") {
                    m_pharseChapterScript = n2.firstChild().toCDATASection().data();
                } else if(n2.nodeName() == "pharseInVerse") {
                    m_pharseVerseScript = n2.firstChild().toCDATASection().data();
                } else if(n2.nodeName() == "pharseOut") {
                    m_pharseOutScript = n2.firstChild().toCDATASection().data();
                } else if(n2.nodeName() == "books") {
                    QDomNode n3 = n2.firstChild();
                    int i = 0;
                    while(!n3.isNull()) {
                         if(n3.nodeName() == "book") {
                             const int bookID = n3.toElement().attribute("id", QString::number(i)).toInt();
                             struct WebCommentaryBooksData d = {bookID, n3.firstChild().toText().data()};
                             m_books.insert(bookID, d);
                             i++;
                         }
                         n3 = n3.nextSibling();
                    }
                }
                n2 = n2.nextSibling();
            }
        }
        n = n.nextSibling();
    }

    if(!m_v11nName.isEmpty()) {
        settings->versificationFile = "";
        settings->versificationName = m_v11nName;
        m_versification = settings->loadVersification();
    }
    m_loaded = true;
    m_loadedModuleID = m_moduleID;

}

void WebCommentary::search(SearchQuery query, SearchResult *result)
{

}

MetaInfo WebCommentary::readInfo(const QString &name)
{
    loadModuleData(m_moduleID, name);
    MetaInfo ret;
    ret.setName(m_name);
    ret.setShortName(m_shortName);
    return ret;
}
bool WebCommentary::loaded()
{
    return m_loaded && m_loadedModuleID == m_moduleID;
}
QString WebCommentary::pharseUrl(const QUrl &url)
{
    DEBUG_FUNC_NAME
    if(!loaded())
        loadModuleData(m_moduleID);

    QScriptValue fun = myEngine.evaluate(m_pharseOutScript);
    QScriptValueList args;
    args << url.toString();
    QScriptValue n = fun.call(QScriptValue(), args);
    return n.toString();
}
void WebCommentary::clearData()
{

}
