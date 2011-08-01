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
//#include <valgrind/callgrind.h>
Bible::Bible()
{
    m_bookID = 0;
    m_loaded = false;
    m_bibleModule = 0;
    m_lastTextRanges = 0;
    m_versification = 0;
}
Bible::~Bible()
{
    //Bible do not delete anything!
}



bool Bible::loaded() const
{
    return m_loaded;
}

int Bible::loadModuleData(const int moduleID)
{
    DEBUG_FUNC_NAME;
    myDebug() << "moduleID = " << moduleID;
    //m_versification = 0;
    m_module = m_map->m_map.value(moduleID, NULL);

    //not valid module
    if(moduleID < 0 || m_module == NULL) {
        myWarning() << "invalid bibleID = " << moduleID;
        return 1;
    }

    m_moduleType = m_module->moduleType();
    if(moduleType() == OBVCore::NoneType || m_module->moduleClass() != OBVCore::BibleModuleClass) {
        myDebug() << "invalid type " << moduleType();
        return 2;
    }
    m_moduleID = moduleID;
    ModuleSettings *m = m_settings->getModuleSettings(m_moduleID);

    m_bookPath.clear();
    m_modulePath.clear();
    int loaded = 1;
    if(moduleType() == OBVCore::BibleQuoteModule) {
        if(m_module->m_bibleModule) {
            m_bibleModule = m_module->m_bibleModule;
        } else {
            m_bibleModule = new BibleQuote();
            m_module->m_bibleModule = m_bibleModule;
        }

        m_bibleModule->setSettings(m_settings);
        loaded = m_bibleModule->loadBibleData(m_moduleID, m_module->path());

        m_bookPath = ((BibleQuote *)m_bibleModule)->m_bookPath;
        m_modulePath = m_bibleModule->modulePath();
    } else if(moduleType() == OBVCore::ZefaniaBibleModule || moduleType() == OBVCore::TheWordBibleModule || moduleType() == OBVCore::SwordBibleModule) {
        if(m_module->m_bibleModule) {
            m_bibleModule = m_module->m_bibleModule;
        } else {
            if(moduleType() == OBVCore::ZefaniaBibleModule) {
                m_bibleModule = new ZefaniaBible();
            } else if(moduleType() == OBVCore::TheWordBibleModule) {
                m_bibleModule = new TheWordBible();
            } else if(moduleType() == OBVCore::SwordBibleModule) {
                m_bibleModule = new SwordBible();
            }
            m_module->m_bibleModule = m_bibleModule;

        }
        m_bibleModule->setSettings(m_settings);
        loaded = m_bibleModule->loadBibleData(m_moduleID, m_module->path());
        myDebug() << "loaded = " << loaded;
    }
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
/**
 * Load only the book without pharsing.
 */
int Bible::readBook(const int id)
{
    DEBUG_FUNC_NAME
    m_bookID = id;
    return m_bibleModule->readBook(id);
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
    //myDebug() << "loaded = " << m_loaded;

    TextRange ret;
    ret.setModuleID(range.moduleID());
    if(ignoreModuleID) {
        if(!m_loaded) {
            loadModuleData(m_moduleID);
        }
    } else {
        if(range.moduleID() != m_moduleID || !m_loaded) {
            loadModuleData(range.moduleID());
        }
    }
    if(!m_loaded) {
        ret.setFailed(true);
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
        ret.setFailed(true);
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
    myDebug() << "bookID = " << bookID << " chapterID " << chapterID;
    std::pair<int, int> minMax = m_bibleModule->minMaxVerse(bookID, chapterID);
    myDebug() << "min = " << minMax.first << " max = " << minMax.second;

    int startVerse = 0;
    int endVerse = 0;
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

    myDebug() << "startVerse = " << startVerse << " endVerse = " << endVerse;

    TextRange rawRange = m_bibleModule->rawTextRange(bookID, chapterID, startVerse, endVerse);
    ret.setBookID(bookID);
    ret.setChapterID(chapterID);
    ret.setModuleID(m_moduleID);
    QMap<int, Verse> verseMap = rawRange.verseMap();

    bool currentVerse = false;

    QMutableMapIterator<int, Verse> it(verseMap);
    while(it.hasNext()) {
        it.next();
        Verse verse = it.value();

        //main formatting
        if(m_notes != 0 && m_moduledisplaysettings->showNotes() == true) {
            for(int n = 0; n < m_notes->getIDList().size(); ++n) {
                const QString noteID = m_notes->getIDList().at(n);
                if(m_notes->getType(noteID) == "text") {
                    const QString link = m_notes->getRef(noteID, "link");
                    VerseUrl url;
                    url.fromString(link);
                    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
                    urlConverter.setSettings(m_settings);
                    urlConverter.setModuleMap(m_map);
                    VerseUrl newUrl = urlConverter.convert();
                    if(newUrl.contains(m_moduleID, bookID, chapterID, it.key())) {
                        //myDebug() << "append note icon";
                        verse.append("<a href='note://" + noteID + "'><img src='qrc:/icons/16x16/view-pim-notes.png' class='noteIcon' title='" + m_notes->getTitle(noteID) + "' /></a>");
                    }
                }
            }
        }

        if(moduleType() == OBVCore::TheWordBibleModule || moduleType() == OBVCore::ZefaniaBibleModule || moduleType() == OBVCore::SwordBibleModule) {
            QString prepend;
            QString append;
            prepend = "<span class=\"verseNumber\">" + QString::number(verse.verseID() + 1) + "</span> ";
            if(moduleSettings->zefbible_textFormatting == ModuleSettings::NewLine) {
                append = "<br />";
            } else {
                append = "\n";//not visible line break
            }

            verse.prepend(prepend);
            verse.append(append);
        } else if(moduleType() == OBVCore::BibleQuoteModule) {
        }

        if(range.selectedVerse().contains(it.key())) {
            if(!currentVerse) {
                currentVerse = true;//todo: currently the first selected entry is the current entry
                //change this to provide maybe more future features
                verse.prepend("<div id = \"currentEntry\" class = \"selectedEntry\"> ");
            } else {
                verse.prepend("div class = \"selectedEntry\">");
            }
            verse.append("</div>");
        }


        //replace
        it.setValue(verse);
    }

    if(m_notes != 0 && m_moduledisplaysettings->showMarks() == true) {
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
                urlConverter.setModuleMap(m_map);
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
    // now add id
    //it have to be done as last
    QMapIterator<int, Verse> i(verseMap);
    if(moduleType() == OBVCore::BibleQuoteModule) {
        const QString pre = "<span verseID='";

        const QString pre2 = "' chapterID='" + QString::number(chapterID) +
                             "' bookID='" + QString::number(bookID) +
                             "' moduleID='" + QString::number(m_moduleID) + "'>\n";

        const QString ap = "</span><br />\n";
        while(i.hasNext()) {
            i.next();
            Verse verse = i.value();

            if(i.key() >= 1) {//because of the chapter
                verse.prepend(pre + QString::number(i.key() - 1) + pre2);
                verse.append(ap);
            }
            ret.addVerse(verse);
            //myDebug() << verse.data();
        }

    } else if(moduleType() == OBVCore::ZefaniaBibleModule || moduleType() == OBVCore::TheWordBibleModule || moduleType() == OBVCore::SwordBibleModule) {
        const QString pre = "<span verseID='";
        const QString pre2 =  "' chapterID='" + QString::number(chapterID) +
                              "' bookID='" + QString::number(bookID) +
                              "' moduleID='" + QString::number(m_moduleID) + "'>\n";
        const QString ap = "</span>\n";
        while(i.hasNext()) {
            i.next();
            Verse verse = i.value();

            verse.prepend(pre + QString::number(i.key()) + pre2);
            verse.append(ap);
            ret.addVerse(verse);
        }
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

void Bible::search(SearchQuery query, SearchResult *result)
{
    DEBUG_FUNC_NAME
    if(!m_loaded)
        return;

    m_lastSearchQuery = query;

    if(!m_bibleModule->hasIndex())
        m_bibleModule->buildIndex();

    m_bibleModule->search(query, result);
    //myDebug() << "hits.size() = " << result->hits().size();
}
/**
 * Used only by BibleQuote modules.
 * @returns A list of paths, where images can be found.
 */
QStringList Bible::getSearchPaths() const
{
    if(moduleType() == OBVCore::BibleQuoteModule) {
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

QString Bible::moduleUID() const
{
    return m_moduleUID;
}

QString Bible::moduleTitle() const
{
    return m_moduleTitle;
}

QString Bible::moduleShortTitle() const
{
    if(m_moduleShortTitle.isEmpty())
        return moduleTitle();
    return m_moduleShortTitle;
}

QStringList Bible::bookPath() const
{
    return m_bookPath;
}

QList<int> Bible::bookIDs() const
{
    return m_versification->bookNames().keys();
}
BibleModule * Bible::module()
{
    return m_bibleModule;
}
