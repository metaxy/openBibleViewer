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
}
ZefaniaBible::~ZefaniaBible()
{
    DEBUG_FUNC_NAME
    //delete m_versification;
}
void ZefaniaBible::loadBibleData(const int &id, const QString &path)
{
    //DEBUG_FUNC_NAME
    m_moduleName = "";
    m_moduleID = id;
    /*if(m_versification != NULL)
        delete m_versification;
    m_versification = NULL;*/
    if(m_settings->getModuleSettings(m_moduleID).zefbible_hardCache == false && m_settings->getModuleSettings(m_moduleID).zefbible_softCache == false) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Please activate Caching.(Hard or Soft Cache)"));
        return;
    }
    m_uid = path;
    if(m_settings->getModuleSettings(m_moduleID).zefbible_hardCache == true) {
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
    //DEBUG_FUNC_NAME
    QDomElement e;

    const QString pre = m_settings->homePath + "cache/" + m_settings->hash(path) + "/";

    QFile file(pre + QString::number(bookID) + ".xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot read the file."));
        myWarning() << "can't read the file" << pre + QString::number(bookID) + ".xml";
        return e;
    }
    QDomDocument doc;
    if(!doc.setContent(&file)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid."));
        myWarning() << "the file isn't valid";
        return e;
    }
    QDomElement root = doc.documentElement();
    return root.firstChild();

}
int ZefaniaBible::readBook(const int &id)
{
    QDomNode ncache;
    //book is not in soft cache
    if(m_settings->getModuleSettings(m_moduleID).zefbible_hardCache == true &&
            (!m_softCacheData.contains(id) || m_settings->getModuleSettings(m_moduleID).zefbible_softCache == false)) {
        ncache = readBookFromHardCache(m_modulePath, id);
    } else {
        m_book = softCache(id);
        ((Versification_Zefania*)m_versification)->setMaxChapter(id, m_book.size());
        return 0;
    }
    //reading loaded data
    m_book.clear();
    m_bookID = id;
    m_book.setID(m_bookID);

    QDomNode n = ncache.firstChild();
    QString outtext;
    int chapterCounter;
    for(chapterCounter = 0; !n.isNull(); ++chapterCounter) {
        Chapter c(chapterCounter);
        outtext = "";
        QDomNode n2 = n.firstChild();
        int verseCount = 0;
        while(!n2.isNull()) {  //alle verse
            QDomElement e2 = n2.toElement();
            format(&e2);
            if(e2.tagName().toLower() == "vers") { // read only verse
                const int verseID = e2.attribute("vnumber", QString::number(verseCount + 1)).toInt() - 1;
                Verse verse(verseID, e2.text());
                c.addVerse(verseID, verse);
                verseCount++;
            } //todo: all other data
            n2 = n2.nextSibling();
        }
        m_book.addChapter(chapterCounter, c);

        n = n.nextSibling();
    }

    ((Versification_Zefania*)m_versification)->setMaxChapter(id, m_book.size());
    setSoftCache(m_bookID, m_book);
    return 0;
}
/**
  Convert a node into a chapterlist.
  \param bookID The bookID.
  \param ncache The node to convert.
  */
Book ZefaniaBible::fromHardToSoft(const int &bookID, const QDomNode *ncache)
{
    Book book;
    book.setID(bookID);
    QDomNode n = ncache->firstChild();
    int chapterCounter;
    for(chapterCounter = 0; !n.isNull(); ++chapterCounter) {
        Chapter c(chapterCounter);
        QDomNode n2 = n.firstChild();
        int verseCount = 0;
        while(!n2.isNull()) {  //alle verse
            QDomElement e2 = n2.toElement();
            format(&e2);
            if(e2.tagName().toLower() == "vers") { // read only verse
                const int verseID = e2.attribute("vnumber", QString::number(verseCount + 1)).toInt() - 1;
                Verse verse(verseID, e2.text());
                c.addVerse(verseID, verse);
                verseCount++;
            } //todo: all other data
            n2 = n2.nextSibling();
        }
        book.addChapter(chapterCounter, c);

        n = n.nextSibling();
    }
    return book;
}
QDomElement* ZefaniaBible::format(QDomElement *e)
{
    ModuleSettings moduleSettings = m_settings->getModuleSettings(m_moduleID);
    QDomNode n = e->firstChild();
    while(!n.isNull()) {  //all verses
        QDomElement element = n.toElement();
        if(n.nodeName().toLower() == "note") {
            QDomNode node = n;

            QDomText t = node.firstChild().toText();
            if(moduleSettings.zefbible_showStudyNote == true && element.attribute("type", "") == "x-studynote") {
                t.setData("<span class =\"studynote\">" + t.data() + "</span>");
            } else {
                t.setData("");
            }
            node.replaceChild(t, node.firstChild());
            e->replaceChild(node, n);
        } else if(moduleSettings.zefbible_showStrong == true && (n.nodeName().toLower() == "gram" || n.nodeName().toLower() == "gr") && element.attribute("str", "") != "") {
            QDomNode node = n;
            QDomText t = n.firstChild().toText();
            QString add;
            //todo: that isn't  nice
            if(m_bookID < 39)
                add = "H";
            else
                add = "G";

            t.setData(t.data() + "<span class=\"gramlink\"><a href=\"gram://" + add + element.attribute("str", "") + "\">" + add + element.attribute("str", "") + "</a></span>");
            node.replaceChild(t, node.firstChild());
            e->replaceChild(node, n);
        } else if(n.nodeName().toLower() == "reflink") {
            QDomNode node = n;
            QDomText t = n.firstChild().toText();

            QString mscope = element.attribute("mscope", ";;;");
            const QStringList list = mscope.split(";");
            const int bookID = list.at(0).toInt() - 1;
            const int chapterID = list.at(1).toInt() - 1;
            const int verseID = list.at(2).toInt() - 1;


            VerseUrlRange range;
            range.setModule(VerseUrlRange::LoadCurrentModule);
            range.setBook(bookID);
            range.setChapter(chapterID);
            range.setWholeChapter();
            range.setActiveVerse(verseID);
            VerseUrl burl(range);

            QString name;
            if(bookID < m_settings->defaultVersification->bookNames(Versification::ReturnAll).size()) {
                name = m_settings->defaultVersification->bookNames(Versification::ReturnAll).value(bookID) + " " + list.at(1) + "," + list.at(2);
            } else {
                name = list.at(0) + " " + list.at(1) + "," + list.at(2);
            }
            t.setData("<span class=\"crossreference\"><a class=\"reflink\" href=\"" + burl.toString() + "\">" + name + "</a></span>");
            node.replaceChild(t, node.firstChild());
            e->replaceChild(node, n);
        }
        if(n.childNodes().count() > 0) {
            const QDomNode oldChild = n;
            QDomElement *el = format(&element);
            e->replaceChild(*el, oldChild);
        }
        n = n.nextSibling();
    }
    return e;

}
/**
  Returns the soft cache for all books.
  */
QHash<int, Book> ZefaniaBible::softCache() const
{
    if(m_settings->getModuleSettings(m_moduleID).zefbible_softCache == true) {
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
    if(m_settings->getModuleSettings(m_moduleID).zefbible_softCache == true) {
        return m_softCacheData.value(bookID);
    }
    return Book();
}
/**
  Set the soft cache
  \param QMap<int, QList<Chapter> > cache The cache data.
  */
void ZefaniaBible::setSoftCache(const QHash<int, Book > &cache)
{
    DEBUG_FUNC_NAME
    if(m_settings->getModuleSettings(m_moduleID).zefbible_softCache == true) {
        m_softCacheData = cache;
    }
    return;
}
/**
  Sets the cache of a book.
  \param bookID The ID of the book.
  \param chapterList New cache.
  */
void ZefaniaBible::setSoftCache(const int &bookID, const Book &book)
{
    if(m_settings->getModuleSettings(m_moduleID).zefbible_softCache == true) {
        m_softCacheData.insert(bookID, book);
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
bool ZefaniaBible::checkForCacheFiles(const QString &path) const
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
    if(m_moduleID != id) {
        clearSoftCache();
    }
    QStringList fullName;
    QStringList shortName;
    QStringList bookIDs;
    m_moduleID = id;
    ModuleSettings moduleSettings = m_settings->getModuleSettings(m_moduleID);
    progress.setValue(1);

    m_modulePath = path;
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
    //if codecString is not a valid decoder name, there can be a crash
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
    m_moduleName = root.attribute("biblename", "");
    if(m_moduleName == "") {
        m_moduleName = moduleSettings.moduleName;
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
            fullName << e.attribute("bname", e.attribute("bsname", ""));
            shortName << e.attribute("bsname", "");
            bookIDs << bookID;
            c++;
        }
        n = n.nextSibling();
    }
    fileList.clear();
    bool hasAny = false;
    for(int i = 0; i < fullName.size(); i++) {
        if(fullName.at(i) != "") {
            hasAny = true;
            break;
        }
    }
    if(!hasAny) {
        //whole bible
        /* if(m_bookFullName.size() == 66) {
             m_bookFullName = m_settings->defaultVersification->bookNames(Versification::ReturnNT | Versification::ReturnOT);
         } else if(m_bookFullName.size() == 27) {
             m_bookFullName = m_settings->defaultVersification->bookNames(Versification::ReturnNT);
         } */
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
        out << m_moduleName << fullName << shortName << bookIDs;
        file.close();
    }
}

/**
  load only booknames and not every book and his data
  */
void ZefaniaBible::loadCached(const int &id, const QString &path)
{
    if(m_moduleID != id) {
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
    m_moduleName = bibleName;
    m_versification = new Versification_Zefania(fullName, shortName, bookIDs);
    //myDebug() << m_bookShortName;
    m_moduleID = id;
    m_modulePath = path;
    if(m_moduleName == "") {
        m_moduleName = m_settings->getModuleSettings(m_moduleID).moduleName;
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
    m_moduleName = root.attribute("biblename", "");
    file.close();
    return m_moduleName;
}
/**
  Read the module file and returns the bible name
  \param fileName The fileName of the module.
 */
QString ZefaniaBible::readInfo(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return "";
    return readInfo(file);
}
/*int ZefaniaBible::bookID() const
{
    return m_bookID;
}*/
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
    for(int bookCounter = 0; bookCounter < m_versification->bookCount(); ++bookCounter) {
        if(progress.wasCanceled()) {
            canceled = true;
            break;
        }
        progress.setValue(bookCounter);
        QDomNode ncache;
        ncache = readBookFromHardCache(m_modulePath, bookCounter);
        QDomNode n = ncache.firstChild();
        int chapterCounter;
        const QString book = QString::number(bookCounter);
        for(chapterCounter = 0; !n.isNull(); ++chapterCounter) {
            QDomNode n2 = n.firstChild();
            int verseCount = 0;
            const QString chapter = QString::number(chapterCounter);
            while(!n2.isNull()) {  //all verse
                verseCount++;
                QDomElement e2 = n2.toElement();
                if(e2.tagName().toLower() == "vers") { // read only verse
                    doc.clear();
                    const QString t = e2.text();
                    const QString verse = QString::number(e2.attribute("vnumber", QString::number(verseCount)).toInt() - 1);

                    QString key = book + ";" + chapter + ";" + verse;
                    doc.add(*new Field(_T("key"), key.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_NO));
                    doc.add(*new Field(_T("content"), t.toStdWString().c_str(), Field::STORE_YES |  Field::INDEX_TOKENIZED));

                    writer->addDocument(&doc);
                }
                n2 = n2.nextSibling();
            }
            n = n.nextSibling();
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
void ZefaniaBible::search(const SearchQuery &query, SearchResult *res) const
{
    DEBUG_FUNC_NAME
    const QString index = indexPath();
    //myDebug() << " index = " << index;
    const TCHAR* stop_words[] = { NULL };
    standard::StandardAnalyzer analyzer(stop_words);
    IndexReader* reader = IndexReader::open(index.toStdString().c_str());
    IndexSearcher s(reader);
    Query* q = QueryParser::parse(query.searchText.toStdWString().c_str(), _T("content"), &analyzer);
    Hits* h = s.search(q);
    //myDebug() << "query string = " << q->toString();
    for(size_t i = 0; i < h->length(); i++) {
        Document* doc = &h->doc(i);
        const QString stelle = QString::fromWCharArray(doc->get(_T("key")));
        //myDebug() << "found stelle = " << stelle;
        const QStringList l = stelle.split(";");
        //hacky filter
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
        myWarning() << "book not loaded";
        return ret;
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
    while (i.hasNext()) {
        i.next();
        if(i.key() <= endVerse && i.key() >= startVerse)
            ret.addVerse(i.value());
    }
    return ret;
}

std::pair<int,int> ZefaniaBible::minMaxVerse(int bookID, int chapterID)
{
    std::pair<int, int> ret;
    if(m_book.bookID() != bookID) {
        myWarning() << "book not loaded";
        return ret;
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
