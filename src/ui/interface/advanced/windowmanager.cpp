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
#include "windowmanager.h"
#include "src/core/dbghelper.h"
#include "bibleform.h"
#include "windowsessiondata.h"
#include <QtGui/QMdiSubWindow>
WindowManager::WindowManager(QObject *parent) :
    QObject(parent)
{
    m_nameCounter = 0;
    m_currentWindowID = new int;
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

void WindowManager::init()
{
    connect(m_area, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(reloadWindow(QMdiSubWindow *)));
    connect(m_actions, SIGNAL(_setTabbedView()), this, SLOT(setTabbedView()));
    connect(m_actions, SIGNAL(_setSubWindowView()), this, SLOT(setSubWindowView()));
    connect(m_actions, SIGNAL(_setTitle(QString)), this, SLOT(setTitle(QString)));
}

void WindowManager::newSubWindow(bool doAutoLayout)
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
    bibleForm->init();

    *m_currentWindowID = bibleForm->id();
    layout->addWidget(bibleForm);

    widget->setLayout(layout);
    QMdiSubWindow *subWindow = m_area->addSubWindow(widget);
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
    if(doAutoLayout && m_area->viewMode() == QMdiArea::SubWindowView) {
        autoLayout();
    }

    //todo: clear old stuff
    //clearBooks();
    //clearChapters();
}

void WindowManager::autoLayout()
{
    if(!m_enableReload)
        return;
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

/*int WindowManager::currentWindowName()
{
    if(m_area->activeSubWindow() != NULL) {
        return m_area->activeSubWindow()->objectName().toInt();
    }
    return -1;
}*/

void WindowManager::setTitle(const QString &title)
{
    activeMdiChild()->setWindowTitle(title);
}

void WindowManager::closeSubWindow()
{
    activeMdiChild()->close();
}

int WindowManager::closingWindow()
{
    if(!m_enableReload)
        return 1;
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

    if(m_area->subWindowList().isEmpty()) {
        return 1;
    }
    *m_currentWindowID = activeForm()->id();
    activeForm()->activated();
    myDebug() << "currentWindowID = " << *m_currentWindowID;
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
    DEBUG_FUNC_NAME;
    reloadWindow(activeMdiChild());
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
    m_enableReload = false;
}

void WindowManager::enable()
{
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
    QMdiSubWindow *currentSubWindow = activeMdiChild();
    for(int i = 0, count = i < m_area->subWindowList().count(); i < count; i++) {
        QMdiSubWindow *a = m_area->subWindowList().at(i);
        data.setWindowID(i);
        if(currentSubWindow == a)
            current = i;
        BibleForm *form = a->widget()->findChild<BibleForm *>("mdiForm");
        BibleList *list = form->m_bibleList;
        QString u = "";
        if(list > 0) {
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
        data.setUrl(u);
        data.setScrollPosition(form->m_view->page()->mainFrame()->scrollPosition());
        data.setZoom(form->m_view->zoomFactor());
        data.setGeo(a->geometry());
    }
    data.write();
    m_settings->session.setData("viewMode", m_area->viewMode());
    m_settings->session.setData("windowID", current);
}
void WindowManager::restore()
{
    WindowSessionData data;
    data.setSettings(m_settings);
    data.read();
    for(int i = 0; i < data.size(); ++i) {
        newSubWindow(true);
        data.setWindowID(i);
        //load bible
        const QString url = data.url();
        m_moduleManager->bibleList()->clear();

        if(!url.isEmpty() && url.size() != 0) {
            QStringList list = url.split("|");
            foreach(QString part, list) {
                QStringList a = part.split(":");
                const int x = a.at(0).toInt();
                const int y = a.at(1).toInt();
                const QString u = a.at(2);
                UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, u);
                urlConverter.setSettings(m_settings);
                urlConverter.setModuleMap(m_moduleManager->m_moduleMap);
                urlConverter.pharse();
                m_moduleManager->newBible(urlConverter.m_moduleID, QPoint(x, y));

                m_actions->get(urlConverter.convert() + ",force=true"); //todo: MOVE IT OUT
            }
        } else {
            Bible *b = new Bible();
            m_moduleManager->initBible(b);
            m_moduleManager->bibleList()->addBible(b, QPoint(0, 0));
        }
        activeMdiChild()->setGeometry(data.geo());
        QWebView *v = activeForm()->m_view;
        v->page()->mainFrame()->setScrollPosition(data.scrollPosition());
        v->setZoomFactor(data.zoom());
    }
    const int viewMode = m_settings->session.getData("viewMode").toInt();
    if(viewMode == 0)
        m_actions->setSubWindowView();
    else
        m_actions->setTabbedView();

    const int id = m_settings->session.getData("windowID", -1).toInt();
    if(id < m_area->subWindowList().size() && id > 0) {
        m_area->setActiveSubWindow(m_area->subWindowList().at(id));
    }
}
/*
 * Todo: Use it
 */
/*
void AdvancedInterface::installResizeFilter()
{
    m_mdiAreaFilter = new MdiAreaFilter(ui->mdiArea);
    connect(m_mdiAreaFilter, SIGNAL(resized()), this, SLOT(mdiAreaResized()));
    ui->mdiArea->installEventFilter(m_mdiAreaFilter);
}*/


