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
#include <QtCore/QFile>
#include <QtCore/QRegExp>
#include <QtCore/QTimer>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>
#include <QtXml/QDomAttr>
#include <QtXml/QDomElement>
#include <QtXml/QtXml>
#include "src/module/zefania-bible.h"
#include "src/core/KoXmlWriter.h"
#include "src/core/dbghelper.h"
#include "src/core/bibleurl.h"
#include "config.h"
#include <QDir>

#ifdef _CLUCENE_LEGACY
const unsigned long MAX_LUCENE_FIELD_LENGTH = 1024 * 124;
#include <CLucene.h>
#include <CLucene/util/Misc.h>
#include <CLucene/util/Reader.h>

#ifndef Q_OS_WIN32
using namespace lucene::search;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::analysis::standard;
#endif

#else
#include <CLucene.h>
#include <CLucene/util/dirent.h>
#include <CLucene/util/StringBuffer.h>
#include "CLucene/StdHeader.h"
#include "CLucene/_clucene-config.h"

#include "CLucene/config/repl_tchar.h"
#include "CLucene/config/repl_wchar.h"
#include "CLucene/util/Misc.h"

using namespace lucene::store;
using namespace std;
using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::util;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;
#endif

ZefaniaBible::ZefaniaBible()
{
    m_settings = 0;
    m_bibleID = 0;
    m_bookID = 0;
}
void ZefaniaBible::setSettings(Settings *set)
{
    m_settings = set;
}
void ZefaniaBible::loadBibleData(const int &id, const QString &path)
{
    DEBUG_FUNC_NAME
    m_bibleName = "";
    m_bibleID = id;

    if(m_settings->getModuleSettings(m_bibleID).zefbible_hardCache == false && m_settings->getModuleSettings(m_bibleID).zefbible_softCache == false) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Please activate Caching.(Hard or Soft Cache)"));
        return;
    }
    if(m_settings->getModuleSettings(m_bibleID).zefbible_hardCache == true) {
        if(checkForCacheFiles(path)) {
            loadCached(id, path);//load the hard cache files
        } else {
            loadNoCached(id, path);//read the entire xml file
        }

    } else {
        loadNoCached(id, path);//read the entire xml file
    }
}
void ZefaniaBible::removeHardCache(const QString &path)
{
    QDir d(m_settings->homePath + "cache/");
    d.rmdir(m_settings->homePath + "cache/" + m_settings->hash(path));
}

QDomNode ZefaniaBible::readBookFromHardCache(QString path, int bookID)
{
    DEBUG_FUNC_NAME
    QDomElement e;

    const QString pre = m_settings->homePath + "cache/" + m_settings->hash(path) + "/";

    QFile file(pre + QString::number(bookID) + ".xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot read the file."));
        myWarning() << "can't read the file" << pre << QString::number(bookID) << ".xml";
        return e;
    }
    QDomDocument doc;
    if(!doc.setContent(&file)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid."));
        myWarning() << "the file isn't valid";
        return e;
    }
    QDomElement root = doc.documentElement();
    //cache in ram
    //e = root.firstChild().toElement();
    if(!m_softCacheData.contains(bookID) && m_settings->getModuleSettings(m_bibleID).zefbible_softCache == true) {
        /*  softCache[bookID] = e;
        softCacheAvi[bookID] = true;*///todo: check
    }
    return root.firstChild();

}
void ZefaniaBible::readBook(const int &id)
{
    //  DEBUG_FUNC_NAME
    QDomNode ncache;

    if(!m_softCacheData.contains(id)) {
        //myDebug() << "soft cache is empty";
    }
    //book is not in soft cache
    if(m_settings->getModuleSettings(m_bibleID).zefbible_hardCache == true && (!m_softCacheData.contains(id) || m_settings->getModuleSettings(m_bibleID).zefbible_softCache == false)) {
        ncache = readBookFromHardCache(m_biblePath, id);
    } else {
        m_book = softCache(id);
        m_bookCount[id] = m_book.size();
        return;
    }
    //reading loaded data
    m_book.clear();
    m_bookID = id;
    QDomNode n = ncache.firstChild();
    QString outtext;
    int i;
    for(i = 0; !n.isNull(); ++i) {
        QDomElement e = n.toElement();
        Chapter c;
        outtext = "";
        QDomNode n2 = n.firstChild();
        int verseCount = 0;
        while(!n2.isNull()) {  //alle verse
            verseCount++;
            QDomElement e2 = n2.toElement();
            if(e2.tagName().toLower() == "vers") { // read only verse
                c.data <<  e2.text();
                c.verseNumber << e2.attribute("vnumber", "");
            } //todo: all other data
            n2 = n2.nextSibling();
        }
        c.chapterName = e.attribute("cnumber", QString::number(i));
        c.chapterID = c.chapterName.toInt() - 1;

        c.verseCount = verseCount;
        c.bookName = m_bookFullName.at(m_bookIDs.indexOf(QString::number(id)));//todo: use also in zefania-bible the qhash system
        m_book.addChapter(c.chapterID, c);

        n = n.nextSibling();
    }

    m_bookCount[id] = i;
    setSoftCache(m_bookID, m_book);
}
QDomElement ZefaniaBible::format(QDomElement e)
{
    ModuleSettings moduleSettings = m_settings->getModuleSettings(m_bibleID);
    QDomNode n = e.firstChild();
    while(!n.isNull()) {  //all verses
        if(n.nodeName().toLower() == "note") {
            QDomNode node = n;

            QDomText t = node.firstChild().toText();
            if(moduleSettings.zefbible_showStudyNote == true && n.toElement().attribute("type", "") == "x-studynote") {
                t.setData("[<span style=\" font-size:small;\">" + t.data() + "</span>]");
            } else {
                t.setData("");
            }
            node.replaceChild(t, node.firstChild());
            e.replaceChild(node, n);
        } else if((n.nodeName().toLower() == "gram" || n.nodeName().toLower() == "gr") && n.toElement().attribute("str", "") != "" && moduleSettings.zefbible_showStrong == true) {
            QDomNode node = n;
            QDomText t = n.firstChild().toText();
            QDomElement b = n.toElement();
            QString add;
            //todo: that isn't  nice
            if(m_bookID < 39)
                add = "H";
            else
                add = "G";

            t.setData(t.data() + "<sup><a href=\"strong://" + add + b.attribute("str", "") + "\">" + add + b.attribute("str", "") + "</a></sup>  ");
            node.replaceChild(t, node.firstChild());
            e.replaceChild(node, n);
        } else if(n.nodeName().toLower() == "reflink") {
            QDomNode node = n;
            QDomElement b = n.toElement();
            QDomText t = n.firstChild().toText();

            QString mscope = b.attribute("mscope", ";;;");
            const QStringList list = mscope.split(";");
            const int bookID = list.at(0).toInt() - 1;
            const int chapterID = list.at(1).toInt() - 1;
            const int verseID = list.at(2).toInt() - 1;

            BibleUrl burl;
            burl.setBible(BibleUrl::LoadCurrentBible);
            burl.setBookID(bookID);
            burl.setChapterID(chapterID);
            burl.setVerseID(verseID);
            const QString url = burl.toString();

            QString name = "";
            if(bookID < m_settings->bookFullNames.size()) {
                name = m_settings->bookFullNames.at(bookID) + " " + list.at(1) + "," + list.at(2);
            } else {
                name = list.at(0) + " " + list.at(1) + "," + list.at(2);
            }
            t.setData(" <a href=\"" + url + "\">" + name + "</a> ");
            node.replaceChild(t, node.firstChild());
            e.replaceChild(node, n);
        }
        if(n.childNodes().count() > 0)
            e.replaceChild(format(n.toElement()), n);
        n = n.nextSibling();
    }
    return e;

}
/**
  Returns the soft cache for all books.
  */
QHash<int, Book> ZefaniaBible::softCache() const
{
    if(m_settings->getModuleSettings(m_bibleID).zefbible_softCache == true) {
        return m_softCacheData;
    }
    return QHash<int, Book>();
}
/**
  Returns the soft cache for a book
  \param bookID The ID of the book.
  */
Book ZefaniaBible::softCache(const int &bookID) const
{
    // DEBUG_FUNC_NAME
    if(m_settings->getModuleSettings(m_bibleID).zefbible_softCache == true) {
        return m_softCacheData[bookID];
    }
    return Book();
}
/**
  Set the soft cache
  \param QMap<int, QList<Chapter> > cache The cache data.
  */
void ZefaniaBible::setSoftCache(QHash<int, Book > cache)
{
    DEBUG_FUNC_NAME
    if(m_settings->getModuleSettings(m_bibleID).zefbible_softCache == true) {
        m_softCacheData = cache;
    }
    return;
}
/**
  Sets the cache of a book.
  \param bookID The ID of the book.
  \param chapterList New cache.
  */
void ZefaniaBible::setSoftCache(const int &bookID, Book book)
{
    if(m_settings->getModuleSettings(m_bibleID).zefbible_softCache == true) {
        m_softCacheData[bookID] = book;
    }
}
/**
  Clears the soft cache.
  */
void ZefaniaBible::clearSoftCache()
{
    m_softCacheData.clear();
}

/**
  Checks if there are cache files for a given module. If not it returns false.
  \param path The path of the module.
 */
bool ZefaniaBible::checkForCacheFiles(const QString &path)
{
    const QString fileName = m_settings->homePath + "cache/" + m_settings->hash(path) + "/";
    QFile file(fileName + "data");
    if(file.exists())
        return true;
    return false;
}
/**
  Reads the entire xml file and if caching is enabled, generate cache file
  \param id The ID of the module(bible).
  \param path. Path to the module file.
  */
void ZefaniaBible::loadNoCached(const int &id, const QString &path)
{
    // DEBUG_FUNC_NAME
    QProgressDialog progress(QObject::tr("Loading Bible"), QObject::tr("Cancel"), 0, 76);
    progress.setWindowModality(Qt::WindowModal);
    // Q_ASSERT(m_bibleID != id);
    if(m_bibleID != id) {
        m_bookFullName.clear();
        m_bookShortName.clear();
        m_bookCount.clear();
        clearSoftCache();
    }
    m_bibleID = id;
    ModuleSettings moduleSettings = m_settings->getModuleSettings(m_bibleID);
    progress.setValue(1);

    m_biblePath = path;
    //hard cache: genrate fileName
    const QString fileName = m_settings->homePath + "cache/" + m_settings->hash(path) + "/";
    QDir dir;
    dir.mkpath(fileName);
    //
    progress.setValue(2);
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Can not read the file"));
        myWarning() << "can't read the file";
        return;
    }
    progress.setValue(3);
    KoXmlDocument doc;

    QStringList fileList;
    QString data;
    QByteArray first = file.readLine();
    QString firstString(first);
    QString codecString;
    if(moduleSettings.encoding == "Default" || moduleSettings.encoding == "") {
        QRegExp rxlen("encoding=\"(.*)\"");//todo: (.*) should be without whitespaces ,too
        int pos = rxlen.indexIn(firstString);

        if(pos > -1 && !rxlen.cap(1).contains("\"")) {
            codecString = rxlen.cap(1);
        } else {
            codecString = "UTF-8";
        }
    } else {
        codecString = moduleSettings.encoding;
    }
    //myDebug() << "encoding = " << codecString;
    //if codecString is not a valid decoder name, there can be problems
#ifdef Q_WS_WIN
    //windows need some extra decoder functions, i do not know why
    if(codecString.toLower() == "utf-8") {
        while(!file.atEnd()) {
            QByteArray byteline = file.readLine();
            QString l = QString::fromLocal8Bit(byteline.constData());
            data += l;
            fileList << l;
        }
    } else {
        QTextCodec *codec = QTextCodec::codecForName(codecString.toStdString().c_str());
        QTextDecoder *decoder = codec->makeDecoder();
        while(!file.atEnd()) {
            QByteArray byteline = file.readLine();
            QString l = decoder->toUnicode(byteline);
            data += l;
            fileList << l;
        }
    }
#else
    QTextCodec *codec = QTextCodec::codecForName(codecString.toStdString().c_str());
    QTextDecoder *decoder = codec->makeDecoder();
    while(!file.atEnd()) {
        QByteArray byteline = file.readLine();
        QString l = decoder->toUnicode(byteline);
        data += l;
        fileList << l;
    }
#endif
    progress.setValue(5);
    QString error;
    int l;
    int c;

    if(!doc.setContent(data, &error, &l, &c)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid. Errorstring: %1 in Line %2 at Position %3").arg(error).arg(l).arg(c));
        myWarning() << "the file isn't valid";
        return;
    }
    data.clear();



    progress.setValue(10);
    KoXmlElement root = doc.documentElement();
    m_bibleName = root.attribute("biblename", "");
    if(m_bibleName == "") {
        m_bibleName = moduleSettings.moduleName;
    }

    KoXmlNode n = doc.documentElement().firstChild();
    int progressCounter = 10;
    int currentPos = 0;
    for(int c = 0; !n.isNull();) {
        progressCounter++;
        if(progressCounter < 76)
            progress.setValue(progressCounter);
        KoXmlElement e = n.toElement();
        if(e.attribute("bname", "") != "" || e.attribute("bnumber", "") != "") {
            //it is the caching mechanisme
            int start = 0, end = 0;
            QString bookID = QString::number(e.attribute("bnumber").toInt() - 1); //i count from zero
            for(int i = currentPos; i < fileList.size(); ++i) {
                QString line = fileList.at(i);
                if(line.contains("<BIBLEBOOK", Qt::CaseInsensitive)) {
                    currentPos = i + 1;
                    start = i;
                    break;
                }

            }
            for(int i = currentPos; i < fileList.size(); ++i) {
                QString line = fileList.at(i);
                if(line.contains("</BIBLEBOOK", Qt::CaseInsensitive)) {
                    currentPos = i + 1;
                    end = i;
                    break;
                }

            }
            QString data = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<cache>\n";
            for(int i = start; i <= end; ++i) {
                data += fileList.at(i);
            }
            data += "</cache>";
            QFile file(fileName + "/" + bookID + ".xml");
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                return;
            }
            QTextStream out(&file);
            out << data;
            file.close();
            m_bookFullName << e.attribute("bname", e.attribute("bsname", ""));
            m_bookShortName << e.attribute("bsname", "");
            m_bookIDs << bookID;
            c++;
        }
        n = n.nextSibling();
    }
    fileList.clear();
    bool hasAny = false;
    for(int i = 0; i < m_bookFullName.size(); i++) {
        if(m_bookFullName.at(i) != "") {
            hasAny = true;
            break;
        }
    }
    if(!hasAny) {
        //whole bible
        if(m_bookFullName.size() == 66) {
            m_bookFullName.clear();
            //load default booknames
            m_bookFullName = m_settings->bookFullNames;
        } else if(m_bookFullName.size() == 27) {
            m_bookFullName.clear();
            //load default booknames
            QStringList b = m_settings->bookFullNames;
            for(int v = 0; v < 39; v++) {
                b.removeFirst();
            }
            m_bookFullName = b;
        } /* what else ?*/
    }
    progress.hide();
    file.close();
    if(moduleSettings.zefbible_hardCache == true) {
        //hard cache: write data
        //write the booknames in a cache file
        QFile file(fileName + "data");
        if(!file.open(QIODevice::WriteOnly)) {
            return;
        }
        QDataStream out(&file);
        out << m_bibleName << m_bookFullName << m_bookShortName << m_bookIDs;
        file.close();
    }
}

/*
  load only booknames and not every book and his data
  */
void ZefaniaBible::loadCached(const int &id, const QString &path)
{
    if(m_bibleID != id) {
        //clear old data
        m_bookFullName.clear();
        m_bookShortName.clear();
        m_bookCount.clear();
        clearSoftCache();
    }


    const QString fileName = m_settings->homePath + "cache/" + m_settings->hash(path) + "/";
    QFile file(fileName + "data");
    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QDataStream in(&file);
    QString bibleName;
    QStringList fullName;
    QStringList shortName;
    QStringList bookIDs;
    in >> bibleName >> fullName >> shortName >> bookIDs;
    m_bibleName = bibleName;
    m_bookFullName = fullName;
    m_bookShortName = shortName;
    m_bookIDs = bookIDs;
    //myDebug() << m_bookShortName;
    m_bibleID = id;
    m_biblePath = path;
    if(m_bibleName == "") {
        m_bibleName = m_settings->getModuleSettings(m_bibleID).moduleName;
    }
}

QString ZefaniaBible::readInfo(QFile &file)
{
    KoXmlDocument doc;
    if(!doc.setContent(&file)) {
        file.close();
        return "";
    }

    KoXmlElement root = doc.documentElement();
    m_bibleName = root.attribute("biblename", "");
    file.close();
    return m_bibleName;
}
/**
  Read the module file and returns the bible name
  \param content  Content of the module file.
 */
QString ZefaniaBible::readInfo(const QString &content)
{
    //todo: read only the first 20 lines and find in them the biblename
    KoXmlDocument doc;
    if(!doc.setContent(content)) {
        return "";
    }
    KoXmlElement root = doc.documentElement();
    m_bibleName = root.attribute("biblename", "");
    return m_bibleName;
}
/**
  Convert a node into a chapterlist.
  \param bookID The bookID.
  \param ncache The node to convert.
  */
Book ZefaniaBible::fromHardToSoft(const int &bookID, QDomNode ncache)
{
    Book book;
    QDomNode n = ncache.firstChild();
    for(int i = 0; !n.isNull(); ++i) {
        Chapter c;
        QDomNode n2 = n.firstChild();
        QDomElement e = n.toElement();
        int verseCount = 0;
        while(!n2.isNull()) {
            verseCount++;
            QDomElement e2 = n2.toElement();
            e2 = format(e2);
            c.data.append(e2.text());
            c.verseNumber.append(e2.attribute("vnumber", ""));
            n2 = n2.nextSibling();
        }
        c.chapterName = e.attribute("cnumber", QString::number(i));
        c.chapterID = c.chapterName.toInt() - 1;//i count from zero
        c.verseCount = verseCount;
        c.bookName = m_bookFullName.at(bookID);

        book.addChapter(c.chapterID, c);

        n = n.nextSibling();
    }
    return book;
}
bool ZefaniaBible::hasIndex() const
{
    DEBUG_FUNC_NAME
    QDir d;
    if(!d.exists(m_settings->homePath + "index")) {
        return false;
    }
    //todo: check versions

    const QString index = m_settings->homePath + "index/" + m_settings->hash(m_biblePath);

#ifdef _CLUCENE_LEGACY
    return IndexReader::indexExists(index.toLocal8Bit().constData());
#else
    return IndexReader::indexExists(index.toStdString().c_str());
#endif
}
//A faster (10%) alternative
/*
void ZefaniaBible::buildIndex()
{
    DEBUG_FUNC_NAME
    QTime t;
    t.start();

    QProgressDialog progress(QObject::tr("Build index"), QObject::tr("Cancel"), 0, m_bookFullName.size()+2);
    progress.setValue(1);
    const QString index = m_settings->homePath + "index/" + m_settings->hash(m_biblePath);
    QDir dir("/");
    dir.mkpath(index);

    // do not use any stop words
    const TCHAR* stop_words[]  = { NULL };
    StandardAnalyzer an((const TCHAR**)stop_words);

    if(IndexReader::indexExists(index.toLocal8Bit().constData())) {
        if(IndexReader::isLocked(index.toLocal8Bit().constData())) {
            IndexReader::unlock(index.toLocal8Bit().constData());
        }
    }

    QScopedPointer< IndexWriter> writer(new  IndexWriter(index.toLocal8Bit().constData(), &an, true));   //always create a new index
    progress.setValue(2);

    QFile file(m_biblePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Can not read the file"));
        myWarning() << "can't read the file";
        return;
    }
    KoXmlDocument xmlDoc;
    QString error;
    int l;
    int c;
    if(!xmlDoc.setContent(&file, &error, &l, &c)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid. Errorstring: %1 in Line %2 at Position %3").arg(error).arg(l).arg(c));
        myWarning() << "the file isn't valid";
        return;
    }
    wchar_t wcharBuffer[MAX_LUCENE_FIELD_LENGTH + 1];
    myDebug() << "start indexing";
    KoXmlNode nBooks = xmlDoc.documentElement().firstChild();
    //QScopedPointer<Document> doc(new  Document());
    for(int c = 0; !nBooks.isNull();) {
        KoXmlElement eBooks = nBooks.toElement();
        if(eBooks.attribute("bname", "") != "" || eBooks.attribute("bnumber", "") != "") {
            const QString bookID = QString::number(eBooks.attribute("bnumber").toInt() - 1);
            myDebug() << bookID;
            progress.setValue(c+3);
            KoXmlNode nChapters = nBooks.firstChild();
            for(int i = 0; !nChapters.isNull(); ++i) {
                KoXmlNode nVerse = nChapters.firstChild();
                const QString chapterID = nChapters.toElement().attribute("cnumber", QString::number(i));
                while(!nVerse.isNull()) {
                    KoXmlElement eVerse = nVerse.toElement();
                    if(eVerse.tagName().toLower() == "vers") {// read only verse
                        QScopedPointer<Document> doc(new Document());
                        const QString verseText = eVerse.text();
                        const QString verseID = eVerse.attribute("vnumber", "");
                        const QString key = bookID + ";" + chapterID + ";" + verseID;

                        lucene_utf8towcs(wcharBuffer, key.toLocal8Bit().constData(), MAX_LUCENE_FIELD_LENGTH);
                        doc->add(*(new  Field((const TCHAR*)_T("key"), (const TCHAR*)wcharBuffer,  Field::STORE_YES |  Field::INDEX_NO)));

                        lucene_utf8towcs(wcharBuffer, bookID.toLocal8Bit().constData(), MAX_LUCENE_FIELD_LENGTH);
                        doc->add(*(new  Field((const TCHAR*)_T("book"), (const TCHAR*)wcharBuffer,  Field::STORE_YES |  Field::INDEX_UNTOKENIZED)));

                        lucene_utf8towcs(wcharBuffer, chapterID.toLocal8Bit().constData(), MAX_LUCENE_FIELD_LENGTH);
                        doc->add(*(new  Field((const TCHAR*)_T("chapter"), (const TCHAR*)wcharBuffer,  Field::STORE_YES |  Field::INDEX_UNTOKENIZED)));

                        lucene_utf8towcs(wcharBuffer, verseID.toLocal8Bit().constData(), MAX_LUCENE_FIELD_LENGTH);
                        doc->add(*(new  Field((const TCHAR*)_T("verse"), (const TCHAR*)wcharBuffer,  Field::STORE_YES |  Field::INDEX_UNTOKENIZED)));



                        lucene_utf8towcs(wcharBuffer, verseText.toUtf8().constData() , MAX_LUCENE_FIELD_LENGTH);
                        doc->add(*(new  Field((const TCHAR*)_T("content"),
                                              (const TCHAR*) wcharBuffer,
                                              Field::STORE_YES |  Field::INDEX_TOKENIZED)));
                        writer->addDocument(doc.data());

                    }
                    nVerse = nVerse.nextSibling();
                }
                nChapters = nChapters.nextSibling();
            }
            c++;
        }
        nBooks = nBooks.nextSibling();
    }
    xmlDoc.clear();
    writer->optimize();
    writer->close();
    writer.reset();
    progress.close();
    myDebug() << "Time elapsed: " << t.elapsed() << "ms";

}
*/


void ZefaniaBible::buildIndex()
{
    DEBUG_FUNC_NAME
    QProgressDialog progress(QObject::tr("Build index"), QObject::tr("Cancel"), 0, m_bookFullName.size() + 2);
    progress.setValue(1);

    const QString index = m_settings->homePath + "index/" + m_settings->hash(m_biblePath);
    QDir dir("/");
    dir.mkpath(index);
#ifdef _CLUCENE_LEGACY
    // do not use any stop words
    const TCHAR* stop_words[]  = { NULL };
    StandardAnalyzer an((const TCHAR**)stop_words);

    if(IndexReader::indexExists(index.toLocal8Bit().constData())) {
        if(IndexReader::isLocked(index.toLocal8Bit().constData())) {
            IndexReader::unlock(index.toLocal8Bit().constData());
        }
    }

    QScopedPointer< IndexWriter> writer(new  IndexWriter(index.toLocal8Bit().constData(), &an, true));   //always create a new index
    progress.setValue(2);
    progress.setWindowModality(Qt::NonModal);

    for(int i = 0; i < m_bookFullName.size(); ++i) {
        progress.setValue(i + 3);
        Book book;
        myDebug() << "book = " << i;

        if(m_settings->getModuleSettings(m_bibleID).zefbible_hardCache == true && (!m_softCacheData.contains(i) || m_settings->getModuleSettings(m_bibleID).zefbible_softCache == false)) {
            book = fromHardToSoft(i, readBookFromHardCache(m_biblePath, i));
            setSoftCache(i, book);
        } else {
            book = softCache(i);
        }

        wchar_t wcharBuffer[MAX_LUCENE_FIELD_LENGTH + 1];
        QHash<int, Chapter>::const_iterator it = book.m_chapters.constBegin();
        while(it != book.m_chapters.constEnd()) {
            Chapter c = it.value();
            QStringList verse = c.data;
            for(int verseCounter = 0; verseCounter < verse.size(); ++verseCounter) {
                const QString t = verse.at(verseCounter);
                QScopedPointer< Document> doc(new  Document());
                const QString book = QString::number(i);
                const QString chapter = QString::number(it.key());
                const QString verse = QString::number(verseCounter);

                QString key = book + ";" + chapter + ";" + verse;


                lucene_utf8towcs(wcharBuffer, key.toLocal8Bit().constData(), MAX_LUCENE_FIELD_LENGTH);
                doc->add(*(new  Field((const TCHAR*)_T("key"), (const TCHAR*)wcharBuffer,  Field::STORE_YES |  Field::INDEX_NO)));

                lucene_utf8towcs(wcharBuffer, t.toUtf8().constData(), MAX_LUCENE_FIELD_LENGTH);
                doc->add(*(new  Field((const TCHAR*)_T("content"),
                                      (const TCHAR*)wcharBuffer,
                                      Field::STORE_YES |  Field::INDEX_TOKENIZED)));

                writer->addDocument(doc.data());
            }
            it++;
        }
    }

    writer->optimize();
    writer->close();
    writer.reset();
#else
    IndexWriter* writer = NULL;
    lucene::analysis::WhitespaceAnalyzer an;
    if(IndexReader::indexExists(index.toStdString().c_str())) {
        if(IndexReader::isLocked(index.toStdString().c_str())) {
            myDebug() << "Index was locked... unlocking it.";
            IndexReader::unlock(index.toStdString().c_str());
        }
    }
    writer = _CLNEW IndexWriter(index.toStdString().c_str() , &an, true);

    writer->setMaxFieldLength(0x7FFFFFFFL);
    writer->setUseCompoundFile(false);

    //index
    Document doc;
    myDebug() << m_bookFullName.size();
    for(int i = 0; i < m_bookFullName.size(); ++i) {
        progress.setValue(i + 3);
        Book book;
        myDebug() << "book = " << i;

        if(m_settings->getModuleSettings(m_bibleID).zefbible_hardCache == true && (!m_softCacheData.contains(i) || m_settings->getModuleSettings(m_bibleID).zefbible_softCache == false)) {
            book = fromHardToSoft(i, readBookFromHardCache(m_biblePath, i));
            setSoftCache(i, book);
        } else {
            book = softCache(i);
        }

        QHash<int, Chapter>::const_iterator it = book.m_chapters.constBegin();
        while(it != book.m_chapters.constEnd()) {
            Chapter c = it.value();
            QStringList verse = c.data;
            for(int verseCounter = 0; verseCounter < verse.size(); ++verseCounter) {
                doc.clear();
                const QString t = verse.at(verseCounter);
                const QString book = QString::number(i);
                const QString chapter = QString::number(it.key());
                const QString verse = QString::number(verseCounter);

                QString key = book + ";" + chapter + ";" + verse;
                doc.add(*_CLNEW Field(_T("key"), key.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_NO));
                doc.add(*_CLNEW Field(_T("content"), t.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));

                writer->addDocument(&doc);
            }
            it++;
        }
    }


    writer->setUseCompoundFile(true);
    writer->optimize();

    writer->close();
    _CLLDELETE(writer);
#endif
    progress.close();

}
void ZefaniaBible::search(SearchQuery query, SearchResult *res)
{
    DEBUG_FUNC_NAME
    const QString i = m_settings->homePath + "index/" + m_settings->hash(m_biblePath);

#ifdef _CLUCENE_LEGACY

    char utfBuffer[MAX_LUCENE_FIELD_LENGTH + 1];
    wchar_t wcharBuffer[MAX_LUCENE_FIELD_LENGTH + 1];

    const TCHAR* stop_words[] = { NULL };
    StandardAnalyzer analyzer(stop_words);

    IndexSearcher searcher(i.toLocal8Bit().constData());
    lucene_utf8towcs(wcharBuffer, query.searchText.toUtf8().constData(), MAX_LUCENE_FIELD_LENGTH);
    QScopedPointer<Query> q(QueryParser::parse((const TCHAR*)wcharBuffer, (const TCHAR*)_T("content"), &analyzer));

    QScopedPointer<Hits> h(searcher.search(q.data(), Sort::INDEXORDER));

    Document* doc = 0;
    for(int i = 0; i < h->length(); ++i) {
        doc = &h->doc(i);
        lucene_wcstoutf8(utfBuffer, (const wchar_t*)doc->get((const TCHAR*)_T("key")), MAX_LUCENE_FIELD_LENGTH);
        QString stelle(utfBuffer);
        QStringList l = stelle.split(";");
        //todo: quite hacky the hardcoded book count
        if(query.range == SearchQuery::Whole || (query.range == SearchQuery::OT && l.at(0).toInt() <= 38) || (query.range == SearchQuery::NT && l.at(0).toInt() > 38)) {
            lucene_wcstoutf8(utfBuffer, (const wchar_t*)doc->get((const TCHAR*)_T("content")), MAX_LUCENE_FIELD_LENGTH);
            //myDebug() << stelle << QString::fromUtf8(utfBuffer);
            SearchHit hit;
            hit.setType(SearchHit::BibleHit);
            hit.setValue(SearchHit::BibleID, m_bibleID);
            hit.setValue(SearchHit::BookID, l.at(0).toInt());
            hit.setValue(SearchHit::ChapterID, l.at(1).toInt());
            hit.setValue(SearchHit::VerseID, l.at(2).toInt());
            hit.setValue(SearchHit::VerseText, QString::fromUtf8(utfBuffer));
            res->addHit(hit);
        }
    }
#else
    const TCHAR* stop_words[] = { NULL };
    standard::StandardAnalyzer analyzer(stop_words);
    IndexReader* reader = IndexReader::open(i.toStdString().c_str());
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
#endif
}
