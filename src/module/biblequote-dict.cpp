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

#ifdef CLUCENE_LEGACY
const unsigned long MAX_LUCENE_FIELD_LENGTH = 1024 * 512;
#include <CLucene.h>
#include <CLucene/util/Misc.h>
#include <CLucene/util/Reader.h>
#ifndef Q_OS_WIN32
using namespace lucene::search;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::analysis::standard;
#endif
#endif

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
    /*DEBUG_FUNC_NAME
    QDir d;
    if(!d.exists(m_settings->homePath + "index")) {
        return false;
    }
    //todo: check versions
    QString index = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
    return  IndexReader::indexExists(index.toAscii().constData());*/
    return false;
}
void BibleQuoteDict::buildIndex()
{
    /*DEBUG_FUNC_NAME

    // pharse both and add docs to the indexwriter
    //myDebug() << m_modulePath;
    QFileInfo fileInfo(m_modulePath);
    //myDebug() << fileInfo.absoluteDir();
    QDir moduleDir(fileInfo.absoluteDir());
    moduleDir.setFilter(QDir::Files);
    QFileInfoList list = moduleDir.entryInfoList();

    QFileInfo htmlFileInfo;
    foreach(QFileInfo info, list) {
        //myDebug() << info.suffix() << info.baseName() << fileInfo.baseName();
        if((info.suffix() == "html" || info.suffix() == "htm") && info.baseName().compare(fileInfo.baseName(), Qt::CaseInsensitive) == 0) {
            htmlFileInfo = info;
        }
    }
    //myDebug() << htmlFileInfo.absoluteFilePath();

    QFile configFile(fileInfo.absoluteFilePath());
    QFile htmlFile(htmlFileInfo.absoluteFilePath());

    const QString encoding = m_settings->encoding;
    QTextCodec *codec = QTextCodec::codecForName(encoding.toStdString().c_str());

    if(!configFile.open(QIODevice::ReadOnly | QIODevice::Text) || !htmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream configIn(&configFile);
    configIn.setCodec(codec);
    QTextStream htmlIn(&htmlFile);
    htmlIn.setCodec(codec);

    QString index = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
    QDir dir("/");
    dir.mkpath(index);

    // do not use any stop words
    const TCHAR* stop_words[]  = { NULL };
    StandardAnalyzer an((const TCHAR**)stop_words);

    if(IndexReader::indexExists(index.toAscii().constData())) {
        if(IndexReader::isLocked(index.toAscii().constData())) {
            IndexReader::unlock(index.toAscii().constData());
        }
    }
    QScopedPointer< IndexWriter> writer(new  IndexWriter(index.toAscii().constData(), &an, true));   //always create a new index

    QProgressDialog progress(QObject::tr("Build index"), QObject::tr("Cancel"), 0, 0);
    progress.setWindowModality(Qt::WindowModal);

    QByteArray textBuffer;
    wchar_t wcharBuffer[MAX_LUCENE_FIELD_LENGTH + 1];
    const QString title = configIn.readLine();
    QString id = configIn.readLine();
    long num = configIn.readLine().toLong();
    const QString pre = htmlIn.read(num - 1);
    //myDebug() << title << pre;

    while(!configIn.atEnd()) {

        int n = num;
        QString key = id;

        id = configIn.readLine();
        num = configIn.readLine().toLong();
        const QString data = htmlIn.read(num - n - 1);
        //myDebug() << num << n << key << data;
        QScopedPointer< Document> doc(new  Document());

        lucene_utf8towcs(wcharBuffer, key.toUtf8().constData(), MAX_LUCENE_FIELD_LENGTH);

        doc->add(*(new  Field((const TCHAR*)_T("key"), (const TCHAR*)wcharBuffer,  Field::STORE_YES |  Field::INDEX_TOKENIZED)));

        lucene_utf8towcs(wcharBuffer, data.toUtf8().constData(), MAX_LUCENE_FIELD_LENGTH);

        doc->add(*(new  Field((const TCHAR*)_T("content"),
                              (const TCHAR*)wcharBuffer,
                              Field::STORE_YES |  Field::INDEX_TOKENIZED)));
        textBuffer.resize(0); //clean up
        writer->addDocument(doc.data());
    }

    writer->optimize();
    writer->close();
    progress.close();*/
}

QString BibleQuoteDict::getEntry(const QString &id)
{
    /*DEBUG_FUNC_NAME
    if(!hasIndex())
        buildIndex();

    QString index = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
    const QString queryText = "key:" + id;
    //myDebug() << queryText;

    char utfBuffer[ MAX_LUCENE_FIELD_LENGTH  + 1];
    wchar_t wcharBuffer[ MAX_LUCENE_FIELD_LENGTH + 1];
    const TCHAR* stop_words[]  = { NULL };
    StandardAnalyzer analyzer(stop_words);
    IndexSearcher searcher(index.toLocal8Bit().constData());
    lucene_utf8towcs(wcharBuffer, queryText.toUtf8().constData(), MAX_LUCENE_FIELD_LENGTH);;
    QScopedPointer< Query> q(QueryParser::parse((const TCHAR*)wcharBuffer, (const TCHAR*)_T("content"), &analyzer));
    QScopedPointer< Hits> h(searcher.search(q.data(),  Sort::INDEXORDER));
    Document* doc = 0;
    //myDebug() << h->length();
    QString ret = "";
    for(int i = 0; i < h->length(); ++i) {
        doc = &h->doc(i);
        lucene_wcstoutf8(utfBuffer, (const wchar_t*)doc->get((const TCHAR*)_T("content")), MAX_LUCENE_FIELD_LENGTH);
        if(ret.isEmpty())
            ret.append(QString::fromUtf8(utfBuffer));
        else
            ret.append("<hr /> " + QString::fromUtf8(utfBuffer));
    }

    return ret;*/
    return QString();
}
