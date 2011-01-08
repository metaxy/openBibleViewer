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
#include "biblemanager.h"

BibleManager::BibleManager(QObject *parent) :
    QObject(parent)
{
}
void BibleManager::setWidget(QWidget *p)
{
    m_p = p;
}
void BibleManager::init()
{
    connect(m_actions, SIGNAL(_get(VerseUrl)), this, SLOT(pharseUrl(VerseUrl)));
    connect(m_actions, SIGNAL(_previousChapter()), this, SLOT(previousChapter()));
    connect(m_actions, SIGNAL(_nextChapter()), this, SLOT(nextChapter()));
    connect(m_actions, SIGNAL(_loadBibleList(bool)), this, SLOT(loadBibleList(bool)));
    connect(m_actions, SIGNAL(_reshowCurrentRange()), this, SLOT(reshowCurrentRange()));
    connect(m_actions, SIGNAL(_reloadBible()), this, SLOT(reloadBible()));
}
void BibleManager::createDocks()
{
    m_moduleDockWidget = new ModuleDockWidget(m_p);
    setAll(m_moduleDockWidget);
    m_moduleDockWidget->init();

    m_bookDockWidget = new BookDockWidget(m_p);
    setAll(m_bookDockWidget);
    m_bookDockWidget->init();
    m_bookDockWidget->hide();

    m_quickJumpDockWidget = new QuickJumpDockWidget(m_p);
    setAll(m_quickJumpDockWidget);
    m_quickJumpDockWidget->init();
    m_quickJumpDockWidget->hide();
}

QHash<DockWidget*, Qt::DockWidgetArea> BibleManager::docks()
{
    //DEBUG_FUNC_NAME
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.insert(m_bookDockWidget, Qt::LeftDockWidgetArea);
    ret.insert(m_moduleDockWidget, Qt::LeftDockWidgetArea);
    ret.insert(m_quickJumpDockWidget, Qt::RightDockWidgetArea);
    return ret;

}
Ranges BibleManager::bibleUrlRangeToRanges(VerseUrlRange range)
{
    //DEBUG_FUNC_NAME
    Ranges ranges;
    Range r;
    //todo: currently we do not support real ranges but its ok
    if(range.bible() == VerseUrlRange::LoadBibleByID) {
        r.setModule(range.bibleID());
    } else if(range.bible() == VerseUrlRange::LoadCurrentModule) {
        r.setModule(m_moduleManager->verseModule()->moduleID());
    }

    if(range.book() == VerseUrlRange::LoadFirstBook) {
        r.setBook(RangeEnum::FirstBook);
    } else if(range.book() == VerseUrlRange::LoadLastBook) {
        r.setBook(RangeEnum::LastBook);
    } else if(range.book() == VerseUrlRange::LoadCurrentBook) {
        r.setBook(RangeEnum::CurrentBook);
    } else {
        r.setBook(range.bookID());
    }

    if(range.startChapter() == VerseUrlRange::LoadFirstChapter) {
        r.setChapter(RangeEnum::FirstChapter);
    } else if(range.startChapter() == VerseUrlRange::LoadLastChapter) {
        r.setChapter(RangeEnum::LastChapter);
    } else if(range.startChapter() == VerseUrlRange::LoadCurrentChapter) {
        r.setChapter(RangeEnum::CurrentChapter);
    } else {
        r.setChapter(range.startChapterID());
    }

    if(range.startVerse() == VerseUrlRange::LoadFirstVerse) {
        r.setStartVerse(RangeEnum::FirstVerse);
    } /*else if(range.startVerse() == BibleUrlRange::LoadCurrentVerse) {
        r.setStartVerse(m_moduleManager->bible()->verseID());
    } */else if(range.startVerse() == VerseUrlRange::LoadLastVerse) {
        r.setStartVerse(RangeEnum::LastVerse);
    } else {
        r.setStartVerse(range.startVerseID());
    }

    if(range.endVerse() == VerseUrlRange::LoadFirstVerse) {
        r.setEndVerse(RangeEnum::FirstVerse);
    } /*else if(range.endVerse() == BibleUrlRange::LoadCurrentVerse) {
        r.setEndVerse(m_moduleManager->bible()->verseID());
    } */else if(range.endVerse() == VerseUrlRange::LoadLastVerse) {
        r.setEndVerse(RangeEnum::LastVerse);
    } else {
        r.setEndVerse(range.endVerseID());
    }
    /* if(range.activeVerse() == BibleUrlRange::LoadFirstVerse) {
         //r.setEndVerse(RangeEnum::FirstVerse);
     } else if(range.activeVerse() == BibleUrlRange::LoadCurrentVerse) {
         //r.setEndVerse(m_moduleManager->bible()->verseID());
     } else if(range.activeVerse() == BibleUrlRange::LoadLastVerse) {
         //r.setEndVerse(RangeEnum::LastVerse);
     } else {
         r.setSelectedVerse(range.activeVerseID());
     }*/
    if(range.activeVerse() == VerseUrlRange::LoadVerseByID) {
        r.setSelectedVerse(range.activeVerseID());
    }

    ranges.addRange(r);
    return ranges;
}
void BibleManager::pharseUrl(const VerseUrl &url)
{
    DEBUG_FUNC_NAME;
    myDebug() << "url = " << url.toString();
    m_actions->newSubWindowIfEmpty();
    Ranges ranges;
    foreach(VerseUrlRange range, url.ranges()) {
        ranges.addRanges(bibleUrlRangeToRanges(range));
    }
    showRanges(ranges, url);
}

void BibleManager::pharseUrl(const QString &url)
{
    DEBUG_FUNC_NAME;
    const QString bible = "verse://";
    const QString bq = "go";
    myDebug() << "url = " << url;
    if(url.startsWith(bible)) {
        m_actions->newSubWindowIfEmpty();
        VerseUrl bibleUrl;
        Ranges ranges;
        if(!bibleUrl.fromString(url)) {
            return;
        }

        foreach(VerseUrlRange range, bibleUrl.ranges()) {
            ranges.addRanges(bibleUrlRangeToRanges(range));
        }
        showRanges(ranges, bibleUrl);
    } else if(url.startsWith(bq)) {
        //its a biblequote internal link, but i dont have the specifications!!!
        /* QStringList internal = url.split(" ");
         const QString bibleID = internal.at(1);//todo: use it
         //myDebug() << "bibleID = " << bibleID;
         int bookID = internal.at(2).toInt() - 1;
         int chapterID = internal.at(3).toInt() - 1;
         int verseID = internal.at(4).toInt();
         if(bibleID != m_moduleManager->bible()->bibleID())
         {
             loadModuleDataByID(bibleID);
             readBookByID(bookID);
             setCurrentBook(bookID);
             showChapter(chapterID),verseID);
             setCurrentChapter(chapterID);
             //load bible
         }
         else if(bookID != m_moduleManager->bible()->bookID()) {
             readBookByID(bookID);
             setCurrentBook(bookID);
             showChapter(chapterID, verseID);
             setCurrentChapter(chapterID);
             //load book
         } else if(chapterID != m_moduleManager->bible()->chapterID()) {
             showChapter(chapterID, verseID);
             setCurrentChapter(chapterID);
             //load chapter
         } else {
             showChapter(chapterID, verseID);
             setCurrentChapter(chapterID);
         }*/
    }
}
void BibleManager::showRanges(const Ranges &ranges, const VerseUrl &url)
{
    std::pair<QString, TextRanges> r = m_moduleManager->verseTable()->readRanges(ranges);
    m_actions->showTextRanges(r.first, r.second, url);

    m_actions->updateChapters(m_moduleManager->verseModule()->chapterNames());
    m_actions->updateBooks(m_moduleManager->verseModule()->versification());
    m_actions->setCurrentBook(r.second.bookIDs());
    m_actions->setCurrentChapter(r.second.chapterIDs());
    m_actions->setTitle(m_moduleManager->verseModule()->moduleTitle());
}

void BibleManager::nextChapter()
{
    if(!m_moduleManager->bibleLoaded())
        return;
    if(m_moduleManager->verseModule()->lastTextRanges()->minChapterID() < m_moduleManager->verseModule()->chaptersCount() - 1) {
        VerseUrl bibleUrl;
        VerseUrlRange range;
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(VerseUrlRange::LoadCurrentBook);
        range.setChapter(m_moduleManager->verseModule()->lastTextRanges()->minChapterID() - 1);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    } else if(m_moduleManager->verseModule()->lastTextRanges()->minBookID() < m_moduleManager->verseModule()->booksCount() - 1) {
        VerseUrl bibleUrl;
        VerseUrlRange range;
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(m_moduleManager->verseModule()->lastTextRanges()->minBookID() + 1);
        range.setChapter(VerseUrlRange::LoadFirstChapter);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    }
}

void BibleManager::previousChapter()
{
    if(!m_moduleManager->bibleLoaded())
        return;
    if(m_moduleManager->verseModule()->lastTextRanges()->minChapterID() > 0) {
        VerseUrl bibleUrl;
        VerseUrlRange range;
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(VerseUrlRange::LoadCurrentBook);
        range.setChapter(m_moduleManager->verseModule()->lastTextRanges()->minChapterID() - 1);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    } else if(m_moduleManager->verseModule()->lastTextRanges()->minBookID() > 0) {
        VerseUrl bibleUrl;
        VerseUrlRange range;
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(m_moduleManager->verseModule()->lastTextRanges()->minBookID() - 1);
        range.setChapter(VerseUrlRange::LoadLastChapter);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    }
}
void BibleManager::loadBibleList(bool hadBible)
{
    if(hadBible) {
        VerseUrl url;
        VerseUrlRange range;
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(VerseUrlRange::LoadCurrentBook);
        range.setChapter(VerseUrlRange::LoadCurrentChapter);
        range.setWholeChapter();
        url.setParam("force", "true");
        url.addRange(range);
        m_actions->get(url);
    } else {
        VerseUrl url;
        VerseUrlRange range;
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(VerseUrlRange::LoadFirstBook);
        range.setChapter(VerseUrlRange::LoadFirstChapter);
        range.setWholeChapter();
        url.setParam("force", "true");
        url.addRange(range);
        m_actions->get(url);
    }
}
void BibleManager::reshowCurrentRange()
{
    DEBUG_FUNC_NAME
    if(!m_moduleManager->bibleLoaded())
        return;
    m_actions->get(*m_moduleManager->verseTable()->lastVerseUrl());
}
void BibleManager::reloadBible()
{
    DEBUG_FUNC_NAME
    //todo: make it
    myWarning() << "implement it";
}

BookDockWidget *BibleManager::bookDockWidget()
{
    return m_bookDockWidget;
}

ModuleDockWidget *BibleManager::moduleDockWidget()
{
    return m_moduleDockWidget;
}

QuickJumpDockWidget * BibleManager::quickJumpDockWidget()
{
    return m_quickJumpDockWidget;
}
