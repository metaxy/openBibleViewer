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
    connect(m_actions, SIGNAL(_get(VerseUrl,Actions::OpenLinkModifiers)), this, SLOT(parseUrl(VerseUrl,Actions::OpenLinkModifiers)));
    connect(m_actions, SIGNAL(_getIn(VerseUrl, Actions::OpenLinkType)), this, SLOT(parseUrlIn(VerseUrl,Actions::OpenLinkType)));
    connect(m_actions, SIGNAL(_previousChapter()), this, SLOT(previousChapter()));
    connect(m_actions, SIGNAL(_nextChapter()), this, SLOT(nextChapter()));
    connect(m_actions, SIGNAL(_reloadCurrentRange(bool)), this, SLOT(reloadCurrentRange(bool)));
    connect(m_actions, SIGNAL(_setActiveItem(int)), this, SLOT(setActiveItem(int)));
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
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.insert(m_bookDockWidget, Qt::LeftDockWidgetArea);
    ret.insert(m_moduleDockWidget, Qt::LeftDockWidgetArea);
    ret.insert(m_quickJumpDockWidget, Qt::LeftDockWidgetArea);
    return ret;

}

void VerseModuleManager::parseUrl(const VerseUrl &url, const Actions::OpenLinkModifiers mod)
{
    myDebug() << url.toString() << mod;
    Form::FormType type = Form::BibleForm;
    bool forceType = false;

    if(!url.ranges().isEmpty()) {
        //by default the form type is BibleForm, but if the moduleID says we need a commentary, then it will be CommentaryForm
        const int moduleID = url.ranges().first().moduleID();
        if(moduleID != -1 && m_moduleManager->getModule(moduleID) && m_moduleManager->getModule(moduleID)->moduleClass() == ModuleTools::CommentaryClass) {
            type = Form::CommentaryForm;
        }
        //if there is a specific moduleid to load, then we have to force the form type.
        foreach(const VerseUrlRange &r, url.ranges()) {
            if(r.module() == VerseUrlRange::LoadModuleByID || r.module() == VerseUrlRange::LoadModuleByUID) {
                forceType = true;
            }
        }

    }
    myDebug() << "forcetype=" << forceType;

    QMdiSubWindow *window = nullptr;

    if(mod == Actions::OpenInNewWindow) {
        window = m_windowManager->newSubWindow(type);
    } else if(mod == Actions::OpenInOtherWindow) {
        if(forceType){
            window = m_windowManager->needWindow(type,
                                                 [] (Form *) {return false;},
                                                 [type] (Form *f) {return f->type() == type;}
                                                 );
        } else {
            window = m_windowManager->needWindow(type,
                                                 [](Form *) { return false;},
                                                 [](Form *f) { return f->type() == Form::CommentaryForm || f->type() == Form::BibleForm;}
                                                 );
        }
    } else {
        if(forceType){
            window = m_windowManager->needWindow(type);
        } else {
            window = m_windowManager->needWindow(type, [](Form *f) { return f->type() == Form::CommentaryForm || f->type() == Form::BibleForm;});
        }
    }
    this->parseUrlInWindow(url, window, mod);
}
void VerseModuleManager::parseUrlInWindow(const VerseUrl &url, QMdiSubWindow* window, const Actions::OpenLinkModifiers mod)
{
    Q_ASSERT(window != nullptr);
    Form *f = m_windowManager->getForm(window);

    if(f->type() == Form::BibleForm) {
        parseUrl((BibleForm*) f, url, mod);
    } else if(f->type() == Form::CommentaryForm) {
        parseUrl((CommentaryForm*) f, url);
    } else {
        myWarning() << "unkown window type";
    }

}

void VerseModuleManager::parseUrl(QString url, const Actions::OpenLinkModifiers mod)
{
    myDebug() << url;
    if(url.startsWith(ModuleTools::verseScheme)) {
        VerseUrl verseUrl(url);
        parseUrl(verseUrl, mod);
    } else if(url.startsWith(ModuleTools::theWordScheme)) {
        VerseUrl verseUrl;
        if(verseUrl.fromTheWord(url)) {
            parseUrl(verseUrl, mod);
        } else {
            myWarning() << "could not parse url" << url;
        }
    }
}
void VerseModuleManager::parseUrl(BibleForm *f, const VerseUrl &url, const Actions::OpenLinkModifiers mod)
{
    DEBUG_FUNC_NAME
    //only if there is nothing at all
    if(!f->verseTableLoaded()) {
        if(url.ranges().first().module() == VerseUrlRange::LoadModuleByID) {
            f->newModule(url.ranges().first().moduleID());
        } else {
            f->newModule();
        }
    }

    if(f->verseTableLoaded()) {
        if(mod == Actions::OpenParallelH) f->addParallelH(url.ranges().first().moduleID());
        if(mod == Actions::OpenParallelV) f->addParallelV(url.ranges().first().moduleID());

        f->parseUrl(url);
    } else {
        myWarning() << "verseTable not loaded";
    }
}

void VerseModuleManager::parseUrl(CommentaryForm *form, const VerseUrl &url)
{
     form->parseUrl(url);
}

void VerseModuleManager::parseUrlIn(VerseUrl url, const Actions::OpenLinkType t)
{
    QMdiSubWindow *window = nullptr;
    url.removeModuleID();
    url.setOpenToTransformation(true);

    if(t == Actions::BibleType) {
        window = m_windowManager->needWindow(Form::BibleForm);
    } else if(t == Actions::CommentaryType){
        window = m_windowManager->needWindow(Form::CommentaryForm);
    } else {
        myWarning() << "OpenLinkType not supportet" << t;
        return;
    }
    this->parseUrlInWindow(url, window);
}

void VerseModuleManager::nextChapter()
{
    auto *f = m_windowManager->activeForm();
    Q_ASSERT(f != nullptr);

    if(f->type() == Form::BibleForm) {
        ((BibleForm*)f)->nextChapter();
    }
}

void VerseModuleManager::previousChapter()
{
    auto *f = m_windowManager->activeForm();
    Q_ASSERT(f != nullptr);

    if(f->type() == Form::BibleForm) {
        ((BibleForm*)f)->previousChapter();
    }
}

void VerseModuleManager::reloadCurrentRange(bool full)
{
    auto *f = m_windowManager->activeForm();
    if(f == nullptr)
        return;

    if(f->type() == Form::BibleForm) {
        ((BibleForm*)f)->reload(full);
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

void VerseModuleManager::setActiveItem(const int verseTableID)
{
    auto *f = m_windowManager->activeForm();
    if(f->type() == Form::BibleForm) {
        ((BibleForm*)f)->verseTable()->setActiveItem(verseTableID);
    }
}
