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
#include <QtCore/QMapIterator>
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
    //!chtodo: wenn ich biblelists aktivire ( nach neustart) und einfach eine Bibel hinzufüge ohne die bestehende zu verändernd sind all dies Sache in Bible nicht gesetzt

}
void Bible::setModuleMap(ModuleMap *map)
{
    m_map = map;
}
void Bible::setBibleType(const Module::ModuleType &type)
{
    m_bibleType = type;
}
void Bible::setSettings(Settings *settings)
{
    m_settings = settings;
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
    m_loaded = true;
    m_module = m_map->m_map.value(moduleID);
    if (moduleID < 0 || !m_module) {
        myDebug() << "invalid bibleID = " << moduleID;
        return 1;
    }
    m_moduleID = moduleID;


    QString path = m_module->m_path;
    myDebug()  << path;

    switch (m_bibleType) {
    case Module::BibleQuoteModule: {
        if(m_module->m_bibleQuote) {
            m_bq = m_module->m_bibleQuote;
        }
        else {
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
        m_bookPath = m_bq->m_bookPath;
        m_biblePath = m_bq->m_biblePath;
        //ModuleCache
        m_settings->setTitle(path,m_bibleTitle);
        m_settings->setBookCount(path,bookCount);
        m_settings->setBookNames(path,m_bookFullName);

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
        myDebug() << "moduleID = " << m_moduleID << " name = "  << m.moduleName << " m = " << m.modulePath;

        m_bibleTitle = m.moduleName;


        bookCount = m_zef->bookCount;
        m_bookFullName = m_zef->bookFullName;
        m_biblePath = m_zef->currentBiblePath;
        //ModuleCache
        m_settings->setTitle(path,m_bibleTitle);
        m_settings->setBookCount(path,bookCount);
        m_settings->setBookNames(path,m_bookFullName);

        break;
    }
    default:
        return 1;
    }
    return 0;
}
/**
  Load only the book without pharsing.
*/
int Bible::readBook(int id)
{
    // DEBUG_FUNC_NAME
    m_bookID = id;
    myDebug() << id;
    //myDebug() << "id = " << id << " bibleType = " << m_bibleType;
    switch (m_bibleType) {
    case Module::BibleQuoteModule: {
        m_chapterData.clear();
        m_chapterNames.clear();
        if (id < m_bookPath.size()) {
            int r = m_bq->readBook(id, m_bookPath.at(id));
            myDebug() << "bq readbook = " << r;
            if (r != 0)
                return r;
        } else {
            myDebug() << "index out of range bookPath.size() = " << m_bookPath.size() << " , id = " << id;
            return 1;
        }
        m_chapterData = m_bq->m_chapterData;

        int cc = bookCount[id];
        int start = 1;
        if (m_bq->m_chapterZero) {
            start = 0;
        }
        for (int i = start; i < cc + start; i++) {
            m_chapterNames << QString::number(i);
        }
        break;
    }
    case Module::ZefaniaBibleModule: { //zefania
        m_chapterData.clear();
        m_chapterNames.clear();
        m_zef->readBook(id);
        m_chapterData = m_zef->chapterData;
        for (int i = 1; i <= m_zef->bookCount[id]; i++) {
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
    myDebug() << chapterID;
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
    if (startVerse < 0)
        startVerse = 0;
    if (saveRawData)
        m_chapterDataList.clear();
    ModuleSettings moduleSettings = m_settings->getModuleSettings(m_moduleID);
    QString out = "";//Return
    QStringList versList;
    switch (m_bibleType) {
    case Module::BibleQuoteModule: {
        if (chapterID+1 >= m_chapterData.size()) {
            myDebug() << "index out of range index chapter chapterID = " << chapterID+1  << " chapterData.size() = " << m_chapterData.size();
            break;
        }
        Chapter chapter = m_chapterData.at(chapterID+1);//get data for this chapter
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
            if(i > 1) {//because of the chapter
                vers.prepend("<span verseID='"+QString::number(i-1)+"' chapterID='"+QString::number(chapterID)+"' bookID='"+QString::number(m_bookID)+"' moduleID='"+QString::number(m_moduleID)+"'>\n");
                vers.append("</span>\n");
            }
            if (i == markVerseID) {
                vers.prepend("<b>");
                vers.append("</b>"); //make the current verse bold
            }

            versList << vers;
        }

        break;
    }
    case Module::ZefaniaBibleModule: { //zefania

        if (chapterID >= m_chapterData.size()) {
            myDebug() << "index out of range index chapterID = " << chapterID  << " chapterData.size() = " << m_chapterData.size();
            break;
        }
        Chapter c = m_chapterData.at(chapterID);

        if (saveRawData) {
            //textTitle = "<b><font size=\"+5\">" + c.bookName + " " + c.chapterName + "</font></b><br /><br />";
            out += textTitle;//title
            //m_chapterDataList << out;
        }

        int end;
        if (endVerse == -1) {
            end = c.data.size();
        } else {
            end = endVerse;
        }
        for (int i = startVerse; i < end; i++) {
            QString vers = c.data.at(i);
            //main formatting

            if (m_notes != 0 && m_bibleDisplaySettings->loadNotes == true) {
                for (int n = 0; n < m_notes->getIDList().size(); ++n) {
                    QString noteID = m_notes->getIDList().at(n);
                    if (m_notes->getType(noteID) == "text") {
                        QString link = m_notes->getRef(noteID, "link");
                        UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::None, link);
                        urlConverter.setModuleMap(m_map);
                        urlConverter.pharse();

                        if (urlConverter.m_moduleID == m_moduleID && urlConverter.m_bookID == m_bookID && urlConverter.m_chapterID == chapterID && urlConverter.m_verseID == i) {
                            vers.append("<a href=\"note://" + noteID + "\"><img src=\":/icons/16x16/view-pim-notes.png\" title=\"" + m_notes->getRef(noteID, "title") + "\"></a>");
                        }
                    }
                }
            }


            if (i == markVerseID) {
                vers.prepend("<a name=\"currentVerse\"><span style=\"font-weight: bold;\">");
                vers.append("</span></a>");
            }
            if (moduleSettings.zefbible_textFormatting == 0) {
                vers.prepend("<sup>" + c.verseNumber.at(i) + "</sup> ");
                vers.append("<br />");
            } else {
                vers.prepend("<sup>"+c.verseNumber.at(i)+"</sup>    ");
                vers.append("");
            }
            vers.prepend("<span verseID='"+QString::number(i)+"' chapterID='"+QString::number(chapterID)+"' bookID='"+QString::number(m_bookID)+"' moduleID='"+QString::number(m_moduleID)+"'>\n");
            vers.append("</span>\n");
            versList << vers;
        }
        break;
    }
    default:
        return "";
    }
   /* if (m_notes != 0 && m_bibleDisplaySettings->loadNotes == true) {
        for (int n = 0; n <  m_notes->getIDList().size(); ++n) {
            QString noteID = m_notes->getIDList().at(n);
            if (m_notes->getType(noteID) == "mark") {
                QString link = m_notes->getRef(noteID, "link");
                UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::None, link);
                urlConverter.setModuleMap(m_map);
                urlConverter.pharse();

                if (urlConverter.m_moduleID == m_moduleID && urlConverter.m_bookID == m_bookID && urlConverter.m_chapterID == chapterID) {
                    myDebug() << "mark found id = " << noteID;
                    versList = toUniformHtml(versList);
                    if (m_notes->getRef(noteID, "start") == m_notes->getRef(noteID, "end")) {
                        int versID = m_notes->getRef(noteID, "start").toInt();
                        if (endVerse != -1) {
                            //todo: do something
                        }
                        QString vers = versList.at(versID);
                        int startPos = vers.lastIndexOf(m_notes->getRef(noteID, "startString"));
                        int endPos = vers.lastIndexOf(m_notes->getRef(noteID, "endString")) + m_notes->getRef(noteID, "endString").size();
                        myDebug() << "vers = " << vers << " startPos = " << startPos << " endPos = " << endPos << " startString = " << m_notes->getRef(noteID, "startString") << " endString = " << m_notes->getRef(noteID, "endString");
                        QString vB = vers, vB2 = vers;

                        vB.remove(startPos, vB.size());
                        int lastOpen = vB.lastIndexOf("<");
                        if (vB.lastIndexOf(">", lastOpen) == -1) {
                            myDebug() << "startPos in in tag";

                        }
                        vB2.remove(0, endPos);
                        int firstClose = vB2.indexOf(">");
                        if (vB.indexOf("<") > firstClose || vB.indexOf("<") == -1) {
                            myDebug() << "endPos in tag";
                        }


                        vers.insert(endPos, "</span>");
                        vers.insert(startPos, "<span name=\"mark\" style=\"background-color:" + m_notes->getRef(noteID, "color") + "\">");
                        versList.replace(versID, vers);

                    } else {
                        int startVersID = m_notes->getRef(noteID, "start").toInt();
                        int endVersID = m_notes->getRef(noteID, "end").toInt();
                        if (endVerse != -1) {
                            //todo: do something
                        }
                        if (startVersID >= 0 && endVersID >= 0 && startVersID < versList.size() && endVersID < versList.size()) {
                            myDebug() << " start = " << startVersID << " size = " << versList.size();
                            QString startVers = versList.at(startVersID);
                            QString endVers = versList.at(endVersID);
                            int startPos = startVers.lastIndexOf(m_notes->getRef(noteID, "startString"));
                            int endPos = endVers.lastIndexOf(m_notes->getRef(noteID, "endString")) + m_notes->getRef(noteID, "endString").size();
                            myDebug() << "StartVers = " << startVers << " endVers = " << endVers << " startPos = " << startPos << " endPos = " << endPos << " startString = " << m_notes->getRef(noteID, "startString") << " endString = " << m_notes->getRef(noteID, "endString");

                            startVers.insert(startPos, "<span class=\"mark\" style=\"background-color:" + m_notes->getRef(noteID, "color") + "\">");
                            endVers.insert(endPos, "</span>");
                            versList.replace(startVersID, startVers);
                            versList.replace(endVersID, endVers);
                        }
                    }

                }
            }
        }
    }

*/
    for (int i = 0; i < versList.size(); ++i) {
        out += versList.at(i);
        if (saveRawData)
            m_chapterDataList.append(versList.at(i));
    }

    if (saveRawData) {
        m_lastout = out;
    }
    return out;
}
QString Bible::toUniformHtml(QString string)
{
    DEBUG_FUNC_NAME
    QTextDocument t;
    t.setHtml(string);
    QString raw = t.toHtml();
    int firstPStyle = raw.indexOf("<p style=");
    Q_ASSERT(firstPStyle != -1);
    int endOfPStyle = raw.indexOf(">", firstPStyle);

    int i2 = raw.lastIndexOf("</p></body></html>");

    Q_ASSERT(i2 != -1);

    raw.remove(i2, raw.size());
    raw.remove(0, endOfPStyle + 1);

    return raw;
}
QStringList Bible::toUniformHtml(QStringList string)
{
    DEBUG_FUNC_NAME
    QString wholeText = string.join("[VERSEINSERT_FROM_OPENBIBLEVIEWER]");
    QTextDocument t;
    t.setHtml(wholeText);
    QString raw = t.toHtml();

    int firstPStyle = raw.indexOf("<p style=");
    Q_ASSERT(firstPStyle != -1);
    int endOfPStyle = raw.indexOf(">", firstPStyle);

    int i2 = raw.lastIndexOf("</p></body></html>");

    Q_ASSERT(i2 != -1);

    raw.remove(i2, raw.size());
    myDebug() << raw.remove(0, endOfPStyle + 1);//todo: wtf?
    return raw.split("[VERSEINSERT_FROM_OPENBIBLEVIEWER]");
}
/**
  Search in the current module
  \query The search query.
  */
SearchResult Bible::search(SearchQuery query)
{
    m_lastSearchQuery = query;
    SearchResult result;
    switch (m_bibleType) {
    case Module::BibleQuoteModule: {
        result = m_bq->search(query);
        break;
    }
    case Module::ZefaniaBibleModule: {
        result = m_zef->search(query);
        break;
    }
    default:
        break;
    }
    m_lastSearchResult = result;
    return result;
}


QStringList Bible::getSearchPaths()
{
    if (m_bibleType == Module::ZefaniaBibleModule) {
        return QStringList();
    } else if (m_bibleType == Module::BibleQuoteModule) {
        QStringList l;
        l.append(QString(m_biblePath + QDir::separator()));
        if (m_bookID < m_bookPath.size()) {
            QString p = m_bookPath.at(m_bookID);
            int pos = p.lastIndexOf(QDir::separator());
            if (pos != -1) {
                p = p.remove(pos, p.size());
            }
            if (!p.startsWith(m_biblePath)) {
                p = m_biblePath + QDir::separator() + p + QDir::separator();
            }
            l.append(p);
        }
        return l;
    }
    return QStringList();
}

int Bible::moduleID()
{
    return m_moduleID;
}
void Bible::setModuleID(const int &bible)
{
    m_moduleID = bible;
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
    //todo: if there are another module rethink this function
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
    return m_bibleType;
}
int Bible::verseID()
{
    return m_verseID;
}


