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
#include "windowmanager.h"

WindowManager::WindowManager(QObject *parent) :
    QObject(parent)
{
    m_nameCounter = 0;
    m_currentWindowID = new int(-1);
}

void WindowManager::setMdiArea(QMdiArea *area)
{
    m_area = area;
}

void WindowManager::setApi(Api *api)
{
    m_api = api;
}

void WindowManager::setBibleManager(BibleManager *bibleManager)
{
    m_bibleManager = bibleManager;
}

void WindowManager::setNotesManager(NotesManager *notesManager)
{
    m_notesManager = notesManager;
}

void WindowManager::setBookmarksManager(BookmarksManager *bookmarksManager)
{
    m_bookmarksManager = bookmarksManager;
}

void WindowManager::init()
{
    connect(m_area, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(reloadWindow(QMdiSubWindow *)));

    connect(m_actions, SIGNAL(_setTabbedView()), this, SLOT(setTabbedView()));
    connect(m_actions, SIGNAL(_setSubWindowView()), this, SLOT(setSubWindowView()));
    connect(m_actions, SIGNAL(_setTitle(QString)), this, SLOT(setTitle(QString)));
    connect(m_actions, SIGNAL(_reloadActive()), this, SLOT(reloadActive()));
    connect(m_actions, SIGNAL(_reloadChapter(bool)), this, SLOT(reloadChapter(bool)));
    connect(m_actions, SIGNAL(_newSubWindowIfEmpty()), this, SLOT(newSubWindowIfEmpty()));

    //only install if we have autolayout enabled
    if(m_settings->autoLayout > 0) {
        installResizeFilter();
    }
}

void WindowManager::newSubWindowIfEmpty()
{
    if(usableWindowList().isEmpty())
        newSubWindow();
}

void WindowManager::newSubWindow(bool doAutoLayout, bool forceMax)
{
    setEnableReload(false);

    const int windowsCount = usableWindowList().size();
    QMdiSubWindow *firstSubWindow = new QMdiSubWindow();
    if(windowsCount == 1) {
        firstSubWindow = usableWindowList().at(0);
    }

    QWidget *widget = new QWidget(m_area);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    m_nameCounter++;
    BibleForm *bibleForm = new BibleForm(widget);
    bibleForm->setID(m_nameCounter);
    bibleForm->setObjectName("mdiForm");
    bibleForm->currentWindowID = m_currentWindowID;
    setAll(bibleForm);
    bibleForm->setApi(m_api);
    bibleForm->setBibleManager(m_bibleManager);
    bibleForm->setNotesManager(m_notesManager);
    bibleForm->setBookmarksManager(m_bookmarksManager);
    bibleForm->init();

    *m_currentWindowID = bibleForm->id();
    layout->addWidget(bibleForm);

    widget->setLayout(layout);
    QMdiSubWindow *subWindow = m_area->addSubWindow(widget);
    subWindow->setWindowIcon(QIcon(":/icons/16x16/main.png"));
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();
    m_area->setActiveSubWindow(subWindow);
    bibleForm->activated();


    if(forceMax) {
        myDebug() << "show maximized";
        subWindow->showMaximized();
    } else if(m_area->viewMode() == QMdiArea::SubWindowView) {
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

    connect(bibleForm->m_view->page(), SIGNAL(linkClicked(QUrl)), m_actions, SLOT(get(QUrl)));

    bibleForm->m_view->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    bibleForm->m_view->settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
#if QT_VERSION >= 0x040700
    bibleForm->m_view->settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    bibleForm->m_view->settings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    bibleForm->m_view->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    bibleForm->m_view->settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
    bibleForm->m_view->settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
#endif

    connect(bibleForm, SIGNAL(historyGo(QString)), m_actions, SLOT(get(QString)));
    connect(bibleForm, SIGNAL(previousChapter()), m_actions, SLOT(previousChapter()));
    connect(bibleForm, SIGNAL(nextChapter()), m_actions, SLOT(nextChapter()));
    connect(this, SIGNAL(historySetUrl(QString)), bibleForm, SLOT(historyGetUrl(QString)));
    connect(subWindow, SIGNAL(destroyed(QObject*)), this, SLOT(closingWindow()));

    setEnableReload(true);
    if(doAutoLayout && m_area->viewMode() == QMdiArea::SubWindowView && windowsCount > 0) {
        autoLayout();
    }
    m_actions->clearBooks();
    m_actions->clearChapters();
}

void WindowManager::autoLayout()
{
    if(!m_enableReload)
        return;
    if(usableWindowList().size() > 1) {
        if(m_settings->autoLayout == 1) {
            tile();
        } else if(m_settings->autoLayout == 2) {
            tileVertical();
        } else if(m_settings->autoLayout == 3) {
            tileHorizontal();
        } else if(m_settings->autoLayout == 4) {
            cascade();
        }
    }
}

QMdiSubWindow * WindowManager::activeSubWindow()
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
    } else if(usableWindowList().size() > 0) {
        m_area->setActiveSubWindow(usableWindowList().at(0));
        if(QMdiSubWindow *activeSubWindow = m_area->activeSubWindow())
            return activeSubWindow;
    }
    return NULL;
}
BibleForm * WindowManager::activeForm()
{
    if(activeSubWindow()) {
        return activeSubWindow()->widget()->findChild<BibleForm *>("mdiForm");
    }
    return NULL;
}

void WindowManager::tileVertical()
{
    QList<QMdiSubWindow*> windows = usableWindowList();
    if(!m_enableReload || windows.isEmpty()) {
        return;
    }
    setEnableReload(false);
    QMdiSubWindow* active = m_area->activeSubWindow();

    const int widthForEach = m_area->width() / windows.size();
    unsigned int x = 0;
    foreach(QMdiSubWindow * window, windows) {
        window->showNormal();

        const int preferredWidth = window->minimumWidth() + window->baseSize().width();
        const int actWidth = qMax(widthForEach, preferredWidth);

        window->setGeometry(x, 0, actWidth, m_area->height());
        x += actWidth;
    }

    if(active)
        active->setFocus();
    setEnableReload(true);
}

void WindowManager::tileHorizontal()
{
    QList<QMdiSubWindow*> windows = usableWindowList();
    if(!m_enableReload || windows.isEmpty()) {
        return;
    }

    setEnableReload(false);
    QMdiSubWindow* active = m_area->activeSubWindow();

    const int heightForEach = m_area->height() / windows.size();
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

void WindowManager::cascade()
{
    QList<QMdiSubWindow*> windows = usableWindowList();
    if(!m_enableReload || windows.isEmpty()) {
        return;
    }

    setEnableReload(false);
    if(m_area->activeSubWindow() && m_area->activeSubWindow()->isMaximized()) {
        if(m_area->activeSubWindow()->size() != m_area->size()) {
            m_area->activeSubWindow()->resize(m_area->size());
        }
    } else if(windows.size() == 1) {
        windows.at(0)->showMaximized();
    } else {

        QMdiSubWindow* active = m_area->activeSubWindow();

        const unsigned int offsetX = 40;
        const unsigned int offsetY = 40;
        const unsigned int windowWidth =  m_area->width() - (windows.size() - 1) * offsetX;
        const unsigned int windowHeight = m_area->height() - (windows.size() - 1) * offsetY;
        unsigned int x = 0;
        unsigned int y = 0;

        foreach(QMdiSubWindow * window, windows) {
            if(active == window)
                continue;
            window->raise();
            window->setGeometry(x, y, windowWidth, windowHeight);
            x += offsetX;
            y += offsetY;
        }
        active->setGeometry(x, y, windowWidth, windowHeight);
        active->raise();
        active->activateWindow();
    }
    setEnableReload(true);
}

void WindowManager::tile()
{
    if(!m_enableReload || usableWindowList().isEmpty()) {
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

void WindowManager::setTitle(const QString &title)
{
    activeSubWindow()->setWindowTitle(title);
}

void WindowManager::closeSubWindow()
{
    activeSubWindow()->close();
}

int WindowManager::closingWindow()
{
    DEBUG_FUNC_NAME
    if(!m_enableReload)
        return 1;
    if(m_area->subWindowList().isEmpty()) {
        myDebug() << "last window closed";
        m_actions->clearBooks();
        m_actions->clearChapters();
        //m_moduleManager->m_bibleList = NULL;
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
    if(!m_enableReload || window == NULL) {
        myDebug() << "reload is not enabled or window == NULL";
        return 1;
    }
    if(m_area->subWindowList().isEmpty()) {
        myDebug() << "sub window list is empty";
        return 1;
    }
    BibleForm *form =  window->widget()->findChild<BibleForm *>("mdiForm");
    *m_currentWindowID = form->id();
    myDebug() << "window ID  = " << *m_currentWindowID;
    form->activated();
    m_area->setActiveSubWindow(window);
    return 0;
}
void WindowManager::mdiAreaResized()
{
    //DEBUG_FUNC_NAME
    //todo: really every do time autoLayout ?
    if(m_area->viewMode() == QMdiArea::SubWindowView)
        autoLayout();
}
void WindowManager::reloadActive()
{
    //DEBUG_FUNC_NAME
    reloadWindow(activeSubWindow());
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
void WindowManager::disable()
{
    //DEBUG_FUNC_NAME
    m_enableReload = false;
}

void WindowManager::enable()
{
    //DEBUG_FUNC_NAME
    m_enableReload = true;
}
void WindowManager::setSubWindowView()
{
    m_area->setViewMode(QMdiArea::SubWindowView);
}
void WindowManager::setTabbedView()
{
    m_area->setViewMode(QMdiArea::TabbedView);
}

void WindowManager::save()
{
    WindowSessionData data;
    data.setSettings(m_settings);
    int current = 0;
    QMdiSubWindow *currentSubWindow = activeSubWindow();
    for(int i = 0, count = m_area->subWindowList().size(); i < count; i++) {
        QMdiSubWindow *a = m_area->subWindowList().at(i);
        data.setWindowID(i);
        if(currentSubWindow == a)
            current = i;
        BibleForm *form = a->widget()->findChild<BibleForm *>("mdiForm");
        VerseTable *list = form->m_verseTable;

        QList<QString> urls;
        QList<QPoint> points;
        if(list > 0) {
            QHashIterator<int, VerseModule *> i(list->m_modules);
            while(i.hasNext()) {
                i.next();
                VerseModule *b = i.value();
                if(b != NULL && b->moduleID() >= 0) {
                    myDebug() << "key = " << i.key();
                    VerseUrl bibleUrl;
                    bibleUrl.addRanges(b->lastTextRanges()->toBibleUrlRanges(i.key()));

                    UrlConverter urlConverter(UrlConverter::InterfaceUrl, UrlConverter::PersistentUrl, bibleUrl);
                    urlConverter.setSettings(m_settings);
                    urlConverter.setModuleMap(m_moduleManager->m_moduleMap);
                    VerseUrl newUrl = urlConverter.convert();

                    const QString url = newUrl.toString();
                    const QPoint point = list->m_points.value(i.key());
                    myDebug() << "url = " << url << " at " << point;
                    urls << url;
                    points << point;
                }
            }
        }
        data.setUrl(urls);
        data.setBiblePoint(points);

        data.setScrollPosition(form->m_view->page()->mainFrame()->scrollPosition());
        data.setZoom(form->m_view->zoomFactor());
        data.setGeo(a->geometry());
        data.setMaximized(a->isMaximized());
        myDebug() << "max = " << a->isMaximized();
        //myDebug() << a->windowState();
        data.setWindowState(a->windowState());
    }
    data.write();
    m_settings->session.setData("viewMode", m_area->viewMode());
    m_settings->session.setData("windowID", current);
    //no more reloading
    disable();
}
void WindowManager::restore()
{
    WindowSessionData data;
    data.setSettings(m_settings);
    data.read();
    const int viewMode = m_settings->session.getData("viewMode").toInt();
    if(viewMode == 0)
        m_actions->setSubWindowView();
    else
        m_actions->setTabbedView();
    //myDebug() << "size = " << data.size();
    for(int i = 0; i < data.size(); ++i) {
        data.setWindowID(i);
        //myDebug() << "max = " << data.maximized();
        newSubWindow(true, data.maximized());

        //load bible
        m_moduleManager->verseTable()->clear();
        const QList<QString> urls = data.url();
        const QList<QPoint> points = data.biblePoint();
        for(int j = 0; j < urls.size() && j < points.size(); j++) {
            const QString url = urls.at(j);
            const QPoint point = points.at(j);
            //myDebug() << "url = " << url << " point = " << point;
            UrlConverter2 urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
            urlConverter.setSM(m_settings, m_moduleManager->m_moduleMap);
            urlConverter.convert();
            if(urlConverter.moduleID() != -1) {
                m_moduleManager->newVerseModule(urlConverter.moduleID(), point);
                m_actions->get(urlConverter.url());
                myDebug() << urlConverter.url().toString();
            }
        }
        if(viewMode == 0 && !data.maximized()) {
            //myDebug() << "setting geo";
            activeSubWindow()->setGeometry(data.geo());
        }
        activeSubWindow()->setWindowState(data.windowState());
        QWebView *v = activeForm()->m_view;
        v->page()->mainFrame()->setScrollPosition(data.scrollPosition());
        v->setZoomFactor(data.zoom());
    }

    const int id = m_settings->session.getData("windowID", -1).toInt();
    if(id < m_area->subWindowList().size() && id > 0) {
        m_area->setActiveSubWindow(m_area->subWindowList().at(id));
    }
}
/*
 * Todo: Use it
 */

void WindowManager::installResizeFilter()
{
    m_mdiAreaFilter = new MdiAreaFilter(m_area);
    connect(m_mdiAreaFilter, SIGNAL(resized()), this, SLOT(mdiAreaResized()));
    m_area->installEventFilter(m_mdiAreaFilter);
}
void WindowManager::reloadChapter(bool full)
{
    DEBUG_FUNC_NAME;

    const QWebView *v = activeForm()->m_view;
    const QPoint p = v->page()->mainFrame()->scrollPosition();
    if(full) {
        m_actions->reloadBible();
    } else {
        m_actions->reshowCurrentRange();
    }
    v->page()->mainFrame()->setScrollPosition(p);
}

