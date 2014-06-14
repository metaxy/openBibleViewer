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
#include "windowmanager.h"
#include <qmath.h>
#include <QScrollBar>
#include "src/ui/interface/advanced/subwindowfilter.h"
#include "src/api/api.h"

WindowManager::WindowManager(QObject *parent) :
    QObject(parent)
{
    m_nameCounter = 0;
    m_currentWindowID = new int(-1);
}
WindowManager::~WindowManager()
{
    delete m_currentWindowID;
    m_currentWindowID = nullptr;
}

void WindowManager::setMdiArea(MdiArea *area)
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

    //only connect if we have autolayout enabled
    if(m_settings->autoLayout > 0) {
        connect(m_area, SIGNAL(resized()), this, SLOT(mdiAreaResized()));
    }
}

QMdiSubWindow* WindowManager::newSubWindow(Form::FormType type, bool forceMax)
{
    setEnableReload(false);

    const int windowsCount = usableWindowList().size();

    QWidget *widget = new QWidget(m_area);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    m_nameCounter++;
    Form *form = Form::createForm(widget, type);

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
    MdiSubWindow *subWindow = new MdiSubWindow(m_area);
    subWindow->setWidget(widget);
    subWindow->setParent(m_area);

    subWindow->setWindowIcon(QIcon(":/icons/16x16/main.png"));
    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    connect(subWindow, SIGNAL(close()), this, SLOT(closingWindow()));


    if(forceMax) {
        subWindow->showMaximized();
    } else {
        subWindow->show();
    }

    form->setParentSubWindow(subWindow);
    m_area->setActiveSubWindow(subWindow);
    form->activated();

    setEnableReload(true);
    if(m_area->viewMode() == QMdiArea::SubWindowView && windowsCount > 0) {
        autoLayout();
    }
    return subWindow;
}
void WindowManager::newBibleSubWindow()
{
    newSubWindow(Form::BibleForm);
}

void WindowManager::newWebSubWindow()
{
    newSubWindow(Form::WebForm);
}

void WindowManager::newDictionarySubWindow()
{
    newSubWindow(Form::DictionaryForm);
}

void WindowManager::newBookSubWindow()
{
    newSubWindow(Form::BookForm);
}
void WindowManager::newCommentarySubWindow()
{
    newSubWindow(Form::CommentaryForm);
}
void WindowManager::newConsoleSubWindow()
{
    newSubWindow(Form::ConsoleForm);
}
QMdiSubWindow* WindowManager::needWindow(Form::FormType type)
{
   return needWindow(type, [type](Form *f){ return f->type() == type;});
}
QMdiSubWindow* WindowManager::needWindow(Form::FormType type, std::function<bool (Form*)> func)
{
   return needWindow(type, func, func);
}
QMdiSubWindow* WindowManager::needWindow(Form::FormType type, std::function<bool (Form*)> activeFunc, std::function<bool (Form*)> otherFunc)
{
    QMdiSubWindow *activeWindow = activeSubWindow();
    if(usableWindowList().isEmpty()) {
        return newSubWindow(type);
    } else if(activeForm() != nullptr) {
        if(activeFunc(activeForm())) {
            return activeWindow;
        } else {
            QMdiSubWindow *window = nullptr;
            foreach(QMdiSubWindow * w, usableWindowList()) {
                if(w == activeWindow)
                    continue;
                Form *f = getForm(w);
                if(otherFunc(f)) {
                    w->activateWindow();
                    m_area->setActiveSubWindow(w);
                    window = w;
                    break;
                }
            }
            if(window) {
                return window;
            } else {
                return newSubWindow(type);
            }
        }
    }
    return newSubWindow(type);
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
        return newSubWindow(type);
    } else if(activeForm() != nullptr) {
        if(activeForm()->type() != type) {
            QMdiSubWindow *window = nullptr;
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
                return newSubWindow(type);
            }
        } else {
            return activeSubWindow();
        }
    }
    return newSubWindow(type);
}


QMdiSubWindow* WindowManager::hasDictWindow(ModuleTools::DefaultModule d)
{
    foreach(QMdiSubWindow * w, usableWindowList()) {
        Form *f = getForm(w);
        if(f->type() == Form::DictionaryForm) {
            DictionaryForm *form = (DictionaryForm*)(f);
            if(form->dictionary() != nullptr) {
                const int moduleID = form->dictionary()->moduleID();
                if(m_settings->getModuleSettings(moduleID)->defaultModule == d) {
                    return w;
                }
            }
        }
    }
    return nullptr;
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
    return nullptr;
}

void WindowManager::activate(QMdiSubWindow *w)
{
    w->activateWindow();
    m_area->setActiveSubWindow(w);
}

void WindowManager::autoLayout()
{
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
    return nullptr;
}

Form * WindowManager::activeForm()
{
    return getForm(activeSubWindow());
}

Form * WindowManager::getForm(QMdiSubWindow *w) const
{
    if(w == nullptr)
        return nullptr;

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
/**
 * @brief  WindowManager::tile is QMdiArea::tile with some features, like a different ordering of the windows, based on the content type
 */
void WindowManager::tile()
{
    if(!m_enableReload || usableWindowList().isEmpty()) {
        return;
    }
    setEnableReload(false);

    QList<QWidget *> widgets;
    QList<QMdiSubWindow*> subWindows = usableWindowList();
    myDebug() << subWindows;
    QSize minSubWindowSize;

    if(m_settings->intelligentLayout) {
        QMultiMap<int, QMdiSubWindow*> m;
        foreach(QMdiSubWindow *c, subWindows) {
            int id;
            Form::FormRole role = getForm(c)->role();
            switch(role) {
                case Form::MainBibleRole:
                    id = 0;
                    break;
                case Form::SecondBibleRole:
                    id = 1;
                    break;
                case Form::ThirdBibleRole:
                    id = 2;
                    break;
                case Form::BibleRole:
                    id = 3;
                    break;
                case Form::DictionaryRole:
                    id = 4;
                    break;
                case Form::CommentaryRole:
                    id = 5;
                    break;
                default:
                    id = 6;
                    break;
            }
            m.insert(id, c);
        }
        subWindows.clear();
        QMapIterator<int, QMdiSubWindow*> it(m);
        while (it.hasNext()) {
             it.next();
             subWindows.append(it.value());
        }
    }


    foreach(QMdiSubWindow *child, subWindows) {

        if(child->isMinimized() && !child->isShaded())
            continue;

        if (child->isMaximized() || child->isShaded())
            child->showNormal();

        minSubWindowSize = minSubWindowSize.expandedTo(child->minimumSize()).expandedTo(child->minimumSize());
        widgets.append(child);
    }
    QRect domain = m_area->viewport()->rect();
    domain = resizeToMinimumTileSize(minSubWindowSize, widgets.count());

    const int n = widgets.size();
    const int ncols = qMax(qCeil(qSqrt(qreal(n))), 1);
    const int nrows = qMax((n % ncols) ? (n / ncols + 1) : (n / ncols), 1);
    const int nspecial = (n % ncols) ? (ncols - n % ncols) : 0;
    const int dx = domain.width()  / ncols;
    const int dy = domain.height() / nrows;

    int i = 0;

    for (int row = 0; row < nrows; ++row) {
        const int y1 = int(row * (dy + 1));
        for (int col = 0; col < ncols; ++col) {
            if (row == 1 && col < nspecial)
                continue;

            const int x1 = int(col * (dx + 1));
            int x2 = int(x1 + dx);
            int y2 = int(y1 + dy);

            if (row == 0 && col < nspecial) {
                y2 *= 2;
                if (nrows != 2)
                    y2 += 1;
                else
                    y2 = domain.bottom();
            }

            if (col == ncols - 1 && x2 != domain.right())
                x2 = domain.right();

            if (row == nrows - 1 && y2 != domain.bottom())
                y2 = domain.bottom();


            QWidget *widget = widgets.at(i++);
            QRect newGeometry = QRect(QPoint(x1, y1), QPoint(x2, y2));
            widget->setGeometry(QStyle::visualRect(widget->layoutDirection(), domain, newGeometry));

        }

    }
    //myDebug() << usableWindowList();
    //m_area->updateScrollBars();
    setEnableReload(true);
}

QRect WindowManager::resizeToMinimumTileSize(const QSize &minSubWindowSize, int subWindowCount)
{

    if (!minSubWindowSize.isValid() || subWindowCount <= 0)
        return m_area->viewport()->rect();

    // Calculate minimum size.

    const int columns = qMax(qCeil(qSqrt(qreal(subWindowCount))), 1);
    const int rows = qMax((subWindowCount % columns) ? (subWindowCount / columns + 1)
                                                     : (subWindowCount / columns), 1);

    const int minWidth = minSubWindowSize.width() * columns;
    const int minHeight = minSubWindowSize.height() * rows;

    // Increase area size if necessary. Scroll bars are provided if we're not able

    // to resize to the minimum size.
    bool tileCalledFromResizeEvent = true;

    if (!tileCalledFromResizeEvent) {

        QWidget *topLevel = m_area;

        // Find the topLevel for this area, either a real top-level or a sub-window.

        while (topLevel && !topLevel->isWindow() && topLevel->windowType() != Qt::SubWindow)

            topLevel = topLevel->parentWidget();

        // We don't want sub-subwindows to be placed at the edge, thus add 2 pixels.

        int minAreaWidth = minWidth /*+ left + right */+ 2;
        int minAreaHeight = minHeight /*+ top + bottom */+ 2;

        if (m_area->horizontalScrollBar()->isVisible())
            minAreaHeight += m_area->horizontalScrollBar()->height();

        if (m_area->verticalScrollBar()->isVisible())
            minAreaWidth += m_area->verticalScrollBar()->width();

        if (m_area->style()->styleHint(QStyle::SH_ScrollView_FrameOnlyAroundContents, 0, m_area)) {
            const int frame = m_area->style()->pixelMetric(QStyle::PM_DefaultFrameWidth, 0, m_area);
            minAreaWidth += 2 * frame;
            minAreaHeight += 2 * frame;

        }

        const QSize diff = QSize(minAreaWidth, minAreaHeight).expandedTo(m_area->size()) - m_area->size();
        topLevel->resize(topLevel->size() + diff);

    }

    QRect domain = m_area->viewport()->rect();

    // Adjust domain width and provide horizontal scroll bar.

    if (domain.width() < minWidth) {
        domain.setWidth(minWidth);
        if (m_area->horizontalScrollBarPolicy() == Qt::ScrollBarAlwaysOff)
            m_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        else
            m_area->horizontalScrollBar()->setValue(0);

    }

    // Adjust domain height and provide vertical scroll bar.

    if (domain.height() < minHeight) {
        domain.setHeight(minHeight);
        if (m_area->verticalScrollBarPolicy()  == Qt::ScrollBarAlwaysOff)
            m_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        else
            m_area->verticalScrollBar()->setValue(0);

    }

    return domain;

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
    //DEBUG_FUNC_NAME
	if(activeSubWindow())
		activeSubWindow()->close();
}

int WindowManager::closingWindow()
{
    if(m_area->subWindowList().isEmpty()) {
        m_actions->clearBooks();
        m_actions->clearChapters();
        return 1;
    }
  /*  myDebug() << "reload window";
    reloadWindow(m_area->currentSubWindow());*/

    if(m_area->viewMode() == QMdiArea::SubWindowView) {
        QTimer::singleShot(10, this, SLOT(autoLayout()));
    }

    return 0;
}

int WindowManager::reloadWindow(QMdiSubWindow * window)
{
    if(!m_enableReload || window == nullptr) {
        return 1;
    }

    if(m_area->subWindowList().isEmpty()) {
        return 1;
    }

    Form *form = this->getForm(window);

    if(form == nullptr) {
        myWarning() << "form == nullptr";
        return 1;
    }

    if(form->moduleID() != ModuleIDNotSet && m_settings->getModuleSettings(form->moduleID()) == nullptr) {
        window->close();
        myWarning() << "activiated an not existing module";
        return 1;
    }

    *m_currentWindowID = form->id();
    form->activated();
    m_area->setActiveSubWindow(window);

    return 0;
}

void WindowManager::mdiAreaResized()
{
    if(m_area->viewMode() == QMdiArea::SubWindowView) {
        const QMdiSubWindow * w = m_area->activeSubWindow();
        if(w != nullptr && w->isMaximized()) {
            return;
        } else {
            QTimer::singleShot(1, this, SLOT(autoLayout()));
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
        }  else if(type == "tbook") {
            t = Form::TreeBookForm;
        } else if(type == "console") {
            t = Form::ConsoleForm;
        }
        QMdiSubWindow *w = newSubWindow(t, max);

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

ModuleTools::ContentType WindowManager::contentType(QMdiSubWindow* window)
{
    if(window == nullptr)
        return ModuleTools::UnkownContent;

    Form *f = getForm(window);

    if(f == nullptr)
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
        if(s != nullptr)
            return s->contentType;
    }
    return ModuleTools::UnkownContent;
}

ModuleTools::ContentType WindowManager::contentType(WebForm *form)
{
    Q_UNUSED(form);
	return ModuleTools::UnkownContent;
}

ModuleTools::ContentType WindowManager::contentType(BibleForm *form)
{
    if(form) {
        ModuleSettings *s = m_settings->getModuleSettings(form->verseModule()->moduleID());
        if(s != nullptr)
            return s->contentType;
    }
    return ModuleTools::UnkownContent;
}

void WindowManager::closeBadWindows()
{
    foreach(QMdiSubWindow * w, m_area->subWindowList(QMdiArea::ActivationHistoryOrder)) {
        Form *form = this->getForm(w);
        if(form->moduleID() == ModuleIDNotSet) continue;
        if(m_settings->getModuleSettings(form->moduleID()) == nullptr) {
            w->close();
        }
    }
}
