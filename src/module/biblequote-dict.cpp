/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#include <QtCore/QTextDecoder>
#include <QtCore/QDir>
#include <QtGui/QProgressDialog>
#include <QtCore/QTextStream>
#include "src/core/dbghelper.h"
#include "CLucene.h"
#include "CLucene/_clucene-config.h"

using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;

BibleQuoteDict::BibleQuoteDict()
{
}

void BibleQuoteDict::setSettings(Settings *set)
{
    m_settings = set;
}
void BibleQuoteDict::setID(const int &id, const QString &path)
{
    m_moduleID = id;
    m_modulePath = path;
}

/**
  Reads the ini file and returns the dictionary name.
  */
QString BibleQuoteDict::readInfo(QFile &file)
{
    //todo: use module default encoding
    const QString encoding = m_settings->encoding;
    QTextCodec *codec = QTextCodec::codecForName(encoding.toStdString().c_str());
    QTextDecoder *decoder = codec->makeDecoder();
    QByteArray byteline = file.readLine();
    QString line = decoder->toUnicode(byteline);
    file.close();
    return line.simplified();
}

bool BibleQuoteDict::hasIndex()
{
    DEBUG_FUNC_NAME
    QDir d;
    if(!d.exists(m_settings->homePath + "index")) {
        return false;
    }
    const QString index = indexPath();

    //todo: check versions
    return IndexReader::indexExists(index.toStdString().c_str());
}
int BibleQuoteDict::buildIndex()
{
    DEBUG_FUNC_NAME

    // pharse both and add docs to the indexwriter
    //myDebug() << m_modulePath;
    QFileInfo fileInfo(m_modulePath);
    //myDebug() << fileInfo.absoluteDir();
    QDir moduleDir(fileInfo.absoluteDir());
    moduleDir.setFilter(QDir::Files);
    QFileInfoList list = moduleDir.entryInfoList();

    QFileInfo htmlFileInfo;
    foreach(const QFileInfo &info, list) {
        if((info.suffix().compare("html",Qt::CaseInsensitive) == 0 || info.suffix().compare("htm",Qt::CaseInsensitive) == 0) && info.baseName().compare(fileInfo.baseName(), Qt::CaseInsensitive) == 0) {
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

    writer->setMaxFieldLength(0x7FFFFFFFL);
    writer->setUseCompoundFile(false);

    //index
    Document indexdoc;

    const QString title = configIn.readLine();
    QString id = configIn.readLine();
    long num = configIn.readLine().toLong();
    const QString pre = htmlIn.read(num - 1);
    myDebug() << title << pre;

    while(!configIn.atEnd()) {
        long n = num;
        const QString key = id;
        id = configIn.readLine();
        num = configIn.readLine().toLong();
        const QString data = htmlIn.read(num - n - 1);

        indexdoc.clear();
        indexdoc.add(*_CLNEW Field(_T("key"), key.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));
        indexdoc.add(*_CLNEW Field(_T("content"), data.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));
        writer->addDocument(&indexdoc);

    }

    writer->setUseCompoundFile(true);
    writer->optimize();

    writer->close();
    delete writer;
    return 0;
}

QString BibleQuoteDict::getEntry(const QString &key)
{
    DEBUG_FUNC_NAME
    if(!hasIndex()) {
        if(buildIndex() != 0) {
            return QObject::tr("Cannot build index.");
        }
    }
    const QString index = indexPath();
    const QString queryText = "key:" + key;

    const TCHAR* stop_words[] = { NULL };
    standard::StandardAnalyzer analyzer(stop_words);
    IndexReader* reader = IndexReader::open(index.toStdString().c_str());
    IndexSearcher s(reader);
    Query* q = QueryParser::parse(queryText.toStdWString().c_str(), _T("content"), &analyzer); //todo: or use querytext and as the field content
    Hits* h = s.search(q);
    QString ret = "";
    for(size_t i = 0; i < h->length(); i++) {
        Document* doc = &h->doc(i);
        if(!ret.isEmpty())
            ret.append("<hr /> ");
        ret.append(QString::fromWCharArray(doc->get(_T("content"))));
    }
    return ret.isEmpty() ? QObject::tr("Nothing found for %1").arg(key) : ret;
}
QStringList BibleQuoteDict::getAllKeys()
{
    DEBUG_FUNC_NAME
    if(!hasIndex())
        buildIndex();
    const QString index = indexPath();
    IndexReader* reader = IndexReader::open(index.toStdString().c_str());
    QStringList ret;
    for(size_t i = 0; i < reader->numDocs(); i++) {
        Document* doc = reader->document(i);
        ret.append(QString::fromWCharArray(doc->get(_T("key"))));
    }
    return ret;
}
QString BibleQuoteDict::indexPath() const
{
    return m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
}
