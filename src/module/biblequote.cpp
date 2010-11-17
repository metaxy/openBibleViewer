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
#include "biblequote.h"
#include "src/core/dbghelper.h"
#include <QtCore/QString>
#include <QtCore/QFile>

#include <QtCore/QDir>
#include <QtGui/QProgressDialog>
#include <QtGui/QMessageBox>
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
    m_bibleID = -1;
    m_codec = NULL;
}
BibleQuote::~BibleQuote()
{

}
void BibleQuote::setSettings(Settings *set)
{
    m_settings = set;
}
QString BibleQuote::formatFromIni(QString input)
{
    input.replace(QString("\n"), QString(""));
    return input;
}
void BibleQuote::loadBibleData(const int &bibleID, const QString &path)
{
    m_bibleID = bibleID;
    m_bookFullName.clear();
    m_bookPath.clear();
    m_bookShortName.clear();
    m_bookCount.clear();
    m_bibleName = "";
    m_chapterSign = "";
    m_removeHtml = "";
    m_verseSign = "";
    m_chapterZero = false;
    int lastPos = path.lastIndexOf("/");
    QString path_ = path;
    m_biblePath = path_.remove(lastPos, path.size());
    bool started = false;
    bool started2 = false;
    int count = 0;

    QFile file;
    file.setFileName(path);
    QString encoding;
    if(m_settings->getModuleSettings(m_bibleID).encoding == "Default" || m_settings->getModuleSettings(m_bibleID).encoding == "") {
        encoding = m_settings->encoding;
    } else {
        encoding = m_settings->getModuleSettings(m_bibleID).encoding;
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
                m_bibleName = formatFromIni(line.remove(QRegExp("BibleName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            }
            if(line.contains("BibleShortName", Qt::CaseInsensitive)) {
                m_bibleShortName = formatFromIni(line.remove(QRegExp("BibleShortName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
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
                        m_bookCount[i] = formatFromIni(line.remove(QRegExp("ChapterQty(\\s*)=(\\s*)", Qt::CaseInsensitive))).toInt();
                        i++;
                        started2 = false;
                    } else if(line.contains("FullName", Qt::CaseInsensitive)) {
                        m_bookFullName << formatFromIni(line.remove(QRegExp("FullName(\\s*)=(\\s*)", Qt::CaseInsensitive)));

                    } else if(line.contains("ShortName", Qt::CaseInsensitive)) {
                        const QString s = formatFromIni(line.remove(QRegExp("ShortName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
                        QStringList l = s.split(" ");
                        m_bookShortName.append(l);

                    }
                } else if(line.contains("PathName", Qt::CaseInsensitive)) {
                    count++;
                    started2 = true;
                    m_bookPath << formatFromIni(line.remove(QRegExp("PathName(\\s*)=(\\s*)", Qt::CaseInsensitive)));

                }
            }

        }
    }
}
/**
  Reads the ini-file and returns the bible name. If the file is invalid is returns an empty QString.
  */
QString BibleQuote::readInfo(QFile &file)
{
    bool useShortName = false;
    m_bibleName.clear();
    m_bibleShortName.clear();
    int countlines = 0;
    if(m_codec == NULL) {
        QString encoding;
        if(m_settings->getModuleSettings(m_bibleID).encoding == "Default" || m_settings->getModuleSettings(m_bibleID).encoding == "") {
            encoding = m_settings->encoding;
        } else {
            encoding = m_settings->getModuleSettings(m_bibleID).encoding;
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
            m_bibleName = formatFromIni(line.remove(QRegExp("BibleName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            if(m_bibleName == "") {
                useShortName = true;
            }
            if(useShortName && m_bibleShortName != "") {
                break;
            } else if(!useShortName) {
                break;
            }
        }
        if(line.contains("BibleShortName", Qt::CaseInsensitive)) {
            m_bibleShortName = formatFromIni(line.remove(QRegExp("BibleShortName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            if(useShortName)
                break;
        }

    }
    file.close();
    if(useShortName) {
        m_bibleName = m_bibleShortName;
    }
    if(m_bibleName.isEmpty()) {
        myWarning() << "invalid ini File " << file.fileName();
    }
    return m_bibleName;
}
int BibleQuote::readBook(const int &id)
{
    m_book.clear();

    m_bookID = id;
    const QString path = m_biblePath + "/" + m_bookPath.at(id);
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
            if(m_settings->getModuleSettings(m_bibleID).biblequote_removeHtml == true && removeHtml2.size() > 0) {
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
                m_bookPath.replace(id, f.remove(m_biblePath + "/"));
                return readBook(id);
            }
        }
        return 2;//don't show a error
    }
    if(ccount2 == 0) {
        chapterText << out2;
        ccount2 = 1;
    }

    //todo: its slow
    for(int i = 1/* why 1?*/; i < chapterText.size(); i++) {
        Chapter c;
        QString a = chapterText.at(i);
        QStringList b = a.split(m_verseSign);
        for(int j = 0; j < b.size(); j++) { //split removes versesign but it is needed
            QString v = b.at(j);
            b.replace(j, v.prepend(m_verseSign));
        }
        c.data << b;
        m_book.addChapter(i, c);
    }
    file.close();
    return 0;

}
QString BibleQuote::indexPath() const
{
    return  m_settings->homePath + "index/" + m_settings->hash(m_biblePath);
}
bool BibleQuote::hasIndex() const
{
    DEBUG_FUNC_NAME
    QDir d;
    if(!d.exists(m_settings->homePath + "index")) {
        return false;
    }
    //todo: check versions

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
        if(m_settings->getModuleSettings(m_bibleID).encoding == "Default" || m_settings->getModuleSettings(m_bibleID).encoding == "") {
            encoding = m_settings->encoding;
        } else {
            encoding = m_settings->getModuleSettings(m_bibleID).encoding;
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
        const QString path = m_biblePath + "/" + m_bookPath.at(id);

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
                doc.add(*new Field(_T("key"), key.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_NO));
                doc.add(*new Field(_T("content"), t.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));

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
void BibleQuote::search(const SearchQuery &query, SearchResult *res)
{
    DEBUG_FUNC_NAME
    const QString index = indexPath();
    const TCHAR* stop_words[] = { NULL };
    standard::StandardAnalyzer analyzer(stop_words);
    myDebug() << index.toStdString().c_str() << QString::fromWCharArray(query.searchText.toStdWString().c_str());
    IndexReader* reader = IndexReader::open(index.toStdString().c_str());
    IndexSearcher s(reader);
    Query* q = QueryParser::parse(query.searchText.toStdWString().c_str(), _T("content"), &analyzer);
    Hits* h = s.search(q);
    for(size_t i = 0; i < h->length(); i++) {
        Document* doc = &h->doc(i);
        const QString stelle = QString::fromWCharArray(doc->get(_T("key")));
        // h->score(i)
        QStringList l = stelle.split(";");
        if(query.range == SearchQuery::Whole || (query.range == SearchQuery::OT && l.at(0).toInt() <= 38) || (query.range == SearchQuery::NT && l.at(0).toInt() > 38)) {
            SearchHit hit;
            hit.setType(SearchHit::BibleHit);
            hit.setValue(SearchHit::BibleID, m_bibleID);
            hit.setValue(SearchHit::BookID, l.at(0).toInt());
            hit.setValue(SearchHit::ChapterID, l.at(1).toInt());
            hit.setValue(SearchHit::VerseID, l.at(2).toInt());
            hit.setValue(SearchHit::VerseText, QString::fromWCharArray(doc->get(_T("content"))));
            res->addHit(hit);
        }
    }
    reader->close();
    delete reader;
}
int BibleQuote::bibleID() const
{
    return m_bibleID;
}
QString BibleQuote::biblePath() const
{
    return m_biblePath;
}
QString BibleQuote::bibleName(bool preferShortName) const
{
    return m_bibleName;
}
QMap<int, int> BibleQuote::bookCount()
{
    return m_bookCount;
}
BookNames BibleQuote::getBookNames()
{
    BookNames names;

    for(int c = 0; c < m_bookFullName.size(); ++c) {
        names.m_bookIDs.append(c);
        names.m_bookFullName[c] = m_bookFullName.at(c);
    }
    //todo: if m_bookFullName.size() != >m_bookShortName.size() should then i really add the shortnames or let them be empty
    for(int c = 0; c < m_bookShortName.size(); ++c) {
        names.m_bookShortName[c] = m_bookShortName.at(c);
    }
    return names;
}
Book BibleQuote::book() const
{
    return m_book;
}
