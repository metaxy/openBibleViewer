/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#include "versemodulemanager.h"
#include "src/ui/interface/advanced/form/commentaryform.h"
VerseModuleManager::VerseModuleManager(QObject *parent) :
    QObject(parent)
{
}

void VerseModuleManager::setWidget(QWidget *p)
{
    m_p = p;
}

void VerseModuleManager::setWindowManager(WindowManager *windowManager)
{
    m_windowManager = windowManager;
}

void VerseModuleManager::init()
{
    connect(m_actions, SIGNAL(_get(VerseUrl,Actions::OpenLinkModifiers)), this, SLOT(pharseUrl(VerseUrl,Actions::OpenLinkModifiers)));
    connect(m_actions, SIGNAL(_previousChapter()), this, SLOT(previousChapter()));
    connect(m_actions, SIGNAL(_nextChapter()), this, SLOT(nextChapter()));
    connect(m_actions, SIGNAL(_reloadCurrentRange(bool)), this, SLOT(reloadCurrentRange(bool)));
    //connect(m_actions, SIGNAL(_reloadBible()), this, SLOT(reloadBible()));
    connect(m_actions, SIGNAL(_setCurrentVerseTableID(int)), this, SLOT(setCurrentVerseTableID(int)));
}

void VerseModuleManager::createDocks()
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

QHash<DockWidget*, Qt::DockWidgetArea> VerseModuleManager::docks()
{
    //DEBUG_FUNC_NAME
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.insert(m_bookDockWidget, Qt::LeftDockWidgetArea);
    ret.insert(m_moduleDockWidget, Qt::LeftDockWidgetArea);
    ret.insert(m_quickJumpDockWidget, Qt::LeftDockWidgetArea);
    return ret;

}

void VerseModuleManager::pharseUrl(VerseUrl url, const Actions::OpenLinkModifiers mod)
{
    myDebug() << url.toString();
    Form::FormType type = Form::BibleForm;
    //todo: it may be a problem if the url says that the module="current"
    if(!url.ranges().isEmpty()) {
        const int moduleID = url.ranges().first().moduleID();
        if(moduleID != -1 &&  m_moduleManager->getModule(moduleID) && m_moduleManager->getModule(moduleID)->moduleClass() == ModuleTools::CommentaryClass) {
            type = Form::CommentaryForm;
        }
    }

    QMdiSubWindow *window;

    if(mod == Actions::OpenInNewWindow) {
        window = m_windowManager->newSubWindow(type);
    } else {
        window = m_windowManager->needWindow(type);
    }

    if(type == Form::BibleForm) {
        BibleForm *f = (BibleForm*)(m_windowManager->getForm(window));
        pharseUrl(f, url);
    } else {
        CommentaryForm *form = (CommentaryForm*)(m_windowManager->getForm(window));
        pharseUrl(form, url);
    }


}

void VerseModuleManager::pharseUrl(QString url, const Actions::OpenLinkModifiers mod)
{
    myDebug() << url;
    if(url.startsWith(ModuleTools::verseScheme)) {
        VerseUrl verseUrl(url);
        pharseUrl(verseUrl, mod);
    } else if(url.startsWith(ModuleTools::theWordScheme)) {
        VerseUrl verseUrl;
        if(verseUrl.fromTheWord(url)) {
            pharseUrl(verseUrl, mod);
        } else {
            myWarning() << "could not pharse url" << url;
        }
    }
}
void VerseModuleManager::pharseUrl(BibleForm *f, const VerseUrl &url)
{
    if(!f->verseTableLoaded()) {
        if(url.ranges().first().module() == VerseUrlRange::LoadModuleByID) {
            f->newModule(url.ranges().first().moduleID());
        } else {
            f->newModule();
        }
    }
    if(f->verseTableLoaded()) {
        f->pharseUrl(url);
    } else {
        myDebug() << "verseTable not loaded";
    }
}

void VerseModuleManager::pharseUrl(CommentaryForm *form, const VerseUrl &url)
{
     form->pharseUrl(url);
}

void VerseModuleManager::nextChapter()
{
    if(m_windowManager->activeForm()->type() == Form::BibleForm) {
        ((BibleForm*)(m_windowManager->activeForm()))->nextChapter();
    }
}

void VerseModuleManager::previousChapter()
{
    if(m_windowManager->activeForm()->type() == Form::BibleForm) {
        ((BibleForm*)(m_windowManager->activeForm()))->previousChapter();
    }
}


void VerseModuleManager::reloadCurrentRange(bool full)
{
    if(!m_windowManager->activeForm())
        return;
    if(m_windowManager->activeForm()->type() == Form::BibleForm) {
        ((BibleForm*)(m_windowManager->activeForm()))->reload(full);
    }

}
BookDockWidget *VerseModuleManager::bookDockWidget()
{
    return m_bookDockWidget;
}

ModuleDockWidget *VerseModuleManager::moduleDockWidget()
{
    return m_moduleDockWidget;
}

QuickJumpDockWidget * VerseModuleManager::quickJumpDockWidget()
{
    return m_quickJumpDockWidget;
}
void VerseModuleManager::setCurrentVerseTableID(const int verseTableID)
{
    if(m_windowManager->activeForm()->type() == Form::BibleForm) {
        ((BibleForm*)(m_windowManager->activeForm()))->verseTable()->setCurrentVerseTableID(verseTableID);
    }
}
