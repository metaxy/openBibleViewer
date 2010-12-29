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

#include "src/core/dbghelper.h"
#include "src/core/bible/bibleurl.h"
#include <QtCore/QFile>
#include <QtCore/QVariantList>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>
#include "src/core/KoXmlReader.h"
#include "CLucene.h"
#include "CLucene/_clucene-config.h"

using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;

ZefaniaLex::ZefaniaLex()
{
}
void ZefaniaLex::setSettings(Settings *settings)
{
    m_settings = settings;
}
void ZefaniaLex::setID(int moduleID, const QString &path)
{
    m_moduleID = moduleID;
    m_modulePath = path;
}

/**
  Load a Zefania XML Lex file the first time. Generates an index for fast access.
  */
QString ZefaniaLex::buildIndexFromData(const QString &fileData, const QString &fileName)
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
        return QString();
    }
    return buildIndexFromXmlDoc(&xmldoc);
}
QString ZefaniaLex::buildIndexFromFile(const QString &fileName)
{
    KoXmlDocument xmldoc;
    QString errorMsg;
    int eLine;
    int eCol;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return QString();

    if(!xmldoc.setContent(&file, &errorMsg, &eLine, &eCol)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
        myWarning() << "the file isn't valid , error = " << errorMsg
                    << " line = " << eLine
                    << " column = " << eCol;
        return QString();
    }
    const QString ret = buildIndexFromXmlDoc(&xmldoc);
    file.close();
    return ret;
}

/**
  Returns a Entry.
  \id The key of the entry.
  */
QString ZefaniaLex::getEntry(const QString &key)
{
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
    Query* q = QueryParser::parse(queryText.toStdWString().c_str(), _T("content"), &analyzer);
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

QStringList ZefaniaLex::getAllKeys()
{
    if(!hasIndex()) {
        if(buildIndex() != 0) {
            return QStringList();
        }
    }
    const QString index = indexPath();
    IndexReader* reader = IndexReader::open(index.toStdString().c_str());
    QStringList ret;
    for(int i = 0; i < reader->numDocs(); i++) {
        Document* doc = reader->document(i);
        ret.append(QString::fromWCharArray(doc->get(_T("key"))));
    }
    return ret;
}
QString ZefaniaLex::indexPath() const
{
    return m_settings->homePath + "cache/" + m_settings->hash(m_modulePath);
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
    return buildIndexFromXmlDoc(&xmlDoc).isEmpty() ? 2 : 0;
}

QString ZefaniaLex::buildIndexFromXmlDoc(KoXmlDocument *xmldoc)
{
    Document indexdoc;
    const QString index = indexPath();
    QString fileTitle = "";
    QDir dir("/");
    dir.mkpath(index);

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
    for(int c = 0; !item.isNull();) {
        QString key = "";
        QString title = "";
        QString trans = "";
        QString pron = "";
        QString desc = "";
        KoXmlElement e = item.toElement();
        if(e.tagName().compare("INFORMATION", Qt::CaseInsensitive) == 0) {
            KoXmlNode titel = item.namedItem("title");
            fileTitle = titel.toElement().text();
        } else if(e.tagName().compare("item", Qt::CaseInsensitive) == 0) {
            key = e.attribute("id");
            KoXmlNode details = item.firstChild();
            while(!details.isNull()) {
                KoXmlElement edetails = details.toElement();
                if(edetails.tagName().compare("title", Qt::CaseInsensitive) == 0) {
                    title = edetails.text();
                } else if(edetails.tagName().compare("transliteration", Qt::CaseInsensitive) == 0) {
                    trans =  edetails.text();
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
                            KoXmlElement descElement  = descNode.toElement();
                            if(descElement.tagName().compare("reflink", Qt::CaseInsensitive) == 0) {
                                QString mscope = descElement.attribute("mscope", ";;;");
                                const QStringList list = mscope.split(";");
                                const int bookID = list.at(0).toInt() - 1;
                                const int chapterID = list.at(1).toInt() - 1;
                                const int verseID = list.at(2).toInt() - 1;

                                BibleUrl burl;
                                BibleUrlRange range;

                                range.setBible(BibleUrlRange::LoadCurrentBible);
                                range.setBook(bookID);
                                range.setChapter(chapterID);
                                range.setWholeChapter();
                                range.setActiveVerse(verseID);
                                burl.addRange(range);
                                const QString url = burl.toString();


                                QString name;
                                if(bookID < m_settings->defaultVersification->getBookNames(Versification::ReturnAll).size()) {
                                    name = m_settings->defaultVersification->getBookNames(Versification::ReturnAll).at(bookID) + " " + list.at(1) + "," + list.at(2);
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
            QString content = "<h3 class='strongTitle'>" + key + " - " + title + "</h3>";
            if(!trans.isEmpty()) {
                content += " (" + trans + ") ";
            }
            if(!pron.isEmpty()) {
                content += " [" + pron + "] ";
            }
            content += "<br />" + desc;
            indexdoc.clear();
            indexdoc.add(*new Field(_T("key"), key.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));
            indexdoc.add(*new Field(_T("content"), content.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));
            writer->addDocument(&indexdoc);

        }
        item = item.nextSibling();
        c++;
    }
    writer->setUseCompoundFile(true);
    writer->optimize();

    writer->close();
    delete writer;
    return fileTitle;
}
