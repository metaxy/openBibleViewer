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
#include "webcommentary.h"
#include "src/module/response/urlresponse.h"
#include <QtCore/QFile>
#include <QDomDocument>

WebCommentary::WebCommentary()
{
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
    ret.second = m_versification->maxVerse().value(bookID).at(chapterID);
    return ret;
}

int WebCommentary::loadModuleData(const int moduleID, const QString &fileName)
{
    DEBUG_FUNC_NAME

    m_moduleID = moduleID;
    m_modulePath = fileName;

    ModuleSettings *settings = m_settings->getModuleSettings(m_moduleID);
    QDomDocument doc;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        myWarning() << "failed to read" << fileName;
        return 1;
    }
    QString error;
    int line, col;
    if(!doc.setContent(&file, &error, &line, &col)) {
        myDebug() << "fail to pharse content of" << fileName << error << line << col;
        file.close();
        return 1;
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
    return 0;

}

void WebCommentary::search(SearchQuery query, SearchResult *result)
{

}

MetaInfo WebCommentary::readInfo(const QString &name)
{
    MetaInfo ret;
    QDomDocument doc;
    QFile file(name);
    if(!file.open(QIODevice::ReadOnly)) {
        myWarning() << "failed to read" << name;
        return ret;
    }
    QString error;
    int line, col;
    if(!doc.setContent(&file, &error, &line, &col)) {
        myDebug() << "fail to pharse content of" << name << error << line << col;
        file.close();
        return ret;
    }
    file.close();
    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        if(n.nodeName() == "meta") {
            QDomNode n2 = n.firstChild();
            while(!n2.isNull()) {
                if(n2.nodeName() == "name") {
                    ret.setName(n2.firstChild().toText().data());
                } else if(n2.nodeName() == "shortName") {
                    ret.setShortName(n2.firstChild().toText().data());
                }
                n2 = n2.nextSibling();
            }
        }
        n = n.nextSibling();
    }
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
        loadModuleData(m_moduleID, m_modulePath);

    QScriptValue fun = myEngine.evaluate(m_pharseOutScript);
    QScriptValueList args;
    args << url.toString();
    QScriptValue n = fun.call(QScriptValue(), args);
    QString newUrl = n.toString();
    if(newUrl.startsWith(ModuleTools::userInputScheme)) {
        myDebug() << "try to replace";
        newUrl.remove(0, ModuleTools::userInputScheme.size());
        foreach(WebCommentaryBooksData data, m_books) {
            if(newUrl.startsWith(data.key)) {
                myDebug() << "should replace";
                myDebug() << data.bookID;
                newUrl = newUrl.replace(data.key, "{"+QString::number(data.bookID)+"}");
                break;
            }
        }
        newUrl.prepend(ModuleTools::userInputScheme);
    }
    myDebug() << newUrl;
    return newUrl;
}
void WebCommentary::clearData()
{

}
Response * WebCommentary::readVerseRange(const int bookID,const int chapterID, const int startVerseID, const int endVerseID)
{
    QScriptValue fun = myEngine.evaluate(m_pharseVerseScript);
    QScriptValueList args;
    args << bookID << m_books.value(bookID).key << chapterID << startVerseID << endVerseID;
    QScriptValue url = fun.call(QScriptValue(), args);
    return new UrlResponse(url.toString());
}

Response * WebCommentary::readChapter(const int bookID, const int chapterID)
{
    QScriptValue fun = myEngine.evaluate(m_pharseChapterScript);
    QScriptValueList args;
    args << bookID << m_books.value(bookID).key << chapterID;
    QScriptValue url = fun.call(QScriptValue(), args);
    return new UrlResponse(url.toString());
}

Response * WebCommentary::readBook(const int bookID)
{
    QScriptValue fun = myEngine.evaluate(m_pharseBookScript);
    QScriptValueList args;
    args << bookID << m_books.value(bookID).key;
    QScriptValue url = fun.call(QScriptValue(), args);
    return new UrlResponse(url.toString());
}
CommentaryModule::LinkPolicy WebCommentary::linkPolicy() const
{
    return CommentaryModule::OpenWebLinksHere;
}
