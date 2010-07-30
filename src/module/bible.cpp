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
#include "bible.h"
#include "src/core/settings.h"
#include "src/core/modulesettings.h"
#include "src/core/dbghelper.h"
#include "src/core/urlconverter.h"
#include <QtCore/QDir>
#include <QtGui/QTextDocument>
Bible::Bible()
{
    m_moduleID = -1;
    m_bookID = 0;
    m_chapterID = 0;
    m_verseID = 0;
    textTitle = "";
    m_loaded = false;
    m_moduleType = Module::NoneType;
}



void Bible::setBibleDisplaySettings(BibleDisplaySettings *bibleDisplaySettings)
{
    m_bibleDisplaySettings = bibleDisplaySettings;
}

bool Bible::loaded()
{
    return m_loaded;
}

int Bible::loadModuleData(const int &moduleID)
{
    m_module = m_map->m_map.value(moduleID);
    if(moduleID < 0 || !m_module) {
        myWarning() << "invalid bibleID = " << moduleID;
        return 1;
    }
    m_moduleID = moduleID;
    const QString path = m_module->m_path;

    switch(m_moduleType) {
    case Module::BibleQuoteModule: {
        if(m_module->m_bibleQuote) {
            m_bq = m_module->m_bibleQuote;
        } else {
            m_bq = new BibleQuote();
            m_module->m_bibleQuote = m_bq;
        }
        ModuleSettings m = m_settings->getModuleSettings(m_moduleID);
        m_bq->setSettings(m_settings);
        m_bq->loadBibleData(moduleID, path);
        if(m.moduleName.isEmpty())
            m_bibleTitle = m_bq->m_bibleName;
        else
            m_bibleTitle = m.moduleName;
        m_bibleShortTitle = m_bq->m_bibleShortName;

        bookCount = m_bq->m_bookCount;
        m_bookFullName = m_bq->m_bookFullName;

        m_bookShortName = m_bq->m_bookShortName;
        m_bookPath = m_bq->m_bookPath;
        m_biblePath = m_bq->m_biblePath;
        //ModuleCache
        m_settings->setTitle(path, m_bibleTitle);
        m_settings->setBookCount(path, bookCount);
        m_settings->setBookNames(path, m_bookFullName);

        break;
    }
    case Module::ZefaniaBibleModule: {
        if(m_module->m_zefaniaBible) {
            m_zef = m_module->m_zefaniaBible;
        } else {
            m_zef = new ZefaniaBible();
            m_module->m_zefaniaBible = m_zef;
        }
        ModuleSettings m = m_settings->getModuleSettings(m_moduleID);
        m_zef->setSettings(m_settings);
        m_zef->loadBibleData(moduleID, path);
        m_bibleTitle = m.moduleName;


        bookCount = m_zef->m_bookCount;
        m_bookFullName = m_zef->m_bookFullName;
        m_bookShortName.clear();
        foreach(QString s,m_zef->m_bookShortName) {
            m_bookShortName.append(QStringList(s));
        }
        m_biblePath = m_zef->m_biblePath;
        //ModuleCache
        m_settings->setTitle(path, m_bibleTitle);
        m_settings->setBookCount(path, bookCount);
        m_settings->setBookNames(path, m_bookFullName);

        break;
    }
    default:
        return 1;
    }
    m_loaded = true;
    return 0;
}
/**
  Load only the book without pharsing.
*/
int Bible::readBook(int id)
{
    // DEBUG_FUNC_NAME
    m_bookID = id;
    switch(m_moduleType) {
    case Module::BibleQuoteModule: {
        m_chapterData.clear();
        m_chapterNames.clear();
        if(id < m_bookPath.size()) {
            int r = m_bq->readBook(id, m_bookPath.at(id));
            if(r != 0)
                return r;
        } else {
            myWarning() << "index out of range bookPath.size() = " << m_bookPath.size() << " , id = " << id;
            return 1;
        }
        m_chapterData = m_bq->m_chapterData;

        int cc = bookCount[id];
        int start = 1;
        if(m_bq->m_chapterZero) {
            start = 0;
        }
        for(int i = start; i < cc + start; ++i) {
            m_chapterNames << QString::number(i);
        }
        break;
    }
    case Module::ZefaniaBibleModule: { //zefania
        m_chapterData.clear();
        m_chapterNames.clear();
        m_zef->readBook(id);
        m_chapterData = m_zef->m_chapterData;
        for(int i = 1; i <= m_zef->m_bookCount[id]; ++i) {
            m_chapterNames << QString::number(i);
        }
        break;
    }
    default:
        return 1;
    }
    return 0;
}

QString Bible::readChapter(int chapterID, int verseID = -1)
{
    if(verseID != -1) {
        m_verseID = verseID;
    } else {
        m_verseID = 0;
    }
    m_chapterID = chapterID;
    return readVerse(chapterID, 0, -1, verseID, true);
}
/**
    Pharse the loaded book.
 */
QString Bible::readVerse(int chapterID, int startVerse, int endVerse, int markVerseID = -1, bool saveRawData = false)
{
    //DEBUG_FUNC_NAME
    //endVerse == -1 means all verse
    m_chapterID = chapterID;
    if(startVerse < 0)
        startVerse = 0;
    if(saveRawData)
        m_chapterDataList.clear();
    ModuleSettings moduleSettings = m_settings->getModuleSettings(m_moduleID);
    QString out = "";//Return
    QStringList versList;
    switch(m_moduleType) {
    case Module::BibleQuoteModule: {
        if(chapterID + 1 >= m_chapterData.size()) {
            myWarning() << "index out of range index chapter chapterID = " << chapterID + 1  << " chapterData.size() = " << m_chapterData.size();
            break;
        }
        Chapter chapter = m_chapterData.at(chapterID + 1); //get data for this chapter
        //find out whereto read verse
        int end;
        if(endVerse == -1) {
            end = chapter.data.size();
        } else {
            end  = endVerse;
        }
        for(int i = startVerse; i < end; i++) {
            //no title formatting, because it is already formatted
            QString vers = toUniformHtml(chapter.data.at(i));//todo: that is too slow, use something else to make their invalid html code valid
            if(i == markVerseID + 1) {//because of the chapter title
                vers.prepend("<b>");
                vers.append("</b>");//make the current verse bold
            }
            versList << vers;
        }

        break;
    }
    case Module::ZefaniaBibleModule: { //zefania

        if(chapterID >= m_chapterData.size()) {
            myWarning() << "index out of range index chapterID = " << chapterID  << " chapterData.size() = " << m_chapterData.size();
            break;
        }
        Chapter c = m_chapterData.at(chapterID);

        if(saveRawData) {
            out += textTitle;//title
        }

        int end;
        if(endVerse == -1) {
            end = c.data.size();
        } else {
            end = endVerse;
        }
        for(int i = startVerse; i < end; i++) {
            QString vers = c.data.at(i);
            //main formatting

            if(m_notes != 0 && m_bibleDisplaySettings->loadNotes == true) {
                for(int n = 0; n < m_notes->getIDList().size(); ++n) {
                    QString noteID = m_notes->getIDList().at(n);
                    if(m_notes->getType(noteID) == "text") {
                        QString link = m_notes->getRef(noteID, "link");
                        UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::None, link);
                        urlConverter.setSettings(m_settings);
                        urlConverter.setModuleMap(m_map);
                        urlConverter.pharse();
                        if(urlConverter.m_moduleID == m_moduleID && urlConverter.m_bookID == m_bookID && urlConverter.m_chapterID == chapterID && urlConverter.m_verseID == i) {
                            vers.append("<a href=\"note://" + noteID + "\"><img src=\":/icons/16x16/view-pim-notes.png\" title=\"" + m_notes->getRef(noteID, "title") + "\"></a>");
                        }
                    }
                }
            }


            if(i == markVerseID) {
                vers.prepend("<a name=\"currentVerse\"><span style=\"font-weight: bold;\">");
                vers.append("</span></a>");
            }
            if(moduleSettings.zefbible_textFormatting == 0) {
                vers.prepend("<sup>" + c.verseNumber.at(i) + "</sup> ");
                vers.append("<br />");
            } else {
                vers.prepend("<sup>" + c.verseNumber.at(i) + "</sup>    ");
                vers.append("");
            }

            versList << vers;
        }
        break;
    }
    default:
        return "";
    }
    if(m_notes != 0 && m_bibleDisplaySettings->loadNotes == true) {
        for(int n = 0; n <  m_notes->getIDList().size(); ++n) {
            QString noteID = m_notes->getIDList().at(n);
            if(m_notes->getType(noteID) == "mark") {
                QString link = m_notes->getRef(noteID, "link");
                UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::None, link);
                urlConverter.setSettings(m_settings);
                urlConverter.setModuleMap(m_map);
                urlConverter.pharse();

                const QString pre = "<span class=\"mark\" style=\"" + m_notes->getRef(noteID, "style") + "\">";

                const QString ap = "</span>";
                if(urlConverter.m_moduleID == m_moduleID && urlConverter.m_bookID == m_bookID && urlConverter.m_chapterID == chapterID) {
                    versList = versList;
                    if(m_notes->getRef(noteID, "start") == m_notes->getRef(noteID, "end")) {
                        const int versID = m_notes->getRef(noteID, "start").toInt();
                        if(endVerse != -1) {
                            //todo: do something
                        }
                        QString vers = versList.at(versID);
                        const int startPos = vers.lastIndexOf(m_notes->getRef(noteID, "startString"));
                        const int endPos = vers.lastIndexOf(m_notes->getRef(noteID, "endString")) + m_notes->getRef(noteID, "endString").size();
                        /* QString vB = vers, vB2 = vers;

                         vB.remove(startPos, vB.size());
                         int lastOpen = vB.lastIndexOf("<");
                         if(vB.lastIndexOf(">", lastOpen) == -1) {
                             myDebug() << "startPos in in tag";

                         }
                         vB2.remove(0, endPos);
                         int firstClose = vB2.indexOf(">");
                         if(vB.indexOf("<") > firstClose || vB.indexOf("<") == -1) {
                             myDebug() << "endPos in tag";
                         }*/
                        vers.insert(endPos, ap);
                        vers.insert(startPos, pre);
                        versList.replace(versID, vers);

                    } else {

                        int startVersID = m_notes->getRef(noteID, "start").toInt();
                        int endVersID = m_notes->getRef(noteID, "end").toInt();
                        if(endVerse != -1) {
                            //todo: do something
                        }
                        if(startVersID > 0 && endVersID > 0 && startVersID < versList.size() && endVersID < versList.size()) {
                            QString startVers = versList.at(startVersID);
                            QString endVers = versList.at(endVersID);
                            const int startPos = startVers.lastIndexOf(m_notes->getRef(noteID, "startString"));
                            const int endPos = endVers.lastIndexOf(m_notes->getRef(noteID, "endString")) + m_notes->getRef(noteID, "endString").size();

                            startVers.insert(startPos, pre);
                            startVers.append(ap);
                            for(int i = startVersID + 1; i < endVersID; i++) {
                                QString v = versList.at(i);
                                v.prepend(pre);
                                v.append(ap);
                                versList.replace(i, v);
                            }
                            endVers.insert(endPos, ap);
                            endVers.prepend(pre);

                            versList.replace(startVersID, startVers);
                            versList.replace(endVersID, endVers);
                        }
                    }

                }
            }
        }
    }

    for(int i = 0; i < versList.size(); ++i) {
        QString vers = versList.at(i);
        switch(m_moduleType) {
        case Module::BibleQuoteModule: {
            if(i > 1) {//because of the chapter
                vers.prepend("<span verseID='" + QString::number(i - 1) + "' chapterID='" + QString::number(chapterID) + "' bookID='" + QString::number(m_bookID) + "' moduleID='" + QString::number(m_moduleID) + "'>\n");
                vers.append("</span><br />\n");
            }
            break;

        }
        case Module::ZefaniaBibleModule: {
            vers.prepend("<span verseID='" + QString::number(i) + "' chapterID='" + QString::number(chapterID) + "' bookID='" + QString::number(m_bookID) + "' moduleID='" + QString::number(m_moduleID) + "'>\n");
            vers.append("</span>\n");
            break;
        }
        default:
            break;
        }
        out += vers;
        if(saveRawData)
            m_chapterDataList.append(vers);
    }

    if(saveRawData)
        m_lastout = out;
    return out;
}

QString Bible::toUniformHtml(QString string)
{
    QTextDocument t;
    t.setHtml(string);
    QString raw = t.toHtml();
    int firstPStyle = raw.indexOf("<p style=");
    if(firstPStyle == -1)
        return string;
    int endOfPStyle = raw.indexOf(">", firstPStyle);

    int i2 = raw.lastIndexOf("</p></body></html>");

    if(i2 == -1)
        return string;

    raw.remove(i2, raw.size());
    raw.remove(0, endOfPStyle + 1);

    return raw;
}

/**
  Search in the current bible. Returns the Results.
  */
void Bible::search(SearchQuery query,SearchResult *result)
{
    m_lastSearchQuery = query;
    switch(m_moduleType) {
    case Module::BibleQuoteModule: {
        if(!m_bq->hasIndex())
            m_bq->buildIndex();
        m_bq->search(query,result);
        break;
    }
    case Module::ZefaniaBibleModule: {
        if(!m_zef->hasIndex())
            m_zef->buildIndex();
        m_zef->search(query,result);
        break;
    }
    default:
        break;
    }
}

QStringList Bible::getSearchPaths()
{
    if(m_moduleType == Module::ZefaniaBibleModule) {
        return QStringList();
    } else if(m_moduleType == Module::BibleQuoteModule) {
        QStringList l;
        l.append(QString(m_biblePath + QDir::separator()));
        if(m_bookID < m_bookPath.size()) {
            QString p = m_bookPath.at(m_bookID);
            int pos = p.lastIndexOf(QDir::separator());
            if(pos != -1) {
                p = p.remove(pos, p.size());
            }
            if(!p.startsWith(m_biblePath)) {
                p = m_biblePath + QDir::separator() + p + QDir::separator();
            }
            l.append(p);
        }
        return l;
    }
    return QStringList();
}

int Bible::bookID()
{
    return m_bookID;
}

int Bible::chapterID()
{
    return m_chapterID;
}

int Bible::booksCount()
{
    return m_bookFullName.size();
}

int Bible::chaptersCount()
{
    return m_chapterData.size();
}

QString Bible::biblePath()
{
    return m_biblePath;
}

QString Bible::bibleTitle()
{
    return m_bibleTitle;
}

QString Bible::bibleShortTitle()
{
    if(m_bibleShortTitle.isEmpty())
        return bibleTitle();
    return m_bibleShortTitle;
}

QStringList Bible::bookFullName()
{
    return m_bookFullName;
}
QList<QStringList> Bible::bookShortName()
{
    return m_bookShortName;
}

QStringList Bible::bookPath()
{
    return m_bookPath;
}

QStringList Bible::chapterDataList()
{
    return m_chapterDataList;
}

QStringList Bible::chapterNames()
{
    return m_chapterNames;
}

Module::ModuleType Bible::bibleType()
{
    return m_moduleType;
}

int Bible::verseID()
{
    return m_verseID;
}
SearchQuery Bible::lastSearchQuery()
{
    return m_lastSearchQuery;
}


