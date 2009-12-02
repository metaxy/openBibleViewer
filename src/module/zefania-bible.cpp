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
zefaniaBible::zefaniaBible()
{
}
int zefaniaBible::setSettings(struct settings_s settings , struct moduleConfig mConfig_)
{
    qDebug() << "zefaniaBible::setSettings";
    zefset = settings;
    mConfig = mConfig_;
    return 1;
}
void zefaniaBible::loadBibleData(const int &id, const QString &path)
{
    if (mConfig.zefbible_hardCache == false && mConfig.zefbible_softCache == false) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Please activate Caching.(Hard or Soft Cache)"));
    }
    if (mConfig.zefbible_hardCache == true) {
        if (checkForCacheFiles(path)) {
            loadCached(id, path);//load the hard cache files
        } else {
            loadNoCached(id, path);//read the entire xml file
        }

    } else {
        loadNoCached(id, path);//read the entire xml file
    }

}
void zefaniaBible::removeHardCache(const QString &path)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(path.toLocal8Bit());
    QDir d(zefset.homePath + "cache/");
    d.rmdir(zefset.homePath + "cache/" + hash.result().toBase64());
}

QDomNode zefaniaBible::readBookFromHardCache(QString path, int bookID)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(path.toLocal8Bit());
    QString fileName = zefset.homePath + "cache/" + hash.result().toBase64() + "/";
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
    if (softCacheAvi[bookID] == false && mConfig.zefbible_softCache == true) {
        /*  softCache[bookID] = e;
        softCacheAvi[bookID] = true;*/
    }

    //qDebug() << "zefania::readBookFromHardCache()" << doc.toString();
    return root.firstChild();

}
void zefaniaBible::readBook(const int &id)
{
    myDebug() << "hardCache = " << mConfig.zefbible_hardCache  << " softCache = " << mConfig.zefbible_softCache << " cacheAviable = " << softCacheAvi;
    QDomNode ncache;
    if (mConfig.zefbible_hardCache == true && (softCacheAvi[id] == false || mConfig.zefbible_softCache == false)) {
        ncache = readBookFromHardCache(currentBiblePath, id);
    } else {
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
    myDebug() << "chapterData.size() = " << chapterData.size();
}
QMap<int, QList<Chapter> > zefaniaBible::softCache()
{
    if (mConfig.zefbible_softCache == true) {
        return softCacheData;
    }
    return QMap<int, QList<Chapter> >();
}
QList<Chapter> zefaniaBible::softCache(int bookID)
{
    if (mConfig.zefbible_softCache == true) {
        return softCacheData[bookID];
    }
    return QList<Chapter>();
}
bool zefaniaBible::setSoftCache(QMap< int, QList<Chapter> > cache)
{
    DEBUG_FUNC_NAME
    if (mConfig.zefbible_softCache == true) {
        softCacheData.clear();
        softCacheAvi.clear();
        softCacheData = cache;
        QMapIterator< int, QList<Chapter> > i(cache);
        while (i.hasNext()) {
            softCacheAvi[i.key()] = true;
            i.next();
        }
    }
    return true;
}
bool zefaniaBible::setSoftCache(int bookID, QList<Chapter> c)
{
    qDebug() << "zefaniaBible::setSoftCache() softCache = " << mConfig.zefbible_softCache << " , bookID = " << bookID;
    if (mConfig.zefbible_softCache == true) {
        softCacheData[bookID] = c;
        softCacheAvi[bookID] = true;
    }
    return true;
}
bool zefaniaBible::clearSoftCache()
{
    qDebug() << "zefaniaBible::clearSoftCache()";
    softCacheData.clear();
    softCacheAvi.clear();
    return true;
}

QDomElement zefaniaBible::format(QDomElement e)
{
    QDomNode n = e.firstChild();
    while (!n.isNull()) { //all verses
        if (n.nodeName().toLower() == "note") {
            QDomNode node = n;
            QDomText t = node.firstChild().toText();
            if (mConfig.zefbible_showStudyNote == true && n.toElement().attribute("type", "") == "x-studynote") {
                t.setData("[<font size=\"-1\" <i>" + t.data() + "</i> </font>]");
            } else {
                t.setData("");
            }
            node.replaceChild(t, node.firstChild());
            e.replaceChild(node, n);
        } else if ((n.nodeName().toLower() == "gram" || n.nodeName().toLower() == "gr") && n.toElement().attribute("str", "") != "" && mConfig.zefbible_showStrong == true) {
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
bool zefaniaBible::checkForCacheFiles(const QString &path)
{
    myDebug() << "path = " << path;
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(path.toLocal8Bit());
    QString fileName = zefset.homePath + "cache/" + hash.result().toBase64() + "/";
    QFile file(fileName + "data");
    if (file.exists())
        return true;
    return false;
}
/*
  read the entire xml file
  and if caching is enabled, generate cache files
  */
void zefaniaBible::loadNoCached(const int &id, const QString &path)
{
    //todo: maybe i should remove the qdom support because koxml works really fine
    DEBUG_FUNC_NAME
    QProgressDialog progress(QObject::tr("Loading Bible"), QObject::tr("Cancel"), 0, 76);
    progress.setWindowModality(Qt::WindowModal);

    bibleID = id;
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
    QString fileName = zefset.homePath + "cache/" + hash.result().toBase64() + "/";
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
    if(mConfig.encoding == "Default" || mConfig.encoding == "")
    {
        QRegExp rxlen("encoding=\"(.*)\"");//todo: (.*) should be without whitespaces ,too
        int pos = rxlen.indexIn(firstString);

        if (pos > -1 && !rxlen.cap(1).contains("\"")) {
            codecString = rxlen.cap(1);
        } else {
            codecString = "UTF-8";
        }
    }
    else
    {
        codecString = mConfig.encoding;
    }
    qDebug() << "zefaniaBible::loadNoCached() encoding = " << codecString;
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
            if (mConfig.zefbible_hardCache == false) {
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
        qDebug() << "zefaniaBible::loadNoCached() bookFullName.size() = " << bookFullName.size();
        //whole bible
        if (bookFullName.size() == 66) {
            bookFullName.clear();
            //load default booknames
            bookFullName = zefset.bookNames;
        } else if (bookFullName.size() == 27) {
            bookFullName.clear();
            //load default booknames
            QStringList b = zefset.bookNames;
            for (int v = 0; v < 39; v++) {
                b.removeFirst();
            }
            bookFullName = b;

        }
    }
    progress.hide();
    file.close();
    if (mConfig.zefbible_hardCache == true) {
        //hard cache: write data
#ifdef KOXML_USE_QDOM
        //generate cache files
        QProgressDialog progressCache(QObject::tr("Generate Cache"), QObject::tr("Cancel"), 0, myCache.size() + 1);
        progressCache.setWindowModality(Qt::WindowModal);
        progressCache.setValue(1);
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(path.toLocal8Bit());
        QString fileName = zefset.homePath + "cache/" + hash.result().toBase64();
        QDir dir(path);
        dir.mkpath(fileName + "/");

        QFile file(fileName + "/data");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QDataStream out(&file);
        out << bookFullName;

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
        out << bookFullName;
        file.close();
#endif
    }
}

/*
  load only booknames and not every book and his data
  */
void zefaniaBible::loadCached(const int &id, const QString &path)
{
    bookFullName.clear();
    bookShortName.clear();
    bookCount.clear();
    clearSoftCache();

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(path.toLocal8Bit());
    QString fileName = zefset.homePath + "cache/" + hash.result().toBase64() + "/";
    QFile file(fileName + "data");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QDataStream in(&file);
    QStringList str;
    in >> str;
    bookFullName = str;
    bibleID = id;
    currentBiblePath = path;
}

QString zefaniaBible::readInfo(QFile &file)
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
QString zefaniaBible::readInfo(const QString &content)
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
QList<Chapter> zefaniaBible::fromHardToSoft(int id, QDomNode ncache)
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
  struct bibleInfo zefaniaBible::readBibleInfoFromCache(QString */
struct stelle zefaniaBible::search(struct searchQuery query) {
    if (query.whole == true) {
        query.text = " " + query.text + " ";
    }
    lastSearch = query.text;
    struct stelle st2;
    /*if(mConfig.zefbible_hardCache== true)//hard cache
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
            return st2;
        }
        QList<Chapter> chapterList;

        //load book from cache(soft or hard)
        //todo: make a check : if only 2 or 3 book are aviable in softcache then load whole book else load everything from hardcache
        //todo: cant load book if only softcache is enabled

        if (mConfig.zefbible_hardCache == true && (softCacheAvi[i] == false || mConfig.zefbible_softCache == false)) {
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
                if (query.regexp == true) {
                    b = etext.contains(QRegExp(query.text));
                } else {
                    if (query.caseSensitive == true) {
                        b = etext.contains(query.text, Qt::CaseSensitive);
                    } else {
                        b = etext.contains(query.text, Qt::CaseInsensitive);
                    }
                }
                if (b == true) {
                    st2.book << i;
                    st2.chapter << chapterCounter + 1;
                    st2.verse << verseCounter + 1;
                    st2.text << etext;
                }
            }
        }
    }
    progress.close();
    return st2;
}
