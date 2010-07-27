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
#include <QtCore/QDebug>
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

//Maximum index entry size, 1MiB for now
//Lucene default is too small
const unsigned long BT_MAX_LUCENE_FIELD_LENGTH = 1024 * 1024;
#include <CLucene.h>
#include <CLucene/util/Misc.h>
#include <CLucene/util/Reader.h>

ZefaniaBible::ZefaniaBible()
{
    m_settings = new Settings();
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
    // DEBUG_FUNC_NAME
    QDomElement e;

    const QString pre = m_settings->homePath + "cache/" + m_settings->hash(path) + "/";
    QFile file(pre + QString::number(bookID) + ".xml");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot read the file."));
        myWarning() << "can't read the file";
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
        myDebug() << "soft cache is empty";
    }
    //book is not in soft cache
    if(m_settings->getModuleSettings(m_bibleID).zefbible_hardCache == true && (!m_softCacheData.contains(id) || m_settings->getModuleSettings(m_bibleID).zefbible_softCache == false)) {
        ncache = readBookFromHardCache(m_biblePath, id);
    } else {
        m_chapterData = softCache(id);
        m_bookCount[id] = m_chapterData.size();
        return;
    }
    //reading loaded data
    m_chapterData.clear();
    m_bookID = id;
    QDomNode n = ncache.firstChild();
    QString outtext;
    int i;
    for(i = 0; !n.isNull(); ++i) {
        Chapter c;
        outtext = "";
        QDomNode n2 = n.firstChild();
        int verseCount = 0;
        while(!n2.isNull()) {  //alle verse
            verseCount++;
            QDomElement e2 = n2.toElement();
            c.data <<  e2.text();
            c.verseNumber << e2.attribute("vnumber", "");
            n2 = n2.nextSibling();
        }
        c.chapterName = QString::number(i + 1, 10);
        c.verseCount = verseCount;
        c.bookName = m_bookFullName.at(id);
        m_chapterData << c;

        n = n.nextSibling();
    }

    m_bookCount[id] = i;
    setSoftCache(m_bookID, m_chapterData);
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
QMap<int, QList<Chapter> > ZefaniaBible::softCache()
{
    if(m_settings->getModuleSettings(m_bibleID).zefbible_softCache == true) {
        return m_softCacheData;
    }
    return QMap<int, QList<Chapter> >();
}
/**
  Returns the soft cache for a book
  \param bookID The ID of the book.
  */
QList<Chapter> ZefaniaBible::softCache(int bookID)
{
    // DEBUG_FUNC_NAME
    if(m_settings->getModuleSettings(m_bibleID).zefbible_softCache == true) {
        return m_softCacheData[bookID];
    }
    return QList<Chapter>();
}
/**
  Set the soft cache
  \param QMap<int, QList<Chapter> > cache The cache data.
  */
void ZefaniaBible::setSoftCache(QMap<int, QList<Chapter> > cache)
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
void ZefaniaBible::setSoftCache(int bookID, QList<Chapter> chapterList)
{
    if(m_settings->getModuleSettings(m_bibleID).zefbible_softCache == true) {
        m_softCacheData[bookID] = chapterList;
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
    myDebug() << "encoding = " << codecString;
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

    progress.setValue(5);
    QString error;
    int l;
    int c;

    if(!doc.setContent(data, &error, &l, &c)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid. Errorstring: %1 in Line %2 at Position %2").arg(error).arg(l).arg(c));
        myWarning() << "the file isn't valid";
        return;
    }
    data.clear();
#endif


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
            QFile file(fileName + "/" + QString::number(c) + ".xml");
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                return;
            }
            QTextStream out(&file);
            out << data;
            file.close();
            m_bookFullName << e.attribute("bname", e.attribute("bsname", ""));
            m_bookShortName << e.attribute("bsname", "");
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
            m_bookFullName = m_settings->bookNames;
        } else if(m_bookFullName.size() == 27) {
            m_bookFullName.clear();
            //load default booknames
            QStringList b = m_settings->bookNames;
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
        out << m_bibleName << m_bookFullName << m_bookShortName;
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
    in >> bibleName >> fullName >> shortName;
    m_bibleName = bibleName;
    m_bookFullName = fullName;
    m_bookShortName = shortName;
    myDebug() << m_bookShortName;
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
    QString cbiblename;
    KoXmlDocument doc;
    if(!doc.setContent(content)) {
        return "";
    }
    KoXmlElement root = doc.documentElement();
    cbiblename = root.attribute("biblename", "");
    m_bibleName = cbiblename;
    return m_bibleName;
}
/**
  Convert a node into a chapterlist.
  \param bookID The bookID.
  \param ncache The node to convert.
  */
QList<Chapter> ZefaniaBible::fromHardToSoft(int bookID, QDomNode ncache)
{
    QList<Chapter> ret;
    QDomNode n = ncache.firstChild();
    for(int i = 0; !n.isNull(); ++i) {
        Chapter c;
        QDomNode n2 = n.firstChild();
        int verseCount = 0;
        while(!n2.isNull()) {
            verseCount++;
            QDomElement e2 = n2.toElement();
            e2 = format(e2);
            c.data.append(e2.text());
            c.verseNumber.append(e2.attribute("vnumber", ""));
            n2 = n2.nextSibling();
        }
        c.chapterName = QString::number(i + 1, 10);
        c.verseCount = verseCount;
        c.bookName = m_bookFullName.at(bookID);
        ret.append(c);
        n = n.nextSibling();
    }
    return ret;
}
bool ZefaniaBible::hasIndex()
{
    DEBUG_FUNC_NAME
    QDir d;
    if(!d.exists(m_settings->homePath + "index")) {
        return false;
    }
    //todo: check versions
    QString index = m_settings->homePath + "index/" + m_settings->hash(m_biblePath);

    return lucene::index::IndexReader::indexExists(index.toAscii().constData());
}

void ZefaniaBible::buildIndex()
{
    DEBUG_FUNC_NAME

    QString index = m_settings->homePath + "index/" + m_settings->hash(m_biblePath);
    QDir dir("/");
    dir.mkpath(index);

    // do not use any stop words
    const TCHAR* stop_words[]  = { NULL };
    lucene::analysis::standard::StandardAnalyzer an((const TCHAR**)stop_words);

    if(lucene::index::IndexReader::indexExists(index.toAscii().constData())) {
        if(lucene::index::IndexReader::isLocked(index.toAscii().constData())) {
            lucene::index::IndexReader::unlock(index.toAscii().constData());
        }
    }
    QScopedPointer<lucene::index::IndexWriter> writer(new lucene::index::IndexWriter(index.toAscii().constData(), &an, true));   //always create a new index

    QProgressDialog progress(QObject::tr("Build index"), QObject::tr("Cancel"), 0, m_bookFullName.size());
    progress.setWindowModality(Qt::WindowModal);
    for(int i = 0; i < m_bookFullName.size(); ++i) {
        progress.setValue(i);
        QList<Chapter> chapterList;

        //load book from cache(soft or hard)
        //todo: check : if only 2 or 3 book are aviable in softcache then load whole book else load everything from hardcache
        //todo: cant load book if only softcache is enabled

        if(m_settings->getModuleSettings(m_bibleID).zefbible_hardCache == true && (!m_softCacheData.contains(i) || m_settings->getModuleSettings(m_bibleID).zefbible_softCache == false)) {
            chapterList = fromHardToSoft(i, readBookFromHardCache(m_biblePath, i));
            setSoftCache(i, chapterList);
        } else {
            chapterList =  softCache(i);
        }
        QByteArray textBuffer;
        wchar_t wcharBuffer[BT_MAX_LUCENE_FIELD_LENGTH + 1];
        for(int chapterCounter = 0; chapterCounter < chapterList.size(); ++chapterCounter) {
            Chapter c = chapterList.at(chapterCounter);
            QStringList verse = c.data;
            for(int verseCounter = 0; verseCounter < verse.size(); ++verseCounter) {
                QString t = verse.at(verseCounter);
                QScopedPointer<lucene::document::Document> doc(new lucene::document::Document());
                QString key = QString::number(i) + ";" + QString::number(chapterCounter) + ";" + QString::number(verseCounter);

                lucene_utf8towcs(wcharBuffer, key.toLocal8Bit().constData(), BT_MAX_LUCENE_FIELD_LENGTH);

                doc->add(*(new lucene::document::Field((const TCHAR*)_T("key"), (const TCHAR*)wcharBuffer, lucene::document::Field::STORE_YES | lucene::document::Field::INDEX_NO)));

                lucene_utf8towcs(wcharBuffer, t.toUtf8().constData(), BT_MAX_LUCENE_FIELD_LENGTH);

                doc->add(*(new lucene::document::Field((const TCHAR*)_T("content"),
                                                       (const TCHAR*)wcharBuffer,
                                                       lucene::document::Field::STORE_YES | lucene::document::Field::INDEX_TOKENIZED)));
                textBuffer.resize(0); //clean up
                writer->addDocument(doc.data());
            }
        }
    }
    writer->optimize();
    writer->close();
    progress.close();

}
SearchResult ZefaniaBible::search(SearchQuery query)
{
    SearchResult res;
    res.searchQuery = query;
    QString index = m_settings->homePath + "index/" + m_settings->hash(m_biblePath);
    char utfBuffer[BT_MAX_LUCENE_FIELD_LENGTH  + 1];
    wchar_t wcharBuffer[BT_MAX_LUCENE_FIELD_LENGTH + 1];


    const TCHAR* stop_words[]  = { NULL };
    lucene::analysis::standard::StandardAnalyzer analyzer(stop_words);

    lucene::search::IndexSearcher searcher(index.toLocal8Bit().constData());
    lucene_utf8towcs(wcharBuffer, query.searchText.toUtf8().constData(), BT_MAX_LUCENE_FIELD_LENGTH);
    QScopedPointer<lucene::search::Query> q(lucene::queryParser::QueryParser::parse((const TCHAR*)wcharBuffer, (const TCHAR*)_T("content"), &analyzer));

    QScopedPointer<lucene::search::Hits> h(searcher.search(q.data(), lucene::search::Sort::INDEXORDER));

    lucene::document::Document* doc = 0;
    for(int i = 0; i < h->length(); ++i) {
        doc = &h->doc(i);
        lucene_wcstoutf8(utfBuffer, (const wchar_t*)doc->get((const TCHAR*)_T("key")), BT_MAX_LUCENE_FIELD_LENGTH);
        QString stelle(utfBuffer);
        QStringList l = stelle.split(";");
        lucene_wcstoutf8(utfBuffer, (const wchar_t*)doc->get((const TCHAR*)_T("content")), BT_MAX_LUCENE_FIELD_LENGTH);

        QString content(utfBuffer);
        SearchHit hit;
        hit.setType(SearchHit::BibleHit);
        hit.setValue(SearchHit::BibleID,m_bibleID);
        hit.setValue(SearchHit::BookID,l.at(0).toInt());
        hit.setValue(SearchHit::ChapterID,l.at(1).toInt());
        hit.setValue(SearchHit::VerseID,l.at(2).toInt());
        hit.setValue(SearchHit::VerseText,content);
        res.addHit(hit);

    }
    return res;
}
