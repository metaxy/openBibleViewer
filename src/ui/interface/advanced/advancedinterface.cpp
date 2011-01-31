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
#include "advancedinterface.h"
#include "ui_advancedinterface.h"


AdvancedInterface::AdvancedInterface(QWidget *parent) :
    Interface(parent),
    ui(new Ui::AdvancedInterface)
{
    ui->setupUi(this);
}

AdvancedInterface::~AdvancedInterface()
{
    DEBUG_FUNC_NAME;

    if(m_bibleDisplaySettings != NULL) {
        delete m_bibleDisplaySettings;
        m_bibleDisplaySettings = 0;
    }

    if(ui != NULL) {
        delete ui;
        ui = 0;
    }
}

void AdvancedInterface::init()
{
    DEBUG_FUNC_NAME
    m_bibleDisplaySettings = new BibleDisplaySettings();
    m_bibleDisplaySettings->setShowMarks(true);
    m_bibleDisplaySettings->setShowNotes(true);
    m_bibleDisplaySettings->setLoadNotes(true);

    connect(m_actions, SIGNAL(_get(QString)), this, SLOT(pharseUrl(QString)));

    m_moduleManager->setBibleDisplaySettings(m_bibleDisplaySettings);

    m_api = new Api();
    setAll(m_api);
    m_api->init();
    m_bibleManager = new BibleManager(this);
    setAll(m_bibleManager);
    m_bibleManager->setWidget(this->parentWidget());
    m_bibleManager->init();

    m_notesManager = new NotesManager(this);
    m_notesManager->setWidget(this->parentWidget());
    setAll(m_notesManager);

    m_bookmarksManager = new BookmarksManager(this);
    setAll(m_bookmarksManager);
    m_bookmarksManager->setWidget(this->parentWidget());

    m_windowManager = new WindowManager(this);
    setAll(m_windowManager);
    m_windowManager->setMdiArea(ui->mdiArea);
    m_windowManager->setApi(m_api);
    m_windowManager->setBibleManager(m_bibleManager);
    m_windowManager->setNotesManager(m_notesManager);
    m_windowManager->setBookmarksManager(m_bookmarksManager);

    m_windowManager->init();

    m_searchManager = new SearchManager(this);
    setAll(m_searchManager);
    m_searchManager->setWindowManager(m_windowManager);
    m_searchManager->setWidget(this->parentWidget());



    if(m_settings->session.getData("windowUrls").toStringList().size() == 0)
        QTimer::singleShot(10, m_windowManager, SLOT(newSubWindow()));
    connect(m_actions, SIGNAL(_setTitle(QString)), this , SLOT(setTitle(QString)));
    connect(m_actions, SIGNAL(_setTabbedView()), this, SLOT(setTabbedView()));
    connect(m_actions, SIGNAL(_setSubWindowView()), this, SLOT(setSubWindowView()));
}
void AdvancedInterface::createDocks()
{
    m_bibleManager->createDocks();
    m_notesManager->createDocks();
    m_searchManager->createDocks();
    m_bookmarksManager->createDocks();

    m_dictionaryDockWidget = new DictionaryDockWidget(this->parentWidget());
    setAll(m_dictionaryDockWidget);
    m_dictionaryDockWidget->init();
    m_dictionaryDockWidget->hide();
}
void AdvancedInterface::createMenu()
{

}

QHash<DockWidget*, Qt::DockWidgetArea> AdvancedInterface::docks()
{
    DEBUG_FUNC_NAME
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.unite(m_bibleManager->docks());
    ret.unite(m_notesManager->docks());
    ret.unite(m_searchManager->docks());
    ret.unite(m_bookmarksManager->docks());
    ret.insert(m_dictionaryDockWidget, Qt::BottomDockWidgetArea);
    return ret;

}

void AdvancedInterface::pharseUrl(QUrl url)
{
    DEBUG_FUNC_NAME
    pharseUrl(url.toString());
}
void AdvancedInterface::pharseUrl(QString url)
{
    DEBUG_FUNC_NAME

    QString url_backup = url;
    //setEnableReload(false);
    myDebug() << "url = " << url;

    const QString bible = "verse:/";
    const QString strong = "strong://";
    const QString gram = "gram://";
    const QString http = "http://";
    const QString bq = "go";
    const QString anchor = "#";
    const QString note = "note://";
    const QString persistent = "persistent:";

    if(url.startsWith(bible)) {
        m_bibleManager->pharseUrl(url);
    } else if(url.startsWith(strong)) {
        url = url.remove(0, strong.size());
        m_dictionaryDockWidget->showStrong(url);
        //strong://strongID
    } else if(url.startsWith(gram)) {
        url = url.remove(0, gram.size());
        m_dictionaryDockWidget->showStrong(url);
        //gram://strongID
    } else if(url.startsWith(http)) {
        QDesktopServices::openUrl(url);
        //its a web link
    } else if(url.startsWith(bq)) {
        m_bibleManager->pharseUrl(url);
    } else if(url.startsWith(anchor)) {
        //todo:
        /*url = url.remove(0, anchor.size());
        bool ok;
        int c = url.toInt(&ok, 10);
        if(ok && c < m_moduleManager->bible()->chaptersCount() && m_moduleManager->bible()->bibleType() == OBVCore::BibleQuoteModule && m_moduleManager->bible()->chapterID() != c) {
            showChapter(c, 0);
            setCurrentChapter(c);
        } else {
            if(m_windowManager->activeForm())
                m_windowManager->activeForm()->scrollToAnchor(url);
        }*/

    } else if(url.startsWith(note)) {
        url = url.remove(0, note.size());
        m_notesManager->openNote(url);
    } else if(url.startsWith(persistent)) {
        url = url.remove(0, persistent.size());
        /*UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
        urlConverter.setSettings(m_settings);
        urlConverter.setModuleMap(m_moduleManager->m_moduleMap);
        urlConverter.pharse();
        const QString i = urlConverter.convert();//it now a normal interface url
        pharseUrl(i);*/
    } else {
        //todo: unterstand links like about:blank#a04
        //todo: uncomment
        /*if(m_moduleManager->verseModule()->moduleType() == OBVCore::BibleQuoteModule) {
            myDebug() << m_moduleManager->verseModule()->modulePath();
            bool isInBookPath = false;
            int b = 0;
            const QStringList books = m_moduleManager->verseModule()->bookPath();
            myDebug() << books;
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
                myDebug() << "getting";
                m_actions->get("bible://current/" + QString::number(b));
            }
        } else {
            if(m_windowManager->activeForm())
                m_windowManager->activeForm()->evaluateJavaScript(url);
        }*/
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
    DEBUG_FUNC_NAME
    //reload books
    bool reloadBibles = false;
    if(oldSettings.encoding != newSettings.encoding) {
        myDebug() << "encoding changed from "  << oldSettings.encoding << " to " << newSettings.encoding;
        reloadBibles = true;
    }
    if(reloadBibles == false) {
        if(oldSettings.m_moduleSettings.size() != newSettings.m_moduleSettings.size()) {
            myDebug() << "module settings size changed from "  << oldSettings.m_moduleSettings.size() << " to " << newSettings.m_moduleSettings.size();
            reloadBibles = true;
        }
    }
    if(modifedModuleSettings == true) {
        reloadBibles = true;
        /*
        for(int i = 0; i < newSettings.m_moduleSettings.size(); ++i) {
            if(oldSettings.m_moduleSettings.size() < i || oldSettings.m_moduleSettings.empty()) {
                myDebug() << "not enough " << i;
                reloadBibles = true;
                break;
            } else {
                ModuleSettings m1, m2;
                m1 = newSettings.m_moduleSettings.at(i);
                m2 = oldSettings.m_moduleSettings.at(i);
                if(memcmp(&m1, &m2, sizeof(ModuleSettings)) != 0) {
                    myDebug() << "m1 not equal m2";
                    reloadBibles = true;
                    break;
                }
            }*/
    }
    if(reloadBibles == true) {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        //myDebug() << "reload Module";
        m_moduleManager->loadAllModules();
        m_bibleManager->moduleDockWidget()->init();
        m_dictionaryDockWidget->init();
        //showText(""); //todo:
        //m_moduleManager->bible()->clearSoftCache();
        //if(m_moduleManager->bibleLoaded())

        //reloadChapter(true);
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
    QAction *actionNewSubWindow = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("New SubWindow"), menuFile);
    connect(actionNewSubWindow, SIGNAL(triggered()), m_windowManager, SLOT(newSubWindow()));
    actionNewSubWindow->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));//using const KeySequence on KDE will be Ctrl+Shift+N

    //Close Sub Window
    QAction *actionCloseSubWindow = new QAction(QIcon::fromTheme("tab-close", QIcon(":/icons/16x16/tab-close.png")), tr("Close SubWindow"), menuFile);
    connect(actionCloseSubWindow, SIGNAL(triggered()), m_windowManager, SLOT(closeSubWindow()));
    actionCloseSubWindow->setShortcut(QKeySequence::Close);

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

    menuFile->addAction(actionNewSubWindow);
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
    QAction *actionCascade = new QAction(QIcon(":/icons/svg/cascade.svg"), tr("Cascade"), menuView);
    connect(actionCascade, SIGNAL(triggered()), m_windowManager, SLOT(cascade()));

    //Tile
    QAction *actionTile = new QAction(QIcon(":/icons/svg/tile.svg"), tr("Tile"), menuView);
    connect(actionTile, SIGNAL(triggered()), m_windowManager, SLOT(tile()));

    //Tile Vertical
    QAction *actionTileVertical = new QAction(QIcon(":/icons/svg/tile_vert.svg"), tr("Tile Vertical"), menuView);
    connect(actionTileVertical, SIGNAL(triggered()), m_windowManager, SLOT(tileVertical()));

    //Tile Horizontal
    QAction *actionTileHorizontal = new QAction(QIcon(":/icons/svg/tile_horiz.svg"), tr("Tile Horizontal"), menuView);
    connect(actionTileHorizontal, SIGNAL(triggered()), m_windowManager, SLOT(tileHorizontal()));



    menuView->addAction(actionZoomIn);
    menuView->addAction(actionZoomOut);
    menuView->addSeparator();
    menuView->addAction(m_actionTabbedView);
    menuView->addAction(m_actionSubWindowView);
    menuView->addSeparator();
    menuView->addAction(actionCascade);
    menuView->addAction(actionTile);
    menuView->addAction(actionTileVertical);
    menuView->addAction(actionTileHorizontal);


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
    connect(m_dictionaryDockWidget, SIGNAL(visibilityChanged(bool)), actionStrong, SLOT(setChecked(bool)));
    connect(actionStrong, SIGNAL(triggered(bool)), m_dictionaryDockWidget, SLOT(setVisible(bool)));
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
    DEBUG_FUNC_NAME
    m_mainBar = new QToolBar(this);
    m_mainBar->setObjectName("mainToolBar");
    m_mainBar->setIconSize(QSize(16, 16));
    m_mainBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
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
    m_searchBar->setObjectName("searchToolBar");
    m_searchBar->setIconSize(QSize(16, 16));
    m_searchBar->setWindowTitle(tr("Search Bar"));

    QLineEdit *edit = new QLineEdit(m_searchBar);
    edit->setObjectName("lineEdit");

    connect(edit, SIGNAL(returnPressed()), m_searchManager, SLOT(search()));
    m_searchBar->addWidget(edit);
}

QList<QToolBar *> AdvancedInterface::toolBars()
{
    QList<QToolBar *> list;
    list.append(m_mainBar);
    list.append(m_searchBar);
    return list;
}


void AdvancedInterface::showBookmarksDock()
{
    /*if(m_bookmarksDockWidget->isVisible()) {
        m_bookmarksDockWidget->hide();
    } else {
        m_bookmarksDockWidget->show();
    }*/
}

void AdvancedInterface::showNotesDock()
{
    /*if(m_notesDockWidget->isVisible()) {
        m_notesDockWidget->hide();
    } else {
        m_notesDockWidget->show();
    }*/
}

/*void AdvancedInterface::newBookmark()
{
    if(!m_moduleManager->bibleLoaded() && !m_windowManager->activeMdiChild())
        return;
    if(m_bookmarksDockWidget->isHidden())
        m_bookmarksDockWidget->show();
    m_bookmarksDockWidget->newBookmark(verseSelection());
}*//*

void AdvancedInterface::newNoteWithLink()
{
    if(!m_moduleManager->bibleLoaded() && !m_windowManager->activeMdiChild())
        return;
    if(m_notesDockWidget->isHidden())
        m_notesDockWidget->show();
    m_notesDockWidget->newNoteWithLink(verseSelection());
}
*/
void AdvancedInterface::onlineHelp()
{
    //open the online faq
    QDesktopServices::openUrl(tr("http://openbv.uucyc.name/faq.html"));
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

int AdvancedInterface::showAboutDialog(void)
{
    AboutDialog aDialog;
    aDialog.setWindowTitle(tr("About openBibleViewer"));
    aDialog.show();
    aDialog.setText(tr("openBibleViewer <br /> version: %1 build: %2<br /> "
                       "<a href='http://openbv.uucyc.name/'> Official Website</a> <br />"
                       "<a href='https://github.com/metaxy/openBibleViewer/issues'>Bug report</a>")
                    .arg(m_settings->version).arg(m_settings->build));
    return aDialog.exec();
}

void AdvancedInterface::showMarkCategories()
{

}

void AdvancedInterface::showMarkList()
{
    //todo: scoped pointer ?
    MarkList *markList = new MarkList(this);
    setAll(markList);
    markList->init();
    markList->exec();
}

void AdvancedInterface::showNotesEditor()
{
    NotesEditor *notesEditor = new NotesEditor(this);
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

void AdvancedInterface::changeEvent(QEvent *e)
{
    //QWidget::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        //retranslate menu bar
        m_mainBar->setWindowTitle(tr("Main Tool Bar"));
        m_mainBarActionSearch->setText(tr("Search"));
        m_mainBarActionBookmarks->setText(tr("Bookmarks"));
        m_mainBarActionNotes->setText(tr("Notes"));
        m_mainBarActionNewWindow->setText(tr("New Window"));
        m_mainBarActionZoomIn->setText(tr("Zoom In"));
        m_mainBarActionZoomOut->setText(tr("Zoom Out"));
        m_mainBarActionModule->setText(tr("Module"));
        m_searchBar->setWindowTitle(tr("Search Bar"));

        break;
    default:
        break;
    }
}
