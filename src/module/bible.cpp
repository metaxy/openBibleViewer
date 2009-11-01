/***************************************************************************
openBibleViewer - Free Bibel Viewer
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
#include "../core/config.h"
#include "../core/moduleconfig.h"
#include <QtCore/QtDebug>
#include <QtCore/QMapIterator>
bible::bible()
{
    currentBibleID = -1;
}
void bible::setBibleType(int type)
{
    //check if type is valid
    bibleType = type;
    return;
}
int bible::loadBibleData(int bibleID, QString path)
{
    qDebug() << "bible::loadBibleData() bibleID = " << bibleID << " path = " << path << " bibleType =" << bibleType;
    currentBibleID = bibleID;
    switch (bibleType) {
    case 1: { //biblequote
        bq.setSettings(settings);

        bq.loadBibleData(bibleID, path);
        bibleName = bq.bibleName;
        bookCount = bq.bookCount;
        bookFullName = bq.bookFullName;
        bookPath = bq.bookPath;
        chapterAdd = 1;
        currentBiblePath = bq.currentBiblePath;
        break;
    }
    case 2: { //zefania bible

        moduleConfig m = settings.module.at(settings.moduleID[currentBibleID]);
        zef.setSettings(settings, m);

        zef.loadBibleData(bibleID, path);
        bibleName = m.moduleName;
        qDebug() << "bible::loadBibleData bibleName = " << m.moduleName << " path = " << m.modulePath << " currentBibleID = " << currentBibleID << " moduleID = " << settings.moduleID[currentBibleID];
        bookCount = zef.bookCount;
        bookFullName = zef.bookFullName;
        //bookPath = zef.bookPath;
        chapterAdd = 0;
        currentBiblePath = zef.currentBiblePath;
        break;
    }
    }
    qDebug() << "bible::loadBibleData() end";
    return 0;
}
int bible::readBook(int id)
{
    currentBookID = id;
    qDebug() << "bible::readBook() id= " << id << " bibleType =" << bibleType;
    switch (bibleType) {
    case 1: { //biblequote
        chapterText.clear();
        chapterData.clear();
        chapterNames.clear();
        if (id < bookPath.size()) {
            bq.readBook(id, bookPath.at(id));
        } else {
            qDebug() << "bible::readBook() index out of range bookPath.size() = " << bookPath.size() << " , id = " << id;
            return 1;
        }
        chapterText = bq.chapterText;
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
        qDebug() << "bible::readBook() chapterText.size() = " << chapterText.size();
        break;
    }
    case 2: { //zefania
        chapterText.clear();
        chapterData.clear();
        chapterNames.clear();
        zef.readBook(id);
        chapterText = zef.chapterText;
        chapterData = zef.chapterData;
        for (int i = 1; i <= zef.bookCount[id]; i++) {
            chapterNames << QString::number(i);
        }

        qDebug() << "bible::readBook() chapterText.size() = " << chapterText.size();
        break;
    }
    }
    qDebug() << "bible::readBook() end";
    return 0;
}
void bible::setSettings(struct settings_s settings_)
{
    settings = settings_;
    if (settings.module.size() > 0) {
        moduleConfig m = settings.module.at(settings.moduleID[currentBibleID]);
        zef.setSettings(settings, m);
        bq.setSettings(settings);
    } else {
        zef.setSettings(settings, moduleConfig());
        bq.setSettings(settings);
    }
    return;
}
QString bible::readChapter(int chapterID, int verseID = -1)
{
    return readVerse(chapterID, 0, -1, verseID, true);

}
QString bible::readVerse(int chapterID, int startVerse, int endVerse, int markVerseID = -1, bool saveRawData = false)
{
    //endVerse == -1 means all erse
    if (saveRawData)
        chapterDataList.clear();
    qDebug() << "bible::readVerse() start";
    currentChapterID = chapterID;
    QString out = "";
    switch (bibleType) {
    case 1: { //biblequote
        chapter c;
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
            qDebug() << "bible::readVerse() out.size() = " << out.size();

        } else {
            qDebug() << "bible::readVerse() index out of range index = chapterText.size() =";
        }
        break;
    }
    case 2: { //zefania
        qDebug() << "bible::readVerse() zefania read";
        chapter c;
        if (chapterID < chapterData.size()) {

            c = chapterData.at(chapterID);
            qDebug() << "bible::readVerse() chapterID = " << chapterID << " chapterdata.size() = " << chapterData.size() << " a.size() = " << c.data.size();
            if (settings.module.at(settings.moduleID[currentBibleID]).zefbible_textFormatting == 0) {
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
            } else { /*if(settings.module.at(currentBibleID).zefbible_textFormatting == 1)*/
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

            qDebug() << "bible::readVerse() out.size() = " << out.size();

        } else {
            qDebug() << "bible::readVerse() index out of range index = chapterText.size() =";
        }
        break;
    }
    }
    if (saveRawData)
        lastout = out;
    qDebug() << "bible::readVerse() end";
    return out;
}
struct stelle bible::search(QString searchstring, bool regexp, bool whole, bool casesen) {
    lastSearchString = searchstring;
    struct stelle st;
    switch (bibleType) {
    case 1: { //biblequote
        st = bq.search(searchstring, regexp, whole, casesen);
        break;
    }
    case 2: { //zefania
        st = zef.search(searchstring, regexp, whole, casesen);
        break;
    }
    }
    st.bibleID = currentBibleID;
    return st;
}
QMap<int, QList<chapter> > bible::getZefCache()
{
    return zef.softCache();
}
void bible::clearZefCache()
{
    zef.clearSoftCache();
}
void bible::setZefCache(QMap<int, QList<chapter> > cache)
{
    zef.setSoftCache(cache);
    /*QMapIterator<int, KoXmlElement> i(cache);
    while (i.hasNext())
    {
        i.next();
        zef.softCacheAvi[i.key()] = true;
    }*/

}
