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
#include "thewordbible.h"
#include "config.h"
#include "CLucene.h"
#include "CLucene/clucene-config.h"
using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;
extern bool removeDir(const QString &dirName);
TheWordBible::TheWordBible()
{

}
TheWordBible::~TheWordBible()
{

}

void TheWordBible::setSettings(Settings *set)
{
    m_settings = set;
}
/**
  * Loads the Data.
  */
int TheWordBible::loadBibleData(const int id, const QString &path)
{
    //DEBUG_FUNC_NAME
    if(!m_books.isEmpty())
        m_books.clear();
    m_moduleID = id;
    m_modulePath = path;
    QString dataFilePath = path;
    QFile file(dataFilePath);
    Versification::VersificationFilterFlags flags;
    if(path.endsWith(".nt")) {
        m_settings->getModuleSettings(id)->versificationName = "kjv-nt";
        flags = Versification::ReturnNT;
    } else if(path.endsWith(".ot")) {
        m_settings->getModuleSettings(id)->versificationName = "kjv-ot";
        flags = Versification::ReturnOT;
    } else if(path.endsWith(".ont")) {
        m_settings->getModuleSettings(id)->versificationName = "kjv";
        flags = Versification::ReturnAll;
    }
    m_versification = QSharedPointer<Versification>(new Versification_KJV());
    m_versification->setFlags(flags);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 1;

    QTextStream in(&file);
    int book = 0;
    int chapter = 0;
    int verse = 0;
    Book currentBook = Book();
    currentBook.setID(book);
    Chapter currentChapter = Chapter();
    currentChapter.setChapterID(chapter);
    const int linesToSkip = 31102;//see spec
    bool readingVerse = true;
    ModuleDisplaySettings *displaySettings = m_settings->getModuleSettings(m_moduleID)->displaySettings().data();
    for(int lineCount = 0; !in.atEnd(); lineCount++) {
        QString line = in.readLine();
        if(lineCount >= linesToSkip || line.isEmpty()) {
            readingVerse = false;
        }
        if(readingVerse) {
            QRegExp strong("<W(G|H)(\\d+)(x|s)?>");
            QRegExp newLine("<CL>");
            if(displaySettings->showStrong())
                line.replace(strong, "<span class=\"stronglink\"><a href=\"strong://\\1\\2\">\\1\\2</a></span>");
            else
                line.replace(strong, "");

            line.replace(newLine, "<br />");
            Verse v(verse, line);
            currentChapter.addVerse(v);
            if(verse + 1 < m_versification->maxVerse(flags).value(book).at(chapter)) {
                verse++;
            } else {
                if(chapter + 1 < m_versification->maxChapter(flags).value(book)) {
                    currentBook.addChapter(currentChapter);
                    chapter++;
                    verse = 0;

                    currentChapter = Chapter();
                    currentChapter.setChapterID(chapter);
                } else {
                    currentBook.addChapter(currentChapter);
                    m_books.insert(currentBook.bookID(), currentBook);

                    book++;
                    chapter = 0;
                    verse = 0;

                    currentBook = Book(book);

                    currentChapter = Chapter(chapter);
                }
            }
        } else {
            //info
            if(line.startsWith("title")) {
                const QStringList list = line.split("=");
                m_moduleName = list.last();
            } else if(line.startsWith("short.title")) {
                const QStringList list = line.split("=");
                m_shortModuleName = list.last();
            } else if(line.startsWith("id")) {
                const QStringList list = line.split("=");
                m_uID = list.last();
            }
        }

    }
    if(m_uID.isEmpty()) {
        m_uID = path;
    }
    return 0;

}
int TheWordBible::readBook(const int id)
{
    m_bookID = id;
    return 0;
}

MetaInfo TheWordBible::readInfo(QFile &file)
{
    MetaInfo info;
    bool skipping = true;
    QTextStream in(&file);
    const int linesToSkip = 31102;//see spec
    for(int lineCount = 0; !in.atEnd(); lineCount++) {
        const QString line = in.readLine();
        if(skipping) {
            if(line.startsWith("title")) {
                const QStringList list = line.split("=");
                MetaInfo info;
                info.setName(list.last());
                return info;
            }
        }
        if(line.isEmpty() || lineCount >= linesToSkip)
            skipping = false;

    }
    return MetaInfo();
}
MetaInfo TheWordBible::readInfo(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return MetaInfo();
    return readInfo(file);
}

void TheWordBible::search(const SearchQuery &query, SearchResult *res) const
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
bool TheWordBible::hasIndex() const
{
    const QString index = indexPath();
    QDir d;
    if(!d.exists(index)) {
        return false;
    }
    return IndexReader::indexExists(index.toStdString().c_str());
}
void TheWordBible::buildIndex()
{
    QProgressDialog progress(QObject::tr("Build index"), QObject::tr("Cancel"), 0, m_books.size());
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
    QMapIterator<int, Book> bookIt(m_books);
    for(int c = 0; bookIt.hasNext(); c++) {
        if(progress.wasCanceled()) {
            canceled = true;
            break;
        }
        progress.setValue(c);
        bookIt.next();
        QMapIterator<int, Chapter> chapterIt(bookIt.value().data());
        while(chapterIt.hasNext()) {
            chapterIt.next();
            QMapIterator<int, Verse> verseIt(chapterIt.value().data());
            while(verseIt.hasNext()) {
                verseIt.next();
                doc.clear();
                const QString key = QString::number(bookIt.value().bookID()) + ";" + QString::number(chapterIt.value().chapterID()) + ";" + QString::number(verseIt.value().verseID());
                const QString text = verseIt.value().data();
#ifdef OBV_USE_WSTRING
                doc.add(*new Field(_T("key"), key.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_NO));
                doc.add(*new Field(_T("content"), text.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));
#else
                doc.add(*new Field(_T("key"), reinterpret_cast<const wchar_t *>(key.utf16()), Field::STORE_YES |  Field::INDEX_NO));
                doc.add(*new Field(_T("content"), reinterpret_cast<const wchar_t *>(text.utf16()), Field::STORE_YES |  Field::INDEX_TOKENIZED));
#endif
                writer->addDocument(&doc);

            }
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
}

int TheWordBible::moduleID() const
{
    return m_moduleID;
}

QString TheWordBible::modulePath() const
{
    return m_modulePath;
}
QString TheWordBible::moduleName(bool preferShortName) const
{
    if(preferShortName) {
        if(!m_shortModuleName.isEmpty()) {
            return m_shortModuleName;
        } else {
            return m_moduleName;
        }
    } else {
        if(!m_moduleName.isEmpty()) {
            return m_moduleName;
        } else {
            return m_shortModuleName;
        }
    }
}

bool TheWordBible::hasNT() const
{
    QFileInfo f(m_modulePath + ".nt");
    return f.exists();
}
bool TheWordBible::hasOT() const
{
    QFileInfo f(m_modulePath + ".ot");
    return f.exists();
}
bool TheWordBible::hasONT() const
{
    QFileInfo f(m_modulePath + ".ont");
    return f.exists();
}
/**
  * Returns the path, where all indexed files are stored.
  */
QString TheWordBible::indexPath() const
{
    return m_settings->homePath + "index/" + m_settings->hash(m_modulePath);
}
QString TheWordBible::uid() const
{
    return m_uID;
}
TextRange TheWordBible::rawTextRange(int bookID, int chapterID, int startVerse, int endVerse)
{
    TextRange ret;
    Book book = m_books.value(bookID);

    if(book.bookID() != bookID) {
        readBook(bookID);
        myDebug() << "book not loaded";

    }
    if(!book.hasChapter(chapterID)) {
        myWarning() << "index out of range index chapterID = " << chapterID;
        return ret;
    }
    ret.setModuleID(m_moduleID);
    ret.setBookID(bookID);
    ret.setChapterID(chapterID);

    const Chapter c = book.getChapter(chapterID);
    QMap<int, Verse> data = c.data();
    QMapIterator<int, Verse> i(data);
    while(i.hasNext()) {
        i.next();
        if(i.key() <= endVerse && i.key() >= startVerse)
            ret.addVerse(i.value());
    }
    return ret;
}

std::pair<int, int> TheWordBible::minMaxVerse(int bookID, int chapterID)
{
    std::pair<int, int> ret;
    Book book = m_books.value(bookID);

    if(book.bookID() != bookID) {
        readBook(bookID);
        myDebug() << "book not loaded";
    }
    if(!book.hasChapter(chapterID)) {
        myWarning() << "index out of range index chapterID = " << chapterID;
        return ret;
    }

    const Chapter c = book.getChapter(chapterID);
    //because c.data() is a map and it is sorted by key
    ret.first = c.data().keys().first();
    ret.second = c.data().keys().last();

    return ret;
}
