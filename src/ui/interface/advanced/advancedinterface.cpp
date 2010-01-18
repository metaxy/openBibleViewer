#include "advancedinterface.h"
#include "ui_advancedinterface.h"
#include "src/core/dbghelper.h"
#include <QtGui/QVBoxLayout>
#include <QtGui/QSizePolicy>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopServices>
#include <QtGui/QClipboard>
#include <QtGui/QTextDocumentFragment>
#include <QtGui/QScrollBar>
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
    connect(m_notesDockWidget,SIGNAL(reloadChapter()),this,SLOT(reloadChapter()));

    connect(m_bibleDisplay, SIGNAL(newHtml(QString)), this, SLOT(showText(QString)));
    connect(this, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    BibleDisplaySettings bibleDisplaySettings;
    bibleDisplaySettings.loadNotes = true;
    bibleDisplaySettings.showMarks = true;
    bibleDisplaySettings.showNotes = true;
    m_moduleManager->m_bible.setBibleDisplaySettings(bibleDisplaySettings);

    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(reloadWindow(QMdiSubWindow *)));

    newMdiChild();

}


void AdvancedInterface::newMdiChild()
{
    DEBUG_FUNC_NAME
    int windowsCount = usableWindowList().size();
    QMdiSubWindow *firstSubWindow = new QMdiSubWindow();
    if (windowsCount == 1) {
        firstSubWindow = usableWindowList().at(0);
    }
    m_windowCache.newWindow();
    QWidget *widget = new QWidget(ui->mdiArea);
    widget->setMinimumHeight(0);
    widget->setMinimumWidth(0);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    MdiForm *mForm = new MdiForm(this);
    layout->addWidget(mForm);
    widget->setLayout(layout);
    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(widget);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->setWindowTitle("");
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

    if (windowsCount >= 1) {
        if (m_settings->autoLayout == 1) {
            myTileVertical();
        } else if (m_settings->autoLayout == 2) {
            myTileHorizontal();
        } else if (m_settings->autoLayout == 3) {
            myCascade();
        }
    }

    m_internalWindows << subWindow;
    m_enableReload = true;
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
    if (!m_enableReload ||  !usableWindowList().count()) {
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
    QList<QMdiSubWindow*> ret;
    foreach(QMdiSubWindow* w, ui->mdiArea->subWindowList()) {
        if (w->isMinimized() /*|| w->isHidden()*/) //not usable for us
            continue;
        ret.append(w);
    }
    return ret;
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
    DEBUG_FUNC_NAME
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

    qDebug() << "MainWindow::reloadWindow() setCurrentTab id = " << id;
   /* m_windowCache.setCurrentWindowID(id);
    if (m_windowCache.getBible().m_bibleType == Bible::None) { //probaly no bible loaded in this window
        //myDebug() << "m_windowCache.getBibleType() == 0";
        ui->listWidget_chapters->clear();
        ui->listWidget_books->clear();
        m_moduleManager->m_bible.setBibleID(-2);
    } else {
        if (m_moduleManager->m_bible.bibleID() == m_windowCache.getBible().bibleID())
            return 1;
        m_moduleManager->m_bible = m_windowCache.getBible();
       // setTitle(m_windowCache.getBibleName());
        myDebug() << "m_moduleManager->m_bible.chapterNames.size() = " << m_moduleManager->m_bible.chapterNames.size() << " m_moduleManager->m_bible.chapterID() = " << m_moduleManager->m_bible.chapterID();
       // setChapters(m_moduleManager->m_bible.chapterNames);
        ///setCurrentChapter(m_moduleManager->m_bible.chapterID());

       // QStringList tbookFullName = m_windowCache.getBooks();//show all books
       // setBooks(tbookFullName);
       // setCurrentBook(m_windowCache.getCurrentBook());
    }*/

    return 0;
}
QTextBrowser* AdvancedInterface::getCurrentTextBrowser()
{
    if (activeMdiChild()) {
        QTextBrowser *t = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
        return t;
    }
}
void AdvancedInterface::setEnableReload(bool enable)
{
    m_enableReload = enable;
}
void AdvancedInterface::zoomIn()
{
    QTextBrowser *t = getCurrentTextBrowser();
    if(t) {
        t->zoomIn();
    }

}
void AdvancedInterface::zoomOut()
{
    QTextBrowser *t = getCurrentTextBrowser();
    if(t) {
        t->zoomOut();
    }
}
void AdvancedInterface::loadModuleDataByID(int id)
{
    DEBUG_FUNC_NAME
    myDebug() << "id = " << id;
    if (m_moduleManager->m_moduleList.size() < id)
        return;


    m_moduleManager->m_bible.setBibleType(m_moduleManager->m_moduleList.at(id).m_moduleType);
    m_moduleManager->m_bible.loadBibleData(id, m_moduleManager->m_moduleList.at(id).m_iniPath);
    m_moduleManager->m_bible.setSoftCache(m_windowCache.getSoftCache(id));//todo: if it is empty then do nothing

    m_windowCache.setCurrentWindowID(currentWindowID());
    m_windowCache.setBible(m_moduleManager->m_bible);//todo: !before loading an another bible, save the last

    setTitle(m_moduleManager->m_bible.bibleName);
    setBooks(m_moduleManager->m_bible.bookFullName);

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
        //showStrong(url);
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
        myDebug() << "anchor";
        /* if (url.contains("\"")) {
             url = url.remove("\"");
         }*/
        if (activeMdiChild()) {
            QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
            textBrowser->scrollToAnchor(url);
        }
    }  else if (url.startsWith(note)) {
        url = url.remove(0, note.size());
        //showNote(url);
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
    QTextBrowser *t = getCurrentTextBrowser();
    if(t) {
        t->setHtml(text);
        if (m_moduleManager->m_bible.m_verseID > 1)
            t->scrollToAnchor("currentVerse");
    }
}
void AdvancedInterface::setTitle(const QString &title)
{
    this->parentWidget()->setWindowTitle(title);//todo: + programmtitle
}

void AdvancedInterface::setChapters(const QStringList &chapters)
{
    m_bookDockWidget->setChapters(chapters);
}

void AdvancedInterface::setBooks(const QStringList &books)
{
    m_bookDockWidget->setBooks(books);
}

void AdvancedInterface::setCurrentBook(const int &bookID)
{
    m_bookDockWidget->setCurrentBook(bookID);
}

void AdvancedInterface::setCurrentChapter(const int &chapterID)
{
    m_bookDockWidget->setCurrentChapter(chapterID);
}
void AdvancedInterface::readBook(const int &id)
{
    myDebug() << "id = " << id;
    emit get("bible://current/" + QString::number(id) + ",0,0");
}
void AdvancedInterface::readBookByID(int id)
{
    myDebug() << "id = " << id;
    if (id < 0) {
        QMessageBox::critical(0, tr("Error"), tr("This book is not available."));
        myDebug() << "invalid bookID - 1";
        return;
    }
    if (id >= m_moduleManager->m_bible.bookFullName.size()) {
        QMessageBox::critical(0, tr("Error"), tr("This book is not available."));
        myDebug() << "invalid bookID - 2(no book loaded)";
        return;
    }
    if (m_moduleManager->m_bible.readBook(id) != 0) {
        QMessageBox::critical(0, tr("Error"), tr("Cannot read the book."));
        //error while reading
        return;
    }
    setChapters(m_moduleManager->m_bible.chapterNames);
    QTextBrowser *t = getCurrentTextBrowser();
    if(t) {
        t->setSearchPaths(m_moduleManager->m_bible.getSearchPaths());
    }

}
void AdvancedInterface::readChapter(const int &id)
{
    DEBUG_FUNC_NAME
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
    DEBUG_FUNC_NAME
    if (m_moduleManager->m_bible.chapterID() < m_moduleManager->m_bible.chapterData.size() - 1)
        readChapter(m_moduleManager->m_bible.chapterID() + 1);
}
void AdvancedInterface::previousChapter()
{
    DEBUG_FUNC_NAME
    if (m_moduleManager->m_bible.chapterID() > 0)
        readChapter(m_moduleManager->m_bible.chapterID() - 1);
}
VerseSelection AdvancedInterface::verseSelectionFromCursor(QTextCursor cursor)
{
    VerseSelection selection;
    DEBUG_FUNC_NAME
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
    //currentTextCursor = cursor;
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
   // connect(actionRemoveMark, SIGNAL(triggered()), this , SLOT(removeMark()));

    QAction *actionBookmark = new QAction(this);
    actionBookmark->setText(tr("Add Bookmark"));
    QIcon bookmarkIcon;
    bookmarkIcon.addPixmap(QPixmap(":/icons/16x16/bookmark-new.png"), QIcon::Normal, QIcon::Off);
    actionBookmark->setIcon(bookmarkIcon);
   // connect(actionBookmark, SIGNAL(triggered()), this , SLOT(newBookmark()));

    QAction *actionNote = new QAction(this);
    actionNote->setText(tr("Add Note"));
    QIcon noteIcon;
    noteIcon.addPixmap(QPixmap(":/icons/16x16/view-pim-notes.png"), QIcon::Normal, QIcon::Off);
    actionNote->setIcon(noteIcon);
   // connect(actionNote, SIGNAL(triggered()), this , SLOT(newNoteWithLink()));

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
    m_notesDockWidget->newMark(selection,QColor(255, 255, 0));
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
    m_notesDockWidget->newMark(selection,QColor(146, 243, 54));
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
    m_notesDockWidget->newMark(selection,QColor(77, 169, 243));
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
    m_notesDockWidget->newMark(selection,QColor(243, 181, 57));
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
    m_notesDockWidget->newMark(selection,QColor(169, 102, 240));
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

AdvancedInterface::~AdvancedInterface()
{
    delete ui;
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
