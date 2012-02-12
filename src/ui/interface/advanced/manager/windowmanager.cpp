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

#include "src/ui/interface/advanced/subwindowfilter.h"

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
    setEnableReload(false);

    const int windowsCount = usableWindowList().size();

    QWidget *widget = new QWidget(m_area);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    m_nameCounter++;
    Form *form = NULL;
    if(type == Form::BibleForm) {
        form = new BibleForm(widget);
    } else if(type == Form::WebForm) {
        form = new WebForm(widget);
    } else if(type == Form::DictionaryForm) {
        form = new DictionaryForm(widget);
    } else if(type == Form::BookForm) {
        form = new BookForm(widget);
    } else if(type == Form::CommentaryForm) {
        form = new CommentaryForm(widget);
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

    SubWindowFilter *filter = new SubWindowFilter(subWindow);
    connect(filter, SIGNAL(close()), this, SLOT(closingWindow()));

    subWindow->installEventFilter(filter);

    if(forceMax) {
        subWindow->showMaximized();
    } else {
        subWindow->show();
    }

    form->setParentSubWindow(subWindow);
    m_area->setActiveSubWindow(subWindow);
    form->activated();

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
QMdiSubWindow* WindowManager::newBookSubWindow(bool doAutoLayout, bool forceMax)
{
    return newSubWindow(doAutoLayout, forceMax, Form::BookForm);
}
QMdiSubWindow* WindowManager::needWindow(Form::FormType type)
{
    if(usableWindowList().isEmpty()) {
        return newSubWindow(true, false, type);
    } else if(activeForm() != NULL) {
        if(activeForm()->type() != type) {
            QMdiSubWindow *window = NULL;
            foreach(QMdiSubWindow * w, usableWindowList()) {
                Form *f = getForm(w);
                if(f->type() == type) {
                    w->activateWindow();
                    m_area->setActiveSubWindow(w);
                    window = w;
                    break;
                }
            }
            if(window) {
                return window;
            } else {
                return newSubWindow(true, false, type);
            }
        } else {
            return activeSubWindow();
        }
    }
    return newSubWindow(true, false, type);
}
/**
  * No Windows => Create Window
  * If Active Window has the need type => ok
  * If there is a window with the need FormType and ContentType => ok
  * else create Window
  */
QMdiSubWindow* WindowManager::needWindow(Form::FormType type, ModuleTools::ContentType cType)
{
    if(usableWindowList().isEmpty()) {
        return newSubWindow(true, false, type);
    } else if(activeForm() != NULL) {
        if(activeForm()->type() != type) {
            QMdiSubWindow *window = NULL;
            foreach(QMdiSubWindow * w, usableWindowList()) {
                Form *f = getForm(w);
                if(f->type() == type) {
                    ModuleTools::ContentType mContentType = contentType(f);
                    if(ModuleTools::alsoOk(mContentType, cType) || mContentType == ModuleTools::UnkownContent) {
                        w->activateWindow();
                        m_area->setActiveSubWindow(w);
                        window = w;
                        break;
                    }
                }
            }
            if(window) {
                return window;
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
QMdiSubWindow* WindowManager::needDictionaryWindow(ModuleTools::ContentType contentType)
{
    return needWindow(Form::DictionaryForm, contentType);
}

QMdiSubWindow* WindowManager::needWebWindow()
{
    return needWindow(Form::WebForm);
}

QMdiSubWindow* WindowManager::hasDictWindow(ModuleTools::DefaultModule d)
{
    foreach(QMdiSubWindow * w, usableWindowList()) {
        Form *f = getForm(w);
        if(f->type() == Form::DictionaryForm) {
            DictionaryForm *form = (DictionaryForm*)(f);
            if(form->dictionary() != NULL) {
                const int moduleID = form->dictionary()->moduleID();
                if(m_settings->getModuleSettings(moduleID)->defaultModule == d) {
                    return w;
                }
            }
        }
    }
    return NULL;
}

QMdiSubWindow* WindowManager::hasDictWindow(const int moduleID)
{
    foreach(QMdiSubWindow * w, usableWindowList()) {
        Form *f = getForm(w);
        if(f->type() == Form::DictionaryForm) {
            DictionaryForm *form = (DictionaryForm*)(f);
            if(form->dictionary()->moduleID() == moduleID) {
                return w;
            }
        }
    }
    return NULL;
}

void WindowManager::activate(QMdiSubWindow *w)
{
    w->activateWindow();
    m_area->setActiveSubWindow(w);
}

void WindowManager::autoLayout()
{
    DEBUG_FUNC_NAME
    if(!m_enableReload)
        return;

    QList<QMdiSubWindow*> list = usableWindowList();

    if(list.size() > 1) {
        if(m_settings->autoLayout == 1) {
            tile();
        } else if(m_settings->autoLayout == 2) {
            tileVertical();
        } else if(m_settings->autoLayout == 3) {
            tileHorizontal();
        } else if(m_settings->autoLayout == 4) {
            cascade();
        }
    } else if(list.size() == 1) {
        QMdiSubWindow * w = list.first();
        w->showMaximized();
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

Form * WindowManager::getForm(QMdiSubWindow *w) const
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
void WindowManager::cascade(bool checked)
{
    if(checked) {
        m_settings->autoLayout = Settings::Cascade;
        cascade();
    }
}

void WindowManager::tileVertical(bool checked)
{
    if(checked) {
        m_settings->autoLayout = Settings::VerticalTile;
        tileVertical();
    }
}

void WindowManager::tileHorizontal(bool checked)
{
    if(checked) {
        m_settings->autoLayout = Settings::HorizontalTile;
        tileHorizontal();
    }
}

void WindowManager::tile(bool checked)
{
    if(checked) {
        m_settings->autoLayout = Settings::Tile;
        tile();
    }
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
	if(activeSubWindow())
		activeSubWindow()->setWindowTitle(title);
}

void WindowManager::closeSubWindow()
{
	if(activeSubWindow())
		activeSubWindow()->close();
}

int WindowManager::closingWindow()
{
    myDebug() << "signal recieved - close";

    if(m_area->subWindowList().isEmpty()) {
        myDebug() << "last window closed";
        m_actions->clearBooks();
        m_actions->clearChapters();
        return 1;
    }
  /*  myDebug() << "reload window";
    reloadWindow(m_area->currentSubWindow());*/

    myDebug() << "auto";

    if(m_area->viewMode() == QMdiArea::SubWindowView) {
        QTimer::singleShot(10, this, SLOT(autoLayout()));
    }

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

    foreach(const QString & id, groups) {
        const QString pre = m_settings->session.id() + "/windows/" + id + "/";
        const QString type = m_settings->session.file()->value(pre + "type").toString();
        const bool max = m_settings->session.file()->value(pre + "maximized").toBool();
        const QRect geo = m_settings->session.file()->value(pre + "geo").toRect();

        Form::FormType t = Form::BibleForm;
        if(type == "bible") {
            t = Form::BibleForm;
        } else if(type == "web") {
            t = Form::WebForm;
        } else if(type == "dictionary") {
            t = Form::DictionaryForm;
        } else if(type == "book") {
            t = Form::BookForm;
        } else if(type == "commentary") {
            t = Form::CommentaryForm;
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

ModuleTools::ContentType WindowManager::contentType(QMdiSubWindow* window)
{
    if(window == NULL)
        return ModuleTools::UnkownContent;

    Form *f = getForm(window);

    if(f == NULL)
        return ModuleTools::UnkownContent;

    return contentType(f);
}

ModuleTools::ContentType WindowManager::contentType(Form *form)
{
    if(form->type() == Form::DictionaryForm) {
        DictionaryForm *f = (DictionaryForm*) form;
        return contentType(f);
    } else if(form->type() == Form::BibleForm) {
        BibleForm *f = (BibleForm*) form;
        return contentType(f);
    }
    return ModuleTools::UnkownContent;
}

ModuleTools::ContentType WindowManager::contentType(DictionaryForm *form)
{
    if(form->dictionary()) {
        ModuleSettings *s = m_settings->getModuleSettings(form->dictionary()->moduleID());
        if(s != NULL)
            return s->contentType;
    }
    return ModuleTools::UnkownContent;
}

ModuleTools::ContentType WindowManager::contentType(WebForm *form)
{
	return ModuleTools::UnkownContent;
}

ModuleTools::ContentType WindowManager::contentType(BibleForm *form)
{
    if(form) {
        ModuleSettings *s = m_settings->getModuleSettings(form->verseModule()->moduleID());
        if(s != NULL)
            return s->contentType;
    }
    return ModuleTools::UnkownContent;
}
