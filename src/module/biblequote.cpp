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
#include <QtCore/QTextCodec>
#include <QtCore/QDir>
#include <QtGui/QProgressDialog>
#include <QtGui/QMessageBox>
#include <CLucene.h>
#include <CLucene/util/Misc.h>
#include <CLucene/util/Reader.h>

//Maximum index entry size, 1MiB for now
//Lucene default is too small
const unsigned long BT_MAX_LUCENE_FIELD_LENGTH = 1024 * 1024;
#ifndef Q_OS_WIN32
using namespace lucene::search;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::analysis::standard;
#endif
BibleQuote::BibleQuote()
{
    m_bibleID = -1;
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
void BibleQuote::loadBibleData(int bibleID, QString path)
{
    //DEBUG_FUNC_NAME
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
    //myDebug() << "id = " << m_bibleID << " fileName = " << file.fileName() << " currentBiblePath = " << m_biblePath;
    QString encoding;
    if(m_settings->getModuleSettings(m_bibleID).encoding == "Default" || m_settings->getModuleSettings(m_bibleID).encoding == "") {
        encoding = m_settings->encoding;
    } else {
        encoding = m_settings->getModuleSettings(m_bibleID).encoding;
    }
    QTextCodec *codec = QTextCodec::codecForName(encoding.toStdString().c_str());
    QTextDecoder *decoder = codec->makeDecoder();
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        int i = 0;
        while(!file.atEnd()) {
            const QByteArray byteline = file.readLine();
            QString line = decoder->toUnicode(byteline);

            if(line.startsWith("//")) {//it is a comment
                continue;
            }

            if(line.contains("BibleName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                m_bibleName = formatFromIni(line.remove(QRegExp("BibleName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            }
            if(line.contains("BibleShortName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                m_bibleShortName = formatFromIni(line.remove(QRegExp("BibleShortName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            }
            if(line.contains("ChapterSign", Qt::CaseInsensitive) && !line.startsWith("//")) {
                m_chapterSign = formatFromIni(line.remove(QRegExp("ChapterSign(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            }
            if(line.contains("HTMLFilter", Qt::CaseInsensitive) && !line.startsWith("//")) {
                m_removeHtml = formatFromIni(line.remove(QRegExp("HTMLFilter(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            }
            if(line.contains("VerseSign", Qt::CaseInsensitive) && !line.startsWith("//")) {
                m_verseSign = formatFromIni(line.remove(QRegExp("VerseSign(\\s*)=(\\s*)", Qt::CaseInsensitive)));
            }
            if(line.contains("ChapterZero", Qt::CaseInsensitive) && !line.startsWith("//")) {
                const QString zero = formatFromIni(line.remove(QRegExp("ChapterZero(\\s*)=(\\s*)", Qt::CaseInsensitive)));
                if(zero.compare("Y", Qt::CaseInsensitive) == 0) {
                    m_chapterZero = true;
                } else {
                    m_chapterZero = false;
                }
            }
            if(started == false && line.contains("BookQty", Qt::CaseInsensitive) && !line.startsWith("//")) {
                started = true;
            }
            if(started == true) {
                if(started2 == true) {
                    if(line.contains("ChapterQty", Qt::CaseInsensitive) && !line.startsWith("//")) {
                        m_bookCount[i] = formatFromIni(line.remove(QRegExp("ChapterQty(\\s*)=(\\s*)", Qt::CaseInsensitive))).toInt();
                        i++;
                        //chapterqty auslesen
                        started2 = false;
                    } else if(line.contains("FullName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                        m_bookFullName << formatFromIni(line.remove(QRegExp("FullName(\\s*)=(\\s*)", Qt::CaseInsensitive)));

                    } else if(line.contains("ShortName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                        const QString s = formatFromIni(line.remove(QRegExp("ShortName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
                        QStringList l = s.split(" ");
                        m_bookShortName.append(l);

                    }
                } else if(line.contains("PathName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                    count++;
                    started2 = true;
                    m_bookPath << formatFromIni(line.remove(QRegExp("PathName(\\s*)=(\\s*)", Qt::CaseInsensitive)));
                    //pathname auslesen
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
    QString encoding;
    if(m_settings->getModuleSettings(m_bibleID).encoding == "Default" || m_settings->getModuleSettings(m_bibleID).encoding == "") {
        encoding = m_settings->encoding;
    } else {
        encoding = m_settings->getModuleSettings(m_bibleID).encoding;
    }
    if(encoding == "")
        encoding = m_settings->encoding;
    QTextCodec *codec = QTextCodec::codecForName(encoding.toStdString().c_str());
    QTextDecoder *decoder = codec->makeDecoder();
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
int BibleQuote::readBook(int id, QString path)
{
    m_book.clear();

    m_bookID = id;
    path = m_biblePath + "/" + path;

    QFile file;
    file.setFileName(path);

    QString out();
    QString out2();
    bool chapterstarted = false;
    int ccount2 = 0;
    QStringList chapterText;
    const QStringList removeHtml2 = m_removeHtml.split(" ");

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString encoding;
        if(m_settings->getModuleSettings(m_bibleID).encoding == "Default" || m_settings->getModuleSettings(m_bibleID).encoding == "") {
            encoding = m_settings->encoding;
        } else {
            encoding = m_settings->getModuleSettings(m_bibleID).encoding;
        }
        myDebug() << "encoding = " << encoding;
        QTextCodec *codec = QTextCodec::codecForName(encoding.toStdString().c_str());
        QTextDecoder *decoder = codec->makeDecoder();
        while(!file.atEnd()) {
            const QByteArray byteline = file.readLine();
            QString line = decoder->toUnicode(byteline);

            //filterout
            if(m_settings->getModuleSettings(m_bibleID).biblequote_removeHtml == true && removeHtml2.size() > 0) {
                foreach(QString r, removeHtml2) {
                     line = line.remove(r, Qt::CaseInsensitive);
                }

                /*line = line.remove(QRegExp("CLASS=\"(\\w+)\">"));
                line = line.remove(QRegExp("<DIV CLASS=\"(\\w+)\">"));
                line = line.remove("CLASS=\"Tx\">");*/
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
                return readBook(id, f);
            }
        }
        return 2;//don't show a error
    }
    if(ccount2 == 0) {
        chapterText << out2;
        ccount2 = 1;
    }

    for(int i = 1/* why 1?*/; i < chapterText.size(); i++) {
        Chapter c;
        QString a = chapterText.at(i);
        QStringList b = a.split(m_verseSign);
        for(int j = 0; j < b.size(); j++) { //split removes versesign but it is needed
            const QString v = b.at(j);
            b.replace(v, a.prepend(m_verseSign));
        }
        c.data << b;
        m_book.addChapter(i, c);
    }
    file.close();
    return 0;

}
bool BibleQuote::hasIndex()
{
    DEBUG_FUNC_NAME
    QDir d;
    if(!d.exists(m_settings->homePath + "index")) {
        return false;
    }
    //todo: check versions
    QString index = m_settings->homePath + "index/" + m_settings->hash(m_biblePath);

    return  IndexReader::indexExists(index.toAscii().constData());
}

void BibleQuote::buildIndex()
{
    DEBUG_FUNC_NAME

    QString index = m_settings->homePath + "index/" + m_settings->hash(m_biblePath);
    QDir dir("/");
    dir.mkpath(index);

    const TCHAR* stop_words[]  = { NULL };
    StandardAnalyzer an((const TCHAR**)stop_words);

    if(IndexReader::indexExists(index.toAscii().constData())) {
        if(IndexReader::isLocked(index.toAscii().constData())) {
            IndexReader::unlock(index.toAscii().constData());
        }
    }
    QScopedPointer< IndexWriter> writer(new  IndexWriter(index.toAscii().constData(), &an, true));   //always create a new index

    QStringList ctext;
    QList<QByteArray> bytetext;
    QProgressDialog progress(QObject::tr("Indexing"), QObject::tr("Cancel"), 0, m_bookPath.size());
    progress.setWindowModality(Qt::WindowModal);

    QString encoding;
    if(m_settings->getModuleSettings(m_bibleID).encoding == "Default" || m_settings->getModuleSettings(m_bibleID).encoding == "") {
        encoding = m_settings->encoding;
    } else {
        encoding = m_settings->getModuleSettings(m_bibleID).encoding;
    }
    QTextCodec *codec  = QTextCodec::codecForName(encoding.toStdString().c_str());
    QScopedPointer<QTextDecoder> decoder(codec->makeDecoder());
    QByteArray textBuffer;


    for(int id = 0; id < m_bookPath.size(); id++) {
        progress.setValue(id);
        bytetext.clear();
        ctext.clear();
        const QString path = m_bookPath.at(id);
        path = m_biblePath + "/" + path;
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

        wchar_t wcharBuffer[BT_MAX_LUCENE_FIELD_LENGTH + 1];
        for(int chapterit = 0; chapterit < ctext.size(); ++chapterit) {
            QStringList verses = ctext[chapterit].split(m_verseSign);
            for(int verseit = 0; verseit < verses.size(); ++verseit) {
                QString t = verses.at(verseit);
                QScopedPointer< Document> doc(new  Document());
                QString key = QString::number(id) + ";" + QString::number(chapterit - 1) + ";" + QString::number(verseit - 1);
                lucene_utf8towcs(wcharBuffer, key.toLocal8Bit().constData(), BT_MAX_LUCENE_FIELD_LENGTH);

                doc->add(*(new  Field((const TCHAR*)_T("key"),
                                      (const TCHAR*)wcharBuffer,
                                      Field::STORE_YES |  Field::INDEX_NO)));

                lucene_utf8towcs(wcharBuffer, (const char*) textBuffer.append(t), BT_MAX_LUCENE_FIELD_LENGTH);

                doc->add(*(new  Field((const TCHAR*)_T("content"),
                                      (const TCHAR*)wcharBuffer,
                                      Field::STORE_YES |  Field::INDEX_TOKENIZED)));
                textBuffer.resize(0); //clean up
                writer->addDocument(doc.data());
            }
        }
    }
    writer->optimize();
    writer->close();
    progress.close();
}
void BibleQuote::search(SearchQuery query, SearchResult *res)
{
    QString index = m_settings->homePath + "index/" + m_settings->hash(m_biblePath);
    char utfBuffer[BT_MAX_LUCENE_FIELD_LENGTH  + 1];
    wchar_t wcharBuffer[BT_MAX_LUCENE_FIELD_LENGTH + 1];


    const TCHAR* stop_words[]  = { NULL };
    StandardAnalyzer analyzer(stop_words);

    IndexSearcher searcher(index.toLocal8Bit().constData());
    lucene_utf8towcs(wcharBuffer, query.searchText.toUtf8().constData(), BT_MAX_LUCENE_FIELD_LENGTH);
    QScopedPointer< Query> q(QueryParser::parse((const TCHAR*)wcharBuffer, (const TCHAR*)_T("content"), &analyzer));

    QScopedPointer< Hits> h(searcher.search(q.data(),  Sort::INDEXORDER));

    Document* doc = 0;
    for(int i = 0; i < h->length(); ++i) {
        doc = &h->doc(i);
        lucene_wcstoutf8(utfBuffer, (const wchar_t*)doc->get((const TCHAR*)_T("key")), BT_MAX_LUCENE_FIELD_LENGTH);
        QString stelle(utfBuffer);
        QStringList l = stelle.split(";");
        if(query.range == SearchQuery::Whole ||
                (query.range == SearchQuery::OT && l.at(0).toInt() <= 38) ||
                (query.range == SearchQuery::NT && l.at(0).toInt() > 38)) {
            lucene_wcstoutf8(utfBuffer, (const wchar_t*)doc->get((const TCHAR*)_T("content")), BT_MAX_LUCENE_FIELD_LENGTH);

            const QString content = QString::fromUtf8(utfBuffer);
            SearchHit hit;
            hit.setType(SearchHit::BibleHit);
            hit.setValue(SearchHit::BibleID, m_bibleID);
            hit.setValue(SearchHit::BookID, l.at(0).toInt());
            hit.setValue(SearchHit::ChapterID, l.at(1).toInt());
            hit.setValue(SearchHit::VerseID, l.at(2).toInt());
            hit.setValue(SearchHit::VerseText, content);
            res->addHit(hit);
        }

    }

}
