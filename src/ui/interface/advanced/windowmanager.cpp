#include "windowmanager.h"
#include "src/core/dbghelper.h"
#include "bibleform.h"
#include <QtGui/QMdiSubWindow>
WindowManager::WindowManager(QObject *parent) :
    QObject(parent)
{
}
void WindowManager::setMdiArea(QMdiArea *area)
{
    m_area = area;
}
void WindowManager::setApi(Api *api)
{
    m_api = api;
}
void WindowManager::init()
{
    connect(m_area, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(reloadWindow(QMdiSubWindow *)));
}

void WindowManager::newSubWindow(bool doAutoLayout)
{
    DEBUG_FUNC_NAME;
    setEnableReload(false);
    int windowsCount = usableWindowList().size();
    QMdiSubWindow *firstSubWindow = new QMdiSubWindow();
    if(windowsCount == 1) {
        firstSubWindow = usableWindowList().at(0);
    }

    QWidget *widget = new QWidget(m_area);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    BibleForm *bibleForm = new BibleForm(widget);
    bibleForm->setObjectName("mdiForm");
    setAll(bibleForm);
    bibleForm->setApi(m_api);
    bibleForm->init();

    layout->addWidget(bibleForm);

    widget->setLayout(layout);
    QMdiSubWindow *subWindow = m_area->addSubWindow(widget);
    //subWindow->setObjectName(QString::number(windowName)); //todo:
    subWindow->setWindowIcon(QIcon(":/icons/16x16/main.png"));
    subWindow->setWindowOpacity(1.0);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();
    m_area->setActiveSubWindow(subWindow);


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

    connect(bibleForm->m_view->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(pharseUrl(QUrl)));
    connect(bibleForm->m_view, SIGNAL(contextMenuRequested(QContextMenuEvent*)), this, SLOT(showContextMenu(QContextMenuEvent*)));

    bibleForm->m_view->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    bibleForm->m_view->settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
#if QT_VERSION >= 0x040700
    bibleForm->m_view->settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    bibleForm->m_view->settings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    bibleForm->m_view->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    bibleForm->m_view->settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
    bibleForm->m_view->settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
#endif

    connect(bibleForm, SIGNAL(historyGo(QString)), this, SLOT(pharseUrl(QString)));
    connect(bibleForm, SIGNAL(previousChapter()), this, SLOT(previousChapter()));
    connect(bibleForm, SIGNAL(nextChapter()), this, SLOT(nextChapter()));
    connect(this, SIGNAL(historySetUrl(QString)), bibleForm, SLOT(historyGetUrl(QString)));
    connect(subWindow, SIGNAL(destroyed(QObject*)), this, SLOT(closingWindow()));

    setEnableReload(true);
    if(doAutoLayout && m_area->viewMode() == QMdiArea::SubWindowView) {
        autoLayout();
    }

    //todo: clear old stuff
    //clearBooks();
    //clearChapters();
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
    return NULL;
}
BibleForm * WindowManager::activeForm()
{
    if(activeMdiChild()) {
        return activeMdiChild()->widget()->findChild<BibleForm *>("mdiForm");
    }
    return NULL;
}

void WindowManager::myTileVertical()
{
    if(!m_enableReload || !usableWindowList().count()) {
        return;
    }
    QList<QMdiSubWindow*> windows = usableWindowList();
    m_enableReload = false;
    QMdiSubWindow* active = m_area->activeSubWindow();

    const int widthForEach = m_area->width() / windows.count();
    unsigned int x = 0;
    foreach(QMdiSubWindow * window, windows) {
        window->showNormal();

        const int preferredWidth = window->minimumWidth() + window->baseSize().width();
        const int actWidth = qMax(widthForEach, preferredWidth);

        window->setGeometry(x, 0, actWidth, m_area->height());
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

    const int heightForEach = m_area->height() / windows.count();
    unsigned int y = 0;
    foreach(QMdiSubWindow * window, windows) {
        window->showNormal();

        const int preferredHeight = window->minimumHeight() + window->baseSize().height();
        const int actHeight = qMax(heightForEach, preferredHeight);

        window->setGeometry(0, y, m_area->width(), actHeight);
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
        if(m_area->activeSubWindow()->size() != m_area->size()) {
            m_area->activeSubWindow()->resize(m_area->size());
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
        //clearBooks();
        //clearChapters();
        return 1;
    }

    if(m_area->subWindowList().isEmpty()) {  //last window closed
        myDebug() << "last closed";
        //clearBooks();
        //clearChapters();
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
    activeForm()->activated();


    return 0;
}
void WindowManager::mdiAreaResized()
{
    DEBUG_FUNC_NAME
    //todo: really every do time autoLayout ?
    if(m_area->viewMode() == QMdiArea::SubWindowView)
        autoLayout();
}
void WindowManager::reloadActive()
{
    DEBUG_FUNC_NAME
    //setEnableReload(true);
    reloadWindow(m_area->currentSubWindow());
    //setEnableReload(false);
}

void WindowManager::setEnableReload(bool enable)
{
    m_enableReload = enable;
}

void WindowManager::zoomIn()
{
    if(activeForm())
        activeForm()->zoomIn();
}

void WindowManager::zoomOut()
{
    if(activeForm())
        activeForm()->zoomOut();
}
