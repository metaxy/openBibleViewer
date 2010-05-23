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
#include "biblequote.h"
#include "src/core/dbghelper.h"
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QDir>
#include <QtGui/QProgressDialog>
#include <QtGui/QMessageBox>
BibleQuote::BibleQuote()
{
    m_settings = new Settings();
    m_bibleID = -1;
}

int BibleQuote::setSettings(Settings *set)
{
    m_settings = set;
    return 1;
}
QString BibleQuote::formatFromIni(QString input)
{
    input.replace(QString("\n"), QString(""));//und kein zeilenumbrüche
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
    bool started = false, started2 = false;
    int count = 0;
    QFile file;
    file.setFileName(path);
    myDebug() << "id = " << m_bibleID << " fileName = " << file.fileName() << " currentBiblePath = " << m_biblePath;
    QString encoding;
    if (m_settings->getModuleSettings(m_bibleID).encoding == "Default" || m_settings->getModuleSettings(m_bibleID).encoding == "") {
        encoding = m_settings->encoding;
    } else {
        encoding = m_settings->getModuleSettings(m_bibleID).encoding;
    }
    QTextCodec *codec = QTextCodec::codecForName(encoding.toStdString().c_str());
    QTextDecoder *decoder = codec->makeDecoder();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        int countlines = 0;
        myDebug() << "encoding = " << m_settings->encoding;
        int i = 0;
        while (!file.atEnd()) {

            countlines++;
            QByteArray byteline = file.readLine();
            QString line = decoder->toUnicode(byteline);

            if (line.contains("BibleName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                m_bibleName = formatFromIni(line.remove(QRegExp("BibleName(\\s*)=(\\s*)",Qt::CaseInsensitive)));

            }
            if (line.contains("ChapterSign", Qt::CaseInsensitive) && !line.startsWith("//")) {
                m_chapterSign = formatFromIni(line.remove(QRegExp("ChapterSign(\\s*)=(\\s*)",Qt::CaseInsensitive)));
                myDebug() << "ChapterSign = " << m_chapterSign;
                //chaptersign auslesen
            }
            if (line.contains("HTMLFilter", Qt::CaseInsensitive) && !line.startsWith("//")) {
                m_removeHtml = formatFromIni(line.remove(QRegExp("HTMLFilter(\\s*)=(\\s*)",Qt::CaseInsensitive)));
                myDebug() << "HtmlFilter = " << m_removeHtml;
                //htmlfilter auslesen
            }
            if (line.contains("VerseSign", Qt::CaseInsensitive) && !line.startsWith("//")) {
                m_verseSign = formatFromIni(line.remove(QRegExp("VerseSign(\\s*)=(\\s*)",Qt::CaseInsensitive)));
                myDebug() << "VerseSign = " << m_verseSign;
                //verse sign auslesen
            }
            if (line.contains("ChapterZero", Qt::CaseInsensitive) && !line.startsWith("//")) {
                QString a = formatFromIni(line.remove(QRegExp("ChapterZero(\\s*)=(\\s*)",Qt::CaseInsensitive)));
                if (a.compare("Y", Qt::CaseInsensitive) == 0) {
                    m_chapterZero = true;
                } else {
                    m_chapterZero = false;
                }
                myDebug() << "chapterZero = " << m_chapterZero;
                //verse sign auslesen
            }
            if (started == false && line.contains("BookQty", Qt::CaseInsensitive) && !line.startsWith("//")) {
                started = true;
            }
            if (started == true) {
                if (started2 == true) {
                    if (line.contains("ChapterQty", Qt::CaseInsensitive) && !line.startsWith("//")) {
                        m_bookCount[i] = formatFromIni(line.remove( QRegExp("ChapterQty(\\s*)=(\\s*)",Qt::CaseInsensitive))).toInt();
                        i++;
                        //chapterqty auslesen
                        started2 = false;
                    } else if (line.contains("FullName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                        m_bookFullName << formatFromIni(line.remove(QRegExp("FullName(\\s*)=(\\s*)",Qt::CaseInsensitive)));

                    } else if (line.contains("ShortName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                        m_bookShortName << formatFromIni(line.remove(QRegExp("ShortName(\\s*)=(\\s*)",Qt::CaseInsensitive)));

                    }
                } else if (line.contains("PathName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                    count++;
                    started2 = true;
                    m_bookPath << formatFromIni(line.remove(QRegExp("PathName(\\s*)=(\\s*)",Qt::CaseInsensitive)));
                    //pathname auslesen
                }
            }

        }
    }
    m_settings->setBookCount(m_bibleID, m_bookCount);
    m_settings->setBookNames(m_bibleID, m_bookFullName);
    m_settings->setBiblePath(m_bibleID, path);
    m_settings->setBibleName(m_bibleID, m_bibleName);
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
    if (m_settings->getModuleSettings(m_bibleID).encoding == "Default" || m_settings->getModuleSettings(m_bibleID).encoding == "") {
        encoding = m_settings->encoding;
    } else {
        encoding = m_settings->getModuleSettings(m_bibleID).encoding;
    }
    if (encoding == "")
        encoding = m_settings->encoding;
    QTextCodec *codec = QTextCodec::codecForName(encoding.toStdString().c_str());
    QTextDecoder *decoder = codec->makeDecoder();
    while (!file.atEnd()) {
        /*if (countlines > 50) { //wenn eine ini datei ungueltig ist soll damit nicht zuviel zeit verguedet werden
            break;
        }*/
        QByteArray byteline = file.readLine();
        QString line = decoder->toUnicode(byteline);
        if (!line.startsWith("//")) {
            countlines++;
        } else {
            continue;
        }
        if (line.contains("BibleName", Qt::CaseInsensitive)) {
            m_bibleName = formatFromIni(line.remove(QRegExp("BibleName(\\s*)=(\\s*)",Qt::CaseInsensitive)));
            if(m_bibleName == "") {
                useShortName = true;
            }
            if(useShortName && m_bibleShortName != "") {
                break;
            } else if(!useShortName) {
                break;
            }
        }
        if (line.contains("BibleShortName", Qt::CaseInsensitive)) {
            m_bibleShortName = formatFromIni(line.remove(QRegExp("BibleShortName(\\s*)=(\\s*)",Qt::CaseInsensitive)));
            if(useShortName)
                break;
        }


    }
    file.close();
    if(useShortName) {
        m_bibleName = m_bibleShortName;
    }
    if (m_bibleName.isEmpty()) {
        myDebug() << "invalid ini File " << file.fileName();
    }
    return m_bibleName;
}
int BibleQuote::readBook(int id, QString path)
{
    //DEBUG_FUNC_NAME
    //chapterText.clear();
    m_chapterData.clear();

    m_bookID = id;
    path = m_biblePath + "/" + path;

    QFile file;
    file.setFileName(path);

    QString out = "", out2;
    bool chapterstarted = false;
    int ccount2 = 0;
    QStringList chapterText;
    QStringList removeHtml2 = m_removeHtml.split(" ");
    myDebug() << "id = " << id << " path = " << path << " fileName = " << file.fileName();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString encoding;
        if (m_settings->getModuleSettings(m_bibleID).encoding == "Default" || m_settings->getModuleSettings(m_bibleID).encoding == "") {
            encoding = m_settings->encoding;
        } else {
            encoding = m_settings->getModuleSettings(m_bibleID).encoding;
        }
        myDebug() << "encoding = " << encoding;
        QTextCodec *codec = QTextCodec::codecForName(encoding.toStdString().c_str());
        QTextDecoder *decoder = codec->makeDecoder();
        while (!file.atEnd()) {
            QByteArray byteline = file.readLine();
            QString line = decoder->toUnicode(byteline);
            
            //filterout
            if (m_settings->getModuleSettings(m_bibleID).biblequote_removeHtml == true && removeHtml2.size() > 0) {
                for (int i = 0; i < removeHtml2.size(); i++) {
                    QString r = removeHtml2.at(i);
                    line = line.remove(r, Qt::CaseInsensitive);
                }
                /*line = line.remove(QRegExp("CLASS=\"(\\w+)\">"));
                line = line.remove(QRegExp("<DIV CLASS=\"(\\w+)\">"));
                line = line.remove("CLASS=\"Tx\">");*/
            }
            out2 += line;
            if (chapterstarted == false && line.contains(m_chapterSign)) {
                chapterstarted = true;
            }
            if (chapterstarted == true && line.contains(m_chapterSign)) {
                ccount2++;
                chapterText << out;
                out = line;
            } else if (chapterstarted == true) {
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

        foreach(QString f,list) {
            QFileInfo info2(f);
            if(info2.baseName().compare(info.baseName(),Qt::CaseInsensitive) == 0) {
                return readBook(id,f);
            }
        }
        return 2;//don't show a error
    }
    if (ccount2 == 0) {
        chapterText << out2;
        ccount2 = 1;
    }

    for (int i = 0; i < chapterText.size(); i++) {
        Chapter c;
        QString a = chapterText.at(i);
        QStringList b = a.split(m_verseSign);
        for(int j = 0; j < b.size(); j++) { //split removes versesign but it is needed
            QString a = b.at(j);
            b.replace(j,a.prepend(m_verseSign));
        }
        c.data << b;
        m_chapterData << c;
    }
    file.close();
    return 0;

}

SearchResult BibleQuote::search(SearchQuery query)
{
    //DEBUG_FUNC_NAME
    if (query.wholeWord == true) {
        query.searchText = " " + query.searchText + " ";
    }
    QStringList ctext;
    QList<QByteArray> bytetext;
    QProgressDialog progress(QObject::tr("Searching"), QObject::tr("Cancel"), 0, m_bookPath.size());
    progress.setWindowModality(Qt::WindowModal);
    SearchResult result;
    result.searchQuery = query;
    for (int id = 0; id < m_bookPath.size(); id++) {
        if (progress.wasCanceled())
            return result;
        progress.setValue(id);
        bytetext.clear();
        ctext.clear();
        QString path = m_bookPath.at(id);
        path = m_biblePath + "/" + path;
        QFile file(path);
        QByteArray out = "", out2 = "";
        bool chapterstarted = false;
        int ccount2 = 0;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {

            while (!file.atEnd()) {
                QByteArray byteline = file.readLine();
                QString line(byteline);
                out2 += byteline;
                if (chapterstarted == false && line.contains(m_chapterSign)) {
                    chapterstarted = true;
                }
                if (chapterstarted == true && line.contains(m_chapterSign)) {
                    ccount2++;
                    bytetext << out;
                    out = byteline;
                } else if (chapterstarted == true) {
                    out += byteline;
                }
            }
            bytetext << out;
        } else {
            myDebug() << "cannot open the file " << file.fileName();
            continue;
            //return result;
        }
        QString encoding;
        if (m_settings->getModuleSettings(m_bibleID).encoding == "Default" || m_settings->getModuleSettings(m_bibleID).encoding == "") {
            encoding = m_settings->encoding;
        } else {
            encoding = m_settings->getModuleSettings(m_bibleID).encoding;
        }
        QTextCodec *codec = QTextCodec::codecForName(encoding.toStdString().c_str());
        QTextDecoder *decoder = codec->makeDecoder();
        if (ccount2 == 0) {
            ctext << decoder->toUnicode(out2);
            ccount2 = 1;
        } else {
            for (int i = 0; i < bytetext.size(); i++) {
                ctext << decoder->toUnicode(bytetext.at(i));
            }
        }
        myDebug() << "ctext.size() = " << ctext.size();
        for (int chapterit = 0; chapterit < ctext.size(); chapterit++) {
            QStringList verses = ctext[chapterit].split(m_verseSign);
            for (int verseit = 0; verseit < verses.size(); ++verseit) {
                QString t = verses.at(verseit);
                bool b2;
                if (query.regExp == true) {
                    b2 = t.contains(QRegExp(query.searchText));
                } else {
                    if (query.caseSensitive == true) {
                        b2 = t.contains(query.searchText, Qt::CaseSensitive);
                    } else {
                        b2 = t.contains(query.searchText, Qt::CaseInsensitive);
                    }
                }
                if (b2) {
                    result.addHit(m_bookID, id, chapterit, verseit, t);
                }
            }
        }
    }
    progress.close();
    return result;

}
