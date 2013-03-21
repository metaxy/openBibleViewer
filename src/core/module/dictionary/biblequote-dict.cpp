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
#include "biblequote-dict.h"
#include "CLucene.h"
#include "src/core/module/response/stringresponse.h"
#include "src/core/search/searchtools.h"
#ifdef Q_WS_WIN
#include <tchar.h>
#endif
using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;

BibleQuoteDict::BibleQuoteDict()
{
}


/**
  Reads the ini file and returns the dictionary name.
  */
MetaInfo BibleQuoteDict::readInfo(QFile &file)
{
    const QString encoding = m_settings->encoding;
    QTextCodec *codec = QTextCodec::codecForName(encoding.toStdString().c_str());
    QTextDecoder *decoder = codec->makeDecoder();
    QByteArray byteline = file.readLine();
    QString line = decoder->toUnicode(byteline);
    file.close();
    MetaInfo info;
    info.setName(line.simplified());
    return info;
}
MetaInfo BibleQuoteDict::readInfo(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return MetaInfo();
    return readInfo(file);
}

bool BibleQuoteDict::hasIndex()
{
    QDir d;
    if(!d.exists(m_settings->homePath + "index")) {
        return false;
    }
    const QString index = indexPath();
    return IndexReader::indexExists(index.toStdString().c_str());
}
int BibleQuoteDict::buildIndex()
{
    DEBUG_FUNC_NAME

    // parse both and add docs to the indexwriter
    //myDebug() << m_modulePath;
    QFileInfo fileInfo(m_modulePath);
    //myDebug() << fileInfo.absoluteDir();
    QDir moduleDir(fileInfo.absoluteDir());
    moduleDir.setFilter(QDir::Files);
    QFileInfoList list = moduleDir.entryInfoList();

    QFileInfo htmlFileInfo;
    foreach(const QFileInfo & info, list) {
        if((info.suffix().compare("html", Qt::CaseInsensitive) == 0 || info.suffix().compare("htm", Qt::CaseInsensitive) == 0) && info.baseName().compare(fileInfo.baseName(), Qt::CaseInsensitive) == 0) {
            htmlFileInfo = info;
            break;
        }
    }
    //myDebug() << htmlFileInfo.absoluteFilePath();
    if(!htmlFileInfo.isReadable() || !fileInfo.isReadable()) {
        myWarning() << "cannot open file to build index";
        //todo: qmessagebox
        return 1;
    }

    QFile configFile(fileInfo.absoluteFilePath());
    QFile htmlFile(htmlFileInfo.absoluteFilePath());

    const QString encoding = m_settings->encoding;
    QTextCodec *codec = QTextCodec::codecForName(encoding.toStdString().c_str());

    if(!configFile.open(QIODevice::ReadOnly | QIODevice::Text) || !htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        myWarning() << "cannot open file to build index";
        return 1;
    }
    QTextStream configIn(&configFile);
    configIn.setCodec(codec);
    QTextStream htmlIn(&htmlFile);
    htmlIn.setCodec(codec);

    const QString index = indexPath();
    QDir dir("/");
    dir.mkpath(index);
    QProgressDialog progress(QObject::tr("Build index"), QObject::tr("Cancel"), 0, 0);
    progress.setWindowModality(Qt::WindowModal);

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

    //writer->setMaxFieldLength(0x7FFFFFFFL);
    writer->setUseCompoundFile(false);

    //index
    Document indexdoc;

    const QString title = configIn.readLine();
    QString id = configIn.readLine();
    long num = configIn.readLine().toLong();
    const QString pre = htmlIn.read(num - 1);
    myDebug() << title << pre;
    TCHAR *buffer = SearchTools::createBuffer();
    while(!configIn.atEnd()) {
        long n = num;
        const QString key = id;
        id = configIn.readLine();
        num = configIn.readLine().toLong();
        const QString data = htmlIn.read(num - n - 1);
        if(key.isEmpty() || data.isEmpty())
            continue;

        indexdoc.clear();

        indexdoc.add(*_CLNEW Field(_T("key"), SearchTools::toTCHAR(key, buffer), Field::STORE_YES |  Field::INDEX_TOKENIZED));
        indexdoc.add(*_CLNEW Field(_T("content"), SearchTools::toTCHAR(data, buffer), Field::STORE_YES |  Field::INDEX_TOKENIZED));

        writer->addDocument(&indexdoc);

    }

    writer->setUseCompoundFile(true);
    writer->optimize();

    writer->close();
    _CLLDELETE(writer);
    return 0;
}

Response* BibleQuoteDict::getEntry(const QString &key)
{
    DEBUG_FUNC_NAME
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
    //todo: or use querytext and as the field content
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
}
QStringList BibleQuoteDict::getAllKeys()
{
    DEBUG_FUNC_NAME
    if(!m_entryList.isEmpty()) {
        return m_entryList;
    }
    if(!hasIndex())
        buildIndex();
    const QString index = indexPath();
    IndexReader* reader = IndexReader::open(index.toStdString().c_str());
    QStringList ret;
    for(int32_t i = 0; i < reader->numDocs(); i++) {
        Document doc;
        reader->document(i, doc);

        ret.append(SearchTools::toQString(doc.get(_T("key"))));

    }
    m_entryList = ret;
    return ret;
}

QString BibleQuoteDict::indexPath() const
{
    return m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
}
Response::ResponseType BibleQuoteDict::responseType() const
{
    return Response::StringReponse;
}
