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
#include "zefania-bible.h"
#include "src/core/verse/reftext.h"
#include "src/core/link/strongurl.h"
#include "src/core/zefania.h"
#include "config.h"
#include "CLucene.h"
#include "CLucene/clucene-config.h"
#include "src/core/search/searchtools.h"
#include <QTextDocument> //!!!
#include <QtXml/QXmlStreamReader>
#include <QDir>

#include "src/core/obvcore.h" //???
#include "src/core/raw/rmetadata.h"
#include <src/core/raw/chapterblock.h>
#include <src/core/raw/textfragment.h>
#include "src/core/raw/blocktools.h"
#include "src/core/raw/parser/rawtotextrangeparser.h"
#include <QtConcurrentRun>

using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;
extern bool removeDir(const QString &dirName);

ZefaniaBible::ZefaniaBible() : m_moduleID(-1), m_modulePath(""), m_moduleName("")
{
}
ZefaniaBible::~ZefaniaBible()
{
    m_versification.clear();
}
void ZefaniaBible::clearData()
{
    //todo: clear data
}
int ZefaniaBible::loadData(const int id, const QString &path)
{
   // DEBUG_FUNC_NAME;
    m_moduleID = id;
    m_moduleName = "";
    m_modulePath = path;
    m_set = m_settings->getModuleSettings(m_moduleID);
    if(!m_set) {
        return 1;
    }

    m_versification = m_set->loadVersification();

    if(m_set->noV11n() || !hasHardCache(m_modulePath)) {
        removeHardCache(m_modulePath);
        getVersification();
    }
    //m_rightToLeft = (ModuleTools::languageToDirection(m_set->moduleLanguage) == Qt::RightToLeft);

    m_moduleID = id;
    m_modulePath = path;
    m_uid = path;

    if(m_moduleName.isEmpty()) {
        m_moduleName = m_set->moduleName;
    }
    return 0;
}

void ZefaniaBible::removeHardCache(const QString &path)
{
    QDir d(m_settings->homePath + "cache/");
    d.rmdir(m_settings->homePath + "cache/" + m_settings->hash(path));
}
bool ZefaniaBible::hasHardCache(const QString &path)
{
    const QString pre = m_settings->homePath + "cache/" + m_settings->hash(path) + "/";

    foreach(int bookID, m_set->getV11n()->bookIDs()) {
        QFileInfo inf(pre + QString::number(bookID) + ".xml");
        if(!inf.exists() || !inf.isReadable()) {
            return false;
        }
    }
    return true;
}

void ZefaniaBible::getVersification()
{
    QFile file(m_modulePath);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return;
    QXmlStreamReader *xml = new QXmlStreamReader(&file);
    QMap<int, BookV11N> map;
    QList< std::pair<qint64, qint64> > lines;
    if(xml->readNextStartElement()) {
        if(ZefaniaXmlReader::cmp(xml->name(), "XMLBIBLE")) {
            while(xml->readNextStartElement()) {
                if(ZefaniaXmlReader::cmp(xml->name(), "BIBLEBOOK")) {
                    std::pair<qint64, qint64> p;//start and end of a biblebook
                    p.first = xml->lineNumber();
                    BookV11N b;
                    b.bookID = xml->attributes().value("bnumber").toString().toInt() - 1;
                    b.name = xml->attributes().value("bname").toString();
                    QStringList sname;
                    sname << xml->attributes().value("bsname").toString();
                    b.shortNames = sname;
                    //all chapters
                    while(xml->readNextStartElement()) {
                        if(ZefaniaXmlReader::cmp(xml->name(), "CHAPTER")) {
                            const int chapterID = xml->attributes().value("cnumber").toString().toInt();
                            b.maxChapter = chapterID;
                            //all Verse
                            int maxVerse = 0;
                            while(xml->readNextStartElement()) {
                                if(ZefaniaXmlReader::cmp(xml->name(), "VERS")) {
                                    const int verseID = xml->attributes().value("vnumber").toString().toInt();
                                    maxVerse = verseID;
                                }
                                xml->skipCurrentElement();

                            }
                            b.maxVerse << maxVerse;
                        } else {
                            xml->skipCurrentElement();
                        }

                    }
                    map.insert(b.bookID, b);
                    p.second = xml->lineNumber();
                    lines.append(p);
                } else {
                    xml->skipCurrentElement();
                }

            }
        }
    }
    delete xml;

    file.close();
    bool hasAny = false;
    foreach(const BookV11N &b, map) {
        if(!b.name.isEmpty()) {
            hasAny = true;
        }
    }

    if(!hasAny) {
        QString v = "kjv";
        if(map.size() == 27) {
            v += "-nt";
        } else if(map.size() == 39) {
            v += "-ot";
        }
        //myDebug() << v;
        m_set->versificationFile = "";
        m_set->versificationName = v;
        m_versification = m_set->loadVersification();

    } else {
        m_versification = QSharedPointer<Versification>(new Versification_Cache(map));
        m_set->v11n = m_versification.toWeakRef();
        m_set->versificationFile = m_settings->v11nFile(m_modulePath);
        m_set->versificationName = "";

        m_set->saveVersification();
    }
    generateCache(lines);    //make cache

}
/**
 * ZefaniaBible::generateCache generates a cache for faster access by splitting the big book in small books
 * It uses a start and end points for a file the data from ZefaniaBible::getVersification(), see list.
 * You don't need to call it.
 * @param list
 */
void ZefaniaBible::generateCache(QList<std::pair<qint64, qint64> > list)
{
    //DEBUG_FUNC_NAME;
    const QString pre = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath) + "/";
    QDir d(pre);
    if(!d.exists())
        d.mkpath(pre);

    QFile file(m_modulePath);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return;
    QSharedPointer<Versification> v11n = m_set->getV11n();
    for(int i = 0; i < list.size(); ++i) {
        std::pair<qint64, qint64> p = list.at(i);
        int bookID = v11n->bookIDs().at(i);
        QFile file2(pre + QString::number(bookID) + ".xml");
        //myDebug() << file2.fileName();
        if(!file2.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        //skip the first part
        if(i == 0) {
            for(int y = 0; y < p.first - 1; y++) {
                file.readLine();
            }
        }
        file2.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?><XMLBIBLE>");

        for(int y = p.first; y <= p.second; y++) {
            file2.write(file.readLine());
        }
        file2.write("</XMLBIBLE>");
        file2.close();
    }
}

int ZefaniaBible::moduleID() const
{
    return m_moduleID;
}
QString ZefaniaBible::modulePath() const
{
    return m_modulePath;
}
QString ZefaniaBible::moduleName(bool preferShortName) const
{
    //todo: use preferShortName
    return m_moduleName;
}

bool ZefaniaBible::hasIndex() const
{
    //DEBUG_FUNC_NAME
    const QString index = indexPath();
    QDir d;
    if(!d.exists(index)) {
        return false;
    }

    return IndexReader::indexExists(index.toStdString().c_str());
}
void ZefaniaBible::buildIndex()
{
    DEBUG_FUNC_NAME
    ZefaniaXmlReader reader(m_modulePath, m_versification);
    reader.buildIndex(indexPath());
}

void ZefaniaXmlReader::buildIndex(const QString &indexPath)
{
    //DEBUG_FUNC_NAME
    create();

    QProgressDialog progress(QObject::tr("Build index"), QObject::tr("Cancel"), 0, m_versification->bookCount());
    const QString index = indexPath;
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

    //index
    Document doc;
    bool canceled = false;

    TCHAR *buffer = SearchTools::createBuffer();
    if(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), XmlBible)) {
            while(m_xml->readNextStartElement()) {
                if(cmp(m_xml->name(), Biblebook)) {

                    const int bookID = m_xml->attributes().value("bnumber").toString().toInt() - 1;
                    progress.setValue(bookID);
                    //all chaptersQFuture<void> future = QtConcurrent::run(&image, &QImage::invertPixels, QImage::InvertRgba);
                    while(m_xml->readNextStartElement()) {
                        if(cmp(m_xml->name(), Chapter)) {
                            const int chapterID = m_xml->attributes().value("cnumber").toString().toInt() - 1;
                            while(m_xml->readNextStartElement()) {
                                if(cmp(m_xml->name(), ZefaniaXmlReader::Vers)) {
                                    const int verseID = m_xml->attributes().value("vnumber").toString().toInt() - 1;
                                    const QString t = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);

                                    doc.clear();
                                    const QString key = QString::number(bookID) + ";" + QString::number(chapterID) + ";" + QString::number(verseID);
                                    doc.add(*new Field(_T("key"), SearchTools::toTCHAR(key, buffer), Field::STORE_YES |  Field::INDEX_NO));
                                    doc.add(*new Field(_T("content"), SearchTools::toTCHAR(t, buffer), Field::STORE_YES |  Field::INDEX_TOKENIZED));

                                    writer->addDocument(&doc);
                                } else {
                                    m_xml->skipCurrentElement();
                                }

                            }
                        } else {
                            m_xml->skipCurrentElement();
                        }

                    }
                } else {
                    m_xml->skipCurrentElement();
                }

            }
        } else {
            myDebug() << "not a file";
        }
    }
    if(canceled) {
        ModuleTools::removeDir(index);
    } else {
        writer->setUseCompoundFile(true);
        writer->optimize();
    }

    writer->close();
    delete writer;
    delete[] buffer;
    progress.close();
    destroy();
}

void ZefaniaBible::search(const SearchQuery &query, SearchResult *res) const
{
    const QString index = indexPath();

    const TCHAR* stop_words[] = { NULL };
    standard::StandardAnalyzer analyzer(stop_words);
    IndexReader* reader = IndexReader::open(index.toStdString().c_str());
    IndexSearcher s(reader);

    auto queryText = SearchTools::toTCHAR(query.searchText);
    Query* q = QueryParser::parse(queryText, _T("content"), &analyzer);

    Hits* h = s.search(q);
    for(size_t i = 0; i < h->length(); i++) {
        Document* doc = &h->doc(i);
        const QString stelle = SearchTools::toQString(doc->get(_T("key")));
        const QStringList l = stelle.split(";");
        //hacky filter
        if(l.size() < 2)
            continue;
        if(query.range == SearchQuery::Whole || (query.range == SearchQuery::OT && l.at(0).toInt() <= 38) || (query.range == SearchQuery::NT && l.at(0).toInt() > 38)) {
            SearchHit hit;
            hit.setType(SearchHit::BibleHit);
            hit.setValue(SearchHit::ModuleID, m_moduleID);
            hit.setValue(SearchHit::BookID, l.at(0).toInt());
            hit.setValue(SearchHit::ChapterID, l.at(1).toInt());
            hit.setValue(SearchHit::VerseID, l.at(2).toInt());
            hit.setValue(SearchHit::VerseText, SearchTools::toQString(doc->get(_T("content"))));

            hit.setValue(SearchHit::Score, (double) h->score(i));

            res->addHit(hit);
        }
    }
    reader->close();
    delete reader;
#ifndef Q_WS_WIN //it just crashes on windows
    delete[] queryText;
#endif
}

/**
  * Returns the path, where all indexed files are stored.
  */
QString ZefaniaBible::indexPath() const
{
    return m_settings->homePath + "index/" + m_settings->hash(m_modulePath);
}

QString ZefaniaBible::uid() const
{
    return m_uid;
}
QString ZefaniaBible::path(const int book)
{
    const QString cacheFile = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath) + "/" + QString::number(book) + ".xml";
    QFileInfo info(cacheFile);
    if(info.exists()) {
        return cacheFile;
    } else {
        return m_modulePath;
    }
}

TextRange ZefaniaBible::rawTextRange(int bookID, int chapterID, int startVerse, int endVerse)
{
    //myDebug() << bookID << path(bookID);

    ZefaniaXmlReader reader(path(bookID), m_versification);

    ChapterBlock block = reader.readChapterBlock(bookID, chapterID);
    RawToTextRangeParser parser(m_set->displaySettings());

    TextRange ret = parser.toTextRange(&block);
    ret.setModuleID(m_moduleID);
    ret.setBookID(bookID);
    ret.filter(startVerse, endVerse);
    return ret;
}

std::pair<int, int> ZefaniaBible::minMaxVerse(int bookID, int chapterID)
{
    std::pair<int, int> ret;
    ret.first = 0;
    ret.second = 10;
    /*if(m_book.bookID() != bookID) {
        readBook(bookID);
        myDebug() << "book not loaded";
    }
    if(!m_book.hasChapter(chapterID)) {
        myDebug() << m_book.data().keys();
        myWarning() << "index out of range index chapterID = " << chapterID;
        return ret;
    }

    const Chapter c = m_book.getChapter(chapterID);
    //because c.data() is a map and it is sorted by key
    ret.first = c.data().keys().first();
    ret.second = c.data().keys().last();
	*/
    return ret;
}
MetaInfo ZefaniaBible::readInfo(const QString &fileName)
{
    //DEBUG_FUNC_NAME;
    ZefaniaXmlReader reader(fileName);
    return reader.readMetaInfo();
}
