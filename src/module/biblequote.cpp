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
}

int BibleQuote::setSettings(Settings *set)
{
    m_settings = set;
    return 1;
}
QString BibleQuote::formatFromIni(QString input)
{
    if (input.startsWith(" ")) { //leerzeichen am anfang
        input.remove(0, 1);//remove
    }
    input.replace(QString("\n"), QString(""));//und kein zeilenumbrüche
    return input;
}
void BibleQuote::loadBibleData(int bibleID, QString path)
{
    //DEBUG_FUNC_NAME
    m_bibleID = bibleID;
    bookFullName.clear();
    bookPath.clear();
    bookShortName.clear();
    bookCount.clear();
    bibleName = "";
    chaptersign = "";
    removeHtml = "";
    versesign = "";
    chapterZero = false;
    int lastPos = path.lastIndexOf("/");
    QString path_ = path;
    currentBiblePath = path_.remove(lastPos, path.size());
    bool started = false, started2 = false;
    int count = 0;
    QFile file;
    file.setFileName(path);
    myDebug() << "id = " << m_bibleID << " fileName = " << file.fileName() << " currentBiblePath = " << currentBiblePath;
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
                bibleName = formatFromIni(line.remove("BibleName =", Qt::CaseInsensitive));
            }
            if (line.contains("ChapterSign", Qt::CaseInsensitive) && !line.startsWith("//")) {
                chaptersign = formatFromIni(line.remove("ChapterSign =", Qt::CaseInsensitive));
                myDebug() << "ChapterSign = " << chaptersign;
                //chaptersign auslesen
            }
            if (line.contains("HTMLFilter", Qt::CaseInsensitive) && !line.startsWith("//")) {
                removeHtml = formatFromIni(line.remove("HTMLFilter =", Qt::CaseInsensitive));
                myDebug() << "HtmlFilter = " << removeHtml;
                //htmlfilter auslesen
            }
            if (line.contains("VerseSign", Qt::CaseInsensitive) && !line.startsWith("//")) {
                versesign = formatFromIni(line.remove("VerseSign =", Qt::CaseInsensitive));
                myDebug() << "VerseSign = " << versesign;
                //verse sign auslesen
            }
            if (line.contains("ChapterZero", Qt::CaseInsensitive) && !line.startsWith("//")) {
                QString a = formatFromIni(line.remove("ChapterZero =", Qt::CaseInsensitive));
                if (a.contains("Y", Qt::CaseInsensitive)) {
                    chapterZero = true;
                } else {
                    chapterZero = false;
                }
                myDebug() << "chapterZero = " << chapterZero;
                //verse sign auslesen
            }
            if (started == false && line.contains("BookQty", Qt::CaseInsensitive) && !line.startsWith("//")) {
                started = true;
            }
            if (started == true) {
                if (started2 == true) {
                    if (line.contains("ChapterQty", Qt::CaseInsensitive) && !line.startsWith("//")) {
                        bookCount[i] = formatFromIni(line.remove("ChapterQty =", Qt::CaseInsensitive)).toInt();
                        i++;
                        //chapterqty auslesen
                        started2 = false;
                    } else if (line.contains("FullName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                        bookFullName << formatFromIni(line.remove("FullName =", Qt::CaseInsensitive));

                    } else if (line.contains("ShortName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                        bookShortName << formatFromIni(line.remove("ShortName =", Qt::CaseInsensitive));

                    }
                } else if (line.contains("PathName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                    count++;
                    started2 = true;
                    bookPath << formatFromIni(line.remove("PathName =", Qt::CaseInsensitive));
                    //pathname auslesen
                }
            }

        }
    }
    m_settings->setBookCount(m_bibleID, bookCount);
    m_settings->setBookNames(m_bibleID, bookFullName);
    m_settings->setBiblePath(m_bibleID, path);
    m_settings->setBibleName(m_bibleID, bibleName);
}
QString BibleQuote::readInfo(QFile &file)
{
    int countlines = 0;
    int invalid = true;
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
        if (countlines > 50) { //wenn eine ini datei ungueltig ist soll damit nicht zuviel zeit verguedet werden
            break;
        }
        QByteArray byteline = file.readLine();
        QString line = decoder->toUnicode(byteline);
        if (!line.startsWith("//")) {
            countlines++;
        } else {
            continue;
        }
        if (line.contains("BibleName", Qt::CaseInsensitive)) {
            bibleName = formatFromIni(line.remove("BibleName =", Qt::CaseInsensitive));//todo: use regexp
            if (line.contains("BibleName=", Qt::CaseInsensitive)) {
                bibleName = formatFromIni(line.remove("BibleName=", Qt::CaseInsensitive));
            }
            invalid = false;
            break;
        }

    }
    if (invalid == true) {
        myDebug() << "invalid ini File " << file.fileName();
    }
    file.close();
    return bibleName;
}
int BibleQuote::readBook(int id, QString path)
{
    //DEBUG_FUNC_NAME
    //chapterText.clear();
    chapterData.clear();

    m_bookID = id;
    path = currentBiblePath + "/" + path;

    QFile file;
    file.setFileName(path);
    QString out = "", out2;
    bool chapterstarted = false;
    int ccount2 = 0;
    QStringList chapterText;
    QStringList removeHtml2 = removeHtml.split(" ");
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
            //todo: ugly
            line = line.remove(QRegExp("CLASS=\"(\\w+)\">"));
            line = line.remove(QRegExp("<DIV CLASS=\"(\\w+)\">"));
            line = line.remove("CLASS=\"Tx\">");
            //filterout
            if (m_settings->getModuleSettings(m_bibleID).biblequote_removeHtml == true && removeHtml2.size() > 0) {
                myDebug() << "removing real html";
                for (int i = 0; i < removeHtml2.size(); i++) {
                    QString r = removeHtml2.at(i);
                    //myDebug() << removeHtml2.at(i);
                    line = line.remove(r, Qt::CaseInsensitive);
                }
            }
            out2 += line;
            if (chapterstarted == false && line.contains(chaptersign)) {
                chapterstarted = true;
            }
            if (chapterstarted == true && line.contains(chaptersign)) {
                ccount2++;
                chapterText << out;
                out = line;
            } else if (chapterstarted == true) {
                out += line;
            }
        }
        chapterText << out;
    } else {
        return 1;
    }
    if (ccount2 == 0) {
        chapterText << out2;
        ccount2 = 1;
    }

    for (int i = 0; i < chapterText.size(); i++) {
        Chapter c;
        QString a = chapterText.at(i);
        QStringList b = a.split(versesign);
        c.data << b;
        chapterData << c;
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
    lastSearchQuery = query;
    QStringList ctext;
    QList<QByteArray> bytetext;
    QProgressDialog progress(QObject::tr("Searching"), QObject::tr("Cancel"), 0, bookPath.size());
    progress.setWindowModality(Qt::WindowModal);
    SearchResult result;
    result.searchQuery = query;
    for (int id = 0; id < bookPath.size(); id++) {
        if (progress.wasCanceled())
            return result;
        progress.setValue(id);
        bytetext.clear();
        ctext.clear();
        QString path = bookPath.at(id);
        path = currentBiblePath + "/" + path;
        QFile file(path);
        QByteArray out = "", out2 = "";
        bool chapterstarted = false;
        int ccount2 = 0;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {

            while (!file.atEnd()) {
                QByteArray byteline = file.readLine();
                QString line(byteline);
                out2 += byteline;
                if (chapterstarted == false && line.contains(chaptersign)) {
                    chapterstarted = true;
                }
                if (chapterstarted == true && line.contains(chaptersign)) {
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
            QStringList verses = ctext[chapterit].split(versesign);
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
