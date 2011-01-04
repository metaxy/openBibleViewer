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
#include "src/core/settings/settings.h"
#include "src/core/settings/modulesettings.h"
#include "src/core/dbghelper.h"
#include "src/core/urlconverter.h"
#include "src/core/verseselection.h"
#include "src/core/versereplacer.h"
#include <QtCore/QDir>
#include <QtGui/QTextDocument>
//#include <valgrind/callgrind.h>
Bible::Bible()
{
    m_moduleID = -1;
    m_bookID = 0;
    m_chapterID = 0;
    m_verseID = 0;
    m_loaded = false;
    m_moduleType = Module::NoneType;
    m_bibleModule = 0;
    m_lastTextRanges = 0;
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
    DEBUG_FUNC_NAME
    //CALLGRIND_START_INSTRUMENTATION;
    m_module = m_map->m_map.value(moduleID, 0);
    if(moduleID < 0 || !m_module) {
        myWarning() << "invalid bibleID = " << moduleID;
        return 1;
    }
    m_moduleID = moduleID;
    const QString path = m_module->m_path;
    m_book.clear();
    switch(m_module->m_moduleType) {
    case Module::BibleQuoteModule: {
        if(m_module->m_bibleModule) {
            m_bibleModule = m_module->m_bibleModule;
        } else {
            m_bibleModule = new BibleQuote();
            m_module->m_bibleModule = m_bibleModule;
        }
        ModuleSettings m = m_settings->getModuleSettings(m_moduleID);
        m_bibleModule->setSettings(m_settings);
        m_bibleModule->loadBibleData(moduleID, path);
        if(m.moduleName.isEmpty())
            m_moduleTitle = m_bibleModule->moduleName(false);
        else
            m_moduleTitle = m.moduleName;
        m_moduleShortTitle = m_bibleModule->moduleName(true);

        bookCount = m_bibleModule->bookCount();
        m_names = m_bibleModule->getBookNames();

        m_bookPath = ((BibleQuote *)m_bibleModule)->m_bookPath;
        m_modulePath = m_bibleModule->modulePath();
        //ModuleCache
        m_settings->setTitle(path, m_moduleTitle);
        m_settings->setBookCount(path, bookCount);
        m_settings->setBookNames(path, m_names.m_bookFullName);

        break;
    }
    case Module::ZefaniaBibleModule: case Module::TheWordBibleModule:{
        if(m_module->m_bibleModule) {
            m_bibleModule = m_module->m_bibleModule;
        } else {
            m_bibleModule = new ZefaniaBible();
            m_module->m_bibleModule = m_bibleModule;
        }
        ModuleSettings m = m_settings->getModuleSettings(m_moduleID);
        m_bibleModule->setSettings(m_settings);
        m_bibleModule->loadBibleData(moduleID, path);
        m_moduleTitle = m.moduleName;

        bookCount = m_bibleModule->bookCount();
        m_names = m_bibleModule->getBookNames();
        m_modulePath = m_bibleModule->modulePath();
        //ModuleCache
        m_settings->setTitle(path, m_moduleTitle);
        m_settings->setBookCount(path, bookCount);
        m_settings->setBookNames(path, m_names.m_bookFullName);

        break;
    }
    default:
        return 1;
    }
    myDebug() << "loaded bible";
    m_loaded = true;
    return 0;
    // CALLGRIND_STOP_INSTRUMENTATION;
}
/**
  Load only the book without pharsing.
*/
int Bible::readBook(int id)
{
    DEBUG_FUNC_NAME
    m_bookID = id;
    switch(m_module->m_moduleType) {
    case Module::BibleQuoteModule: {
        m_book.clear();
        m_chapterNames.clear();
        if(id < m_bookPath.size()) {
            int r = m_bibleModule->readBook(id);
            if(r != 0)
                return r;
        } else {
            myWarning() << "index out of range bookPath.size() = " << m_bookPath.size() << " , id = " << id;
            return 1;
        }
        m_book = m_bibleModule->book();

        int cc = bookCount[id];
        int start = 1;
        if(((BibleQuote *)m_bibleModule)->m_chapterZero) {
            start = 0;
        }
        for(int i = start; i < cc + start; ++i) {
            m_chapterNames << QString::number(i);
        }
        break;
    }
    case Module::ZefaniaBibleModule: case Module::TheWordBibleModule: {
        myDebug() << "read zefania bible with id = " << id;
        m_book.clear();
        m_chapterNames.clear();
        m_bibleModule->readBook(id);
        m_book = m_bibleModule->book();
        for(int i = 1; i <= m_bibleModule->bookCount().value(id, 0); ++i) {
            m_chapterNames << QString::number(i);
        }
        break;
    }
    default:
        return 1;
    }
    //myDebug() << "read book";
    return 0;
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
TextRange Bible::readRange(const Range &range, bool ignoreModuleID)
{
    DEBUG_FUNC_NAME
    myDebug() << m_loaded;
    //todo: what about biblequote
    TextRange ret;
    if((range.moduleID() != m_moduleID && !ignoreModuleID) || !m_loaded) {
        loadModuleData(range.moduleID());
    }

    int bookID = -1;
    if(range.book() == RangeEnum::BookByID) {
        bookID = range.bookID();
    } else if(range.book() == RangeEnum::FirstBook) {
        foreach(int id, bookIDs()) {
            if(id < bookID || bookID == -1) {
                bookID = id;
            }
        }
        if(bookID == -1)
            bookID = 0;
    } else if(range.book() == RangeEnum::LastBook) {
        foreach(int id, bookIDs()) {
            if(id > bookID || bookID == -1) {
                bookID = id;
            }
        }
        if(bookID == -1)
            bookID = 0;
    } else if(range.book() == RangeEnum::CurrentBook) {

        if(m_lastTextRanges != 0 && !m_lastTextRanges->isEmpty() && !m_lastTextRanges->booksIDs().isEmpty()) {
            bookID = *m_lastTextRanges->booksIDs().begin();
        } else {
            foreach(int id, bookIDs()) {
                if(id < bookID || bookID == -1) {
                    bookID = id;
                }
            }
        }
        myDebug() << "current book " << bookID;
    }

    if(bookID < 0) {
        myWarning() << "index out of range index bookID = " << bookID;
        return ret;
    }

    if(m_book.bookID() != bookID) {
        readBook(bookID);
    }

    const ModuleSettings moduleSettings = m_settings->getModuleSettings(m_moduleID);

    int chapterID = -1;
    if(range.chapter() == RangeEnum::ChapterByID) {
        chapterID = range.chapterID();
    } else if(range.chapter() == RangeEnum::FirstChapter) {
        foreach(int id,  m_book.m_chapters.keys()) {
            if(chapterID == -1 || id < chapterID)
                chapterID = id;
        }
        if(chapterID == -1)
            chapterID = 0;
    } else if(range.chapter() == RangeEnum::LastChapter) {
        foreach(int id,  m_book.m_chapters.keys()) {
            if(chapterID == -1 || id > chapterID)
                chapterID = id;
        }
        if(chapterID == -1)
            chapterID = 0;
    } else if(range.chapter() == RangeEnum::CurrentChapter) {
        if(m_lastTextRanges != 0  && !m_lastTextRanges->isEmpty() && !m_lastTextRanges->chapterIDs().isEmpty()) {
            chapterID = *m_lastTextRanges->chapterIDs().begin();
        } else {
            foreach(int id,  m_book.m_chapters.keys()) {
                if(chapterID == -1 || id < chapterID)
                    chapterID = id;
            }
        }
        myDebug() << "current chapter = " << chapterID;
    }
    if(!m_book.hasChapter(chapterID)) {
        myDebug() << "size = " << m_book.size();
        myWarning() << "index out of range index chapterID = " << chapterID;
        return ret;
    }

    const Chapter c = m_book.getChapter(chapterID);
    ret.setBookID(bookID);
    ret.setChapterID(chapterID);
    int startVerse = 0;
    int endVerse = 0;
    QHash<int, Verse> data = c.getData();
    int max = 0;
    int min = 0;

    foreach(const int & key, data.keys()) {
        if(key >= max) {
            max = key;
        } else if(key <= min) {
            min = key;
        }
    }

    if(range.startVerse() == RangeEnum::VerseByID) {
        startVerse = range.startVerseID();
    } else if(range.startVerse() == RangeEnum::FirstVerse) {
        startVerse = min;
    } else if(range.startVerse() == RangeEnum::LastVerse) {
        startVerse = max;
    }

    if(range.endVerse() == RangeEnum::VerseByID) {
        endVerse = range.endVerseID();
    } else if(range.endVerse() == RangeEnum::FirstVerse) {
        endVerse = min;
    } else if(range.endVerse() == RangeEnum::LastVerse) {
        endVerse = max;
    }

    QMap<int, Verse> verseMap;
    bool currentVerse = false;
    for(int verseCounter = startVerse; verseCounter <= endVerse; verseCounter++) {
        if(!data.contains(verseCounter))
            continue; //todo: or should i better add an empty verse?
        Verse verse = data.value(verseCounter);

        //main formatting
        if(m_notes != 0 && m_bibleDisplaySettings->showNotes() == true) {
            for(int n = 0; n < m_notes->getIDList().size(); ++n) {
                const QString noteID = m_notes->getIDList().at(n);
                if(m_notes->getType(noteID) == "text") {
                    const QString link = m_notes->getRef(noteID, "link");
                    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::None, link);
                    urlConverter.setSettings(m_settings);
                    urlConverter.setModuleMap(m_map);
                    urlConverter.pharse();
                    if(urlConverter.m_moduleID == m_moduleID && urlConverter.m_bookID == m_bookID && urlConverter.m_chapterID == chapterID && urlConverter.m_verseID == verseCounter) {
                        verse.append("<a href=\"note://" + noteID + "\"><img src=\":/icons/16x16/view-pim-notes.png\" title=\"" + m_notes->getRef(noteID, "title") + "\"></a>");
                    }
                }
            }
        }


        if(range.selectedVerse().contains(verseCounter)) {
            if(!currentVerse) {
                currentVerse = true;//todo: cuurently the first selected entry is the current entry
                //change this to provide maybe more future features
                verse.prepend("<span id = \"currentEntry\" class = \"selectedEntry\"> ");
            } else {
                verse.prepend("<span class = \"selectedEntry\">");
            }
            verse.append("</span>");
        }

        {
            QString prepend;
            QString append;
            prepend = "<span class=\"verseNumber\">" + QString::number(verse.verseID() + 1) + "</span> ";
            if(moduleSettings.zefbible_textFormatting == 0) {
                append = "<br />";
            } else {
                append = "\n";//not visible line break
            }

            verse.prepend(prepend);
            verse.append(append);
        }
        verseMap.insert(verse.verseID(), verse);
    }

    //todo: insert marks

    if(m_notes != 0 && m_bibleDisplaySettings->showMarks() == true) {
        //myDebug() << "insert notes";
        VerseReplacer replacer;
        for(int n = 0; n <  m_notes->getIDList().size(); ++n) {
            const QString noteID = m_notes->getIDList().at(n);
            if(m_notes->getType(noteID) == "mark") {
                const QString link = m_notes->getRef(noteID, "link");
                //myDebug() << "link = " << link;
                UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::None, link);
                urlConverter.setSettings(m_settings);
                urlConverter.setModuleMap(m_map);
                urlConverter.pharse();

                const QString pre = "<span class=\"mark\" style=\"" + m_notes->getRef(noteID, "style") + "\">";
                const QString ap = "</span>";
                if(urlConverter.m_moduleID == m_moduleID && urlConverter.m_bookID == bookID && urlConverter.m_chapterID == chapterID) {
                    //myDebug() << "insert note id = " << noteID << " link " << link;
                    if(m_notes->getRef(noteID, "start") == m_notes->getRef(noteID, "end")) {
                        VerseSelection::SelectionPosInTextType type = VerseSelection::typeFromString(m_notes->getRef(noteID, "selection_pos_type"));
                        if(type == VerseSelection::ShortestString) {
                            const int verseID = m_notes->getRef(noteID, "start").toInt();
                            if(endVerse != -1) {
                                //todo: do something
                            }
                            Verse verse = verseMap.value(verseID);
                            const int startPos = verse.data().lastIndexOf(m_notes->getRef(noteID, "startString"));
                            const int endPos = verse.data().lastIndexOf(m_notes->getRef(noteID, "endString")) + m_notes->getRef(noteID, "endString").size();
                            replacer.setInsert(verseID, endPos, ap);
                            replacer.setInsert(verseID, startPos, pre);
                        } else if(type == VerseSelection::RepeatOfLongestString) {

                        }

                    } else {

                        int startVersID = m_notes->getRef(noteID, "start").toInt();
                        int endVersID = m_notes->getRef(noteID, "end").toInt();
                        if(endVerse != -1) {
                            //todo: do something
                        }
                        if(startVersID > 0 && endVersID > 0 /*&& startVersID < versList.size() && endVersID < versList.size()*/) {
                            Verse startVerse = verseMap.value(startVersID);
                            Verse endVerse = verseMap.value(endVersID);
                            const int startPos = startVerse.data().lastIndexOf(m_notes->getRef(noteID, "startString"));
                            const int endPos = endVerse.data().lastIndexOf(m_notes->getRef(noteID, "endString")) + m_notes->getRef(noteID, "endString").size();


                            replacer.setInsert(startVersID, startPos, pre);
                            replacer.setAppend(startVersID, ap);
                            for(int i = startVersID + 1; i < endVersID; i++) {
                                replacer.setAppend(i, ap);
                                replacer.setPrepend(i, pre);

                            }
                            replacer.setInsert(endVersID, endPos, ap);
                            replacer.setPrepend(endVersID, pre);

                        }
                    }

                }
            }
        }
        replacer.exec(&verseMap);
    }
    // now add id
    //it have to be done as last
    QMapIterator<int, Verse> i(verseMap);
    while(i.hasNext()) {
        i.next();
        Verse verse = i.value();

        switch(m_moduleType) {
        case Module::BibleQuoteModule: {
            if(i.key() > 1) {//because of the chapter
                verse.prepend("<span verseID='" + QString::number(i.key() - 1) +
                              "' chapterID='" + QString::number(chapterID) +
                              "' bookID='" + QString::number(bookID) +
                              "' moduleID='" + QString::number(m_moduleID) + "'>\n");
                verse.append("</span><br />\n");
            }
            break;

        }
        case Module::ZefaniaBibleModule: case Module::TheWordBibleModule: {
            verse.prepend("<span verseID='" + QString::number(i.key()) +
                          "' chapterID='" + QString::number(chapterID) +
                          "' bookID='" + QString::number(bookID) +
                          "' moduleID='" + QString::number(m_moduleID) + "'>\n");
            verse.append("</span>\n");
            break;
        }
        default:
            break;
        }
        ret.addVerse(verse);

    }
    return ret;

}
TextRanges Bible::readRanges(const Ranges &ranges, bool ignoreModuleID)
{
    TextRanges textRanges;
    foreach(const Range & r, ranges.getList()) {
        textRanges.addTextRange(readRange(r, ignoreModuleID));
    }
    return textRanges;
}

/**
  Search in the current bible. Returns the Results.
  */
void Bible::search(SearchQuery query, SearchResult *result)
{
    DEBUG_FUNC_NAME
    if(!m_loaded)
        return;
    m_lastSearchQuery = query;
    if(!m_bibleModule->hasIndex())
        m_bibleModule->buildIndex();
    m_bibleModule->search(query, result);
    m_lastSearchQuery = query;
    myDebug() << "hits.size() = " << result->hits().size();
}
QStringList Bible::getSearchPaths() const
{
    if(m_moduleType == Module::BibleQuoteModule) {
        QStringList l;
        l.append(QString(m_modulePath + QDir::separator()));
        if(m_bookID < m_bookPath.size()) {
            QString p = m_bookPath.at(m_bookID);
            const int pos = p.lastIndexOf(QDir::separator());
            if(pos != -1) {
                p = p.remove(pos, p.size());
            }
            if(!p.startsWith(m_modulePath)) {
                p = m_modulePath + QDir::separator() + p + QDir::separator();
            }
            l.append(p);
        }
        return l;
    }
    return QStringList();
}

int Bible::bookID() const
{
    return m_bookID;
}

int Bible::chapterID() const
{
    return m_chapterID;
}

int Bible::booksCount() const
{
    return m_names.m_bookFullName.size();
}

int Bible::chaptersCount() const
{
    return m_book.size();
}

QString Bible::modulePath()
{
    return m_modulePath;
}

QString Bible::moduleTitle()
{
    return m_moduleTitle;
}

QString Bible::moduleShortTitle()
{
    if(m_moduleShortTitle.isEmpty())
        return moduleTitle();
    return m_moduleShortTitle;
}

QHash<int, QString> Bible::bookFullNames()
{
    return m_names.m_bookFullName;
}

QHash<int, QStringList> Bible::bookShortNames()
{
    return m_names.m_bookShortName;
}

QStringList Bible::bookPath()
{
    return m_bookPath;
}

QStringList Bible::chapterNames()
{
    return m_chapterNames;
}

Module::ModuleType Bible::bibleType() const
{
    return m_moduleType;
}

int Bible::verseID() const
{
    return m_verseID;
}
SearchQuery Bible::lastSearchQuery()
{
    return m_lastSearchQuery;
}

QList<int> Bible::bookIDs()
{
    return m_names.m_bookIDs;
}
QString Bible::bookName(const int &bookID, bool preferShort)
{
    //todo: use preferShort
    if(m_names.m_bookShortName.value(bookID).size() != 0) {
        if(preferShort) {
            return m_names.m_bookShortName.value(bookID).first();
        } else {
            return m_names.m_bookFullName.value(bookID, m_names.m_bookShortName.value(bookID).first());
        }
    } else {
        return m_names.m_bookFullName.value(bookID);
    }
}
QHash<int, QString> Bible::bookNames(bool preferShort)
{
    if(preferShort) {
        if(m_names.m_bookShortName.size() != 0) {
            QHash<int, QString> ret;
            QHashIterator<int, QStringList> i(m_names.m_bookShortName);
            while(i.hasNext()) {
                ret[i.key()] = i.value().first();
            }
            return ret;
        } else {
            return m_names.m_bookFullName;
        }
    } else {
        if(m_names.m_bookFullName.size() != 0) {
            return m_names.m_bookFullName;
        } else {
            QHash<int, QString> ret;
            QHashIterator<int, QStringList> i(m_names.m_bookShortName);
            while(i.hasNext()) {
                ret[i.key()] = i.value().first();
            }
            return ret;
        }
    }
}
void Bible::setLastTextRanges(TextRanges *textRanges)
{
    m_lastTextRanges = textRanges;
}
TextRanges *Bible::lastTextRanges()
{
    return m_lastTextRanges;
}
