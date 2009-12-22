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
#include "bible.h"
#include "../core/settings.h"
#include "../core/modulesettings.h"
#include "../core/dbghelper.h"
#include "../core/urlconverter.h"
#include <QtCore/QtDebug>
#include <QtCore/QMapIterator>
#include <QtCore/QDir>
#include <QtGui/QTextDocument>
Bible::Bible()
{
    currentBibleID = -1;
    m_settings = new Settings();
}
void Bible::setBibleType(const int &type)
{
    //todo:check if type is valid
    bibleType = type;
    return;
}
int Bible::loadBibleData(const int &bibleID, const QString &path)
{
    DEBUG_FUNC_NAME
    myDebug() << "bibleID = " << bibleID << " path = " << path << " bibleType =" << bibleType;
    currentBibleID = bibleID;
    switch (bibleType) {
    case BibleQuoteModule: {
        bq.setSettings(m_settings);

        bq.loadBibleData(bibleID, path);
        bibleName = bq.bibleName;
        bookCount = bq.bookCount;
        bookFullName = bq.bookFullName;
        bookPath = bq.bookPath;
        chapterAdd = 1;
        currentBiblePath = bq.currentBiblePath;
        break;
    }
    case ZefaniaBibleModule: { //zefania bible

        ModuleSettings m = m_settings->getModuleSettings(currentBibleID);
        zef.setSettings(m_settings);

        zef.loadBibleData(bibleID, path);
        bibleName = m.moduleName;
        bookCount = zef.bookCount;
        bookFullName = zef.bookFullName;

        chapterAdd = 0;
        currentBiblePath = zef.currentBiblePath;
        break;
    }
    }
    return 0;
}
/*
  only load book, not pharsed
*/
int Bible::readBook(int id)
{
    currentBookID = id;
    qDebug() << "bible::readBook() id= " << id << " bibleType =" << bibleType;
    switch (bibleType) {
    case BibleQuoteModule: {
        chapterData.clear();
        chapterNames.clear();
        if (id < bookPath.size()) {
            bq.readBook(id, bookPath.at(id));
        } else {
            qDebug() << "bible::readBook() index out of range bookPath.size() = " << bookPath.size() << " , id = " << id;
            return 1;
        }
        chapterData = bq.chapterData;

        int cc = bookCount[id];
        if (bq.chapterZero == true) {
            for (int i = 0; i < cc; i++) {
                chapterNames << QString::number(i);
            }
        } else {
            for (int i = 1; i <= cc; i++) {
                chapterNames << QString::number(i);
            }
        }
        break;
    }
    case ZefaniaBibleModule: { //zefania
        chapterData.clear();
        chapterNames.clear();
        zef.readBook(id);
        chapterData = zef.chapterData;
        for (int i = 1; i <= zef.bookCount[id]; i++) {
            chapterNames << QString::number(i);
        }
        break;
    }
    }
    qDebug() << "bible::readBook() end";
    return 0;
}
void Bible::setSettings(Settings *set)
{
    DEBUG_FUNC_NAME
    m_settings = set;
    //  myDebug() << " setings.module.size() = " << m_settings->module.size() << " moduleID = " << m_settings->moduleID;
    zef.setSettings(m_settings);
    bq.setSettings(m_settings);
}
QString Bible::readChapter(int chapterID, int verseID = -1)
{
    return readVerse(chapterID, 0, -1, verseID, true);

}
/* pharse the loaded book
   */
QString Bible::readVerse(int chapterID, int startVerse, int endVerse, int markVerseID = -1, bool saveRawData = false)
{
    DEBUG_FUNC_NAME
    //endVerse == -1 means all verse
    currentChapterID = chapterID;
    if (saveRawData)
        chapterDataList.clear();

    QString out = "";//Return
    switch (bibleType) {
    case BibleQuoteModule: {
        if (chapterID >= chapterData.size()) {
            myDebug() << "index out of range index";
            break;
        }
        Chapter chapter = chapterData.at(chapterID);//get data for this chapter
        //find out whereto read verse
        int end;
        if (endVerse == -1) {
            end = chapter.data.size();
        } else {
            end  = endVerse;
        }
        for (int i = startVerse; i < end; i++) {
            //no title formatting, because it is already formatted
            QString vers = chapter.data.at(i);

            if (i == markVerseID) {
                vers.prepend("<b>");
                vers.append("</b>"); //make the current verse vold
            }
            out += vers;
            if (saveRawData)
                chapterDataList.append(vers);
        }

        break;
    }
    case ZefaniaBibleModule: { //zefania
        qDebug() << "bible::readVerse() zefania read";

        if (chapterID >= chapterData.size()) {
            myDebug() << "index out of range index";
            break;
        }
        Chapter c = chapterData.at(chapterID);
        if (saveRawData)
            out = "<b><font size=\"+5\">" + c.bookName + " " + c.chapterName + "</font></b><br /><br />";//title
        if (saveRawData)
            chapterDataList << out;

        int end;
        if (endVerse == -1) {
            end = c.data.size();
        } else {
            end = endVerse;
        }
        for (int i = startVerse; i < end; i++) {
            QString vers = c.data.at(i);
            //main formatting
            if (m_settings->getModuleSettings(currentBibleID).zefbible_textFormatting == 0) {
                vers.prepend("<span style=\" font-style:italic;\">" + c.verseNumber.at(i) + "</span>");
                vers.append("<br />");
            } else {
                vers.prepend(c.verseNumber.at(i) + " ");
            }
            //if is current verse
            if (i == markVerseID) {
                vers.prepend("<a name=\"currentVerse\"><b>");
                vers.append("</b></a>");
            }

            if (m_notes != 0) {
                for (int n = 0; n < m_notes->getIDList().size(); ++n) {
                    QString noteID = m_notes->getIDList().at(n);
                    if (m_notes->getType(noteID) == "mark") {
                        QString link = m_notes->getRef(noteID, "link");
                        UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::None, link);
                        urlConverter.m_biblesIniPath = biblesIniPath;
                        urlConverter.pharse();

                        if (urlConverter.m_bibleID.toInt() == currentBibleID && urlConverter.m_bookID == currentBookID && urlConverter.m_chapterID == chapterID && (m_notes->getRef(noteID, "start").toInt() == i || m_notes->getRef(noteID, "end").toInt() == i)) {
                            QTextDocument t;
                            t.setHtml(vers);
                            QString raw = t.toHtml();
                            QString a1 = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
                            QString a2 = "</p></body></html>";
                            int i1 = raw.indexOf(a1);
                            int i2 = raw.indexOf(a2);
                            raw.remove(i2, raw.size());
                            raw.remove(0, i1 + a1.size());
                            vers = raw;
                            if (m_notes->getRef(noteID, "start") == m_notes->getRef(noteID, "end")) {
                                vers.insert(m_notes->getRef(noteID, "endPos").toInt(), "</span>");
                                vers.insert(m_notes->getRef(noteID, "startPos").toInt(), "<span name=\"mark\" style=\"background-color:#ffff00\">");

                            } else {
                                if (m_notes->getRef(noteID, "start").toInt() == i) {
                                    myDebug() << " vers = " << vers << " startPos = " << m_notes->getRef(noteID, "startPos") << " vers = " << vers;
                                    vers.insert(m_notes->getRef(noteID, "startPos").toInt(), "<span name=\"mark\" style=\"background-color:#ffff00\">");
                                }
                                if (m_notes->getRef(noteID, "end").toInt() == i) {
                                    myDebug() << " vers = " << vers << " endPos = " << m_notes->getRef(noteID, "endPos") << " vers = " << vers;
                                    vers.insert(m_notes->getRef(noteID, "endPos").toInt(), "</span>");
                                }
                            }

                        }
                    }
                }
            }
            out += vers;
            if (saveRawData)
                chapterDataList << vers;
        }
        break;
    }
    }

    if (saveRawData)
        lastout = out;
    return out;
}
SearchResult Bible::search(SearchQuery query)
{
    lastSearchQuery = query;
    SearchResult result;
    switch (bibleType) {
    case BibleQuoteModule: {
        result = bq.search(query);
        break;
    }
    case ZefaniaBibleModule: {
        result = zef.search(query);
        break;
    }
    }
    return result;
}
QMap<int, QList<Chapter> > Bible::getZefCache()
{
    return zef.softCache();
}
void Bible::clearZefCache()
{
    zef.clearSoftCache();
}
void Bible::setZefCache(QMap<int, QList<Chapter> > cache)
{
    zef.setSoftCache(cache);
}
QStringList Bible::getSearchPaths()
{
    if (bibleType == ZefaniaBibleModule) {
        return QStringList();
    } else if (bibleType == BibleQuoteModule) {
        QStringList l;
        l.append(QString(currentBiblePath + QDir::separator()));
        if (currentBookID < bookPath.size()) {
            QString p = bookPath.at(currentBookID);
            int pos = p.lastIndexOf(QDir::separator());
            if (pos != -1) {
                p = p.remove(pos, p.size());
            }
            if (!p.startsWith(currentBiblePath)) {
                p = currentBiblePath + QDir::separator() + p + QDir::separator();
            }
            l.append(p);
        }
        return l;
    }
    return QStringList();
}
void Bible::setNotes(Notes *n)
{
    m_notes = n;
}
