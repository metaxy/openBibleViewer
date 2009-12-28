#include "simpleinterface.h"
#include "ui_simpleinterface.h"
#include "src/core/dbghelper.h"
#include <QtGui/QDesktopServices>
#include <QtGui/QMessageBox>
#include <QtGui/QKeyEvent>
SimpleInterface::SimpleInterface(QWidget *parent) :
        Interface(parent),
        ui(new Ui::SimpleInterface)
{
    ui->setupUi(this);
    ui->textBrowser->installEventFilter(this);
}
void SimpleInterface::setBookDockWidget(BookDockWidget *bookDockWidget)
{
    m_bookDockWidget = bookDockWidget;
}
void SimpleInterface::setModuleDockWidget(ModuleDockWidget *moduleDockWidget)
{
    m_moduleDockWidget = moduleDockWidget;
}

void SimpleInterface::init()
{
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
    m_notes = 0;
    m_moduleManager->m_bible.setSettings(m_settings);

    connect(m_bibleDisplay, SIGNAL(newHtml(QString)), this, SLOT(showText(QString)));
    connect(this, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

}
void SimpleInterface::loadModuleDataByID(int id)
{
    DEBUG_FUNC_NAME
    myDebug() << "id = " << id;
    if (m_moduleManager->m_moduleList.size() < id)//this bible dosent existist
        return;

    m_moduleManager->m_bible.setBibleType(m_moduleManager->m_moduleList.at(id).m_moduleClass);
    m_moduleManager->m_bible.loadBibleData(id, m_moduleManager->m_moduleList.at(id).m_iniPath);

    setTitle(m_moduleManager->m_bible.bibleName);
    setBooks(m_moduleManager->m_bible.bookFullName);

}
void SimpleInterface::pharseUrl(QUrl url)
{
    pharseUrl(url.toString());
}
void SimpleInterface::pharseUrl(QString url)
{
    DEBUG_FUNC_NAME
    myDebug() << "url = " << url;
    const QString bible = "bible://";
    const QString http = "http://";
    const QString bq = "go";
    const QString anchor = "#";
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
                    setCurrentBook(bookID);;
                } else if (bookID != m_moduleManager->m_bible.bookID()) {
                    readBookByID(bookID);
                    setCurrentBook(bookID);;
                }
                showChapter(chapterID + m_moduleManager->m_bible.chapterAdd(), verseID);
                setCurrentChapter(chapterID);
            } else {
                myDebug() << "invalid URL";
            }
        } else {
            myDebug() << "invalid URL";
        }
        //bible://bibleID/bookID,chapterID,verseID
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
        //emit historySetUrl(url_backup);

    } else if (url.startsWith(anchor)) {
        url = url.remove(0, anchor.size());
        myDebug() << "anchor";
        ui->textBrowser->scrollToAnchor(url);
    } else {
        myDebug() << " bookPath = " << m_moduleManager->m_bible.bookPath;
        if (m_moduleManager->m_bible.bibleType == 1 && m_moduleManager->m_bible.bookPath.contains(url)) {
            emit get("bible://current/" + m_moduleManager->m_bible.bookPath.lastIndexOf(url));//search in bible bookPath for this string, if it exixsts it is a book link
        } else {

            myDebug() << "invalid URL";
        }
    }
    return;
}
void SimpleInterface::showText(const QString &text)
{
    ui->textBrowser->setHtml(text);
    if (m_moduleManager->m_bible.m_verseID > 1)
        ui->textBrowser->scrollToAnchor("currentVerse");
}
void SimpleInterface::setTitle(const QString &title)
{
    //myDebug() << "title = " << title;
    setWindowTitle(title);
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
    if (id < 0) {
        QMessageBox::critical(0, tr("Error"), tr("This book is not available."));
        myDebug() << "invalid bookID";
        return;
    }
    if (id >= m_moduleManager->m_bible.bookFullName.size()) {
        QMessageBox::critical(0, tr("Error"), tr("This book is not available."));
        myDebug() << "invalid bookID";
        return;
    }
    if (m_moduleManager->m_bible.readBook(id) != 0) {
        QMessageBox::critical(0, tr("Error"), tr("Cannot read the book."));
        //error while reading
        return;
    }
    setChapters(m_moduleManager->m_bible.chapterNames);
    ui->textBrowser->setSearchPaths(m_moduleManager->m_bible.getSearchPaths());

}
void SimpleInterface::readChapter(const int &id)
{
    DEBUG_FUNC_NAME
    emit get("bible://current/" + QString::number(m_moduleManager->m_bible.bookID()) + "," + QString::number(id) + ",0");
}

void SimpleInterface::showChapter(const int &chapterID, const int &verseID)
{
    m_moduleManager->m_bible.m_verseID = verseID;
    m_bibleDisplay->setHtml((m_moduleManager->m_bible.readChapter(chapterID, verseID)));
    setCurrentChapter(chapterID - m_moduleManager->m_bible.chapterAdd());
}
void SimpleInterface::nextChapter()
{
    DEBUG_FUNC_NAME
    if (m_moduleManager->m_bible.chapterID() < m_moduleManager->m_bible.chapterData.size() - 1)
        readChapter(m_moduleManager->m_bible.chapterID() + 1);
}
void SimpleInterface::previousChapter()
{
    DEBUG_FUNC_NAME
    if (m_moduleManager->m_bible.chapterID() > 0)
        readChapter(m_moduleManager->m_bible.chapterID() - 1);
}
bool SimpleInterface::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->textBrowser) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Left) {
                previousChapter();
                return true;
            } else if (keyEvent->key() == Qt::Key_Right) {
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
}
SimpleInterface::~SimpleInterface()
{
    delete ui;
}

void SimpleInterface::changeEvent(QEvent *e)
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
