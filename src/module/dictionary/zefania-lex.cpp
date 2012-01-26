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
#include "zefania-lex.h"
#include "src/core/verse/reftext.h"
#include "config.h"
#include "CLucene.h"
#include "CLucene/clucene-config.h"
#include "src/module/response/stringresponse.h"

using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;

ZefaniaLex::ZefaniaLex()
{
}


/**
  Load a Zefania XML Lex file the first time. Generates an index for fast access.
  */
MetaInfo ZefaniaLex::buildIndexFromData(const QString &fileData, const QString &fileName)
{
    DEBUG_FUNC_NAME
    m_modulePath = fileName;

    KoXmlDocument xmldoc;

    QString errorMsg;
    int eLine;
    int eCol;
    if(!xmldoc.setContent(fileData, &errorMsg, &eLine, &eCol)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
        myWarning() << "the file isn't valid , error = " << errorMsg
                    << " line = " << eLine
                    << " column = " << eCol;
        return MetaInfo();
    }
    return buildIndexFromXmlDoc(&xmldoc);
}
MetaInfo ZefaniaLex::buildIndexFromFile(const QString &fileName)
{
    KoXmlDocument xmldoc;
    QString errorMsg;
    int eLine;
    int eCol;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return MetaInfo();

    if(!xmldoc.setContent(&file, &errorMsg, &eLine, &eCol)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
        myWarning() << "the file isn't valid , error = " << errorMsg
                    << " line = " << eLine
                    << " column = " << eCol;
        return MetaInfo();
    }
    const MetaInfo ret = buildIndexFromXmlDoc(&xmldoc);
    file.close();
    return ret;
}

/**
  Returns a Entry.
  \id The key of the entry.
  */
Response * ZefaniaLex::getEntry(const QString &key)
{
    try {
        if(!hasIndex()) {
            if(buildIndex() != 0) {
                return new StringResponse(QObject::tr("Cannot build index."));
            }
        }
        const QString index = indexPath();
        const QString queryText = "key:" + key;
        const TCHAR* stop_words[] = { NULL };
        standard::StandardAnalyzer analyzer(stop_words);
        IndexReader* reader = IndexReader::open(index.toStdString().c_str());
        IndexSearcher s(reader);
    #ifdef OBV_USE_WSTRING
        Query* q = QueryParser::parse(queryText.toStdWString().c_str(), _T("content"), &analyzer);
    #else
        Query* q = QueryParser::parse(reinterpret_cast<const wchar_t *>(queryText.utf16()), _T("content"), &analyzer);
    #endif
        Hits* h = s.search(q);
        QString ret = "";
        for(size_t i = 0; i < h->length(); i++) {
            Document* doc = &h->doc(i);
            if(!ret.isEmpty())
                ret.append("<hr /> ");
    #ifdef OBV_USE_WSTRING
            ret.append(QString::fromWCharArray(doc->get(_T("content"))));
    #else
            ret.append(QString::fromUtf16((const ushort*)doc->get(_T("content"))));
    #endif
        }
        return ret.isEmpty() ? new StringResponse(QObject::tr("Nothing found for %1").arg(key)) : new StringResponse(ret);
    } catch(...) {
        return new StringResponse(QString());
    }
}

QStringList ZefaniaLex::getAllKeys()
{
    if(!m_entryList.isEmpty()) {
        return m_entryList;
    }
    try {
        if(!hasIndex()) {
            if(buildIndex() != 0) {
                return QStringList();
            }
        }
        const QString index = indexPath();
        IndexReader* reader = IndexReader::open(index.toStdString().c_str());
        QStringList ret;
        for(int i = 0; i < reader->numDocs(); i++) {
            Document doc;
            reader->document(i, doc);
    #ifdef OBV_USE_WSTRING
            ret.append(QString::fromWCharArray(doc.get(_T("key"))));
    #else
            ret.append(QString::fromUtf16((const ushort*)doc.get(_T("key"))));
    #endif
        }
        m_entryList = ret;
        return ret;
    }
    catch(...)
    {
        return QStringList();
    }
}

bool ZefaniaLex::hasIndex()
{
    QDir d;
    if(!d.exists(m_settings->homePath + "index")) {
        return false;
    }
    const QString index = indexPath();
    return IndexReader::indexExists(index.toStdString().c_str());
}
int ZefaniaLex::buildIndex()
{
    QFile file(m_modulePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Can not read the file"));
        myWarning() << "can't read the file";
        return 1;
    }
    KoXmlDocument xmlDoc;
    QString error;
    int l;
    int c;
    if(!xmlDoc.setContent(&file, &error, &l, &c)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid. Errorstring: %1 in Line %2 at Position %3").arg(error).arg(l).arg(c));
        myWarning() << "the file isn't valid";
        return 1;
    }
    return buildIndexFromXmlDoc(&xmlDoc).name().isEmpty() ? 2 : 0;
}

MetaInfo ZefaniaLex::buildIndexFromXmlDoc(KoXmlDocument *xmldoc)
{
    try {

    MetaInfo info;
    int couldBe = 0;//1 = RMac

    Document indexdoc;
    const QString index = indexPath();
    QString fileTitle;
    QString uid;
    QString type;

    QDir dir("/");
    dir.mkpath(index);

    RefText refText;
    refText.setSettings(m_settings);

    IndexWriter* writer = NULL;
    const TCHAR* stop_words[] = { NULL };
    standard::StandardAnalyzer an(stop_words);

    if(IndexReader::indexExists(index.toStdString().c_str())) {
        if(IndexReader::isLocked(index.toStdString().c_str())) {
            myDebug() << "Index was locked... unlocking it.";
            IndexReader::unlock(index.toStdString().c_str());
        }
    }
    writer = new IndexWriter(index.toStdString().c_str() , &an, true);

    writer->setMaxFieldLength(0x7FFFFFFFL);
    writer->setUseCompoundFile(false);

    KoXmlNode item = xmldoc->documentElement().firstChild();
    type = xmldoc->documentElement().toElement().attribute("type", "");

    for(int c = 0; !item.isNull();) {
        QString key = "";
        QString title = "";
        QString trans = "";
        QString pron = "";
        QString desc = "";
        KoXmlElement e = item.toElement();
        if(e.tagName().compare("INFORMATION", Qt::CaseInsensitive) == 0) {
            KoXmlNode title = item.namedItem("subject");
            KoXmlNode identifer = item.namedItem("identifier");

            fileTitle = title.toElement().text();
            uid = identifer.toElement().text();

        } else if(e.tagName().compare("item", Qt::CaseInsensitive) == 0) {
            key = e.attribute("id");
            KoXmlNode details = item.firstChild();
            while(!details.isNull()) {
                KoXmlElement edetails = details.toElement();
                if(edetails.tagName().compare("title", Qt::CaseInsensitive) == 0) {
                    title = edetails.text();
                } else if(edetails.tagName().compare("transliteration", Qt::CaseInsensitive) == 0) {
                    trans = edetails.text();
                } else if(edetails.tagName().compare("pronunciation", Qt::CaseInsensitive) == 0) {
                    KoXmlNode em = details.firstChild();
                    while(!em.isNull()) {
                        if(em.toElement().tagName().compare("em", Qt::CaseInsensitive) == 0)
                            pron = "<em>" + em.toElement().text() + "</em>";
                        em = em.nextSibling();
                    }
                } else if(edetails.tagName().compare("description", Qt::CaseInsensitive) == 0) {
                    KoXmlNode descNode = details.firstChild();
                    while(!descNode.isNull()) {
                        if(descNode.nodeType() == 2) {
                            desc += descNode.toText().data();
                        } else if(descNode.nodeType() == 1) {
                            KoXmlElement descElement = descNode.toElement();
                            if(descElement.tagName().compare("reflink", Qt::CaseInsensitive) == 0) {
                                if(descElement.hasAttribute("mscope")) {
                                    const QString mscope = descElement.attribute("mscope", ";;;");

                                    VerseUrl url;
                                    url.fromMscope(mscope);

                                    desc += " <a class='crossreference' href=\"" + url.toString() + "\">" + refText.toString(url) + "</a> ";
                                } else if(descElement.hasAttribute("target")) {
                                    desc += descElement.text();
                                }

                            } else if(descElement.tagName().compare("see", Qt::CaseInsensitive) == 0) {
                                const QString target = descElement.attribute("target", "");
                                //todo: currently we assume target = x-self
                                StrongUrl url;
                                bool ok = url.fromText(descElement.text());
                                if(ok)
                                    desc += " <a class='crossreference' href=\"" + url.toString() + "\">" + descElement.text() + "</a> ";
                            }
                        }

                        descNode = descNode.nextSibling();
                    }
                    desc += "<span class=\"hr\" />";
                }
                details = details.nextSibling();
            }
            if(couldBe == 0) {
                if(key.toUpper() == "A-APF" || key.toUpper() == "X-NSN" || key.toUpper() == "V-PAP-DPN") {
                    couldBe = 1;
                }
            }
            QString content = "<h3 class='title'>" + key + " - " + title + "</h3>";
            if(!trans.isEmpty()) {
                content += "<span class='transliteration'>" + trans + "</span>";
            }
            if(!pron.isEmpty()) {
                content += "<span class='pronunciation'>" + pron + "</span>";
            }
            content += "<span class='description'>" + desc + "</span>";
            indexdoc.clear();
#ifdef OBV_USE_WSTRING
            indexdoc.add(*_CLNEW Field(_T("key"), key.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));
            indexdoc.add(*_CLNEW Field(_T("content"), content.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));
#else
            indexdoc.add(*_CLNEW Field(_T("key"), reinterpret_cast<const wchar_t *>(key.utf16()), Field::STORE_YES |  Field::INDEX_TOKENIZED));
            indexdoc.add(*_CLNEW Field(_T("content"), reinterpret_cast<const wchar_t *>(content.utf16()), Field::STORE_YES |  Field::INDEX_TOKENIZED));
#endif
            writer->addDocument(&indexdoc);

        }
        item = item.nextSibling();
        c++;
    }
    writer->setUseCompoundFile(true);
    writer->optimize();

    writer->close();
    delete writer;
    info.setName(fileTitle);
    info.setUID(uid);
    if(type == "x-strong") {
        info.setDefaultModule(ModuleTools::DefaultStrongDictModule);
        info.setContent(ModuleTools::StrongsContent);
    } else if(type == "x-dictionary") {
        if(couldBe == 1) {
            info.setDefaultModule(ModuleTools::DefaultRMACDictModule);
            info.setContent(ModuleTools::RMACContent);
        } else {
            info.setDefaultModule(ModuleTools::DefaultDictModule);
        }
    }
    return info;
    }
    catch(...) {
        return MetaInfo();
    }
}
QString ZefaniaLex::indexPath() const
{
    return m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
}
Response::ResponseType ZefaniaLex::responseType() const
{
    return Response::StringReponse;
}
