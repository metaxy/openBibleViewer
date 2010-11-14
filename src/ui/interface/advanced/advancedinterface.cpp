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
#include "advancedinterface.h"
#include "ui_advancedinterface.h"
#include "src/core/faststart.h"
#include "src/core/core.h"
#include "src/core/search.h"
#include "src/core/bibleurl.h"
#include "src/core/dbghelper.h"
#include "src/ui/dialog/searchdialog.h"
#include "src/ui/dialog/aboutdialog.h"
#include "src/ui/noteseditor.h"
#include "src/ui/marklist.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QSizePolicy>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopServices>
#include <QtGui/QClipboard>
#include <QtGui/QTextDocumentFragment>
#include <QtGui/QScrollBar>
#include <QtGui/QFileDialog>
#include <QtGui/QPrintDialog>
#include <QtGui/QPrinter>
#include <QtGui/QColorDialog>
#include <QtGui/QKeySequence>
#include <QtGui/QLineEdit>
#include <QtCore/QScopedPointer>
#include <QtCore/QTimer>
#include <QtWebKit/QWebInspector>
#include <QtWebKit/QWebElementCollection>

AdvancedInterface::AdvancedInterface(QWidget *parent) :
    Interface(parent),
    ui(new Ui::AdvancedInterface)
{
    ui->setupUi(this);
    createToolBars();
    m_lastActiveWindow = -1;
}

AdvancedInterface::~AdvancedInterface()
{
    m_windowCache.clearAll();
    if(m_bibleDisplaySettings != NULL) {
        delete m_bibleDisplaySettings;
        m_bibleDisplaySettings = 0;
    }

    if(ui != NULL) {
        delete ui;
        ui = 0;
    }
}

void AdvancedInterface::setBookDockWidget(BookDockWidget *bookDockWidget)
{
    m_bookDockWidget = bookDockWidget;
}

void AdvancedInterface::setModuleDockWidget(ModuleDockWidget *moduleDockWidget)
{
    m_moduleDockWidget = moduleDockWidget;
}

void AdvancedInterface::setAdvancedSearchResultDockWidget(AdvancedSearchResultDockWidget *advancedSearchResultDockWidget)
{
    m_advancedSearchResultDockWidget = advancedSearchResultDockWidget;
}

void AdvancedInterface::setNotesDockWidget(NotesDockWidget *notesDockWidget)
{
    m_notesDockWidget = notesDockWidget;
}

void AdvancedInterface::setBookmarksDockWidget(BookmarksDockWidget *bookmarksDockWidget)
{
    m_bookmarksDockWidget = bookmarksDockWidget;
}

void AdvancedInterface::setDictionaryDockWidget(DictionaryDockWidget *dictionaryDockWidget)
{
    m_dictionaryDockWidget = dictionaryDockWidget;
}

void AdvancedInterface::setQuickJumpDockWidget(QuickJumpDockWidget *quickJumpDockWidget)
{
    m_quickJumpDockWidget = quickJumpDockWidget;
}

void AdvancedInterface::init()
{
    m_bibleDisplaySettings = new BibleDisplaySettings();
    m_bibleDisplaySettings->setShowMarks(true);
    m_bibleDisplaySettings->setShowNotes(true);
    m_bibleDisplaySettings->setLoadNotes(true);


    connect(m_bibleDisplay, SIGNAL(newHtml(QString)), this, SLOT(showText(QString)));
    connect(m_bibleDisplay, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));
    connect(m_bibleDisplay, SIGNAL(get(QUrl)), this, SLOT(pharseUrl(QUrl)));

    setAll(m_moduleDockWidget);
    m_moduleDockWidget->init();
    connect(m_moduleDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    setAll(m_bookDockWidget);
    m_bookDockWidget->hide();
    connect(m_bookDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    setAll(m_advancedSearchResultDockWidget);
    m_advancedSearchResultDockWidget->init();
    m_advancedSearchResultDockWidget->hide();
    connect(m_advancedSearchResultDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    setAll(m_notesDockWidget);
    m_notesDockWidget->init();
    m_notesDockWidget->hide();
    connect(m_notesDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));
    connect(m_notesDockWidget, SIGNAL(reloadChapter()), this, SLOT(reloadChapter()));
    connect(m_notesDockWidget, SIGNAL(visibilityChanged(bool)), m_mainBarActionNotes, SLOT(setChecked(bool)));

    setAll(m_bookmarksDockWidget);
    m_bookmarksDockWidget->init();
    m_bookmarksDockWidget->hide();
    connect(m_bookmarksDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));
    connect(m_bookmarksDockWidget, SIGNAL(visibilityChanged(bool)), m_mainBarActionBookmarks, SLOT(setChecked(bool)));


    setAll(m_dictionaryDockWidget);
    m_dictionaryDockWidget->init();
    m_dictionaryDockWidget->hide();

    setAll(m_quickJumpDockWidget);
    m_quickJumpDockWidget->init();
    m_quickJumpDockWidget->hide();
    connect(m_quickJumpDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));


    connect(this, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    m_moduleManager->setBibleDisplaySettings(m_bibleDisplaySettings);

    m_bibleApi = new BibleApi();
    setAll(m_bibleApi);

    createDefaultMenu();
    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(reloadWindow(QMdiSubWindow *)));

    if(m_settings->session.getData("windowUrls").toStringList().size() == 0)
        QTimer::singleShot(10, this, SLOT(newSubWindow()));
    
    //QTimer::singleShot(1000, this, SLOT(installResizeFilter()));
    

}
/*
 * Todo: Use it
 */
void AdvancedInterface::installResizeFilter()
{
    m_mdiAreaFilter = new MdiAreaFilter(ui->mdiArea);
    connect(m_mdiAreaFilter, SIGNAL(resized()), this, SLOT(mdiAreaResized()));
    ui->mdiArea->installEventFilter(m_mdiAreaFilter);
}

void AdvancedInterface::attachApi()
{
    QWebFrame * frame = getView()->page()->mainFrame();
    /*{
        QFile file(":/data/js/jquery-1.4.2.min.js");
        if(!file.open(QFile::ReadOnly | QFile::Text))
            return;
        QTextStream stream(&file);
        QString jquery = stream.readAll();
        file.close();
        frame->evaluateJavaScript(jquery);
    }
*/
    {
        QFile file(":/data/js/tools.js");
        if(!file.open(QFile::ReadOnly | QFile::Text))
            return;
        QTextStream stream(&file);
        QString tools = stream.readAll();
        file.close();
        frame->evaluateJavaScript(tools);
    }

    frame->addToJavaScriptWindowObject("Bible", m_bibleApi);
    m_bibleApi->setFrame(frame);
}

void AdvancedInterface::newSubWindow(bool doAutoLayout)
{
    DEBUG_FUNC_NAME
    m_enableReload = false;
    int windowsCount = usableWindowList().size();
    QMdiSubWindow *firstSubWindow = new QMdiSubWindow();
    if(windowsCount == 1) {
        firstSubWindow = usableWindowList().at(0);
    }

    const int windowName = m_windowCache.newWindow();

    QWidget *widget = new QWidget(ui->mdiArea);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    MdiForm *mForm = new MdiForm(widget);
    mForm->setObjectName("mdiForm");
    setAll(mForm);
    layout->addWidget(mForm);

    widget->setLayout(layout);
    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(widget);
    subWindow->setObjectName(QString::number(windowName));
    subWindow->setWindowIcon(QIcon(":/icons/16x16/main.png"));
    subWindow->setWindowOpacity(1.0);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();
    ui->mdiArea->setActiveSubWindow(subWindow);
    attachApi();

    m_moduleManager->m_bibleList = new BibleList();
    Bible *b = new Bible();
    m_moduleManager->initBible(b);
    m_moduleManager->bibleList()->addBible(b, QPoint(0, 0));
    m_windowCache.setBibleList(m_moduleManager->m_bibleList);


    if(ui->mdiArea->viewMode() == QMdiArea::SubWindowView) {
        if(windowsCount == 0  && doAutoLayout) {
            subWindow->showMaximized();
        } else if(windowsCount == 1 && doAutoLayout) {
            firstSubWindow->resize(600, 600);
            firstSubWindow->showNormal();
            subWindow->resize(600, 600);
            subWindow->show();
        } else if(doAutoLayout) {
            subWindow->resize(600, 600);
            subWindow->show();
        }
    }

    connect(mForm->m_view->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(pharseUrl(QUrl)));
    connect(mForm->m_view, SIGNAL(contextMenuRequested(QContextMenuEvent*)), this, SLOT(showContextMenu(QContextMenuEvent*)));
    connect(mForm->m_view->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attachApi()));

    mForm->m_view->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    mForm->m_view->settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
#if QT_VERSION >= 0x040700
    mForm->m_view->settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    mForm->m_view->settings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    mForm->m_view->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    mForm->m_view->settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
    mForm->m_view->settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
#endif

    connect(mForm, SIGNAL(historyGo(QString)), this, SLOT(pharseUrl(QString)));
    connect(mForm, SIGNAL(previousChapter()), this, SLOT(previousChapter()));
    connect(mForm, SIGNAL(nextChapter()), this, SLOT(nextChapter()));
    connect(this, SIGNAL(historySetUrl(QString)), mForm, SLOT(historyGetUrl(QString)));
    connect(subWindow, SIGNAL(destroyed(QObject*)), this, SLOT(closingWindow()));
    m_enableReload = true;
    if(doAutoLayout && ui->mdiArea->viewMode() == QMdiArea::SubWindowView) {
        autoLayout();
    }

    //clear old stuff
    clearBooks();
    clearChapters();
}

void AdvancedInterface::autoLayout()
{
    if(usableWindowList().size() > 1) {
        if(m_settings->autoLayout == 1) {
            myTile();
        } else if(m_settings->autoLayout == 2) {
            myTileVertical();
        } else if(m_settings->autoLayout == 3) {
            myTileHorizontal();
        } else if(m_settings->autoLayout == 4) {
            myCascade();
        }
    }
}

QMdiSubWindow * AdvancedInterface::activeMdiChild()
{
    QList<QMdiSubWindow*> list = usableWindowList();
    if(QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow()) {
        for(int i = 0; i < list.size(); i++) {
            if(list.at(i) == activeSubWindow) {
                m_lastActiveWindow = i;
            }
        }
        return activeSubWindow;
    } else if(m_lastActiveWindow < list.size() && m_lastActiveWindow >= 0) {
        ui->mdiArea->setActiveSubWindow(usableWindowList().at(m_lastActiveWindow));
        if(QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
            return activeSubWindow;
    } else if(usableWindowList().count() > 0) {
        ui->mdiArea->setActiveSubWindow(usableWindowList().at(0));
        if(QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
            return activeSubWindow;
    }
    return 0;
}

void AdvancedInterface::myTileVertical()
{
    if(!m_enableReload || !usableWindowList().count()) {
        return;
    }
    QList<QMdiSubWindow*> windows = usableWindowList();
    m_enableReload = false;
    QMdiSubWindow* active = ui->mdiArea->activeSubWindow();

    const int widthForEach = width() / windows.count();
    unsigned int x = 0;
    foreach(QMdiSubWindow * window, windows) {
        window->showNormal();

        const int preferredWidth = window->minimumWidth() + window->baseSize().width();
        const int actWidth = qMax(widthForEach, preferredWidth);

        window->setGeometry(x, 0, actWidth, height());
        x += actWidth;
    }

    if(active) active->setFocus();
    m_enableReload = true;
}

void AdvancedInterface::myTileHorizontal()
{
    if(!m_enableReload || !usableWindowList().count()) {
        return;
    }
    QList<QMdiSubWindow*> windows = usableWindowList();
    setEnableReload(false);
    QMdiSubWindow* active = ui->mdiArea->activeSubWindow();

    const int heightForEach = height() / windows.count();
    unsigned int y = 0;
    foreach(QMdiSubWindow * window, windows) {
        window->showNormal();

        const int preferredHeight = window->minimumHeight() + window->baseSize().height();
        const int actHeight = qMax(heightForEach, preferredHeight);

        window->setGeometry(0, y, width(), actHeight);
        y += actHeight;
    }
    if(active)
        active->setFocus();
    setEnableReload(true);
}

void AdvancedInterface::myCascade()
{
    if(!m_enableReload || !usableWindowList().count()) {
        return;
    }

    QList<QMdiSubWindow*> windows = usableWindowList();

    if(ui->mdiArea->activeSubWindow() && ui->mdiArea->activeSubWindow()->isMaximized()) {
        if(ui->mdiArea->activeSubWindow()->size() != this->size()) {
            ui->mdiArea->activeSubWindow()->resize(this->size());
        }
    } else if(windows.count() == 1) {
        windows.at(0)->showMaximized();
    } else {

        QMdiSubWindow* active = ui->mdiArea->activeSubWindow();
        setEnableReload(false);
        const unsigned int offsetX = 40;
        const unsigned int offsetY = 40;
        const unsigned int windowWidth =  ui->mdiArea->width() - (windows.count() - 1) * offsetX;
        const unsigned int windowHeight = ui->mdiArea->height() - (windows.count() - 1) * offsetY;
        unsigned int x = 0;
        unsigned int y = 0;

        foreach(QMdiSubWindow * window, windows) {
            if(window == active) //leave out the active window which should be the top window
                continue;
            window->raise(); //make it the on-top-of-window-stack window to make sure they're in the right order
            window->setGeometry(x, y, windowWidth, windowHeight);
            x += offsetX;
            y += offsetY;
        }
        active->setGeometry(x, y, windowWidth, windowHeight);
        active->raise();
        active->activateWindow();
        setEnableReload(true);
    }
}

void AdvancedInterface::myTile()
{
    if(!m_enableReload || !usableWindowList().count()) {
        return;
    }
    ui->mdiArea->tileSubWindows();
}

QList<QMdiSubWindow*> AdvancedInterface::usableWindowList()
{
    QList<QMdiSubWindow*> ret;
    foreach(QMdiSubWindow * w, ui->mdiArea->subWindowList()) {
        if(w->isHidden())
            continue;
        ret.append(w);
    }
    return  ret;
}

int AdvancedInterface::currentWindowName()
{
    if(ui->mdiArea->activeSubWindow() != NULL) {
        return ui->mdiArea->activeSubWindow()->objectName().toInt();
    }
    return -1;
}


void AdvancedInterface::closeSubWindow()
{
    activeMdiChild()->close();
}

int AdvancedInterface::closingWindow()
{
    DEBUG_FUNC_NAME
    // myDebug() << "enable reload = " << m_enableReload << "subWIndowList = " << ui->mdiArea->subWindowList() << "internalWindow " << m_internalWindows;
    if(ui->mdiArea->subWindowList().isEmpty()) {
        myDebug() << "subWindowList is empty";
        clearBooks();
        clearChapters();
        m_windowCache.clearAll();
        return 1;
    }
    //if one in the internal subwindow list list is missing that window was closed
    QList<int> nameList = m_windowCache.nameList();
    QList<int> deletedList = nameList;
    foreach(QMdiSubWindow * win, ui->mdiArea->subWindowList()) {
        if(nameList.contains(win->objectName().toInt())) {
            deletedList.removeOne(win->objectName().toInt());
        }

    }
    foreach(const int & name, deletedList) {
        m_windowCache.removeWindow(name);
    }

    if(ui->mdiArea->subWindowList().isEmpty()) {  //last window closed
        myDebug() << "last closed";
        clearBooks();
        clearChapters();
        m_windowCache.clearAll();
        return 1;
    }
    reloadWindow(ui->mdiArea->currentSubWindow());
    if(ui->mdiArea->viewMode() == QMdiArea::SubWindowView)
        autoLayout();
    return 0;
}

int AdvancedInterface::reloadWindow(QMdiSubWindow * window)
{
    DEBUG_FUNC_NAME
    //myDebug() << " enable reload = " << m_enableReload;
    if(!m_enableReload || window == NULL) {
        return 1;
    }
    const int windowName = window->objectName().toInt();
    myDebug() << "window name = " << windowName;

    if(ui->mdiArea->subWindowList().count() <= 0) {
        return 1;
    }
    m_windowCache.setCurrentWindowName(windowName);
    BibleList *list = m_windowCache.getBibleList();
    if(list == NULL || list->bible() == NULL) {
        clearChapters();
        clearBooks();
        setTitle("");
        m_moduleManager->m_bibleList = NULL;
        return 1;
    }
    if(list->bible()->moduleID() < 0) {
        clearChapters();
        clearBooks();
        setTitle("");
        m_moduleManager->m_bibleList = list;
        return 1;
    }


    m_moduleManager->m_bibleList = list;
    myDebug() << "current bible title = " << m_moduleManager->bible()->bibleTitle();
    setTitle(m_moduleManager->bible()->bibleTitle());
    m_moduleDockWidget->loadedModule(m_moduleManager->bible()->moduleID());

    setChapters(m_moduleManager->bible()->chapterNames());
    setCurrentChapter(m_moduleManager->bible()->chapterID());

    setBooks(m_moduleManager->bible()->bookNames(), m_moduleManager->bible()->bookIDs());
    setCurrentBook(m_moduleManager->bible()->bookID());

    return 0;
}
void AdvancedInterface::mdiAreaResized()
{
    DEBUG_FUNC_NAME
    //todo: really every do time autoLayout ?
    if(ui->mdiArea->viewMode() == QMdiArea::SubWindowView)
        autoLayout();
}

bool AdvancedInterface::loadModuleDataByID(int moduleID)
{
    DEBUG_FUNC_NAME
    //myDebug() << "id = " << id;
    //open a new window if they are none
    if(ui->mdiArea->subWindowList().size() == 0)
        newSubWindow();

    if(moduleID < 0 || !m_moduleManager->contains(moduleID)) {
        myWarning() << "failed id = " << moduleID << m_moduleManager->m_moduleMap->m_map;
        return 1;
    }
    if(m_moduleManager->getModule(moduleID)->m_moduleClass != Module::BibleModule) {
        myWarning() << "non bible module " << m_moduleManager->getModule(moduleID)->m_moduleClass;
        return 1;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);

    m_windowCache.setCurrentWindowName(currentWindowName());
    m_windowCache.setBibleList(m_moduleManager->bibleList());

    Module::ModuleType type = m_moduleManager->getModule(moduleID)->m_moduleType;
    m_moduleManager->bible()->setModuleType(type);
    m_moduleManager->bible()->loadModuleData(moduleID);


    setTitle(m_moduleManager->bible()->bibleTitle());
    setBooks(m_moduleManager->bible()->bookNames(), m_moduleManager->bible()->bookIDs());
    m_moduleDockWidget->loadedModule(moduleID);//select current Module
    QApplication::restoreOverrideCursor();
    return 0;

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
    setEnableReload(false);
    myDebug() << "url = " << url;

    const QString bible = "bible://";
    const QString strong = "strong://";
    const QString gram = "gram://";
    const QString http = "http://";
    const QString bq = "go";
    const QString anchor = "#";
    const QString note = "note://";
    const QString persistent = "persistent:";

    if(url.startsWith(bible)) {
        BibleUrl bibleUrl;
        if(!bibleUrl.fromString(url)) {
            return;
        }
        if(bibleUrl.bible() == BibleUrl::ReloadActive) {
            reloadActive();
        } else {
            bool reloadBible = false;
            bool reloadBook = false;
            bool reloadChapter = false;
            bool reloadVerse = false;
            if(!activeMdiChild()) {
                newSubWindow();
            }
            if((bibleUrl.bibleID() != m_moduleManager->bible()->moduleID()  && bibleUrl.bible() == BibleUrl::LoadBibleByID) ||
                    !m_moduleManager->bible()->loaded() || (bibleUrl.getParam("force") == "true")) {
                reloadBible = true;
            }

            if(reloadBible) {
                int id;
                if(bibleUrl.bible() == BibleUrl::LoadBibleByID) {
                    id = bibleUrl.bibleID();
                } else if(bibleUrl.bible() == BibleUrl::LoadCurrentBible) {
                    id = m_moduleManager->bible()->moduleID();
                }
                int ret = loadModuleDataByID(bibleUrl.bibleID());
                if(ret == 1) {
                    showText(tr("No such bible found."));
                    return;
                }
            }

            int firstBook = 0;
            if(bibleUrl.book() == BibleUrl::LoadFirstBook) {
                firstBook = m_moduleManager->bible()->bookIDs().first();
            }
            if((bibleUrl.bookID() != m_moduleManager->bible()->bookID() && bibleUrl.book() == BibleUrl::LoadBookByID) ||
                    (firstBook != m_moduleManager->bible()->bookID() && bibleUrl.book() == BibleUrl::LoadFirstBook) ||
                    reloadBible || bibleUrl.getParam("forceReloadBook") == "true") {
                reloadBook = true;
            }

            int firstChapter = 0;
            /*if(bibleUrl.chapter() == BibleUrl::LoadFirstChapter) {
                firstChapter = 0; // todo: change when we support to start chapters from another value
            }*/
            if((bibleUrl.chapterID() != m_moduleManager->bible()->chapterID() && bibleUrl.chapter() == BibleUrl::LoadChapterByID) ||
                    (firstChapter != m_moduleManager->bible()->chapterID() && bibleUrl.chapter() == BibleUrl::LoadFirstChapter) ||
                    reloadBible || reloadBook || bibleUrl.getParam("forceReloadChapter") == "true") {
                reloadChapter = true;
            }

            int firstVerse = 0;
            if(bibleUrl.verse() == BibleUrl::LoadFirstVerse) {
                firstVerse = 0; // todo: change when we support to start verse from another value
            }
            if(((bibleUrl.verseID() != m_moduleManager->bible()->verseID() && bibleUrl.verse() == BibleUrl::LoadVerseByID) ||
                    (firstVerse != m_moduleManager->bible()->verseID() && bibleUrl.verse() == BibleUrl::LoadFirstVerse))
                    && !reloadBible && !reloadBook && !reloadChapter) {
                reloadVerse = true;
            }

            if(reloadBook) {
                int bookID;
                if(bibleUrl.book() == BibleUrl::LoadFirstBook) {
                    bookID = firstBook;
                } else if(bibleUrl.book() == BibleUrl::LoadCurrentBook) {
                    bookID = m_moduleManager->bible()->bookID();
                } else {
                    bookID = bibleUrl.bookID();
                }
                readBookByID(bookID);
                setCurrentBook(bookID);
            }

            int chapterID;
            if(bibleUrl.chapter() == BibleUrl::LoadFirstChapter) {
                chapterID = firstChapter;
            } else if(bibleUrl.chapter() == BibleUrl::LoadCurrentChapter) {
                chapterID = m_moduleManager->bible()->chapterID();
            } else {
                chapterID = bibleUrl.chapterID();
            }

            int verseID;
            if(bibleUrl.verse() == BibleUrl::LoadFirstVerse) {
                verseID = firstVerse;
            } else if(bibleUrl.verse() == BibleUrl::LoadCurrentVerse) {
                verseID = m_moduleManager->bible()->verseID();
            } else {
                verseID = bibleUrl.verseID();
            }

            if(reloadChapter) {
                showChapter(chapterID, verseID);
                setCurrentChapter(chapterID);
            }
            if(reloadVerse) {
                showChapter(chapterID, verseID);
                setCurrentChapter(chapterID);
            }

            if(bibleUrl.hasParam("searchInCurrentText") && bibleUrl.getParam("searchInCurrentText") == "true") {
                searchInText(m_moduleManager->bible()->lastSearchQuery());
            }
            emit historySetUrl(url_backup);
        }
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
        //its a biblequote internal link, but i dont have the specifications!!!
        QStringList internal = url.split(" ");
        const QString bibleID = internal.at(1);//todo: use it
        //myDebug() << "bibleID = " << bibleID;
        int bookID = internal.at(2).toInt() - 1;
        int chapterID = internal.at(3).toInt() - 1;
        int verseID = internal.at(4).toInt();
        /*if(bibleID != m_moduleManager->bible()->bibleID())
        {
            loadModuleDataByID(bibleID);
            readBookByID(bookID);
            setCurrentBook(bookID);
            showChapter(chapterID),verseID);
            setCurrentChapter(chapterID);
            //load bible
        }
        else */if(bookID != m_moduleManager->bible()->bookID()) {
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
        }
        emit historySetUrl(url_backup);

    } else if(url.startsWith(anchor)) {
        url = url.remove(0, anchor.size());
        bool ok;
        int c = url.toInt(&ok, 10);
        if(ok && c < m_moduleManager->bible()->chaptersCount() && m_moduleManager->bible()->bibleType() == Module::BibleQuoteModule && m_moduleManager->bible()->chapterID() != c) {
            showChapter(c, 0);
            setCurrentChapter(c);
        } else {
            if(activeMdiChild()) {
                getView()->page()->mainFrame()->evaluateJavaScript("window.location.hash='" + url + "';");
            }
        }

    } else if(url.startsWith(note)) {
        url = url.remove(0, note.size());
        if(!m_notesDockWidget->isVisible()) {
            m_notesDockWidget->show();
        }
        m_notesDockWidget->showNote(url);
    } else if(url.startsWith(persistent)) {
        url = url.remove(0, persistent.size());
        UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
        urlConverter.setSettings(m_settings);
        urlConverter.setModuleMap(m_moduleManager->m_moduleMap);
        urlConverter.pharse();
        const QString i = urlConverter.convert();//it now a normal interface url
        pharseUrl(i);
    } else {
        //todo: unterstand links like about:blank#a04
        if(m_moduleManager->bible()->bibleType() == Module::BibleQuoteModule) {
            myDebug() << m_moduleManager->bible()->biblePath();
            bool isInBookPath = false;
            int b = 0;
            const QStringList books = m_moduleManager->bible()->bookPath();
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
                emit get("bible://current/" + QString::number(b));
            }
        } else {
            getView()->page()->mainFrame()->evaluateJavaScript(url);
        }
    }
    setEnableReload(true);
    return;
}

QWebView* AdvancedInterface::getView()
{
    if(activeMdiChild()) {
        QWebView *t = activeMdiChild()->widget()->findChild<QWebView *>("webView");
        return t;
    }
    return 0;
}

void AdvancedInterface::setEnableReload(bool enable)
{
    m_enableReload = enable;
}

void AdvancedInterface::zoomIn()
{
    QWebView *v = getView();
    if(v) {
        v->setZoomFactor(v->zoomFactor() + 0.1);
    }
}

void AdvancedInterface::zoomOut()
{
    QWebView *v = getView();
    if(v) {
        v->setZoomFactor(v->zoomFactor() - 0.1);
    }
}

void AdvancedInterface::showText(const QString &text)
{
    QWebView *v = getView();
    if(v) {
        QString cssFile = m_settings->getModuleSettings(m_moduleManager->bible()->moduleID()).styleSheet;
        if(cssFile.isEmpty())
            cssFile = ":/data/css/default.css";

        QFile file(cssFile);
        if(file.open(QFile::ReadOnly))
            v->settings()->setUserStyleSheetUrl(QUrl("data:text/css;charset=utf-8;base64," + file.readAll().toBase64()));

        v->setHtml(text);
        if(m_moduleManager->bible()->verseID() > 1) {
#if QT_VERSION >= 0x040700
            v->page()->mainFrame()->scrollToAnchor("currentVerse");
#else
            v->page()->mainFrame()->evaluateJavaScript("window.location.href = '#currentVerse';");
#endif
            if(m_moduleManager->bibleList()->hasTopBar())
                v->page()->mainFrame()->scroll(0, -40); //due to the biblelist bar on top
        }

        if(m_moduleManager->bible()->bibleType() == Module::BibleQuoteModule) {
            QWebElementCollection collection = v->page()->mainFrame()->documentElement().findAll("img");
            QStringList searchPaths = m_moduleManager->bible()->getSearchPaths();

            foreach(QWebElement paraElement, collection) {
                QString url = paraElement.attribute("src");
                if(url.startsWith(":/") || url.startsWith("http"))
                    continue;
                foreach(const QString & pre, searchPaths) {
                    QFileInfo i(pre + url);
                    if(i.exists()) {
                        paraElement.setAttribute("src", "file://" + pre + url);
                        break;
                    } else {
                        QDir d(pre);
                        QStringList list = d.entryList();
                        foreach(const QString & f, list) {
                            QFileInfo info2(f);
                            if(info2.baseName().compare(i.baseName(), Qt::CaseInsensitive) == 0) {
                                paraElement.setAttribute("src", "file://" + pre + f);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void AdvancedInterface::setTitle(const QString &title)
{
    this->parentWidget()->setWindowTitle(title + " - " + tr("openBibleViewer"));
    if(activeMdiChild()) {
        activeMdiChild()->widget()->setWindowTitle(title);
    }
}

void AdvancedInterface::setChapters(const QStringList &chapters)
{
    m_bookDockWidget->setChapters(chapters);
    if(activeMdiChild()) {
        QComboBox *comboBox_chapters = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_chapters");
        if(comboBox_chapters) {
            bool same = true;
            if(comboBox_chapters->count() == chapters.count()) {
                for(int i = 0; i < chapters.count(); i++) {
                    if(comboBox_chapters->itemText(i) != chapters.at(i)) {
                        same = false;
                    }
                }
            } else {
                same = false;
            }
            if(!same) {
                comboBox_chapters->clear();
                comboBox_chapters->insertItems(0, chapters);
            }
        }
    }
}

void AdvancedInterface::setCurrentChapter(const int &chapterID)
{
    DEBUG_FUNC_NAME
    m_bookDockWidget->setCurrentChapter(chapterID);
    if(activeMdiChild()) {
        QComboBox *comboBox_chapters = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_chapters");
        MdiForm *mForm = activeMdiChild()->widget()->findChild<MdiForm *>("mdiForm");
        disconnect(mForm->m_ui->comboBox_chapters, SIGNAL(activated(int)), mForm, SLOT(readChapter(int)));
        comboBox_chapters->setCurrentIndex(chapterID);
        connect(mForm->m_ui->comboBox_chapters, SIGNAL(activated(int)), mForm, SLOT(readChapter(int)));
    }
}
void AdvancedInterface::clearChapters()
{
    m_bookDockWidget->clearChapters();
    if(activeMdiChild()) {
        QComboBox *comboBox_chapters = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_chapters");
        if(comboBox_chapters)
            comboBox_chapters->clear();
    }
}
void AdvancedInterface::setBooks(const QHash<int, QString> &books, QList<int> ids)
{
    m_bookDockWidget->setBooks(books);
    m_quickJumpDockWidget->setBooks(books.values());
    if(activeMdiChild()) {
        QComboBox *comboBox_books = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_books");
        MdiForm *mForm = activeMdiChild()->widget()->findChild<MdiForm *>("mdiForm");
        if(comboBox_books && mForm) {
            bool same = true;
            QHashIterator<int, QString> i(books);
            int count = 0;
            if(comboBox_books->count() == books.count()) {
                while(i.hasNext()) {
                    i.next();
                    if(comboBox_books->itemText(count) != i.value()) {
                        same = false;
                        break;
                    }
                    count++;
                }
            } else {
                same = false;
            }
            if(!same) {
                comboBox_books->clear();
                comboBox_books->insertItems(0, books.values());
                mForm->m_bookIDs = ids;
            }
        }
    }
}

void AdvancedInterface::setCurrentBook(const int &bookID)
{
    m_bookDockWidget->setCurrentBook(bookID);
    if(activeMdiChild()) {
        QComboBox *comboBox_books = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_books");
        MdiForm *mForm = activeMdiChild()->widget()->findChild<MdiForm *>("mdiForm");
        disconnect(mForm->m_ui->comboBox_books, SIGNAL(activated(int)), mForm, SLOT(readBook(int)));

        if(comboBox_books) {
            comboBox_books->setCurrentIndex(m_moduleManager->bible()->bookIDs().indexOf(bookID));
        }
        connect(mForm->m_ui->comboBox_books, SIGNAL(activated(int)), mForm, SLOT(readBook(int)));

    }
}
void AdvancedInterface::clearBooks()
{
    m_bookDockWidget->clearBooks();
    if(activeMdiChild()) {
        QComboBox *comboBox_books = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_books");
        if(comboBox_books) {
            comboBox_books->clear();
        }
    }
}
void AdvancedInterface::readBook(const int &id)
{
    BibleUrl url;
    url.setBible(BibleUrl::LoadCurrentBible);
    url.setBookID(id);
    url.setChapter(BibleUrl::LoadFirstChapter);
    url.setVerse(BibleUrl::LoadFirstVerse);
    emit get(url.toString());
}

void AdvancedInterface::readBookByID(int id)
{
    QWebView *v = getView();
    if(v) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        if(id < 0) {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(0, tr("Error"), tr("This book is not available."));
            myWarning() << "invalid bookID - 1";
            return;
        }
        if(!m_moduleManager->bible()->bookIDs().contains(id)) {
            myWarning() << "invalid bookID - 2(no book loaded) id = " << id << " count = " << m_moduleManager->bible()->booksCount();
        }
        const int read = m_moduleManager->bibleList()->readBook(id);
        if(read != 0) {
            QApplication::restoreOverrideCursor();
            if(read == 2) {
                clearChapters();
            } else {
                QMessageBox::critical(0, tr("Error"), tr("Cannot read the book."));
            }
            myWarning() << "read = " << read;
            //error while reading
            return;
        }
        QApplication::restoreOverrideCursor();
        setChapters(m_moduleManager->bible()->chapterNames());

    }

}

void AdvancedInterface::readChapter(const int &id)
{
    BibleUrl url;
    url.setBible(BibleUrl::LoadCurrentBible);
    url.setBook(BibleUrl::LoadCurrentBook);
    url.setChapterID(id);
    url.setVerse(BibleUrl::LoadCurrentVerse);
    emit get(url.toString());
}

void AdvancedInterface::showChapter(const int &chapterID, const int &verseID)
{
    m_bibleDisplay->setHtml(m_moduleManager->bibleList()->readChapter(chapterID, verseID));
    setCurrentChapter(chapterID);

}

void AdvancedInterface::nextChapter()
{
    if(m_moduleManager->bible()->chapterID() < m_moduleManager->bible()->chaptersCount() - 1) {
        BibleUrl burl;
        burl.setBible(BibleUrl::LoadCurrentBible);
        burl.setBook(BibleUrl::LoadCurrentBook);
        burl.setChapterID(m_moduleManager->bible()->chapterID() + 1);
        burl.setVerse(BibleUrl::LoadFirstVerse);
        const QString url = burl.toString();
        emit get(url);
    } else if(m_moduleManager->bible()->bookID() < m_moduleManager->bible()->booksCount() - 1) {
        BibleUrl burl;
        burl.setBible(BibleUrl::LoadCurrentBible);
        burl.setBookID(m_moduleManager->bible()->bookID() + 1);
        burl.setChapter(BibleUrl::LoadFirstChapter);
        burl.setVerse(BibleUrl::LoadFirstVerse);
        const QString url = burl.toString();
        emit get(url);
    }
}

void AdvancedInterface::previousChapter()
{
    if(m_moduleManager->bible()->chapterID() > 0) {
        BibleUrl burl;
        burl.setBible(BibleUrl::LoadCurrentBible);
        burl.setBook(BibleUrl::LoadCurrentBook);
        burl.setChapterID(m_moduleManager->bible()->chapterID() - 1);
        burl.setVerse(BibleUrl::LoadFirstVerse);
        const QString url = burl.toString();
        emit get(url);
    } else if(m_moduleManager->bible()->bookID() > 0) {
        BibleUrl burl;
        burl.setBible(BibleUrl::LoadCurrentBible);
        burl.setBookID(m_moduleManager->bible()->bookID() - 1);
        burl.setChapterID(m_moduleManager->bible()->chaptersCount() - 1);//todo: use lastchapter
        burl.setVerse(BibleUrl::LoadFirstVerse);
        const QString url = burl.toString();
        emit get(url);
    }
}

void AdvancedInterface::reloadChapter(bool full)
{
    DEBUG_FUNC_NAME
    if(!activeMdiChild())
        return;
    const QWebView *v = getView();
    const QPoint p = v->page()->mainFrame()->scrollPosition();
    if(full) {
        BibleUrl url;
        url.setBible(BibleUrl::LoadCurrentBible);
        url.setBook(BibleUrl::LoadCurrentBook);
        url.setChapter(BibleUrl::LoadCurrentChapter);
        url.setVerse(BibleUrl::LoadCurrentVerse);
        url.setParam("force", "true");
        emit get(url.toString());
    } else {
        BibleUrl url;
        url.setBible(BibleUrl::LoadCurrentBible);
        url.setBook(BibleUrl::LoadCurrentBook);
        url.setChapter(BibleUrl::LoadCurrentChapter);
        url.setVerse(BibleUrl::LoadCurrentVerse);
        url.setParam("forceReloadChapter", "true");
        emit get(url.toString());
    }
    v->page()->mainFrame()->setScrollPosition(p);
}

void AdvancedInterface::reloadActive()
{
    DEBUG_FUNC_NAME
    setEnableReload(true);
    reloadWindow(ui->mdiArea->currentSubWindow());
    setEnableReload(false);
}

VerseSelection AdvancedInterface::verseSelection()
{
    QWebFrame *f = getView()->page()->mainFrame();
    VerseSelection s;
    if(!f)
        return s;

    f->evaluateJavaScript("var verseSelection = new VerseSelection();verseSelection.getSelection();");
    s.startVerse = f->evaluateJavaScript("verseSelection.startVerse;").toInt();
    s.endVerse = f->evaluateJavaScript("verseSelection.endVerse;").toInt();
    s.moduleID = f->evaluateJavaScript("verseSelection.moduleID;").toInt();
    s.bookID  = f->evaluateJavaScript("verseSelection.bookID;").toInt();
    s.chapterID = f->evaluateJavaScript("verseSelection.chapterID;").toInt();
    Chapter c = m_moduleManager->bible()->rawChapter();

    const QString startVerseText = c.data.at(s.startVerse);
    QString endVerseText;
    if(s.startVerse != s.endVerse)
        endVerseText = c.data.at(s.endVerse);
    else
        endVerseText = startVerseText;

    const QString selectedText = f->evaluateJavaScript("verseSelection.selectedText;").toString();

    myDebug() << "startVerseText = " << startVerseText;
    myDebug() << "endVerseText = " << endVerseText;
    {
        QString sText;
        for(int i = 0; i < selectedText.size() - 1; i++) {
            sText += selectedText.at(i);
            const int pos = startVerseText.indexOf(sText);
            if(pos != -1 && startVerseText.lastIndexOf(sText) == pos) {
                s.shortestStringInStartVerse = sText;
                break;
            }
        }
        if(s.shortestStringInStartVerse.isEmpty() && s.startVerse != s.endVerse) {
            //find the last long string if the selection is over more than one verse long
            QString lastLongest = selectedText;
            int lastPos = -2;
            for(int i = selectedText.size() - 1; i > 0; i--) {
                const int pos = startVerseText.lastIndexOf(lastLongest);
                if(pos != -1) {
                    lastPos = pos;
                    break;
                }
                lastLongest.remove(i, selectedText.size());
            }
            //and shorten it
            sText.clear();
            for(int i = 0; i < selectedText.size() - 1; i++) {
                sText += selectedText.at(i);
                const int pos = startVerseText.lastIndexOf(sText);
                if(pos != -1 && lastPos == pos) {
                    s.shortestStringInStartVerse = sText;
                    break;
                }
            }


        }
        sText.clear();
        for(int i = 0; i < selectedText.size() - 1; i++) {
            sText.prepend(selectedText.at(selectedText.size() - i - 1));
            const int pos = endVerseText.indexOf(sText);
            if(pos != -1 && endVerseText.lastIndexOf(sText) == pos) {
                s.shortestStringInEndVerse = sText;
                break;
            }
        }
        if(s.shortestStringInEndVerse.isEmpty() && s.startVerse != s.endVerse) {
            //find the first longest string if the selection is over more than one verse long
            QString firstLongest = selectedText;
            int firstPos = -2;
            for(int i = 0; i < selectedText.size(); i++) {
                const int pos = endVerseText.indexOf(firstLongest);
                if(pos != -1) {
                    firstPos = pos;
                    break;
                }
                firstLongest.remove(0, 1);
            }
            //and shorten it
            sText.clear();
            for(int i = 0; i < selectedText.size() - 1; i++) {
                sText.prepend(selectedText.at(selectedText.size() - i - 1));
                const int pos = endVerseText.indexOf(sText);
                if(pos != -1 && firstPos == pos) {
                    s.shortestStringInEndVerse = sText;
                    break;
                }
            }
        }
        s.type = VerseSelection::ShortestString;
        if(s.shortestStringInStartVerse.isEmpty() || s.shortestStringInEndVerse.isEmpty()) {
            s.setCanBeUsedForMarks(false);
        } else {
            s.setCanBeUsedForMarks(true);
        }
    }
    myDebug() << s.shortestStringInStartVerse << s.shortestStringInEndVerse;
    //todo: 0.6
    /* if(s.canBeUsedForMarks() == false) {
         //now the ultimative alogrithm
         f->evaluateJavaScript("var adVerseSelection = new AdVerseSelection();adVerseSelection.getSelection();");
         const QString startVerseText2 = f->evaluateJavaScript("adVerseSelection.startVerseText;").toString();

         const QString uniqueString = "!-_OPENBIBLEVIEWER_INSERT_-!";
         int posOfInsert = startVerseText2.lastIndexOf(uniqueString);

         QString back = selectedText;
         QString longestString;
         for(int i = selectedText.size()-1; i > 0; i--) {
             const int pos = startVerseText2.indexOf(back);
             if(pos != -1) {
                 longestString = back;
                 break;
             }
             back.remove(i,selectedText.size());
         }

         int count = 0;
         int currentPos = 0;
         while(true) {
             currentPos = startVerseText2.indexOf(longestString,currentPos+1);
             if(currentPos > posOfInsert || currentPos == -1) {
                 break;
             }
             count++;
         }
         s.type = VerseSelection::RepeatOfLongestString;
         s.repeat = count;
         s.longestString = longestString;
         //s.setCanBeUsedForMarks(true);
         //todo: end
         //myDebug() << longestString << " count = " << count;

     }*/
    return s;
}

void AdvancedInterface::createDefaultMenu()
{
    m_actionCopy = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Copy"), this);
    connect(m_actionCopy, SIGNAL(triggered()), this, SLOT(copy()));

    m_actionSelect = new QAction(QIcon::fromTheme("edit-select-all", QIcon(":/icons/16x16/edit-select-all.png")), tr("Select All"), this);
    connect(m_actionSelect, SIGNAL(triggered()), this , SLOT(selectAll()));

    m_menuMark = new QMenu(this);
    m_menuMark->setTitle(tr("Mark this"));
    m_menuMark->setIcon(QIcon::fromTheme("format-fill-color.png", QIcon(":/icons/16x16/format-fill-color.png")));

    QAction *actionYellowMark = new QAction(QIcon(":/icons/16x16/mark-yellow.png"), tr("Yellow"), m_menuMark);
    actionYellowMark->setObjectName("yellowMark");
    connect(actionYellowMark, SIGNAL(triggered()), this , SLOT(newColorMark()));
    m_menuMark->addAction(actionYellowMark);

    QAction *actionGreenMark = new QAction(QIcon(":/icons/16x16/mark-green.png"), tr("Green"), m_menuMark);
    actionGreenMark->setObjectName("greenMark");
    connect(actionGreenMark, SIGNAL(triggered()), this , SLOT(newColorMark()));
    m_menuMark->addAction(actionGreenMark);

    QAction *actionBlueMark = new QAction(QIcon(":/icons/16x16/mark-blue.png"), tr("Blue"), m_menuMark);
    actionBlueMark->setObjectName("blueMark");
    connect(actionBlueMark, SIGNAL(triggered()), this , SLOT(newColorMark()));
    m_menuMark->addAction(actionBlueMark);

    QAction *actionOrangeMark = new QAction(QIcon(":/icons/16x16/mark-orange.png"), tr("Orange"), m_menuMark);
    actionOrangeMark->setObjectName("orangeMark");
    connect(actionOrangeMark, SIGNAL(triggered()), this , SLOT(newColorMark()));
    m_menuMark->addAction(actionOrangeMark);

    QAction *actionVioletMark = new QAction(QIcon(":/icons/16x16/mark-violet.png"), tr("Violet"), m_menuMark);
    actionVioletMark->setObjectName("violetMark");
    connect(actionVioletMark, SIGNAL(triggered()), this , SLOT(newColorMark()));
    m_menuMark->addAction(actionVioletMark);

    QAction *actionCustomMark  = new QAction(QIcon(":/icons/16x16/format-fill-color.png"), tr("Custom Color"), m_menuMark);
    connect(actionCustomMark, SIGNAL(triggered()), this , SLOT(newCustomColorMark()));
    m_menuMark->addAction(actionCustomMark);
    m_menuMark->addSeparator();

    QAction *actionBoldMark  = new QAction(QIcon::fromTheme("format-text-bold", QIcon(":/icons/16x16/format-text-bold.png")), tr("Bold"), m_menuMark);
    connect(actionBoldMark, SIGNAL(triggered()), this , SLOT(newBoldMark()));
    m_menuMark->addAction(actionBoldMark);

    QAction *actionItalicMark  = new QAction(QIcon::fromTheme("format-text-italic", QIcon(":/icons/16x16/format-text-italic.png")), tr("Italic"), m_menuMark);
    connect(actionItalicMark, SIGNAL(triggered()), this , SLOT(newItalicMark()));
    m_menuMark->addAction(actionItalicMark);

    QAction *actionUnderlineMark  = new QAction(QIcon::fromTheme("format-text-underline", QIcon(":/icons/16x16/format-text-underline.png")), tr("Underline"), m_menuMark);
    connect(actionUnderlineMark, SIGNAL(triggered()), this , SLOT(newUnderlineMark()));
    m_menuMark->addAction(actionUnderlineMark);


    m_actionRemoveMark = new QAction(QIcon(":/icons/16x16/mark-yellow.png"), tr("Remove Mark"), this);
    connect(m_actionRemoveMark, SIGNAL(triggered()), this , SLOT(removeMark()));

    m_actionBookmark = new QAction(QIcon::fromTheme("bookmark-new", QIcon(":/icons/16x16/bookmark-new.png")), tr("Add Bookmark"), this);
    connect(m_actionBookmark, SIGNAL(triggered()), this , SLOT(newBookmark()));

    m_actionNote = new QAction(QIcon::fromTheme("view-pim-notes", QIcon(":/icons/16x16/view-pim-notes.png")), tr("Add Note"), this);
    connect(m_actionNote, SIGNAL(triggered()), this , SLOT(newNoteWithLink()));
}

void AdvancedInterface::showContextMenu(QContextMenuEvent* ev)
{
    QMenu *contextMenu = new QMenu(this);

    QAction *actionCopyWholeVerse = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Copy Verse"), contextMenu);
    VerseSelection selection = verseSelection();
    if(selection.startVerse != -1) {
        QString addText;
        if(selection.startVerse != selection.endVerse)
            addText = " " + QString::number(selection.startVerse + 1) + " - " + QString::number(selection.endVerse + 1);
        else
            addText = " " + QString::number(selection.startVerse + 1);
        if(selection.startVerse < 0 || selection.endVerse < 0) {
            actionCopyWholeVerse->setText(tr("Copy Verse"));
            actionCopyWholeVerse->setEnabled(false);
        } else {
            actionCopyWholeVerse->setText(tr("Copy Verse %1").arg(addText));
            actionCopyWholeVerse->setEnabled(true);

            connect(actionCopyWholeVerse, SIGNAL(triggered()), this , SLOT(copyWholeVerse()));
        }
    } else {
        actionCopyWholeVerse->setEnabled(false);
    }

    QAction *dbg = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Debugger"), contextMenu);
    connect(dbg, SIGNAL(triggered()), this, SLOT(debugger()));

    contextMenu->addAction(m_actionCopy);
    contextMenu->addAction(actionCopyWholeVerse);
    contextMenu->addAction(m_actionSelect);
    contextMenu->addSeparator();
    contextMenu->addMenu(m_menuMark);
    contextMenu->addAction(m_actionRemoveMark);
    contextMenu->addSeparator();
    contextMenu->addAction(m_actionBookmark);
    contextMenu->addAction(m_actionNote);
    contextMenu->addAction(dbg);
    contextMenu->exec(ev->globalPos());

}

int AdvancedInterface::copyWholeVerse(void)
{
    if(!activeMdiChild())
        return 1;
    VerseSelection selection = verseSelection();
    if(selection.startVerse != -1) {
        QString sverse;
        if(selection.startVerse == selection.endVerse) {
            sverse = "," + QString::number(selection.startVerse + 1);
        } else {
            sverse = "," + QString::number(selection.startVerse + 1) + "-" + QString::number(selection.endVerse + 1);
        }

        int add = 0;
        if(m_moduleManager->bible()->bibleType() == Module::BibleQuoteModule)
            add = 1; //because of the title
        QString stext = m_moduleManager->bible()->readVerse(m_moduleManager->bible()->chapterID(), selection.startVerse + add, selection.endVerse + 1 + add, -1, false);
        QTextDocument doc2;
        doc2.setHtml(stext);
        stext = doc2.toPlainText();

        const QString curChapter = QString::number(m_moduleManager->bible()->chapterID() + 1);

        const QString newText = m_moduleManager->bible()->bookName(m_moduleManager->bible()->bookID()) + " " + curChapter + sverse + "\n" + stext;
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(newText);

    }
    return 0;
}

void AdvancedInterface::debugger()
{
    getView()->page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    QWebInspector *inspector = new QWebInspector;
    inspector->setPage(getView()->page());
    inspector->showNormal();
}

void AdvancedInterface::newColorMark()
{
    if(!m_moduleManager->bibleLoaded()) {
        return;
    }
    if(!activeMdiChild())
        return;
    const QString colorName = sender()->objectName();
    QColor color;
    if(colorName == "yellowMark") {
        color = QColor(255, 255, 0);
    } else if(colorName == "greenMark") {
        color = QColor(146, 243, 54);
    } else if(colorName == "blueMark") {
        color = QColor(77, 169, 243);
    } else if(colorName == "orangeMark") {
        color = QColor(243, 181, 57);
    } else if(colorName == "violetMark") {
        color = QColor(169, 102, 240);
    } else {
        color = QColor(255, 255, 0);
    }

    VerseSelection selection = verseSelection();
    m_notesDockWidget->newMark(selection, color);
}

void AdvancedInterface::newCustomColorMark()
{
    if(!m_moduleManager->bibleLoaded()) {
        return;
    }
    if(!activeMdiChild())
        return;
    VerseSelection selection = verseSelection();
    const QColor color = QColorDialog::getColor(QColor(255, 255, 0), this);
    if(color.isValid()) {
        m_notesDockWidget->newMark(selection, color);
    }

}

void AdvancedInterface::newBoldMark()
{
    if(!m_moduleManager->bibleLoaded()) {
        return;
    }
    if(!activeMdiChild())
        return;
    VerseSelection selection = verseSelection();
    m_notesDockWidget->newStyleMark(selection, "font-weight: bold;");

}

void AdvancedInterface::newItalicMark()
{
    if(!m_moduleManager->bibleLoaded()) {
        return;
    }
    if(!activeMdiChild())
        return;
    VerseSelection selection = verseSelection();
    m_notesDockWidget->newStyleMark(selection, "font-style: italic;");

}
void AdvancedInterface::newUnderlineMark()
{
    if(!m_moduleManager->bibleLoaded()) {
        return;
    }
    if(!activeMdiChild())
        return;
    VerseSelection selection = verseSelection();
    m_notesDockWidget->newStyleMark(selection, "text-decoration:underline;");

}

void AdvancedInterface::removeMark()
{
    if(!m_moduleManager->bibleLoaded() && !activeMdiChild()) {
        return;
    }
    VerseSelection selection = verseSelection();
    m_notesDockWidget->removeMark(selection);
}

void AdvancedInterface::closing()
{
    m_notesDockWidget->saveNote();
    m_bookmarksDockWidget->saveBookmarks();

    QStringList windowUrls;
    QList<QVariant> windowGeo;
    QList<QVariant> scrollPos;
    QList<QVariant> zoom;
    const int current = m_windowCache.currentWindowID();
    for(int i = 0; i < ui->mdiArea->subWindowList().count(); i++) {
        //todo:
        m_windowCache.setCurrentWindowID(i);
        BibleList *list = m_windowCache.getBibleList();
        QString u = "";
        if(m_windowCache.getBibleList() != 0) {
            QHashIterator<int, Bible *> i(list->m_bibles);
            while(i.hasNext()) {
                i.next();
                Bible *b = i.value();
                if(b != NULL && b->moduleID() >= 0) {
                    UrlConverter urlConverter(UrlConverter::None, UrlConverter::PersistentUrl, "");
                    urlConverter.setModuleMap(m_moduleManager->m_moduleMap);
                    urlConverter.setSettings(m_settings);
                    urlConverter.m_moduleID = b->moduleID();
                    urlConverter.m_bookID = b->bookID();
                    urlConverter.m_chapterID = b->chapterID();
                    urlConverter.m_verseID = 0;
                    const QString url = urlConverter.convert();
                    const QPoint point = list->m_biblePoints.value(i.key());
                    if(u.isEmpty())
                        u += QString::number(point.x()) + ":" + QString::number(point.y()) + ":" + url;
                    else
                        u += "|" + QString::number(point.x()) + ":" + QString::number(point.y()) + ":" + url;
                }
            }
        }
        windowUrls << u;
        QWebView *v = getView();
        windowGeo << ui->mdiArea->subWindowList().at(i)->geometry();
        scrollPos << v->page()->mainFrame()->scrollPosition();
        zoom << v->zoomFactor();
    }
    m_settings->session.setData("windowUrls", windowUrls);
    m_settings->session.setData("windowGeo", windowGeo);
    m_settings->session.setData("scrollPos", scrollPos);
    m_settings->session.setData("zoom", zoom);
    m_settings->session.setData("viewMode", ui->mdiArea->viewMode());
    m_settings->session.setData("windowID", current);
}

void AdvancedInterface::restoreSession()
{
    DEBUG_FUNC_NAME
    const QStringList windowUrls = m_settings->session.getData("windowUrls").toStringList();
    const QVariantList windowGeo = m_settings->session.getData("windowGeo").toList();
    const QVariantList scrollPos = m_settings->session.getData("scrollPos").toList();
    const QVariantList zoom = m_settings->session.getData("zoom").toList();

    for(int i = 0; i < windowUrls.size(); ++i) {
        newSubWindow(true);
        //load bible
        QString url = windowUrls.at(i);
        m_moduleManager->bibleList()->clear();
        if(!url.isEmpty() && url.size() != 0) {
            QStringList list = url.split("|");
            foreach(QString part, list) {
                QStringList a = part.split(":");
                int x = a.at(0).toInt();
                int y = a.at(1).toInt();
                QString u = a.at(2);
                UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, u);
                urlConverter.setSettings(m_settings);
                urlConverter.setModuleMap(m_moduleManager->m_moduleMap);
                urlConverter.pharse();
                m_moduleManager->newBible(urlConverter.m_moduleID, QPoint(x, y));
                pharseUrl(urlConverter.convert() + ",force=true"); //todo: MOVE IT OUT
            }
        } else {
            Bible *b = new Bible();
            m_moduleManager->initBible(b);
            m_moduleManager->bibleList()->addBible(b, QPoint(0, 0));
        }
        //set geometry
        myDebug() << "new Window " << i << " url = " << url << "rect = " << windowGeo.at(i).toRect();
        QWebView *v = getView();
        activeMdiChild()->setGeometry(windowGeo.at(i).toRect());
        v->page()->mainFrame()->setScrollPosition(scrollPos.at(i).toPoint());
        if(zoom.size() != 0 && i < zoom.size() && zoom.at(i).toReal() > 0)
            v->setZoomFactor(zoom.at(i).toReal());
    }
    const int viewMode = m_settings->session.getData("viewMode").toInt();
    if(viewMode == 0)
        setSubWindowView();
    else
        setTabView();

    const int id = m_settings->session.getData("windowID", -1).toInt();
    //myDebug() << id << ui->mdiArea->subWindowList();
    if(id < ui->mdiArea->subWindowList().size() && id > 0) {
        ui->mdiArea->setActiveSubWindow(ui->mdiArea->subWindowList().at(id));
    }
}

void AdvancedInterface::settingsChanged(Settings oldSettings, Settings newSettings)
{
    //DEBUG_FUNC_NAME
    //reload books
    bool reloadBibles = false;
    if(oldSettings.encoding != newSettings.encoding) {
        reloadBibles = true;
    }
    if(oldSettings.m_moduleSettings.size() > newSettings.m_moduleSettings.size())
        reloadBibles = true;
    for(int i = 0; i < newSettings.m_moduleSettings.size(); ++i) {
        if(oldSettings.m_moduleSettings.size() < i || oldSettings.m_moduleSettings.empty()) {
            reloadBibles = true;
            break;
        } else {
            ModuleSettings m1, m2;
            m1 = newSettings.m_moduleSettings.at(i);
            m2 = oldSettings.m_moduleSettings.at(i);
            if(memcmp(&m1, &m2, sizeof(ModuleSettings))) {
                reloadBibles = true;
                break;
            }
        }
    }
    if(reloadBibles == true) {
        FastStart fastStart;
        fastStart.setSettings(m_settings);
        fastStart.remove();
        //myDebug() << "reload Module";
        m_moduleManager->loadAllModules();
        m_moduleDockWidget->init();
        m_dictionaryDockWidget->init();
        showText("");
        //m_windowCache.clearZefCache();
        //m_moduleManager->bible()->clearSoftCache();
        if(m_moduleManager->bibleLoaded())
            reloadChapter(true);
    }

}

void AdvancedInterface::showSearchDialog()
{
    SearchDialog *sDialog = new SearchDialog(this);
    connect(sDialog, SIGNAL(searched(SearchQuery)), this, SLOT(search(SearchQuery)));
    const QString text = getView()->selectedText();
    if(!text.isEmpty()) {
        sDialog->setText(text);
    }
    sDialog->show();
    sDialog->exec();
}
void AdvancedInterface::search()
{
    DEBUG_FUNC_NAME
    SearchQuery query;
    query.searchText = ((QLineEdit *) sender())->text();
    query.searchInNotes = true;
    query.queryType = SearchQuery::Simple;
    search(query);
}

void AdvancedInterface::search(SearchQuery query)
{
    DEBUG_FUNC_NAME
    m_advancedSearchResultDockWidget->show();
    Search s;
    setAll(&s);
    SearchResult *res = s.search(query);
    m_advancedSearchResultDockWidget->setSearchResult(*res);
}

void AdvancedInterface::searchInText(SearchQuery query)
{
    DEBUG_FUNC_NAME
    if(query.queryType == SearchQuery::Simple) {
        QString s = query.searchText;
        //todo hacky
        s.remove('*');
        s.remove('?');
        getView()->findText(s, QWebPage::HighlightAllOccurrences);
    }
}

void AdvancedInterface::copy()
{
    getView()->page()->triggerAction(QWebPage::Copy);
}

void AdvancedInterface::selectAll()
{
    getView()->page()->triggerAction(QWebPage::SelectAll);
}

void AdvancedInterface::nextVerse()
{
    m_advancedSearchResultDockWidget->nextVerse();
}

void AdvancedInterface::previousVerse()
{
    m_advancedSearchResultDockWidget->previousVerse();
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
    connect(actionNewSubWindow, SIGNAL(triggered()), this, SLOT(newSubWindow()));
    actionNewSubWindow->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));//using const KeySequence on KDE will be Ctrl+Shift+N

    //Close Sub Window
    QAction *actionCloseSubWindow = new QAction(QIcon::fromTheme("tab-close", QIcon(":/icons/16x16/tab-close.png")), tr("Close SubWindow"), menuFile);
    connect(actionCloseSubWindow, SIGNAL(triggered()), this, SLOT(closeSubWindow()));
    actionCloseSubWindow->setShortcut(QKeySequence::Close);

    //Save As
    QAction *actionSaveAs = new QAction(QIcon::fromTheme("document-save-as", QIcon(":/icons/16x16/document-save-as.png")), tr("Save As"), menuFile);
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(saveFile()));
    actionSaveAs->setShortcut(QKeySequence::SaveAs);

    //Print
    QAction *actionPrint = new QAction(QIcon::fromTheme("document-print", QIcon(":/icons/16x16/document-print.png")), tr("Print"), menuFile);
    connect(actionPrint, SIGNAL(triggered()), this, SLOT(printFile()));
    actionPrint->setShortcut(QKeySequence::Print);

    //Close
    QAction *actionClose = new QAction(QIcon::fromTheme("application-exit", QIcon(":/icons/16x16/application-exit.png")), tr("Quit"), menuFile);
    connect(actionClose, SIGNAL(triggered()), this->parentWidget(), SLOT(close()));
    actionClose->setShortcut(QKeySequence::Quit);

    menuFile->addAction(actionNewSubWindow);
    menuFile->addAction(actionCloseSubWindow);
    menuFile->addSeparator();
    menuFile->addAction(actionSaveAs);
    menuFile->addAction(actionPrint);
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
    connect(actionSearch, SIGNAL(triggered()), this, SLOT(showSearchDialog()));

    //Find Next
    QAction *actionFindNext = new QAction(QIcon::fromTheme("go-down-search", QIcon(":/icons/16x16/go-down-search.png")), tr("Find Next"), menuEdit);
    connect(actionFindNext, SIGNAL(triggered()), this, SLOT(nextVerse()));
    actionFindNext->setShortcut(QKeySequence::FindNext);

    //Find Previous
    QAction *actionFindPrevious = new QAction(QIcon::fromTheme("go-up-search", QIcon(":/icons/16x16/go-up-search.png")), tr("Find Previous"), menuEdit);
    connect(actionFindPrevious, SIGNAL(triggered()), this, SLOT(previousVerse()));
    actionFindPrevious->setShortcut(QKeySequence::FindPrevious);

    //Next Chapter
    QAction *actionNextChapter = new QAction(QIcon(""), tr("Next Chapter"), menuEdit);
    connect(actionNextChapter, SIGNAL(triggered()), this, SLOT(nextChapter()));
    actionNextChapter->setShortcut(QKeySequence::MoveToNextPage);

    //Prev Chapter
    QAction *actionPrevChapter = new QAction(QIcon(""), tr("Previous Chapter"), menuEdit);
    connect(actionPrevChapter, SIGNAL(triggered()), this, SLOT(previousChapter()));
    actionPrevChapter->setShortcut(QKeySequence::MoveToPreviousPage);

    //Config
    QAction *actionConfiguration = new QAction(QIcon::fromTheme("configure", QIcon(":/icons/16x16/configure.png")), tr("Configuration"), menuEdit);
    connect(actionConfiguration, SIGNAL(triggered()), this->parent(), SLOT(showSettingsDialog_General()));
    actionConfiguration->setShortcut(QKeySequence::Preferences);

    menuEdit->addAction(actionCopy);
    menuEdit->addAction(actionSelectAll);
    menuEdit->addSeparator();
    menuEdit->addAction(actionSearch);
    menuEdit->addAction(actionFindNext);
    menuEdit->addAction(actionFindPrevious);
    menuEdit->addSeparator();
    menuEdit->addAction(actionNextChapter);
    menuEdit->addAction(actionPrevChapter);
    menuEdit->addSeparator();
    menuEdit->addAction(actionConfiguration);

    QMenu *menuView = new QMenu(tr("View"), bar);

    //Zoom In
    QAction *actionZoomIn = new QAction(QIcon::fromTheme("zoom-in", QIcon(":/icons/16x16/zoom-in.png")), tr("Zoom In"), menuView);
    connect(actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
    actionZoomIn->setShortcut(QKeySequence::ZoomIn);

    //Zoom Out
    QAction *actionZoomOut = new QAction(QIcon::fromTheme("zoom-out", QIcon(":/icons/16x16/zoom-out.png")), tr("Zoom Out"), menuView);
    actionZoomOut->setShortcut(QKeySequence::ZoomOut);
    connect(actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));

    //TabView
    m_actionTabView = new QAction(QIcon(), tr("Tabbed View"), menuView);
    m_actionTabView->setCheckable(true);
    connect(m_actionTabView, SIGNAL(triggered()), this, SLOT(setTabView()));

    //SubWindowView
    m_actionSubWindowView = new QAction(QIcon(), tr("Sub Window View"), menuView);
    m_actionSubWindowView->setCheckable(true);
    connect(m_actionSubWindowView, SIGNAL(triggered()), this, SLOT(setSubWindowView()));

    //Cascade
    QAction *actionCascade = new QAction(QIcon(":/icons/svg/cascade.svg"), tr("Cascade"), menuView);
    connect(actionCascade, SIGNAL(triggered()), this, SLOT(myCascade()));

    //Tile
    QAction *actionTile = new QAction(QIcon(":/icons/svg/tile.svg"), tr("Tile"), menuView);
    connect(actionTile, SIGNAL(triggered()), this, SLOT(myTile()));

    //Tile Vertical
    QAction *actionTileVertical = new QAction(QIcon(":/icons/svg/tile_vert.svg"), tr("Tile Vertical"), menuView);
    connect(actionTileVertical, SIGNAL(triggered()), this, SLOT(myTileVertical()));

    //Tile Horizontal
    QAction *actionTileHorizontal = new QAction(QIcon(":/icons/svg/tile_horiz.svg"), tr("Tile Horizontal"), menuView);
    connect(actionTileHorizontal, SIGNAL(triggered()), this, SLOT(myTileHorizontal()));



    menuView->addAction(actionZoomIn);
    menuView->addAction(actionZoomOut);
    menuView->addSeparator();
    menuView->addAction(m_actionTabView);
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
    connect(m_moduleDockWidget, SIGNAL(visibilityChanged(bool)), actionModule, SLOT(setChecked(bool)));
    connect(actionModule, SIGNAL(triggered(bool)), m_moduleDockWidget, SLOT(setVisible(bool)));
    actionModule->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_M));

    QAction *actionBooks = new QAction(tr("Books"), menuDocks);
    actionBooks->setCheckable(true);
    connect(m_bookDockWidget, SIGNAL(visibilityChanged(bool)), actionBooks, SLOT(setChecked(bool)));
    connect(actionBooks, SIGNAL(triggered(bool)), m_bookDockWidget, SLOT(setVisible(bool)));
    actionBooks->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C));

    QAction *actionNotes = new QAction(QIcon::fromTheme("notes-edit", QIcon(":/icons/16x16/notes-edit.png")), tr("Notes"), menuDocks);
    actionNotes->setCheckable(true);
    connect(m_notesDockWidget, SIGNAL(visibilityChanged(bool)), actionNotes, SLOT(setChecked(bool)));
    connect(actionNotes, SIGNAL(triggered(bool)), m_notesDockWidget, SLOT(setVisible(bool)));
    actionNotes->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));

    QAction *actionStrong = new QAction(tr("Dictionay"), menuDocks);
    actionStrong->setCheckable(true);
    connect(m_dictionaryDockWidget, SIGNAL(visibilityChanged(bool)), actionStrong, SLOT(setChecked(bool)));
    connect(actionStrong, SIGNAL(triggered(bool)), m_dictionaryDockWidget, SLOT(setVisible(bool)));
    actionStrong->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_D));

    QAction *actionBookmarks = new QAction(QIcon::fromTheme("bookmarks-organize", QIcon(":/icons/16x16/bookmarks-organize.png")), tr("Bookmarks"), menuDocks);
    actionBookmarks->setCheckable(true);
    connect(m_bookmarksDockWidget, SIGNAL(visibilityChanged(bool)), actionBookmarks, SLOT(setChecked(bool)));
    connect(actionBookmarks, SIGNAL(triggered(bool)), m_bookmarksDockWidget, SLOT(setVisible(bool)));
    actionBookmarks->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_B));

    QAction *actionQuickJump = new QAction(tr("Quick Jump"), menuDocks);
    actionQuickJump->setCheckable(true);
    connect(m_quickJumpDockWidget, SIGNAL(visibilityChanged(bool)), actionQuickJump, SLOT(setChecked(bool)));
    connect(actionQuickJump, SIGNAL(triggered(bool)), m_quickJumpDockWidget, SLOT(setVisible(bool)));
    actionQuickJump->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Q));

    QAction *actionSearchResults = new QAction(QIcon::fromTheme("table", QIcon(":/icons/16x16/table.png")), tr("Search Results"), menuDocks);
    actionSearchResults->setCheckable(true);
    connect(m_advancedSearchResultDockWidget, SIGNAL(visibilityChanged(bool)), actionSearchResults, SLOT(setChecked(bool)));
    connect(actionSearchResults , SIGNAL(triggered(bool)), m_advancedSearchResultDockWidget, SLOT(setVisible(bool)));
    actionSearchResults->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));


    menuDocks->addAction(actionModule);
    menuDocks->addAction(actionBooks);
    menuDocks->addAction(actionNotes);
    menuDocks->addAction(actionBookmarks);
    menuDocks->addAction(actionQuickJump);
    menuDocks->addAction(actionStrong);
    menuDocks->addAction(actionSearchResults);

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
    bar->addMenu(menuDocks);
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
    connect(m_mainBarActionSearch, SIGNAL(triggered()), this, SLOT(showSearchDialog()));

    m_mainBarActionBookmarks = new QAction(QIcon::fromTheme("bookmarks-organize", QIcon(":/icons/16x16/bookmarks-organize.png")), tr("Bookmarks"), m_mainBar);
    connect(m_mainBarActionBookmarks, SIGNAL(triggered()), this, SLOT(showBookmarksDock()));
    m_mainBarActionBookmarks->setCheckable(true);

    m_mainBarActionNotes = new QAction(QIcon::fromTheme("view-pim-notes", QIcon(":/icons/16x16/view-pim-notes.png")), tr("Notes"), m_mainBar);
    connect(m_mainBarActionNotes, SIGNAL(triggered()), this, SLOT(showNotesDock()));
    m_mainBarActionNotes->setCheckable(true);


    m_mainBarActionNewWindow = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("New Window"), m_mainBar);
    connect(m_mainBarActionNewWindow, SIGNAL(triggered()), this, SLOT(newSubWindow()));

    m_mainBarActionZoomIn = new QAction(QIcon::fromTheme("zoom-in", QIcon(":/icons/16x16/zoom-in.png")), tr("Zoom In"), m_mainBar);
    connect(m_mainBarActionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));

    m_mainBarActionZoomOut = new QAction(QIcon::fromTheme("zoom-out", QIcon(":/icons/16x16/zoom-out.png")), tr("Zoom Out"), m_mainBar);
    connect(m_mainBarActionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));

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

    connect(edit, SIGNAL(returnPressed()), this, SLOT(search()));
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
    if(m_bookmarksDockWidget->isVisible()) {
        m_bookmarksDockWidget->hide();
    } else {
        m_bookmarksDockWidget->show();
    }
}

void AdvancedInterface::showNotesDock()
{
    if(m_notesDockWidget->isVisible()) {
        m_notesDockWidget->hide();
    } else {
        m_notesDockWidget->show();
    }
}

void AdvancedInterface::newBookmark()
{
    if(!m_moduleManager->bibleLoaded() && !activeMdiChild())
        return;
    if(m_bookmarksDockWidget->isHidden())
        m_bookmarksDockWidget->show();
    m_bookmarksDockWidget->newBookmark(verseSelection());
}

void AdvancedInterface::newNoteWithLink()
{
    if(!m_moduleManager->bibleLoaded() && !activeMdiChild())
        return;
    if(m_notesDockWidget->isHidden())
        m_notesDockWidget->show();
    m_notesDockWidget->newNoteWithLink(verseSelection());
}

void AdvancedInterface::onlineHelp()
{
    //open the online faq
    QDesktopServices::openUrl(tr("http://openbv.uucyc.name/faq.html"));
}

int AdvancedInterface::printFile(void)
{
    //DEBUG_FUNC_NAME
    QPrinter printer;
    QScopedPointer<QPrintDialog> dialog(new QPrintDialog(&printer, this));
    dialog->setWindowTitle(tr("Print"));
    if(dialog->exec() != QDialog::Accepted)
        return 1;
    if(activeMdiChild()) {
        getView()->print(&printer);
    }
    return 0;
}

int AdvancedInterface::saveFile(void)
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    const QString lastPlace = m_settings->session.getData("lastSaveFilePlace").toString();
    const QString fileName = dialog.getSaveFileName(this, tr("Save output"), lastPlace, tr("Html (*.html *.htm);;PDF (*.pdf);;Plain (*.txt)"));

    if(activeMdiChild()) {
        QWebView *v = getView();
        QFileInfo fi(fileName);
        m_settings->session.setData("lastSaveFilePlace", fi.path());
        if(fi.suffix().compare("html", Qt::CaseInsensitive) == 0 ||
                fi.suffix().compare("htm", Qt::CaseInsensitive) == 0) {
            QFile file(fileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return 1;
            QTextStream out(&file);
            out << v->page()->mainFrame()->toHtml();
            file.close();
        } else if(fi.suffix().compare("pdf", Qt::CaseInsensitive) == 0) {
            QPrinter printer;
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(fileName);
            v->print(&printer);

        } else {
            QFile file(fileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return 1;
            QTextStream out(&file);
            out << v->page()->mainFrame()->toPlainText();
            file.close();
        }
    }
    return 0;
}

int AdvancedInterface::showAboutDialog(void)
{
    AboutDialog aDialog;
    aDialog.setWindowTitle(tr("About openBibleViewer"));
    aDialog.show();
    aDialog.setText(tr("openBibleViewer <br> version: %1 build: %2<br> <a href=\"http://openbv.uucyc.name/\"> Official Website</a> | <a href=\"http://openbv.uucyc.name/bug/\">Bug report</a>")
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

void AdvancedInterface::setTabView()
{
    ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    m_actionTabView->setChecked(true);
    m_actionSubWindowView->setChecked(false);
}

void AdvancedInterface::setSubWindowView()
{
    ui->mdiArea->setViewMode(QMdiArea::SubWindowView);
    m_actionTabView->setChecked(false);
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
        m_mainBarActionNewWindow->setText( tr("New Window"));
        m_mainBarActionZoomIn->setText(tr("Zoom In"));
        m_mainBarActionZoomOut->setText( tr("Zoom Out"));
        m_mainBarActionModule->setText(tr("Module"));
        m_searchBar->setWindowTitle(tr("Search Bar"));

        break;
    default:
        break;
    }
}
