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
#include "biblequote.h"
#include "config.h"
#include "CLucene.h"
#include "CLucene/_clucene-config.h"
using namespace lucene::store;
using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;

BibleQuote::BibleQuote()
{
    m_moduleID = -1;
    m_codec = NULL;
    m_versification = NULL;
}
BibleQuote::~BibleQuote()
{
    if(m_versification != NULL) {
        delete m_versification;
        m_versification = NULL;
    }

}
void BibleQuote::setSettings(Settings *set)
{
    m_settings = set;
}
QString BibleQuote::formatFromIni(QString input)
{
    return input.trimmed();
}
void BibleQuote::loadBibleData(const int &bibleID, const QString &path)
{
    /*if(m_versification != NULL)
        delete m_versification;
    m_versification = NULL;*/

    QStringList bookFullName;
    QList<QStringList> bookShortName;
    QMap<int, int> bookCount;

    m_moduleID = bibleID;
    m_bookPath.clear();
    m_moduleName = "";
    m_chapterSign = "";
    m_removeHtml = "";
    m_verseSign = "";
    m_chapterZero = false;

    m_uid = path;

    int lastPos = path.lastIndexOf("/");
    QString path_ = path;
    m_modulePath = path_.remove(lastPos, path.size());
    bool started = false;
    bool started2 = false;
    int count = 0;

    QFile file;
    file.setFileName(path);
    QString encoding;
    if(m_settings->getModuleSettings(m_moduleID)->encoding == "Default" || m_settings->getModuleSettings(m_moduleID)->encoding.isEmpty()) {
        encoding = m_settings->encoding;
    } else {
        encoding = m_settings->getModuleSettings(m_moduleID)->encoding;
    }
    m_codec = QTextCodec::codecForName(encoding.toStdString().c_str());
    QTextDecoder *decoder = m_codec->makeDecoder();
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        int i = 0;
        while(!file.atEnd()) {
            const QByteArray byteline = file.readLine();
            QString line = decoder->toUnicode(byteline);

            if(line.startsWith("//")) {//it is a comment
                continue;
            }

            if(line.contains("BibleName", Qt::CaseInsensitive)) {
                m_moduleName = formatFromIni(line.remove(QRegExp("BibleName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            }
            if(line.contains("BibleShortName", Qt::CaseInsensitive)) {
                m_moduleShortName = formatFromIni(line.remove(QRegExp("BibleShortName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            }
            if(line.contains("ChapterSign", Qt::CaseInsensitive)) {
                m_chapterSign = formatFromIni(line.remove(QRegExp("ChapterSign(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            }
            if(line.contains("HTMLFilter", Qt::CaseInsensitive)) {
                m_removeHtml = formatFromIni(line.remove(QRegExp("HTMLFilter(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            }
            if(line.contains("VerseSign", Qt::CaseInsensitive)) {
                m_verseSign = formatFromIni(line.remove(QRegExp("VerseSign(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            }
            if(line.contains("ChapterZero", Qt::CaseInsensitive)) {
                const QString zero = formatFromIni(line.remove(QRegExp("ChapterZero(\\s*)=(\\s*)", Qt::CaseInsensitive)));
                if(zero.compare("Y", Qt::CaseInsensitive) == 0) {
                    m_chapterZero = true;
                } else {
                    m_chapterZero = false;
                }
            }

            if(started == false && line.contains("BookQty", Qt::CaseInsensitive)) {
                started = true;
            }
            if(started == true) {
                if(started2 == true) {
                    if(line.contains("ChapterQty", Qt::CaseInsensitive)) {
                        bookCount[i] = formatFromIni(line.remove(QRegExp("ChapterQty(\\s*)=(\\s*)", Qt::CaseInsensitive))).toInt();
                        i++;
                        started2 = false;
                    } else if(line.contains("FullName", Qt::CaseInsensitive)) {
                        bookFullName << formatFromIni(line.remove(QRegExp("FullName(\\s*)=(\\s*)", Qt::CaseInsensitive)));

                    } else if(line.contains("ShortName", Qt::CaseInsensitive)) {
                        const QString s = formatFromIni(line.remove(QRegExp("ShortName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
                        QStringList l = s.split(" ");
                        bookShortName.append(l);

                    }
                } else if(line.contains("PathName", Qt::CaseInsensitive)) {
                    count++;
                    started2 = true;
                    m_bookPath << formatFromIni(line.remove(QRegExp("PathName(\\s*)=(\\s*)", Qt::CaseInsensitive)));

                }
            }

        }
    }
    ModuleSettings *mset = m_settings->getModuleSettings(m_moduleID);
    mset->loadVersification();
    if(mset->v11n == NULL) {
        mset->v11n = new Versification_BibleQuote(bookFullName, bookShortName, bookCount);
        mset->versificationName = "";
        mset->versificationFile = m_settings->v11nFile(path);
    }
    m_versification = mset->v11n;
}
/**
  Reads the ini-file and returns the bible name. If the file is invalid is returns an empty QString.
  */
QString BibleQuote::readInfo(QFile &file)
{
    //todo: do not use every time getModuleSettings
    bool useShortName = false;
    m_moduleName.clear();
    m_moduleShortName.clear();
    int countlines = 0;

    if(m_codec == NULL) {
        QString encoding;
        if(m_settings->getModuleSettings(m_moduleID) == NULL) {
            encoding = m_settings->encoding;
        } else {
            if(m_settings->getModuleSettings(m_moduleID)->encoding == "Default" || m_settings->getModuleSettings(m_moduleID)->encoding.isEmpty()) {
                encoding = m_settings->encoding;
            } else {
                encoding = m_settings->getModuleSettings(m_moduleID)->encoding;
            }
        }
        m_codec = QTextCodec::codecForName(encoding.toStdString().c_str());
    }

    QTextDecoder *decoder = m_codec->makeDecoder();
    while(!file.atEnd()) {
        /*if (countlines > 50) { //wenn eine ini datei ungueltig ist soll damit nicht zuviel zeit verguedet werden
            break;
        }*/
        QByteArray byteline = file.readLine();
        QString line = decoder->toUnicode(byteline);
        if(!line.startsWith("//")) {
            countlines++;
        } else {
            continue;
        }
        if(line.contains("BibleName", Qt::CaseInsensitive)) {
            m_moduleName = formatFromIni(line.remove(QRegExp("BibleName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            if(m_moduleName.isEmpty()) {
                useShortName = true;
            }
            if(useShortName && !m_moduleShortName.isEmpty()) {
                break;
            } else if(!useShortName) {
                break;
            }
        }
        if(line.contains("BibleShortName", Qt::CaseInsensitive)) {
            m_moduleShortName = formatFromIni(line.remove(QRegExp("BibleShortName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            if(useShortName)
                break;
        }

    }
    file.close();
    if(useShortName) {
        m_moduleName = m_moduleShortName;
    }
    if(m_moduleName.isEmpty()) {
        myWarning() << "invalid ini File " << file.fileName();
    }
    return m_moduleName;
}
QString BibleQuote::readInfo(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return "";
    return readInfo(file);
}
int BibleQuote::readBook(const int &id)
{
    m_book.clear();
    m_book.setID(id);
    const QString path = m_modulePath + "/" + m_bookPath.at(id);
    QFile file;
    file.setFileName(path);

    QString out;
    QString out2;
    bool chapterstarted = false;
    int ccount2 = 0;
    QStringList chapterText;
    const QStringList removeHtml2 = m_removeHtml.split(" ");

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextDecoder *decoder = m_codec->makeDecoder();
        while(!file.atEnd()) {
            const QByteArray byteline = file.readLine();
            QString line = decoder->toUnicode(byteline);

            //filterout
            if(m_settings->getModuleSettings(m_moduleID)->biblequote_removeHtml == true && removeHtml2.size() > 0) {
                foreach(const QString & r, removeHtml2) {
                    line = line.remove(r, Qt::CaseInsensitive);
                }

            }
            out2 += line;
            if(chapterstarted == false && line.contains(m_chapterSign)) {
                chapterstarted = true;
            }
            if(chapterstarted == true && line.contains(m_chapterSign)) {
                ccount2++;
                chapterText << out;
                out = line;
            } else if(chapterstarted == true) {
                out += line;
            }
        }
        chapterText << out;
    } else {
        //becauce windows filename are case insensensitive
        //there are some filename typos in the ini files
        //and you cannot open this files on linux
        QFileInfo info(file.fileName());
        QDir d(info.absoluteDir());
        QStringList list = d.entryList();

        foreach(QString f, list) {
            QFileInfo info2(f);
            if(info2.baseName().compare(info.baseName(), Qt::CaseInsensitive) == 0) {
                m_bookPath.replace(id, f.remove(m_modulePath + "/"));
                return readBook(id);
            }
        }
        return 1;
    }
    if(ccount2 == 0) {
        chapterText << out2;
        ccount2 = 1;
    }

    //todo: its slow
    for(int i = 0; i < chapterText.size() - 1; i++) {
        Chapter c;
        c.setChapterID(i);
        QStringList rawVerseList = chapterText.at(i + 1).split(m_verseSign);
        for(int j = 0; j < rawVerseList.size(); j++) { //split removes versesign but it is needed
            QString verseText = rawVerseList.at(j);
            Verse v(j, verseText.prepend(m_verseSign));
            c.addVerse(j, v);
        }
        m_book.addChapter(i, c);
    }
    file.close();
    return 0;

}
QString BibleQuote::indexPath() const
{
    return m_settings->homePath + "index/" + m_settings->hash(m_modulePath);
}
bool BibleQuote::hasIndex() const
{
    DEBUG_FUNC_NAME
    QDir d;
    if(!d.exists(m_settings->homePath + "index")) {
        return false;
    }
    return IndexReader::indexExists(indexPath().toStdString().c_str());
}

void BibleQuote::buildIndex()
{
    DEBUG_FUNC_NAME
    const QString index = indexPath();
    QDir dir("/");
    dir.mkpath(index);


    if(m_codec == NULL) {
        QString encoding;
        if(m_settings->getModuleSettings(m_moduleID)->encoding == "Default" || m_settings->getModuleSettings(m_moduleID)->encoding.isEmpty()) {
            encoding = m_settings->encoding;
        } else {
            encoding = m_settings->getModuleSettings(m_moduleID)->encoding;
        }
        m_codec = QTextCodec::codecForName(encoding.toStdString().c_str());
    }

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

    QStringList ctext;
    QList<QByteArray> bytetext;
    QProgressDialog progress(QObject::tr("Indexing"), QObject::tr("Cancel"), 0, m_bookPath.size());
    progress.setWindowModality(Qt::WindowModal);

    QScopedPointer<QTextDecoder> decoder(m_codec->makeDecoder());

    for(int id = 0; id < m_bookPath.size(); id++) {
        progress.setValue(id);
        bytetext.clear();
        ctext.clear();
        const QString path = m_modulePath + "/" + m_bookPath.at(id);

        QFile file(path);
        QByteArray out;
        QByteArray out2;
        bool chapterstarted = false;
        int ccount2 = 0;
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            while(!file.atEnd()) {
                QByteArray byteline = file.readLine();
                QString line(byteline);
                out2 += byteline;
                if(chapterstarted == false && line.contains(m_chapterSign)) {
                    chapterstarted = true;
                }
                if(chapterstarted == true && line.contains(m_chapterSign)) {
                    ccount2++;
                    bytetext << out;
                    out = byteline;
                } else if(chapterstarted == true) {
                    out += byteline;
                }
            }
            bytetext << out;
        } else {
            myWarning() << "cannot open the file " << file.fileName();
            continue;
        }

        if(ccount2 == 0) {
            ctext << decoder->toUnicode(out2);
            ccount2 = 1;
        } else {
            for(int i = 0; i < bytetext.size(); i++) {
                ctext << decoder->toUnicode(bytetext.at(i));
            }
        }

        for(int chapterit = 0; chapterit < ctext.size(); ++chapterit) {
            const QStringList verses = ctext[chapterit].split(m_verseSign);
            for(int verseit = 0; verseit < verses.size(); ++verseit) {
                doc.clear();
                const QString t = verses.at(verseit);
                const QString key = QString::number(id) + ";" + QString::number(chapterit - 1) + ";" + QString::number(verseit - 1);
#ifdef _USE_WSTRING
                doc.add(*new Field(_T("key"), key.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_NO));
                doc.add(*new Field(_T("content"), t.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));
#else
                doc.add(*new Field(_T("key"), reinterpret_cast<const wchar_t *>(key.utf16()), Field::STORE_YES |  Field::INDEX_NO));
                doc.add(*new Field(_T("content"),reinterpret_cast<const wchar_t *>(t.utf16()), Field::STORE_YES |  Field::INDEX_TOKENIZED));
#endif


                writer->addDocument(&doc);
            }
        }
    }
    writer->setUseCompoundFile(true);
    writer->optimize();

    writer->close();
    delete writer;
    progress.close();

}
void BibleQuote::search(const SearchQuery &query, SearchResult *res) const
{
    DEBUG_FUNC_NAME
    const QString index = indexPath();
    const TCHAR* stop_words[] = { NULL };
    standard::StandardAnalyzer analyzer(stop_words);

    IndexReader* reader = IndexReader::open(index.toStdString().c_str());
    IndexSearcher s(reader);
#ifdef _USE_WSTRING
    Query* q = QueryParser::parse(query.searchText.toStdWString().c_str(), _T("content"), &analyzer);
#else
    Query* q = QueryParser::parse(reinterpret_cast<const wchar_t *>(query.searchText.utf16()), _T("content"), &analyzer);
#endif
    Hits* h = s.search(q);
    for(size_t i = 0; i < h->length(); i++) {
        Document* doc = &h->doc(i);
#ifdef _USE_WSTRING
        const QString stelle = QString::fromWCharArray(doc->get(_T("key")));
#else
         const QString stelle = QString::fromUtf16((const ushort*)doc->get(_T("key")));
#endif
        // h->score(i)
        QStringList l = stelle.split(";");
        if(query.range == SearchQuery::Whole || (query.range == SearchQuery::OT && l.at(0).toInt() <= 38) || (query.range == SearchQuery::NT && l.at(0).toInt() > 38)) {
            SearchHit hit;
            hit.setType(SearchHit::BibleHit);
            hit.setValue(SearchHit::BibleID, m_moduleID);
            hit.setValue(SearchHit::BookID, l.at(0).toInt());
            hit.setValue(SearchHit::ChapterID, l.at(1).toInt());
            hit.setValue(SearchHit::VerseID, l.at(2).toInt());
#ifdef _USE_WSTRING
             hit.setValue(SearchHit::VerseText, QString::fromWCharArray(doc->get(_T("key"))));
#else
             hit.setValue(SearchHit::VerseText, QString::fromUtf16((const ushort*)doc->get(_T("key"))));
#endif
            res->addHit(hit);
        }
    }
    reader->close();
    delete reader;
}
int BibleQuote::moduleID() const
{
    return m_moduleID;
}
QString BibleQuote::modulePath() const
{
    return m_modulePath;
}
QString BibleQuote::moduleName(bool preferShortName) const
{
    return m_moduleName;
}
QString BibleQuote::uid() const
{
    return m_uid;
}
TextRange BibleQuote::rawTextRange(int bookID, int chapterID, int startVerse, int endVerse)
{
    TextRange ret;
    if(m_book.bookID() != bookID) {
        readBook(bookID);
        myDebug() << "book not loaded";
    }
    if(!m_book.hasChapter(chapterID)) {
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

std::pair<int, int> BibleQuote::minMaxVerse(int bookID, int chapterID)
{
    std::pair<int, int> ret;
    if(m_book.bookID() != bookID) {
        readBook(bookID);
        myDebug() << "book not loaded";
    }
    if(!m_book.hasChapter(chapterID)) {
        myWarning() << "index out of range index chapterID = " << chapterID;
        return ret;
    }

    const Chapter c = m_book.getChapter(chapterID);
    QMap<int, Verse> data = c.data();
    int max = 0;
    int min = 0;

    foreach(int key, data.keys()) {
        if(key >= max) {
            max = key;
        } else if(key <= min) {
            min = key;
        }
    }
    ret.first = min;
    ret.second = max;

    return ret;
}
