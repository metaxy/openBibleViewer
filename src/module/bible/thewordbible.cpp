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

using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;
extern bool removeDir(const QString &dirName);
TheWordBible::TheWordBible()
{
    m_versification = new Versification_KJV();
}
TheWordBible::~TheWordBible()
{
    DEBUG_FUNC_NAME
    delete m_versification;
    m_versification = 0;
}

void TheWordBible::setSettings(Settings *set)
{
    m_settings = set;
}
/**
  * Loads the Data.
  */
void TheWordBible::loadBibleData(const int &id, const QString &path)
{
    //DEBUG_FUNC_NAME
    m_moduleID = id;
    m_modulePath = path;
    QString dataFilePath = path;
    QFile file(dataFilePath);
    Versification::VersificationFilterFlags flags;
    if(path.endsWith(".nt")) {
        flags = Versification::ReturnNT;
    } else if(path.endsWith(".ot")) {
        flags = Versification::ReturnOT;
    } else if(path.endsWith(".ont")) {
        flags = Versification::ReturnAll;
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    int book = 0;
    int chapter = 0;
    int verse = 0;
    Book *currentBook = new Book();
    currentBook->setID(book);
    Chapter *currentChapter = new Chapter();
    currentChapter->setChapterID(chapter);
    const int linesToSkip = 31102;//see spec
    bool readingVerse = true;
    for(int lineCount = 0; !in.atEnd(); lineCount++) {
        const QString line = in.readLine();
        if(lineCount >= linesToSkip || line == "") {
            readingVerse = false;
        }
        if(readingVerse) {
            Verse v(verse, line);
            currentChapter->addVerse(verse, v);
            if(verse + 1 < m_versification->maxVerse(flags).value(book).at(chapter)) {
                verse++;
            } else {
                //myDebug() << " book = " << book << " maxChapter = " << m_versification->maxChapter(flags).at(book);
                if(chapter + 1 < m_versification->maxChapter(flags).value(book)) {
                    //myDebug() << "next chapter = " << chapter;
                    currentBook->addChapter(chapter, *currentChapter);
                    chapter++;
                    verse = 0;

                    currentChapter = new Chapter();
                    currentChapter->setChapterID(chapter);
                } else {
                    currentBook->addChapter(chapter, *currentChapter);
                    m_books.insert(book, *currentBook);

                    book++;
                    chapter = 0;
                    verse = 0;

                    currentBook = new Book();
                    currentBook->setID(book);

                    currentChapter = new Chapter();
                    currentChapter->setChapterID(chapter);
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

}
int TheWordBible::readBook(const int &id)
{
    m_currentBookID = id;
    return 0;
}

QString TheWordBible::readInfo(QFile &file)
{
    bool skipping = true;
    QTextStream in(&file);
    const int linesToSkip = 31102;//see spec
    for(int lineCount = 0; !in.atEnd(); lineCount++) {
        const QString line = in.readLine();
        if(skipping) {
            myDebug() << line;
            if(line.startsWith("title")) {
                const QStringList list = line.split("=");
                return list.last();
            }
        }
        if(line == "" || lineCount >= linesToSkip)
            skipping = false;

    }
    return "";
}
QString TheWordBible::readInfo(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return "";
    return readInfo(file);
}

void TheWordBible::search(const SearchQuery &query, SearchResult *res) const
{
    const QString index = indexPath();
    const TCHAR* stop_words[] = { NULL };
    standard::StandardAnalyzer analyzer(stop_words);
    IndexReader* reader = IndexReader::open(index.toStdString().c_str());
    IndexSearcher s(reader);
    Query* q = QueryParser::parse(query.searchText.toStdWString().c_str(), _T("content"), &analyzer);
    Hits* h = s.search(q);
    for(size_t i = 0; i < h->length(); i++) {
        Document* doc = &h->doc(i);
        const QString stelle = QString::fromWCharArray(doc->get(_T("key")));
        const QStringList l = stelle.split(";");
        if(query.range == SearchQuery::Whole || (query.range == SearchQuery::OT && l.at(0).toInt() <= 38) || (query.range == SearchQuery::NT && l.at(0).toInt() > 38)) {
            SearchHit hit;
            hit.setType(SearchHit::BibleHit);
            hit.setValue(SearchHit::BibleID, m_moduleID);
            hit.setValue(SearchHit::BookID, l.at(0).toInt());
            hit.setValue(SearchHit::ChapterID, l.at(1).toInt());
            hit.setValue(SearchHit::VerseID, l.at(2).toInt());
            hit.setValue(SearchHit::VerseText, QString::fromWCharArray(doc->get(_T("content"))));
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
    QHashIterator<int, Book> bookIt(m_books);
    for(int c = 0; bookIt.hasNext(); c++) {
        if(progress.wasCanceled()) {
            canceled = true;
            break;
        }
        progress.setValue(c);
        bookIt.next();
        QHashIterator<int, Chapter> chapterIt(bookIt.value().m_chapters);
        while(chapterIt.hasNext()) {
            chapterIt.next();
            QHashIterator<int, Verse> verseIt(chapterIt.value().getData());
            while(verseIt.hasNext()) {
                verseIt.next();
                doc.clear();
                const QString key = QString::number(bookIt.value().bookID()) + ";" + QString::number(chapterIt.value().chapterID()) + ";" + QString::number(verseIt.value().verseID());
                const QString text = verseIt.value().data();
                doc.add(*new Field(_T("key"), key.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_NO));
                doc.add(*new Field(_T("content"), text.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));
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

Book TheWordBible::book() const
{
    return m_books.value(m_currentBookID);
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
