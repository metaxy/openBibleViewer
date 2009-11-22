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
#include "../core/stelle.h"
#include "../core/dbghelper.h"
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QDir>
#include <QtGui/QProgressDialog>
biblequote::biblequote()
{
}

int biblequote::setSettings(struct settings_s settings)
{
    bqset = settings;
    return 1;
}
QString biblequote::formatfromini(QString input)
{
    if (input.startsWith(" ")) { //leerzeichen am anfang
        input.remove(0, 1);//remove
    }
    input.replace(QString("\n"), QString(""));//und kein zeilenumbrüche
    return input;
}
void biblequote::readBook(int id, QString path)
{
    DEBUG_FUNC_NAME
    myDebug() << "id = " << id;
    //chapterText.clear();
    chapterData.clear();

    currentBookID = id;
    path = currentBiblePath + "/" + path;

    QFile file;
    file.setFileName(path);
    QString out = "", out2;
    bool chapterstarted = false;
    int ccount2 = 0;
    QStringList chapterText;
    QStringList removeHtml2 = removeHtml.split(" ");
    QList<QByteArray> bytes;
    myDebug() << "id = " << id << " path = " << path << " fileName = " << file.fileName();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextCodec *codec = QTextCodec::codecForName(bqset.encoding.toStdString().c_str());
        QTextDecoder *decoder = codec->makeDecoder();
        while (!file.atEnd()) {
            QByteArray byteline = file.readLine();
            QString line = decoder->toUnicode(byteline);
            line = line.remove(QRegExp("CLASS=\"(\\w+)\">"));
            line = line.remove(QRegExp("<DIV CLASS=\"(\\w+)\">"));
            line = line.remove("CLASS=\"Tx\">");
            //filterout
            if (bqset.module.at(bqset.moduleID[currentBibleID]).biblequote_removeHtml == true && removeHtml.size() > 0) {
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
    }
    if (ccount2 == 0) {
        chapterText << out2;
        ccount2 = 1;
    }

    for (int i = 0; i < chapterText.size(); i++) {
        chapter c;
        QString a = chapterText.at(i);
        QStringList b = a.split(versesign);
        c.data << b;
        chapterData << c;
    }
    file.close();

}
void biblequote::loadBibleData(int bibleID, QString path)
{
    DEBUG_FUNC_NAME
    currentBibleID = bibleID;
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
    myDebug() << "id = " << currentBibleID << " fileName = " << file.fileName() << " currentBiblePath = " << currentBiblePath;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString out = "";
        int countlines = 0;
        myDebug() << "encoding = " << bqset.encoding;
        QTextCodec *codec = QTextCodec::codecForName(bqset.encoding.toStdString().c_str());
        QTextDecoder *decoder = codec->makeDecoder();
        int i = 0;
        while (!file.atEnd()) {

            countlines++;
            QByteArray byteline = file.readLine();
            QString line = decoder->toUnicode(byteline);

            if (line.contains("BibleName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                bibleName = formatfromini(line.remove("BibleName =", Qt::CaseInsensitive));
            }
            if (line.contains("ChapterSign", Qt::CaseInsensitive) && !line.startsWith("//")) {
                chaptersign = formatfromini(line.remove("ChapterSign =", Qt::CaseInsensitive));
                //chaptersign auslesen
            }
            if (line.contains("HTMLFilter", Qt::CaseInsensitive) && !line.startsWith("//")) {
                removeHtml = formatfromini(line.remove("HTMLFilter =", Qt::CaseInsensitive));
                //htmlfilter auslesen
            }
            if (line.contains("VerseSign", Qt::CaseInsensitive) && !line.startsWith("//")) {
                versesign = formatfromini(line.remove("VerseSign =", Qt::CaseInsensitive));
                //verse sign auslesen
            }
            if (line.contains("ChapterZero", Qt::CaseInsensitive) && !line.startsWith("//")) {
                QString a = formatfromini(line.remove("ChapterZero =", Qt::CaseInsensitive));
                if (a.contains("Y", Qt::CaseInsensitive)) {
                    chapterZero = true;
                } else {
                    chapterZero = false;
                }
                //verse sign auslesen
            }
            if (started == false && line.contains("BookQty", Qt::CaseInsensitive) && !line.startsWith("//")) {
                started = true;
            }
            if (started == true) {
                if (started2 == true) {
                    if (line.contains("ChapterQty", Qt::CaseInsensitive) && !line.startsWith("//")) {
                        bookCount[i] = formatfromini(line.remove("ChapterQty =", Qt::CaseInsensitive)).toInt();
                        i++;
                        //chapterqty auslesen
                        started2 = false;
                    } else if (line.contains("FullName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                        bookFullName << formatfromini(line.remove("FullName =", Qt::CaseInsensitive));

                    } else if (line.contains("ShortName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                        bookShortName << formatfromini(line.remove("ShortName =", Qt::CaseInsensitive));

                    }
                } else if (line.contains("PathName", Qt::CaseInsensitive) && !line.startsWith("//")) {
                    count++;
                    started2 = true;
                    bookPath << formatfromini(line.remove("PathName =", Qt::CaseInsensitive));
                    //pathname auslesen
                }
            }

        }
    }
}
QString biblequote::readInfo(QFile &file)
{
    //Biblequote
    QString out = "";
    int countlines = 0;
    int invalid = true;
    while (!file.atEnd()) {
        if (countlines > 100) { //wenn eine ini datei ungueltig ist soll damit nicht zuviel zeit verguedet werden
            break;
        }
        countlines++;
        QByteArray byteline = file.readLine();
        QTextCodec *codec = QTextCodec::codecForName(bqset.encoding.toStdString().c_str());
        QTextDecoder *decoder = codec->makeDecoder();
        QString line;
        line = decoder->toUnicode(byteline);
        if (!line.startsWith("//") && line.contains("BibleName", Qt::CaseInsensitive)) {
            bibleName = formatfromini(line.remove("BibleName =", Qt::CaseInsensitive));
            if (line.contains("BibleName=", Qt::CaseInsensitive)) {
                bibleName = formatfromini(line.remove("BibleName=", Qt::CaseInsensitive));
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
struct stelle biblequote::search(QString searchstring, bool regexp, bool whole, bool casesen)
{
    DEBUG_FUNC_NAME
    if (whole == true) {
        searchstring = " " + searchstring + " ";
    }
    lastSearch = searchstring;
    QStringList ctext;
    QList<QByteArray> bytetext;
    QProgressDialog progress(QObject::tr("Searching"), QObject::tr("Cancel"), 0, bookPath.size());
    progress.setWindowModality(Qt::WindowModal);
    struct stelle st2;
    st2.bibleID = currentBibleID;
    for (int id = 0; id < bookPath.size(); id++) {
        if (progress.wasCanceled())
            return st2;
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
            return st2;
        }
        QTextCodec *codec = QTextCodec::codecForName(bqset.encoding.toStdString().c_str());
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
            bool b;
            if (regexp == true) {
                b = ctext.at(chapterit).contains(QRegExp(searchstring));
            } else {
                if (casesen == true) {
                    b = ctext.at(chapterit).contains(searchstring, Qt::CaseSensitive);
                } else {
                    b = ctext.at(chapterit).contains(searchstring, Qt::CaseInsensitive);
                }
            }
            if (b) {
                QStringList verses = ctext[chapterit].split(versesign);
                for (int verseit = 0; verseit < verses.size(); ++verseit) {
                    QString t = verses.at(verseit);
                    bool b2;
                    if (regexp == true) {
                        b2 = t.contains(QRegExp(searchstring));
                    } else {
                        if (casesen == true) {
                            b2 = t.contains(searchstring, Qt::CaseSensitive);
                        } else {
                            b2 = t.contains(searchstring, Qt::CaseInsensitive);
                        }
                    }
                    if (b2) {
                        st2.book << id;
                        st2.chapter << chapterit;
                        st2.verse << verseit;
                        st2.text << t;
                    }
                }
            }
        }
    }
    progress.close();
    return st2;

}
