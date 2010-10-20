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
#include "zefania-lex.h"

#include <QtXml/QDomNode>
#include "src/core/KoXmlReader.h"
#include "src/core/dbghelper.h"
#include "src/core/bibleurl.h"
#include <QtCore/QFile>
#include <QtCore/QVariantList>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>
#include <QtXml/QDomElement>
#include "config.h"
#ifdef _CLUCENE_LEGACY
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

#else
#include <CLucene.h>
#include <CLucene/util/dirent.h>
#include <CLucene/util/StringBuffer.h>
#include "CLucene/StdHeader.h"
#include "CLucene/_clucene-config.h"

#include "CLucene/config/repl_tchar.h"
#include "CLucene/config/repl_wchar.h"
#include "CLucene/util/Misc.h"

using namespace lucene::store;
using namespace std;
using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::util;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;
#endif
ZefaniaLex::ZefaniaLex()
{
}
void ZefaniaLex::setSettings(Settings *settings)
{
    m_settings = settings;
}
/**
  Load a Zefania XML Lex file the first time. Generate a cache file for fast access.
  */
QString ZefaniaLex::loadFile(QString fileData, QString fileName)
{
    DEBUG_FUNC_NAME
    m_modulePath = fileName;

    //todo: progressdialog
    /* QProgressDialog progress(QObject::tr("Loading Strongmodule"), QObject::tr("Cancel"), 0, 100);
     progress.setWindowModality(Qt::WindowModal);
     progress.setValue(1);*/


    QString fileTitle = "";

    const QString index = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
    QDir dir("/");
    dir.mkpath(index);
    #ifdef _CLUCENE_LEGACY
    // do not use any stop words
    const TCHAR* stop_words[]  = { NULL };
    StandardAnalyzer an((const TCHAR**)stop_words);

    if(IndexReader::indexExists(index.toAscii().constData())) {
        if(IndexReader::isLocked(index.toAscii().constData())) {
            IndexReader::unlock(index.toAscii().constData());
        }
    }
    QScopedPointer< IndexWriter> writer(new  IndexWriter(index.toAscii().constData(), &an, true));   //always create a new index

    QByteArray textBuffer;
    wchar_t wcharBuffer[ IndexWriter::DEFAULT_MAX_FIELD_LENGTH + 1];
    #else
    IndexWriter* writer = NULL;
    const TCHAR* stop_words[] = { NULL };
    standard::StandardAnalyzer an(stop_words);

    if(IndexReader::indexExists(index.toStdString().c_str())) {
        if(IndexReader::isLocked(index.toStdString().c_str())) {
            myDebug() << "Index was locked... unlocking it.";
            IndexReader::unlock(index.toStdString().c_str());
        }
    }
    writer = _CLNEW IndexWriter(index.toStdString().c_str() , &an, true);

    writer->setMaxFieldLength(0x7FFFFFFFL);
    writer->setUseCompoundFile(false);

    //index
    Document indexdoc;
    #endif
    KoXmlDocument xmldoc;
    QString errorMsg;
    int eLine;
    int eCol;
    if(!xmldoc.setContent(fileData, &errorMsg, &eLine, &eCol)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
        myWarning() << "the file isn't valid error = " << errorMsg << eLine << eCol << fileData.remove(1000, fileData.size());
        return QString();
    }

    // progress.setMaximum(doc.documentElement().childNodesCount()+1);
    KoXmlNode item = xmldoc.documentElement().firstChild();
    for(int c = 0; !item.isNull();) {
        //  progress.setValue(c+2);
        QString id = "";
        QString title = "";
        QString trans = "";
        QString pron = "";
        QString desc = "";
        KoXmlElement e = item.toElement();
        if(e.tagName() == "INFORMATION") {
            KoXmlNode titel = item.namedItem("title");
            fileTitle = titel.toElement().text();
        } else if(e.tagName() == "item") {
            id = e.attribute("id");
            KoXmlNode details = item.firstChild();
            while(!details.isNull()) {
                KoXmlElement edetails = details.toElement();
                if(edetails.tagName() == "title") {
                    title = edetails.text();
                } else if(edetails.tagName() == "transliteration") {
                    trans =  edetails.text();
                } else if(edetails.tagName() == "pronunciation") {
                    KoXmlNode em = details.firstChild();
                    while(!em.isNull()) {
                        if(em.toElement().tagName() == "em")
                            pron = "<em>" + em.toElement().text() + "</em>";
                        em = em.nextSibling();
                    }
                } else if(edetails.tagName() == "description") {
                    KoXmlNode descNode = details.firstChild();
                    while(!descNode.isNull()) {
                        if(descNode.nodeType() == 2) {
                            desc += descNode.toText().data();
                        } else if(descNode.nodeType() == 1) {
                            KoXmlElement descElement  = descNode.toElement();
                            if(descElement.tagName() == "reflink") {
                                QString mscope = descElement.attribute("mscope", ";;;");
                                const QStringList list = mscope.split(";");
                                const int bookID = list.at(0).toInt() - 1;
                                const int chapterID = list.at(1).toInt() - 1;
                                const int verseID = list.at(2).toInt() - 1;

                                BibleUrl burl;
                                burl.setBible(BibleUrl::LoadCurrentBible);
                                burl.setBookID(bookID);
                                burl.setChapterID(chapterID);
                                burl.setVerseID(verseID);
                                const QString url = burl.toString();

                                QString name = "";

                                if(bookID < m_settings->bookFullNames.size()) {
                                    name = m_settings->bookFullNames.at(bookID) + " " + list.at(1) + "," + list.at(2);
                                } else {
                                    name = list.at(0) + " " + list.at(1) + "," + list.at(2);
                                }
                                desc += " <a href=\"" + url + "\">" + name + "</a> ";
                            }
                        }
                        descNode = descNode.nextSibling();
                    }
                    desc += "<hr />";
                }
                details = details.nextSibling();
            }
            QString ret = "<h3 class='strongTitle'>" + id + " - " + title + "</h3>";
            if(!trans.isEmpty()) {
                ret += " (" + trans + ") ";
            }
            if(!pron.isEmpty()) {
                ret += " [" + pron + "] ";
            }
            ret += "<br />" + desc;
            #ifdef _CLUCENE_LEGACY
            QScopedPointer< Document> doc(new  Document());

            lucene_utf8towcs(wcharBuffer, id.toUtf8().constData(),      IndexWriter::DEFAULT_MAX_FIELD_LENGTH);

            doc->add(*(new  Field((const TCHAR*)_T("key"), (const TCHAR*)wcharBuffer,  Field::STORE_YES |  Field::INDEX_TOKENIZED)));

            lucene_utf8towcs(wcharBuffer, ret.toUtf8().constData(),      IndexWriter::DEFAULT_MAX_FIELD_LENGTH);

            doc->add(*(new  Field((const TCHAR*)_T("content"),
                                  (const TCHAR*)wcharBuffer,
                                  Field::STORE_YES |  Field::INDEX_TOKENIZED)));
            textBuffer.resize(0); //clean up
            writer->addDocument(doc.data());
            #else
            indexdoc.clear();
            indexdoc.add(*_CLNEW Field(_T("key"), id.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));
            indexdoc.add(*_CLNEW Field(_T("content"), ret.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));
            writer->addDocument(&indexdoc);
            #endif

        }
        item = item.nextSibling();
        c++;
    }
    #ifdef _CLUCENE_LEGACY
    doc.clear();
    doc.unload();

    writer->optimize();
    writer->close();

    #else
    writer->setUseCompoundFile(true);
    writer->optimize();

    writer->close();
    _CLLDELETE(writer);
    #endif
    return fileTitle;
}
/**
  Returns a Entry.
  \id The id of the entry.
  */
QString ZefaniaLex::getEntry(const QString &id)
{
    DEBUG_FUNC_NAME
    const QString index = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
    const QString queryText = "key:" + id;
    myDebug() << "index = " << index;
#ifdef _CLUCENE_LEGACY

    char utfBuffer[  IndexWriter::DEFAULT_MAX_FIELD_LENGTH  + 1];
    wchar_t wcharBuffer[  IndexWriter::DEFAULT_MAX_FIELD_LENGTH + 1];
    const TCHAR* stop_words[]  = { NULL };
    StandardAnalyzer analyzer(stop_words);
    IndexSearcher searcher(index.toLocal8Bit().constData());
    lucene_utf8towcs(wcharBuffer, queryText.toUtf8().constData(),  IndexWriter::DEFAULT_MAX_FIELD_LENGTH);;
    QScopedPointer< Query> q(QueryParser::parse((const TCHAR*)wcharBuffer, (const TCHAR*)_T("content"), &analyzer));
    QScopedPointer< Hits> h(searcher.search(q.data(),  Sort::INDEXORDER));
    Document* doc = 0;
    for(int i = 0; i < h->length(); ++i) {
        doc = &h->doc(i);
        lucene_wcstoutf8(utfBuffer, (const wchar_t*)doc->get((const TCHAR*)_T("content")),  IndexWriter::DEFAULT_MAX_FIELD_LENGTH);
        QString content = QString::fromUtf8(utfBuffer);
        return content;
    }
#else
    const TCHAR* stop_words[] = { NULL };
    standard::StandardAnalyzer analyzer(stop_words);
    IndexReader* reader = IndexReader::open(index.toStdString().c_str());
    IndexSearcher s(reader);
    Query* q = QueryParser::parse(queryText.toStdWString().c_str(), _T("content"), &analyzer); //todo: or use querytext and as the field content
    Hits* h = s.search(q);
    myDebug() << "hits = " << h->length() << "queryText = " << queryText;
    for(size_t i = 0; i < h->length(); i++) {
        Document* doc = &h->doc(i);
        return QString::fromWCharArray(doc->get(_T("content")));
    }
    return QString();
#endif
}

