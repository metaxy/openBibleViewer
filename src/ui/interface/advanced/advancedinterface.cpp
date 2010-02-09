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
#include "src/ui/dialog/aboutdialog.h"
#include "src/ui/noteseditor.h"
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
void AdvancedInterface::init()
{
    m_moduleManager->m_bible.setSettings(m_settings);

    m_moduleDockWidget->setBibleDisplay(m_bibleDisplay);
    m_moduleDockWidget->setNotes(m_notes);
    m_moduleDockWidget->setSettings(m_settings);
    m_moduleDockWidget->setModuleManager(m_moduleManager);
    m_moduleDockWidget->init();
    connect(m_moduleDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    m_bookDockWidget->setBibleDisplay(m_bibleDisplay);
    m_bookDockWidget->setNotes(m_notes);
    m_bookDockWidget->setSettings(m_settings);
    m_bookDockWidget->setModuleManager(m_moduleManager);
    connect(m_bookDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    m_searchResultDockWidget->setBibleDisplay(m_bibleDisplay);
    m_searchResultDockWidget->setNotes(m_notes);
    m_searchResultDockWidget->setSettings(m_settings);
    m_searchResultDockWidget->setModuleManager(m_moduleManager);
    m_searchResultDockWidget->hide();
    connect(m_searchResultDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    m_notesDockWidget->setBibleDisplay(m_bibleDisplay);
    m_notesDockWidget->setNotes(m_notes);
    m_notesDockWidget->setSettings(m_settings);
    m_notesDockWidget->setModuleManager(m_moduleManager);
    m_notesDockWidget->init();
    m_notesDockWidget->hide();
    connect(m_notesDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));
    connect(m_notesDockWidget, SIGNAL(reloadChapter()), this, SLOT(reloadChapter()));

    m_bookmarksDockWidget->setBibleDisplay(m_bibleDisplay);
    m_bookmarksDockWidget->setNotes(m_notes);
    m_bookmarksDockWidget->setSettings(m_settings);
    m_bookmarksDockWidget->setModuleManager(m_moduleManager);
    m_bookmarksDockWidget->init();
    m_bookmarksDockWidget->hide();
    connect(m_bookmarksDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    m_strongDockWidget->setBibleDisplay(m_bibleDisplay);
    m_strongDockWidget->setNotes(m_notes);
    m_strongDockWidget->setSettings(m_settings);
    m_strongDockWidget->setModuleManager(m_moduleManager);
    m_strongDockWidget->init();
    m_strongDockWidget->hide();
    connect(m_strongDockWidget, SIGNAL(get(QUrl)), this, SLOT(pharseUrl(QUrl)));

    connect(m_bibleDisplay, SIGNAL(newHtml(QString)), this, SLOT(showText(QString)));
    connect(this, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    BibleDisplaySettings bibleDisplaySettings;
    bibleDisplaySettings.loadNotes = true;
    bibleDisplaySettings.showMarks = true;
    bibleDisplaySettings.showNotes = true;
    m_moduleManager->m_bible.setBibleDisplaySettings(bibleDisplaySettings);

    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(reloadWindow(QMdiSubWindow *)));
    myDebug() << "homepath = " << m_settings->homePath;
    if(usableWindowList().size() == 0 &&  m_settings->session.getData("windowUrls").toStringList().size() == 0)
        QTimer::singleShot(10, this, SLOT(newMdiChild()));//todo: fix this ugly bug
    myDebug() << " windowlist = " << usableWindowList();
}


void AdvancedInterface::newMdiChild(bool autoLayout)
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
    layout->addWidget(mForm);

    widget->setLayout(layout);
    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(widget);
    subWindow->setWindowIcon(QIcon(":/icons/16x16/main.png"));
    subWindow->setWindowOpacity(1.0);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();
    ui->mdiArea->setActiveSubWindow(subWindow);
    if (windowsCount == 0) {
        subWindow->showMaximized();
    } else if (windowsCount == 1) {
        firstSubWindow->resize(600, 600);
        firstSubWindow->showNormal();
        subWindow->resize(600, 600);
        subWindow->show();
    } else {
        subWindow->resize(600, 600);
        subWindow->show();
    }

    connect(mForm->m_ui->comboBox_books, SIGNAL(activated(int)), this, SLOT(readBook(int)));
    connect(mForm->m_ui->comboBox_chapters, SIGNAL(activated(int)), this, SLOT(readChapter(int)));
    connect(mForm->m_ui->textBrowser, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(textBrowserContextMenu(QPoint)));
    connect(mForm->m_ui->textBrowser, SIGNAL(anchorClicked(QUrl)), this, SLOT(pharseUrl(QUrl)));
    connect(mForm, SIGNAL(historyGo(QString)), this, SLOT(pharseUrl(QString)));
    connect(mForm, SIGNAL(previousChapter()), this, SLOT(previousChapter()));
    connect(mForm, SIGNAL(nextChapter()), this, SLOT(nextChapter()));
    connect(this, SIGNAL(historySetUrl(QString)), mForm, SLOT(historyGetUrl(QString)));
    connect(subWindow, SIGNAL(destroyed(QObject*)), this, SLOT(closeWindow()));
    m_enableReload = true;
    if(autoLayout) {
        if (usableWindowList().size() > 1) {
            if (m_settings->autoLayout == 1) {
                myTileVertical();
            } else if (m_settings->autoLayout == 2) {
                myTileHorizontal();
            } else if (m_settings->autoLayout == 3) {
                myCascade();
            }
        }
    }
    m_internalWindows << subWindow;

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
    if (ui->mdiArea->activeSubWindow() && ui->mdiArea->activeSubWindow()->isMaximized()) {
        if (ui->mdiArea->activeSubWindow()->size() != this->size()) {
            ui->mdiArea->activeSubWindow()->resize(this->size());
        }
    } else if (windows.count() == 1) {
        windows.at(0)->showMaximized();
    } else {
        m_enableReload = false;
        QMdiSubWindow* active = ui->mdiArea->activeSubWindow();
        ui->mdiArea->tileSubWindows();
        if (active) active->setFocus();
        m_enableReload = true;
    }
}

void AdvancedInterface::myTileHorizontal()
{
    DEBUG_FUNC_NAME
    if (!m_enableReload || !usableWindowList().count()) {
        return;
    }
    QList<QMdiSubWindow*> windows = usableWindowList();

    if (ui->mdiArea->activeSubWindow() && ui->mdiArea->activeSubWindow()->isMaximized()) {
        if (ui->mdiArea->activeSubWindow()->size() != ui->mdiArea->size()) {
            ui->mdiArea->activeSubWindow()->resize(ui->mdiArea->size());
        }
    } else if (windows.count() == 1) {
        windows.at(0)->showMaximized();
    } else {
        m_enableReload = false;

        QMdiSubWindow* active = ui->mdiArea->activeSubWindow();

        const int heightForEach = ui->mdiArea->height() / windows.count();
        unsigned int y = 0;
        foreach(QMdiSubWindow *window, windows) {
            window->showNormal();

            const int preferredHeight = window->minimumHeight() + window->baseSize().height();
            const int actHeight = qMax(heightForEach, preferredHeight);

            window->setGeometry(0, y, ui->mdiArea->width(), actHeight);
            y += actHeight;
        }
        active->setFocus();
        m_enableReload = true;
    }
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
QList<QMdiSubWindow*> AdvancedInterface::usableWindowList()
{
    //only if !ChildAdded-Event is triggered
  /*  QList<QMdiSubWindow*> ret;
    foreach(QMdiSubWindow* w, ui->mdiArea->subWindowList()) {
        if (w->isMinimized() || w->isHidden()) //not usable for us
            continue;
        ret.append(w);
    }*/
   // return ret;
    return  ui->mdiArea->subWindowList();
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
int AdvancedInterface::closeWindow()
{
    //DEBUG_FUNC_NAME
    if (!m_enableReload) {
        myDebug() << "reload is not enabled";
        return 1;
    }
    //if one in the internal subwindow list list is missing that window was closed
    if (ui->mdiArea->subWindowList().isEmpty()) {
        myDebug() << "subWIndowList is empty";
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
    qDebug() << "MainWindow::closeWindow() closing";
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

    myDebug() << "setCurrentTab id = " << id;
    m_windowCache.setCurrentWindowID(id);
    //todo: add last active window and if it is the same do nohting
    if (m_windowCache.getBibleType() == Module::NoneType) { //probaly no bible loaded in this window
        myDebug() << "m_windowCache.getBibleType() == 0";
        setChapters(QStringList());
        setBooks(QStringList());
        m_moduleManager->m_bible.setBibleID(-2);
    } else {
        /*if (m_moduleManager->m_bible.bibleID() == m_windowCache.clearAll();)
            return 1;*/

        m_moduleManager->m_bible = m_windowCache.getBible();
        setTitle(m_moduleManager->m_bible.bibleTitle);

        setChapters(m_moduleManager->m_bible.chapterNames);
        myDebug() << "chapterNames = " << m_moduleManager->m_bible.chapterNames;
        setCurrentChapter(m_moduleManager->m_bible.chapterID());

        setBooks(m_moduleManager->m_bible.bookFullName);
        setCurrentBook(m_moduleManager->m_bible.bookID());
    }

    return 0;
}
QTextBrowser* AdvancedInterface::getCurrentTextBrowser()
{
    if (activeMdiChild()) {
        QTextBrowser *t = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
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
    QTextBrowser *t = getCurrentTextBrowser();
    if (t) {
        t->zoomIn();
    }

}
void AdvancedInterface::zoomOut()
{
    QTextBrowser *t = getCurrentTextBrowser();
    if (t) {
        t->zoomOut();
    }
}
void AdvancedInterface::loadModuleDataByID(int id)
{
    //DEBUG_FUNC_NAME
    myDebug() << "id = " << id;
    if(ui->mdiArea->subWindowList().size() == 0)
        newMdiChild();
    if (id < 0 || m_moduleManager->m_moduleList.size() < id) {
        QApplication::restoreOverrideCursor();
        return;
    }
    if(!m_moduleManager->m_moduleList.at(id).m_moduleClass == Module::BibleModule) {
        QApplication::restoreOverrideCursor();
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_moduleManager->m_bible.setBibleType(m_moduleManager->m_moduleList.at(id).m_moduleType);
    m_moduleManager->m_bible.loadBibleData(id, m_moduleManager->m_moduleList.at(id).m_path);
    m_moduleManager->m_bible.setSoftCache(m_windowCache.getSoftCache(id));//todo: if it is empty then do nothing

    m_windowCache.setCurrentWindowID(currentWindowID());
    m_windowCache.setBible(m_moduleManager->m_bible);//todo: !before loading an another bible, save the last

    setTitle(m_moduleManager->m_bible.bibleTitle);
    setBooks(m_moduleManager->m_bible.bookFullName);
    m_moduleDockWidget->loadedModule(id);
    QApplication::restoreOverrideCursor();

}
void AdvancedInterface::pharseUrl(QUrl url)
{
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
    if (url.startsWith(bible)) {
        url = url.remove(0, bible.size());
        QStringList a = url.split("/");
        if (a.size() == 2) {
            QStringList c = a.at(1).split(",");
            if (c.size() >= 3) {
                int bibleID;
                if (a.at(0) == "current") {
                    bibleID = m_moduleManager->m_bible.bibleID();
                } else {
                    bibleID = a.at(0).toInt();
                }
                int bookID = c.at(0).toInt();
                int chapterID = c.at(1).toInt();
                int verseID = c.at(2).toInt();
                if (bibleID != m_moduleManager->m_bible.bibleID()) {
                    loadModuleDataByID(bibleID);//todo: select the right module in treewidget
                    readBookByID(bookID);
                    setCurrentBook(bookID);
                    showChapter(chapterID + m_moduleManager->m_bible.chapterAdd(), verseID);
                    setCurrentChapter(chapterID);
                    //load bible
                } else if (bookID != m_moduleManager->m_bible.bookID()) {
                    readBookByID(bookID);
                    setCurrentBook(bookID);
                    showChapter(chapterID + m_moduleManager->m_bible.chapterAdd(), verseID);
                    setCurrentChapter(chapterID);
                    //load book
                } else if (chapterID != m_moduleManager->m_bible.chapterID()) {
                    showChapter(chapterID + m_moduleManager->m_bible.chapterAdd(), verseID);
                    setCurrentChapter(chapterID);
                    //load chapter
                } else {
                    showChapter(chapterID + m_moduleManager->m_bible.chapterAdd(), verseID);
                    setCurrentChapter(chapterID);
                }
                if (c.size() == 4 && c.at(3) == "searchInCurrentText=true") {//todo: not nice
                    //searchInCurrentText(lastsearch);
                }
                emit historySetUrl(url_backup);
            } else {
                myDebug() << "invalid URL";
            }
        } else {
            myDebug() << "invalid URL";
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
        //  qDebug() << "MainWindow::pharseUrl() internal = " << internal << " internalChapter = " <<internal.at(3).toInt() << " chapterID" << chapterID << " chapterAdd = "<< m_moduleManager->m_bible.chapterAdd();
        /*if(bibleID != m_moduleManager->m_bible.bibleID())
        {
            loadModuleDataByID(bibleID);
            readBookByID(bookID);
            setCurrentBook(bookID);
            showChapter(chapterID+m_moduleManager->m_bible.chapterAdd(),verseID);
            setCurrentChapter(chapterID);
            //load bible
        }
        else */if (bookID != m_moduleManager->m_bible.bookID())
        {
            readBookByID(bookID);
            setCurrentBook(bookID);
            showChapter(chapterID + m_moduleManager->m_bible.chapterAdd(), verseID);
            setCurrentChapter(chapterID);
            //load book
        } else if (chapterID != m_moduleManager->m_bible.chapterID()) {
            showChapter(chapterID + m_moduleManager->m_bible.chapterAdd(), verseID);
            setCurrentChapter(chapterID);
            //load chapter
        } else {
            showChapter(chapterID + m_moduleManager->m_bible.chapterAdd(), verseID);
            setCurrentChapter(chapterID);
        }
        emit historySetUrl(url_backup);

    } else if (url.startsWith(anchor)) {
        url = url.remove(0, anchor.size());

        bool ok;
        int c = url.toInt(&ok, 10);
        myDebug() << "c = " << c;
        if(ok && c < m_moduleManager->m_bible.chaptersCount()&& m_moduleManager->m_bible.m_bibleType == Bible::BibleQuoteModule && m_moduleManager->m_bible.chapterID() != c) {
            myDebug() << "bq chapter link";
            showChapter(c + m_moduleManager->m_bible.chapterAdd(),0);
            setCurrentChapter(c);
        } else {
            myDebug() << "anchor";
            if (activeMdiChild()) {
                QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
                textBrowser->scrollToAnchor(url);
            }
        }

    }  else if (url.startsWith(note)) {
        url = url.remove(0, note.size());
        if (!m_notesDockWidget->isVisible()) {
            m_notesDockWidget->show();
        }
        m_notesDockWidget->showNote(url);
    } else {
        myDebug() << " bookPath = " << m_moduleManager->m_bible.bookPath;
        if (m_moduleManager->m_bible.m_bibleType == Bible::BibleQuoteModule && m_moduleManager->m_bible.bookPath.contains(url)) {
            emit get("bible://current/" + m_moduleManager->m_bible.bookPath.lastIndexOf(url));//search in bible bookPath for this string, if it exixsts it is a book link
        } else {

            myDebug() << "invalid URL";
        }
    }
    setEnableReload(true);
    return;
}
void AdvancedInterface::showText(const QString &text)
{
    m_windowCache.setBible(m_moduleManager->m_bible);
    QTextBrowser *t = getCurrentTextBrowser();
    if (t) {
        t->setHtml(text);
        if (m_moduleManager->m_bible.m_verseID > 1)
            t->scrollToAnchor("currentVerse");
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
            comboBox_chapters->clear();
            comboBox_chapters->insertItems(0, chapters);
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
    if (activeMdiChild()) {
            QComboBox *comboBox_books = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_books");
            if(comboBox_books) {
                comboBox_books->clear();
                comboBox_books->insertItems(0, books);
            }
        }
}

void AdvancedInterface::setCurrentBook(const int &bookID)
{
    m_bookDockWidget->setCurrentBook(bookID);
    if (activeMdiChild()) {
        QComboBox *comboBox_books = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_books");
        if(comboBox_books) {
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
    QTextBrowser *t = getCurrentTextBrowser();
    if (t) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        if (id < 0) {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(0, tr("Error"), tr("This book is not available."));
            myDebug() << "invalid bookID - 1";
            return;
        }
        if (id >= m_moduleManager->m_bible.booksCount()) {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(0, tr("Error"), tr("This book is not available."));
            myDebug() << "invalid bookID - 2(no book loaded)";

            return;
        }
        if (m_moduleManager->m_bible.readBook(id) != 0) {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(0, tr("Error"), tr("Cannot read the book."));
            //error while reading
            return;
        }
        setChapters(m_moduleManager->m_bible.chapterNames);

        t->setSearchPaths(m_moduleManager->m_bible.getSearchPaths());
        QApplication::restoreOverrideCursor();
    }

}
void AdvancedInterface::readChapter(const int &id)
{
    //DEBUG_FUNC_NAME
    emit get("bible://current/" + QString::number(m_moduleManager->m_bible.bookID()) + "," + QString::number(id) + ",0");
}

void AdvancedInterface::showChapter(const int &chapterID, const int &verseID)
{
    m_moduleManager->m_bible.m_verseID = verseID;
    m_bibleDisplay->setHtml((m_moduleManager->m_bible.readChapter(chapterID, verseID)));
    setCurrentChapter(chapterID - m_moduleManager->m_bible.chapterAdd());
}
void AdvancedInterface::nextChapter()
{
    //DEBUG_FUNC_NAME
    if (m_moduleManager->m_bible.chapterID() < m_moduleManager->m_bible.chaptersCount() - 1) {
        readChapter(m_moduleManager->m_bible.chapterID() + 1);
    } else if(m_moduleManager->m_bible.bookID() < m_moduleManager->m_bible.booksCount() - 1) {
        readBook(m_moduleManager->m_bible.bookID() + 1);
    }
}
void AdvancedInterface::previousChapter()
{
    //DEBUG_FUNC_NAME
    if (m_moduleManager->m_bible.chapterID() > 0) {
        readChapter(m_moduleManager->m_bible.chapterID() - 1);
    } else if(m_moduleManager->m_bible.bookID() > 0) {
        readBook(m_moduleManager->m_bible.bookID() - 1);
        readChapter(m_moduleManager->m_bible.chaptersCount() - 1);
    }
}
VerseSelection AdvancedInterface::verseSelectionFromCursor(QTextCursor cursor)
{
    VerseSelection selection;
    //DEBUG_FUNC_NAME
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
        QStringList chapterData = m_moduleManager->m_bible.toUniformHtml(m_moduleManager->m_bible.chapterDataList);

        QString text = chapterData.join("");
        int startFragment = text.indexOf(fragment);
        qDebug() << "pos = " << startFragment << " fragemnt = " << fragment;
        //find out start verse and end verse
        int counter = 0;
        for (int i = 0; i < chapterData.size(); ++i) {
            counter += chapterData.at(i).size();
            myDebug() << "i = " << i << " counter = " << counter;
            if (selection.startVerse == -1 && startFragment < counter) {
                myDebug() << "setted start";
                selection.startVerse = i;
                selection.posInStartVerse = startFragment - (counter - chapterData.at(i).size()) ;
                for (int s = 3; s < 100; s++) {
                    QString b = fragment;
                    QString searchString = b.remove(s, fragment.size());
                    //todo: if it starts with a html tag remove that
                    QString v = chapterData.at(i);
                    int a1 = v.lastIndexOf(searchString);
                    int a2 = v.indexOf(searchString);
                    if (a1 == a2) {
                        myDebug() << "s = " << s << " searchString = " << searchString;
                        selection.shortestStringInStartVerse = searchString;
                        break;
                    }
                }
                myDebug() << "posInstartverse = " << selection.posInStartVerse;
            }
            if (selection.endVerse == -1 && (startFragment + fragment.size()  < (counter))) {
                myDebug() << "setted end";
                selection.endVerse = i;
                selection.posInEndVerse = (startFragment + fragment.size()) - (counter - chapterData.at(i).size()) ;
                for (int s = 3; s < 100; s++) {
                    QString b = fragment;
                    QString searchString = b.remove(0, b.size() - s);
                    //todo: if it starts with a html tag remove that
                    QString v = chapterData.at(i);
                    int a1 = v.lastIndexOf(searchString);
                    int a2 = v.indexOf(searchString);
                    if (a1 == a2) {
                        myDebug() << "s = " << s << " searchString = " << searchString;
                        selection.shortestStringInEndVerse = searchString;
                        break;
                    }
                }
                myDebug() << "posInEndverse = " << selection.posInEndVerse;
                break;
            }
        }
        myDebug() << " start = " << selection.startVerse << " end = " << selection.endVerse;
    } else {
        int pos = cursor.position();
        QStringList chapterData = m_moduleManager->m_bible.toUniformHtml(m_moduleManager->m_bible.chapterDataList);
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
    }

    return selection;
}
int AdvancedInterface::textBrowserContextMenu(QPoint pos)
{
    if (!activeMdiChild())
        return 1;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    qDebug() << "MainWindow::textBrowserContextMenu() start";
    QMenu *contextMenu = new QMenu(this);
    QAction *actionCopy = new QAction(this);
    QIcon copyIcon;
    copyIcon.addPixmap(QPixmap(":/icons/16x16/edit-copy.png"), QIcon::Normal, QIcon::Off);
    actionCopy->setIcon(copyIcon);
    actionCopy->setText(tr("Copy"));
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
        qDebug() << "MainWindow::textBrowserContextMenu() no selection";
        actionCopyWholeVerse->setText(tr("Copy Verse"));
        actionCopyWholeVerse->setEnabled(false);
    }
    QAction *actionSelect = new QAction(this);
    actionSelect->setText(tr("Select All"));
    QIcon selectIcon;
    selectIcon.addPixmap(QPixmap(":/icons/16x16/edit-select-all.png"), QIcon::Normal, QIcon::Off);
    actionSelect->setIcon(selectIcon);
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

    QAction *actionYellowMark = new QAction(menuMark);
    actionYellowMark->setText(tr("Yellow"));
    QIcon yellowMarkIcon;
    yellowMarkIcon.addPixmap(QPixmap(":/icons/16x16/mark-yellow.png"), QIcon::Normal, QIcon::Off);
    actionYellowMark->setIcon(yellowMarkIcon);
    connect(actionYellowMark, SIGNAL(triggered()), this , SLOT(newYellowMark()));
    menuMark->addAction(actionYellowMark);

    QAction *actionGreenMark = new QAction(menuMark);
    actionGreenMark->setText(tr("Green"));
    QIcon greenMarkIcon;
    greenMarkIcon.addPixmap(QPixmap(":/icons/16x16/mark-green.png"), QIcon::Normal, QIcon::Off);
    actionGreenMark->setIcon(greenMarkIcon);
    connect(actionGreenMark, SIGNAL(triggered()), this , SLOT(newGreenMark()));
    menuMark->addAction(actionGreenMark);

    QAction *actionBlueMark = new QAction(menuMark);
    actionBlueMark->setText(tr("Blue"));
    QIcon blueMarkIcon;
    blueMarkIcon.addPixmap(QPixmap(":/icons/16x16/mark-blue.png"), QIcon::Normal, QIcon::Off);
    actionBlueMark->setIcon(blueMarkIcon);
    connect(actionBlueMark, SIGNAL(triggered()), this , SLOT(newBlueMark()));
    menuMark->addAction(actionBlueMark);

    QAction *actionOrangeMark = new QAction(menuMark);
    actionOrangeMark->setText(tr("Orange"));
    QIcon orangeMarkIcon;
    orangeMarkIcon.addPixmap(QPixmap(":/icons/16x16/mark-orange.png"), QIcon::Normal, QIcon::Off);
    actionOrangeMark->setIcon(orangeMarkIcon);
    connect(actionOrangeMark, SIGNAL(triggered()), this , SLOT(newOrangeMark()));
    menuMark->addAction(actionOrangeMark);

    QAction *actionVioletMark = new QAction(menuMark);
    actionVioletMark->setText(tr("Violet"));
    QIcon violetMarkIcon;
    violetMarkIcon.addPixmap(QPixmap(":/icons/16x16/mark-violet.png"), QIcon::Normal, QIcon::Off);
    actionVioletMark->setIcon(violetMarkIcon);
    connect(actionVioletMark, SIGNAL(triggered()), this , SLOT(newVioletMark()));
    menuMark->addAction(actionVioletMark);

    QAction *actionRemoveMark = new QAction(this);
    actionRemoveMark->setText(tr("Remove Mark"));
    /*QIcon removeMarkIcon;
    removeMarkIcon.addPixmap(QPixmap(":/icons/16x16/mark-yellow.png"), QIcon::Normal, QIcon::Off);
    actionRemoveMark->setIcon(removeMarkIcon);*/
    connect(actionRemoveMark, SIGNAL(triggered()), this , SLOT(removeMark()));

    QAction *actionBookmark = new QAction(this);
    actionBookmark->setText(tr("Add Bookmark"));
    QIcon bookmarkIcon;
    bookmarkIcon.addPixmap(QPixmap(":/icons/16x16/bookmark-new.png"), QIcon::Normal, QIcon::Off);
    actionBookmark->setIcon(bookmarkIcon);
    connect(actionBookmark, SIGNAL(triggered()), this , SLOT(newBookmark()));

    QAction *actionNote = new QAction(this);
    actionNote->setText(tr("Add Note"));
    QIcon noteIcon;
    noteIcon.addPixmap(QPixmap(":/icons/16x16/view-pim-notes.png"), QIcon::Normal, QIcon::Off);
    actionNote->setIcon(noteIcon);
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
    return 0;
}

int AdvancedInterface::copyWholeVerse(void)
{
    if (!activeMdiChild())
        return 1;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QTextCursor cursor = textBrowser->textCursor();
    VerseSelection selection = verseSelectionFromCursor(cursor);
    if (selection.startVerse != -1) {

        QString sverse = "";
        if (selection.startVerse == selection.endVerse) {
            sverse = "," + QString::number(selection.startVerse);
        } else {
            sverse = " " + QString::number(selection.startVerse) + "-" + QString::number(selection.endVerse);
        }


        QString stext;
        if (m_moduleManager->m_bible.m_bibleType == Bible::BibleQuoteModule) {
            stext = m_moduleManager->m_bible.readVerse(m_moduleManager->m_bible.chapterID(), selection.startVerse, selection.endVerse + 1, -1, false);
        } else if (m_moduleManager->m_bible.m_bibleType == Bible::ZefaniaBibleModule) {
            stext = m_moduleManager->m_bible.readVerse(m_moduleManager->m_bible.chapterID(), selection.startVerse - 1, selection.endVerse, -1, false);
        }

        QTextDocument doc2;
        doc2.setHtml(stext);
        stext = doc2.toPlainText();

        QString curChapter;
        if (m_moduleManager->m_bible.m_bibleType == Bible::BibleQuoteModule) {
            curChapter = QString::number(m_moduleManager->m_bible.chapterID());
        } else if (m_moduleManager->m_bible.m_bibleType == Bible::ZefaniaBibleModule) {
            curChapter = QString::number(m_moduleManager->m_bible.chapterID() + 1);
        }

        QString newText = m_moduleManager->m_bible.bookFullName.at(m_moduleManager->m_bible.bookID()) + " " + curChapter + sverse + "\n" + stext;
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(newText);

    } else {
        myDebug() << "nothing is selected";
    }

    return 0;
}
void AdvancedInterface::newYellowMark()
{
    if (!m_moduleManager->bibleLoaded()) {
        return;
    }
    if (!activeMdiChild())
        return;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QTextCursor cursor = textBrowser->textCursor();
    VerseSelection selection = verseSelectionFromCursor(cursor);
    m_notesDockWidget->newMark(selection, QColor(255, 255, 0));
}

void AdvancedInterface::newGreenMark()
{
    if (!m_moduleManager->bibleLoaded()) {
        return;
    }
    if (!activeMdiChild())
        return;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QTextCursor cursor = textBrowser->textCursor();
    VerseSelection selection = verseSelectionFromCursor(cursor);
    m_notesDockWidget->newMark(selection, QColor(146, 243, 54));
}
void AdvancedInterface::newBlueMark()
{
    if (!m_moduleManager->bibleLoaded()) {
        return;
    }
    if (!activeMdiChild())
        return;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QTextCursor cursor = textBrowser->textCursor();
    VerseSelection selection = verseSelectionFromCursor(cursor);
    m_notesDockWidget->newMark(selection, QColor(77, 169, 243));
}
void AdvancedInterface::newOrangeMark()
{
    if (!m_moduleManager->bibleLoaded()) {
        return;
    }
    if (!activeMdiChild())
        return;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QTextCursor cursor = textBrowser->textCursor();
    VerseSelection selection = verseSelectionFromCursor(cursor);
    m_notesDockWidget->newMark(selection, QColor(243, 181, 57));
}
void AdvancedInterface::newVioletMark()
{
    if (!m_moduleManager->bibleLoaded()) {
        return;
    }
    if (!activeMdiChild())
        return;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    QTextCursor cursor = textBrowser->textCursor();
    VerseSelection selection = verseSelectionFromCursor(cursor);
    m_notesDockWidget->newMark(selection, QColor(169, 102, 240));
}
void AdvancedInterface::removeMark()
{
    if (!m_moduleManager->bibleLoaded() && !activeMdiChild()) {
        return;
    }
    VerseSelection selection = verseSelectionFromCursor(m_textCursor);
    m_notesDockWidget->removeMark(selection);
}
void AdvancedInterface::reloadChapter()
{
    if (!activeMdiChild())
        return;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");

    int vsliderPosition = textBrowser->verticalScrollBar()->sliderPosition();
    int hsliderPosition = textBrowser->horizontalScrollBar()->sliderPosition();//horizontal
    readChapter(m_moduleManager->m_bible.chapterID());
    textBrowser->verticalScrollBar()->setSliderPosition(vsliderPosition);
    textBrowser->horizontalScrollBar()->setSliderPosition(hsliderPosition);
}
void AdvancedInterface::closing()
{
    DEBUG_FUNC_NAME
    m_notesDockWidget->saveNote();
    m_bookmarksDockWidget->saveBookmarks();
    m_settings->session.setData("bookmarksDockGeometry",m_bookmarksDockWidget->saveGeometry());
    m_settings->session.setData("notesDockGeometry",m_notesDockWidget->saveGeometry());
    m_settings->session.setData("bookDockGeometry",m_bookDockWidget->saveGeometry());
    m_settings->session.setData("moduleDockGeometry",m_moduleDockWidget->saveGeometry());
    m_settings->session.setData("searchResultDockGeometry",m_searchResultDockWidget->saveGeometry());
    m_settings->session.setData("strongDockGeometry",m_strongDockWidget->saveGeometry());
    QStringList windowUrls;
    QList<QVariant> windowGeo;
    QList<QVariant> vSlider;
    QList<QVariant> hSlider;
    for (int i = 0; i < ui->mdiArea->subWindowList().count(); i++) {
         m_windowCache.setCurrentWindowID(i);
         Bible b = m_windowCache.getBible();
         UrlConverter urlConverter(UrlConverter::None, UrlConverter::PersistentUrl, "");
         urlConverter.m_biblesIniPath = b.biblesIniPath;
         urlConverter.m_bibleID = QString::number(b.bibleID());
         urlConverter.m_bookID = b.bookID();
         urlConverter.m_chapterID = b.chapterID() -b.chapterAdd();
         urlConverter.m_verseID = 0;
         windowUrls << urlConverter.convert();
         windowGeo << ui->mdiArea->subWindowList().at(i)->geometry();
         QTextBrowser *textBrowser =  ui->mdiArea->subWindowList().at(i)->findChild<QTextBrowser *>("textBrowser");
         vSlider <<  textBrowser->verticalScrollBar()->sliderPosition();
         hSlider << textBrowser->horizontalScrollBar()->sliderPosition();
    }
    m_settings->session.setData("windowUrls",windowUrls);
    m_settings->session.setData("windowGeo",windowGeo);
    m_settings->session.setData("vSlider",vSlider);
    m_settings->session.setData("hSlider",hSlider);

    m_settings->session.setData("viewMode",ui->mdiArea->viewMode());


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
    QStringList windowUrls = m_settings->session.getData("windowUrls").toStringList();
    QVariantList windowGeo = m_settings->session.getData("windowGeo").toList();
    QVariantList vSlider = m_settings->session.getData("vSlider").toList();
    QVariantList hSlider = m_settings->session.getData("hSlider").toList();
    for(int i = 0; i < windowUrls.size(); ++i) {
        newMdiChild(false);
        myDebug() << "current window is " << tabIDof(activeMdiChild()) << " while window count is " << usableWindowList();
        //load bible
        UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, windowUrls.at(i));
        urlConverter.m_biblesIniPath = m_moduleManager->m_bible.biblesIniPath;
        urlConverter.pharse();
        myDebug() << "url = " << urlConverter.convert();
        pharseUrl(urlConverter.convert());
        //set geometry
        activeMdiChild()->setGeometry(windowGeo.at(i).toRect());
        QTextBrowser *textBrowser =  activeMdiChild()->findChild<QTextBrowser *>("textBrowser");
        //set slider
        //todo: reallly stange
        textBrowser->verticalScrollBar()->setSliderPosition(vSlider.at(i).toInt());
        textBrowser->horizontalScrollBar()->setSliderPosition(hSlider.at(i).toInt());
    }
    if(m_settings->session.getData("viewMode").toInt() == 0)
        ui->mdiArea->setViewMode(QMdiArea::SubWindowView);
    else if (m_settings->session.getData("viewMode").toInt() == 1)
        ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    //restore
}
void AdvancedInterface::settingsChanged(Settings set)
{
    //DEBUG_FUNC_NAME
    //reload books
    bool reloadBibles = false;
    if (m_settings->encoding != set.encoding) {
        reloadBibles = true;
    }
    for (int i = 0; i < set.module.size(); ++i) {
        if (m_settings->module.size() < i || m_settings->module.empty()) {
            reloadBibles = true;
            break;
        } else {
            ModuleSettings m1, m2;
            m1 = set.module.at(i);
            m2 = m_settings->module.at(i);
            if (memcmp(&m1, &m2, sizeof(ModuleSettings))) {
                reloadBibles = true;
                break;
            }
        }
    }
    if (reloadBibles == true) {
        myDebug() << "reload Bibles";
        m_moduleManager->loadAllModules();
        m_moduleDockWidget->init();
        m_strongDockWidget->init();
        //ui->textBrowser->setHtml("");
        //todo: clear everything
    }
}
void AdvancedInterface::showSearchDialog()
{
    DEBUG_FUNC_NAME
    SearchDialog *sDialog = new SearchDialog(this);
    connect(sDialog, SIGNAL(searched(SearchQuery)), this, SLOT(search(SearchQuery)));
    QTextBrowser *t = getCurrentTextBrowser();
    if (t && t->textCursor().hasSelection() == true) {
        sDialog->setText(t->textCursor().selectedText());
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
    result = m_moduleManager->m_bible.search(query);
    m_moduleManager->m_bible.lastSearchResult = result;
    m_searchResultDockWidget->setSearchResult(result);
}
void AdvancedInterface::copy()
{
    if (activeMdiChild()) {
        QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
        textBrowser->copy();
    }
}
void AdvancedInterface::selectAll()
{
    if (activeMdiChild()) {
        QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
        textBrowser->selectAll();
    }
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
    QMenuBar *bar = new QMenuBar(this->parentWidget());
    QMenu *menuFile = new QMenu(tr("File"), bar);
    QAction *actionSaveAs = new QAction(QIcon(":/icons/16x16/document-save-as.png"), tr("Save As"), menuFile);
    connect(actionSaveAs,SIGNAL(triggered()),this,SLOT(saveFile()));

    QAction *actionPrint = new QAction(QIcon(":/icons/16x16/document-print.png"), tr("Print"), menuFile);
    connect(actionPrint,SIGNAL(triggered()),this,SLOT(printFile()));

    QAction *actionClose = new QAction(QIcon(":/icons/16x16/application-exit.png"), tr("Close"), menuFile);
    connect(actionClose,SIGNAL(triggered()),this->parentWidget(),SLOT(close()));
    menuFile->addAction(actionSaveAs);
    menuFile->addAction(actionPrint);
    menuFile->addSeparator();
    menuFile->addAction(actionClose);


    QMenu *menuEdit = new QMenu(tr("Edit"), bar);
    QAction *actionCopy = new QAction(QIcon(":/icons/16x16/edit-copy.png"), tr("Copy"), menuEdit);
    connect(actionCopy,SIGNAL(triggered()),this,SLOT(copy()));

    QAction *actionSelectAll = new QAction(QIcon(":/icons/16x16/edit-select-all.png"), tr("Select All"), menuEdit);
    connect(actionSelectAll,SIGNAL(triggered()),this,SLOT(selectAll()));

    QAction *actionSearch = new QAction(QIcon(":/icons/16x16/edit-find.png"), tr("Search"), menuEdit);
    connect(actionSearch,SIGNAL(triggered()),this,SLOT(showSearchDialog()));
    QAction *actionFindNext = new QAction(QIcon(":/icons/16x16/go-down-search.png"), tr("Find Next"), menuEdit);
    connect(actionFindNext,SIGNAL(triggered()),this,SLOT(nextVerse()));
    QAction *actionFindPrevious = new QAction(QIcon(":/icons/16x16/go-up-search.png"), tr("Find Previous"), menuEdit);
    connect(actionFindPrevious,SIGNAL(triggered()),this,SLOT(previousVerse()));

    QAction *actionConfiguration = new QAction(QIcon(":/icons/16x16/configure.png"), tr("Configuration"), menuEdit);
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

    QAction *actionZoomIn = new QAction(QIcon(":/icons/16x16/zoom-in.png"), tr("Zoom In"), menuView);
    connect(actionZoomIn,SIGNAL(triggered()),this,SLOT(zoomIn()));
    QAction *actionZoomOut = new QAction(QIcon(":/icons/16x16/zoom-out.png"), tr("Zoom Out"), menuView);
    connect(actionZoomOut,SIGNAL(triggered()),this,SLOT(zoomOut()));

    QAction *actionTabView = new QAction(QIcon(), tr("Tabbed View"), menuView);
    connect(actionTabView,SIGNAL(triggered()),this,SLOT(setTabView()));
    QAction *actionSubWindowView = new QAction(QIcon(), tr("Sub Window View"), menuView);
    connect(actionSubWindowView,SIGNAL(triggered()),this,SLOT(setSubWindowView()));

    QAction *actionTileVertical = new QAction(QIcon(), tr("Tile Vertical"), menuView);
    connect(actionTileVertical,SIGNAL(triggered()),this,SLOT(myTileVertical()));
    QAction *actionTileHorizontal = new QAction(QIcon(), tr("Tile Horizontal"), menuView);
    connect(actionTileHorizontal,SIGNAL(triggered()),this,SLOT(myTileHorizontal()));
    QAction *actionCascade= new QAction(QIcon(), tr("Cascade"), menuView);
    connect(actionCascade,SIGNAL(triggered()),this,SLOT(myCascade()));


    menuView->addAction(actionZoomIn);
    menuView->addAction(actionZoomOut);
    menuView->addSeparator();
    menuView->addAction(actionTabView);
    menuView->addAction(actionSubWindowView);
    menuView->addSeparator();
    menuView->addAction(actionTileVertical);
    menuView->addAction(actionTileHorizontal);
    menuView->addAction(actionCascade);

    QMenu *menuNotes = new QMenu(tr("Notes"), bar);
    QAction *actionNotesEditor = new QAction(QIcon(":/icons/16x16/notes-edit.png"), tr("Notes Editor"), menuNotes);
    connect(actionNotesEditor,SIGNAL(triggered()),this,SLOT(showNotesEditor()));

    QAction *actionMarkList = new QAction(QIcon(":/icons/16x16/table.png"), tr("Mark List"), menuNotes);
    connect(actionMarkList,SIGNAL(triggered()),this,SLOT(showMarkList()));
    QAction *actionMarkCategories = new QAction(QIcon(), tr("Mark Categories"), menuNotes);
    connect(actionMarkCategories,SIGNAL(triggered()),this,SLOT(showMarkCategories()));
    menuNotes->addAction(actionNotesEditor);
    menuNotes->addSeparator();
    menuNotes->addAction(actionMarkList);
    menuNotes->addAction(actionMarkCategories);

    QMenu *menuHelp = new QMenu(tr("Help"), bar);
    QAction *actionAbout = new QAction(QIcon(":/icons/16x16/help-about.png"), tr("About"), menuHelp);
    connect(actionAbout,SIGNAL(triggered()),this,SLOT(showAboutDialog()));
    QAction *actionOnlineHelp = new QAction(QIcon(":/icons/16x16/help-contents.png"), tr("Online Help"), menuHelp);
    connect(actionOnlineHelp,SIGNAL(triggered()),this,SLOT(onlineHelp()));

    menuHelp->addAction(actionOnlineHelp);
    menuHelp->addAction(actionAbout);
    bar->addMenu(menuFile);
    bar->addMenu(menuEdit);
    bar->addMenu(menuView);
    bar->addMenu(menuNotes);
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
    bar->setIconSize(QSize(32, 32));
    bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    QAction *actionSearch = new QAction(QIcon(":/icons/32x32/edit-find.png"), tr("Search"), bar);
    connect(actionSearch, SIGNAL(triggered()), this, SLOT(showSearchDialog()));

    QAction *actionBookmarks = new QAction(QIcon(":/icons/32x32/bookmarks-organize.png"), tr("Bookmarks"), bar);
    connect(actionBookmarks, SIGNAL(triggered()), this, SLOT(showBookmarksDock()));
    actionBookmarks->setCheckable(true);
    connect(m_bookmarksDockWidget,SIGNAL(visibilityChanged(bool)),actionBookmarks,SLOT(setChecked(bool)));

    QAction *actionNotes = new QAction(QIcon(":/icons/32x32/view-pim-notes.png"), tr("Notes"), bar);
    connect(actionNotes, SIGNAL(triggered()), this, SLOT(showNotesDock()));
    actionNotes->setCheckable(true);
    connect(m_notesDockWidget,SIGNAL(visibilityChanged(bool)),actionNotes,SLOT(setChecked(bool)));

    QAction *actionNewWindow = new QAction(QIcon(":/icons/32x32/window-new.png"), tr("New Window"), bar);
    connect(actionNewWindow, SIGNAL(triggered()), this, SLOT(newMdiChild()));

    QAction *actionZoomIn = new QAction(QIcon(":/icons/32x32/zoom-in.png"), tr("Zoom In"), bar);
    connect(actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
    QAction *actionZoomOut = new QAction(QIcon(":/icons/32x32/zoom-out.png"), tr("Zoom Out"), bar);
    connect(actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));

    QAction *actionModule = new QAction(QIcon(":/icons/32x32/module.png"), tr("Module"), bar);
    connect(actionModule, SIGNAL(triggered()), this->parent(), SLOT(showSettingsDialog_Module()));

    bar->addAction(actionSearch);
    bar->addSeparator();
    bar->addAction(actionNewWindow);
    bar->addSeparator();
    bar->addAction(actionBookmarks);
    bar->addAction(actionNotes);
    bar->addSeparator();
    bar->addAction(actionZoomIn);
    bar->addAction(actionZoomOut);
    bar->addSeparator();
    bar->addAction(actionModule);
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
    if(!m_moduleManager->bibleLoaded() && !activeMdiChild())
        return;
    m_bookmarksDockWidget->newBookmark(verseSelectionFromCursor(m_textCursor));
}
void AdvancedInterface::newNoteWithLink()
{
    if(!m_moduleManager->bibleLoaded() && !activeMdiChild())
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
        QTextBrowser *t = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
        t->print(&printer);
    }
    return 0;
}
int AdvancedInterface::saveFile(void)
{
    QFileDialog dialog(this);
    //dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Html (*.html *.htm);;PDF (*.pdf);;Plain (*.txt)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    QString fileName = QFileDialog::getSaveFileName();
    qDebug() << "MainWindow::saveFile() fileName = " << fileName;
    if (activeMdiChild()) {
        QTextBrowser *t = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
        if (fileName.endsWith(".html") || fileName.endsWith(".htm")) {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return 1;
            QTextStream out(&file);
            out << t->toHtml();
            file.close();
        } else if (fileName.endsWith(".pdf")) {
            QPrinter printer;
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(fileName);
            t->print(&printer);

        } else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return 1;
            QTextStream out(&file);
            out << t->toPlainText();
            file.close();
        }
    }
    return 0;
}

int AdvancedInterface::showAboutDialog(void)
{
    myDebug() << &m_notes;
    AboutDialog aDialog;
    aDialog.setWindowTitle(tr("About openBibleViewer"));
    aDialog.show();
    aDialog.setText(tr("openBibleViewer <br> version: %1 build: %2<br> <a href=\"http://openbv.uucyc.name/\"> Official Website</a> | <a href=\"http://openbv.uucyc.name/bug/\">Bug report</a>").arg("0.3.a1").arg(""));
    return aDialog.exec();
}
void AdvancedInterface::showMarkCategories()
{
}
void AdvancedInterface::showMarkList()
{
}
void AdvancedInterface::showNotesEditor()
{
    //m_notesDockWidget->saveNote();//todo: ugly

    NotesEditor notesEditor;
    myDebug() << &m_notes;
    notesEditor.setSettings(m_settings);
    notesEditor.setModuleManager(m_moduleManager);
    notesEditor.setNotes(m_notes);

    notesEditor.init();
    notesEditor.show();
    notesEditor.exec();
}
void AdvancedInterface::setTabView()
{
     ui->mdiArea->setViewMode(QMdiArea::TabbedView);
}
void AdvancedInterface::setSubWindowView()
{
    ui->mdiArea->setViewMode(QMdiArea::SubWindowView);
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
