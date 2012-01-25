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
#include "simpleinterface.h"
#include "ui_simpleinterface.h"
#include "src/core/dbghelper.h"
#include "src/ui/dialog/searchdialog.h"
#include "src/core/settings/moduledisplaysettings.h"
#include "src/module/moduletools.h"
#include "src/core/search/search.h"
#include "src/module/response/textrangesresponse.h"
#include <QtCore/QPointer>
#include <QtGui/QDesktopServices>
#include <QtGui/QMessageBox>
#include <QtGui/QKeyEvent>
#include <QtWebKit/QWebView>
SimpleInterface::SimpleInterface(QWidget *parent) :
    Interface(parent),
    ui(new Ui::SimpleInterface)
{
    ui->setupUi(this);
    m_view = new WebView(this);
    m_view->load(QUrl("about:blank"));
    m_view->show();
    ui->verticalLayout->addWidget(m_view);
}
void SimpleInterface::init()
{
    m_moduleManager->newDisplaySettings();
    m_moduleManager->moduleDisplaySetings()->setLoadNotes(false);
    m_moduleManager->moduleDisplaySetings()->setShowMarks(false);
    m_moduleManager->moduleDisplaySetings()->setShowNotes(false);

    m_module = new Bible();
    m_moduleManager->initVerseModule(m_module);

    connect(m_actions, SIGNAL(_get(VerseUrl)), this, SLOT(pharseUrl(VerseUrl)));
    connect(m_actions, SIGNAL(_get(QString)), this, SLOT(pharseUrl(QString)));
    connect(m_view, SIGNAL(linkClicked(QUrl)), m_actions, SLOT(get(QUrl)));


}
void SimpleInterface::createDocks()
{
    m_moduleDockWidget = new ModuleDockWidget(this->parentWidget());
    setAll(m_moduleDockWidget);
    m_moduleDockWidget->init();

    m_bookDockWidget = new BookDockWidget(this->parentWidget());
    setAll(m_bookDockWidget);
    m_bookDockWidget->init();

    m_searchResultDockWidget = new SearchResultDockWidget(this->parentWidget());
    setAll(m_searchResultDockWidget);
    m_searchResultDockWidget->hide();
}

void SimpleInterface::createToolBars()
{
    m_bar = new QToolBar(this->parentWidget());
    m_bar->setObjectName("mainToolBar");
    m_bar->setIconSize(QSize(32, 32));
    m_bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    m_actionSearch = new QAction(QIcon::fromTheme("edit-find", QIcon(":/icons/32x32/edit-find.png")), tr("Search"), m_bar);
    connect(m_actionSearch, SIGNAL(triggered()), this, SLOT(showSearchDialog()));
    m_actionSearch->setShortcut(QKeySequence::Find);

    m_actionZoomIn = new QAction(QIcon::fromTheme("zoom-in", QIcon(":/icons/32x32/zoom-in.png")), tr("Zoom In"), m_bar);
    connect(m_actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));

    m_actionZoomOut = new QAction(QIcon::fromTheme("zoom-out", QIcon(":/icons/32x32/zoom-out.png")), tr("Zoom Out"), m_bar);
    connect(m_actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));

    m_actionModule = new QAction(QIcon(":/icons/32x32/module.png"), tr("Module"), m_bar);
    connect(m_actionModule, SIGNAL(triggered()), this->parent(), SLOT(showSettingsDialog_Module()));

    m_bar->addAction(m_actionSearch);
    m_bar->addSeparator();
    m_bar->addAction(m_actionZoomIn);
    m_bar->addAction(m_actionZoomOut);
    m_bar->addSeparator();
    m_bar->addAction(m_actionModule);
}

void SimpleInterface::createMenu()
{
    //don't have one
}

QHash<DockWidget*, Qt::DockWidgetArea> SimpleInterface::docks()
{
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.insert(m_searchResultDockWidget, Qt::RightDockWidgetArea);
    ret.insert(m_bookDockWidget, Qt::LeftDockWidgetArea);
    ret.insert(m_moduleDockWidget, Qt::LeftDockWidgetArea);
    return ret;

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
QList<QToolBar *> SimpleInterface::toolBars()
{
    QList<QToolBar *> list;
    list.append(m_bar);
    return list;
}
void SimpleInterface::zoomIn()
{
    m_view->setZoomFactor(m_view->zoomFactor() + 0.1);
    //ui->textBrowser->zoomIn();
}
void SimpleInterface::zoomOut()
{
    m_view->setZoomFactor(m_view->zoomFactor() - 0.1);
    //ui->textBrowser->zoomOut();
}
void SimpleInterface::pharseUrl(QUrl url)
{
    pharseUrl(url.toString());
}
void SimpleInterface::pharseUrl(const VerseUrl &url)
{
    m_url = m_url.applyUrl(url);

    Ranges ranges;
    foreach(VerseUrlRange range, m_url.ranges()) {
        ranges.addRange(range.toRange());
    }
    ranges.setSource(m_url);
    showRanges(ranges, m_url);
}

void SimpleInterface::pharseUrl(const QString &string)
{
    const QString bible = "verse:/";
    const QString bq = "go";

    if(string.startsWith(bible)) {

        VerseUrl url;
        Ranges ranges;
        if(!url.fromString(string)) {
            return;
        }
        m_url = m_url.applyUrl(url);

        foreach(VerseUrlRange range, m_url.ranges()) {
            ranges.addRange(range.toRange());
        }
        ranges.setSource(m_url);
        showRanges(ranges, m_url);
        /*if(m_url.hasParam("searchInCurrentText")) {
            m_actions->searchInText();
        }*/
    } else if(string.startsWith(bq)) {
        //its a biblequote internal link, but i dont have the specifications!!!
        QStringList internal = string.split(" ");
        const QString bibleID = internal.at(1);//todo: use it
        myDebug() << "bibleID = " << bibleID;

        int bookID = internal.at(2).toInt() - 1;
        int chapterID = internal.at(3).toInt() - 1;
        int verseID = internal.at(4).toInt();
        VerseUrlRange range;
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(bookID);
        range.setChapter(chapterID);
        range.setStartVerse(verseID);
        VerseUrl url(range);
        m_actions->get(url);
    }
}

void SimpleInterface::showRanges(const Ranges &ranges, const VerseUrl &url)
{
    DEBUG_FUNC_NAME
    TextRangesResponse *res = (TextRangesResponse*) m_module->readRanges(ranges);

    TextRanges t = res->ranges();
    delete res;
    m_module->setLastTextRanges(&t);

    if(!t.error()) {
        showTextRanges(t.join(""), t, url);
        m_actions->updateChapters(t.minBookID(), m_module->versification());
        m_actions->updateBooks(m_module->versification());
        m_actions->setCurrentBook(t.bookIDs());
        m_actions->setCurrentChapter(t.chapterIDs());
        m_actions->setTitle(m_module->moduleTitle());
    } else {
        showTextRanges(t.join(""), t, url);
        m_actions->clearBooks();
        m_actions->clearChapters();
    }
}

void SimpleInterface::showTextRanges(const QString &html, const TextRanges &range, const VerseUrl &url)
{
    showText(html);
    m_lastTextRanges = range;
    m_lastUrl = url;
    m_module->setLastTextRanges(&m_lastTextRanges);
    //m_verseTable->setLastUrl(&m_lastUrl);
    //historySetUrl(url.toString());
}

void SimpleInterface::showText(const QString &text)
{
    QString cssFile = m_settings->getModuleSettings(m_module->moduleID())->styleSheet;
    if(cssFile.isEmpty())
        cssFile = ":/data/css/default.css";
    m_view->setHtml(text);
    m_view->settings()->setUserStyleSheetUrl(QUrl::fromLocalFile(cssFile));

    if(m_module->lastTextRanges()->minVerseID() > 1)
        m_view->scrollToAnchor("currentVerse");
}
void SimpleInterface::setTitle(const QString &title)
{
    this->parentWidget()->setWindowTitle(title + " - " + tr("openBibleViewer"));
}


void SimpleInterface::nextChapter()
{
    if(!m_moduleManager->metaModuleLoaded(m_module))
        return;
    if(m_module->lastTextRanges()->minChapterID() <
            m_module->versification()->maxChapter().value(m_module->lastTextRanges()->minBookID()) - 1) {
        VerseUrl bibleUrl;
        VerseUrlRange range;
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(VerseUrlRange::LoadCurrentBook);
        range.setChapter(m_module->lastTextRanges()->minChapterID() + 1);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    } else if(m_module->lastTextRanges()->minBookID() < m_module->versification()->bookCount() - 1) {
        VerseUrl bibleUrl;
        VerseUrlRange range;
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(m_module->lastTextRanges()->minBookID() + 1);
        range.setChapter(VerseUrlRange::LoadFirstChapter);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    }
}
void SimpleInterface::previousChapter()
{
    //see also BibleManager
    if(!m_moduleManager->metaModuleLoaded(m_module))
        return;
    if(m_module->lastTextRanges()->minChapterID() > 0) {
        VerseUrl bibleUrl;
        VerseUrlRange range;
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(VerseUrlRange::LoadCurrentBook);
        range.setChapter(m_module->lastTextRanges()->minChapterID() - 1);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    } else if(m_module->lastTextRanges()->minBookID() > 0) {
        VerseUrl bibleUrl;
        VerseUrlRange range;
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(m_module->lastTextRanges()->minBookID() - 1);
        range.setChapter(VerseUrlRange::LoadLastChapter);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    }
}
bool SimpleInterface::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_view) {
        if(event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_PageUp) {
                previousChapter();
                return true;
            } else if(keyEvent->key() == Qt::Key_Right || keyEvent->key() == Qt::Key_PageDown) {
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
    if(m_module != NULL) {
        delete m_module;
        m_module = NULL;
    }
}
void SimpleInterface::settingsChanged(Settings oldSettings, Settings newSettings, bool modifedModuleSettings)
{
    //DEBUG_FUNC_NAME
    //reload books
    /*bool reloadBibles = false;
    if(oldSettings.encoding != newSettings.encoding) {
        reloadBibles = true;
    }*/
    /*for(int i = 0; i < newSettings.m_moduleSettings.size(); ++i) {
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
    }*/
    /*   if(modifedModuleSettings) {
           reloadBibles = true;
       }
       if(reloadBibles == true) {
           FastStart fastStart;
           fastStart.setSettings(m_settings);
           fastStart.remove();
           //myDebug() << "reload Bibles";
           m_moduleManager->loadAllModules();
           m_moduleDockWidget->init();
           ui->textBrowser->setHtml("");
           //todo: clear everything
       }*/

}
void SimpleInterface::showSearchDialog()
{
    QPointer<SearchDialog> sDialog = new SearchDialog(this);
    connect(sDialog, SIGNAL(searched(SearchQuery)), this, SLOT(search(SearchQuery)));
    /*  if(ui->textBrowser->textCursor().hasSelection() == true) //something is selected
          sDialog->setText(ui->textBrowser->textCursor().selectedText());*/
    sDialog->exec();
    delete sDialog;
}
void SimpleInterface::search(SearchQuery query)
{
    m_searchResultDockWidget->show();
    Search s;
    setAll(&s);
    SearchResult *res = s.search(query);
    m_searchResultDockWidget->setSearchResult(res);
}
void SimpleInterface::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        m_actionSearch->setText(tr("Search"));
        m_actionZoomIn->setText(tr("Zoom In"));
        m_actionZoomOut->setText(tr("Zoom Out"));
        m_actionModule->setText(tr("Module"));
        break;
    default:
        break;
    }
}
QString SimpleInterface::name() const
{
    return "simple";
}
