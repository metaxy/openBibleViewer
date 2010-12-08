#include "biblemanager.h"
#include "src/core/dbghelper.h"
#include "src/core/bible/bibleurl.h"
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
BibleManager::BibleManager(QObject *parent) :
    QObject(parent)
{
}
void BibleManager::setWidget(QWidget *p)
{
    m_p = p;
}
void BibleManager::init()
{

}
void BibleManager::createDocks()
{
    m_moduleDockWidget = new ModuleDockWidget(m_p);
    setAll(m_moduleDockWidget);
    m_moduleDockWidget->init();
    connect(m_moduleDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    m_bookDockWidget = new BookDockWidget(m_p);
    setAll(m_bookDockWidget);
    m_bookDockWidget->hide();
    connect(m_bookDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    m_advancedSearchResultDockWidget = new AdvancedSearchResultDockWidget(m_p);
    setAll(m_advancedSearchResultDockWidget);
    m_advancedSearchResultDockWidget->init();
    m_advancedSearchResultDockWidget->hide();
    connect(m_advancedSearchResultDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    m_quickJumpDockWidget = new QuickJumpDockWidget(m_p);
    setAll(m_quickJumpDockWidget);
    m_quickJumpDockWidget->init();
    m_quickJumpDockWidget->hide();
    connect(m_quickJumpDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));
}

QHash<DockWidget*, Qt::DockWidgetArea> BibleManager::docks()
{
    DEBUG_FUNC_NAME
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.insert(m_advancedSearchResultDockWidget, Qt::LeftDockWidgetArea);
    ret.insert(m_bookDockWidget, Qt::LeftDockWidgetArea);
    ret.insert(m_moduleDockWidget, Qt::LeftDockWidgetArea);
    ret.insert(m_quickJumpDockWidget, Qt::RightDockWidgetArea);
    return ret;

}
void BibleManager::pharseUrl(const QString &url)
{
    const QString bible = "bible://";
    const QString bq = "go";

    if(url.startsWith(bible)) {
        BibleUrl bibleUrl;
        if(!bibleUrl.fromString(url)) {
            return;
        }
        if(bibleUrl.bible() == BibleUrl::ReloadActive) {
            //m_windowManager->reloadActive();//todo:
        } else {
            bool reloadBible = false;
            bool reloadBook = false;
            bool reloadChapter = false;
            bool reloadVerse = false;
            /*if(!m_windowManager->activeMdiChild()) {
                m_windowManager->newSubWindow();//todo:
            }*/
            if((bibleUrl.bibleID() != m_moduleManager->bible()->moduleID()  && bibleUrl.bible() == BibleUrl::LoadBibleByID) ||
                    !m_moduleManager->bible()->loaded() || (bibleUrl.getParam("force") == "true")) {
                reloadBible = true;
            }

            if(reloadBible) {
                int id = -1;
                if(bibleUrl.bible() == BibleUrl::LoadBibleByID) {
                    id = bibleUrl.bibleID();
                } else if(bibleUrl.bible() == BibleUrl::LoadCurrentBible) {
                    id = m_moduleManager->bible()->moduleID();
                }
                myDebug() << "reloadBible with id = " << id;
                int ret = loadModuleDataByID(id);
                if(ret == 1) {
                    showText(tr("No such bible found."));
                    return;
                }
            }

            int firstBook = 0;
            if(bibleUrl.book() == BibleUrl::LoadFirstBook) {
                firstBook = m_moduleManager->bible()->bookIDs().first();
            }
            if((bibleUrl.bookID() != m_moduleManager->bible()->bookID() && bibleUrl.book() == BibleUrl::LoadBookByID) ||
                    (firstBook != m_moduleManager->bible()->bookID() && bibleUrl.book() == BibleUrl::LoadFirstBook) ||
                    reloadBible || bibleUrl.getParam("forceReloadBook") == "true") {
                reloadBook = true;
            }

            int firstChapter = 0;
            /*if(bibleUrl.chapter() == BibleUrl::LoadFirstChapter) {
                firstChapter = 0; // todo: change when we support to start chapters from another value
            }*/
            if((bibleUrl.chapterID() != m_moduleManager->bible()->chapterID() && bibleUrl.chapter() == BibleUrl::LoadChapterByID) ||
                    (firstChapter != m_moduleManager->bible()->chapterID() && bibleUrl.chapter() == BibleUrl::LoadFirstChapter) ||
                    reloadBible || reloadBook || bibleUrl.getParam("forceReloadChapter") == "true") {
                reloadChapter = true;
            }

            int firstVerse = 0;
            if(bibleUrl.verse() == BibleUrl::LoadFirstVerse) {
                firstVerse = 0; // todo: change when we support to start verse from another value
            }
            if(((bibleUrl.verseID() != m_moduleManager->bible()->verseID() && bibleUrl.verse() == BibleUrl::LoadVerseByID) ||
                    (firstVerse != m_moduleManager->bible()->verseID() && bibleUrl.verse() == BibleUrl::LoadFirstVerse))
                    && !reloadBible && !reloadBook && !reloadChapter) {
                reloadVerse = true;
            }

            if(reloadBook) {
                int bookID;
                if(bibleUrl.book() == BibleUrl::LoadFirstBook) {
                    bookID = firstBook;
                } else if(bibleUrl.book() == BibleUrl::LoadCurrentBook) {
                    bookID = m_moduleManager->bible()->bookID();
                } else {
                    bookID = bibleUrl.bookID();
                }
                readBookByID(bookID);
                emit setCurrentBook(bookID);
            }

            int chapterID;
            if(bibleUrl.chapter() == BibleUrl::LoadFirstChapter) {
                chapterID = firstChapter;
            } else if(bibleUrl.chapter() == BibleUrl::LoadCurrentChapter) {
                chapterID = m_moduleManager->bible()->chapterID();
            } else {
                chapterID = bibleUrl.chapterID();
            }

            int verseID;
            if(bibleUrl.verse() == BibleUrl::LoadFirstVerse) {
                verseID = firstVerse;
            } else if(bibleUrl.verse() == BibleUrl::LoadCurrentVerse) {
                verseID = m_moduleManager->bible()->verseID();
            } else {
                verseID = bibleUrl.verseID();
            }
            myDebug() << "chapterID = " << chapterID << " verseID = " << verseID;
            if(reloadChapter) {
                showChapter(chapterID, verseID);
                emit setCurrentChapter(chapterID);
            }
            if(reloadVerse) {
                showChapter(chapterID, verseID);
                emit setCurrentChapter(chapterID);
            }

            if(bibleUrl.hasParam("searchInCurrentText") && bibleUrl.getParam("searchInCurrentText") == "true") {
                //searchInText(m_moduleManager->bible()->lastSearchQuery());
                //todo:
            }
            //emit historySetUrl(url_backup);
        }
    } else if(url.startsWith(bq)) {
        //its a biblequote internal link, but i dont have the specifications!!!
        QStringList internal = url.split(" ");
        const QString bibleID = internal.at(1);//todo: use it
        //myDebug() << "bibleID = " << bibleID;
        int bookID = internal.at(2).toInt() - 1;
        int chapterID = internal.at(3).toInt() - 1;
        int verseID = internal.at(4).toInt();
        /*if(bibleID != m_moduleManager->bible()->bibleID())
        {
            loadModuleDataByID(bibleID);
            readBookByID(bookID);
            setCurrentBook(bookID);
            showChapter(chapterID),verseID);
            setCurrentChapter(chapterID);
            //load bible
        }
        else */if(bookID != m_moduleManager->bible()->bookID()) {
            readBookByID(bookID);
            setCurrentBook(bookID);
            showChapter(chapterID, verseID);
            setCurrentChapter(chapterID);
            //load book
        } else if(chapterID != m_moduleManager->bible()->chapterID()) {
            showChapter(chapterID, verseID);
            setCurrentChapter(chapterID);
            //load chapter
        } else {
            showChapter(chapterID, verseID);
            setCurrentChapter(chapterID);
        }
        //emit historySetUrl(url_backup);//todo:
    }
}

bool BibleManager::loadModuleDataByID(const int &moduleID)
{
    DEBUG_FUNC_NAME
    myDebug() << "id = " << moduleID;
    //open a new window if they are none
   /* if(ui->mdiArea->subWindowList().size() == 0)
        m_windowManager->newSubWindow();*/ //todo:

    if(moduleID < 0 || !m_moduleManager->contains(moduleID)) {
        myWarning() << "failed id = " << moduleID << m_moduleManager->m_moduleMap->m_map;
        return 1;
    }
    if(m_moduleManager->getModule(moduleID)->m_moduleClass != Module::BibleModuleClass) {
        myWarning() << "non bible module " << m_moduleManager->getModule(moduleID)->m_moduleClass;
        return 1;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);

    Module::ModuleType type = m_moduleManager->getModule(moduleID)->m_moduleType;
    m_moduleManager->bible()->setModuleType(type);
    m_moduleManager->bible()->loadModuleData(moduleID);

    emit setTitle(m_moduleManager->bible()->bibleTitle());
    emit updateBooks(m_moduleManager->bible()->bookNames(), m_moduleManager->bible()->bookIDs());
    m_moduleDockWidget->loadedModule(moduleID);//select current Module
    QApplication::restoreOverrideCursor();
    return 0;

}
/*void BibleManager::readBook(const int &id)
{
    myDebug() << "id = " << id;
    BibleUrl url;
    url.setBible(BibleUrl::LoadCurrentBible);
    url.setBookID(id);
    url.setChapter(BibleUrl::LoadFirstChapter);
    url.setVerse(BibleUrl::LoadFirstVerse);
    emit get(url.toString());
}*/

void BibleManager::readBookByID(const int &id)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(id < 0) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(0, tr("Error"), tr("This book is not available."));
        myWarning() << "invalid bookID - 1";
        return;
    }
    if(!m_moduleManager->bible()->bookIDs().contains(id)) {
        myWarning() << "invalid bookID - 2(no book loaded) id = " << id << " count = " << m_moduleManager->bible()->booksCount();
    }
    const int read = m_moduleManager->bibleList()->readBook(id);
    if(read != 0) {
        QApplication::restoreOverrideCursor();
        if(read == 2) {
            emit updateChapters();
        } else {
            QMessageBox::critical(0, tr("Error"), tr("Cannot read the book."));
        }
        myWarning() << "read = " << read;
        //error while reading
        return;
    }
    QApplication::restoreOverrideCursor();
    emit updateChapters(m_moduleManager->bible()->chapterNames());
}

/*void BibleManager::readChapter(const int &id)
{
    DEBUG_FUNC_NAME
    myDebug() << "id = " << id;
    BibleUrl url;
    url.setBible(BibleUrl::LoadCurrentBible);
    url.setBook(BibleUrl::LoadCurrentBook);
    url.setChapterID(id);
    url.setVerse(BibleUrl::LoadCurrentVerse);
    emit get(url.toString());
}
*/
void BibleManager::showChapter(const int &chapterID, const int &verseID)
{
    DEBUG_FUNC_NAME
    myDebug() << "chapter ID = " << chapterID << "verse ID = " << verseID;
    m_bibleDisplay->setHtml(m_moduleManager->bibleList()->readChapter(chapterID, verseID));
    emit setCurrentChapter(chapterID);
}

void BibleManager::nextChapter()
{
    if(m_moduleManager->bible()->chapterID() < m_moduleManager->bible()->chaptersCount() - 1) {
        BibleUrl burl;
        burl.setBible(BibleUrl::LoadCurrentBible);
        burl.setBook(BibleUrl::LoadCurrentBook);
        burl.setChapterID(m_moduleManager->bible()->chapterID() + 1);
        burl.setVerse(BibleUrl::LoadFirstVerse);
        const QString url = burl.toString();
        emit get(url);
    } else if(m_moduleManager->bible()->bookID() < m_moduleManager->bible()->booksCount() - 1) {
        BibleUrl burl;
        burl.setBible(BibleUrl::LoadCurrentBible);
        burl.setBookID(m_moduleManager->bible()->bookID() + 1);
        burl.setChapter(BibleUrl::LoadFirstChapter);
        burl.setVerse(BibleUrl::LoadFirstVerse);
        const QString url = burl.toString();
        emit get(url);
    }
}

void BibleManager::previousChapter()
{
    if(m_moduleManager->bible()->chapterID() > 0) {
        BibleUrl burl;
        burl.setBible(BibleUrl::LoadCurrentBible);
        burl.setBook(BibleUrl::LoadCurrentBook);
        burl.setChapterID(m_moduleManager->bible()->chapterID() - 1);
        burl.setVerse(BibleUrl::LoadFirstVerse);
        const QString url = burl.toString();
        emit get(url);
    } else if(m_moduleManager->bible()->bookID() > 0) {
        BibleUrl burl;
        burl.setBible(BibleUrl::LoadCurrentBible);
        burl.setBookID(m_moduleManager->bible()->bookID() - 1);
        burl.setChapterID(m_moduleManager->bible()->chaptersCount() - 1);//todo: use lastchapter
        burl.setVerse(BibleUrl::LoadFirstVerse);
        const QString url = burl.toString();
        emit get(url);
    }
}

void BibleManager::reloadChapter(bool full)
{
    //todo:
  /*  DEBUG_FUNC_NAME
    if(!m_windowManager->activeMdiChild())
        return;
    //setEnableReload(false);
    //todo: hacky
    const QWebView *v = m_windowManager->activeForm()->m_view;
    const QPoint p = v->page()->mainFrame()->scrollPosition();
    myDebug() << m_moduleManager->bible()->moduleID();
    if(full) {
        BibleUrl url;
        url.setBible(BibleUrl::LoadCurrentBible);
        url.setBook(BibleUrl::LoadCurrentBook);
        url.setChapter(BibleUrl::LoadCurrentChapter);
        url.setVerse(BibleUrl::LoadCurrentVerse);
        url.setParam("force", "true");
        emit get(url.toString());
    } else {
        BibleUrl url;
        url.setBible(BibleUrl::LoadCurrentBible);
        url.setBook(BibleUrl::LoadCurrentBook);
        url.setChapter(BibleUrl::LoadCurrentChapter);
        url.setVerse(BibleUrl::LoadCurrentVerse);
        url.setParam("forceReloadChapter", "true");
        emit get(url.toString());
    }
    v->page()->mainFrame()->setScrollPosition(p);
    //setEnableReload(true);*/
}
