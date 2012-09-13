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
#include "zefania-lex.h"
#include "CLucene.h"
#include "src/module/response/stringresponse.h"
#include "src/core/search/searchtools.h"

using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;

ZefaniaLex::ZefaniaLex()
{
}

MetaInfo ZefaniaLex::readInfo(const QString &name)
{
    QFile file(name);
    int couldBe = 0;//1 = RMac
    QString uid = "";
    QString type;

    MetaInfo info;

    //open the xml file
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return MetaInfo();
    m_xml = new QXmlStreamReader(&file);


    if(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), "dictionary")) {
            type = m_xml->attributes().value("type").toString();
            if(type != "x-dictionary") {
                couldBe = -1;// do not scan the keys
            }
            while(m_xml->readNextStartElement()) {
                if(cmp(m_xml->name(), "information")) {
                    info = readMetaInfo(info);
                } else if(cmp(m_xml->name(), "item")) {
                    const QString key = m_xml->attributes().value("id").toString();
                    if(couldBe == 0) {
                        if(key.toUpper() == "A-APF" || key.toUpper() == "X-NSN" || key.toUpper() == "V-PAP-DPN") {//todo: speed
                            couldBe = 1;
                        }
                    } else {
                        break;
                    }
                } else {
                    m_xml->skipCurrentElement();
                }
            }
        }
    }
    if(info.name().isEmpty() && !info.subject.isEmpty()) {
        info.setName(info.subject);
    }

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
            info.setContent(ModuleTools::DictionaryContent);
        }
    }


    file.close();
    delete m_xml;
    m_xml = NULL;

    return info;
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

        Query* q = QueryParser::parse(SearchTools::toTCHAR(queryText), _T("content"), &analyzer);
        Hits* h = s.search(q);
        QString ret = "";
        for(size_t i = 0; i < h->length(); i++) {
            Document* doc = &h->doc(i);
            if(!ret.isEmpty())
                ret.append("<hr /> ");
            ret.append(SearchTools::toQString(doc->get(_T("content"))));
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

            ret.append(SearchTools::toQString(doc.get(_T("key"))));
        }
        m_entryList = ret;
        return ret;
    }
    catch(CLuceneError &err) {
        myWarning() << "clucene error = " << err.what();
        return QStringList();

    }
    catch(...) {
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
    DEBUG_FUNC_NAME;

    myDebug() << "building index!!!";
    QFile file(m_modulePath);
    Document indexdoc;
    const QString index = indexPath();
    QDir dir("/");
    dir.mkpath(index);


    m_refText.setSettings(m_settings);
    IndexWriter* writer = NULL;
    const TCHAR* stop_words[] = { NULL };
    standard::StandardAnalyzer an(stop_words);

    //open the xml file
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return 1;
    m_xml = new QXmlStreamReader(&file);

    try {

        if(IndexReader::indexExists(index.toStdString().c_str())) {
            if(IndexReader::isLocked(index.toStdString().c_str())) {
                myDebug() << "Index was locked... unlocking it.";
                IndexReader::unlock(index.toStdString().c_str());
            }
        }
        writer = new IndexWriter(index.toStdString().c_str() , &an, true);

        writer->setMaxFieldLength(0x7FFFFFFFL);
        writer->setUseCompoundFile(false);

        TCHAR *buffer = SearchTools::createBuffer();

        if(m_xml->readNextStartElement()) {
            if(cmp(m_xml->name(), "dictionary")) {
                while(m_xml->readNextStartElement()) {
                    if(cmp(m_xml->name(), "item")) {
                        QString content;
                        const QString key = m_xml->attributes().value("id").toString();
                        bool hasTitle = false;
                        while(true) {
                            m_xml->readNext();

                            if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), QLatin1String("item"))))
                                break;

                            if(m_xml->tokenType() == QXmlStreamReader::Characters) {
                                content += m_xml->text().toString();
                            } else if(cmp(m_xml->name(), "title")) {
                                const QString title = parseTitle();
                                content += "<h3 class='title'>" + key;
                                if(!title.isEmpty()) {
                                    content.append(" - " +  title);
                                }
                                content.append("</h3>");
                                hasTitle = true;
                            } else if(cmp(m_xml->name(), "transliteration")) {
                                const QString trans = parseTrans();
                                if(!trans.isEmpty()) {
                                    content += "<span class='transliteration'>" + trans + "</span>" ;
                                }
                            }  else if(cmp(m_xml->name(), "pronunciation")) {
                                const QString pr = parsePron();
                                if(!pr.isEmpty()) {
                                    content += "<span class='pronunciation'>" + pr + "</span>";
                                }
                            } else if(cmp(m_xml->name(), "description")) {
                                content += "<span class='description'>" + parseDesc() + "</span>";
                            } else {
                                content += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
                            }
                        }
                        if(!hasTitle) {
                            content.prepend("<h3 class='title'>" + key + "</h3>");
                        }
                        indexdoc.clear();
                        indexdoc.add(*_CLNEW Field(_T("key"), SearchTools::toTCHAR(key, buffer), Field::STORE_YES |  Field::INDEX_TOKENIZED));
                        indexdoc.add(*_CLNEW Field(_T("content"), SearchTools::toTCHAR(content, buffer), Field::STORE_YES |  Field::INDEX_TOKENIZED));
                        writer->addDocument(&indexdoc);

                    } else {
                        m_xml->skipCurrentElement();
                    }
                }
            } else {
                myWarning() << "not a file";
            }
        }

        writer->setUseCompoundFile(true);
        writer->optimize();

        writer->close();
        delete writer;
    }
    catch(...) {
    }

    file.close();
    delete m_xml;
    m_xml = NULL;
    return 0;
}
QString ZefaniaLex::parseTitle()
{
    QString ret;
    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), QLatin1String("title"))))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            ret += m_xml->text().toString();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return ret;
}
QString ZefaniaLex::parseTrans()
{
    QString ret;
    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), QLatin1String("transliteration"))))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            ret += m_xml->text().toString();
        } else if(cmp(m_xml->name(), "em")) {
            ret += parseEm();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return ret;
}
QString ZefaniaLex::parseEm()
{
    const QString pre("<em>");
    QString post("</em>");

    QString ret;
    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), QLatin1String("em"))))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            ret += m_xml->text().toString();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return pre+ret+post;
}

QString ZefaniaLex::parsePron()
{
    QString ret;
    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), QLatin1String("pronunciation"))))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            ret += m_xml->text().toString();
        } else if(cmp(m_xml->name(), "em")) {
            ret += parseEm();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return ret;
}
QString ZefaniaLex::parseDesc()
{
    QString ret;
    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), QLatin1String("description"))))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            ret += m_xml->text().toString();
        } else if(cmp(m_xml->name(), "em")) {
            ret += parseEm();
        } else if(cmp(m_xml->name(), "see")) {
            ret += parseSee();
        } else if(cmp(m_xml->name(), "reflink")) {
            ret += parseReflink();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return ret;
}

QString ZefaniaLex::parseReflink()
{
    const QStringRef fscope = m_xml->attributes().value("fscope");
    const QStringRef mscope = m_xml->attributes().value("mscope");
    const QStringRef target = m_xml->attributes().value("target");

    QString ret;
    if(!mscope.isEmpty()) {
        VerseUrl burl;
        burl.fromMscope(mscope.toString());

        QString text;
        if(!fscope.isEmpty()) {
            text = fscope.toString();
        } else  {
            text = m_refText.toString(burl);
        }
        m_xml->skipCurrentElement();
        ret = "<span class=\"crossreference\"><a class=\"reflink\" href=\"" + burl.toString() + "\">" + text + "</a></span>";

    } else if(!target.isEmpty()) {
       /* if(target == "XXX") {

        }*/
        ret = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
    }

    return ret;
}
QString ZefaniaLex::parseSee()
{
    //todo: currently we assume target = x-self
    const QStringRef target = m_xml->attributes().value("target");

    QString ret;
    if(!target.isEmpty()) {
        StrongUrl url;
        if(url.fromText(m_xml->text().toString()))
            ret = " <a class='crossreference' href='" + url.toString() + "'>" + m_xml->text().toString() + "</a> ";
    }
    m_xml->skipCurrentElement();
    return ret;
}
MetaInfo ZefaniaLex::readMetaInfo(MetaInfo ret)
{
    while(m_xml->readNextStartElement()) {
        if(m_xml->name() == QLatin1String("publisher")) {
            ret.publisher = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("contributors")) {
            ret.contributors = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("date")) {
            ret.date = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("type")) {
            ret.type = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("format")) {
            ret.format = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("identifier")) {
            ret.identifier = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("source")) {
            ret.source = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("language")) {
            ret.language = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("coverage")) {
            ret.coverage = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("rights")) {
            ret.rights = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("subject")) {
            ret.subject = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("description")) {
            ret.description = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("creator")) {
            ret.creator = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == QLatin1String("title")) {
            ret.setName(m_xml->readElementText(QXmlStreamReader::IncludeChildElements));
        } else {
            m_xml->skipCurrentElement();
        }
    }
    return ret;
}
QString ZefaniaLex::indexPath() const
{
    return m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
}
Response::ResponseType ZefaniaLex::responseType() const
{
    return Response::StringReponse;
}
bool ZefaniaLex::cmp(const QStringRef &r, const QString &s)
{
    return r == s || r == s.toLower();
}
