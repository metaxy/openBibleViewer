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
#include "src/core/dbghelper.h"
#include "src/ui/dialog/searchdialog.h"
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
#include <QtCore/QTimer>
#include <QtGui/QColorDialog>
#include <QtGui/QKeySequence>
#include "src/ui/dialog/aboutdialog.h"
#include "src/ui/noteseditor.h"
#include "src/ui/marklist.h"
#include "src/core/core.h"
AdvancedInterface::AdvancedInterface(QWidget *parent) :
        Interface(parent),
        ui(new Ui::AdvancedInterface)
{
    ui->setupUi(this);
}
void AdvancedInterface::setBookDockWidget(BookDockWidget *bookDockWidget)
{
    m_bookDockWidget = bookDockWidget;
}
void AdvancedInterface::setModuleDockWidget(ModuleDockWidget *moduleDockWidget)
{
    m_moduleDockWidget = moduleDockWidget;
}
void AdvancedInterface::setSearchResultDockWidget(SearchResultDockWidget *searchResultDockWidget)
{
    m_searchResultDockWidget = searchResultDockWidget;
}
void AdvancedInterface::setNotesDockWidget(NotesDockWidget *notesDockWidget)
{
    m_notesDockWidget = notesDockWidget;
}
void AdvancedInterface::setBookmarksDockWidget(BookmarksDockWidget *bookmarksDockWidget)
{
    m_bookmarksDockWidget = bookmarksDockWidget;
}
void AdvancedInterface::setStrongDockWidget(StrongDockWidget *strongDockWidget)
{
    m_strongDockWidget = strongDockWidget;
}
void AdvancedInterface::setQuickJumpDockWidget(QuickJumpDockWidget *quickJumpDockWidget)
{
    m_quickJumpDockWidget = quickJumpDockWidget;
}
void AdvancedInterface::init()
{
    m_bibleDisplaySettings = new BibleDisplaySettings();
    m_bibleDisplaySettings->loadNotes = true;
    m_bibleDisplaySettings->showMarks = true;
    m_bibleDisplaySettings->showNotes = true;

    connect(m_bibleDisplay, SIGNAL(newHtml(QString)), this, SLOT(showText(QString)));
    connect(m_bibleDisplay, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));
    connect(m_bibleDisplay, SIGNAL(get(QUrl)), this, SLOT(pharseUrl(QUrl)));


    setAll(m_moduleDockWidget);
    m_moduleDockWidget->init();
    connect(m_moduleDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    setAll(m_bookDockWidget);
    connect(m_bookDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    setAll(m_searchResultDockWidget);
    m_searchResultDockWidget->hide();
    connect(m_searchResultDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    setAll(m_notesDockWidget);
    m_notesDockWidget->init();
    m_notesDockWidget->hide();
    connect(m_notesDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));
    connect(m_notesDockWidget, SIGNAL(reloadChapter()), this, SLOT(reloadChapter()));

    setAll(m_bookmarksDockWidget);
    m_bookmarksDockWidget->init();
    m_bookmarksDockWidget->hide();
    connect(m_bookmarksDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    setAll(m_strongDockWidget);
    m_strongDockWidget->init();
    m_strongDockWidget->hide();
    connect(m_strongDockWidget, SIGNAL(get(QUrl)), this, SLOT(pharseUrl(QUrl)));

    setAll(m_quickJumpDockWidget);
    m_quickJumpDockWidget->init();
    m_quickJumpDockWidget->hide();
    connect(m_quickJumpDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));


    connect(this, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    m_moduleManager->setBibleDisplaySettings(m_bibleDisplaySettings);

    m_bibleApi = new BibleApi();
    setAll(m_bibleApi);

    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(reloadWindow(QMdiSubWindow *)));

    if (usableWindowList().size() == 0 &&  m_settings->session.getData("windowUrls").toStringList().size() == 0)
        QTimer::singleShot(0, this, SLOT(newSubWindow()));

}

void AdvancedInterface::attachApi()
{
    QFile file(":/data/js/jquery-1.4.2.min.js");
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return;
    QTextStream stream(&file);
    QString jquery = stream.readAll();
    file.close();

    QWebFrame * frame = getView()->page()->mainFrame();
    frame->addToJavaScriptWindowObject("Bible",m_bibleApi);
    frame->evaluateJavaScript(jquery);
    m_bibleApi->setFrame(frame);
}

void AdvancedInterface::newSubWindow(bool doAutoLayout)
{
    DEBUG_FUNC_NAME
    m_enableReload = false;
    int windowsCount = usableWindowList().size();
    QMdiSubWindow *firstSubWindow = new QMdiSubWindow();
    if (windowsCount == 1) {
        firstSubWindow = usableWindowList().at(0);
    }

    m_windowCache.newWindow();


    QWidget *widget = new QWidget(ui->mdiArea);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    MdiForm *mForm = new MdiForm(widget);
    setAll(mForm);
    layout->addWidget(mForm);

    widget->setLayout(layout);
    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(widget);
    subWindow->setWindowIcon(QIcon(":/icons/16x16/main.png"));
    subWindow->setWindowOpacity(1.0);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();
    ui->mdiArea->setActiveSubWindow(subWindow);
    attachApi();

    if(ui->mdiArea->viewMode() == QMdiArea::SubWindowView) {
        if (windowsCount == 0  && doAutoLayout) {
            subWindow->showMaximized();
        } else if (windowsCount == 1 && doAutoLayout) {
            firstSubWindow->resize(600, 600);
            firstSubWindow->showNormal();
            subWindow->resize(600, 600);
            subWindow->show();
        } else if (doAutoLayout) {
            subWindow->resize(600, 600);
            subWindow->show();
        }
    }
    connect(mForm->m_ui->comboBox_books, SIGNAL(activated(int)), this, SLOT(readBook(int)));
    connect(mForm->m_ui->comboBox_chapters, SIGNAL(activated(int)), this, SLOT(readChapter(int)));
    //todo: webview
    //connect(mForm->m_ui->textBrowser, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(textBrowserContextMenu(QPoint)));
    connect(mForm->m_view->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(pharseUrl(QUrl)));
    connect(getView()->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attachApi()) );

    connect(mForm, SIGNAL(historyGo(QString)), this, SLOT(pharseUrl(QString)));
    connect(mForm, SIGNAL(previousChapter()), this, SLOT(previousChapter()));
    connect(mForm, SIGNAL(nextChapter()), this, SLOT(nextChapter()));
    connect(this, SIGNAL(historySetUrl(QString)), mForm, SLOT(historyGetUrl(QString)));
    connect(subWindow, SIGNAL(destroyed(QObject*)), this, SLOT(closingWindow()));
    m_enableReload = true;
    if (doAutoLayout && ui->mdiArea->viewMode() == QMdiArea::SubWindowView) {
        autoLayout();
    }
    m_internalWindows << subWindow;

    m_moduleManager->m_bible = new Bible();
    m_moduleManager->initBible();
    m_moduleManager->m_bibleList = new BibleList();

    m_moduleManager->bibleList()->addBible(m_moduleManager->m_bible,QPoint(0,0));
    m_windowCache.setBibleList(m_moduleManager->m_bibleList);
    //clear old stuff
    setBooks(QStringList());
    setChapters(QStringList());

}
void AdvancedInterface::autoLayout()
{
    DEBUG_FUNC_NAME
    if (usableWindowList().size() > 1) {
        if (m_settings->autoLayout == 1) {
            myTile();
        } else if (m_settings->autoLayout == 2) {
            myTileVertical();
        } else if (m_settings->autoLayout == 3) {
            myTileHorizontal();
        } else if (m_settings->autoLayout == 4) {
            myCascade();
        }
    }
}

QMdiSubWindow * AdvancedInterface::activeMdiChild()
{
    QList<QMdiSubWindow*> list = usableWindowList();
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow()) {
        for (int i = 0; i < list.size(); i++) {
            if (list.at(i) == activeSubWindow) {
                m_lastActiveWindow = i;
            }
        }
        return activeSubWindow;
    } else if (m_lastActiveWindow < list.size()) {
        ui->mdiArea->setActiveSubWindow(usableWindowList().at(m_lastActiveWindow));
        if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
            return activeSubWindow;
    } else if (usableWindowList().count() > 0) {
        ui->mdiArea->setActiveSubWindow(usableWindowList().at(0));
        if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
            return activeSubWindow;
    }
    return 0;
}
void AdvancedInterface::myTileVertical()
{
    DEBUG_FUNC_NAME
    if (!m_enableReload || !usableWindowList().count()) {
        return;
    }
    QList<QMdiSubWindow*> windows = usableWindowList();
    m_enableReload = false;
    QMdiSubWindow* active = ui->mdiArea->activeSubWindow();

    const int widthForEach = width() / windows.count();
    unsigned int x = 0;
    foreach(QMdiSubWindow *window, windows) {
        window->showNormal();

        const int preferredWidth = window->minimumWidth() + window->baseSize().width();
        const int actWidth = qMax(widthForEach, preferredWidth);

        window->setGeometry(x, 0, actWidth, height());
        x += actWidth;
    }

    if (active) active->setFocus();
    m_enableReload = true;
}

void AdvancedInterface::myTileHorizontal()
{
    DEBUG_FUNC_NAME
    if (!m_enableReload || !usableWindowList().count()) {
        return;
    }
    QList<QMdiSubWindow*> windows = usableWindowList();
    m_enableReload = false;
    QMdiSubWindow* active = ui->mdiArea->activeSubWindow();

    const int heightForEach = height() / windows.count();
    unsigned int y = 0;
    foreach(QMdiSubWindow *window, windows) {
        window->showNormal();

        const int preferredHeight = window->minimumHeight() + window->baseSize().height();
        const int actHeight = qMax(heightForEach, preferredHeight);

        window->setGeometry(0, y, width(), actHeight);
        y += actHeight;
    }
    if (active)
        active->setFocus();
    m_enableReload = true;

}

void AdvancedInterface::myCascade()
{
    DEBUG_FUNC_NAME
    if (!m_enableReload || !usableWindowList().count()) {
        return;
    }

    QList<QMdiSubWindow*> windows = usableWindowList();

    if (ui->mdiArea->activeSubWindow() && ui->mdiArea->activeSubWindow()->isMaximized()) {
        if (ui->mdiArea->activeSubWindow()->size() != this->size()) {
            ui->mdiArea->activeSubWindow()->resize(this->size());
        }
    } else if (windows.count() == 1) {
        windows.at(0)->showMaximized();
    } else {

        QMdiSubWindow* active = ui->mdiArea->activeSubWindow();
        m_enableReload = false;
        const unsigned int offsetX = 40;
        const unsigned int offsetY = 40;
        const unsigned int windowWidth =  ui->mdiArea->width() - (windows.count() - 1) * offsetX;
        const unsigned int windowHeight = ui->mdiArea->height() - (windows.count() - 1) * offsetY;
        unsigned int x = 0;
        unsigned int y = 0;

        foreach(QMdiSubWindow* window, windows) {
            if (window == active)//leave out the active window which should be the top window
                continue;
            window->raise(); //make it the on-top-of-window-stack window to make sure they're in the right order
            window->setGeometry(x, y, windowWidth, windowHeight);
            x += offsetX;
            y += offsetY;
        }
        active->setGeometry(x, y, windowWidth, windowHeight);
        active->raise();
        active->activateWindow();
        m_enableReload = true;
    }
}

void AdvancedInterface::myTile()
{
    if (!m_enableReload || !usableWindowList().count()) {
        return;
    }
    ui->mdiArea->tileSubWindows();
}


QList<QMdiSubWindow*> AdvancedInterface::usableWindowList()
{
    //only if !ChildAdded-Event is triggered
    QList<QMdiSubWindow*> ret;
    foreach(QMdiSubWindow* w, ui->mdiArea->subWindowList()) {
        if (w->isHidden())
            continue;
        ret.append(w);
    }
    // return ret;
    return  ret;
}
int AdvancedInterface::currentWindowID()
{
    for (int i = 0; i < ui->mdiArea->subWindowList().count(); i++) {
        if (ui->mdiArea->activeSubWindow() ==  ui->mdiArea->subWindowList().at(i)) {
            return i;
        }
    }
    return -1;
}
int AdvancedInterface::tabIDof(QMdiSubWindow* window)
{
    for (int i = 0; i < ui->mdiArea->subWindowList().count(); i++) {
        if (window ==  ui->mdiArea->subWindowList().at(i)) {
            return i;
        }
    }
    return -1;
}
void AdvancedInterface::closeSubWindow()
{
    activeMdiChild()->close();
}
int AdvancedInterface::closingWindow()
{
    //DEBUG_FUNC_NAME
    if (!m_enableReload) {
        myDebug() << "reload is not enabled";
        return 1;
    }
    //if one in the internal subwindow list list is missing that window was closed
    if (ui->mdiArea->subWindowList().isEmpty()) {
        myDebug() << "subWindowList is empty";
        setBooks(QStringList());
        setChapters(QStringList());
        m_windowCache.clearAll();
        return 1;
    }
    if (m_internalWindows.isEmpty()) {
        myDebug() << "internaL is empty";
        setBooks(QStringList());
        setChapters(QStringList());
        m_windowCache.clearAll();
        return 1;
    }
    for (int i = 0; i < m_internalWindows.size(); i++) {
        if (ui->mdiArea->subWindowList().lastIndexOf(m_internalWindows.at(i)) == -1) {
            myDebug() << "found closed Window id = " << i;
            m_windowCache.removeWindow(i);
            m_internalWindows.removeAt(i);
            break;
        }
    }
    if (ui->mdiArea->subWindowList().isEmpty()) { //last window closed
        myDebug() << "last closed";
        setBooks(QStringList());
        setChapters(QStringList());
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

    if (!m_enableReload) {
        return 1;
    }
    int id = tabIDof(window);
    if (id == -1) {
        return 1;
    }
    if (ui->mdiArea->subWindowList().count() <= 0) {
        return 1;
    }


    m_windowCache.setCurrentWindowID(id);

    //todo: add last active window and if it is the same do nohting
    if(!m_windowCache.getBibleList()) {
        setChapters(QStringList());
        setBooks(QStringList());
        return 1;
    }
    if(!m_windowCache.getBibleList()->bible()) {
        setChapters(QStringList());
        setBooks(QStringList());
        return 1;
    }
    if(m_windowCache.getBibleList()->bible()->moduleID() < 0) {
        setChapters(QStringList());
        setBooks(QStringList());
        return 1;
    }
    //todo: think about it
   /* if (m_moduleManager->bibleList()->bible()->moduleID() == m_windowCache.getBibleList()->bible()->moduleID())
        return 1;*/
    m_moduleManager->m_bibleList = m_windowCache.getBibleList();
    m_moduleManager->m_bible = m_moduleManager->m_bibleList->bible();
    setTitle(m_moduleManager->bible()->bibleTitle());

    setChapters(m_moduleManager->bible()->chapterNames());

    setCurrentChapter(m_moduleManager->bible()->chapterID());

    setBooks(m_moduleManager->bible()->bookFullName());
    setCurrentBook(m_moduleManager->bible()->bookID());
    m_moduleDockWidget->loadedModule(m_moduleManager->bible()->moduleID());

    return 0;
}

void AdvancedInterface::loadModuleDataByID(int id)
{
    DEBUG_FUNC_NAME
    myDebug() << "id = " << id;
    if (ui->mdiArea->subWindowList().size() == 0)
        newSubWindow();
    if (id < 0 || !m_moduleManager->contains(id)) {
        myDebug() << "id = " << id << m_moduleManager->m_moduleMap->m_map;
        return;
    }
    if (m_moduleManager->getModule(id)->m_moduleClass != Module::BibleModule) {
        myDebug() << "non bible module";
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);

    m_windowCache.setCurrentWindowID(currentWindowID());
    //m_windowCache.setBibleList(m_moduleManager->bibleList());

    Module::ModuleType type = m_moduleManager->getModule(id)->m_moduleType;
    m_moduleManager->bible()->setBibleType(type);
    m_moduleManager->bible()->loadModuleData(id);


    setTitle(m_moduleManager->bible()->bibleTitle());
    setBooks(m_moduleManager->bible()->bookFullName());
    m_moduleDockWidget->loadedModule(id);//select current Module
    /*myDebug() <<"style = " <<  m_settings->getModuleSettings(id).styleSheet;
       QTextBrowser *textBrowser = getTextBrowser();
    QFile file(m_settings->getModuleSettings(id).styleSheet);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        textBrowser->document()->setDefaultStyleSheet(in.readAll());//set stylesheet
    } else {
        myDebug() << "can not open stylesheet = " << file.errorString();
    }*/

    QApplication::restoreOverrideCursor();

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
    const QString http = "http://";
    const QString bq = "go";
    const QString anchor = "#";
    const QString note = "note://";
    const QString persistent = "persistent:";
    if (url.startsWith(bible)) {
        url = url.remove(0, bible.size());
        if(url == "current") {
            reloadChapter(true);
        } else if(url == "reloadActive") {
            reloadActive();
        } else {
            QStringList a = url.split("/");
            if (a.size() == 2) {
                QStringList c = a.at(1).split(",");
                if (c.size() >= 3) {
                    int bibleID;
                    if (a.at(0) == "current") {
                        bibleID = m_moduleManager->bible()->moduleID();
                    } else {
                        bibleID = a.at(0).toInt();
                    }
                    int bookID = c.at(0).toInt();
                    int chapterID = c.at(1).toInt();
                    int verseID = c.at(2).toInt();
                    if (bookID < 0 || chapterID < 0 || verseID < 0) {
                        myDebug() << "invalid url";
                        return;
                    }
                    bool force = false;
                    if (c.size() == 4 && c.at(3) == "force=true") {
                        force = true;
                    }
                    if (bibleID != m_moduleManager->bible()->moduleID() || !m_moduleManager->bible()->loaded() || force) {
                        loadModuleDataByID(bibleID);
                        readBookByID(bookID);
                        setCurrentBook(bookID);
                        showChapter(chapterID + m_moduleManager->bible()->chapterAdd(), verseID);
                        setCurrentChapter(chapterID);
                        //load bible
                    } else if (bookID != m_moduleManager->bible()->bookID()) {
                        readBookByID(bookID);
                        setCurrentBook(bookID);
                        showChapter(chapterID + m_moduleManager->bible()->chapterAdd(), verseID);
                        setCurrentChapter(chapterID);
                        //load book
                    } else if (chapterID != m_moduleManager->bible()->chapterID()) {
                        showChapter(chapterID + m_moduleManager->bible()->chapterAdd(), verseID);
                        setCurrentChapter(chapterID);
                        //load chapter
                    } else {
                        showChapter(chapterID + m_moduleManager->bible()->chapterAdd(), verseID);
                        setCurrentChapter(chapterID);
                    }
                    if (c.size() == 4 && c.at(3) == "searchInCurrentText=true") {//todo: not nice
                        searchInText(m_moduleManager->bible()->m_lastSearchQuery);
                    }
                    emit historySetUrl(url_backup);
                } else if(a.at(1) == "current") {
                    //load another bible but with current book and chapter id
                }
                else {
                    myDebug() << "invalid URL";
                }
            } else {
                myDebug() << "invalid URL";
            }
        }
        //bible://bibleID/bookID,chapterID,verseID
    } else if (url.startsWith(strong)) {
        url = url.remove(0, strong.size());
        m_strongDockWidget->showStrong(url);
        //strong://strongID
    } else if (url.startsWith(http)) {
        QDesktopServices::openUrl(url);
        //its a web link
    } else if (url.startsWith(bq)) {
        //its a biblequote internal link, but i dont have the specifications!!!
        QStringList internal = url.split(" ");
        QString bibleID = internal.at(1);//todo: use it
        int bookID = internal.at(2).toInt() - 1;
        int chapterID = internal.at(3).toInt() - 1;
        int verseID = internal.at(4).toInt();
        /*if(bibleID != m_moduleManager->bible()->bibleID())
        {
            loadModuleDataByID(bibleID);
            readBookByID(bookID);
            setCurrentBook(bookID);
            showChapter(chapterID+m_moduleManager->bible()->chapterAdd(),verseID);
            setCurrentChapter(chapterID);
            //load bible
        }
        else */if (bookID != m_moduleManager->bible()->bookID())
        {
            readBookByID(bookID);
            setCurrentBook(bookID);
            showChapter(chapterID + m_moduleManager->bible()->chapterAdd(), verseID);
            setCurrentChapter(chapterID);
            //load book
        } else if (chapterID != m_moduleManager->bible()->chapterID()) {
            showChapter(chapterID + m_moduleManager->bible()->chapterAdd(), verseID);
            setCurrentChapter(chapterID);
            //load chapter
        } else {
            showChapter(chapterID + m_moduleManager->bible()->chapterAdd(), verseID);
            setCurrentChapter(chapterID);
        }
        emit historySetUrl(url_backup);

    } else if (url.startsWith(anchor)) {
        url = url.remove(0, anchor.size());
        bool ok;
        int c = url.toInt(&ok, 10);
        if (ok && c < m_moduleManager->bible()->chaptersCount() && m_moduleManager->bible()->bibleType() == Module::BibleQuoteModule && m_moduleManager->bible()->chapterID() != c) {
            showChapter(c + m_moduleManager->bible()->chapterAdd(), 0);
            setCurrentChapter(c);
        } else {
            myDebug() << "anchor";
            if (activeMdiChild()) {
                //todo: webview
                /*QTextBrowser *textBrowser = getTextBrowser();
                textBrowser->scrollToAnchor(url);*/
            }
        }

    } else if (url.startsWith(note)) {
        url = url.remove(0, note.size());
        if (!m_notesDockWidget->isVisible()) {
            m_notesDockWidget->show();
        }
        m_notesDockWidget->showNote(url);
    } else if (url.startsWith(persistent)) {
        url = url.remove(0, persistent.size());
        UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
        urlConverter.setModuleMap(m_moduleManager->m_moduleMap);
        urlConverter.pharse();
        QString i = urlConverter.convert();//it now a normal interface url
        pharseUrl(i);
    } else {
        if (m_moduleManager->bible()->bibleType() == Module::BibleQuoteModule && m_moduleManager->bible()->bookPath().contains(url)) {
            emit get("bible://current/" + m_moduleManager->bible()->bookPath().lastIndexOf(url));//search in bible bookPath for this string, if it exixsts it is a book link
        } else {
            getView()->page()->mainFrame()->evaluateJavaScript(url);
            myDebug() << "invalid URL 1";
        }
    }
    setEnableReload(true);
    return;
}
QWebView* AdvancedInterface::getView()
{
    if (activeMdiChild()) {
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
    if (v) {
        v->setZoomFactor(v->zoomFactor()+0.1);
    }

}
void AdvancedInterface::zoomOut()
{
    QWebView *v = getView();
    if (v) {
        v->setZoomFactor(v->zoomFactor()-0.1);
    }
}
void AdvancedInterface::showText(const QString &text)
{
    QWebView *v = getView();
    if (v) {
        QString cssFile = m_settings->getModuleSettings(m_moduleManager->bible()->moduleID()).styleSheet;
        if(cssFile.isEmpty())
            cssFile = ":/data/css/default.css";

        QFile file(cssFile);
        if(file.open(QFile::ReadOnly))
            v->settings()->setUserStyleSheetUrl(QUrl("data:text/css;charset=utf-8;base64," + file.readAll().toBase64()));

        v->setHtml(text);
        if (m_moduleManager->bible()->verseID() > 1)
             v->page()->mainFrame()->evaluateJavaScript("window.location.href = '#currentVerse';");
    }
}
void AdvancedInterface::setTitle(const QString &title)
{
    this->parentWidget()->setWindowTitle(title);//todo: + programmtitle
    if (activeMdiChild()) {
        activeMdiChild()->widget()->setWindowTitle(title);
    }
}

void AdvancedInterface::setChapters(const QStringList &chapters)
{
    //DEBUG_FUNC_NAME
    m_bookDockWidget->setChapters(chapters);
    if (activeMdiChild()) {
        QComboBox *comboBox_chapters = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_chapters");
        if (comboBox_chapters) {
            bool same = true;
            if (comboBox_chapters->count() == chapters.count()) {
                for (int i = 0; i < chapters.count(); i++) {
                    if (comboBox_chapters->itemText(i) != chapters.at(i)) {
                        same = false;
                    }
                }
            } else {
                same = false;
            }
            if (!same) {
                comboBox_chapters->clear();
                comboBox_chapters->insertItems(0, chapters);
            }
        }
    }
}
void AdvancedInterface::setCurrentChapter(const int &chapterID)
{
    //DEBUG_FUNC_NAME
    m_bookDockWidget->setCurrentChapter(chapterID);
    if (activeMdiChild()) {
        QComboBox *comboBox_chapters = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_chapters");
        comboBox_chapters->setCurrentIndex(chapterID);
    }
}

void AdvancedInterface::setBooks(const QStringList &books)
{
    m_bookDockWidget->setBooks(books);
    m_quickJumpDockWidget->setBooks(books);

    if (activeMdiChild()) {
        QComboBox *comboBox_books = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_books");
        if (comboBox_books) {
            bool same = true;
            if (comboBox_books->count() == books.count()) {
                for (int i = 0; i < books.count(); i++) {
                    if (comboBox_books->itemText(i) != books.at(i)) {
                        same = false;
                    }
                }
            } else {
                same = false;
            }
            if (!same) {
                comboBox_books->clear();
                comboBox_books->insertItems(0, books);
            }
        }
    }
}

void AdvancedInterface::setCurrentBook(const int &bookID)
{
    m_bookDockWidget->setCurrentBook(bookID);
    if (activeMdiChild()) {
        QComboBox *comboBox_books = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_books");
        if (comboBox_books) {
            comboBox_books->setCurrentIndex(bookID);
        }

    }
}


void AdvancedInterface::readBook(const int &id)
{
    myDebug() << "id = " << id;
    emit get("bible://current/" + QString::number(id) + ",0,0");
}
void AdvancedInterface::readBookByID(int id)
{
    myDebug() << "id = " << id;
    //todo: webview
    QWebView *v = getView();
    if (v) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        if (id < 0) {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(0, tr("Error"), tr("This book is not available."));
            myDebug() << "invalid bookID - 1";
            return;
        }
        if (id >= m_moduleManager->bible()->booksCount()) {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(0, tr("Error"), tr("This book is not available."));
            myDebug() << "invalid bookID - 2(no book loaded) id = " << id << " count = " << m_moduleManager->bible()->booksCount();

            return;
        }
        int read = m_moduleManager->bibleList()->readBook(id);
        if (read != 0) {
            QApplication::restoreOverrideCursor();
            if(read == 2) {
                setChapters(QStringList());
            } else {
                QMessageBox::critical(0, tr("Error"), tr("Cannot read the book."));
            }
            myDebug() << "read = " << read;
            //error while reading
            return;
        }
        setChapters(m_moduleManager->bible()->chapterNames());
        //todo: webview
        //todo: make a javascript which search all images finds the right path and change it
        //t->setSearchPaths(m_moduleManager->bible()->getSearchPaths());
        QApplication::restoreOverrideCursor();
    }

}
void AdvancedInterface::readChapter(const int &id)
{
    emit get("bible://current/" + QString::number(m_moduleManager->bible()->bookID()) + "," + QString::number(id) + ",0");
}

void AdvancedInterface::showChapter(const int &chapterID, const int &verseID)
{
    m_bibleDisplay->setHtml(m_moduleManager->bibleList()->readChapter(chapterID, verseID));
    setCurrentChapter(chapterID - m_moduleManager->bible()->chapterAdd());
}
void AdvancedInterface::nextChapter()
{
    //DEBUG_FUNC_NAME
    if (m_moduleManager->bible()->chapterID() < m_moduleManager->bible()->chaptersCount() - 1) {
        readChapter(m_moduleManager->bible()->chapterID() + 1);
    } else if (m_moduleManager->bible()->bookID() < m_moduleManager->bible()->booksCount() - 1) {
        readBook(m_moduleManager->bible()->bookID() + 1);
    }
}
void AdvancedInterface::previousChapter()
{
    //DEBUG_FUNC_NAME
    if (m_moduleManager->bible()->chapterID() > 0) {
        readChapter(m_moduleManager->bible()->chapterID() - 1);
    } else if (m_moduleManager->bible()->bookID() > 0) {
        readBook(m_moduleManager->bible()->bookID() - 1);
        readChapter(m_moduleManager->bible()->chaptersCount() - 1);
    }
}
void AdvancedInterface::reloadChapter(bool full)
{
    DEBUG_FUNC_NAME
    if (!activeMdiChild())
        return;
    //todo: webview

  //  QTextBrowser *textBrowser = getView();

   // int vsliderPosition = textBrowser->verticalScrollBar()->sliderPosition();
  //  int hsliderPosition = textBrowser->horizontalScrollBar()->sliderPosition();
    if (full) {
        loadModuleDataByID(m_moduleManager->bible()->moduleID());

        readBookByID(m_moduleManager->bible()->bookID());
        setCurrentBook(m_moduleManager->bible()->bookID());

        readChapter(m_moduleManager->bible()->chapterID());
        setCurrentChapter(m_moduleManager->bible()->chapterID());
    } else {
        readChapter(m_moduleManager->bible()->chapterID());
    }
   // textBrowser->verticalScrollBar()->setSliderPosition(vsliderPosition);
   // textBrowser->horizontalScrollBar()->setSliderPosition(hsliderPosition);
}
void AdvancedInterface::reloadActive()
{
    DEBUG_FUNC_NAME

    setEnableReload(true);
    reloadWindow(ui->mdiArea->currentSubWindow());
    setEnableReload(false);
}

VerseSelection AdvancedInterface::verseSelectionFromCursor(QTextCursor cursor)
{
    VerseSelection selection;
    selection.endVerse = -1;
    selection.startVerse = -1;

    if (cursor.hasSelection() == true) {
        //an option is to remove allhtml tags and search there
        QString fragment = cursor.selection().toHtml();
        int from = fragment.indexOf("<!--StartFragment-->");
        int to = fragment.indexOf("<!--EndFragment-->");
        fragment.remove(to, fragment.size());
        fragment.remove(0, from + QString("<!--StartFragment-->").size());

        //if fragment starts with a tag remove this
        if (fragment.startsWith("<")) {
            fragment.remove(0, fragment.indexOf(">") + 1);
        }
        if (fragment.endsWith(">")) {
            fragment.remove(fragment.lastIndexOf("<"), fragment.size());
        }
        QStringList chapterData = m_moduleManager->bible()->toUniformHtml(m_moduleManager->bible()->chapterDataList());

        QString text = chapterData.join("");
        int startFragment = text.indexOf(fragment);

        // Do it on one way: this way is more stable but you get only the start verse
        int n_pos = cursor.position();
        int n_counter = 0;
        int n_startVerse;
        QStringList n_chapterData = m_moduleManager->bible()->toUniformHtml(m_moduleManager->bible()->chapterDataList());
        for (int i = 0; i < n_chapterData.size(); ++i) {
            QTextDocument t;
            t.setHtml(chapterData.at(i));
            n_counter += t.toPlainText().size();
            if (n_pos < n_counter) {
                n_startVerse = i;
                break;
            }
        }
        myDebug() << "over cursor.position()(2) = " << n_startVerse;
        bool foundBoth = false;

        while (startFragment != -1) {
            //do we have to do it on another way
            int counter = 0;
            VerseSelection newSelection;
            newSelection.endVerse = -1;
            newSelection.startVerse = -1;
            for (int i = 0; i < chapterData.size(); ++i) {
                counter += chapterData.at(i).size();
                myDebug() << "i = " << i << " counter = " << counter;
                if (newSelection.startVerse == -1 && startFragment < counter) {
                    myDebug() << "setted start";
                    newSelection.startVerse = i;
                    newSelection.posInStartVerse = startFragment - (counter - chapterData.at(i).size()) ;
                    bool notFound = true;
                    int biggest = 6;

                    QString v = chapterData.at(i);

                    if (fragment.size() < 6)//make faster
                        biggest = fragment.size();
                    while (notFound) {
                        for (int s = biggest; s < 100; s++) {
                            QString b = fragment;
                            QString searchString = b.remove(s, fragment.size());
                            //todo: if it starts with a html tag remove that

                            int a1 = v.lastIndexOf(searchString);
                            int a2 = v.indexOf(searchString);
                            if (a1 == a2) {
                                myDebug() << "s = " << s << " searchString = " << searchString;
                                newSelection.shortestStringInStartVerse = searchString;
                                notFound = false;
                                break;
                            }
                        }
                        biggest--;
                        if (biggest == 0) {
                            break;
                        }
                    }
                }
                if (newSelection.endVerse == -1 && (startFragment + fragment.size()  < (counter))) {
                    myDebug() << "setted end";
                    newSelection.endVerse = i;
                    newSelection.posInEndVerse = (startFragment + fragment.size()) - (counter - chapterData.at(i).size()) ;
                    bool notFound = true;
                    int biggest = 6;
                    if (fragment.size() < 6)//make faster
                        biggest = fragment.size();
                    while (notFound) {
                        for (int s = biggest; s < 100/* or some another big value*/; s++) {
                            QString b = fragment;
                            QString searchString = b.remove(0, b.size() - s);
                            //todo: if it starts with a html tag remove that
                            QString v = chapterData.at(i);
                            int a1 = v.lastIndexOf(searchString);
                            int a2 = v.indexOf(searchString);
                            if (a1 == a2) {
                                myDebug() << "s = " << s << " searchString = " << searchString;
                                newSelection.shortestStringInEndVerse = searchString;
                                notFound = false;
                                break;
                            }
                        }
                        biggest--;
                        if (biggest == 0) {
                            break;
                        }
                    }
                    myDebug() << "posInEndverse = " << newSelection.posInEndVerse;
                    break;
                }
            }

            if (newSelection.startVerse == n_startVerse) { //and compare both ways
                myDebug() << "break newSelection.startVerse = " << newSelection.startVerse << " n_startVerse  = " << n_startVerse << " repeat = " << newSelection.repeat;
                selection = newSelection;
                foundBoth = true;
                break;
            }
            selection = newSelection;
            //myDebug() << " start = " << selection.startVerse << " end = " << selection.endVerse << " repeat = " << selection.repeat;
            startFragment = text.indexOf(fragment, startFragment + fragment.size());


        }
        //todo: implement repeat is impossible
        /*  QString v = chapterData.at(selection.startVerse);
          int repeat = 0;
          if(v.indexOf(fragment) != -1 && v.indexOf(fragment,v.indexOf(fragment)+fragment.size())) {
              myDebug() << "is more than one time there";
              //todo: if the shortes string is too short(because the user selected a short string), and there is more than one in the verse, say whicht of it is
              int pos = v.indexOf(fragment);
              myDebug() << "pos = " << pos << " selection.posInStartVerse = " << selection.posInStartVerse;
              if(pos != selection.posInStartVerse) {
                  while(pos != -1) {

                      repeat++;
                      if(pos == selection.posInStartVerse) {
                          break;
                      }
                      pos = v.indexOf(fragment,pos+fragment.size());
                      myDebug() << "pos = " << pos << " repeat = " << repeat;


                  }
              }

          }
          myDebug() << "repeat = " << repeat;
          selection.repeat = repeat;*/

    } else {

        int pos = cursor.position();
        QStringList chapterData = m_moduleManager->bible()->toUniformHtml(m_moduleManager->bible()->chapterDataList());
        int counter = 0;
        for (int i = 0; i < chapterData.size(); ++i) {
            QTextDocument t;
            t.setHtml(chapterData.at(i));
            counter += t.toPlainText().size();
            if (selection.startVerse == -1 && pos < counter) {
                selection.startVerse = i;
                break;
            }
        }
        myDebug() << "over cursor.position() = " << selection.startVerse;
    }
    return selection;
}
int AdvancedInterface::textBrowserContextMenu(QPoint pos)
{
  /*  if (!activeMdiChild())
        return 1;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QMenu *contextMenu = new QMenu(this);
    QAction *actionCopy = new QAction(QIcon::fromTheme("edit-copy",":/icons/16x16/edit-copy.png"), tr("Copy"), contextMenu);
    connect(actionCopy, SIGNAL(triggered()), textBrowser, SLOT(copy()));

    QAction *actionCopyWholeVerse = new QAction(this);
    QTextCursor cursor = textBrowser->cursorForPosition(pos);
    QTextCursor cursor2 = textBrowser->textCursor();
    if (cursor2.hasSelection()) {
        cursor = cursor2;
    }
    m_textCursor = cursor;
    VerseSelection selection = verseSelectionFromCursor(cursor);
    if (selection.startVerse != -1) {
        QString addText;
        if (selection.startVerse != selection.endVerse)
            addText = " " + QString::number(selection.startVerse) + " - " + QString::number(selection.endVerse);
        else
            addText = " " + QString::number(selection.startVerse);
        if (selection.startVerse < 0 || selection.endVerse <= 0) {
            actionCopyWholeVerse->setText(tr("Copy Verse"));
            actionCopyWholeVerse->setEnabled(false);
        } else {
            actionCopyWholeVerse->setText(tr("Copy Verse %1").arg(addText));
            actionCopyWholeVerse->setEnabled(true);
            connect(actionCopyWholeVerse, SIGNAL(triggered()), this , SLOT(copyWholeVerse()));
        }
    } else {
        actionCopyWholeVerse->setText(tr("Copy Verse"));
        actionCopyWholeVerse->setEnabled(false);
    }

    QAction *actionSelect = new QAction(QIcon::fromTheme("edit-select-all",":/icons/16x16/edit-select-all.png"), tr("Select All"), contextMenu);
    connect(actionSelect, SIGNAL(triggered()), textBrowser , SLOT(selectAll()));

    QMenu *menuMark = new QMenu(this);

    menuMark->setTitle(tr("Mark this"));
    QIcon markIcon;
    markIcon.addPixmap(QPixmap(":/icons/16x16/format-fill-color.png"), QIcon::Normal, QIcon::Off);
    menuMark->setIcon(markIcon);
    //connect(actionMark, SIGNAL(triggered()), this , SLOT(newMark()));
    if (!cursor.hasSelection()) {
        menuMark->setEnabled(false);
    }

    QAction *actionYellowMark = new QAction(QIcon(":/icons/16x16/mark-yellow.png"), tr("Yellow"), menuMark);
    connect(actionYellowMark, SIGNAL(triggered()), this , SLOT(newYellowMark()));
    menuMark->addAction(actionYellowMark);

    QAction *actionGreenMark = new QAction(QIcon(":/icons/16x16/mark-green.png"), tr("Green"), menuMark);
    connect(actionGreenMark, SIGNAL(triggered()), this , SLOT(newGreenMark()));
    menuMark->addAction(actionGreenMark);

    QAction *actionBlueMark = new QAction(QIcon(":/icons/16x16/mark-blue.png"), tr("Blue"), menuMark);
    connect(actionBlueMark, SIGNAL(triggered()), this , SLOT(newBlueMark()));
    menuMark->addAction(actionBlueMark);

    QAction *actionOrangeMark = new QAction(QIcon(":/icons/16x16/mark-orange.png"), tr("Orange"), menuMark);
    connect(actionOrangeMark, SIGNAL(triggered()), this , SLOT(newOrangeMark()));
    menuMark->addAction(actionOrangeMark);

    QAction *actionVioletMark = new QAction(QIcon(":/icons/16x16/mark-violet.png"), tr("Violet"), menuMark);
    connect(actionVioletMark, SIGNAL(triggered()), this , SLOT(newVioletMark()));
    menuMark->addAction(actionVioletMark);

    QAction *actionCustomMark  = new QAction(QIcon(":/icons/16x16/format-fill-color.png"), tr("Custom Color"), menuMark);
    connect(actionCustomMark, SIGNAL(triggered()), this , SLOT(newCustomMark()));
    menuMark->addAction(actionCustomMark);

    QAction *actionRemoveMark = new QAction(this);
    actionRemoveMark->setText(tr("Remove Mark"));


    QIcon removeMarkIcon;
    removeMarkIcon.addPixmap(QPixmap(":/icons/16x16/mark-yellow.png"), QIcon::Normal, QIcon::Off);
    actionRemoveMark->setIcon(removeMarkIcon);



    connect(actionRemoveMark, SIGNAL(triggered()), this , SLOT(removeMark()));

    QAction *actionBookmark = new QAction(QIcon::fromTheme("bookmark-new",":/icons/16x16/bookmark-new.png"), tr("Add Bookmark"), contextMenu);
    connect(actionBookmark, SIGNAL(triggered()), this , SLOT(newBookmark()));

    QAction *actionNote = new QAction(QIcon::fromTheme("view-pim-notes",":/icons/16x16/view-pim-notes.png"), tr("Add Note"), contextMenu);
    connect(actionNote, SIGNAL(triggered()), this , SLOT(newNoteWithLink()));

    contextMenu->addAction(actionCopy);
    contextMenu->addAction(actionCopyWholeVerse);
    contextMenu->addAction(actionSelect);
    contextMenu->addSeparator();
    contextMenu->addMenu(menuMark);
    contextMenu->addAction(actionRemoveMark);
    contextMenu->addAction(actionBookmark);
    contextMenu->addAction(actionNote);
    contextMenu->exec(QCursor::pos());
    return 0;*/
}

int AdvancedInterface::copyWholeVerse(void)
{
   /* if (!activeMdiChild())
        return 1;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QTextCursor cursor = textBrowser->textCursor();
    VerseSelection selection = verseSelectionFromCursor(cursor);
    if (selection.startVerse != -1) {
        if (m_moduleManager->bible()->bibleType() == Bible::BibleQuoteModule) {
        } else if (m_moduleManager->bible()->bibleType() == Bible::ZefaniaBibleModule) {
            if (selection.startVerse - 1 < 0)
                selection.startVerse = 1;
            if (selection.endVerse < 0)
                selection.endVerse = 0;
        }

        QString sverse = "";
        if (selection.startVerse == selection.endVerse) {
            sverse = "," + QString::number(selection.startVerse);
        } else {
            sverse = " " + QString::number(selection.startVerse) + "-" + QString::number(selection.endVerse);
        }


        QString stext;

        if (m_moduleManager->bible()->bibleType() == Bible::BibleQuoteModule) {
            stext = m_moduleManager->bible()->readVerse(m_moduleManager->bible()->chapterID(), selection.startVerse, selection.endVerse + 1, -1, false);
        } else if (m_moduleManager->bible()->bibleType() == Bible::ZefaniaBibleModule) {
            stext = m_moduleManager->bible()->readVerse(m_moduleManager->bible()->chapterID(), selection.startVerse - 1, selection.endVerse, -1, false);
        }

        QTextDocument doc2;
        doc2.setHtml(stext);
        stext = doc2.toPlainText();

        QString curChapter;
        if (m_moduleManager->bible()->bibleType() == Bible::BibleQuoteModule) {
            curChapter = QString::number(m_moduleManager->bible()->chapterID());
        } else if (m_moduleManager->bible()->bibleType() == Bible::ZefaniaBibleModule) {
            curChapter = QString::number(m_moduleManager->bible()->chapterID() + 1);
        }

        QString newText = m_moduleManager->bible()->bookFullName().at(m_moduleManager->bible()->bookID()) + " " + curChapter + sverse + "\n" + stext;
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(newText);

    } else {
        myDebug() << "nothing is selected";
    }

    return 0;*/
}
void AdvancedInterface::newYellowMark()
{
   /* if (!m_moduleManager->bibleLoaded()) {
        return;
    }
    if (!activeMdiChild())
        return;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QTextCursor cursor = textBrowser->textCursor();
    VerseSelection selection = verseSelectionFromCursor(cursor);
    m_notesDockWidget->newMark(selection, QColor(255, 255, 0));*/
}

void AdvancedInterface::newGreenMark()
{
    /*if (!m_moduleManager->bibleLoaded()) {
        return;
    }
    if (!activeMdiChild())
        return;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QTextCursor cursor = textBrowser->textCursor();
    VerseSelection selection = verseSelectionFromCursor(cursor);
    m_notesDockWidget->newMark(selection, QColor(146, 243, 54));*/
}
void AdvancedInterface::newBlueMark()
{
    /*if (!m_moduleManager->bibleLoaded()) {
        return;
    }
    if (!activeMdiChild())
        return;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QTextCursor cursor = textBrowser->textCursor();
    VerseSelection selection = verseSelectionFromCursor(cursor);
    m_notesDockWidget->newMark(selection, QColor(77, 169, 243));*/
}
void AdvancedInterface::newOrangeMark()
{
   /* if (!m_moduleManager->bibleLoaded()) {
        return;
    }
    if (!activeMdiChild())
        return;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QTextCursor cursor = textBrowser->textCursor();
    VerseSelection selection = verseSelectionFromCursor(cursor);
    m_notesDockWidget->newMark(selection, QColor(243, 181, 57));*/
}
void AdvancedInterface::newVioletMark()
{
    /*if (!m_moduleManager->bibleLoaded()) {
        return;
    }
    if (!activeMdiChild())
        return;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QTextCursor cursor = textBrowser->textCursor();
    VerseSelection selection = verseSelectionFromCursor(cursor);
    m_notesDockWidget->newMark(selection, QColor(169, 102, 240));*/
}
void AdvancedInterface::newCustomMark()
{
    /*if (!m_moduleManager->bibleLoaded()) {
        return;
    }
    if (!activeMdiChild())
        return;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QTextCursor cursor = textBrowser->textCursor();
    VerseSelection selection = verseSelectionFromCursor(cursor);
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid()) {
        m_notesDockWidget->newMark(selection, color);
    }*/

}
void AdvancedInterface::removeMark()
{
    if (!m_moduleManager->bibleLoaded() && !activeMdiChild()) {
        return;
    }
    VerseSelection selection = verseSelectionFromCursor(m_textCursor);
    m_notesDockWidget->removeMark(selection);
}

void AdvancedInterface::closing()
{
    DEBUG_FUNC_NAME
    m_notesDockWidget->saveNote();
    m_bookmarksDockWidget->saveBookmarks();
    m_settings->session.setData("bookmarksDockGeometry", m_bookmarksDockWidget->saveGeometry());
    m_settings->session.setData("notesDockGeometry", m_notesDockWidget->saveGeometry());
    m_settings->session.setData("bookDockGeometry", m_bookDockWidget->saveGeometry());
    m_settings->session.setData("moduleDockGeometry", m_moduleDockWidget->saveGeometry());
    m_settings->session.setData("searchResultDockGeometry", m_searchResultDockWidget->saveGeometry());
    m_settings->session.setData("strongDockGeometry", m_strongDockWidget->saveGeometry());
    QStringList windowUrls;
    QList<QVariant> windowGeo;
    QList<QVariant> vSlider;
    QList<QVariant> hSlider;
    QList<QVariant> zoom;
    for (int i = 0; i < ui->mdiArea->subWindowList().count(); i++) {
        //todo:
        m_windowCache.setCurrentWindowID(i);
        //todo: save also bible list
        if(m_windowCache.getBibleList() != 0 && m_windowCache.getBibleList()->bible() != 0 && m_windowCache.getBibleList()->bible()->moduleID() >= 0) {
            Bible *b = m_windowCache.getBibleList()->bible();
            UrlConverter urlConverter(UrlConverter::None, UrlConverter::PersistentUrl, "");
            urlConverter.setModuleMap(m_moduleManager->m_moduleMap);
            urlConverter.m_moduleID = b->moduleID();
            urlConverter.m_bookID = b->bookID();
            urlConverter.m_chapterID = b->chapterID() - b->chapterAdd();
            urlConverter.m_verseID = 0;
            windowUrls << urlConverter.convert();
        } else {
            windowUrls << "";
        }
        windowGeo << ui->mdiArea->subWindowList().at(i)->geometry();

       /* QTextBrowser *textBrowser =  ui->mdiArea->subWindowList().at(i)->findChild<QTextBrowser *>("textBrowser");
        vSlider <<  textBrowser->verticalScrollBar()->sliderPosition();
        hSlider <<  textBrowser->horizontalScrollBar()->sliderPosition();
#if QT_VERSION >= 0x040600
        zoom << textBrowser->fontPointSize();
#endif*/
    }
    m_settings->session.setData("windowUrls", windowUrls);
    m_settings->session.setData("windowGeo", windowGeo);
    m_settings->session.setData("vSlider", vSlider);
    m_settings->session.setData("hSlider", hSlider);
#if QT_VERSION >= 0x040600
    m_settings->session.setData("zoom", zoom);
#endif
    m_settings->session.setData("viewMode", ui->mdiArea->viewMode());


}
void AdvancedInterface::restoreSession()
{
    DEBUG_FUNC_NAME
    m_bookmarksDockWidget->restoreGeometry(m_settings->session.getData("bookmarksDockGeometry").toByteArray());
    m_notesDockWidget->restoreGeometry(m_settings->session.getData("notesDockGeometry").toByteArray());
    m_bookDockWidget->restoreGeometry(m_settings->session.getData("bookDockGeometry").toByteArray());
    m_moduleDockWidget->restoreGeometry(m_settings->session.getData("moduleDockGeometry").toByteArray());
    m_searchResultDockWidget->restoreGeometry(m_settings->session.getData("searchResultDockGeometry").toByteArray());
    m_strongDockWidget->restoreGeometry(m_settings->session.getData("strongDockGeometry").toByteArray());
    myDebug() << "a";
    QStringList windowUrls = m_settings->session.getData("windowUrls").toStringList();
    QVariantList windowGeo = m_settings->session.getData("windowGeo").toList();
    QVariantList vSlider = m_settings->session.getData("vSlider").toList();
    QVariantList hSlider = m_settings->session.getData("hSlider").toList();
#if QT_VERSION >= 0x040600
    QVariantList zoom = m_settings->session.getData("zoom").toList();
#endif
    myDebug() << "b";

    for (int i = 0; i < windowUrls.size(); ++i) {
        newSubWindow(false);
        myDebug() << "current window is " << tabIDof(activeMdiChild()) << " while window count is " << usableWindowList();
        //load bible
        QString url = windowUrls.at(i);
        if (!url.isEmpty() && url.size() != 0) {
            myDebug() << "windows urls = " << windowUrls.at(i);
            UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, windowUrls.at(i));
            urlConverter.setModuleMap(m_moduleManager->m_moduleMap);
            urlConverter.pharse();
            myDebug() << "url = " << urlConverter.convert();
            pharseUrl(urlConverter.convert());
        }
        //set geometry
        activeMdiChild()->setGeometry(windowGeo.at(i).toRect());
        /*QTextBrowser *textBrowser = getView();
        //set slider
        //todo: really strange
        textBrowser->verticalScrollBar()->setSliderPosition(vSlider.at(i).toInt());
        textBrowser->horizontalScrollBar()->setSliderPosition(hSlider.at(i).toInt());
#if QT_VERSION >= 0x040600
        if (zoom.size() != 0 && i < zoom.size() && zoom.at(i).toReal() > 0)
            textBrowser->setFontPointSize(zoom.at(i).toReal());
#endif*/

    }
    if (m_settings->session.getData("viewMode").toInt() == 0)
        setSubWindowView();
    else if (m_settings->session.getData("viewMode").toInt() == 1)
        setTabView();
    //restore
}
void AdvancedInterface::settingsChanged(Settings oldSettings, Settings newSettings)
{
    //DEBUG_FUNC_NAME
    //reload books
    bool reloadBibles = false;
    if (oldSettings.encoding != newSettings.encoding) {
        reloadBibles = true;
    }
    if (oldSettings.m_moduleSettings.size() > newSettings.m_moduleSettings.size())
        reloadBibles = true;
    for (int i = 0; i < newSettings.m_moduleSettings.size(); ++i) {
        if (oldSettings.m_moduleSettings.size() < i || oldSettings.m_moduleSettings.empty()) {
            reloadBibles = true;
            break;
        } else {
            ModuleSettings m1, m2;
            m1 = newSettings.m_moduleSettings.at(i);
            m2 = oldSettings.m_moduleSettings.at(i);
            if (memcmp(&m1, &m2, sizeof(ModuleSettings))) {
                reloadBibles = true;
                break;
            }
        }
    }
    if (reloadBibles == true) {
        myDebug() << "reload Module";
        m_moduleManager->loadAllModules();
        m_moduleDockWidget->init();
        m_strongDockWidget->init();
        showText("");
        //m_windowCache.clearZefCache();
        //m_moduleManager->bible()->clearSoftCache();
        if (m_moduleManager->bibleLoaded())
            reloadChapter(true);
    }

}
void AdvancedInterface::showSearchDialog()
{
    DEBUG_FUNC_NAME
    SearchDialog *sDialog = new SearchDialog(this);
    connect(sDialog, SIGNAL(searched(SearchQuery)), this, SLOT(search(SearchQuery)));
    QString text = getView()->selectedText();
    if(!text.isEmpty()) {
         sDialog->setText(text);
    }
    sDialog->show();
    sDialog->exec();
}
void AdvancedInterface::search(SearchQuery query)
{
    //DEBUG_FUNC_NAME
    if (!m_moduleManager->bibleLoaded())
        return;
    m_searchResultDockWidget->show();
    SearchResult result;
    result = m_moduleManager->bible()->search(query);
    m_searchResultDockWidget->setSearchResult(result);

}
void AdvancedInterface::searchInText(SearchQuery query)
{
    DEBUG_FUNC_NAME
    //todo: make regexp work
    if (query.queryType == SearchQuery::Simple && !query.regExp) {
        if (query.caseSensitive) {
            getView()->findText(query.searchText,QWebPage::FindCaseSensitively | QWebPage::HighlightAllOccurrences);
        } else {
            getView()->findText(query.searchText,QWebPage::HighlightAllOccurrences);
        }
    }

   /* if (query.queryType == SearchQuery::Simple) {
        QTextDocument *doc = getView()->document();
        QTextDocument::FindFlags flags;
        if (query.caseSensitive) {
            flags |= QTextDocument::FindCaseSensitively;
        }
        if (query.wholeWord) {
            flags |= QTextDocument::FindWholeWords;
        }

        if (query.regExp) {
            QTextCursor cursor;
            cursor = doc->find(QRegExp(query.searchText), 0);
            while (!cursor.isNull()) {
                cursor.insertHtml("<span style=\"background-color:#ffcf3d\">" + cursor.selectedText() + "</span>");
                getView()->setTextCursor(cursor);

                cursor = doc->find(QRegExp(query.searchText), cursor);
            }
        } else {
            QTextCursor cursor;
            cursor = doc->find(query.searchText, 0, flags);
            while (!cursor.isNull()) {

                cursor.insertHtml("<span style=\"background-color:#ffcf3d\">" + cursor.selectedText() + "</span>");
                getView()->setTextCursor(cursor);

                cursor = doc->find(query.searchText, cursor, flags);
            }
        }
    }*/
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
    m_searchResultDockWidget->nextVerse();
}
void AdvancedInterface::previousVerse()
{
    m_searchResultDockWidget->previousVerse();
}
bool AdvancedInterface::hasMenuBar()
{
    return true;
}
QMenuBar* AdvancedInterface::menuBar()
{
    QMenuBar *bar = new QMenuBar(parentWidget());
    QMenu *menuFile = new QMenu(tr("File"), bar);

    QAction *actionNewSubWindow = new QAction(QIcon::fromTheme("tab-new",QIcon(":/icons/16x16/tab-new.png")), tr("New SubWindow"), menuFile);
    connect(actionNewSubWindow, SIGNAL(triggered()), this, SLOT(newSubWindow()));
    actionNewSubWindow->setShortcut(QKeySequence(tr("Ctrl+T")));

    QAction *actionCloseSubWindow = new QAction(QIcon::fromTheme("tab-close",QIcon(":/icons/16x16/tab-close.png")), tr("Close SubWindow"), menuFile);
    connect(actionCloseSubWindow, SIGNAL(triggered()), this, SLOT(closeSubWindow()));

    QAction *actionSaveAs = new QAction(QIcon::fromTheme("document-save-as",QIcon(":/icons/16x16/document-save-as.png")), tr("Save As"), menuFile);
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(saveFile()));

    QAction *actionPrint = new QAction(QIcon::fromTheme("document-print",QIcon(":/icons/16x16/document-print.png")), tr("Print"), menuFile);
    connect(actionPrint, SIGNAL(triggered()), this, SLOT(printFile()));

    QAction *actionClose = new QAction(QIcon::fromTheme("application-exit",QIcon(":/icons/16x16/application-exit.png")), tr("Close"), menuFile);
    connect(actionClose, SIGNAL(triggered()), this->parentWidget(), SLOT(close()));
    menuFile->addAction(actionNewSubWindow);
    menuFile->addAction(actionCloseSubWindow);
    menuFile->addSeparator();
    menuFile->addAction(actionSaveAs);
    menuFile->addAction(actionPrint);
    menuFile->addSeparator();
    menuFile->addAction(actionClose);

    QMenu *menuEdit = new QMenu(tr("Edit"), bar);
    QAction *actionCopy = new QAction(QIcon::fromTheme("edit-copy",QIcon(":/icons/16x16/edit-copy.png")), tr("Copy"), menuEdit);
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copy()));

    QAction *actionSelectAll = new QAction(QIcon::fromTheme("edit-select-all",QIcon(":/icons/16x16/edit-select-all.png")), tr("Select All"), menuEdit);
    connect(actionSelectAll, SIGNAL(triggered()), this, SLOT(selectAll()));

    QAction *actionSearch = new QAction(QIcon::fromTheme("edit-find",QIcon(":/icons/16x16/edit-find.png")), tr("Search"), menuEdit);
    actionSearch->setShortcut(QKeySequence(tr("Ctrl+F")));
    connect(actionSearch, SIGNAL(triggered()), this, SLOT(showSearchDialog()));
    QAction *actionFindNext = new QAction(QIcon::fromTheme("go-down-search",QIcon(":/icons/16x16/go-down-search.png")), tr("Find Next"), menuEdit);
    connect(actionFindNext, SIGNAL(triggered()), this, SLOT(nextVerse()));
    QAction *actionFindPrevious = new QAction(QIcon::fromTheme("go-up-search",QIcon(":/icons/16x16/go-up-search.png")), tr("Find Previous"), menuEdit);
    connect(actionFindPrevious, SIGNAL(triggered()), this, SLOT(previousVerse()));

    QAction *actionConfiguration = new QAction(QIcon::fromTheme("configure",QIcon(":/icons/16x16/configure.png")), tr("Configuration"), menuEdit);
    connect(actionConfiguration, SIGNAL(triggered()), this->parent(), SLOT(showSettingsDialog_General()));

    menuEdit->addAction(actionCopy);
    menuEdit->addAction(actionSelectAll);
    menuEdit->addSeparator();
    menuEdit->addAction(actionSearch);
    menuEdit->addAction(actionFindNext);
    menuEdit->addAction(actionFindPrevious);
    menuEdit->addSeparator();
    menuEdit->addAction(actionConfiguration);

    QMenu *menuView = new QMenu(tr("View"), bar);

    QAction *actionZoomIn = new QAction(QIcon::fromTheme("zoom-in",QIcon(":/icons/16x16/zoom-in.png")), tr("Zoom In"), menuView);
    connect(actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
    actionZoomIn->setShortcut(QKeySequence(tr("Ctrl++")));
    QAction *actionZoomOut = new QAction(QIcon::fromTheme("zoom-out",QIcon(":/icons/16x16/zoom-out.png")), tr("Zoom Out"), menuView);
    actionZoomOut->setShortcut(QKeySequence(tr("Ctrl+-")));
    connect(actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));

    m_actionTabView = new QAction(QIcon(), tr("Tabbed View"), menuView);
    m_actionTabView->setCheckable(true);
    connect(m_actionTabView, SIGNAL(triggered()), this, SLOT(setTabView()));

    m_actionSubWindowView = new QAction(QIcon(), tr("Sub Window View"), menuView);
    m_actionSubWindowView->setCheckable(true);
    connect(m_actionSubWindowView, SIGNAL(triggered()), this, SLOT(setSubWindowView()));

    QAction *actionCascade = new QAction(QIcon(":/icons/svg/cascade.svg"), tr("Cascade"), menuView);
    connect(actionCascade, SIGNAL(triggered()), this, SLOT(myCascade()));
    QAction *actionTile = new QAction(QIcon(":/icons/svg/tile.svg"), tr("Tile"), menuView);
    connect(actionTile, SIGNAL(triggered()), this, SLOT(myTile()));

    QAction *actionTileVertical = new QAction(QIcon(":/icons/svg/tile_vert.svg"), tr("Tile Vertical"), menuView);
    connect(actionTileVertical, SIGNAL(triggered()), this, SLOT(myTileVertical()));
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
    QAction *actionNotesEditor = new QAction(QIcon::fromTheme("notes-edit",QIcon(":/icons/16x16/notes-edit.png")), tr("Notes Editor"), menuNotes);
    connect(actionNotesEditor, SIGNAL(triggered()), this, SLOT(showNotesEditor()));

    QAction *actionMarkList = new QAction(QIcon::fromTheme("table",QIcon(":/icons/16x16/table.png")), tr("Mark List"), menuNotes);
    connect(actionMarkList, SIGNAL(triggered()), this, SLOT(showMarkList()));
    QAction *actionMarkCategories = new QAction(QIcon(), tr("Mark Categories"), menuNotes);
    connect(actionMarkCategories, SIGNAL(triggered()), this, SLOT(showMarkCategories()));
    menuNotes->addAction(actionNotesEditor);
    //menuNotes->addSeparator();
    menuNotes->addAction(actionMarkList);
    // menuNotes->addAction(actionMarkCategories);

    QMenu *menuDocks = new QMenu(tr("Docks"), bar);

    QAction *actionModule = new QAction(tr("Module"), menuDocks);
    actionModule->setCheckable(true);
    connect(m_moduleDockWidget,SIGNAL(visibilityChanged(bool)),actionModule,SLOT(setChecked(bool)));
    connect(actionModule,SIGNAL(triggered(bool)), m_moduleDockWidget, SLOT(setVisible(bool)));

    QAction *actionBooks = new QAction(tr("Books"), menuDocks);
    actionBooks->setCheckable(true);
    connect(m_bookDockWidget,SIGNAL(visibilityChanged(bool)),actionBooks,SLOT(setChecked(bool)));
    connect(actionBooks,SIGNAL(triggered(bool)), m_bookDockWidget, SLOT(setVisible(bool)));

    QAction *actionNotes = new QAction(QIcon::fromTheme("notes-edit",QIcon(":/icons/16x16/notes-edit.png")), tr("Notes"), menuDocks);
    actionNotes->setCheckable(true);
    connect(m_notesDockWidget,SIGNAL(visibilityChanged(bool)),actionNotes,SLOT(setChecked(bool)));
    connect(actionNotes,SIGNAL(triggered(bool)), m_notesDockWidget, SLOT(setVisible(bool)));

    QAction *actionStrong = new QAction( tr("Strong"), menuDocks);
    actionStrong->setCheckable(true);
    connect(m_strongDockWidget,SIGNAL(visibilityChanged(bool)),actionStrong,SLOT(setChecked(bool)));
    connect(actionStrong,SIGNAL(triggered(bool)), m_strongDockWidget, SLOT(setVisible(bool)));

    QAction *actionBookmarks = new QAction(QIcon::fromTheme("bookmarks-organize",QIcon(":/icons/16x16/bookmarks-organize.png")), tr("Bookmarks"), menuDocks);
    actionBookmarks->setCheckable(true);
    connect(m_bookmarksDockWidget,SIGNAL(visibilityChanged(bool)),actionBookmarks,SLOT(setChecked(bool)));
    connect(actionBookmarks,SIGNAL(triggered(bool)), m_bookmarksDockWidget, SLOT(setVisible(bool)));

    QAction *actionQuickJump = new QAction( tr("Quick Jump"), menuDocks);
    actionQuickJump->setCheckable(true);
    connect(m_quickJumpDockWidget,SIGNAL(visibilityChanged(bool)),actionQuickJump,SLOT(setChecked(bool)));
    connect(actionQuickJump,SIGNAL(triggered(bool)), m_quickJumpDockWidget, SLOT(setVisible(bool)));

    QAction *actionSearchResults = new QAction(QIcon::fromTheme("table",QIcon(":/icons/16x16/table.png")), tr("Search Results"), menuDocks);
    actionSearchResults->setCheckable(true);
    connect(m_searchResultDockWidget,SIGNAL(visibilityChanged(bool)),actionSearchResults,SLOT(setChecked(bool)));
    connect(actionSearchResults ,SIGNAL(triggered(bool)), m_searchResultDockWidget, SLOT(setVisible(bool)));


    menuDocks->addAction(actionModule);
    menuDocks->addAction(actionBooks);
    menuDocks->addAction(actionNotes);
    menuDocks->addAction(actionBookmarks);
    menuDocks->addAction(actionQuickJump);
    menuDocks->addAction(actionStrong);
    menuDocks->addAction(actionSearchResults);

    QMenu *menuHelp = new QMenu(tr("Help"), bar);
    QAction *actionAbout = new QAction(QIcon::fromTheme("help-about",QIcon(":/icons/16x16/help-about.png")), tr("About"), menuHelp);
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    QAction *actionOnlineHelp = new QAction(QIcon::fromTheme("help-contents",QIcon(":/icons/16x16/help-contents.png")), tr("Online Help"), menuHelp);
    connect(actionOnlineHelp, SIGNAL(triggered()), this, SLOT(onlineHelp()));

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
QToolBar * AdvancedInterface::toolBar()
{
    QToolBar *bar = new QToolBar(this->parentWidget());
    bar->setObjectName("mainToolBar");
    bar->setIconSize(QSize(16, 16));
#if QT_VERSION >= 0x040600
    bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
#else
    bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
#endif
    bar->setWindowTitle(tr("ToolBar"));
    QAction *actionSearch = new QAction(QIcon::fromTheme("edit-find",QIcon(":/icons/32x32/edit-find.png")), tr("Search"), bar);
    connect(actionSearch, SIGNAL(triggered()), this, SLOT(showSearchDialog()));

    QAction *actionBookmarks = new QAction(QIcon::fromTheme("bookmarks-organize",QIcon(":/icons/32x32/bookmarks-organize.png")), tr("Bookmarks"), bar);
    connect(actionBookmarks, SIGNAL(triggered()), this, SLOT(showBookmarksDock()));
    actionBookmarks->setCheckable(true);
    connect(m_bookmarksDockWidget, SIGNAL(visibilityChanged(bool)), actionBookmarks, SLOT(setChecked(bool)));

    QAction *actionNotes = new QAction(QIcon::fromTheme("view-pim-notes",QIcon(":/icons/32x32/view-pim-notes.png")), tr("Notes"), bar);
    connect(actionNotes, SIGNAL(triggered()), this, SLOT(showNotesDock()));
    actionNotes->setCheckable(true);
    connect(m_notesDockWidget, SIGNAL(visibilityChanged(bool)), actionNotes, SLOT(setChecked(bool)));

    QAction *actionNewWindow = new QAction(QIcon::fromTheme("tab-new",QIcon(":/icons/32x32/tab-new.png")), tr("New Window"), bar);
    connect(actionNewWindow, SIGNAL(triggered()), this, SLOT(newSubWindow()));

    QAction *actionZoomIn = new QAction(QIcon::fromTheme("zoom-in",QIcon(":/icons/32x32/zoom-in.png")), tr("Zoom In"), bar);
    connect(actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
    QAction *actionZoomOut = new QAction(QIcon::fromTheme("zoom-out",QIcon(":/icons/32x32/zoom-out.png")), tr("Zoom Out"), bar);
    connect(actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));

    QAction *actionModule = new QAction(QIcon(":/icons/32x32/module.png"), tr("Module"), bar);
    connect(actionModule, SIGNAL(triggered()), this->parent(), SLOT(showSettingsDialog_Module()));

    bar->addAction(actionSearch);
    bar->addSeparator();
    bar->addAction(actionNewWindow);
    bar->addSeparator();
    bar->addAction(actionZoomIn);
    bar->addAction(actionZoomOut);
    bar->addSeparator();
    bar->addAction(actionModule);
    bar->addSeparator();
    bar->addAction(actionBookmarks);
    bar->addAction(actionNotes);

    return bar;
}
AdvancedInterface::~AdvancedInterface()
{
    delete ui;
}
void AdvancedInterface::showBookmarksDock()
{
    if (m_bookmarksDockWidget->isVisible()) {
        m_bookmarksDockWidget->hide();
    } else {
        m_bookmarksDockWidget->show();
    }

}
void AdvancedInterface::showNotesDock()
{
    if (m_notesDockWidget->isVisible()) {
        m_notesDockWidget->hide();
    } else {
        m_notesDockWidget->show();
    }
}
void AdvancedInterface::newBookmark()
{
    if (!m_moduleManager->bibleLoaded() && !activeMdiChild())
        return;
    m_bookmarksDockWidget->newBookmark(verseSelectionFromCursor(m_textCursor));
}
void AdvancedInterface::newNoteWithLink()
{
    if (!m_moduleManager->bibleLoaded() && !activeMdiChild())
        return;
    m_notesDockWidget->newNoteWithLink(verseSelectionFromCursor(m_textCursor));
}
void AdvancedInterface::onlineHelp()
{
    //open the online faq
    QDesktopServices::openUrl(QString("http://openbv.uucyc.name/faq.html"));
}
int AdvancedInterface::printFile(void)
{
    //DEBUG_FUNC_NAME
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print"));
    if (dialog->exec() != QDialog::Accepted)
        return 1;
    if (activeMdiChild()) {
        getView()->print(&printer);
    }
    return 0;
}
int AdvancedInterface::saveFile(void)
{
    QFileDialog dialog(this);
    //dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    //todo: save last place
    QString fileName = dialog.getSaveFileName(this, tr("Save output"), m_settings->lastPlaceSave, tr("Html (*.html *.htm);;PDF (*.pdf);;Plain (*.txt)"));
    myDebug() << "fileName = " << fileName;
    if (activeMdiChild()) {
        QWebView *v = getView();
        QFileInfo fi(fileName);
        m_settings->lastPlaceSave = fi.path();
        if (fileName.endsWith(".html") || fileName.endsWith(".htm")) {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return 1;
            QTextStream out(&file);
            out << v->page()->mainFrame()->toHtml();
            file.close();
        } else if (fileName.endsWith(".pdf")) {
            QPrinter printer;
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(fileName);
            v->print(&printer);

        } else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
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
    MarkList *markList = new MarkList(this);
    setAll(markList);
    markList->init();
    markList->show();
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
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
