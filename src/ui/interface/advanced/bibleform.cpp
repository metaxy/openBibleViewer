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
#include "bibleform.h"
#include "ui_bibleform.h"
#include "src/core/dbghelper.h"
#include "biblelistwidget.h"
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QCursor>
#include <QtGui/QMessageBox>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElementCollection>
#include <QtWebKit/QWebElement>
#include <QtCore/QDir>
#include "src/core/core.h"
#include "src/core/bible/bibleurl.h"
BibleForm::BibleForm(QWidget *parent) : QWidget(parent), m_ui(new Ui::BibleForm)
{
    m_ui->setupUi(this);

    m_view = new WebView(this);
    m_view->setObjectName("webView");
    m_view->setUrl(QUrl("about:blank"));
    m_ui->verticalLayout->addWidget(m_view);

    m_view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(m_ui->toolButton_backward, SIGNAL(clicked()), this, SLOT(backward()));
    connect(m_ui->toolButton_forward, SIGNAL(clicked()), this, SLOT(forward()));
    connect(m_ui->toolButton_bibleList, SIGNAL(clicked()), this, SLOT(showBibleListMenu()));

    m_ui->toolButton_backward->setShortcut(QKeySequence::Back);
    m_ui->toolButton_forward->setShortcut(QKeySequence::Forward);

    connect(m_ui->comboBox_books, SIGNAL(activated(int)), this, SLOT(readBook(int)));
    connect(m_ui->comboBox_chapters, SIGNAL(activated(int)), this, SLOT(readChapter(int)));

    setButtons();

}
void BibleForm::init()
{
    m_moduleManager->m_bibleList = new BibleList();
    Bible *b = new Bible();
    m_moduleManager->initBible(b);
    m_moduleManager->bibleList()->addBible(b, QPoint(0, 0));
    m_bibleList = m_moduleManager->m_bibleList;
    attachApi();
    connect(m_view->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attachApi()));
}
void BibleForm::setApi(Api *api)
{
    m_api = api;
}

void BibleForm::attachApi()
{
    QWebFrame * frame = m_view->page()->mainFrame();
    /*{
        QFile file(":/data/js/jquery-1.4.2.min.js");
        if(!file.open(QFile::ReadOnly | QFile::Text))
            return;
        QTextStream stream(&file);
        QString jquery = stream.readAll();
        file.close();
        frame->evaluateJavaScript(jquery);
    }
    */
    {
        QFile file(":/data/js/tools.js");
        if(!file.open(QFile::ReadOnly | QFile::Text))
            return;
        QTextStream stream(&file);
        QString tools = stream.readAll();
        file.close();
        frame->evaluateJavaScript(tools);
    }

    frame->addToJavaScriptWindowObject("Bible", m_api->bibleApi());
    m_api->bibleApi()->setFrame(frame);//todo: use it on activated
}

void BibleForm::historyGetUrl(QString url)
{
    browserHistory.setCurrent(url);
    setButtons();
}
void BibleForm::backward()
{
    emit historyGo(browserHistory.backward());
    setButtons();
}
void BibleForm::forward()
{
    emit historyGo(browserHistory.forward());
    setButtons();
}
void BibleForm::setButtons()
{
    if(browserHistory.backwardAvailable()) {
        m_ui->toolButton_backward->setDisabled(false);
    } else {
        m_ui->toolButton_backward->setDisabled(true);
    }
    if(browserHistory.forwardAvailable()) {
        m_ui->toolButton_forward->setDisabled(false);
    } else {
        m_ui->toolButton_forward->setDisabled(true);
    }
}

void BibleForm::showBibleListMenu()
{
    BibleListWidget *w = new BibleListWidget(this);
    setAll(w);
    w->init();
    w->exec();
}

void BibleForm::readChapter(int id)
{
    BibleUrl url;
    url.setBible(BibleUrl::LoadCurrentBible);
    url.setBook(BibleUrl::LoadCurrentBook);
    url.setChapterID(id);
    url.setVerse(BibleUrl::LoadCurrentVerse);
    m_bibleDisplay->emitGet(url.toString());
}

void BibleForm::readBook(int id)
{
    const int i = m_bookIDs.at(id);
    BibleUrl url;
    url.setBible(BibleUrl::LoadCurrentBible);
    url.setBookID(i);
    url.setChapter(BibleUrl::LoadFirstChapter);
    url.setVerse(BibleUrl::LoadCurrentVerse);

    m_bibleDisplay->emitGet(url.toString());
}
void BibleForm::zoomIn()
{
    m_view->setZoomFactor(m_view->zoomFactor() + 0.1);
}

void BibleForm::zoomOut()
{
    m_view->setZoomFactor(m_view->zoomFactor() - 0.1);
}

void BibleForm::setChapters(const QStringList &chapters)
{
    bool same = true;
    if(m_ui->comboBox_chapters->count() == chapters.count()) {
        for(int i = 0; i < chapters.count(); i++) {
            if(m_ui->comboBox_chapters->itemText(i) != chapters.at(i)) {
                same = false;
            }
        }
    } else {
        same = false;
    }
    if(!same) {
        m_ui->comboBox_chapters->clear();
        m_ui->comboBox_chapters->insertItems(0, chapters);
    }
}

void BibleForm::clearChapters()
{
    m_ui->comboBox_chapters->clear();
}
void BibleForm::setCurrentChapter(const int &chapterID)
{
    disconnect(m_ui->comboBox_chapters, SIGNAL(activated(int)), this, SLOT(readChapter(int)));
    m_ui->comboBox_chapters->setCurrentIndex(chapterID);
    connect(m_ui->comboBox_chapters, SIGNAL(activated(int)), this, SLOT(readChapter(int)));
}

void BibleForm::setBooks(const QHash<int, QString> &books, QList<int> ids)
{
    bool same = true;
    QHashIterator<int, QString> i(books);
    int count = 0;
    if(m_ui->comboBox_books->count() == books.count()) {
        while(i.hasNext()) {
            i.next();
            if(m_ui->comboBox_books->itemText(count) != i.value()) {
                same = false;
                break;
            }
            count++;
        }
    } else {
        same = false;
    }
    if(!same) {
        m_ui->comboBox_books->clear();
        m_ui->comboBox_books->insertItems(0, books.values());
        m_bookIDs = ids;
    }

}
void BibleForm::clearBooks()
{
    m_ui->comboBox_books->clear();
}
void BibleForm::setCurrentBook(const int &bookID)
{
    //todo: is there a better way then disconnect and connect?
    disconnect(m_ui->comboBox_books, SIGNAL(activated(int)), this, SLOT(readBook(int)));
    m_ui->comboBox_books->setCurrentIndex(m_moduleManager->bible()->bookIDs().indexOf(bookID));
    connect(m_ui->comboBox_books, SIGNAL(activated(int)), this, SLOT(readBook(int)));
}
void BibleForm::activated()
{
    BibleList *list = m_bibleList;
    if(m_bibleList == NULL || m_bibleList->bible() == NULL) {
        clearChapters();
        clearBooks();
        //setTitle("");
        m_moduleManager->m_bibleList = NULL;
        return;
    }
    if(m_bibleList->bible()->moduleID() < 0) {
        clearChapters();
        clearBooks();
        //setTitle("");
        m_moduleManager->m_bibleList = list;
        return;
    }


    m_moduleManager->m_bibleList = m_bibleList;
    myDebug() << "current bible title = " << m_moduleManager->bible()->bibleTitle();
    //setTitle(m_moduleManager->bible()->bibleTitle());
    //m_moduleDockWidget->loadedModule(m_moduleManager->bible()->moduleID()); //todo:

    setChapters(m_moduleManager->bible()->chapterNames());
    setCurrentChapter(m_moduleManager->bible()->chapterID());

    setBooks(m_moduleManager->bible()->bookNames(), m_moduleManager->bible()->bookIDs());
    setCurrentBook(m_moduleManager->bible()->bookID());
}

void BibleForm::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void BibleForm::scrollToAnchor(const QString &anchor)
{
#if QT_VERSION >= 0x040700
        m_view->page()->mainFrame()->scrollToAnchor(anchor);
#else
        m_view->page()->mainFrame()->evaluateJavaScript("window.location.href = '" + anchor + "';");
#endif

}
void BibleForm::showText(const QString &text)
{
    QString cssFile = m_settings->getModuleSettings(m_moduleManager->bible()->moduleID()).styleSheet;
    if(cssFile.isEmpty())
        cssFile = ":/data/css/default.css";

    QFile file(cssFile);
    if(file.open(QFile::ReadOnly))
        m_view->settings()->setUserStyleSheetUrl(QUrl("data:text/css;charset=utf-8;base64," + file.readAll().toBase64()));

    m_view->setHtml(text);
    if(m_moduleManager->bible()->verseID() > 1) {
        scrollToAnchor("currentVerse");
        if(m_moduleManager->bibleList()->hasTopBar())
            m_view->page()->mainFrame()->scroll(0, -40); //due to the biblelist bar on top
    }

    if(m_moduleManager->bible()->bibleType() == Module::BibleQuoteModule) {
        QWebElementCollection collection = m_view->page()->mainFrame()->documentElement().findAll("img");
        QStringList searchPaths = m_moduleManager->bible()->getSearchPaths();

        foreach(QWebElement paraElement, collection) {
            QString url = paraElement.attribute("src");
            if(url.startsWith(":/") || url.startsWith("http"))
                continue;
            foreach(const QString & pre, searchPaths) {
                QFileInfo i(pre + url);
                if(i.exists()) {
                    paraElement.setAttribute("src", "file://" + pre + url);
                    break;
                } else {
                    QDir d(pre);
                    QStringList list = d.entryList();
                    foreach(const QString & f, list) {
                        QFileInfo info2(f);
                        if(info2.baseName().compare(i.baseName(), Qt::CaseInsensitive) == 0) {
                            paraElement.setAttribute("src", "file://" + pre + f);
                            break;
                        }
                    }
                }
            }
        }
    }

}
void BibleForm::evaluateJavaScript(const QString &js)
{
    m_view->page()->mainFrame()->evaluateJavaScript(js);
}

BibleForm::~BibleForm()
{
    delete m_ui;
    delete m_bibleList;
}

