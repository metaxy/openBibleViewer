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
    KoXmlDocument doc;
    QString errorMsg;
    int eLine;
    int eCol;
    if(!doc.setContent(fileData, &errorMsg, &eLine, &eCol)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
        myWarning() << "the file isn't valid error = " << errorMsg << eLine << eCol << fileData.remove(1000, fileData.size());
        return QString();
    }
    QString fileTitle = "";
    QStringList l_id = QStringList();
    QStringList l_title = QStringList();
    QStringList l_trans = QStringList();
    QStringList l_pron = QStringList();
    QStringList l_desc = QStringList();
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
                                QStringList list = mscope.split(";");
                                int bookID = list.at(0).toInt() - 1;
                                int chapterID = list.at(1).toInt() - 1;
                                int verseID = list.at(2).toInt() - 1;
                                QString url = "bible://current/" + QString::number(bookID) + "," + QString::number(chapterID) + "," + QString::number(verseID) + ",";
                                QString name = "";

                                if(bookID < m_settings->bookNames.size()) {
                                    name = m_settings->bookNames.at(bookID) + " " + list.at(1) + "," + list.at(2);
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
            l_id << id;
            l_title << title;
            l_trans << trans;
            l_pron << pron;
            l_desc << desc;
        }
        item = item.nextSibling();
        c++;
    }
    doc.clear();
    QString index = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
    QDir dir("/");
    dir.mkpath(index);

    // do not use any stop words
    const TCHAR* stop_words[]  = { NULL };
    lucene::analysis::standard::StandardAnalyzer an((const TCHAR**)stop_words);

    if(lucene::index::IndexReader::indexExists(index.toAscii().constData())) {
        if(lucene::index::IndexReader::isLocked(index.toAscii().constData())) {
            lucene::index::IndexReader::unlock(index.toAscii().constData());
        }
    }
    QScopedPointer<lucene::index::IndexWriter> writer(new lucene::index::IndexWriter(index.toAscii().constData(), &an, true));   //always create a new index

    for(int i = 0; i < l_id.size(); ++i) {
        QString id = l_id.at(i);
        QString ret = "<h3 class='strongTitle'>" + l_id.at(i) + " - " + l_title.at(i) + "</h3>";
        if(!l_trans.at(i).isEmpty()) {
            ret += " (" + m_trans.at(i) + ") ";
        }
        if(!l_pron.at(i).isEmpty()) {
            ret += " [" + l_pron.at(i) + "] ";
        }
        ret += "<br />" + l_desc.at(i);
        //write into index

        QByteArray textBuffer;
        wchar_t wcharBuffer[    lucene::index::IndexWriter::DEFAULT_MAX_FIELD_LENGTH + 1];

        QScopedPointer<lucene::document::Document> doc(new lucene::document::Document());
        QString key = id;

        lucene_utf8towcs(wcharBuffer, key.toLocal8Bit().constData(),     lucene::index::IndexWriter::DEFAULT_MAX_FIELD_LENGTH);

        doc->add(*(new lucene::document::Field((const TCHAR*)_T("key"), (const TCHAR*)wcharBuffer, lucene::document::Field::STORE_YES | lucene::document::Field::INDEX_TOKENIZED)));

        lucene_utf8towcs(wcharBuffer, ret.toUtf8().constData(),     lucene::index::IndexWriter::DEFAULT_MAX_FIELD_LENGTH);

        doc->add(*(new lucene::document::Field((const TCHAR*)_T("content"),
                                               (const TCHAR*)wcharBuffer,
                                               lucene::document::Field::STORE_YES | lucene::document::Field::INDEX_TOKENIZED)));
        textBuffer.resize(0); //clean up
        writer->addDocument(doc.data());

    }
    writer->optimize();
    writer->close();

    return fileTitle;
}
/**
  Returns a Strong.
  \strongID The strongID
  */
QString ZefaniaLex::getStrong(const QString &strongID)
{
    QString index = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
    const QString queryText = "key:"+strongID;
    char utfBuffer[ lucene::index::IndexWriter::DEFAULT_MAX_FIELD_LENGTH  + 1];
    wchar_t wcharBuffer[ lucene::index::IndexWriter::DEFAULT_MAX_FIELD_LENGTH + 1];
    const TCHAR* stop_words[]  = { NULL };
    lucene::analysis::standard::StandardAnalyzer analyzer(stop_words);
    lucene::search::IndexSearcher searcher(index.toLocal8Bit().constData());
    lucene_utf8towcs(wcharBuffer, queryText.toUtf8().constData(), lucene::index::IndexWriter::DEFAULT_MAX_FIELD_LENGTH);;
    QScopedPointer<lucene::search::Query> q(lucene::queryParser::QueryParser::parse((const TCHAR*)wcharBuffer, (const TCHAR*)_T("content"), &analyzer));
    QScopedPointer<lucene::search::Hits> h(searcher.search(q.data(), lucene::search::Sort::INDEXORDER));
    lucene::document::Document* doc = 0;
    for(int i = 0; i < h->length(); ++i) {
        doc = &h->doc(i);
        lucene_wcstoutf8(utfBuffer, (const wchar_t*)doc->get((const TCHAR*)_T("content")), lucene::index::IndexWriter::DEFAULT_MAX_FIELD_LENGTH);
        QString content(utfBuffer);
        return content;
    }

    return QString();
}

