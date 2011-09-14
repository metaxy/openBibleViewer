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
#include <typeinfo>
#include "src/ui/interface/advanced/form/dictionaryform.h"

WindowManager::WindowManager(QObject *parent) :
    QObject(parent)
{
    m_nameCounter = 0;
    m_currentWindowID = new int(-1);
}
WindowManager::~WindowManager()
{
    delete m_currentWindowID;
    m_currentWindowID = NULL;
}

void WindowManager::setMdiArea(QMdiArea *area)
{
    m_area = area;
}

void WindowManager::setApi(Api *api)
{
    m_api = api;
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
        newBibleSubWindow();
}
QMdiSubWindow* WindowManager::newSubWindow(bool doAutoLayout, bool forceMax, Form::FormType type)
{
    DEBUG_FUNC_NAME
    setEnableReload(false);

    const int windowsCount = usableWindowList().size();
    QMdiSubWindow *firstSubWindow = new QMdiSubWindow();
    if(windowsCount == 1) {
        firstSubWindow = usableWindowList().at(0);
    }

    QWidget *widget = new QWidget(m_area);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    m_nameCounter++;
    Form *form = NULL;
    if(type == Form::BibleForm) {
        form = new BibleForm(widget);
    } else if(type == Form::WebForm){
        form = new WebForm(widget);
    } else if(type == Form::DictionaryForm){
        form = new DictionaryForm(widget);
    }
    form->setID(m_nameCounter);
    form->setObjectName("mdiForm");
    form->currentWindowID = m_currentWindowID;
    setAll(form);

    form->setApi(m_api);
    form->setNotesManager(m_notesManager);
    form->setBookmarksManager(m_bookmarksManager);
    form->init();

    *m_currentWindowID = form->id();
    layout->addWidget(form);

    widget->setLayout(layout);
    QMdiSubWindow *subWindow = m_area->addSubWindow(widget);
    subWindow->setWindowIcon(QIcon(":/icons/16x16/main.png"));
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();
    form->setParentSubWindow(subWindow);
    m_area->setActiveSubWindow(subWindow);
    form->activated();


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

    setEnableReload(true);
    if(doAutoLayout && m_area->viewMode() == QMdiArea::SubWindowView && windowsCount > 0) {
        autoLayout();
    }
    m_actions->clearBooks();
    m_actions->clearChapters();
    return subWindow;
}

QMdiSubWindow* WindowManager::newBibleSubWindow(bool doAutoLayout, bool forceMax)
{
    return newSubWindow(doAutoLayout, forceMax, Form::BibleForm);
}

QMdiSubWindow* WindowManager::newWebSubWindow(bool doAutoLayout, bool forceMax)
{
   return newSubWindow(doAutoLayout, forceMax, Form::WebForm);
}
QMdiSubWindow* WindowManager::newDictionarySubWindow(bool doAutoLayout, bool forceMax)
{
   return newSubWindow(doAutoLayout, forceMax, Form::DictionaryForm);
}

QMdiSubWindow* WindowManager::needWindow(Form::FormType type)
{
    if(usableWindowList().isEmpty()) {
        return newSubWindow(true, false, type);
    } else if(activeForm() != NULL) {
        if(activeForm()->type() != type) {
            QMdiSubWindow *ww = NULL;
            foreach(QMdiSubWindow *w, usableWindowList()) {
                Form *f = getForm(w);
                if(f->type() == type) {
                    myDebug() << "activate window";
                    w->activateWindow();
                    m_area->setActiveSubWindow(w);
                    ww = w;
                    break;
                }
            }
            if(ww) {
                myDebug() << "found window";
                return ww;
            } else {
                return newSubWindow(true, false, type);
            }
        } else {
            return activeSubWindow();
        }
    }
    return newSubWindow(true, false, type);
}


QMdiSubWindow* WindowManager::needBibleWindow()
{
    return needWindow(Form::BibleForm);
}


QMdiSubWindow* WindowManager::needDictionaryWindow()
{
     return needWindow(Form::DictionaryForm);
}


QMdiSubWindow* WindowManager::needWebWindow()
{
    return needWindow(Form::WebForm);
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
    const QList<QMdiSubWindow*> list = usableWindowList();
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

Form * WindowManager::activeForm()
{
    return getForm(activeSubWindow());
}
Form * WindowManager::getForm(QMdiSubWindow *w)
{
    if(w == NULL)
        return NULL;

    return w->widget()->findChild<Form *>("mdiForm");
}

void WindowManager::tileVertical()
{
    const QList<QMdiSubWindow*> windows = usableWindowList();
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
    const QList<QMdiSubWindow*> windows = usableWindowList();
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

QList<QMdiSubWindow*> WindowManager::usableWindowList() const
{
    QList<QMdiSubWindow*> ret;
    foreach(QMdiSubWindow * w, m_area->subWindowList(QMdiArea::ActivationHistoryOrder)) {
        if(w->isHidden())
            continue;
        ret.prepend(w);
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
    //DEBUG_FUNC_NAME
    if(!m_enableReload)
        return 1;
    if(m_area->subWindowList().isEmpty()) {
        myDebug() << "last window closed";
        m_actions->clearBooks();
        m_actions->clearChapters();
        return 1;
    }

    reloadWindow(m_area->currentSubWindow());
    if(m_area->viewMode() == QMdiArea::SubWindowView)
        autoLayout();
    return 0;
}

int WindowManager::reloadWindow(QMdiSubWindow * window)
{
    if(!m_enableReload || window == NULL) {
        myDebug() << "reload is not enabled or window == NULL";
        return 1;
    }

    if(m_area->subWindowList().isEmpty()) {
        myDebug() << "sub window list is empty";
        return 1;
    }

    Form *form = window->widget()->findChild<Form *>("mdiForm");
    *m_currentWindowID = form->id();
    form->activated();
    m_area->setActiveSubWindow(window);

    return 0;
}

void WindowManager::mdiAreaResized()
{
    //todo: really every do time autoLayout ?
    if(m_area->viewMode() == QMdiArea::SubWindowView) {
        const QMdiSubWindow * w = m_area->activeSubWindow();
        if(w != NULL && w->isMaximized()) {
            return;
        } else {
            autoLayout();
        }
    }
}

void WindowManager::reloadActive()
{
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
    m_settings->session.clearGroup("windows");
    int current = 0;
    QMdiSubWindow *currentSubWindow = activeSubWindow();
    for(int i = 0, count = m_area->subWindowList().size(); i < count; i++) {
        QMdiSubWindow *a = m_area->subWindowList().at(i);
        if(currentSubWindow == a)
            current = i;
        Form *form = a->widget()->findChild<Form *>("mdiForm");
        form->save();
        const QString s = m_settings->session.id() + "/windows/" + QString::number(form->id()) + "/";
        m_settings->session.file()->setValue(s + "geo", a->geometry());
        m_settings->session.file()->setValue(s + "maximized", a->isMaximized());
        //m_settings->session.file()->setValue(a + "windowState", m_parentSubWindow->windowState());
    }

    m_settings->session.setData("viewMode", m_area->viewMode());
    m_settings->session.setData("windowID", current);

    //no more reloading
    disable();
}
void WindowManager::restore()
{
    const int viewMode = m_settings->session.getData("viewMode").toInt();
    if(viewMode == 0)
        m_actions->setSubWindowView();
    else
        m_actions->setTabbedView();

    m_settings->session.file()->beginGroup(m_settings->session.id() + "/windows/");
    const QStringList groups = m_settings->session.file()->childGroups();
    m_settings->session.file()->endGroup();

    foreach(const QString &id, groups) {
        const QString pre = m_settings->session.id() + "/windows/" + id + "/";
        const QString type = m_settings->session.file()->value(pre + "type").toString();
        const bool max = m_settings->session.file()->value(pre + "maximized").toBool();
        const QRect geo = m_settings->session.file()->value(pre + "geo").toRect();

        Form::FormType t = Form::BibleForm;
        if(type == "bible") {
            t = Form::BibleForm;
        } else if(type == "web"){
            t = Form::WebForm;
        } else if(type == "dictionary"){
            t = Form::DictionaryForm;
        }
        QMdiSubWindow *w = newSubWindow(true, max, t);

        if(viewMode == 0 && !max) {
            w->setGeometry(geo);
        }

        Form *f = w->widget()->findChild<Form *>("mdiForm");
        f->restore(id);
    }
    const int id = m_settings->session.getData("windowID", -1).toInt();
    if(id < m_area->subWindowList().size() && id > 0) {
        m_area->setActiveSubWindow(m_area->subWindowList().at(id));
    }
}

void WindowManager::installResizeFilter()
{
    m_mdiAreaFilter = new MdiAreaFilter(m_area);
    connect(m_mdiAreaFilter, SIGNAL(resized()), this, SLOT(mdiAreaResized()));
    m_area->installEventFilter(m_mdiAreaFilter);
}
//only bibleform
void WindowManager::reloadChapter(bool full)
{
    DEBUG_FUNC_NAME;
    if(typeid(activeForm()) != typeid(BibleForm *))
        return;

    const QWebView *view = ((BibleForm*)activeForm())->m_view;
    const QPoint p = view->page()->mainFrame()->scrollPosition();

    if(full) {
        m_actions->reloadBible();
    } else {
        m_actions->reShowCurrentRange();
    }

    view->page()->mainFrame()->setScrollPosition(p);
}

