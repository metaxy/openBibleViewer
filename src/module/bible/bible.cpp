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
#include "bible.h"
#include "src/core/settings/modulesettings.h"
#include "src/core/settings/moduledisplaysettings.h"

#include "src/core/dbghelper.h"
#include "src/core/link/urlconverter.h"
#include "src/core/versereplacer.h"
#include "src/core/verseselection.h"
#include "src/module/bible/biblequote.h"
#include "src/module/bible/zefania-bible.h"
#include "src/module/bible/thewordbible.h"
#include "src/module/bible/swordbible.h"

#include <QtCore/QDir>
#include <QtGui/QTextDocument>

#include "src/module/response/textrangesresponse.h"
Bible::Bible()
{
    m_bookID = 0;
    m_loaded = false;
    m_lastTextRanges = 0;
}
Bible::~Bible()
{
    DEBUG_FUNC_NAME
    m_versification.clear();
    m_bibleModule.clear();
}

bool Bible::loaded() const
{
    return m_loaded;
}

int Bible::loadModuleData(const int moduleID)
{
    myDebug() << "moduleID = " << moduleID;
    m_module = m_map->module(moduleID);

    //not valid module
    if(moduleID < 0 || m_module == NULL) {
        myWarning() << "invalid bibleID = " << moduleID;
        return 1;
    }

    m_moduleType = m_module->moduleType();
    if(moduleType() == ModuleTools::NoneType || m_module->moduleClass() != ModuleTools::BibleModuleClass) {
        myDebug() << "invalid type " << moduleType();
        return 2;
    }
    m_moduleID = moduleID;

    ModuleSettings *m = m_settings->getModuleSettings(m_moduleID);

    if(m_module->m_bibleModule.isNull()) {
        m_bibleModule = m_module->newBibleModule(moduleType());
    } else {
        m_bibleModule = m_module->m_bibleModule;
    }
    if(m_bibleModule == NULL) {
        myWarning() << "invalid module";
        return 1;
    }
    m_bibleModule->setSettings(m_settings);
    int loaded = m_bibleModule->loadBibleData(m_moduleID, m_module->path());

    if(loaded != 0) {
        m_loaded = false;
        myWarning() << "Could not load module.";
        return 1;
    }

    m_versification = m_bibleModule->versification();
    m_moduleUID = m_bibleModule->uid();

    if(m->moduleName.isEmpty())
        m_moduleTitle = m_bibleModule->moduleName(false);
    else
        m_moduleTitle = m->moduleName;

    m_moduleShortTitle = m_bibleModule->moduleName(true);

    m_loaded = true;
    return 0;
}


QString Bible::toUniformHtml(const QString &string)
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
    //DEBUG_FUNC_NAME
    TextRange ret;
    if(!m_settings->getModuleSettings(m_moduleID)) {//check if module exists
        m_moduleID = -1;
        m_loaded = false;
        m_versification.clear();
        m_bibleModule.clear();
        ret.setError(TextRange::FatalError);
        return ret;
    }

    ret.setModuleID(range.moduleID());
    if(ignoreModuleID) {
        if(!loaded()) {
            loadModuleData(m_moduleID);
        }
    } else {
        if(range.moduleID() != m_moduleID || !loaded()) {
            loadModuleData(range.moduleID());
        }
    }

    if(!loaded()) {
        ret.setError(TextRange::FatalError);
        return ret;
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
        if(m_lastTextRanges != 0 && !m_lastTextRanges->isEmpty() && !m_lastTextRanges->bookIDs().isEmpty()) {
            bookID = *m_lastTextRanges->bookIDs().begin();
        } else {
            foreach(int id, bookIDs()) {
                if(id < bookID || bookID == -1) {
                    bookID = id;
                }
            }
            if(bookID == -1)
                bookID = 0;
        }
        //myDebug() << "current book " << bookID;
    }

    if(bookID < 0) {
        myWarning() << "index out of range index bookID = " << bookID;
        ret.setError(TextRange::NotFoundError);
        return ret;
    }
    m_bookID = bookID;

    const ModuleSettings *moduleSettings = m_settings->getModuleSettings(m_moduleID);

    int chapterID = -1;
    if(range.chapter() == RangeEnum::ChapterByID) {
        chapterID = range.chapterID();
    } else if(range.chapter() == RangeEnum::FirstChapter) {
        chapterID = 0;
    } else if(range.chapter() == RangeEnum::LastChapter) {
        chapterID = m_versification->maxChapter().value(bookID) - 1;
    } else if(range.chapter() == RangeEnum::CurrentChapter) {
        if(m_lastTextRanges != 0  && !m_lastTextRanges->isEmpty() && !m_lastTextRanges->chapterIDs().isEmpty()) {
            chapterID = *m_lastTextRanges->chapterIDs().begin();
        } else {
            chapterID = 0;
        }
        //myDebug() << "current chapter = " << chapterID;
    }
    //myDebug() << "bookID = " << bookID << " chapterID " << chapterID;
    std::pair<int, int> minMax = m_bibleModule->minMaxVerse(bookID, chapterID);
    //myDebug() << "min = " << minMax.first << " max = " << minMax.second;

    int startVerse = 0;
    int endVerse = 0;
    //myDebug() << range.startVerse() << range.endVerse();

    if(range.startVerse() == RangeEnum::VerseByID) {
        startVerse = range.startVerseID();
    } else if(range.startVerse() == RangeEnum::FirstVerse) {
        startVerse = minMax.first;
    } else if(range.startVerse() == RangeEnum::LastVerse) {
        startVerse = minMax.second;
    }

    if(range.endVerse() == RangeEnum::VerseByID) {
        endVerse = range.endVerseID();
    } else if(range.endVerse() == RangeEnum::FirstVerse) {
        endVerse = minMax.first;
    } else if(range.endVerse() == RangeEnum::LastVerse) {
        endVerse = minMax.second;
    }
    if(endVerse == -1)
        endVerse = startVerse;

    //todo: hack for biblequote
    if(moduleType() == ModuleTools::BibleQuoteModule)  {
        endVerse++;
        if(startVerse != 0)
            startVerse++;
    }

    //myDebug() << "startVerse = " << startVerse << " endVerse = " << endVerse;
    //myDebug() << "selected verse = " << range.selectedVerse();

    TextRange rawRange = m_bibleModule->rawTextRange(bookID, chapterID, startVerse, endVerse);
    ret.setBookID(bookID);
    ret.setChapterID(chapterID);
    ret.setModuleID(m_moduleID);

    if(rawRange.error() != TextRange::NoError) {
        return rawRange;
    }
    QMap<int, Verse> verseMap = rawRange.verseMap();

    bool currentVerse = false;

    //formatting
    QMutableMapIterator<int, Verse> it(verseMap);
    while(it.hasNext()) {
        it.next();
        //main formatting
        if(m_notes != 0 && m_moduleDisplaySettings->showNotes() == true) {
            for(int n = 0; n < m_notes->getIDList().size(); ++n) {
                const QString noteID = m_notes->getIDList().at(n);
                if(m_notes->getType(noteID) == "text") {
                    const QString link = m_notes->getRef(noteID, "link");
                    VerseUrl url;
                    url.fromString(link);
                    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
                    urlConverter.setSettings(m_settings);
                    urlConverter.setModuleMap(m_map.data());
                    VerseUrl newUrl = urlConverter.convert();
                    if(newUrl.contains(m_moduleID, bookID, chapterID, it.key())) {
                        //myDebug() << "append note icon";
                        it.value().append("<a href='note://" + noteID + "'><img src='qrc:/icons/16x16/view-pim-notes.png' class='noteIcon' title='" + m_notes->getTitle(noteID) + "' /></a>");
                    }
                }
            }
        }

        if(moduleType() != ModuleTools::BibleQuoteModule) {
            QString prepend;
            QString append;
            prepend = "<span class=\"verseNumber\">" + QString::number(it.value().verseID() + 1) + "</span> ";
            if(moduleSettings->zefbible_textFormatting == ModuleSettings::NewLine) {
                append = "<br />";
            } else {
                append = "\n";//not visible line break
            }

            it.value().prepend(prepend);
            it.value().append(append);
        }
        //todo: ugly
        int add = 0;
        if(moduleType() == ModuleTools::BibleQuoteModule)
            add = 1;

        if(range.selectedVerse().contains(it.key() - add)) {
            if(!currentVerse) {
                currentVerse = true;//todo: currently the first selected entry is the current entry
                //change this to provide maybe more future features
                it.value().prepend("<div id = \"currentEntry\" class = \"selectedEntry\"> ");
            } else {
                it.value().prepend("<div class = \"selectedEntry\">");
            }
            it.value().append("</div>");
        }
    }

    //show marks
    if(m_notes != 0 && m_moduleDisplaySettings->showMarks() == true) {
        //myDebug() << "insert notes";
        VerseReplacer replacer;
        for(int n = 0; n <  m_notes->getIDList().size(); ++n) {
            const QString noteID = m_notes->getIDList().at(n);
            if(m_notes->getType(noteID) == "mark") {
                const QString link = m_notes->getRef(noteID, "link");
                //myDebug() << "link = " << link;
                VerseUrl url;
                url.fromString(link);
                UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
                urlConverter.setSettings(m_settings);
                urlConverter.setModuleMap(m_map.data());
                VerseUrl newUrl = urlConverter.convert();

                const QString pre = "<span class=\"mark\" style=\"" + m_notes->getRef(noteID, "style") + "\">";
                const QString ap = "</span>";
                if(newUrl.contains(m_moduleID, bookID, chapterID)) {
                    //myDebug() << "insert note id = " << noteID << " link " << link;
                    if(m_notes->getRef(noteID, "start") == m_notes->getRef(noteID, "end")) {
                        VerseSelection::SelectionPosInTextType type = VerseSelection::typeFromString(m_notes->getRef(noteID, "selection_pos_type"));
                        if(type == VerseSelection::ShortestString) {
                            const int verseID = m_notes->getRef(noteID, "start").toInt();
                            //if we show not the whole range
                            if(endVerse != -1) {
                                //todo: do something
                            }
                            Verse verse = verseMap.value(verseID);
                            const int startPos = verse.data().lastIndexOf(m_notes->getRef(noteID, "startString"));
                            const int endPos = verse.data().lastIndexOf(m_notes->getRef(noteID, "endString")) + m_notes->getRef(noteID, "endString").size();
                            replacer.setInsert(verseID, endPos, ap);
                            replacer.setInsert(verseID, startPos, pre);
                        } else if(type == VerseSelection::RepeatOfLongestString) {
                            myDebug() << "repeat of longest string noteID = " << noteID;
                            const int verseID = m_notes->getRef(noteID, "start").toInt();
                            Verse verse = verseMap.value(verseID);
                            const int repeat = m_notes->getRef(noteID, "repeat").toInt() + 1;
                            const QString longestString = m_notes->getRef(noteID, "longest_string");
                            int startPos = 0;
                            for(int i = 0; i < repeat; i++) {
                                startPos = verse.data().indexOf(longestString, startPos + 1);
                            }
                            if(startPos != -1) {
                                replacer.setInsert(verseID, startPos + longestString.size(), ap);
                                replacer.setInsert(verseID, startPos, pre);
                            }
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


    //now add id
    //it have to be done as last
    QMapIterator<int, Verse> i(verseMap);
    const QString pre = "<span class='verseEntry' verseID='";

    const QString pre2 = "' chapterID='" + QString::number(chapterID) +
                         "' bookID='" + QString::number(bookID) +
                         "' moduleID='" + QString::number(m_moduleID) + "'>\n";


    if(moduleType() == ModuleTools::BibleQuoteModule) {

        const QString ap = "</span><br />\n";
        while(i.hasNext()) {
            i.next();
            Verse verse = i.value();

            if(i.key() >= 1) {//because of the chapter
                verse.prepend(pre + QString::number(i.key() - 1) + pre2);
                verse.append(ap);
            }
            if(verse.layoutDirection() == Qt::RightToLeft) {
                verse.prepend("<span dir= \"rtl\">");
                verse.append("</span>");
            }
            ret.addVerse(verse);
            //myDebug() << verse.data();


        }



    } else {
        const QString pre = "<span class='verseEntry' verseID='";
        const QString pre2 =  "' chapterID='" + QString::number(chapterID) +
                              "' bookID='" + QString::number(bookID) +
                              "' moduleID='" + QString::number(m_moduleID) + "'>\n";
        const QString ap = "</span>\n";
        while(i.hasNext()) {
            i.next();
            Verse verse = i.value();

            verse.prepend(pre + QString::number(i.key()) + pre2);
            verse.append(ap);

            if(verse.layoutDirection() == Qt::RightToLeft) {
                verse.prepend("<span dir= \"rtl\">");
                verse.append("</span>");
            }
            ret.addVerse(verse);
        }
    }
    return ret;

}

Response* Bible::readRanges(const Ranges &ranges, bool ignoreModuleID)
{
    TextRanges textRanges;
    foreach(const Range & r, ranges.getList()) {
        textRanges.addTextRange(readRange(r, ignoreModuleID));
    }
    return new TextRangesResponse(textRanges);
}

void Bible::search(SearchQuery query, SearchResult *result)
{
    DEBUG_FUNC_NAME
    if(!loaded())
        return;

    m_lastSearchQuery = query;

    if(!m_bibleModule->hasIndex())
        m_bibleModule->buildIndex();

    m_bibleModule->search(query, result);
    myDebug() << "hits.size() = " << result->hits().size();
}
/**
 * Used only by BibleQuote modules.
 * @returns A list of paths, where images can be found.
 */
QStringList Bible::getSearchPaths() const
{
    if(moduleType() == ModuleTools::BibleQuoteModule) {
        const QStringList bookPath = ((BibleQuote *)m_bibleModule.data())->booksPath();
        const QString modulePath = m_bibleModule->modulePath();

        QStringList l;
        l.append(QString(modulePath + QDir::separator()));
        if(m_bookID < bookPath.size()) {
            QString p = bookPath.at(m_bookID);
            const int pos = p.lastIndexOf(QDir::separator());
            if(pos != -1) {
                p = p.remove(pos, p.size());
            }
            if(!p.startsWith(modulePath)) {
                p = modulePath + QDir::separator() + p + QDir::separator();
            }
            l.append(p);
        }
        return l;
    }
    return QStringList();
}

QString Bible::moduleUID() const
{
    return m_moduleUID;
}

QList<int> Bible::bookIDs() const
{
    return m_versification->bookNames().keys();
}
BibleModule * Bible::bibleModule() const
{
    return m_bibleModule.data();
}
void Bible::clearData()
{
    if(m_loaded)
        m_bibleModule->clearData();
}
