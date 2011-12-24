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
#include "biblemanager.h"

BibleManager::BibleManager(QObject *parent) :
    QObject(parent)
{
}

void BibleManager::setWidget(QWidget *p)
{
    m_p = p;
}

void BibleManager::setWindowManager(WindowManager *windowManager)
{
    m_windowManager = windowManager;
}

void BibleManager::init()
{
    connect(m_actions, SIGNAL(_get(VerseUrl,Actions::OpenLinkModifiers)), this, SLOT(pharseUrl(VerseUrl,Actions::OpenLinkModifiers)));
    connect(m_actions, SIGNAL(_previousChapter()), this, SLOT(previousChapter()));
    connect(m_actions, SIGNAL(_nextChapter()), this, SLOT(nextChapter()));
    connect(m_actions, SIGNAL(_loadVerseTable(bool)), this, SLOT(loadBibleList(bool)));
    connect(m_actions, SIGNAL(_reloadCurrentRange(bool)), this, SLOT(reloadCurrentRange(bool)));
    //connect(m_actions, SIGNAL(_reloadBible()), this, SLOT(reloadBible()));
    connect(m_actions, SIGNAL(_setCurrentVerseTableID(int)), this, SLOT(setCurrentVerseTableID(int)));
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
    m_quickJumpDockWidget->setWindowManager(m_windowManager);
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
    ret.insert(m_quickJumpDockWidget, Qt::LeftDockWidgetArea);
    return ret;

}

void BibleManager::pharseUrl(VerseUrl url, const Actions::OpenLinkModifiers mod)
{
    DEBUG_FUNC_NAME
    QMdiSubWindow *window;
    if(mod == Actions::OpenInNewWindow) {
        window = m_windowManager->newBibleSubWindow();
    } else {
        window = m_windowManager->needBibleWindow();
    }
    BibleForm *f = (BibleForm*)(m_windowManager->getForm(window));
    myDebug() << f->verseTableLoaded();
    if(!f->verseTableLoaded()) {
        f->newModule();
    }
    f->pharseUrl(url);
}

void BibleManager::pharseUrl(QString url, const Actions::OpenLinkModifiers mod)
{
    DEBUG_FUNC_NAME
    QMdiSubWindow *window;
    if(mod == Actions::OpenInNewWindow) {
        window = m_windowManager->newBibleSubWindow();
    } else {
        window = m_windowManager->needBibleWindow();
    }

    //open a bible window
    BibleForm *f = (BibleForm*)(m_windowManager->getForm(window));
    myDebug() << f->verseTableLoaded();
    if(!f->verseTableLoaded()) {
        f->newModule();
    }
    f->pharseUrl(url);
}
void BibleManager::nextChapter()
{
    if(m_windowManager->activeForm()->type() == Form::BibleForm) {
        ((BibleForm*)(m_windowManager->activeForm()))->nextChapter();
    }
}

void BibleManager::previousChapter()
{
    if(m_windowManager->activeForm()->type() == Form::BibleForm) {
        ((BibleForm*)(m_windowManager->activeForm()))->previousChapter();
    }
}

void BibleManager::loadBibleList(bool hadBible)
{
    DEBUG_FUNC_NAME
    if(hadBible) {
        VerseUrl url;
        VerseUrlRange range;
        range.setOpenToTransformation(false);
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
        range.setOpenToTransformation(false);
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(VerseUrlRange::LoadFirstBook);
        range.setChapter(VerseUrlRange::LoadFirstChapter);
        range.setWholeChapter();
        url.setParam("force", "true");
        url.addRange(range);
        m_actions->get(url);
    }
}


void BibleManager::reloadCurrentRange(bool full)
{
    if(m_windowManager->activeForm()->type() == Form::BibleForm) {
        ((BibleForm*)(m_windowManager->activeForm()))->reload(full);
    }

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
void BibleManager::setCurrentVerseTableID(const int verseTableID)
{
    if(m_windowManager->activeForm()->type() == Form::BibleForm) {
        ((BibleForm*)(m_windowManager->activeForm()))->verseTable()->setCurrentVerseTableID(verseTableID);
    }
}
