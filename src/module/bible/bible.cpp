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

void Bible::setBibleDisplaySettings(BibleDisplaySettings *bibleDisplaySettings)
{
    m_bibleDisplaySettings = bibleDisplaySettings;
}

bool Bible::loaded() const
{
    return m_loaded;
}

int Bible::loadModuleData(const int &moduleID)
{
    DEBUG_FUNC_NAME
    //m_versification = 0;
    m_module = m_map->m_map.value(moduleID, 0);

    //not valid module
    if(moduleID < 0 || !m_module) {
        myWarning() << "invalid bibleID = " << moduleID;
        return 1;
    }

    m_moduleType = m_module->moduleType();
    if(moduleType() == Module::NoneType || m_module->moduleClass() != Module::BibleModuleClass) {
        myDebug() << "invalid type " << moduleType();
        return 2;
    }
    m_moduleID = moduleID;
    ModuleSettings m = m_settings->getModuleSettings(m_moduleID);

    m_book.clear();
    m_bookPath.clear();
    m_modulePath.clear();

    if(moduleType() == Module::BibleQuoteModule) {
        if(m_module->m_bibleModule) {
            m_bibleModule = m_module->m_bibleModule;
        } else {
            m_bibleModule = new BibleQuote();
            m_module->m_bibleModule = m_bibleModule;
        }

        m_bibleModule->setSettings(m_settings);
        m_bibleModule->loadBibleData(m_moduleID, m_module->path());

        m_bookPath = ((BibleQuote *)m_bibleModule)->m_bookPath;
        m_modulePath = m_bibleModule->modulePath();
    } else if(moduleType() == Module::ZefaniaBibleModule || moduleType() == Module::TheWordBibleModule) {
        if(m_module->m_bibleModule) {
            m_bibleModule = m_module->m_bibleModule;
        } else {
            if(moduleType() == Module::ZefaniaBibleModule) {
                m_bibleModule = new ZefaniaBible();
            } else if(moduleType() == Module::TheWordBibleModule) {
                m_bibleModule = new TheWordBible();
            }
            m_module->m_bibleModule = m_bibleModule;
        }
        m_bibleModule->setSettings(m_settings);
        m_bibleModule->loadBibleData(m_moduleID, m_module->path());
    }
    myDebug() << "setting an another versification";
    m_versification = m_bibleModule->versification();
    m_moduleUID = m_bibleModule->uid();

    if(m.moduleName.isEmpty())
        m_moduleTitle = m_bibleModule->moduleName(false);
    else
        m_moduleTitle = m.moduleName;
    m_moduleShortTitle = m_bibleModule->moduleName(true);

    m_settings->setTitle(m_module->path(), m_moduleTitle);
    //todo: port to set cache
    m_loaded = true;
    return 0;
}

int Bible::readBook(const int &id)
{
    DEBUG_FUNC_NAME
    m_bookID = id;
    m_book.clear();
    m_chapterNames.clear();

    if(moduleType() == Module::BibleQuoteModule) {
        if(id < m_bookPath.size()) {
            int r = m_bibleModule->readBook(id);
            if(r != 0)
                return r;
        } else {
            myWarning() << "index out of range bookPath.size() = " << m_bookPath.size() << " , id = " << id;
            return 1;
        }
        const int cc = m_versification->maxChapter().value(id);
        int start = 1;
        if(((BibleQuote *)m_bibleModule)->m_chapterZero) {
            start = 0;
        }
        for(int i = start; i < cc + start; ++i) {
            m_chapterNames << QString::number(i);
        }
    } else if(moduleType() == Module::ZefaniaBibleModule || moduleType() == Module::TheWordBibleModule) {
        m_bibleModule->readBook(id);
        myDebug() << m_versification;
        for(int i = 1; i <= m_versification->maxChapter().value(id, 0); ++i) {
            m_chapterNames << QString::number(i);
        }
    }

    m_book = m_bibleModule->book();

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

    TextRange ret;
    ret.setModuleID(range.moduleID());
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

        if(m_lastTextRanges != 0 && !m_lastTextRanges->isEmpty() && !m_lastTextRanges->bookIDs().isEmpty()) {
            bookID = *m_lastTextRanges->bookIDs().begin();
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
                    VerseUrl url;
                    url.fromString(link);
                    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
                    urlConverter.setSettings(m_settings);
                    urlConverter.setModuleMap(m_map);
                    VerseUrl newUrl = urlConverter.convert();
                    if(newUrl.contains(m_moduleID, bookID, chapterID, verseCounter)) {
                        //myDebug() << "append note icon";
                        verse.append("<a href='note://" + noteID + "'><img src='qrc:/icons/16x16/view-pim-notes.png' class='noteIcon' title='" + m_notes->getTitle(noteID) + "' /></a>");
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

        if(moduleType() == Module::TheWordBibleModule || moduleType() == Module::ZefaniaBibleModule) {
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
        } else if(moduleType() == Module::BibleQuoteModule) {
        }
        verseMap.insert(verse.verseID(), verse);
    }

    if(m_notes != 0 && m_bibleDisplaySettings->showMarks() == true) {
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

    if(moduleType() == Module::BibleQuoteModule) {
        const QString pre = "<span verseID='";
        const QString pre2 = "' chapterID='" + QString::number(chapterID) +
                             "' bookID='" + QString::number(bookID) +
                             "' moduleID='" + QString::number(m_moduleID) + "'>\n";
        const QString ap = "</span><br />\n";
        while(i.hasNext()) {
            i.next();
            Verse verse = i.value();
            if(i.key() > 1) {//because of the chapter
                verse.prepend(pre + QString::number(i.key() - 1) + pre2);
                verse.append(ap);
            }
            ret.addVerse(verse);
        }

    } else if(moduleType() == Module::ZefaniaBibleModule || moduleType() == Module::TheWordBibleModule) {
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
    m_lastSearchQuery = query;
    //myDebug() << "hits.size() = " << result->hits().size();
}
QStringList Bible::getSearchPaths() const
{
    if(moduleType() == Module::BibleQuoteModule) {
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


int Bible::booksCount() const
{
    return m_versification->bookCount();
}

int Bible::chaptersCount() const
{
    return m_book.size();
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


QStringList Bible::bookPath()
{
    return m_bookPath;
}

QStringList Bible::chapterNames()
{
    return m_chapterNames;
}



QString Bible::bookName(const int &bookID, bool preferShort)
{
    return m_versification->bookName(bookID, preferShort);
}


QList<int> Bible::bookIDs() const
{
    return m_versification->maxChapter().keys();
}
