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
#include "simpleinterface.h"
#include "ui_simpleinterface.h"
#include "src/core/dbghelper.h"
#include "src/ui/dialog/searchdialog.h"
#include "src/core/bibledisplaysettings.h"
#include <QtGui/QDesktopServices>
#include <QtGui/QMessageBox>
#include <QtGui/QKeyEvent>
#include "src/core/core.h"
SimpleInterface::SimpleInterface(QWidget *parent) :
    Interface(parent),
    ui(new Ui::SimpleInterface)
{

    ui->setupUi(this);
    ui->textBrowser->installEventFilter(this);
    connect(ui->textBrowser, SIGNAL(anchorClicked(QUrl)), this, SLOT(pharseUrl(QUrl)));

}
void SimpleInterface::setBookDockWidget(BookDockWidget *bookDockWidget)
{
    m_bookDockWidget = bookDockWidget;
}
void SimpleInterface::setModuleDockWidget(ModuleDockWidget *moduleDockWidget)
{
    m_moduleDockWidget = moduleDockWidget;
}
void SimpleInterface::setSearchResultDockWidget(SearchResultDockWidget *searchResultDockWidget)
{
    m_searchResultDockWidget = searchResultDockWidget;
}

void SimpleInterface::init()
{
    m_moduleManager->m_bible = new Bible();
    m_moduleManager->initBible();
    m_moduleManager->m_bibleList = new BibleList();

    m_moduleManager->bibleList()->addBible(m_moduleManager->m_bible, QPoint(0, 0));

    m_moduleManager->bible()->setSettings(m_settings);
    setAll(m_moduleDockWidget);
    m_moduleDockWidget->init();
    connect(m_moduleDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    setAll(m_bookDockWidget);
    connect(m_bookDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    setAll(m_searchResultDockWidget);
    m_searchResultDockWidget->hide();
    connect(m_searchResultDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    connect(m_bibleDisplay, SIGNAL(newHtml(QString)), this, SLOT(showText(QString)));
    connect(this, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    BibleDisplaySettings *bibleDisplaySettings = new BibleDisplaySettings();
    bibleDisplaySettings->loadNotes = false;
    bibleDisplaySettings->showMarks = false;
    bibleDisplaySettings->showNotes = false;
    m_moduleManager->bible()->setBibleDisplaySettings(bibleDisplaySettings);

}
bool SimpleInterface::hasMenuBar()
{
    return false;
}
QMenuBar* SimpleInterface::menuBar()
{
    return 0;
}
bool SimpleInterface::hasToolBar()
{
    return true;
}
QToolBar * SimpleInterface::toolBar()
{
    QToolBar *bar = new QToolBar(this->parentWidget());
    bar->setObjectName("mainToolBar");
    bar->setIconSize(QSize(32, 32));
#if QT_VERSION >= 0x040600
    bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
#else
    bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
#endif

    QAction *actionSearch = new QAction(QIcon::fromTheme("edit-find", QIcon(":/icons/32x32/edit-find.png")), tr("Search"), bar);
    connect(actionSearch, SIGNAL(triggered()), this, SLOT(showSearchDialog()));
    QAction *actionZoomIn = new QAction(QIcon::fromTheme("zoom-in", QIcon(":/icons/32x32/zoom-in.png")), tr("Zoom In"), bar);
    connect(actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
    QAction *actionZoomOut = new QAction(QIcon::fromTheme("zoom-out", QIcon(":/icons/32x32/zoom-out.png")), tr("Zoom Out"), bar);
    connect(actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
    QAction *actionModule = new QAction(QIcon(":/icons/32x32/module.png"), tr("Module"), bar);
    connect(actionModule, SIGNAL(triggered()), this->parent(), SLOT(showSettingsDialog_Module()));
    bar->addAction(actionSearch);
    bar->addSeparator();
    bar->addAction(actionZoomIn);
    bar->addAction(actionZoomOut);
    bar->addSeparator();
    bar->addAction(actionModule);
    return bar;
}
void SimpleInterface::zoomIn()
{
    ui->textBrowser->zoomIn();
}
void SimpleInterface::zoomOut()
{
    ui->textBrowser->zoomOut();
}
void SimpleInterface::loadModuleDataByID(int id)
{
    //DEBUG_FUNC_NAME
    myDebug() << "id = " << id;
    if(id < 0 || !m_moduleManager->contains(id))
        return;
    Module::ModuleType type = m_moduleManager->getModule(id)->m_moduleType;
    m_moduleManager->bible()->setModuleType(type);

    m_moduleManager->bible()->loadModuleData(id);

    setTitle(m_moduleManager->bible()->bibleTitle());
    setBooks(m_moduleManager->bible()->bookFullName());

}
void SimpleInterface::pharseUrl(QUrl url)
{
    pharseUrl(url.toString());
}
void SimpleInterface::pharseUrl(QString url)
{
    //DEBUG_FUNC_NAME
    myDebug() << "url = " << url;
    const QString bible = "bible://";
    const QString http = "http://";
    const QString bq = "go";
    const QString anchor = "#";
    if(url.startsWith(bible)) {
        url = url.remove(0, bible.size());
        QStringList a = url.split("/");
        if(a.size() == 2) {
            QStringList c = a.at(1).split(",");
            if(c.size() >= 3) {
                int bibleID;
                if(a.at(0) == "current") {
                    bibleID = m_moduleManager->bible()->moduleID();
                } else {
                    bibleID = a.at(0).toInt();
                }
                int bookID = c.at(0).toInt();
                int chapterID = c.at(1).toInt();
                int verseID = c.at(2).toInt();
                if(bibleID != m_moduleManager->bible()->moduleID()) {
                    loadModuleDataByID(bibleID);
                    readBookByID(bookID);
                    setCurrentBook(bookID);;
                } else if(bookID != m_moduleManager->bible()->bookID()) {
                    readBookByID(bookID);
                    setCurrentBook(bookID);;
                }
                showChapter(chapterID, verseID);
                setCurrentChapter(chapterID);
            } else {
                myWarning() << "invalid URL";
            }
        } else {
            myWarning() << "invalid URL";
        }
        //bible://bibleID/bookID,chapterID,verseID
    } else if(url.startsWith(http)) {
        QDesktopServices::openUrl(url);
        //its a web link
    } else if(url.startsWith(bq)) {
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
            showChapter(chapterID,verseID);
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
        //emit historySetUrl(url_backup);

    } else if(url.startsWith(anchor)) {
        url = url.remove(0, anchor.size());

        bool ok;
        int c = url.toInt(&ok, 10);
        myDebug() << "c = " << c;
        if(ok && c < m_moduleManager->bible()->chaptersCount() && m_moduleManager->bible()->bibleType() == Module::BibleQuoteModule && m_moduleManager->bible()->chapterID() != c) {
            myDebug() << "bq chapter link";
            showChapter(c, 0);
            setCurrentChapter(c);
            /*
            m_moduleManager->bible()->readChapter(c, 0);*/
        } else {
            myDebug() << "anchor";
            ui->textBrowser->scrollToAnchor(url);
        }
    } else {
        if(m_moduleManager->bible()->bibleType() == Module::BibleQuoteModule && m_moduleManager->bible()->bookPath().contains(url)) {
            emit get("bible://current/" + m_moduleManager->bible()->bookPath().lastIndexOf(url));//search in bible bookPath for this string, if it exixsts it is a book link
        } else {
            myWarning() << "invalid URL";
        }
    }
    return;
}
void SimpleInterface::showText(const QString &text)
{
    ui->textBrowser->setHtml(text);
    if(m_moduleManager->bible()->verseID() > 1)
        ui->textBrowser->scrollToAnchor("currentVerse");
}
void SimpleInterface::setTitle(const QString &title)
{
    this->parentWidget()->setWindowTitle(title + tr("openBibleViewer"));
}

void SimpleInterface::setChapters(const QStringList &chapters)
{
    m_bookDockWidget->setChapters(chapters);
}

void SimpleInterface::setBooks(const QStringList &books)
{
    m_bookDockWidget->setBooks(books);
}

void SimpleInterface::setCurrentBook(const int &bookID)
{
    m_bookDockWidget->setCurrentBook(bookID);
}

void SimpleInterface::setCurrentChapter(const int &chapterID)
{
    m_bookDockWidget->setCurrentChapter(chapterID);
}
void SimpleInterface::readBook(const int &id)
{
    myDebug() << "id = " << id;
    emit get("bible://current/" + QString::number(id) + ",0,0");
}
void SimpleInterface::readBookByID(int id)
{
    myDebug() << "id = " << id;
    if(id < 0) {
        QMessageBox::critical(0, tr("Error"), tr("This book is not available."));
        myWarning() << "invalid bookID - 1";
        return;
    }
    if(id >= m_moduleManager->bible()->booksCount()) {
        QMessageBox::critical(0, tr("Error"), tr("This book is not available."));
        myWarning() << "invalid bookID - 2(no book loaded)";
        return;
    }
    if(m_moduleManager->bible()->readBook(id) != 0) {
        QMessageBox::critical(0, tr("Error"), tr("Cannot read the book."));
        //error while reading
        return;
    }
    setChapters(m_moduleManager->bible()->chapterNames());
    ui->textBrowser->setSearchPaths(m_moduleManager->bible()->getSearchPaths());

}
void SimpleInterface::readChapter(const int &id)
{
    //DEBUG_FUNC_NAME
    emit get("bible://current/" + QString::number(m_moduleManager->bible()->bookID()) + "," + QString::number(id) + ",0");
}

void SimpleInterface::showChapter(int chapterID, int verseID)
{
    //m_moduleManager->bible()->verseID() = verseID;//todo: check
    m_bibleDisplay->setHtml((m_moduleManager->bible()->readChapter(chapterID, verseID)));
    setCurrentChapter(chapterID);
}
void SimpleInterface::nextChapter()
{
    //DEBUG_FUNC_NAME
    if(m_moduleManager->bible()->chapterID() < m_moduleManager->bible()->chaptersCount() - 1) {
        readChapter(m_moduleManager->bible()->chapterID() + 1);
    } else if(m_moduleManager->bible()->bookID() < m_moduleManager->bible()->booksCount() - 1) {
        readBook(m_moduleManager->bible()->bookID() + 1);
    }
}
void SimpleInterface::previousChapter()
{
    //DEBUG_FUNC_NAME
    if(m_moduleManager->bible()->chapterID() > 0) {
        readChapter(m_moduleManager->bible()->chapterID() - 1);
    } else if(m_moduleManager->bible()->bookID() > 0) {
        readBook(m_moduleManager->bible()->bookID() - 1);
        readChapter(m_moduleManager->bible()->chaptersCount() - 1);
    }
}
bool SimpleInterface::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->textBrowser) {
        if(event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->key() == Qt::Key_Left) {
                previousChapter();
                return true;
            } else if(keyEvent->key() == Qt::Key_Right) {
                nextChapter();
                return true;
            } else {
                return QWidget::eventFilter(obj, event);
            }

        } else {
            return QWidget::eventFilter(obj, event);;
        }
    } else {
        return QWidget::eventFilter(obj, event);
    }
    return QWidget::eventFilter(obj, event);
}

SimpleInterface::~SimpleInterface()
{
    delete ui;
}
void SimpleInterface::settingsChanged(Settings oldSettings, Settings newSettings)
{
    //DEBUG_FUNC_NAME
    //reload books
    bool reloadBibles = false;
    if(oldSettings.encoding != newSettings.encoding) {
        reloadBibles = true;
    }
    for(int i = 0; i < newSettings.m_moduleSettings.size(); ++i) {
        if(oldSettings.m_moduleSettings.size() < i || oldSettings.m_moduleSettings.empty()) {
            reloadBibles = true;
            break;
        } else {
            ModuleSettings m1, m2;
            m1 = newSettings.m_moduleSettings.at(i);
            m2 = oldSettings.m_moduleSettings.at(i);
            if(memcmp(&m1, &m2, sizeof(ModuleSettings))) {
                reloadBibles = true;
                break;
            }
        }
    }
    if(reloadBibles == true) {
        myDebug() << "reload Bibles";
        m_moduleManager->loadAllModules();
        m_moduleDockWidget->init();
        ui->textBrowser->setHtml("");
        //todo: clear everything
    }

}
void SimpleInterface::showSearchDialog()
{
    //DEBUG_FUNC_NAME
    SearchDialog *sDialog = new SearchDialog(this);
    connect(sDialog, SIGNAL(searched(SearchQuery)), this, SLOT(search(SearchQuery)));
    if(ui->textBrowser->textCursor().hasSelection() == true) //something is selected
        sDialog->setText(ui->textBrowser->textCursor().selectedText());
    sDialog->show();
    sDialog->exec();
}
void SimpleInterface::search(SearchQuery query)
{
    //DEBUG_FUNC_NAME
    if(!m_moduleManager->bibleLoaded())
        return;
    m_searchResultDockWidget->show();
    SearchResult result;
    result = m_moduleManager->bible()->search(query);
    m_searchResultDockWidget->setSearchResult(result);
}
void SimpleInterface::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
