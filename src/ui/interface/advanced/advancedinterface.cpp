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
#include "advancedinterface.h"
#include "ui_advancedinterface.h"
#include "src/core/link/biblelink.h"
#include "src/core/module/bible/biblequote.h"
#include "src/core/link/strongurl.h"
#include "src/api/api.h"
#include <QShortcut>
AdvancedInterface::AdvancedInterface(QWidget *parent) :
    Interface(parent),
    ui(new Ui::AdvancedInterface)
{
    ui->setupUi(this);
}

AdvancedInterface::~AdvancedInterface()
{
    //DEBUG_FUNC_NAME;
    if(ui != nullptr) {
        delete ui;
        ui = 0;
    }
}

void AdvancedInterface::init()
{
    //DEBUG_FUNC_NAME
    m_moduleManager->newDisplaySettings();
    m_moduleManager->moduleDisplaySetings()->setShowMarks(true);
    m_moduleManager->moduleDisplaySetings()->setShowNotes(true);
    m_moduleManager->moduleDisplaySetings()->setLoadNotes(true);

    connect(m_actions, SIGNAL(_get(QString,Actions::OpenLinkModifiers)), this, SLOT(parseUrl(QString,Actions::OpenLinkModifiers)));

    m_api = new Api(this);
    setAll(m_api);
    m_api->init();

    m_notesManager = new NotesManager(this);
    m_notesManager->setWidget(this->parentWidget());
    setAll(m_notesManager);

    m_bookmarksManager = new BookmarksManager(this);
    setAll(m_bookmarksManager);
    m_bookmarksManager->setWidget(this->parentWidget());

    m_windowManager = new WindowManager(this);
    setAll(m_windowManager);
    MdiArea *mdi = new MdiArea(ui->horizontalLayout->widget());
    ui->horizontalLayout->addWidget(mdi);

    m_windowManager->setMdiArea(mdi);
    m_windowManager->setApi(m_api);
    m_windowManager->setNotesManager(m_notesManager);
    m_windowManager->setBookmarksManager(m_bookmarksManager);

    m_windowManager->init();

    m_bibleManager = new VerseModuleManager(this);
    setAll(m_bibleManager);
    m_bibleManager->setWidget(this->parentWidget());
    m_bibleManager->setWindowManager(m_windowManager);
    m_bibleManager->init();

    m_dictionaryManager = new DictionaryManager(this, this->parentWidget());
    setAll(m_dictionaryManager);
    m_dictionaryManager->setWindowManager(m_windowManager);


    m_searchManager = new SearchManager(this);
    setAll(m_searchManager);
    m_searchManager->setWindowManager(m_windowManager);
    m_searchManager->setWidget(this->parentWidget());
    m_searchManager->init();

    m_webPageManager = new WebPageManager();
    setAll(m_webPageManager);
    m_webPageManager->setWindowManager(m_windowManager);


    m_settings->session.file()->beginGroup(m_settings->session.id() + "/windows/");
    if(m_settings->session.file()->childGroups().size() == 0)
        QTimer::singleShot(10, m_windowManager, SLOT(newSubWindow()));
    m_settings->session.file()->endGroup();

    connect(m_actions, SIGNAL(_setTitle(QString)), this , SLOT(setTitle(QString)));
    connect(m_actions, SIGNAL(_setTabbedView()), this, SLOT(setTabbedView()));
    connect(m_actions, SIGNAL(_setSubWindowView()), this, SLOT(setSubWindowView()));
    connect(m_actions, SIGNAL(_setFullScreen(bool)), this, SLOT(fullscreen(bool)));

}
void AdvancedInterface::createDocks()
{
    m_bibleManager->createDocks();
    m_notesManager->createDocks();
    m_searchManager->createDocks();
    m_bookmarksManager->createDocks();
    m_dictionaryManager->createDocks();
}
void AdvancedInterface::createMenu()
{

}

QHash<DockWidget*, Qt::DockWidgetArea> AdvancedInterface::docks()
{
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.unite(m_bibleManager->docks());
    ret.unite(m_notesManager->docks());
    ret.unite(m_searchManager->docks());
    ret.unite(m_bookmarksManager->docks());
    ret.unite(m_dictionaryManager->docks());
    return ret;
}
void AdvancedInterface::fullscreen(bool enabled)
{

    if(enabled) {

    }
}

void AdvancedInterface::parseUrl(QString url, const Actions::OpenLinkModifiers mod)
{
    //DEBUG_FUNC_NAME
    //setEnableReload(false);
    myDebug() << "url = " << url;

    const QString http = "http://";
    const QString bq = "go";
    const QString anchor = "#";
    const QString note = "note://";

    QString backup = url;

    if(url.startsWith(ModuleTools::verseScheme)) {
        m_bibleManager->parseUrl(url, mod);
    } else if(url.startsWith(ModuleTools::strongScheme)) {
        m_dictionaryManager->parseUrl(url, mod);
    } else if(url.startsWith(ModuleTools::rmacScheme)) {
        m_dictionaryManager->parseUrl(url, mod);
    } else if(url.startsWith(ModuleTools::dictScheme)) {
        m_dictionaryManager->parseUrl(url, mod);
    } else if(url.startsWith(ModuleTools::rmacScheme)) {
        m_dictionaryManager->parseUrl(url, mod);
    } else if(url.startsWith(ModuleTools::webPageScheme)) {
        m_webPageManager->parseUrl(url);
    } else if(url.startsWith(http)) {
        m_webPageManager->parseWebUrl(url);
        //QDesktopServices::openUrl(url);
    } else if(url.startsWith(ModuleTools::bookScheme)) {
        const int moduleID = url.remove(0, ModuleTools::bookScheme.size()).toInt();
        BookForm *f = (BookForm*) m_windowManager->getForm(m_windowManager->needWindow(Form::BookForm));
        if(f) {
            f->loadModule(moduleID);
            f->show();
        }
    } else if(url.startsWith(ModuleTools::treeBookScheme)) {
        url.remove(0, ModuleTools::treeBookScheme.size());
        const QStringList splits = url.split(",");
        if(splits.size() > 1) {
            const int moduleID = splits.first().toInt();
            const int chapterID = splits.last().toInt();
            TreeBookForm *f = (TreeBookForm*) m_windowManager->getForm(m_windowManager->needWindow(Form::TreeBookForm));
            if(f) {
                f->loadModule(moduleID);
                f->showChapter(moduleID, chapterID);
            }
        }
    } else if(url.startsWith(ModuleTools::userInputScheme)) {
         url = url.remove(0, ModuleTools::userInputScheme.size());
         quick(url);
    } else if(url.startsWith(bq)) {
        m_bibleManager->parseUrl(url, mod);
    } else if(url.startsWith(anchor)) {
        //todo:
        url = url.remove(0, anchor.size());
        bool ok;
        int c = url.toInt(&ok);
        if(ok) {
            VerseUrlRange r;
            r.setModule(VerseUrlRange::LoadCurrentModule);
            r.setBook(VerseUrlRange::LoadCurrentBook);
            r.setChapter(c);
            r.setWholeChapter();
            VerseUrl url(r);
            m_actions->get(url);
        } else {
            if(m_windowManager->activeForm()) {
                BibleForm * f = (BibleForm *)m_windowManager->activeForm();
                if(f) {
                    f->m_view->scrollToAnchor(url);
                }
            }
        }

    } else if(url.startsWith(note)) {
        url = url.remove(0, note.size());
        m_notesManager->openNote(url);
    } else if(url.startsWith(ModuleTools::theWordScheme)) {
        url = url.remove(0, ModuleTools::theWordScheme.size());
        if(url.startsWith("bible")) {
            m_bibleManager->parseUrl(backup, mod);
        }
    }else {
        //todo: unterstand links like about:blank#a04
        if(m_windowManager->activeForm() && m_windowManager->activeForm()->type() == Form::BibleForm) {
            BibleForm *f = (BibleForm*)(m_windowManager->activeForm());
            if(f->verseModule()->moduleType() == ModuleTools::BibleQuoteModule) {
                bool isInBookPath = false;
                int b = 0;
                const QStringList books = ((BibleQuote*)(((Bible*)f->verseModule())->bibleModule()))->booksPath();
                int i = 0;
                foreach(const QString & book, books) {
                    if(book.endsWith(url, Qt::CaseInsensitive)) {
                        b = i;
                        isInBookPath = true;
                        myDebug() << b;
                        break; // todo: check if there are more similiar
                    }
                    i++;
                }
                if(isInBookPath) {
                    VerseUrlRange r;
                    r.setModule(VerseUrlRange::LoadCurrentModule);
                    r.setBook(b);
                    r.setChapter(VerseUrlRange::LoadFirstChapter);
                    r.setWholeChapter();
                    VerseUrl url(r);
                    m_actions->get(url);
                }
            }
        } else {
            //todo: not only bibleform
            if(m_windowManager->activeForm() && m_windowManager->activeForm()->type() == Form::BibleForm)
                ((BibleForm*)m_windowManager->activeForm())->evaluateJavaScript(url);
        }
    }
    //setEnableReload(true);
    return;
}

void AdvancedInterface::setTitle(const QString &title)
{
    if(title.isEmpty())
        this->parentWidget()->setWindowTitle(tr("openBibleViewer"));
    else
        this->parentWidget()->setWindowTitle(title + " - " + tr("openBibleViewer"));
}


void AdvancedInterface::closing()
{
    m_notesManager->save();
    m_bookmarksManager->save();
    m_windowManager->save();
}

void AdvancedInterface::restoreSession()
{
    m_windowManager->restore();
}

void AdvancedInterface::settingsChanged(Settings oldSettings, Settings newSettings, bool modifedModuleSettings)
{
    //DEBUG_FUNC_NAME
    //reload books
    bool reloadModules = false;
    if(oldSettings.encoding != newSettings.encoding) {
        myDebug() << "encoding changed from "  << oldSettings.encoding << " to " << newSettings.encoding;
        reloadModules = true;
    }
    if(reloadModules == false) {
        if(oldSettings.m_moduleSettings.size() != newSettings.m_moduleSettings.size()) {
            myDebug() << "module settings size changed from "  << oldSettings.m_moduleSettings.size() << " to " << newSettings.m_moduleSettings.size();
            reloadModules = true;
        }
    }
    if(modifedModuleSettings == true) {
        reloadModules = true;
    }
    if(reloadModules == true) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        //myDebug() << "reload Module";
        m_moduleManager->loadAllModules();
        m_bibleManager->moduleDockWidget()->init();
        m_dictionaryManager->dictionaryDockWidget()->init();
        m_actions->reloadCurrentRange(true);

        QApplication::restoreOverrideCursor();
    }

}

void AdvancedInterface::copy()
{
    if(m_windowManager->activeForm())
        m_windowManager->activeForm()->copy();
}

void AdvancedInterface::selectAll()
{
    if(m_windowManager->activeForm())
        m_windowManager->activeForm()->selectAll();
}

bool AdvancedInterface::hasMenuBar()
{
    return true;
}

QMenuBar* AdvancedInterface::menuBar()
{
    QMenuBar *bar = new QMenuBar(this);
    QMenu *menuFile = new QMenu(tr("File"), bar);

    //New Sub Window
    QMenu *menuNewSubWindow = new QMenu(tr("New Subwindow"), menuFile);

    QAction *actionNewBibleSubWindow = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("New Bible Window"), menuNewSubWindow);
    connect(actionNewBibleSubWindow, SIGNAL(triggered()), m_windowManager, SLOT(newBibleSubWindow()));
    actionNewBibleSubWindow->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));//using const KeySequence on KDE will be Ctrl+Shift+N

    QAction *actionNewWebSubWindow = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("New Web Window"), menuNewSubWindow);
    connect(actionNewWebSubWindow, SIGNAL(triggered()), m_windowManager, SLOT(newWebSubWindow()));

    QAction *actionNewDictionarySubWindow = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("New Dictionary Window"), menuNewSubWindow);
    connect(actionNewDictionarySubWindow, SIGNAL(triggered()), m_windowManager, SLOT(newDictionarySubWindow()));

    QAction *actionNewBookSubWindow = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("New Book Window"), menuNewSubWindow);
    connect(actionNewBookSubWindow, SIGNAL(triggered()), m_windowManager, SLOT(newBookSubWindow()));

    QAction *actionNewCommentarySubWindow = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("New Commentary Window"), menuNewSubWindow);
    connect(actionNewCommentarySubWindow, SIGNAL(triggered()), m_windowManager, SLOT(newCommentarySubWindow()));

    QAction *actionNewConsoleSubWindow = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("New Console Window"), menuNewSubWindow);
    connect(actionNewConsoleSubWindow, SIGNAL(triggered()), m_windowManager, SLOT(newConsoleSubWindow()));

    menuNewSubWindow->addAction(actionNewBibleSubWindow);
    menuNewSubWindow->addAction(actionNewWebSubWindow);
    menuNewSubWindow->addAction(actionNewDictionarySubWindow);
    menuNewSubWindow->addAction(actionNewBookSubWindow);
    menuNewSubWindow->addAction(actionNewCommentarySubWindow);

    //Close Sub Window
    QAction *actionCloseSubWindow = new QAction(QIcon::fromTheme("tab-close", QIcon(":/icons/16x16/tab-close.png")), tr("Close SubWindow"), menuFile);
    connect(actionCloseSubWindow, SIGNAL(triggered()), m_windowManager, SLOT(closeSubWindow()));
    actionCloseSubWindow->setShortcut(QKeySequence::Close);
    actionCloseSubWindow->setShortcutContext(Qt::WidgetShortcut);

    //Save As
    QAction *actionSaveAs = new QAction(QIcon::fromTheme("document-save-as", QIcon(":/icons/16x16/document-save-as.png")), tr("Save As"), menuFile);
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(saveFile()));
    actionSaveAs->setShortcut(QKeySequence::SaveAs);

    //Print
    QAction *actionPrint = new QAction(QIcon::fromTheme("document-print", QIcon(":/icons/16x16/document-print.png")), tr("Print"), menuFile);
    connect(actionPrint, SIGNAL(triggered()), this, SLOT(printFile()));
    actionPrint->setShortcut(QKeySequence::Print);
    //Print

    QAction *actionPrintPreview = new QAction(QIcon::fromTheme("document-print-preview", QIcon(":/icons/16x16/document-print-preview.png")), tr("Print Preview"), menuFile);
    connect(actionPrintPreview, SIGNAL(triggered()), this, SLOT(printPreview()));
    //actionPrint->setShortcut(QKeySequence::Pr);

    //Close
    QAction *actionClose = new QAction(QIcon::fromTheme("application-exit", QIcon(":/icons/16x16/application-exit.png")), tr("Quit"), menuFile);
    connect(actionClose, SIGNAL(triggered()), this->parentWidget(), SLOT(close()));
    actionClose->setShortcut(QKeySequence::Quit);

    menuFile->addMenu(menuNewSubWindow);
    menuFile->addAction(actionCloseSubWindow);
    menuFile->addSeparator();
    menuFile->addAction(actionSaveAs);
    menuFile->addAction(actionPrint);
    menuFile->addAction(actionPrintPreview);
    menuFile->addSeparator();
    menuFile->addAction(actionClose);

    QMenu *menuEdit = new QMenu(tr("Edit"), bar);

    //Copy
    QAction *actionCopy = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Copy"), menuEdit);
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
    actionCopy->setShortcut(QKeySequence::Copy);

    //SelectAll
    QAction *actionSelectAll = new QAction(QIcon::fromTheme("edit-select-all", QIcon(":/icons/16x16/edit-select-all.png")), tr("Select All"), menuEdit);
    connect(actionSelectAll, SIGNAL(triggered()), this, SLOT(selectAll()));
    actionSelectAll->setShortcut(QKeySequence::SelectAll);

    //Search
    QAction *actionSearch = new QAction(QIcon::fromTheme("edit-find", QIcon(":/icons/16x16/edit-find.png")), tr("Search"), menuEdit);
    actionSearch->setShortcut(QKeySequence::Find);
    connect(actionSearch, SIGNAL(triggered()), m_searchManager, SLOT(showSearchDialog()));

    //Find Next
    QAction *actionFindNext = new QAction(QIcon::fromTheme("go-down-search", QIcon(":/icons/16x16/go-down-search.png")), tr("Find Next"), menuEdit);
    connect(actionFindNext, SIGNAL(triggered()), m_searchManager, SLOT(nextVerse()));
    actionFindNext->setShortcut(QKeySequence::FindNext);

    //Find Previous
    QAction *actionFindPrevious = new QAction(QIcon::fromTheme("go-up-search", QIcon(":/icons/16x16/go-up-search.png")), tr("Find Previous"), menuEdit);
    connect(actionFindPrevious, SIGNAL(triggered()), m_searchManager, SLOT(previousVerse()));
    actionFindPrevious->setShortcut(QKeySequence::FindPrevious);

    //Next Chapter
    QAction *actionNextChapter = new QAction(QIcon(""), tr("Next Chapter"), menuEdit);
    connect(actionNextChapter, SIGNAL(triggered()), m_actions, SLOT(nextChapter()));
    actionNextChapter->setShortcut(QKeySequence::MoveToNextPage);

    //Prev Chapter
    QAction *actionPrevChapter = new QAction(QIcon(""), tr("Previous Chapter"), menuEdit);
    connect(actionPrevChapter, SIGNAL(triggered()), m_actions, SLOT(previousChapter()));
    actionPrevChapter->setShortcut(QKeySequence::MoveToPreviousPage);



    menuEdit->addAction(actionCopy);
    menuEdit->addAction(actionSelectAll);
    menuEdit->addSeparator();
    menuEdit->addAction(actionSearch);
    menuEdit->addAction(actionFindNext);
    menuEdit->addAction(actionFindPrevious);
    menuEdit->addSeparator();
    menuEdit->addAction(actionNextChapter);
    menuEdit->addAction(actionPrevChapter);


    QMenu *menuView = new QMenu(tr("View"), bar);

    //Zoom In
    QAction *actionZoomIn = new QAction(QIcon::fromTheme("zoom-in", QIcon(":/icons/16x16/zoom-in.png")), tr("Zoom In"), menuView);
    connect(actionZoomIn, SIGNAL(triggered()), m_windowManager, SLOT(zoomIn()));
    actionZoomIn->setShortcut(QKeySequence::ZoomIn);

    //Zoom Out
    QAction *actionZoomOut = new QAction(QIcon::fromTheme("zoom-out", QIcon(":/icons/16x16/zoom-out.png")), tr("Zoom Out"), menuView);
    actionZoomOut->setShortcut(QKeySequence::ZoomOut);
    connect(actionZoomOut, SIGNAL(triggered()), m_windowManager, SLOT(zoomOut()));

    //TabView
    m_actionTabbedView = new QAction(QIcon(), tr("Tabbed View"), menuView);
    m_actionTabbedView->setCheckable(true);
    connect(m_actionTabbedView, SIGNAL(triggered()), m_actions, SLOT(setTabbedView()));

    //SubWindowView
    m_actionSubWindowView = new QAction(QIcon(), tr("Sub Window View"), menuView);
    m_actionSubWindowView->setCheckable(true);
    connect(m_actionSubWindowView, SIGNAL(triggered()), m_actions, SLOT(setSubWindowView()));

    //Cascade
    m_actionCascade = new QAction(QIcon(":/icons/svg/cascade.svg"), tr("Cascade"), menuView);
    m_actionCascade->setCheckable(true);
    m_actionCascade->setChecked(m_settings->autoLayout == Settings::Cascade);
    m_actionCascade->setObjectName("actionCascade");
    connect(m_actionCascade, SIGNAL(triggered(bool)), m_windowManager, SLOT(cascade(bool)));
    connect(m_actionCascade, SIGNAL(triggered(bool)), this, SLOT(uncheck(bool)));


    //Tile
    m_actionTile = new QAction(QIcon(":/icons/svg/tile.svg"), tr("Tile"), menuView);
    m_actionTile->setCheckable(true);
    m_actionTile->setChecked(m_settings->autoLayout == Settings::Tile);
    m_actionTile->setObjectName("actionTile");
    connect(m_actionTile, SIGNAL(triggered(bool)), m_windowManager, SLOT(tile(bool)));
    connect(m_actionTile, SIGNAL(triggered(bool)), this, SLOT(uncheck(bool)));

    //Tile Vertical
    m_actionTileVertical = new QAction(QIcon(":/icons/svg/tile_vert.svg"), tr("Tile Vertical"), menuView);
    m_actionTileVertical->setCheckable(true);
    m_actionTileVertical->setChecked(m_settings->autoLayout == Settings::VerticalTile);
    m_actionTileVertical->setObjectName("actionTileVertical");
    connect(m_actionTileVertical, SIGNAL(triggered(bool)), m_windowManager, SLOT(tileVertical(bool)));
    connect(m_actionTileVertical, SIGNAL(triggered(bool)), this, SLOT(uncheck(bool)));

    //Tile Horizontal
    m_actionTileHorizontal = new QAction(QIcon(":/icons/svg/tile_horiz.svg"), tr("Tile Horizontal"), menuView);
    m_actionTileHorizontal->setCheckable(true);
    m_actionTileHorizontal->setChecked(m_settings->autoLayout == Settings::HorizontalTile);
    m_actionTileHorizontal->setObjectName("actionTileHorizontal");
    connect(m_actionTileHorizontal, SIGNAL(triggered(bool)), m_windowManager, SLOT(tileHorizontal(bool)));
    connect(m_actionTileHorizontal, SIGNAL(triggered(bool)), this, SLOT(uncheck(bool)));

    QAction *actionDoTiling = new QAction(tr("Layout windows"), menuView);
    actionDoTiling->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    actionDoTiling->setShortcutContext(Qt::WidgetShortcut);
    connect(actionDoTiling, SIGNAL(triggered()), m_windowManager, SLOT(autoLayout()));

    menuView->addAction(actionZoomIn);
    menuView->addAction(actionZoomOut);
    menuView->addSeparator();
    menuView->addAction(m_actionTabbedView);
    menuView->addAction(m_actionSubWindowView);
    menuView->addSeparator();
    menuView->addAction(m_actionCascade);
    menuView->addAction(m_actionTile);
    menuView->addAction(m_actionTileVertical);
    menuView->addAction(m_actionTileHorizontal);
    menuView->addSeparator();
    menuView->addAction(actionDoTiling);

    QMenu *menuNotes = new QMenu(tr("Notes"), bar);

    //Notes Editor
    QAction *actionNotesEditor = new QAction(QIcon::fromTheme("notes-edit", QIcon(":/icons/16x16/notes-edit.png")), tr("Notes Editor"), menuNotes);
    connect(actionNotesEditor, SIGNAL(triggered()), this, SLOT(showNotesEditor()));

    //Mark List
    QAction *actionMarkList = new QAction(QIcon::fromTheme("table", QIcon(":/icons/16x16/table.png")), tr("Mark List"), menuNotes);
    connect(actionMarkList, SIGNAL(triggered()), this, SLOT(showMarkList()));

    //Mark Categories
    QAction *actionMarkCategories = new QAction(QIcon(), tr("Mark Categories"), menuNotes);
    connect(actionMarkCategories, SIGNAL(triggered()), this, SLOT(showMarkCategories()));


    menuNotes->addAction(actionNotesEditor);
    //menuNotes->addSeparator();
    menuNotes->addAction(actionMarkList);
    // menuNotes->addAction(actionMarkCategories);

    QMenu *menuDocks = new QMenu(tr("Docks"), bar);


    QAction *actionModule = new QAction(tr("Module"), menuDocks);
    actionModule->setCheckable(true);

    connect(m_bibleManager->moduleDockWidget(), SIGNAL(visibilityChanged(bool)), actionModule, SLOT(setChecked(bool)));
    connect(actionModule, SIGNAL(triggered(bool)), m_bibleManager->moduleDockWidget(), SLOT(setVisible(bool)));
    actionModule->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_M));

    QAction *actionBooks = new QAction(tr("Books"), menuDocks);
    actionBooks->setCheckable(true);
    connect(m_bibleManager->bookDockWidget(), SIGNAL(visibilityChanged(bool)), actionBooks, SLOT(setChecked(bool)));
    connect(actionBooks, SIGNAL(triggered(bool)), m_bibleManager->bookDockWidget(), SLOT(setVisible(bool)));
    actionBooks->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C));

    QAction *actionNotes = new QAction(QIcon::fromTheme("notes-edit", QIcon(":/icons/16x16/notes-edit.png")), tr("Notes"), menuDocks);
    actionNotes->setCheckable(true);
    connect(m_notesManager->notesDockWidget(), SIGNAL(visibilityChanged(bool)), actionNotes, SLOT(setChecked(bool)));
    connect(actionNotes, SIGNAL(triggered(bool)), m_notesManager->notesDockWidget(), SLOT(setVisible(bool)));
    actionNotes->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));

    QAction *actionStrong = new QAction(tr("Dictionay"), menuDocks);
    actionStrong->setCheckable(true);
    connect(m_dictionaryManager->dictionaryDockWidget(), SIGNAL(visibilityChanged(bool)), actionStrong, SLOT(setChecked(bool)));
    connect(actionStrong, SIGNAL(triggered(bool)), m_dictionaryManager->dictionaryDockWidget(), SLOT(setVisible(bool)));
    actionStrong->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_D));

    QAction *actionBookmarks = new QAction(QIcon::fromTheme("bookmarks-organize", QIcon(":/icons/16x16/bookmarks-organize.png")), tr("Bookmarks"), menuDocks);
    actionBookmarks->setCheckable(true);
    connect(m_bookmarksManager->bookmarksDockWidget(), SIGNAL(visibilityChanged(bool)), actionBookmarks, SLOT(setChecked(bool)));
    connect(actionBookmarks, SIGNAL(triggered(bool)), m_bookmarksManager->bookmarksDockWidget(), SLOT(setVisible(bool)));
    actionBookmarks->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_B));

    QAction *actionQuickJump = new QAction(tr("Quick Jump"), menuDocks);
    actionQuickJump->setCheckable(true);
    connect(m_bibleManager->quickJumpDockWidget(), SIGNAL(visibilityChanged(bool)), actionQuickJump, SLOT(setChecked(bool)));
    connect(actionQuickJump, SIGNAL(triggered(bool)), m_bibleManager->quickJumpDockWidget(), SLOT(setVisible(bool)));
    actionQuickJump->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Q));

    QAction *actionSearchResults = new QAction(QIcon::fromTheme("table", QIcon(":/icons/16x16/table.png")), tr("Search Results"), menuDocks);
    actionSearchResults->setCheckable(true);
    connect(m_searchManager->advancedSearchResultDockWidget(), SIGNAL(visibilityChanged(bool)), actionSearchResults, SLOT(setChecked(bool)));
    connect(actionSearchResults , SIGNAL(triggered(bool)), m_searchManager->advancedSearchResultDockWidget(), SLOT(setVisible(bool)));
    actionSearchResults->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));


    menuDocks->addAction(actionModule);
    menuDocks->addAction(actionBooks);
    menuDocks->addAction(actionNotes);
    menuDocks->addAction(actionBookmarks);
    menuDocks->addAction(actionQuickJump);
    menuDocks->addAction(actionStrong);
    menuDocks->addAction(actionSearchResults);

    QMenu *menuSettings = new QMenu(tr("Settings"), bar);

    //Config
    QAction *actionConfiguration = new QAction(QIcon::fromTheme("configure", QIcon(":/icons/16x16/configure.png")), tr("Configuration"), menuEdit);
    connect(actionConfiguration, SIGNAL(triggered()), this->parent(), SLOT(showSettingsDialog_General()));
    actionConfiguration->setShortcut(QKeySequence::Preferences);

    menuSettings->addMenu(menuDocks);
    menuSettings->addSeparator();
    menuSettings->addAction(actionConfiguration);

    QMenu *menuHelp = new QMenu(tr("Help"), bar);
    //About
    QAction *actionAbout = new QAction(QIcon::fromTheme("help-about", QIcon(":/icons/16x16/help-about.png")), tr("About"), menuHelp);
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

    //Online Help
    QAction *actionOnlineHelp = new QAction(QIcon::fromTheme("help-contents", QIcon(":/icons/16x16/help-contents.png")), tr("Online Help"), menuHelp);
    connect(actionOnlineHelp, SIGNAL(triggered()), this, SLOT(onlineHelp()));
    actionOnlineHelp->setShortcut(QKeySequence::HelpContents);

    menuHelp->addAction(actionOnlineHelp);
    menuHelp->addAction(actionAbout);


    bar->addMenu(menuFile);
    bar->addMenu(menuEdit);
    bar->addMenu(menuView);
    bar->addMenu(menuNotes);
    bar->addMenu(menuSettings);
    bar->addMenu(menuHelp);
    return bar;
}

bool AdvancedInterface::hasToolBar()
{
    return true;
}
void AdvancedInterface::createToolBars()
{
    //DEBUG_FUNC_NAME
    m_mainBar = new QToolBar(this);
    m_mainBar->setObjectName("mainToolBar");
    m_mainBar->setIconSize(QSize(16, 16));
#ifdef Q_WS_WIN
    m_mainBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
#else
    m_mainBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
#endif
    m_mainBar->setWindowTitle(tr("Main Tool Bar"));

    m_mainBarActionSearch = new QAction(QIcon::fromTheme("edit-find", QIcon(":/icons/16x16/edit-find.png")), tr("Search"), m_mainBar);
    connect(m_mainBarActionSearch, SIGNAL(triggered()), m_searchManager, SLOT(showSearchDialog()));

    m_mainBarActionBookmarks = new QAction(QIcon::fromTheme("bookmarks-organize", QIcon(":/icons/16x16/bookmarks-organize.png")), tr("Bookmarks"), m_mainBar);
    m_mainBarActionBookmarks->setCheckable(true);
    connect(m_bookmarksManager->bookmarksDockWidget(), SIGNAL(visibilityChanged(bool)), m_mainBarActionBookmarks, SLOT(setChecked(bool)));
    connect(m_mainBarActionBookmarks, SIGNAL(triggered(bool)), m_bookmarksManager->bookmarksDockWidget(), SLOT(setVisible(bool)));

    m_mainBarActionNotes = new QAction(QIcon::fromTheme("view-pim-notes", QIcon(":/icons/16x16/view-pim-notes.png")), tr("Notes"), m_mainBar);
    m_mainBarActionNotes->setCheckable(true);
    connect(m_notesManager->notesDockWidget(), SIGNAL(visibilityChanged(bool)), m_mainBarActionNotes, SLOT(setChecked(bool)));
    connect(m_mainBarActionNotes, SIGNAL(triggered(bool)), m_notesManager->notesDockWidget(), SLOT(setVisible(bool)));

    m_mainBarActionNewWindow = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("New Window"), m_mainBar);
    connect(m_mainBarActionNewWindow, SIGNAL(triggered()), m_windowManager, SLOT(newSubWindow()));

    m_mainBarActionZoomIn = new QAction(QIcon::fromTheme("zoom-in", QIcon(":/icons/16x16/zoom-in.png")), tr("Zoom In"), m_mainBar);
    connect(m_mainBarActionZoomIn, SIGNAL(triggered()), m_windowManager, SLOT(zoomIn()));

    m_mainBarActionZoomOut = new QAction(QIcon::fromTheme("zoom-out", QIcon(":/icons/16x16/zoom-out.png")), tr("Zoom Out"), m_mainBar);
    connect(m_mainBarActionZoomOut, SIGNAL(triggered()), m_windowManager, SLOT(zoomOut()));

    m_mainBarActionModule = new QAction(QIcon(":/icons/32x32/module.png"), tr("Module"), m_mainBar);
    connect(m_mainBarActionModule, SIGNAL(triggered()), this->parent(), SLOT(showSettingsDialog_Module()));

    m_mainBar->addAction(m_mainBarActionSearch);
    m_mainBar->addSeparator();
    m_mainBar->addAction(m_mainBarActionNewWindow);
    m_mainBar->addSeparator();
    m_mainBar->addAction(m_mainBarActionZoomIn);
    m_mainBar->addAction(m_mainBarActionZoomOut);
    m_mainBar->addSeparator();
    m_mainBar->addAction(m_mainBarActionModule);
    m_mainBar->addSeparator();
    m_mainBar->addAction(m_mainBarActionBookmarks);
    m_mainBar->addAction(m_mainBarActionNotes);

    m_searchBar = new QToolBar(this);
    m_searchBar->setObjectName("quickToolBar");
    m_searchBar->setIconSize(QSize(16, 16));
    m_searchBar->setWindowTitle(tr("Quick Bar"));
    m_searchBar->setMaximumWidth(250);


    m_quickLine = new HistoryLineEdit(m_searchBar);
    m_quickLine->setPlaceholderText(tr("Command"));
    m_quickLine->setObjectName("quickLine");
    m_quickLine->setFocusPolicy(Qt::StrongFocus);
    //m_quickLine->setHistory(m_settings->session.getData("advancedInterfaceQuickLineHistory"));
    connect(m_quickLine, SIGNAL(returnPressed()), this, SLOT(quick()));
    m_searchBar->addWidget(m_quickLine);

    QShortcut *s1 = new QShortcut(QKeySequence(tr("Esc", "Focus QuickOpen")), this);
    connect(s1, SIGNAL(activated()), this, SLOT(focusQuickOpen()));
    
    QShortcut *sFullScreen = new QShortcut(QKeySequence(tr("F11", "Enable/Disable Fullscreen mode")), this);
    connect(sFullScreen, SIGNAL(activated()), m_actions, SLOT(toggleFullScreen()));

    QShortcut *sReload = new QShortcut(QKeySequence(tr("F5", "Reload")), this);
    connect(sReload , SIGNAL(activated()), m_actions, SLOT(reloadCurrentRange()));

    toolBarSetText();
}

QList<QToolBar *> AdvancedInterface::toolBars()
{
    QList<QToolBar *> list;
    list.append(m_mainBar);
    list.append(m_searchBar);
    return list;
}
void AdvancedInterface::quickSearch(const QString &text)
{
    SearchQuery query;
    query.searchText = text;
    query.searchInNotes = true;
    query.queryType = SearchQuery::Simple;
    m_searchManager->search(query);
}
void AdvancedInterface::quick()
{
    quick(((QLineEdit *) sender())->text());
}
void AdvancedInterface::focusQuickOpen()
{
    m_quickLine->setFocus();
    m_quickLine->setText(QString());
}

void AdvancedInterface::quick(QString text)
{
    myDebug() << text;
    if(text.startsWith("search ", Qt::CaseInsensitive) || text.startsWith("s ", Qt::CaseInsensitive)) {
        int whitePos = text.indexOf(" ");
        text.remove(0, whitePos);
        quickSearch(text);
        return;
    } else if(text.startsWith("search:", Qt::CaseInsensitive) || text.startsWith("s:",Qt::CaseInsensitive)) {
        int whitePos = text.indexOf(":");
        text.remove(0, whitePos);
        quickSearch(text);
        return;
    } else if(text.startsWith("open ", Qt::CaseInsensitive)) {
        int whitePos = text.indexOf(" ");
        text.remove(0, whitePos);
    } else if(text.startsWith("open:", Qt::CaseInsensitive)) {
        int whitePos = text.indexOf(":");
        text.remove(0, whitePos);
    } else if(text.compare("new tab", Qt::CaseInsensitive) == 0) {
        m_windowManager->newBibleSubWindow();
        return;
    } else if(text.compare("new dict", Qt::CaseInsensitive) == 0) {
        m_windowManager->newDictionarySubWindow();
        return;
    } else if(text.compare("new web", Qt::CaseInsensitive) == 0) {
        m_windowManager->newWebSubWindow();
        return;
    } else if(text.compare("new com", Qt::CaseInsensitive) == 0) {
        m_windowManager->newCommentarySubWindow();
        return;
    } else if(text.compare("new bible", Qt::CaseInsensitive) == 0) {
        m_windowManager->newBibleSubWindow();
        return;
    } else if(text.compare("new console", Qt::CaseInsensitive) == 0) {
        m_windowManager->newConsoleSubWindow();
        return;
    }


    StrongUrl url;
    if(url.fromText(text)) {
        m_actions->get(url.toString());
    } else if(m_windowManager->activeForm()->type() == Form::BibleForm || BibleLink::fastIsBibleLink(text)){
        QMdiSubWindow* window = m_windowManager->needWindow(Form::BibleForm);
        BibleForm * f = (BibleForm*) m_windowManager->getForm(window);

        if(f->verseTableLoaded()) {
            BibleLink link(f->verseModule()->moduleID(), f->verseModule()->versification());
            if(link.isBibleLink(text)) {
                m_actions->get(link.getUrl(text));
            }  else {
                quickSearch(text);
            }
        } else {

            int defaultModuleID = -1;
            QMapIterator<int, ModuleSettings*> i(m_settings->m_moduleSettings);
            while(i.hasNext()) {
                i.next();
                if(i.value()->defaultModule == ModuleTools::DefaultBibleModule) {
                    defaultModuleID = i.key();
                    break;
                }

            }
            if(defaultModuleID == -1) {
                auto i2 = m_moduleManager->m_moduleMap->it();
                while(i2.hasNext() && defaultModuleID == -1) {
                    i2.next();
                    if(i2.value()->moduleClass() == ModuleTools::BibleModuleClass) {
                        defaultModuleID = i2.key();
                        break;
                    }
                }
            }

            BibleLink link(defaultModuleID, m_settings->getV11N(defaultModuleID));
            if(link.isBibleLink(text)) {
                m_actions->get(link.getUrl(text));
            }
        }
    } else {
        quickSearch(text);
    }
}

void AdvancedInterface::onlineHelp()
{
    //open the online faq
    QDesktopServices::openUrl(QString("http://metaxy.github.io/openBibleViewer/"));
}

void AdvancedInterface::printFile(void)
{
    if(m_windowManager->activeForm()) {
        m_windowManager->activeForm()->print();
    }
}
void AdvancedInterface::printPreview(void)
{
    if(m_windowManager->activeForm()) {
        m_windowManager->activeForm()->printPreview();
    }
}
void AdvancedInterface::saveFile(void)
{
    if(m_windowManager->activeForm()) {
        m_windowManager->activeForm()->saveFile();
    }
}

void AdvancedInterface::showAboutDialog(void)
{
    QPointer<AboutDialog> aDialog = new AboutDialog(this);
    aDialog->setWindowTitle(tr("About openBibleViewer"));
    aDialog->setText(tr("openBibleViewer <br /> version: %1 build: %2<br /> "
                        "<a href='http://metaxy.github.io/openBibleViewer/'>Official Website</a><br />"
                        "<a href='https://github.com/metaxy/openBibleViewer/issues'>Bug report</a>")
                     .arg(m_settings->version).arg(m_settings->build));
    aDialog->exec();
    delete aDialog;
}

void AdvancedInterface::showMarkCategories()
{

}

void AdvancedInterface::showMarkList()
{
    QPointer<MarkList> markList = new MarkList(this);
    setAll(markList);
    markList->init();
    markList->exec();
    delete markList;
}

void AdvancedInterface::showNotesEditor()
{
    QPointer<NotesEditor> notesEditor = new NotesEditor(this);
    setAll(notesEditor);
    notesEditor->init();
    notesEditor->show();
}

void AdvancedInterface::setTabbedView()
{
    m_actionTabbedView->setChecked(true);
    m_actionSubWindowView->setChecked(false);
}

void AdvancedInterface::setSubWindowView()
{
    m_actionTabbedView->setChecked(false);
    m_actionSubWindowView->setChecked(true);
}
void AdvancedInterface::toolBarSetText()
{
    m_mainBar->setWindowTitle(tr("Main Tool Bar"));
    m_mainBarActionSearch->setText(tr("Search"));
    m_mainBarActionSearch->setToolTip(tr("Search in current module."));
    m_mainBarActionBookmarks->setText(tr("Bookmarks"));
    m_mainBarActionBookmarks->setToolTip(tr("Show/Hide the bookmarks dock."));
    m_mainBarActionNotes->setText(tr("Notes"));
    m_mainBarActionNotes->setToolTip(tr("Show/Hide the notes dock."));
    m_mainBarActionNewWindow->setText(tr("New Window"));
    m_mainBarActionZoomIn->setText(tr("Zoom In"));
    m_mainBarActionZoomIn->setToolTip(tr("Zoom In"));
    m_mainBarActionZoomOut->setText(tr("Zoom Out"));
    m_mainBarActionZoomOut->setToolTip(tr("Zoom Out"));
    m_mainBarActionModule->setText(tr("Module"));
    m_mainBarActionModule->setToolTip(tr("Add and edit modules"));
    m_searchBar->setWindowTitle(tr("Search Bar"));
    m_quickLine->setToolTip(tr("Quick open"));
}
void AdvancedInterface::uncheck(bool b)
{
    if(b) {
        if(sender()->objectName() != "actionCascade")
            m_actionCascade->setChecked(false);
        if(sender()->objectName() != "actionTile")
            m_actionTile->setChecked(false);
        if(sender()->objectName() != "actionTileVertical")
            m_actionTileVertical->setChecked(false);
        if(sender()->objectName() != "actionTileHorizontal")
            m_actionTileHorizontal->setChecked(false);
    }
}

void AdvancedInterface::changeEvent(QEvent *e)
{
    DEBUG_FUNC_NAME;
    myDebug() << e->type();

    QWidget::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        toolBarSetText();
        break;
    default:
        break;
    }

}
QString AdvancedInterface::name() const
{
    return "advanced";
}
