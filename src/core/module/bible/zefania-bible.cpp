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
#include "config.h"
#include "CLucene.h"
#include "CLucene/clucene-config.h"
#include "src/core/search/searchtools.h"
#include <QtGui/QTextDocument> //!!!
#include <QtXml/QXmlStreamReader>

#include "src/core/obvcore.h" //???
#include "src/core/raw/rmetadata.h"
#include <src/core/raw/chapterblock.h>
#include <src/core/raw/textfragment.h>

using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;
extern bool removeDir(const QString &dirName);
ZefaniaBible::ZefaniaBible()
{
    m_settings = 0;
    m_moduleID = 0;
    m_bookID = 0;
    m_modulePath = "";
    m_moduleName = "";
    m_xml = NULL;
    m_rightToLeft = false;
}
ZefaniaBible::~ZefaniaBible()
{
    myDebug() << "clear zefania " << m_moduleName;
    m_book.clear();
    if(m_xml != NULL) {
        delete m_xml;
        m_xml = NULL;
    }
    m_versification.clear();
}

int ZefaniaBible::loadBibleData(const int id, const QString &path)
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
    m_rightToLeft = (ModuleTools::languageToDirection(m_set->moduleLanguage) == Qt::RightToLeft);

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
    if(m_xml != NULL) {
        delete m_xml;
        m_xml = NULL;
    }
    QFile file(m_modulePath);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return;
    m_xml = new QXmlStreamReader(&file);
    QMap<int, BookV11N> map;
    QList< std::pair<qint64, qint64> > lines;
    if(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), "XMLBIBLE")) {
            while(m_xml->readNextStartElement()) {
                if(cmp(m_xml->name(), "BIBLEBOOK")) {
                    std::pair<qint64, qint64> p;//start and end of a biblebook
                    p.first = m_xml->lineNumber();
                    BookV11N b;
                    b.bookID = m_xml->attributes().value("bnumber").toString().toInt() - 1;
                    b.name = m_xml->attributes().value("bname").toString();
                    QStringList sname;
                    sname << m_xml->attributes().value("bsname").toString();
                    b.shortNames = sname;
                    //all chapters
                    while(m_xml->readNextStartElement()) {
                        if(cmp(m_xml->name(), "CHAPTER")) {
                            const int chapterID = m_xml->attributes().value("cnumber").toString().toInt();
                            b.maxChapter = chapterID;
                            //all Verse
                            int maxVerse = 0;
                            while(m_xml->readNextStartElement()) {
                                if(cmp(m_xml->name(), "VERS")) {
                                    const int verseID = m_xml->attributes().value("vnumber").toString().toInt();
                                    maxVerse = verseID;
                                }
                                m_xml->skipCurrentElement();

                            }
                            b.maxVerse << maxVerse;
                        } else {
                            m_xml->skipCurrentElement();
                        }

                    }
                    map.insert(b.bookID, b);
                    p.second = m_xml->lineNumber();
                    lines.append(p);
                } else {
                    m_xml->skipCurrentElement();
                }

            }
        }
    }
    delete m_xml;
    m_xml = NULL;
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
 * @brief ZefaniaBible::generateCache generates a cache for faster access by splitting the big book in small books
 * It uses a start and end points for a file the data from ZefaniaBible::getVersification(), see list
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

    for(int i = 0; i < list.size(); ++i) {
        std::pair<qint64, qint64> p = list.at(i);
        int bookID = m_set->getV11n()->bookIDs().at(i);
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

int ZefaniaBible::readBook(const int id)
{
    //DEBUG_FUNC_NAME;
    //myDebug() << id;
    if(m_xml != NULL) {
        delete m_xml;
        m_xml = NULL;
    }
    m_bookID = id;
    QString path;
    const QString cacheFile = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath) + "/" + QString::number(id) + ".xml";
    QFileInfo info(cacheFile);

    if(info.exists()) {
        path = cacheFile;
    } else {
        path = m_modulePath;
    }
    //myDebug() << path;

    genStrongsPrefix();
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return 1;
    m_xml = new QXmlStreamReader(&file);
    if(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), "XMLBIBLE")) {
            while(m_xml->readNextStartElement()) {
                if(cmp(m_xml->name(), "BIBLEBOOK")) {
                    if(m_xml->attributes().value("bnumber").toString().toInt() == id + 1) { //we cou
                        m_book = readBook();
                        delete m_xml;
                        m_xml = NULL;
                        return 0;
                    } else {
                        m_xml->skipCurrentElement();
                    }
                } else {
                    m_xml->skipCurrentElement();
                }
            }
        } else {
            myWarning() << "not a file";
        }
    }
    file.close();
    delete m_xml;
    m_xml = NULL;
    return 0;
}
void ZefaniaBible::genStrongsPrefix()
{
    foreach(int bookID, m_versification->bookIDs()) {
        QString add;
        if(m_versification->bookCount() == 66) {
            if(bookID < 39) {
                add = "H";
            } else {
                add = "G";
            }
        } else if(m_set->versificationName.endsWith("-nt")) {
            add = "G";
        } else if(m_set->versificationName.endsWith("-ot")) {
            add = "H";
        } else if(m_versification->bookCount() == 27) {
            add = "G";
        } else if(m_versification->bookCount() == 39) {
            add = "H";
        }
        m_strongsPrefix[bookID] = add;
    }
}

MetaInfo ZefaniaBible::readInfo(QFile &file)
{
    MetaInfo ret;
    if(m_xml != NULL) {
        delete m_xml;
        m_xml = NULL;
    }
    m_xml = new QXmlStreamReader(&file);
    if(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), "XMLBIBLE")) {
            if(m_xml->attributes().value("type") == "x-bible") {
                ret.setContent(ModuleTools::BibleContent);
                ret.setDefaultModule(ModuleTools::DefaultBibleModule);
            }
            while(m_xml->readNextStartElement()) {
                if(cmp(m_xml->name(), "INFORMATION")) {
                    ret = readMetaInfo(ret);
                    break;
                } else {
                    m_xml->skipCurrentElement();
                }
            }
        } else {
            myWarning() << "not a file";
        }
    }
    file.close();
    delete m_xml;
    m_xml = NULL;
    return ret;
}

MetaInfo ZefaniaBible::readInfo(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return MetaInfo();
    return readInfo(file);
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
    //DEBUG_FUNC_NAME

    if(m_xml != NULL) {
        delete m_xml;
        m_xml = NULL;
    }
    QFile file(m_modulePath);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return; // no deletes
    m_xml = new QXmlStreamReader(&file);

    QProgressDialog progress(QObject::tr("Build index"), QObject::tr("Cancel"), 0, m_versification->bookCount());
    const QString index = indexPath();
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
        if(cmp(m_xml->name(), "XMLBIBLE")) {
            while(m_xml->readNextStartElement()) {
                if(cmp(m_xml->name(), "BIBLEBOOK")) {

                    const int bookID = m_xml->attributes().value("bnumber").toString().toInt() - 1;
                    progress.setValue(bookID);
                    //all chapters
                    while(m_xml->readNextStartElement()) {
                        if(cmp(m_xml->name(), "CHAPTER")) {
                            const int chapterID = m_xml->attributes().value("cnumber").toString().toInt() - 1;
                            while(m_xml->readNextStartElement()) {
                                if(cmp(m_xml->name(), "VERS")) {
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
        removeDir(index);
    } else {
        writer->setUseCompoundFile(true);
        writer->optimize();
    }

    writer->close();
    delete writer;
    delete[] buffer;
    progress.close();
    file.close();
    delete m_xml;
    m_xml = NULL;
}

void ZefaniaBible::search(const SearchQuery &query, SearchResult *res) const
{
    const QString index = indexPath();

    const TCHAR* stop_words[] = { NULL };
    standard::StandardAnalyzer analyzer(stop_words);
    IndexReader* reader = IndexReader::open(index.toStdString().c_str());
    IndexSearcher s(reader);

    const wchar_t *queryText = SearchTools::toTCHAR(query.searchText);
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
    delete[] queryText;
}

/**
  * Returns the path, where all indexed files are stored.
  */
QString ZefaniaBible::indexPath() const
{
    //DEBUG_FUNC_NAME
    return m_settings->homePath + "index/" + m_settings->hash(m_modulePath);
}

QString ZefaniaBible::uid() const
{
    return m_uid;
}

TextRange ZefaniaBible::rawTextRange(int bookID, int chapterID, int startVerse, int endVerse)
{
    TextRange ret;
    if(m_book.bookID() != bookID) {
        myDebug() << "book not loaded";
        readBook(bookID);
    }
    if(!m_book.hasChapter(chapterID)) {
        myDebug() << m_book.data().keys();
        myWarning() << "index out of range index chapterID = " << chapterID;
        ret.setError(TextRange::NotFoundError);
        return ret;
    }
    ret.setModuleID(m_moduleID);
    ret.setBookID(bookID);
    ret.setChapterID(chapterID);

    const Chapter c = m_book.getChapter(chapterID);
    QMap<int, Verse> data = c.data();
    QMapIterator<int, Verse> i(data);
    while(i.hasNext()) {
        i.next();
        if(i.key() <= endVerse && i.key() >= startVerse)
            ret.addVerse(i.value());
    }
    return ret;
}

std::pair<int, int> ZefaniaBible::minMaxVerse(int bookID, int chapterID)
{
    std::pair<int, int> ret;
    if(m_book.bookID() != bookID) {
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

    return ret;
}

VerseBook ZefaniaBible::readBook()
{
    m_bookID = m_xml->attributes().value("bnumber").toString().toInt() - 1;
    VerseBook book(m_bookID);
    while(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), "CHAPTER")) {
            book.addChapter(readChapter());
        } else {
            m_xml->skipCurrentElement();
        }
    }
    return book;
}
BookBlock ZefaniaBible::rawReadBook(quint32 parent)
{
    m_bookID = m_xml->attributes().value("bnumber").toString().toInt() - 1;
    quint32 id = m_idGen.next();
    BookBlock book(id, RMetaData(parent, RMetaData::BookBlock));
    while(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), "CHAPTER")) {
            book.add(rawReadChapter(id));
        } else {
            m_xml->skipCurrentElement();
        }
    }
    return book;
}


Chapter ZefaniaBible::readChapter()
{
    const int chapterID = m_xml->attributes().value("cnumber").toString().toInt() - 1;
    Chapter chapter(chapterID);
    while(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), "VERS")) {
            chapter.addVerse(readVerse());
        } else {
            m_xml->skipCurrentElement();
        }
    }
    return chapter;
}

ChapterBlock ZefaniaBible::rawReadChapter(quint32 parent)
{
    const int chapterID = m_xml->attributes().value("cnumber").toString().toInt() - 1;
    quint32 id = m_idGen.next();
    ChapterBlock chapter(id, RMetaData(parent, RMetaData::ChapterBlock));
    
    while(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), "VERS")) {
            chapter.add(rawReadVerse(parent));
        } else {
            m_xml->skipCurrentElement();
        }
    }
    return chapter;
}


VerseBlock ZefaniaBible::rawReadVerse(quint32 parent)
{
    quint32 id = m_idGen.next();
    VerseBlock verse(id, RMetaData(parent, RMetaData::VerseBlock));
    
    const int verseID = m_xml->attributes().value("vnumber").toString().toInt() - 1;
    
    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), "VERS")))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            TextFragment t(m_idGen.next(), RMetaData(id, RMetaData::TextFragment));
            t.text = Qt::escape(m_xml->text().toString());
            verse.add(t);
        } else if(cmp(m_xml->name(), "STYLE") || m_xml->name() == "st") {
            //out += parseStyle();
        } else if(cmp(m_xml->name(), "NOTE")) {
            //out += parseNote();
        } else if(cmp(m_xml->name(), "BR")) {
            //out += parseBr();
        } else if(cmp(m_xml->name(), "DIV")) {
            //out += parseDiv();
        } else if(cmp(m_xml->name(), "GRAM") || m_xml->name() == QLatin1String("gr")) {
            //out += parseGram();
        } else if(cmp(m_xml->name(), "SUP")) {
            //out += parseSup();
        } else if(cmp(m_xml->name(), "XREF")) {
            //out += parseXRef();
        } else {
            //out += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return verse;
}
Verse ZefaniaBible::readVerse()
{
    const int verseID = m_xml->attributes().value("vnumber").toString().toInt() - 1;
    QString out;
    /*if(m_rightToLeft)
        out += "<div dir= \"rtl\">";*/
    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), "VERS")))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            out += Qt::escape(m_xml->text().toString());
        } else if(cmp(m_xml->name(), "STYLE") || m_xml->name() == "st") {
            out += parseStyle();
        } else if(cmp(m_xml->name(), "NOTE")) {
            out += parseNote();
        } else if(cmp(m_xml->name(), "BR")) {
            out += parseBr();
        } else if(cmp(m_xml->name(), "DIV")) {
            out += parseDiv();
        } else if(cmp(m_xml->name(), "GRAM") || m_xml->name() == QLatin1String("gr")) {
            out += parseGram();
        } else if(cmp(m_xml->name(), "SUP")) {
            out += parseSup();
        } else if(cmp(m_xml->name(), "XREF")) {
            out += parseXRef();
        } else {
            out += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
   /* if(m_rightToLeft)
         out += "</div>";*/

    Verse verse(verseID, out);
    if(m_rightToLeft)
        verse.setLayoutDirection(Qt::RightToLeft);
    return verse;
}
QString ZefaniaBible::parseStyle()
{
    QString ret;
    QString css = m_xml->attributes().value("css").toString();
    const QString pre("<span style=\"" + css + "\">");
    const QString post("</span>");
    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), "STYLE")  || m_xml->name() == "st"))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            ret += m_xml->text().toString();
        } else if(cmp(m_xml->name(), "STYLE") || m_xml->name() == "st") {
            ret += parseStyle();
        } else if(cmp(m_xml->name(), "GRAM") || m_xml->name() == "gr") {
            ret += parseGram();
        } else if(cmp(m_xml->name(), "SUP")) {
            ret += parseSup();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return pre + ret + post;
}
QString ZefaniaBible::parseNote()
{
    if(!m_set->displaySettings()->showStudyNotes()) {
        return "";
    }
    QString ret;
    const QString pre("<span class =\"studynote\">");
    const QString post("</span>");
    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), "NOTE")))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            ret += m_xml->text().toString();
        } else if(cmp(m_xml->name(), "STYLE") || m_xml->name() == "st") {
            ret += parseStyle();
        } else if(cmp(m_xml->name(), "BR")) {
            ret += parseBr();
        } else if(cmp(m_xml->name(), "GRAM") || m_xml->name() == "gr") {
            ret += parseGram();
        } else if(cmp(m_xml->name(), "XREF")) {
            ret += parseXRef();
        } else if(cmp(m_xml->name(), "SUP")) {
            ret += parseSup();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return pre + ret + post;
}
QString ZefaniaBible::parseBr()
{
    const QStringRef art = m_xml->attributes().value("art");
    m_xml->skipCurrentElement();
    if(art == "x-p")
        return "<div class=\"pageBreak\"></div>";
    else if(art == "x-nl")
        return "<br />";
    return "";
}

QString ZefaniaBible::parseGram()
{
    if(!m_set->displaySettings()->showStrong() && !m_set->displaySettings()->showRMAC()) {
        return "";
    }
    QString ret;
    const QString strong = m_xml->attributes().value("str").toString();
    const QString rmac = m_xml->attributes().value("rmac").toString();
    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), "GRAM") || m_xml->name() == "gr"))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            ret += m_xml->text().toString();
        } else if(cmp(m_xml->name(), "STYLE")  || m_xml->name() == QLatin1String("st")) {
            ret += parseStyle();
        } else if(cmp(m_xml->name(), "BR")) {
            ret += parseBr();
        } else if(cmp(m_xml->name(), "GRAM") || m_xml->name() == QLatin1String("gr")) {
            ret += parseGram();
        } else if(cmp(m_xml->name(), "SUP")) {
            ret += parseSup();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    if(!strong.isEmpty() && m_set->displaySettings()->showStrong()) {
        QString add;
        if(!strong.startsWith("G", Qt::CaseInsensitive) && !strong.startsWith("H", Qt::CaseInsensitive)) {
            add = m_strongsPrefix[m_bookID];
        }
        StrongUrl url;
        url.fromText(add+strong);

        if(m_set->displaySettings()->showStrongInline()) {
            ret.prepend("<a class=\"inlinestronglink\" href=\""+url.toString()+"\" title=\""+ add+strong +"\">");
            ret.append("</a>");
        } else {
            ret += "<a class=\"stronglink\" href=\""+url.toString()+"\" title=\""+ add+strong +"\">"
                    + add+strong
                    + "</a>";
        }
    }
    if(!rmac.isEmpty() && m_set->displaySettings()->showRMAC()) {
        ret += "<a class=\"rmaclink\" href=\"" + ModuleTools::rmacScheme + rmac + "\">" + rmac + "</a>";
    }
    return ret;
}

QString ZefaniaBible::parseSup()
{
    QString ret;
    const QStringRef art = m_xml->attributes().value("art");
    QString pre;
    QString post("</span>");
    if(art == "x-sub") {
        pre = "<span class =\"sub\">";
    } else if(art == "x-sup") {
        pre = "<span class =\"sup\">";
    } else {
        post = "";
    }

    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), "SUP")))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            ret += m_xml->text().toString();
        } else if(cmp(m_xml->name(), "STYLE") || m_xml->name() == "st") {
            ret += parseStyle();
        } else if(cmp(m_xml->name(), "GRAM")) {
            ret += parseGram();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return pre + ret + post;
}

QString ZefaniaBible::parseXRef()
{
    if(!m_set->displaySettings()->showRefLinks()) {
        return "";
    }
    const QStringRef vref = m_xml->attributes().value("vref");
    const QStringRef aix = m_xml->attributes().value("aix");
    const QStringRef title = m_xml->attributes().value("tile");
    const QStringRef fscope = m_xml->attributes().value("fscope");
    const QStringRef mscope = m_xml->attributes().value("mscope");
    m_xml->skipCurrentElement();

    if(!mscope.isEmpty()) {
        VerseUrl burl;
        burl.fromMscope(mscope.toString());

        QString text;
        if(!fscope.isEmpty()) {
            text = fscope.toString();
        } else  {
            RefText refText;
            refText.setSettings(m_settings);
            text = refText.toString(burl);
        }

        return "<span class=\"crossreference\"><a class=\"reflink\" href=\"" + burl.toString() + "\">" + text + "</a></span>";
    }
    return QString();
}

QString ZefaniaBible::parseDiv()
{
    QString ret;
    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), QLatin1String("DIV"))))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            ret += m_xml->text().toString();
        } else if(cmp(m_xml->name(), QLatin1String("NOTE"))) {
            ret += parseNote();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return ret;
}

MetaInfo ZefaniaBible::readMetaInfo(MetaInfo ret)
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

bool ZefaniaBible::cmp(const QStringRef &r, const QString &s)
{
    return r == s || r == s.toLower();
}

void ZefaniaBible::clearData()
{
    m_book.clear();
    if(m_xml != NULL) {
        delete m_xml;
        m_xml = NULL;
    }
}