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
#include <QtCore/QFile>
#include <QtCore/QVariantList>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>
#include <QtXml/QDomElement>
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
    m_modulePath = fileName;
    //todo: progressdialog
    /* QProgressDialog progress(QObject::tr("Loading Strongmodule"), QObject::tr("Cancel"), 0, 100);
     progress.setWindowModality(Qt::WindowModal);
     progress.setValue(1);*/
    QString fileTitle = "";
    const QString index = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
    QDir dir("/");
    dir.mkpath(index);

    // do not use any stop words
    const TCHAR* stop_words[]  = { NULL };
     StandardAnalyzer an((const TCHAR**)stop_words);

    if( IndexReader::indexExists(index.toAscii().constData())) {
        if( IndexReader::isLocked(index.toAscii().constData())) {
             IndexReader::unlock(index.toAscii().constData());
        }
    }
    QScopedPointer< IndexWriter> writer(new  IndexWriter(index.toAscii().constData(), &an, true));   //always create a new index

    QByteArray textBuffer;
    wchar_t wcharBuffer[ IndexWriter::DEFAULT_MAX_FIELD_LENGTH + 1];

    KoXmlDocument doc;
    QString errorMsg;
    int eLine;
    int eCol;
    if(!doc.setContent(fileData, &errorMsg, &eLine, &eCol)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
        myWarning() << "the file isn't valid error = " << errorMsg << eLine << eCol << fileData.remove(1000, fileData.size());
        return QString();
    }

    // progress.setMaximum(doc.documentElement().childNodesCount()+1);
    KoXmlNode item = doc.documentElement().firstChild();
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
                                const QString url = "bible://current/" + QString::number(bookID) + "," + QString::number(chapterID) + "," + QString::number(verseID) + ",";
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

            QScopedPointer< Document> doc(new  Document());

            lucene_utf8towcs(wcharBuffer, id.toUtf8().constData(),      IndexWriter::DEFAULT_MAX_FIELD_LENGTH);

            doc->add(*(new  Field((const TCHAR*)_T("key"), (const TCHAR*)wcharBuffer,  Field::STORE_YES |  Field::INDEX_TOKENIZED)));

            lucene_utf8towcs(wcharBuffer, ret.toUtf8().constData(),      IndexWriter::DEFAULT_MAX_FIELD_LENGTH);

            doc->add(*(new  Field((const TCHAR*)_T("content"),
                                                   (const TCHAR*)wcharBuffer,
                                                    Field::STORE_YES |  Field::INDEX_TOKENIZED)));
            textBuffer.resize(0); //clean up
            writer->addDocument(doc.data());

        }
        item = item.nextSibling();
        c++;
    }
    doc.clear();
    doc.unload();

    writer->optimize();
    writer->close();

    return fileTitle;
}
/**
  Returns a Entry.
  \id The id of the entry.
  */
QString ZefaniaLex::getEntry(const QString &id)
{
    QString index = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
    const QString queryText = "key:" + id;
    char utfBuffer[  IndexWriter::DEFAULT_MAX_FIELD_LENGTH  + 1];
    wchar_t wcharBuffer[  IndexWriter::DEFAULT_MAX_FIELD_LENGTH + 1];
    const TCHAR* stop_words[]  = { NULL };
     StandardAnalyzer analyzer(stop_words);
     IndexSearcher searcher(index.toLocal8Bit().constData());
    lucene_utf8towcs(wcharBuffer, queryText.toUtf8().constData(),  IndexWriter::DEFAULT_MAX_FIELD_LENGTH);;
    QScopedPointer< Query> q( QueryParser::parse((const TCHAR*)wcharBuffer, (const TCHAR*)_T("content"), &analyzer));
    QScopedPointer< Hits> h(searcher.search(q.data(),  Sort::INDEXORDER));
     Document* doc = 0;
    for(int i = 0; i < h->length(); ++i) {
        doc = &h->doc(i);
        lucene_wcstoutf8(utfBuffer, (const wchar_t*)doc->get((const TCHAR*)_T("content")),  IndexWriter::DEFAULT_MAX_FIELD_LENGTH);
        QString content = QString::fromUtf8(utfBuffer);
        return content;
    }

    return QString();
}

