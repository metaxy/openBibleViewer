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
    connect(m_actions, SIGNAL(_get(BibleUrl)), this, SLOT(pharseUrl(BibleUrl)));
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
Ranges BibleManager::bibleUrlRangeToRanges(BibleUrlRange range)
{
    //DEBUG_FUNC_NAME
    Ranges ranges;
    Range r;
    //todo: currently we do not support real ranges but its ok
    if(range.bible() == BibleUrlRange::LoadBibleByID) {
        r.setModule(range.bibleID());
    } else if(range.bible() == BibleUrlRange::LoadCurrentBible) {
        r.setModule(m_moduleManager->bible()->moduleID());
    }

    if(range.book() == BibleUrlRange::LoadFirstBook) {
        r.setBook(RangeEnum::FirstBook);
    } else if(range.book() == BibleUrlRange::LoadLastBook) {
        r.setBook(RangeEnum::LastBook);
    } else if(range.book() == BibleUrlRange::LoadCurrentBook) {
        r.setBook(RangeEnum::CurrentBook);
    } else {
        r.setBook(range.bookID());
    }

    if(range.startChapter() == BibleUrlRange::LoadFirstChapter) {
        r.setChapter(RangeEnum::FirstChapter);
    } else if(range.startChapter() == BibleUrlRange::LoadLastChapter) {
        r.setChapter(RangeEnum::LastChapter);
    } else if(range.startChapter() == BibleUrlRange::LoadCurrentChapter) {
        r.setChapter(RangeEnum::CurrentChapter);
    } else {
        r.setChapter(range.startChapterID());
    }

    if(range.startVerse() == BibleUrlRange::LoadFirstVerse) {
        r.setStartVerse(RangeEnum::FirstVerse);
    } /*else if(range.startVerse() == BibleUrlRange::LoadCurrentVerse) {
        r.setStartVerse(m_moduleManager->bible()->verseID());
    } */else if(range.startVerse() == BibleUrlRange::LoadLastVerse) {
        r.setStartVerse(RangeEnum::LastVerse);
    } else {
        r.setStartVerse(range.startVerseID());
    }

    if(range.endVerse() == BibleUrlRange::LoadFirstVerse) {
        r.setEndVerse(RangeEnum::FirstVerse);
    } /*else if(range.endVerse() == BibleUrlRange::LoadCurrentVerse) {
        r.setEndVerse(m_moduleManager->bible()->verseID());
    } */else if(range.endVerse() == BibleUrlRange::LoadLastVerse) {
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
    if(range.activeVerse() == BibleUrlRange::LoadVerseByID) {
        r.setSelectedVerse(range.activeVerseID());
    }

    ranges.addRange(r);
    return ranges;
}
void BibleManager::pharseUrl(const BibleUrl &url)
{
    DEBUG_FUNC_NAME;
    myDebug() << "url = " << url.toString();
    m_actions->newSubWindowIfEmpty();
    Ranges ranges;
    foreach(BibleUrlRange range, url.ranges()) {
        ranges.addRanges(bibleUrlRangeToRanges(range));
    }
    showRanges(ranges, url);
}

void BibleManager::pharseUrl(const QString &url)
{
    DEBUG_FUNC_NAME;
    const QString bible = "bible://";
    const QString bq = "go";
    myDebug() << "url = " << url;
    if(url.startsWith(bible)) {
        m_actions->newSubWindowIfEmpty();
        BibleUrl bibleUrl;
        Ranges ranges;
        if(!bibleUrl.fromString(url)) {
            return;
        }

        foreach(BibleUrlRange range, bibleUrl.ranges()) {
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
void BibleManager::showRanges(const Ranges &ranges, const BibleUrl &url)
{
    std::pair<QString, TextRanges> r = m_moduleManager->bibleList()->readRanges(ranges);
    m_actions->showTextRanges(r.first, r.second, url);

    m_actions->updateChapters(m_moduleManager->bible()->chapterNames());
    m_actions->updateBooks(m_moduleManager->bible()->versification());
    m_actions->setCurrentBook(r.second.bookIDs());
    m_actions->setCurrentChapter(r.second.chapterIDs());
    m_actions->setTitle(m_moduleManager->bible()->moduleTitle());
}

bool BibleManager::loadModuleDataByID(const int &moduleID)
{
    if(moduleID < 0 || !m_moduleManager->contains(moduleID)) {
        myWarning() << "failed id = " << moduleID << m_moduleManager->m_moduleMap->m_map;
        return 1;
    }
    if(m_moduleManager->getModule(moduleID)->moduleClass() != Module::BibleModuleClass) {
        myWarning() << "non bible module " << m_moduleManager->getModule(moduleID)->moduleClass();
        return 1;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);

    Module::ModuleType type = m_moduleManager->getModule(moduleID)->moduleType();
    m_moduleManager->bible()->setModuleType(type);
    m_moduleManager->bible()->loadModuleData(moduleID);

    //update current title and selected module
    m_actions->setTitle(m_moduleManager->bible()->moduleTitle());
    m_actions->setCurrentModule(moduleID);

    QApplication::restoreOverrideCursor();
    return 0;
}
void BibleManager::readBookByID(const int &id)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(id < 0) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(0, tr("Error"), tr("This book is not available."));
        myWarning() << "invalid bookID - 1";
        return;
    }
    /*if(!m_moduleManager->bible()->bookIDs().contains(id)) {
        QApplication::restoreOverrideCursor();
        myWarning() << "invalid bookID - 2(no book loaded) id = " << id << " count = " << m_moduleManager->bible()->booksCount();
        return;
    }*/
    const int read = m_moduleManager->bibleList()->readBook(id);
    if(read != 0) {
        QApplication::restoreOverrideCursor();
        if(read == 2) {
            m_actions->clearChapters();
        } else {
            QMessageBox::critical(0, tr("Error"), tr("Cannot read the book."));
        }
        myWarning() << "read = " << read;
        //error while reading
        return;
    }
    QApplication::restoreOverrideCursor();
    m_actions->updateChapters(m_moduleManager->bible()->chapterNames());
    m_actions->setTitle(m_moduleManager->bible()->moduleTitle());
}

void BibleManager::showChapter(const int &chapterID, const int &verseID)
{
    //DEBUG_FUNC_NAME
    myDebug() << "chapter ID = " << chapterID << "verse ID = " << verseID;
    m_bibleDisplay->setHtml(m_moduleManager->bibleList()->readChapter(chapterID, verseID));
    //emit setCurrentChapter(chapterID);
}

void BibleManager::nextChapter()
{
    if(!m_moduleManager->bibleLoaded())
        return;
    if(m_moduleManager->bible()->lastTextRanges()->minChapterID() < m_moduleManager->bible()->chaptersCount() - 1) {
        BibleUrl bibleUrl;
        BibleUrlRange range;
        range.setBible(BibleUrlRange::LoadCurrentBible);
        range.setBook(BibleUrlRange::LoadCurrentBook);
        range.setChapter(m_moduleManager->bible()->lastTextRanges()->minChapterID() - 1);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    } else if(m_moduleManager->bible()->lastTextRanges()->minBookID() < m_moduleManager->bible()->booksCount() - 1) {
        BibleUrl bibleUrl;
        BibleUrlRange range;
        range.setBible(BibleUrlRange::LoadCurrentBible);
        range.setBook(m_moduleManager->bible()->lastTextRanges()->minBookID() + 1);
        range.setChapter(BibleUrlRange::LoadFirstChapter);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    }
}

void BibleManager::previousChapter()
{
    if(!m_moduleManager->bibleLoaded())
        return;
    if(m_moduleManager->bible()->lastTextRanges()->minChapterID() > 0) {
        BibleUrl bibleUrl;
        BibleUrlRange range;
        range.setBible(BibleUrlRange::LoadCurrentBible);
        range.setBook(BibleUrlRange::LoadCurrentBook);
        range.setChapter(m_moduleManager->bible()->lastTextRanges()->minChapterID() - 1);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    } else if(m_moduleManager->bible()->lastTextRanges()->minBookID() > 0) {
        BibleUrl bibleUrl;
        BibleUrlRange range;
        range.setBible(BibleUrlRange::LoadCurrentBible);
        range.setBook(m_moduleManager->bible()->lastTextRanges()->minBookID() - 1);
        range.setChapter(BibleUrlRange::LoadLastChapter);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    }
}
void BibleManager::loadBibleList(bool hadBible)
{
    if(hadBible) {
        BibleUrl url;
        BibleUrlRange range;
        range.setBible(BibleUrlRange::LoadCurrentBible);
        range.setBook(BibleUrlRange::LoadCurrentBook);
        range.setChapter(BibleUrlRange::LoadCurrentChapter);
        range.setWholeChapter();
        url.setParam("force", "true");
        url.addRange(range);
        m_actions->get(url);
    } else {
        BibleUrl url;
        BibleUrlRange range;
        range.setBible(BibleUrlRange::LoadCurrentBible);
        range.setBook(BibleUrlRange::LoadFirstBook);
        range.setChapter(BibleUrlRange::LoadFirstChapter);
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
    m_actions->get(*m_moduleManager->bibleList()->lastBibleUrl());
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