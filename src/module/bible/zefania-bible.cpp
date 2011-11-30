/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "config.h"
#include "CLucene.h"
#include "CLucene/clucene-config.h"
#include <QXmlStreamReader>
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
    m_versification = NULL;
    m_xml = NULL;
}

int ZefaniaBible::loadBibleData(const int id, const QString &path)
{
    DEBUG_FUNC_NAME;
    m_moduleID = id;
    m_moduleName = "";
    m_modulePath = path;
    m_set = m_settings->getModuleSettings(m_moduleID);
    if(!m_set) {
        return 1;
    }

    m_set->loadVersification();

    if(m_set->v11n == NULL || !hasHardCache(m_modulePath)) {
        removeHardCache(m_modulePath);
        getVersification();
    }
    m_versification = m_set->v11n;



    m_moduleID = id;
    m_modulePath = path;
    m_uid = path;

    if(m_moduleName == "") {
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
    const QString pre = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath) + "/";

    foreach(int bookID, m_set->v11n->bookIDs()) {
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
                    std::pair<qint64, qint64> p;
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
    foreach(BookV11N b, map) {
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
        myDebug() << v;
        m_set->versificationFile = "";
        m_set->versificationName = v;
        m_set->loadVersification();

    } else {
        m_set->v11n = new Versification_Cache(map);

        m_set->versificationFile = m_settings->v11nFile(m_modulePath);
        m_set->versificationName = "";

        m_set->saveVersification();
    }
    generateCache(lines);
    //make cache

}
void ZefaniaBible::generateCache(QList<std::pair<qint64, qint64> > list)
{
    DEBUG_FUNC_NAME;
    const QString pre = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath) + "/";
    QDir d(pre);
    if(!d.exists())
        d.mkpath(pre);

    QFile file(m_modulePath);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return;

    for(int i = 0; i < list.size(); ++i) {
        std::pair<qint64, qint64> p = list.at(i);
        int bookID = m_set->v11n->bookIDs().at(i);
        QFile file2(pre + QString::number(bookID) + ".xml");
        myDebug() << file2.fileName();
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
    DEBUG_FUNC_NAME;
    myDebug() << id;
    if(m_xml != NULL) {
        delete m_xml;
        m_xml = NULL;
    }
    QString path;
    const QString cacheFile = m_settings->homePath + "cache/" + m_settings->hash(m_modulePath) + "/" + QString::number(id) + ".xml";
    QFileInfo info(cacheFile);

    if(info.exists()) {
        path = cacheFile;
    } else {
        path = m_modulePath;
    }
    myDebug() << path;
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
            while(m_xml->readNextStartElement()) {
                if(cmp(m_xml->name(), "INFORMATION")) {
                    ret = readMetaInfo();
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
    return m_moduleName;
}

Book ZefaniaBible::book() const
{
    return m_book;
}

bool ZefaniaBible::hasIndex() const
{
    DEBUG_FUNC_NAME
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

#ifdef OBV_USE_WSTRING
                                    doc.add(*new Field(_T("key"), key.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_NO));
                                    doc.add(*new Field(_T("content"), t.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));
#else
                                    doc.add(*new Field(_T("key"), reinterpret_cast<const wchar_t *>(key.utf16()), Field::STORE_YES |  Field::INDEX_NO));
                                    doc.add(*new Field(_T("content"), reinterpret_cast<const wchar_t *>(t.utf16()), Field::STORE_YES |  Field::INDEX_TOKENIZED));
#endif

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

#ifdef OBV_USE_WSTRING
    Query* q = QueryParser::parse(query.searchText.toStdWString().c_str(), _T("content"), &analyzer);
#else
    Query* q = QueryParser::parse(reinterpret_cast<const wchar_t *>(query.searchText.utf16()), _T("content"), &analyzer);
#endif

    Hits* h = s.search(q);
    for(size_t i = 0; i < h->length(); i++) {
        Document* doc = &h->doc(i);
#ifdef OBV_USE_WSTRING
        const QString stelle = QString::fromWCharArray(doc->get(_T("key")));
#else
        const QString stelle = QString::fromUtf16((const ushort*)doc->get(_T("key")));
#endif
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
#ifdef OBV_USE_WSTRING
            hit.setValue(SearchHit::VerseText, QString::fromWCharArray(doc->get(_T("content"))));
#else
            hit.setValue(SearchHit::VerseText, QString::fromUtf16((const ushort*)doc->get(_T("content"))));
#endif

            hit.setValue(SearchHit::Score, (double) h->score(i));

            res->addHit(hit);
        }
    }
    reader->close();
    delete reader;
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

Book ZefaniaBible::readBook()
{
    Book book(m_xml->attributes().value("bnumber").toString().toInt() - 1);
    while(m_xml->readNextStartElement()) {
        if(cmp(m_xml->name(), "CHAPTER")) {
            book.addChapter(readChapter());
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

Verse ZefaniaBible::readVerse()
{
    const int verseID = m_xml->attributes().value("vnumber").toString().toInt() - 1;
    QString out;
    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), "VERS")))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            out += m_xml->text();
        } else if(cmp(m_xml->name(), "STYLE") || m_xml->name() == "st") {
            out += pharseStyle();
        } else if(cmp(m_xml->name(), "NOTE")) {
            out += pharseNote();
        } else if(cmp(m_xml->name(), "BR")) {
            out += pharseBr();
        } else if(cmp(m_xml->name(), "DIV")) {
            out += pharseDiv();
        } else if(cmp(m_xml->name(), "GRAM") || m_xml->name() == "gr") {
            out += pharseGram();
        } else if(cmp(m_xml->name(), "SUP")) {
            out += pharseSup();
        } else if(cmp(m_xml->name(), "XREF")) {
            out += pharseXRef();
        } else {
            out += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    Verse verse(verseID, out);
    return verse;
}
QString ZefaniaBible::pharseStyle()
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
            ret += pharseStyle();
        } else if(cmp(m_xml->name(), "GRAM") || m_xml->name() == "gr") {
            ret += pharseGram();
        } else if(cmp(m_xml->name(), "SUP")) {
            ret += pharseSup();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return pre + ret + post;
}
QString ZefaniaBible::pharseNote()
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
            ret += pharseStyle();
        } else if(cmp(m_xml->name(), "BR")) {
            ret += pharseBr();
        } else if(cmp(m_xml->name(), "GRAM") || m_xml->name() == "gr") {
            ret += pharseGram();
        } else if(cmp(m_xml->name(), "XREF")) {
            ret += pharseXRef();
        } else if(cmp(m_xml->name(), "SUP")) {
            ret += pharseSup();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return pre + ret + post;
}
QString ZefaniaBible::pharseBr()
{
    const QStringRef art = m_xml->attributes().value("art");
    m_xml->skipCurrentElement();
    if(art == "x-p")
        return "<div class=\"pageBreak\"></div>";
    else if(art == "x-nl")
        return "<br />";
    return "";
}

QString ZefaniaBible::pharseGram()
{
    if(!m_set->displaySettings()->showStrong() && !m_set->displaySettings()->showRMac()) {
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
        } else if(cmp(m_xml->name(), "STYLE")  || m_xml->name() == "st") {
            ret += pharseStyle();
        } else if(cmp(m_xml->name(), "BR")) {
            ret += pharseBr();
        } else if(cmp(m_xml->name(), "GRAM") || m_xml->name() == "gr") {
            ret += pharseGram();
        } else if(cmp(m_xml->name(), "SUP")) {
            ret += pharseSup();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    if(!strong.isEmpty() && m_set->displaySettings()->showStrong()) {
        QString add;
        if(!strong.startsWith("G", Qt::CaseInsensitive) && !strong.startsWith("H", Qt::CaseInsensitive)) {
            //todo: that isn't  nice
            if(m_versification->bookCount() == 66) {
                if(m_bookID < 39) {
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
        }
        ret +=  "<span class=\"stronglink\"><a  href=\"strong://" + add + strong + "\">" + add + strong + "</a></span>";
    }
    if(!rmac.isEmpty() && m_set->displaySettings()->showRMac()) {
        ret +=  "<span class=\"rmaclink\"><a href=\"rmac://" + rmac + "\">" + rmac + "</a></span>";
    }
    return ret;
}

QString ZefaniaBible::pharseSup()
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
            ret += pharseStyle();
        } else if(cmp(m_xml->name(), "GRAM")) {
            ret += pharseGram();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return pre + ret + post;
}

QString ZefaniaBible::pharseXRef()
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
        const QStringList list = mscope.toString().split(";");
        const int bookID = list.at(0).toInt() - 1;
        const int chapterID = list.at(1).toInt() - 1;
        const int verseID = list.at(2).toInt() - 1;
        VerseUrlRange range;
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(bookID);
        range.setChapter(chapterID);
        range.setStartVerse(verseID);
        VerseUrl burl(range);
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
    return "";
}

QString ZefaniaBible::pharseDiv()
{
    QString ret;
    while(true) {
        m_xml->readNext();

        if(m_xml->tokenType() == QXmlStreamReader::EndElement && (cmp(m_xml->name(), "DIV")))
            break;

        if(m_xml->tokenType() == QXmlStreamReader::Characters) {
            ret += m_xml->text().toString();
        } else if(cmp(m_xml->name(), "NOTE")) {
            ret += pharseNote();
        } else {
            ret += m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        }
    }
    return ret;
}

MetaInfo ZefaniaBible::readMetaInfo()
{
    MetaInfo ret;
    while(m_xml->readNextStartElement()) {
        if(m_xml->name() == "publisher") {
            ret.publisher = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == "contributors") {
            ret.contributors = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == "date") {
            ret.date = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == "type") {
            ret.type = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == "format") {
            ret.format = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == "identifier") {
            ret.identifier = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == "source") {
            ret.source = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == "language") {
            ret.language = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == "coverage") {
            ret.coverage = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == "rights") {
            ret.rights = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == "subject") {
            ret.subject = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == "description") {
            ret.description = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == "creator") {
            ret.creator = m_xml->readElementText(QXmlStreamReader::IncludeChildElements);
        } else if(m_xml->name() == "title") {
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

void ZefaniaBible::clear()
{
    m_book.clear();
    if(m_xml != NULL) {
        delete m_xml;
        m_xml = NULL;
    }
}
