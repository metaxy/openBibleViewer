#include "windowmanager.h"
#include "src/core/dbghelper.h"
WindowManager::WindowManager(QObject *parent) :
    QObject(parent)
{
}
void WindowManager::setMdiArea(QMdiArea *area)
{
    m_area = area;
}
void WindowManager::init()
{
    connect(m_area, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(reloadWindow(QMdiSubWindow *)));
}

void WindowManager::newSubWindow(bool doAutoLayout)
{
    DEBUG_FUNC_NAME;
    m_enableReload = false;
    int windowsCount = usableWindowList().size();
    QMdiSubWindow *firstSubWindow = new QMdiSubWindow();
    if(windowsCount == 1) {
        firstSubWindow = usableWindowList().at(0);
    }

    const int windowName = m_windowCache.newWindow();

    QWidget *widget = new QWidget(m_area);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    MdiForm *mForm = new MdiForm(widget);
    mForm->setObjectName("mdiForm");
    setAll(mForm);
    layout->addWidget(mForm);

    widget->setLayout(layout);
    QMdiSubWindow *subWindow = m_area->addSubWindow(widget);
    subWindow->setObjectName(QString::number(windowName));
    subWindow->setWindowIcon(QIcon(":/icons/16x16/main.png"));
    subWindow->setWindowOpacity(1.0);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();
    m_area->setActiveSubWindow(subWindow);
    attachApi();

    m_moduleManager->m_bibleList = new BibleList();
    Bible *b = new Bible();
    m_moduleManager->initBible(b);
    m_moduleManager->bibleList()->addBible(b, QPoint(0, 0));
    m_windowCache.setBibleList(m_moduleManager->m_bibleList);


    if(m_area->viewMode() == QMdiArea::SubWindowView) {
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
    if(doAutoLayout && m_area->viewMode() == QMdiArea::SubWindowView) {
        autoLayout();
    }

    //clear old stuff
    clearBooks();
    clearChapters();
}

void WindowManager::autoLayout()
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

QMdiSubWindow * WindowManager::activeMdiChild()
{
    QList<QMdiSubWindow*> list = usableWindowList();
    if(QMdiSubWindow *activeSubWindow = m_area->activeSubWindow()) {
        for(int i = 0; i < list.size(); i++) {
            if(list.at(i) == activeSubWindow) {
                m_lastActiveWindow = i;
            }
        }
        return activeSubWindow;
    } else if(m_lastActiveWindow < list.size() && m_lastActiveWindow >= 0) {
        m_area->setActiveSubWindow(usableWindowList().at(m_lastActiveWindow));
        if(QMdiSubWindow *activeSubWindow = m_area->activeSubWindow())
            return activeSubWindow;
    } else if(usableWindowList().count() > 0) {
        m_area->setActiveSubWindow(usableWindowList().at(0));
        if(QMdiSubWindow *activeSubWindow = m_area->activeSubWindow())
            return activeSubWindow;
    }
    return 0;
}

void WindowManager::myTileVertical()
{
    if(!m_enableReload || !usableWindowList().count()) {
        return;
    }
    QList<QMdiSubWindow*> windows = usableWindowList();
    m_enableReload = false;
    QMdiSubWindow* active = m_area->activeSubWindow();

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

void WindowManager::myTileHorizontal()
{
    if(!m_enableReload || !usableWindowList().count()) {
        return;
    }
    QList<QMdiSubWindow*> windows = usableWindowList();
    setEnableReload(false);
    QMdiSubWindow* active = m_area->activeSubWindow();

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

void WindowManager::myCascade()
{
    if(!m_enableReload || !usableWindowList().count()) {
        return;
    }

    QList<QMdiSubWindow*> windows = usableWindowList();

    if(m_area->activeSubWindow() && m_area->activeSubWindow()->isMaximized()) {
        if(m_area->activeSubWindow()->size() != this->size()) {
            m_area->activeSubWindow()->resize(this->size());
        }
    } else if(windows.count() == 1) {
        windows.at(0)->showMaximized();
    } else {

        QMdiSubWindow* active = m_area->activeSubWindow();
        setEnableReload(false);
        const unsigned int offsetX = 40;
        const unsigned int offsetY = 40;
        const unsigned int windowWidth =  m_area->width() - (windows.count() - 1) * offsetX;
        const unsigned int windowHeight = m_area->height() - (windows.count() - 1) * offsetY;
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

void WindowManager::myTile()
{
    if(!m_enableReload || !usableWindowList().count()) {
        return;
    }
    m_area->tileSubWindows();
}

QList<QMdiSubWindow*> WindowManager::usableWindowList()
{
    QList<QMdiSubWindow*> ret;
    foreach(QMdiSubWindow * w, m_area->subWindowList()) {
        if(w->isHidden())
            continue;
        ret.append(w);
    }
    return  ret;
}

int WindowManager::currentWindowName()
{
    if(m_area->activeSubWindow() != NULL) {
        return m_area->activeSubWindow()->objectName().toInt();
    }
    return -1;
}


void WindowManager::closeSubWindow()
{
    activeMdiChild()->close();
}

int WindowManager::closingWindow()
{
    DEBUG_FUNC_NAME
    // myDebug() << "enable reload = " << m_enableReload << "subWIndowList = " << m_area->subWindowList() << "internalWindow " << m_internalWindows;
    if(m_area->subWindowList().isEmpty()) {
        myDebug() << "subWindowList is empty";
        clearBooks();
        clearChapters();
        m_windowCache.clearAll();
        return 1;
    }
    //if one in the internal subwindow list list is missing that window was closed
    QList<int> nameList = m_windowCache.nameList();
    QList<int> deletedList = nameList;
    foreach(QMdiSubWindow * win, m_area->subWindowList()) {
        if(nameList.contains(win->objectName().toInt())) {
            deletedList.removeOne(win->objectName().toInt());
        }

    }
    foreach(const int & name, deletedList) {
        m_windowCache.removeWindow(name);
    }

    if(m_area->subWindowList().isEmpty()) {  //last window closed
        myDebug() << "last closed";
        clearBooks();
        clearChapters();
        m_windowCache.clearAll();
        return 1;
    }
    reloadWindow(m_area->currentSubWindow());
    if(m_area->viewMode() == QMdiArea::SubWindowView)
        autoLayout();
    return 0;
}

int WindowManager::reloadWindow(QMdiSubWindow * window)
{
    DEBUG_FUNC_NAME
    //myDebug() << " enable reload = " << m_enableReload;
    if(!m_enableReload || window == NULL) {
        return 1;
    }
    const int windowName = window->objectName().toInt();
    myDebug() << "window name = " << windowName;

    if(m_area->subWindowList().count() <= 0) {
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
void WindowManager::mdiAreaResized()
{
    DEBUG_FUNC_NAME
    //todo: really every do time autoLayout ?
    if(m_area->viewMode() == QMdiArea::SubWindowView)
        autoLayout();
}
