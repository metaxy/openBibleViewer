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
#include <QtCore/QtDebug>
#include <QtCore/QMapIterator>
#include <QtCore/QDir>
Bible::Bible()
{
    currentBibleID = -1;
    m_settings = new Settings();
}
void Bible::setBibleType(int type)
{
    //todo:check if type is valid
    bibleType = type;
    return;
}
int Bible::loadBibleData(int bibleID, QString path)
{
    DEBUG_FUNC_NAME
    myDebug() << "bibleID = " << bibleID << " path = " << path << " bibleType =" << bibleType;
    currentBibleID = bibleID;
    switch (bibleType) {
    case BibleQuote: { //biblequote
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
    case ZefaniaBible: { //zefania bible

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
    case BibleQuote: { //biblequote
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
    case ZefaniaBible: { //zefania
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
    //endVerse == -1 means all verse
    if (saveRawData)
        chapterDataList.clear();
    qDebug() << "bible::readVerse() start";
    currentChapterID = chapterID;
    QString out = "";
    switch (bibleType) {
    case BibleQuote: { //biblequote
        Chapter c;
        if (chapterID < chapterData.size()) {

            c = chapterData.at(chapterID);
            qDebug() << "bible::readVerse() chapterID = " << chapterID << " chapterdata.size() = " << chapterData.size() << " a.size() = " << c.data.size();
            int end;
            if (endVerse == -1) {
                end = c.data.size();
            } else {
                end  = endVerse;
            }
            for (int i = startVerse; i < end; i++) {
                if (i == markVerseID) {
                    out += "<b>" + c.data.at(i) + "</b>";
                } else {
                    out += c.data.at(i);
                }
                if (saveRawData)
                    chapterDataList << c.data.at(i);
            }

        } else {
            qDebug() << "bible::readVerse() index out of range index";
        }
        break;
    }
    case ZefaniaBible: { //zefania
        qDebug() << "bible::readVerse() zefania read";
        Chapter c;
        if (chapterID < chapterData.size()) {

            c = chapterData.at(chapterID);
            qDebug() << "bible::readVerse() chapterID = " << chapterID << " chapterdata.size() = " << chapterData.size() << " a.size() = " << c.data.size();
            if (m_settings->getModuleSettings(currentBibleID).zefbible_textFormatting == 0) {
                if (saveRawData)
                    out = "<b><font size=\"+5\">" + c.bookName + " " + c.chapterName + "</font></b><br /><br />";
                int end;
                if (endVerse == -1) {
                    end = c.data.size();
                } else {
                    end  = endVerse;
                }
                for (int i = startVerse; i < end; i++) {
                    QString o =  "<i>" + c.verseNumber.at(i) + "</i> " + c.data.at(i) + "<br />";
                    if (i == markVerseID) {
                        out += "<a name=\"currentVerse\"><b>" + o + "</b></a>";
                    } else {
                        out += o;
                    }
                    if (saveRawData)
                        chapterDataList << o;
                }
            } else { /*if(m_settings->getModuleSettings(currentBibleID).zefbible_textFormatting == 1)*/
                if (saveRawData)
                    out = "<b><font size=\"+5\">" + c.bookName + " " + c.chapterName + "</font></b><br /><br />";
                int end;
                if (endVerse == -1) {
                    end = c.data.size();
                } else {
                    end  = endVerse;
                }
                for (int i = startVerse; i < end; i++) {
                    QString o =  c.verseNumber.at(i) + " " + c.data.at(i);
                    if (i == markVerseID) {
                        out += "<b>" + o + "</b>";
                    } else {
                        out += o;
                    }
                    if (saveRawData)
                        chapterDataList << o;
                }
            }


        } else {
            qDebug() << "bible::readVerse() index out of range index";
        }
        break;
    }
    }
    if (saveRawData)
        lastout = out;
    qDebug() << "bible::readVerse() end";
    return out;
}
struct stelle Bible::search(struct searchQuery query) {
    lastSearchString = query.text;
    struct stelle st;
    switch (bibleType) {
    case BibleQuote: {
        st = bq.search(query);
        break;
    }
    case ZefaniaBible: {
        st = zef.search(query);
        break;
    }
    }
    st.bibleID = currentBibleID;
    return st;
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
    /*QMapIterator<int, KoXmlElement> i(cache);
    while (i.hasNext())
    {
        i.next();
        zef.softCacheAvi[i.key()] = true;
    }*/

}
QStringList Bible::getSearchPaths()
{
    if (bibleType == ZefaniaBible) {
        return QStringList();
    } else if (bibleType == BibleQuote) {
        QStringList l;
        l << QString(currentBiblePath + QDir::separator());
        if (currentBookID < bookPath.size()) {
            QString p = bookPath.at(currentBookID);
            int pos = p.lastIndexOf(QDir::separator());
            if (pos != -1) {
                p = p.remove(pos, p.size());
            }
            if (!p.startsWith(currentBiblePath)) {
                p = currentBiblePath + QDir::separator() + p + QDir::separator();
            }
            l << p;
        }
        return l;
    }
    return QStringList();
}
