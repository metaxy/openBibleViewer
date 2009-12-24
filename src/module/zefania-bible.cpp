/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#include <QtCore/QCryptographicHash>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>
#include <QtXml/QDomAttr>
#include <QtXml/QDomElement>
#include <QtXml/QtXml>
#include "zefania-bible.h"
#include "../core/KoXmlWriter.h"
#include "../core/dbghelper.h"
ZefaniaBible::ZefaniaBible()
{
    m_settings = new Settings();
}
int ZefaniaBible::setSettings(Settings *set)
{
    DEBUG_FUNC_NAME
    m_settings = set;
    return 1;
}
void ZefaniaBible::loadBibleData(const int &id, const QString &path)
{
    bibleName = "";
    m_bibleID = id;
    if (m_settings->getModuleSettings(m_bibleID).zefbible_hardCache == false && m_settings->getModuleSettings(m_bibleID).zefbible_softCache == false) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Please activate Caching.(Hard or Soft Cache)"));
        return;
    }
    if (m_settings->getModuleSettings(m_bibleID).zefbible_hardCache == true) {
        if (checkForCacheFiles(path)) {
            loadCached(id, path);//load the hard cache files
        } else {
            loadNoCached(id, path);//read the entire xml file
        }

    } else {
        loadNoCached(id, path);//read the entire xml file
    }
    m_settings->setBookCount(m_bibleID, bookCount);
    m_settings->setBookNames(m_bibleID, bookFullName);
    m_settings->setBiblePath(m_bibleID, path);
    myDebug() << "bibleName = " << bibleName;
    m_settings->setBibleName(m_bibleID, bibleName);

}
void ZefaniaBible::removeHardCache(const QString &path)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(path.toLocal8Bit());
    QDir d(m_settings->homePath + "cache/");
    d.rmdir(m_settings->homePath + "cache/" + hash.result().toBase64());
}

QDomNode ZefaniaBible::readBookFromHardCache(QString path, int bookID)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(path.toLocal8Bit());
    QString fileName = m_settings->homePath + "cache/" + hash.result().toBase64() + "/";
    QDomElement e;
    QFile file(fileName + QString::number(bookID) + ".xml");
    myDebug() << "fileName = " << file.fileName();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Can not read the file."));
        myDebug() << "cant read the file";
        return e;
    }
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid."));
        myDebug() << "the file isnt valid";
        return e;
    }
    QDomElement root = doc.documentElement();
    //cache in ram
    //e = root.firstChild().toElement();
    if (softCacheData[bookID].isEmpty() && m_settings->getModuleSettings(m_bibleID).zefbible_softCache == true) {
        /*  softCache[bookID] = e;
        softCacheAvi[bookID] = true;*/
    }

    //qDebug() << "zefania::readBookFromHardCache()" << doc.toString();
    return root.firstChild();

}
void ZefaniaBible::readBook(const int &id)
{
    DEBUG_FUNC_NAME
    myDebug() << "hardCache = " << m_settings->getModuleSettings(m_bibleID).zefbible_hardCache  << " softCache = " << m_settings->getModuleSettings(m_bibleID).zefbible_softCache << " softCacheData[id].size() = " << softCacheData[id].size();
    QDomNode ncache;
    if (m_settings->getModuleSettings(m_bibleID).zefbible_hardCache == true && (softCacheData[id].isEmpty() || m_settings->getModuleSettings(m_bibleID).zefbible_softCache == false)) {
        ncache = readBookFromHardCache(currentBiblePath, id);
    } else {
        myDebug() << "read data from sofcache";
        chapterData = softCache(id);
        return;
    }
    chapterData.clear();
    currentBookID = id;
    QDomNode n = ncache.firstChild();
    QString outtext;
    int i;
    for (i = 0; !n.isNull(); ++i) {
        Chapter c;
        outtext = "";
        QDomNode n2 = n.firstChild();
        int verseCount = 0;
        while (!n2.isNull()) { //alle verse
            verseCount++;
            QDomElement e2 = n2.toElement();
            e2 = format(e2);
            c.data <<  e2.text();
            c.verseNumber << e2.attribute("vnumber", "");
            n2 = n2.nextSibling();
        }
        c.chapterName = QString::number(i + 1, 10);
        c.verseCount = verseCount;
        c.bookName = bookFullName.at(id);
        chapterData << c;

        n = n.nextSibling();
    }
    bookCount[id] = i;
    setSoftCache(currentBookID, chapterData);

    myDebug() << "chapterData.size() = " << chapterData.size() << " bookCount = " << i;
}
QMap<int, QList<Chapter> > ZefaniaBible::softCache()
{
    if (m_settings->getModuleSettings(m_bibleID).zefbible_softCache == true) {
        return softCacheData;
    }
    return QMap<int, QList<Chapter> >();
}
QList<Chapter> ZefaniaBible::softCache(int bookID)
{
    if (m_settings->getModuleSettings(m_bibleID).zefbible_softCache == true) {
        return softCacheData[bookID];
    }
    return QList<Chapter>();
}
bool ZefaniaBible::setSoftCache(QMap< int, QList<Chapter> > cache)
{
    DEBUG_FUNC_NAME
    if (m_settings->getModuleSettings(m_bibleID).zefbible_softCache == true) {
        softCacheData.clear();
        //softCacheAvi.clear();
        softCacheData = cache;
        /*  QMapIterator< int, QList<Chapter> > i(cache);
          while (i.hasNext()) {
              softCacheAvi[i.key()] = true;
              i.next();
          }*/
    }
    return true;
}
bool ZefaniaBible::setSoftCache(int bookID, QList<Chapter> c)
{
    myDebug() << "softCache = " << m_settings->getModuleSettings(m_bibleID).zefbible_softCache << " , bookID = " << bookID;
    if (m_settings->getModuleSettings(m_bibleID).zefbible_softCache == true) {
        softCacheData[bookID] = c;
    }
    return true;
}
void ZefaniaBible::clearSoftCache()
{
    DEBUG_FUNC_NAME
    softCacheData.clear();
}

QDomElement ZefaniaBible::format(QDomElement e)
{
    QDomNode n = e.firstChild();
    while (!n.isNull()) { //all verses
        if (n.nodeName().toLower() == "note") {
            QDomNode node = n;
            QDomText t = node.firstChild().toText();
            if (m_settings->getModuleSettings(m_bibleID).zefbible_showStudyNote == true && n.toElement().attribute("type", "") == "x-studynote") {
                t.setData("[<span style=\" font-size:small;\">" + t.data() + "</span>]");
            } else {
                t.setData("");
            }
            node.replaceChild(t, node.firstChild());
            e.replaceChild(node, n);
        } else if ((n.nodeName().toLower() == "gram" || n.nodeName().toLower() == "gr") && n.toElement().attribute("str", "") != "" && m_settings->getModuleSettings(m_bibleID).zefbible_showStrong == true) {
            QDomNode node = n;
            QDomText t = n.firstChild().toText();
            QDomElement b = n.toElement();
            QString add;
            //todo: that isn't  nice
            if (currentBookID < 39)
                add = "H";
            else
                add = "G";

            t.setData(t.data() + "<sup><a href=\"strong://" + add + b.attribute("str", "") + "\">" + add + b.attribute("str", "") + "</a></sup>  ");
            node.replaceChild(t, node.firstChild());
            e.replaceChild(node, n);
        }
        if (n.childNodes().count() > 0)
            e.replaceChild(format(n.toElement()), n);
        n = n.nextSibling();
    }
    return e;

}
bool ZefaniaBible::checkForCacheFiles(const QString &path)
{
    myDebug() << "path = " << path;
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(path.toLocal8Bit());
    QString fileName = m_settings->homePath + "cache/" + hash.result().toBase64() + "/";
    QFile file(fileName + "data");
    if (file.exists())
        return true;
    return false;
}
/*
  read the entire xml file
  and if caching is enabled, generate cache files
  */
void ZefaniaBible::loadNoCached(const int &id, const QString &path)
{
    //todo: maybe i should remove the qdom support because koxml works really fine
    DEBUG_FUNC_NAME
    QProgressDialog progress(QObject::tr("Loading Bible"), QObject::tr("Cancel"), 0, 76);
    progress.setWindowModality(Qt::WindowModal);

    m_bibleID = id;
    bookFullName.clear();
    bookShortName.clear();
    bookCount.clear();
    clearSoftCache();

    progress.setValue(1);

    currentBiblePath = path;

#ifdef KOXML_USE_QDOM
    QMap<int, KoXmlElement> myCache;
#else
    //hard cache: genrate fileName
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(path.toLocal8Bit());
    QString fileName = m_settings->homePath + "cache/" + hash.result().toBase64() + "/";
    QDir dir;
    dir.mkpath(fileName);
#endif
    //
    progress.setValue(2);
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Can not read the file"));
        myDebug() << "cant read the file";
        return;
    }
    progress.setValue(3);
    KoXmlDocument doc;

#ifdef KOXML_USE_QDOM
    if (!doc.setContent(&file)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
        myDebug() << "the file isnt valid";
        return;
    }
#else
    QStringList fileList;
    QString data;
    QByteArray first = file.readLine();
    QString firstString(first);
    QString codecString;
    if (m_settings->getModuleSettings(m_bibleID).encoding == "Default" || m_settings->getModuleSettings(m_bibleID).encoding == "") {
        QRegExp rxlen("encoding=\"(.*)\"");//todo: (.*) should be without whitespaces ,too
        int pos = rxlen.indexIn(firstString);

        if (pos > -1 && !rxlen.cap(1).contains("\"")) {
            codecString = rxlen.cap(1);
        } else {
            codecString = "UTF-8";
        }
    } else {
        codecString = m_settings->getModuleSettings(m_bibleID).encoding;
    }
    qDebug() << "ZefaniaBible::loadNoCached() encoding = " << codecString;
    //if codecString is not a valid decoder name, there can be problems
#ifdef Q_WS_WIN
    //windows need some extra decoder functions, i do not know why
    if (codecString.toLower() == "utf-8") {
        while (!file.atEnd()) {
            QByteArray byteline = file.readLine();
            QString l = QString::fromLocal8Bit(byteline.constData());
            data += l;
            fileList << l;
        }
    } else {
        QTextCodec *codec = QTextCodec::codecForName(codecString.toStdString().c_str());
        QTextDecoder *decoder = codec->makeDecoder();
        while (!file.atEnd()) {
            QByteArray byteline = file.readLine();
            QString l = decoder->toUnicode(byteline);
            data += l;
            fileList << l;
        }
    }
#else
    QTextCodec *codec = QTextCodec::codecForName(codecString.toStdString().c_str());
    QTextDecoder *decoder = codec->makeDecoder();
    while (!file.atEnd()) {
        QByteArray byteline = file.readLine();
        QString l = decoder->toUnicode(byteline);
        data += l;
        fileList << l;
    }
#endif

    progress.setValue(5);
    if (!doc.setContent(data)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
        myDebug() << "the file isnt valid";
        return;
    }
    data.clear();
#endif


    progress.setValue(10);
    KoXmlElement root = doc.documentElement();
    bibleName = root.attribute("biblename", "");
    if (bibleName == "") {
        bibleName = m_settings->getModuleSettings(m_bibleID).moduleName;
    }

    KoXmlNode n = doc.documentElement().firstChild();
    int progressCounter = 10;
    int currentPos = 0;
    for (int c = 0; !n.isNull();) {
        progressCounter++;
        if (progressCounter < 76)
            progress.setValue(progressCounter);
        KoXmlElement e = n.toElement();
        if (e.attribute("bname", "") != "" || e.attribute("bnumber", "") != "") {
#ifdef KOXML_USE_QDOM
            myCache[c] = e;
            if (m_settings->getModuleSettings(m_bibleID).zefbible_hardCache == false) {
                setSoftCache(i, fromHardToSoft(c, n));
            }
#else
            //it is the caching mechanisme
            int start = 0, end = 0;
            for (int i = currentPos; i < fileList.size(); ++i) {
                QString line = fileList.at(i);
                if (line.contains("<BIBLEBOOK", Qt::CaseInsensitive)) {
                    currentPos = i + 1;
                    start = i;
                    break;
                }

            }
            for (int i = currentPos; i < fileList.size(); ++i) {
                QString line = fileList.at(i);
                if (line.contains("</BIBLEBOOK", Qt::CaseInsensitive)) {
                    currentPos = i + 1;
                    end = i;
                    break;
                }

            }
            qDebug() << "start = " << start << " end = " << end;
            QString data = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<cache>\n";
            for (int i = start; i <= end; ++i) {
                data += fileList.at(i);
            }
            data += "</cache>";
            QFile file(fileName + "/" + QString::number(c) + ".xml");
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                return;
            }
            QTextStream out(&file);
            out << data;
            file.close();
#endif
            bookFullName << e.attribute("bname", e.attribute("bsname", ""));
            bookShortName << e.attribute("bsname", QObject::tr("(unknown)"));
            c++;
        }
        n = n.nextSibling();
    }
    fileList.clear();
    bool hasAny = false;
    for (int i = 0; i < bookFullName.size(); i++) {
        if (bookFullName.at(i) != "") {
            hasAny = true;
            break;
        }
    }
    if (!hasAny) {
        qDebug() << "ZefaniaBible::loadNoCached() bookFullName.size() = " << bookFullName.size();
        //whole bible
        if (bookFullName.size() == 66) {
            bookFullName.clear();
            //load default booknames
            bookFullName = m_settings->bookNames;
        } else if (bookFullName.size() == 27) {
            bookFullName.clear();
            //load default booknames
            QStringList b = m_settings->bookNames;
            for (int v = 0; v < 39; v++) {
                b.removeFirst();
            }
            bookFullName = b;

        }
    }
    progress.hide();
    file.close();
    if (m_settings->getModuleSettings(m_bibleID).zefbible_hardCache == true) {
        //hard cache: write data
#ifdef KOXML_USE_QDOM
        //generate cache files
        QProgressDialog progressCache(QObject::tr("Generate Cache"), QObject::tr("Cancel"), 0, myCache.size() + 1);
        progressCache.setWindowModality(Qt::WindowModal);
        progressCache.setValue(1);
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(path.toLocal8Bit());
        QString fileName = m_settings->homePath + "cache/" + hash.result().toBase64();
        QDir dir(path);
        dir.mkpath(fileName + "/");

        QFile file(fileName + "/data");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QDataStream out(&file);
        out << bookFullName << bibleName;

        QMapIterator<int, KoXmlElement> i(myCache);
        int counter = 1;
        for (int counter = 1; i.hasNext(); ++counter) {
            i.next();
            QDomDocument sdoc("");
            QDomElement root = sdoc.createElement("cache");
            sdoc.appendChild(root);
            root.appendChild(i.value());

            QFile file(fileName + "/" + QString::number(i.key()) + ".xml");
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                return;
            }

            QTextStream out(&file);
            out << "<?xml version=\"1.0\"?>" + sdoc.toString();

            file.close();
            progressCache.setValue(counter);
        }
        progressCache.close();
#else
        //write the booknames in a cache file
        QFile file(fileName + "data");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return;
        }
        QDataStream out(&file);
        out << bookFullName << bibleName;
        file.close();
#endif
    }
}

/*
  load only booknames and not every book and his data
  */
void ZefaniaBible::loadCached(const int &id, const QString &path)
{
    bookFullName.clear();
    bookShortName.clear();
    bookCount.clear();
    clearSoftCache();

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(path.toLocal8Bit());
    QString fileName = m_settings->homePath + "cache/" + hash.result().toBase64() + "/";
    QFile file(fileName + "data");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QDataStream in(&file);
    QString bibleN;
    QStringList str;
    in >> str >> bibleN;
    bibleName = bibleN;
    bookFullName = str;
    m_bibleID = id;
    currentBiblePath = path;
    if (bibleName == "") {
        bibleName = m_settings->getModuleSettings(m_bibleID).moduleName;
    }
}

QString ZefaniaBible::readInfo(QFile &file)
{
    biblepath = file.fileName();
    QString cbiblename;
    KoXmlDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        return "";
    }
    KoXmlElement root = doc.documentElement();
    cbiblename = root.attribute("biblename", "");
    bibleName = cbiblename;
    file.close();
    return bibleName;
}
/*
  returns the bible name
 */
QString ZefaniaBible::readInfo(const QString &content)
{
    //todo: read only the first 20 lines and find in them the biblename
    biblepath = "";
    QString cbiblename;
    KoXmlDocument doc;
    if (!doc.setContent(content)) {
        return "";
    }
    KoXmlElement root = doc.documentElement();
    cbiblename = root.attribute("biblename", "");
    bibleName = cbiblename;
    return bibleName;
}
/*
  convert a node to a chapterlist
  */
QList<Chapter> ZefaniaBible::fromHardToSoft(int id, QDomNode ncache)
{
    QList<Chapter> ret;
    QDomNode n = ncache.firstChild();
    for (int i = 0; !n.isNull(); ++i) {
        Chapter c;
        QDomNode n2 = n.firstChild();
        int verseCount = 0;
        while (!n2.isNull()) {
            verseCount++;
            QDomElement e2 = n2.toElement();
            e2 = format(e2);
            c.data <<  e2.text();
            c.verseNumber << e2.attribute("vnumber", "");
            n2 = n2.nextSibling();
        }
        c.chapterName = QString::number(i + 1, 10);
        c.verseCount = verseCount;
        c.bookName = bookFullName.at(id);
        ret << c;
        n = n.nextSibling();
    }
    return ret;

}
/*
  struct bibleInfo ZefaniaBible::readBibleInfoFromCache(QString */
SearchResult ZefaniaBible::search(SearchQuery query)
{
    if (query.wholeWord == true) {
        query.searchText = " " + query.searchText + " ";
    }
    lastSearchQuery = query;
    SearchResult result;
    /*if(m_settings->getModuleSettings(m_bibleID).zefbible_hardCache== true)//hard cache
    {
        QProgressDialog progressCache(QObject::tr( "Read Cache" ), QObject::tr( "Cancel" ), 0, bookFullName.size()-1);
        progressCache.setWindowModality(Qt::WindowModal);
        for(int i=0;i<bookFullName.size();i++)
        {
            progressCache.setValue(i);
            if(softCacheAvi[i] == false)
            {
                softCache[i] = readBookFromHardCache(currentBiblePath,i);
                softCacheAvi[i] = true;
            }
        }
        progressCache.close();
    }*/
    //todo: read everything from the big data because its faster
    QProgressDialog progress(QObject::tr("Searching"), QObject::tr("Cancel"), 0, bookFullName.size());
    progress.setWindowModality(Qt::WindowModal);
    for (int i = 0; i < bookFullName.size(); ++i) {
        progress.setValue(i);
        if (progress.wasCanceled()) {
            return result;
        }
        QList<Chapter> chapterList;

        //load book from cache(soft or hard)
        //todo: make a check : if only 2 or 3 book are aviable in softcache then load whole book else load everything from hardcache
        //todo: cant load book if only softcache is enabled

        if (m_settings->getModuleSettings(m_bibleID).zefbible_hardCache == true && (softCacheData[i].isEmpty() || m_settings->getModuleSettings(m_bibleID).zefbible_softCache == false)) {
            chapterList = fromHardToSoft(i, readBookFromHardCache(currentBiblePath, i));
            setSoftCache(i, chapterList);
        } else {
            chapterList =  softCache(i);
        }

        for (int chapterCounter = 0; chapterCounter < chapterList.size(); ++chapterCounter) {
            Chapter c = chapterList.at(chapterCounter);
            QStringList verse = c.data;
            for (int verseCounter = 0; verseCounter < verse.size(); ++verseCounter) {
                bool b = false;
                QString etext = verse.at(verseCounter);
                if (query.regExp == true) {
                    b = etext.contains(QRegExp(query.searchText));
                } else {
                    if (query.caseSensitive == true) {
                        b = etext.contains(query.searchText, Qt::CaseSensitive);
                    } else {
                        b = etext.contains(query.searchText, Qt::CaseInsensitive);
                    }
                }
                if (b == true) {
                    result.addHit(m_bibleID, i, chapterCounter + 1, verseCounter + 1, etext);
                }
            }
        }
    }
    progress.close();
    return result;
}
