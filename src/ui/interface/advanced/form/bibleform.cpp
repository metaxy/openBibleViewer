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
#include "bibleform.h"
#include "ui_bibleform.h"
#include <QPointer>

#include "src/core/verse/reftext.h"
#include "src/core/link/urlconverter2.h"
#include "src/core/module/response/textrangesresponse.h"
#include "src/api/api.h"
#include "src/api/moduleapi.h"
BibleForm::BibleForm(QWidget *parent) :
    WebViewForm(parent),
    m_verseTable(nullptr),
    m_ui(new Ui::BibleForm)
{
    m_ui->setupUi(this);
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
BibleForm::~BibleForm()
{
    delete m_ui;
    m_ui = nullptr;

    delete m_verseTable;
    m_verseTable = nullptr;
}
Form::FormType BibleForm::type() const
{
    return Form::BibleForm;
}

void BibleForm::init()
{
    //DEBUG_FUNC_NAME
    m_verseTable = m_moduleManager->newVerseTable();
    attachApi();

    connect(m_view->page(), SIGNAL(linkClicked(QUrl)), m_actions, SLOT(get(QUrl)));
    connect(m_view, SIGNAL(linkMiddleOrCtrlClicked(QUrl)), m_actions, SLOT(newGet(QUrl)));

    connect(m_view->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attachApi()));

    connect(m_actions, SIGNAL(_updateChapters(int, QSharedPointer<Versification> )), this, SLOT(forwardSetChapters(int, QSharedPointer<Versification> )));
    connect(m_actions, SIGNAL(_updateBooks(QSharedPointer<Versification> )), this, SLOT(forwardSetBooks(QSharedPointer<Versification> )));

    connect(m_actions, SIGNAL(_clearBooks()), this, SLOT(forwardClearBooks()));
    connect(m_actions, SIGNAL(_clearChapters()), this, SLOT(forwardClearChapters()));

    connect(m_actions, SIGNAL(_setCurrentBook(QSet<int>)), this, SLOT(forwardSetCurrentBook(QSet<int>)));
    connect(m_actions, SIGNAL(_setCurrentChapter(QSet<int>)), this, SLOT(forwardSetCurrentChapter(QSet<int>)));
    connect(m_actions, SIGNAL(_searchInText(SearchResult*)), this, SLOT(forwardSearchInText(SearchResult*)));
    connect(m_actions, SIGNAL(_reloadIf(VerseUrl)), this, SLOT(reloadIf(VerseUrl)));
    connect(m_actions, SIGNAL(_moduleChanged(int)), this, SLOT(moduleChanged(int)));

      connect(m_actions, SIGNAL(_removeModuleFromVerseTable(int)), this, SLOT(forwardDeleteModule(int)));

    connect(m_view, SIGNAL(contextMenuRequested(QContextMenuEvent*)), this, SLOT(showContextMenu(QContextMenuEvent*)));
    createDefaultMenu();
}
void BibleForm::newModule(const int moduleID)
{
    myDebug() << moduleID;
    m_moduleManager->newTextRangesVerseModule(moduleID, QPoint(0, 0), m_verseTable);
}
int BibleForm::newModule()
{
    int defaultModuleID = -1;
    QMapIterator<int, ModuleSettings*> i(m_settings->m_moduleSettings);
    while(i.hasNext()) {
        i.next();
        if(i.value()->defaultModule == ModuleTools::DefaultBibleModule) {
            defaultModuleID = i.key();
            break;
        }

    }
    //myDebug() << "default" << defaultModuleID;
    if(defaultModuleID == -1) {
        auto i2 = m_moduleManager->m_moduleMap->it();
        while(i2.hasNext() && defaultModuleID == -1) {
            i2.next();
            if(i2.value()->moduleClass() == ModuleTools::BibleModuleClass) {
                defaultModuleID = i2.key();
                break;
            }
        }
    }
    //!!! load this module
    newModule(defaultModuleID);

    return defaultModuleID;
}
void BibleForm::addParallelH(const int moduleID)
{
    DEBUG_FUNC_NAME
    m_moduleManager->newTextRangesVerseModule(moduleID, m_verseTable->bottomLeft(), m_verseTable);
}

void BibleForm::addParallelV(const int moduleID)
{
    DEBUG_FUNC_NAME
    m_moduleManager->newTextRangesVerseModule(moduleID, m_verseTable->topRight(), m_verseTable);
}

void BibleForm::parseUrl(const QString &string)
{
    myDebug() << string;
    const QString bq = "go";

    if(string.startsWith(ModuleTools::verseScheme)) {
        VerseUrl url;
        if(!url.fromStringUrl(string)) {
            return;
        }
        parseUrl(url);
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
        parseUrl(url);
    }
}

void BibleForm::parseUrl(const VerseUrl &url, bool reload)
{
    DEBUG_FUNC_NAME
    bool showStart = false;

    if(m_url.isValid()) {
        m_url = m_url.applyUrl(url);
        showStart = true;
    } else {
        m_url = url;
        if(verseTableLoaded() && !m_url.hasModuleID()) {
            m_url.setModuleID(verseModule()->moduleID());
        }
    }
    m_url.removeNo();

    //fix reload
    if(reload && verseTableLoaded()) {
        m_url.setModuleID(verseModule()->moduleID());
    }

    Ranges ranges;
    foreach(VerseUrlRange range, m_url.ranges()) {
        ranges.addRange(range.toRange());
    }
    ranges.setSource(m_url);

    showRanges(ranges, m_url, showStart, url.hasParam("ignoreModuleID"));

    if(m_url.hasParam("searchInCurrentText")) {
        m_actions->searchInText();
    }
}

void BibleForm::showRanges(const Ranges &ranges, const VerseUrl &url, bool showStart, bool ignoreModuleID)
{
    DEBUG_FUNC_NAME;
    myDebug() << "showStart = " << showStart;
    //m_verseTable->currentVerseTableID();
    const int moduleID = ranges.getList().first().moduleID();

    if(!verseTableLoaded()) {
        m_moduleManager->newTextRangesVerseModule(moduleID, QPoint(0, 0), m_verseTable);
    }

    const int newID = ranges.getList().first().moduleID();

    if(verseModule()->moduleID() != newID && !ignoreModuleID) {
        const QPoint p = m_verseTable->m_points.value(m_verseTable->activeItem());
        m_moduleManager->newTextRangesVerseModule(moduleID, p, m_verseTable);
    }

    std::pair<QString, TextRanges> r = m_verseTable->readRanges(ranges);

    if(!r.second.error()) {
        showTextRanges(r.first, r.second, url);
        m_actions->updateChapters(r.second.minBookID(), verseModule()->versification());
        m_actions->updateBooks(verseModule()->versification());
        m_actions->setCurrentBook(r.second.bookIDs());
        m_actions->setCurrentChapter(r.second.chapterIDs());
        m_actions->setTitle(verseModule()->moduleTitle());
    } else if(r.second.error() == TextRange::FatalError){ //no such module
        //todo: should we show a error message?
        showTextRanges(r.first, r.second, url);
        m_actions->clearBooks();
        m_actions->clearChapters();
        m_actions->setTitle(verseModule()->moduleTitle());
    } else if(r.second.error() == TextRange::NotFoundError) { // this module does not contain this book
        if(showStart) {
            VerseUrlRange range;
            range.setModule(verseModule()->moduleID());
            range.setBook(VerseUrlRange::LoadFirstBook);
            range.setChapter(VerseUrlRange::LoadFirstChapter);
            range.setWholeChapter();
            VerseUrl url(range);
            parseUrl(url);
        } else {
            showTextRanges(r.first, r.second, url);
            m_actions->updateChapters(0, verseModule()->versification());
            m_actions->updateBooks(verseModule()->versification());
            m_actions->setCurrentBook(r.second.bookIDs());
            m_actions->setCurrentChapter(r.second.chapterIDs());
            m_actions->setTitle(verseModule()->moduleTitle());
        }
    }
}
void BibleForm::reload(bool full)
{
    if(!verseTableLoaded()) return;
    const QPoint p = m_view->page()->mainFrame()->scrollPosition();

    if(full) {
        m_verseTable->clearData();
    }
    parseUrl(m_url, true);

    m_view->page()->mainFrame()->setScrollPosition(p);
}
void BibleForm::reloadIf(const VerseUrl &url)
{
    if(!m_verseTable || !verseModule() || !verseModule()->lastTextRanges()) {
        return;
    }

    VerseUrlRange r = url.ranges().first();
    if(verseModule()->moduleID() == r.moduleID() && verseModule()->lastTextRanges()->contains(r.bookID(), r.chapterID())) {
        reload(false);
    }
}
void BibleForm::nextChapter()
{
    if(!verseTableLoaded())
        return;
    if(verseModule()->lastTextRanges()->minChapterID() <
            verseModule()->versification()->maxChapter().value(verseModule()->lastTextRanges()->minBookID()) - 1) {
        VerseUrl bibleUrl;
        VerseUrlRange range;
        range.setOpenToTransformation(false);
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(VerseUrlRange::LoadCurrentBook);
        range.setChapter(verseModule()->lastTextRanges()->minChapterID() + 1);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    } else if(verseModule()->lastTextRanges()->minBookID() < verseModule()->versification()->bookCount() - 1) {
        VerseUrl bibleUrl;
        VerseUrlRange range;
        range.setOpenToTransformation(false);
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(verseModule()->lastTextRanges()->minBookID() + 1);
        range.setChapter(VerseUrlRange::LoadFirstChapter);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    }
}

void BibleForm::previousChapter()
{
    if(!verseTableLoaded())
        return;
    if(verseModule()->lastTextRanges()->minChapterID() > 0) {
        VerseUrl bibleUrl;
        VerseUrlRange range;
        range.setOpenToTransformation(false);
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(VerseUrlRange::LoadCurrentBook);
        range.setChapter(verseModule()->lastTextRanges()->minChapterID() - 1);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    } else if(verseModule()->lastTextRanges()->minBookID() > 0) {
        VerseUrl bibleUrl;
        VerseUrlRange range;
        range.setOpenToTransformation(false);
        range.setModule(VerseUrlRange::LoadCurrentModule);
        range.setBook(verseModule()->lastTextRanges()->minBookID() - 1);
        range.setChapter(VerseUrlRange::LoadLastChapter);
        range.setWholeChapter();
        bibleUrl.addRange(range);
        m_actions->get(bibleUrl);
    }
}

void BibleForm::restore(const QString &key)
{
    const QString a = m_settings->session.id() + "/windows/" + key + "/";
    const qreal zoom = m_settings->session.file()->value(a + "zoom").toReal();
    const QPoint scroll = m_settings->session.file()->value(a + "scrollPosition").toPoint();

    //load verse module

    m_verseTable->clear();
    const QStringList urls = m_settings->session.file()->value(a + "urls").toStringList();
    const QStringList points = m_settings->session.file()->value(a + "biblePoints").toStringList();

    for(int j = 0; j < urls.size() && j < points.size(); j++) {
        const QString url = urls.at(j);
        QStringList tmp = points.at(j).split("|");
        QPoint point;
        point.setX(tmp.first().toInt());
        point.setY(tmp.last().toInt());

        UrlConverter2 urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
        urlConverter.setSM(m_settings, m_moduleManager->m_moduleMap.data());
        urlConverter.convert();

        if(urlConverter.moduleID() != -1) {
            m_moduleManager->newTextRangesVerseModule(urlConverter.moduleID(), point, m_verseTable);
            parseUrl(urlConverter.url());//these urls are handeld by this Form
        }
    }
    const QVariant v = m_settings->session.file()->value(a + "hist1");
    const QVariant v2 = m_settings->session.file()->value(a + "hist2");
    if(!v.toStringList().isEmpty() || !v2.toStringList().isEmpty()) {
        m_browserHistory.setData1(v);
        m_browserHistory.setData2(v2);
        m_browserHistory.setData3(m_settings->session.file()->value(a + "hist3"));
        setButtons();
    }

    m_view->page()->mainFrame()->setScrollPosition(scroll);
    m_view->setZoomFactor(zoom);
}

void BibleForm::save()
{
    QStringList urls;
    QStringList points;
    if(m_verseTable != nullptr) {
        QHashIterator<int, TextRangesVerseModule *> i(m_verseTable->m_modules);
        while(i.hasNext()) {
            i.next();
            TextRangesVerseModule *b = i.value();
            if(b != nullptr && b->moduleID() >= 0) {
                VerseUrl bibleUrl = b->lastTextRanges()->source().source();
                bibleUrl.setModuleID(b->moduleID());

                UrlConverter urlConverter(UrlConverter::InterfaceUrl, UrlConverter::PersistentUrl, bibleUrl);
                urlConverter.setSettings(m_settings);
                urlConverter.setModuleMap(m_moduleManager->m_moduleMap.data());
                VerseUrl newUrl = urlConverter.convert();

                const QString url = newUrl.toString();
                const QPoint point = m_verseTable->m_points.value(i.key());

                points << QString::number(point.x()) + "|" + QString::number(point.y());
                urls << url;
            }
        }
    }
    const QString a = m_settings->session.id() + "/windows/" + QString::number(m_id) + "/";
    m_settings->session.file()->setValue(a + "urls", urls);
    m_settings->session.file()->setValue(a + "biblePoints", QVariant(points));
    m_settings->session.file()->setValue(a + "scrollPosition", m_view->page()->mainFrame()->scrollPosition());
    m_settings->session.file()->setValue(a + "zoom", m_view->zoomFactor());

    m_settings->session.file()->setValue(a + "hist1", m_browserHistory.data1());
    m_settings->session.file()->setValue(a + "hist2", m_browserHistory.data2());
    m_settings->session.file()->setValue(a + "hist3", m_browserHistory.data3());

    m_settings->session.file()->setValue(a + "type", "bible");
}

void BibleForm::attachApi()
{
    addJS(":/data/js/tools.js");
    addJS(":/data/js/jquery-1.4.2.min.js");
    addJS(":/data/js/underscore-min.js");
    addApi(m_api->moduleApi());
}

void BibleForm::historySetUrl(QString url)
{
    m_browserHistory.setCurrent(url);
    setButtons();
}
void BibleForm::backward()
{
    m_actions->get(m_browserHistory.backward());
    setButtons();
}
void BibleForm::forward()
{
    m_actions->get(m_browserHistory.forward());
    setButtons();
}
void BibleForm::setButtons()
{
    if(m_browserHistory.backwardAvailable()) {
        m_ui->toolButton_backward->setDisabled(false);
    } else {
        m_ui->toolButton_backward->setDisabled(true);
    }
    if(m_browserHistory.forwardAvailable()) {
        m_ui->toolButton_forward->setDisabled(false);
    } else {
        m_ui->toolButton_forward->setDisabled(true);
    }
}

void BibleForm::showBibleListMenu()
{
    QPointer<VerseTableWidget> w = new VerseTableWidget(this);
    connect(w, SIGNAL(clear()), this, SLOT(clear()));
    setAll(w);
    w->setVerseTable(m_verseTable);
    w->init();
    w->exec();
    delete w;
}

void BibleForm::readChapter(int id)
{
    VerseUrlRange r;
    r.setOpenToTransformation(false);
    r.setModule(VerseUrlRange::LoadCurrentModule);
    r.setBook(VerseUrlRange::LoadCurrentBook);
    r.setChapter(id);
    r.setWholeChapter();
    VerseUrl url(r);
    m_actions->get(url);
}

void BibleForm::readBook(int id)
{
    const int i = m_bookIDs.at(id);

    VerseUrlRange r;
    r.setOpenToTransformation(false);
    r.setModule(VerseUrlRange::LoadCurrentModule);
    r.setBook(i);
    r.setChapter(VerseUrlRange::LoadFirstChapter);
    r.setWholeChapter();

    VerseUrl url(r);
    m_actions->get(url);
}

void BibleForm::setChapters(int bookID, QSharedPointer<Versification> v11n)
{
    if(v11n == nullptr)
        return;
    const int count = v11n->maxChapter().value(bookID, 0);
    QStringList chapters;
    int add = 0;
    if(v11n->extendedData.hasChapterZero()) {
        add = -1;
    }
    for(int i = 1; i <= count; ++i) {
        chapters << QString::number(i + add);
    }
    //to avoid flickering
    bool same = true;
    if(m_ui->comboBox_chapters->count() == chapters.count()) {
        for(int i = 0; i < chapters.count(); ++i) {
            if(m_ui->comboBox_chapters->itemText(i) != chapters.at(i)) {
                same = false;
                break;
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
void BibleForm::setCurrentChapter(const QSet<int> &chapterID)
{
    disconnect(m_ui->comboBox_chapters, SIGNAL(activated(int)), this, SLOT(readChapter(int)));
    int min = -1;
    foreach(int c, chapterID) {
        if(c < min || min == -1)
            min = c;
    }
    m_ui->comboBox_chapters->setCurrentIndex(min);
    connect(m_ui->comboBox_chapters, SIGNAL(activated(int)), this, SLOT(readChapter(int)));
}

void BibleForm::setBooks(QSharedPointer<Versification> v11n)
{
    //DEBUG_FUNC_NAME
    bool same = true;
    QHash<int, QString> books = v11n->bookNames();
    QHashIterator<int, QString> i(books);
    int count = 0;
    if(m_ui->comboBox_books->count() == books.count()) {
        while(i.hasNext()) {
            i.next();
            if(m_ui->comboBox_books->itemText(count) != i.value()) {
                same = false;
                break;
            }
            ++count;
        }
    } else {
        same = false;
    }
    if(!same) {
        m_ui->comboBox_books->clear();
        m_ui->comboBox_books->insertItems(0, books.values());
        m_bookIDs = books.keys();
    }

}
void BibleForm::clearBooks()
{
    m_ui->comboBox_books->clear();
}
void BibleForm::setCurrentBook(const QSet<int> &bookID)
{
    //todo: is there a better way then disconnect and connect?
    disconnect(m_ui->comboBox_books, SIGNAL(activated(int)), this, SLOT(readBook(int)));
    int min = -1;
    foreach(int b, bookID) {
        if(b < min || min == -1)
            min = b;
    }
    m_ui->comboBox_books->setCurrentIndex(verseModule()->versification()->bookIDs().indexOf(min));
    connect(m_ui->comboBox_books, SIGNAL(activated(int)), this, SLOT(readBook(int)));
}
void BibleForm::forwardClear()
{
    if(!active())
        return;
    clear();
}
void BibleForm::clear()
{
    clearChapters();
    clearBooks();
    m_view->setHtml("");
    m_actions->setTitle("");
    m_verseTable->clear();
}

void BibleForm::activated()
{
    //DEBUG_FUNC_NAME
    //myDebug() << " windowID = " << m_id;
    m_api->moduleApi()->setFrame(m_view->page()->mainFrame());
    if(m_verseTable == nullptr) {
        clearChapters();
        clearBooks();
        m_actions->setTitle("");
        m_verseTable = m_moduleManager->newVerseTable();
        return;
    } else if(verseModule() == nullptr) {
        m_moduleManager->newTextRangesVerseModule(-1, QPoint(0, 0), m_verseTable);
        return;
    }

    if(verseModule()->moduleID() < 0) {
        myDebug() << "invalid moduleID";
        clearChapters();
        clearBooks();
        m_actions->setTitle("");
        return;
    }


    m_actions->setTitle(verseModule()->moduleTitle());
    m_actions->setCurrentModule(verseModule()->moduleID());

    if(verseModule()->lastTextRanges() != nullptr) {
        m_actions->updateChapters(verseModule()->lastTextRanges()->minBookID(), verseModule()->versification());
        m_actions->updateBooks(verseModule()->versification());

        if(m_lastTextRanges.verseCount() != 0 && !m_lastTextRanges.error()) {
            m_actions->setCurrentChapter(m_lastTextRanges.chapterIDs());
            m_actions->setCurrentBook(m_lastTextRanges.bookIDs());
        }

        m_verseTable->setLastTextRanges(&m_lastTextRanges);
        //m_verseTable->setLastUrl(m_lastUrl);
    }
}
QString BibleForm::getStyleSheetUrl()
{
    return m_settings->getModuleSettings(verseModule()->moduleID())->styleSheet;
}

void BibleForm::showText(const QString &text)
{
    //DEBUG_FUNC_NAME
    Q_ASSERT(m_moduleManager->verseTableLoaded(m_verseTable));
    QWebFrame * frame = m_view->page()->mainFrame();
    loadStyleSheet();
    //todo: often it isn't real html but some fragments and sometimes it's a whole html page
    //eg biblequote

    //this make it probably a bit better than append it every attachapi()
    //but does not work with biblequote see above

    /*QString showText;
    if(!text.contains("<html>")) {
        showText = "<html><head><script type='text/javascript' src='qrc:/data/js/tools.js'></script></head><body>"+text + "</body></html>";
    }*/

    m_view->setHtml(text);

    if(m_lastTextRanges.verseCount() > 1) {
        m_view->scrollToAnchor("currentEntry");
        if(m_verseTable->hasTopBar())
            frame->scroll(0, -40 * m_view->zoomFactor());
    }

    //some BibleQuote Hacks
    if(verseModule()->moduleType() == ModuleTools::BibleQuoteModule) {
        QWebElementCollection collection = frame->documentElement().findAll("img");
        const QStringList searchPaths = ((Bible*) verseModule())->getSearchPaths();

        foreach(QWebElement paraElement, collection) {
            QString url = paraElement.attribute("src");
            if(url.startsWith(":/") || url.startsWith("http") || url.startsWith("qrc:/"))
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


void BibleForm::showTextRanges(const QString &html, const TextRanges &range, const VerseUrl &url)
{
    if(!html.contains("<html")) {
       QString f = html;
       f.prepend("<div class='verseModule'>");
       f.append("</div>");
       showText(f);

    } else {
         showText(html);
    }

    m_lastTextRanges = range;
    m_lastUrl = url;
    m_verseTable->setLastTextRanges(&m_lastTextRanges);
    historySetUrl(url.toString());
}
void BibleForm::evaluateJavaScript(const QString &js)
{
    m_view->page()->mainFrame()->evaluateJavaScript(js);
}
void BibleForm::forwardSetChapters(int bookID, QSharedPointer<Versification> v11n)
{
    if(!active())
        return;
    setChapters(bookID, v11n);
}

void BibleForm::forwardSetBooks(QSharedPointer<Versification> v11n)
{
    if(!active())
        return;
    setBooks(v11n);
}

void BibleForm::forwardClearBooks()
{
    if(!active())
        return;
    clearBooks();
}

void BibleForm::forwardClearChapters()
{
    if(!active())
        return;
    clearChapters();
}

void BibleForm::forwardSetCurrentBook(const QSet<int> &bookID)
{
    if(!active())
        return;
    setCurrentBook(bookID);
}

void BibleForm::forwardSetCurrentChapter(const QSet<int> &chapterID)
{
    if(!active())
        return;
    setCurrentChapter(chapterID);
}
void BibleForm::forwardSearchInText(SearchResult *res)
{
    if(!active())
        return;
    searchInText(res);
}

void BibleForm::searchInText(SearchResult *res)
{
    DEBUG_FUNC_NAME
    if(res == nullptr)
        return;
    if(res->searchQuery.queryType == SearchQuery::Simple) {
        QString s = res->searchQuery.searchText;
        //todo: hacky
        s.remove('*');
        s.remove('?');
        m_view->findText(s, QWebPage::HighlightAllOccurrences);
    }
}

void BibleForm::createDefaultMenu()
{
    m_actionCopy = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Copy"), this);
    connect(m_actionCopy, SIGNAL(triggered()), this, SLOT(copy()));

    m_actionSelect = new QAction(QIcon::fromTheme("edit-select-all", QIcon(":/icons/16x16/edit-select-all.png")), tr("Select All"), this);
    connect(m_actionSelect, SIGNAL(triggered()), this , SLOT(selectAll()));

    m_menuMark = new QMenu(this);
    m_menuMark->setTitle(tr("Mark this"));
    m_menuMark->setIcon(QIcon::fromTheme("format-fill-color.png", QIcon(":/icons/16x16/format-fill-color.png")));

    QAction *actionYellowMark = new QAction(QIcon(":/icons/16x16/mark-yellow.png"), tr("Yellow"), m_menuMark);
    actionYellowMark->setObjectName("yellowMark");
    connect(actionYellowMark, SIGNAL(triggered()), this , SLOT(newColorMark()));
    m_menuMark->addAction(actionYellowMark);

    QAction *actionGreenMark = new QAction(QIcon(":/icons/16x16/mark-green.png"), tr("Green"), m_menuMark);
    actionGreenMark->setObjectName("greenMark");
    connect(actionGreenMark, SIGNAL(triggered()), this , SLOT(newColorMark()));
    m_menuMark->addAction(actionGreenMark);

    QAction *actionBlueMark = new QAction(QIcon(":/icons/16x16/mark-blue.png"), tr("Blue"), m_menuMark);
    actionBlueMark->setObjectName("blueMark");
    connect(actionBlueMark, SIGNAL(triggered()), this , SLOT(newColorMark()));
    m_menuMark->addAction(actionBlueMark);

    QAction *actionOrangeMark = new QAction(QIcon(":/icons/16x16/mark-orange.png"), tr("Orange"), m_menuMark);
    actionOrangeMark->setObjectName("orangeMark");
    connect(actionOrangeMark, SIGNAL(triggered()), this , SLOT(newColorMark()));
    m_menuMark->addAction(actionOrangeMark);

    QAction *actionVioletMark = new QAction(QIcon(":/icons/16x16/mark-violet.png"), tr("Violet"), m_menuMark);
    actionVioletMark->setObjectName("violetMark");
    connect(actionVioletMark, SIGNAL(triggered()), this , SLOT(newColorMark()));
    m_menuMark->addAction(actionVioletMark);

    QAction *actionCustomMark  = new QAction(QIcon(":/icons/16x16/format-fill-color.png"), tr("Custom Color"), m_menuMark);
    connect(actionCustomMark, SIGNAL(triggered()), this , SLOT(newCustomColorMark()));
    m_menuMark->addAction(actionCustomMark);
    m_menuMark->addSeparator();

    QAction *actionBoldMark  = new QAction(QIcon::fromTheme("format-text-bold", QIcon(":/icons/16x16/format-text-bold.png")), tr("Bold"), m_menuMark);
    connect(actionBoldMark, SIGNAL(triggered()), this , SLOT(newBoldMark()));
    m_menuMark->addAction(actionBoldMark);

    QAction *actionItalicMark  = new QAction(QIcon::fromTheme("format-text-italic", QIcon(":/icons/16x16/format-text-italic.png")), tr("Italic"), m_menuMark);
    connect(actionItalicMark, SIGNAL(triggered()), this , SLOT(newItalicMark()));
    m_menuMark->addAction(actionItalicMark);

    QAction *actionUnderlineMark  = new QAction(QIcon::fromTheme("format-text-underline", QIcon(":/icons/16x16/format-text-underline.png")), tr("Underline"), m_menuMark);
    connect(actionUnderlineMark, SIGNAL(triggered()), this , SLOT(newUnderlineMark()));
    m_menuMark->addAction(actionUnderlineMark);


    m_actionRemoveMark = new QAction(QIcon(":/icons/16x16/mark-yellow.png"), tr("Remove Mark"), this);
    connect(m_actionRemoveMark, SIGNAL(triggered()), this , SLOT(removeMark()));

    m_actionBookmark = new QAction(QIcon::fromTheme("bookmark-new", QIcon(":/icons/16x16/bookmark-new.png")), tr("Add Bookmark"), this);
    connect(m_actionBookmark, SIGNAL(triggered()), this, SLOT(newBookmark()));

    m_actionNote = new QAction(QIcon::fromTheme("view-pim-notes", QIcon(":/icons/16x16/view-pim-notes.png")), tr("Add Note"), this);
    connect(m_actionNote, SIGNAL(triggered()), this , SLOT(newNoteWithLink()));
}
void BibleForm::deleteDefaultMenu()
{
    delete m_actionCopy;
    m_actionCopy = 0;
    delete m_actionSelect;
    m_actionSelect = 0;
    delete m_menuMark;
    m_menuMark = 0;
    delete m_actionRemoveMark;
    m_actionRemoveMark = 0;
    delete m_actionBookmark;
    m_actionBookmark = 0;
    delete m_actionNote;
    m_actionNote = 0;
}
void BibleForm::showContextMenu(QContextMenuEvent* ev)
{
    //DEBUG_FUNC_NAME
    QScopedPointer<QMenu> contextMenu(new QMenu(this));
    QAction *actionCopy = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Copy"), this);
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copy()));

    if(m_view->hasSelection()) {
        actionCopy->setEnabled(true);
    } else {
        actionCopy->setEnabled(false);
    }


    const QWebHitTestResult hitTest = m_view->page()->mainFrame()->hitTestContent(ev->pos());
    const QUrl url = hitTest.linkUrl();

    ModuleTools::ContentType type = ModuleTools::contentTypeFromUrl(url.toString());
    ModuleTools::ModuleClass cl = ModuleTools::moduleClassFromUrl(url.toString());
    QList<ModuleSettings*> list = m_settings->m_moduleSettings.values();

    qSort(list.begin(), list.end(), ModuleManager::sortModuleByPop);

    if(url.isEmpty()) {
        QAction *actionCopyWholeVerse = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Copy Verse"), contextMenu.data());
        VerseSelection selection = verseSelection();
        m_lastSelection = selection;
        if(selection.startVerse != -1) {
            QString addText;
            if(selection.startVerse != selection.endVerse)
                addText = " " + QString::number(selection.startVerse + 1) + " - " + QString::number(selection.endVerse + 1);
            else
                addText = " " + QString::number(selection.startVerse + 1);
            if(selection.startVerse < 0 || selection.endVerse < 0) {
                actionCopyWholeVerse->setText(tr("Copy Verse"));
                actionCopyWholeVerse->setEnabled(false);
            } else {
                actionCopyWholeVerse->setText(tr("Copy Verse %1", "e.g Copy Verse 1-2 or Copy Verse 2").arg(addText));
                actionCopyWholeVerse->setEnabled(true);

                connect(actionCopyWholeVerse, SIGNAL(triggered()), this , SLOT(copyWholeVerse()));
            }
        } else {
            actionCopyWholeVerse->setEnabled(false);
        }

        QAction *dbg = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Debugger"), contextMenu.data());
        connect(dbg, SIGNAL(triggered()), this, SLOT(debugger()));

        QMenu *openInCommentary = new QMenu(this);
        openInCommentary->setTitle(tr("Open in Commentary"));
        int counter = 0;
        foreach(ModuleSettings* m, list) {
            if(counter > 7)
                break;
            if(ModuleTools::typeToClass(m->moduleType) == ModuleTools::CommentaryClass) {
                counter++;
                QAction *n = new QAction(m->name(true), openInCommentary);
                n->setData(m->moduleID);
                connect(n, SIGNAL(triggered()), this, SLOT(openCommentary()));
                openInCommentary->addAction(n);
            }
        }

        contextMenu->addAction(actionCopy);
        contextMenu->addAction(actionCopyWholeVerse);
        contextMenu->addAction(m_actionSelect);
        contextMenu->addMenu(openInCommentary);
        contextMenu->addSeparator();
        contextMenu->addMenu(m_menuMark);
        contextMenu->addAction(m_actionRemoveMark);
        contextMenu->addSeparator();
        contextMenu->addAction(m_actionBookmark);
        contextMenu->addAction(m_actionNote);
        contextMenu->addAction(dbg);
        contextMenu->exec(ev->globalPos());
    } else {
        myDebug() << "another menu";
        myDebug() << url << url.toString();
        m_contextMenuUrl = url.toString();
        m_contextMenuText = hitTest.linkText();

        QAction *openInNewTab = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("Open in new tab"), contextMenu.data());
        connect(openInNewTab, SIGNAL(triggered()), this, SLOT(openInNewTab()));

        QAction *openHere = new QAction(QIcon::fromTheme("tab-new-background", QIcon(":/icons/16x16/tab-new-background.png")), tr("Open here"), contextMenu.data());
        connect(openHere, SIGNAL(triggered()), this, SLOT(openHere()));

        QAction *copyText = new QAction(tr("Copy Text"), contextMenu.data());
        connect(copyText, SIGNAL(triggered()), this, SLOT(copyLinkText()));

        QAction *copyLink = new QAction(tr("Copy Link"), contextMenu.data());
        connect(copyLink, SIGNAL(triggered()), this, SLOT(copyLinkUrl()));

        QMenu *openIn = new QMenu(this);
        openIn->setTitle(tr("Open in"));

        QMenu *openInNew = new QMenu(this);
        openInNew->setTitle(tr("Open in new"));


        QList<int> usedModules;
        bool showOpenIn = false;

        bool addSep = true;
        int counter = 0;

        if(type != ModuleTools::UnkownContent) {
            //most 4 used with the right content type

            foreach(ModuleSettings* m, list) {
                if(counter > 5)
                    break;
                if(ModuleTools::alsoOk(m->contentType, type)
                        && m->contentType != ModuleTools::UnkownContent
                        && !usedModules.contains(m->moduleID)) {

                    showOpenIn = true;
                    usedModules.append(m->moduleID);
                    counter++;

                    QAction *n = new QAction(m->name(true), openIn);
                    n->setData(m->moduleID);
                    connect(n, SIGNAL(triggered()), this, SLOT(openIn()));
                    openIn->addAction(n);

                    QAction *n2 = new QAction(m->name(true), openInNew);
                    n2->setData(m->moduleID);
                    connect(n2, SIGNAL(triggered()), this, SLOT(openInNew()));
                    openInNew->addAction(n2);
                }
            }
        } else {
            addSep = false;
        }

        counter = 0;
        foreach(ModuleSettings* m, list) {
            if(counter > 3)
                break;
            if(ModuleTools::typeToClass(m->moduleType) == cl
                    && !usedModules.contains(m->moduleID)) {

                showOpenIn = true;
                usedModules.append(m->moduleID);
                counter++;
                if(addSep) {
                    openIn->addSeparator();
                    openInNew->addSeparator();
                    addSep = false;
                }

                QAction *n = new QAction(m->name(true), openIn);
                n->setData(m->moduleID);
                connect(n, SIGNAL(triggered()), this, SLOT(openIn()));
                openIn->addAction(n);

                QAction *n2 = new QAction(m->name(true), openInNew);
                n2->setData(m->moduleID);
                connect(n2, SIGNAL(triggered()), this, SLOT(openInNew()));
                openInNew->addAction(n2);
            }
        }
        contextMenu->addAction(actionCopy);
        contextMenu->addAction(m_actionSelect);
        contextMenu->addAction(copyText);
        contextMenu->addAction(copyLink);

        contextMenu->addSeparator();
        contextMenu->addAction(openHere);
        contextMenu->addAction(openInNewTab);
        if(showOpenIn) {
            QAction *n = new QAction(tr("Select"), openIn);
            connect(n, SIGNAL(triggered()), this, SLOT(openInS()));
            openIn->addSeparator();
            openIn->addAction(n);
            QAction *n2 = new QAction(tr("Select"), openInNew);
            connect(n2, SIGNAL(triggered()), this, SLOT(openInNewS()));
            openInNew->addSeparator();
            openInNew->addAction(n2);

            contextMenu->addMenu(openIn);
            contextMenu->addMenu(openInNew);
        }



        contextMenu->exec(ev->globalPos());
    }
}
void BibleForm::newNoteWithLink()
{
    if(!m_moduleManager->verseTableLoaded(m_verseTable)) {
        return;
    }
    m_notesManager->newNoteWithLink(m_lastSelection, verseModule()->versification());
}
void BibleForm::newBookmark()
{
    if(!m_moduleManager->verseTableLoaded(m_verseTable)) {
        return;
    }
    m_bookmarksManager->newBookmark(m_lastSelection, verseModule()->versification());
}

void BibleForm::copyWholeVerse(void)
{
    //DEBUG_FUNC_NAME
    VerseSelection selection = m_lastSelection;
    if(selection.startVerse != -1) {
        Ranges ranges;
        if(selection.startChapterID == selection.endChapterID) {
            Range r;
            r.setModule(selection.moduleID);
            r.setBook(selection.bookID);
            r.setChapter(selection.startChapterID);
            r.setStartVerse(selection.startVerse);
            r.setEndVerse(selection.endVerse);
            ranges.addRange(r);
        } else {
            for(int i = selection.startChapterID; i <= selection.endChapterID; i++) {
                Range r;
                r.setModule(selection.moduleID);
                r.setBook(selection.bookID);
                r.setChapter(i);
                if(i == selection.startChapterID)
                    r.setStartVerse(selection.startVerse);
                else
                    r.setStartVerse(0);

                if(i == selection.endChapterID)
                    r.setEndVerse(selection.endVerse);
                else
                    r.setEndVerse(m_settings->getModuleSettings(selection.moduleID)->getV11n()->maxVerse().value(selection.bookID).at(selection.startChapterID));

                ranges.addRange(r);
            }
        }
        TextRangesResponse *res = (TextRangesResponse*) verseModule()->readRanges(ranges);
        QString stext = res->ranges().join(" ");
        delete res;

        QTextDocument doc2;
        doc2.setHtml(stext);
        stext = doc2.toPlainText();
        RefText refText(m_settings);
        refText.setShowModuleName(true);
        //todo: new line on windows
        const QString text = refText.toString(ranges) + "\n" + stext;
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(text);
    }
}

void BibleForm::newColorMark()
{
    if(!m_moduleManager->verseTableLoaded(m_verseTable)) {
        return;
    }
    const QString colorName = sender()->objectName();
    QColor color;
    if(colorName == "yellowMark") {
        color = QColor(255, 255, 0);
    } else if(colorName == "greenMark") {
        color = QColor(146, 243, 54);
    } else if(colorName == "blueMark") {
        color = QColor(77, 169, 243);
    } else if(colorName == "orangeMark") {
        color = QColor(243, 181, 57);
    } else if(colorName == "violetMark") {
        color = QColor(169, 102, 240);
    } else {
        color = QColor(255, 255, 0);
    }

    VerseSelection selection = m_lastSelection;
    m_notesManager->newCustomColorMark(selection, color, verseModule()->versification());
}

void BibleForm::newCustomColorMark()
{
    if(!m_moduleManager->verseTableLoaded(m_verseTable)) {
        return;
    }

    VerseSelection selection = m_lastSelection;
    const QColor color = QColorDialog::getColor(QColor(255, 255, 0), this);
    if(color.isValid()) {
        m_notesManager->newCustomColorMark(selection, color, verseModule()->versification());
    }
}

void BibleForm::newBoldMark()
{
    if(!m_moduleManager->verseTableLoaded(m_verseTable)) {
        return;
    }

    VerseSelection selection = m_lastSelection;
    m_notesManager->newBoldMark(selection, verseModule()->versification());
}

void BibleForm::newItalicMark()
{
    if(!m_moduleManager->verseTableLoaded(m_verseTable)) {
        return;
    }

    VerseSelection selection = m_lastSelection;
    m_notesManager->newItalicMark(selection, verseModule()->versification());
}

void BibleForm::newUnderlineMark()
{
    if(!m_moduleManager->verseTableLoaded(m_verseTable)) {
        return;
    }

    VerseSelection selection = m_lastSelection;
    m_notesManager->newUnderlineMark(selection, verseModule()->versification());
}

void BibleForm::removeMark()
{
    //DEBUG_FUNC_NAME;
    if(!m_moduleManager->verseTableLoaded(m_verseTable)) {
        return;
    }
    VerseSelection selection = m_lastSelection;
    m_notesManager->removeMark(selection, verseModule()->versification());
}

VerseSelection BibleForm::verseSelection()
{
    QWebFrame *f = m_view->page()->mainFrame();
    VerseSelection s;
    if(!f)
        return s;

    f->evaluateJavaScript("var vS = new VerseSelection(); vS.getSelection();");
    s.moduleID = f->evaluateJavaScript("vS.moduleID;").toInt();
    s.bookID  = f->evaluateJavaScript("vS.bookID;").toInt();
    s.startChapterID = f->evaluateJavaScript("vS.startChapterID;").toInt();
    s.endChapterID = f->evaluateJavaScript("vS.endChapterID;").toInt();
    s.startVerse = f->evaluateJavaScript("vS.startVerse;").toInt();
    s.endVerse = f->evaluateJavaScript("vS.endVerse;").toInt();
    myDebug() << "start verse = " << s.startVerse << " end verse = " << s.endVerse;

    const QString startVerseText = m_lastTextRanges.getVerse(s.bookID, s.startChapterID, s.startVerse).data();
    QString endVerseText;
    if(s.startVerse != s.endVerse || s.startChapterID != s.endChapterID)
        endVerseText = m_lastTextRanges.getVerse(s.bookID, s.endChapterID, s.endVerse).data();
    else
        endVerseText = startVerseText;

    const QString selectedText = f->evaluateJavaScript("vS.selectedText;").toString();
    myDebug() << "selected text = " << selectedText;
    myDebug() << "startVerseText = " << startVerseText;
    myDebug() << "endVerseText = " << endVerseText;
    {
        QString sText;
        for(int i = 0; i < selectedText.size() - 1; i++) {
            sText += selectedText.at(i);
            const int pos = startVerseText.indexOf(sText);
            if(pos != -1 && startVerseText.lastIndexOf(sText) == pos) {
                s.shortestStringInStartVerse = sText;
                break;
            }
        }
        if(s.shortestStringInStartVerse.isEmpty() && s.startVerse != s.endVerse) {
            //find the last long string if the selection is over more than one verse long
            //todo: it isn't alway unique
            QString lastLongest = selectedText;
            int lastPos = -2;
            for(int i = selectedText.size() - 1; i > 0; i--) {
                const int pos = startVerseText.lastIndexOf(lastLongest);
                if(pos != -1) {
                    lastPos = pos;
                    break;
                }
                lastLongest.remove(i, selectedText.size());
            }
            //and shorten it
            sText.clear();
            for(int i = 0; i < selectedText.size() - 1; i++) {
                sText += selectedText.at(i);
                const int pos = startVerseText.lastIndexOf(sText);
                if(pos != -1 && lastPos == pos) {
                    s.shortestStringInStartVerse = sText;
                    break;
                }
            }


        }
        sText.clear();
        for(int i = 0; i < selectedText.size() - 1; i++) {
            sText.prepend(selectedText.at(selectedText.size() - i - 1));
            const int pos = endVerseText.indexOf(sText);
            if(pos != -1 && endVerseText.lastIndexOf(sText) == pos) {
                s.shortestStringInEndVerse = sText;
                break;
            }
        }
        if(s.shortestStringInEndVerse.isEmpty() && s.startVerse != s.endVerse) {
            //find the first longest string if the selection is over more than one verse long
            QString firstLongest = selectedText;
            int firstPos = -2;
            for(int i = 0; i < selectedText.size(); i++) {
                const int pos = endVerseText.indexOf(firstLongest);
                if(pos != -1) {
                    firstPos = pos;
                    break;
                }
                firstLongest.remove(0, 1);
            }
            //and shorten it
            sText.clear();
            for(int i = 0; i < selectedText.size() - 1; i++) {
                sText.prepend(selectedText.at(selectedText.size() - i - 1));
                const int pos = endVerseText.indexOf(sText);
                if(pos != -1 && firstPos == pos) {
                    s.shortestStringInEndVerse = sText;
                    break;
                }
            }
        }
        s.type = VerseSelection::ShortestString;
        if(s.shortestStringInStartVerse.isEmpty() || s.shortestStringInEndVerse.isEmpty()) {
            s.setCanBeUsedForMarks(false);
        } else {
            s.setCanBeUsedForMarks(true);
        }
    }
    myDebug() << s.shortestStringInStartVerse << s.shortestStringInEndVerse;
    //do not this stuff with BibleQuote because some modules have weird html stuff.
    if(s.canBeUsedForMarks() == false && verseModule()->moduleType() != ModuleTools::BibleQuoteModule) {
        //now the ultimative alogrithm
        myDebug() << f->evaluateJavaScript("var adVerseSelection = new AdVerseSelection(); adVerseSelection.getSelect();");
        const QString startVerseText2 = f->evaluateJavaScript("adVerseSelection.startVerseText;").toString();
       /* myDebug() << "adVerseSelection.startVerse" << f->evaluateJavaScript("adVerseSelection.startVerse;").toString();
        myDebug() << "adVerseSelection.startVerseText;" << f->evaluateJavaScript("adVerseSelection.startVerseText;").toString();
        myDebug() << "adVerseSelection.startVerseContent;" << f->evaluateJavaScript("adVerseSelection.startVerseContent;").toString();
        myDebug() << "adVerseSelection.selectedText;"<< f->evaluateJavaScript("adVerseSelection.selectedText;").toString();*/

        const QString uniqueString = "!-_OPENBIBLEVIEWER_INSERT_-!";
        const int posOfInsert = startVerseText2.lastIndexOf(uniqueString);

        QString back = selectedText;
        QString longestString;
        for(int i = selectedText.size() - 1; i > 0; --i) {
            const int pos = startVerseText2.indexOf(back);
            if(pos != -1) {
                longestString = back;
                break;
            }
            back.remove(i, selectedText.size());
        }

        int count = 0;
        int currentPos = 0;
        while(true) {
            currentPos = startVerseText2.indexOf(longestString, currentPos + 1);
            if(currentPos > posOfInsert || currentPos == -1) {
                break;
            }
            count++;
        }
        s.type = VerseSelection::RepeatOfLongestString;
        s.repeat = count;
        s.longestString = longestString;
        if(!s.longestString.isEmpty())
            s.setCanBeUsedForMarks(true);
        //todo: end
        myDebug() << "longest = " << longestString << " count = " << count;
        f->evaluateJavaScript("removeSelectionStuff()");

    }
    return s;
}

bool BibleForm::verseTableLoaded()
{
    return m_moduleManager->verseTableLoaded(m_verseTable);
}

SearchableModule * BibleForm::searchableModule() const
{
    return (SearchableModule*)(verseModule());
}

VerseTable * BibleForm::verseTable() const
{
    return m_verseTable;
}

TextRangesVerseModule *BibleForm::verseModule() const
{
    return m_verseTable->verseModule();
}
void BibleForm::moduleChanged(const int moduleID)
{
    DEBUG_FUNC_NAME
	if(!m_verseTable)
		return;

    if(m_verseTable->contains(moduleID)) {
        reload(true);
    }
}

void BibleForm::openCommentary()
{
    QAction *s = (QAction*) sender();
    if(s != nullptr) {
        int moduleID = s->data().toInt();
        VerseUrl url = m_lastSelection.url();
        url.setModuleID(moduleID);
        url.setOpenToTransformation(false);
        m_actions->get(url);
        m_settings->getModuleSettings(moduleID)->stats_timesOpened++;
    }
}
void BibleForm::forwardDeleteModule(const int itemID)
{
    DEBUG_FUNC_NAME
    if(active())
        deleteModule(itemID);
}

void BibleForm::deleteModule(const int itemID)
{
    DEBUG_FUNC_NAME
    m_verseTable->removeModule(itemID);
    reload(true);
}


void BibleForm::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    myDebug() << e->type();
    if(e->type() == QEvent::LanguageChange) {
        deleteDefaultMenu();
        createDefaultMenu();
        m_ui->retranslateUi(this);
    }
}
