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
#include "commentaryform.h"
#include "ui_commentaryform.h"
#include "src/core/module/response/urlresponse.h"
#include "src/core/module/response/stringresponse.h"
#include "src/core/module/commentary/webcommentary.h"

#include "src/core/verse/reftext.h"
#include "src/core/link/biblelink.h"
#include "src/core/link/urlconverter2.h"
#include <QClipboard>
#include <QDesktopServices>
CommentaryForm::CommentaryForm(QWidget *parent) :
    WebViewForm(parent),
    ui(new Ui::CommentaryForm),
    m_url(),
    m_com(nullptr)
{
    ui->setupUi(this);
    ui->verticalLayout->addWidget(m_view);
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(changeLocation()));
    //TODO: Webm_view->page()->setLinkDelegationPolicy(QWebEnginePage::DelegateAllLinks);

    connect(ui->toolButton_backward, SIGNAL(clicked()), this, SLOT(backward()));
    connect(ui->toolButton_forward, SIGNAL(clicked()), this, SLOT(forward()));
    connect(ui->toolButton_openInBrowser, SIGNAL(clicked()), this, SLOT(openInBrowser()));
    connect(ui->toolButton_openInBible, SIGNAL(clicked()), this, SLOT(openInBible()));
    ui->toolButton_backward->setShortcut(QKeySequence::Back);
    ui->toolButton_forward->setShortcut(QKeySequence::Forward);
    setButtons();
}


CommentaryForm::~CommentaryForm()
{
    delete ui;
    if(m_com != nullptr) {
        delete m_com;
        m_com = nullptr;
    }
}

Form::FormType CommentaryForm::type() const
{
    return Form::CommentaryForm;
}

ModuleID CommentaryForm::moduleID() const
{
    if(m_com == nullptr) return ModuleIDNotSet;
    return m_com->moduleID();
}

void CommentaryForm::init()
{
    m_com = new Commentary();
    m_moduleManager->initSimpleModule(m_com);

    connect(m_view->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(get(QUrl)));
    connect(m_view, SIGNAL(linkMiddleOrCtrlClicked(QUrl)), this, SLOT(newGet(QUrl)));
    connect(m_view, SIGNAL(contextMenuRequested(QContextMenuEvent*)), this, SLOT(showContextMenu(QContextMenuEvent*)));

}

void CommentaryForm::activated()
{
    actTitle();
}

void CommentaryForm::actTitle()
{
    if(m_com != nullptr) {
        m_actions->setTitle(m_com->moduleTitle());
        m_actions->setCurrentModule(m_com->moduleID());
        if(m_com->loaded()) {
            m_actions->updateChapters(m_com->currentBook(), m_com->versification());
            m_actions->updateBooks(m_com->versification());
            m_actions->setCurrentChapter(m_com->currentChapter());
            m_actions->setCurrentBook(m_com->currentBook());
        }

    } else {
        m_actions->setTitle(QString());
        m_actions->clearBooks();
        m_actions->clearChapters();
    }
}

void CommentaryForm::parseUrl(QString string)
{
    VerseUrl url;
    if(!url.fromStringUrl(string)) {
        myWarning() << "pharsing url failed:" << string;
        return;
    }

    parseUrl(url);
}
void CommentaryForm::parseUrl(VerseUrl url)
{
    myDebug() << url.toString();
    if(m_com == nullptr) {
        myWarning() << "m_com is null";
        return;
    }

    if(m_url.isValid()) {
        m_url = m_url.applyUrl(url);
    } else {
        m_url = url;
        if(loaded() && !m_url.hasModuleID()) {
            m_url.setModuleID(m_com->moduleID());
        }
    }
    //myDebug() << "m_url = " << m_url.toString();
    bool showStart = true;
    foreach(VerseUrlRange range, m_url.ranges()) {
        if(range.book() != VerseUrlRange::LoadCurrentBook ||
           range.chapter() != VerseUrlRange::LoadCurrentChapter ||
           range.startVerse() != VerseUrlRange::LoadCurrentVerse) {
           showStart = false;
        }
    }

    if(showStart) {
        showResponse(m_com->readStart(m_url.moduleID()));
    } else {
        Ranges ranges;
        foreach(VerseUrlRange range, m_url.ranges()) {
            ranges.addRange(range.toRange());
        }
        ranges.setSource(m_url);
        showRanges(ranges, m_url);
    }

}
void CommentaryForm::showRanges(Ranges ranges, const VerseUrl &source)
{
    if(m_com == nullptr) {
        myWarning() << "m_com is null";
        return;
    }

    showResponse(m_com->readRanges(ranges));

    RefText ref(m_settings);
    ui->lineEdit->setText(ref.toString(ranges));

    historySetUrl(source.toString());
    actTitle();
}

void CommentaryForm::showUrlResponse(UrlResponse *res)
{
    myDebug() << res->url();
    m_lastUrl = QUrl(res->url());

    if(!res->blockRules().isEmpty()) {
        m_view->setBlockRules(res->blockRules());
    }

    m_view->load(m_lastUrl);
    this->actTitle();
}

/*
 * when line_edit enter is pressed
 */
void CommentaryForm::changeLocation()
{
    BibleLink link(m_com->moduleID(), m_com->versification());
    VerseUrl url = link.getUrl(ui->lineEdit->text());
    url.setOpenToTransformation(false);
    parseUrl(url);
}

bool CommentaryForm::loaded()
{
    return m_com != nullptr && m_com->loaded();
}
void CommentaryForm::historySetUrl(QString url)
{
    m_browserHistory.setCurrent(url);
    setButtons();
}
void CommentaryForm::backward()
{
    m_actions->get(m_browserHistory.backward());
    setButtons();
}
void CommentaryForm::forward()
{
    m_actions->get(m_browserHistory.forward());
    setButtons();
}
void CommentaryForm::setButtons()
{
    if(m_browserHistory.backwardAvailable()) {
        ui->toolButton_backward->setDisabled(false);
    } else {
        ui->toolButton_backward->setDisabled(true);
    }
    if(m_browserHistory.forwardAvailable()) {
        ui->toolButton_forward->setDisabled(false);
    } else {
        ui->toolButton_forward->setDisabled(true);
    }
    if(m_lastUrl.isEmpty()) {
        ui->toolButton_openInBrowser->hide();
    } else {
        ui->toolButton_openInBrowser->show();
    }
}
void CommentaryForm::showContextMenu(QContextMenuEvent* ev)
{
     //TODO: WEB
     /*
    //DEBUG_FUNC_NAME
    QScopedPointer<QMenu> contextMenu(new QMenu(this));
    QAction * actionSelect = new QAction(QIcon::fromTheme("edit-select-all", QIcon(":/icons/16x16/edit-select-all.png")), tr("Select All"), this);
    connect(actionSelect, SIGNAL(triggered()), this , SLOT(selectAll()));

    QAction *actionCopy = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Copy"), this);
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
    if(m_view->hasSelection()) {
        actionCopy->setEnabled(true);
    } else {
        actionCopy->setEnabled(false);
    }

    const QWebHitTestResult hitTest = m_view->page()->mainFrame()->hitTestContent(ev->pos());
    const QString url = transformUrl(hitTest.linkElement().attribute("href"));
    if(hitTest.linkUrl().isEmpty()) {

        QAction *dbg = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Debugger"), contextMenu.data());
        connect(dbg, SIGNAL(triggered()), this, SLOT(debugger()));

        contextMenu->addAction(actionSelect);
        contextMenu->addAction(actionCopy);
        contextMenu->addAction(dbg);
        contextMenu->exec(ev->globalPos());

    } else {
        myDebug() << "another menu";
        m_contextMenuUrl = url;
        m_contextMenuText = hitTest.linkText();

        QAction *openInNewTab = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("Open in new tab"), contextMenu.data());
        connect(openInNewTab, SIGNAL(triggered()), this, SLOT(openInNewTab()));

        QAction *openHere = new QAction(QIcon::fromTheme("tab-new-background", QIcon(":/icons/16x16/tab-new-background.png")), tr("Open"), contextMenu.data());
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

        ModuleTools::ContentType type = ModuleTools::contentTypeFromUrl(url);
        ModuleTools::ModuleClass cl = ModuleTools::moduleClassFromUrl(url);
        QList<ModuleSettings*> list = m_settings->m_moduleSettings.values();

        qSort(list.begin(), list.end(), ModuleManager::sortModuleByPop);

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
            if(counter > 5)
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
        contextMenu->addAction(actionSelect);
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
*/
}
QString CommentaryForm::transformUrl(const QString &url)
{
    if(m_com && m_com->moduleType() == ModuleTools::WebCommentaryModule) {
        QString nurl = ((WebCommentary*) (m_com->m_commentaryModule.data()))->parseUrl(QUrl(url));
        return nurl;
    } else if(url.startsWith(ModuleTools::theWordScheme)) {
        VerseUrl vurl;
        vurl.fromTheWord(url);
        return vurl.toString();
    }
    return url;
}
void CommentaryForm::get(QUrl url)
{
    if(m_com->moduleType() == ModuleTools::TheWordCommentaryModule) {
        m_actions->get(transformUrl(QString(url.toEncoded())));
        return;
    }

    const QString u = transformUrl(url.toString());
    if(u.startsWith("http") && m_com->m_commentaryModule->linkPolicy() == CommentaryModule::OpenWebLinksHere) {
         m_view->load(QUrl(u));
         return;
    }
    if(url.toString().startsWith(ModuleTools::verseScheme)) {
        //probably a ref to scripture, so check if it really is
        VerseUrl u(url.toString());
        if(u.ranges().size() > 0) {
            VerseUrlRange r = u.ranges().first();
            if(r.module() != VerseUrlRange::LoadModuleByID && r.module() != VerseUrlRange::LoadModuleByUID) {
                myDebug() << "open in other window";
                m_actions->get(url, Actions::OpenInOtherWindow);
                return;
            }
        }
    }
    m_actions->get(u);
    return;
}

void CommentaryForm::newGet(QUrl url)
{
    if(m_com->moduleType() == ModuleTools::TheWordCommentaryModule) {
        m_actions->newGet(transformUrl(QString(url.toEncoded())));
    } else {
        m_actions->newGet(transformUrl(url.toString()));
    }

}
void CommentaryForm::openInBrowser()
{
    if(m_lastUrl.isEmpty()) return;
    QDesktopServices::openUrl(m_lastUrl);
}

void CommentaryForm::openInBible()
{
    m_actions->getIn(m_url, Actions::BibleType);
}

void CommentaryForm::save()
{
    const QString a = m_settings->session.id() + "/windows/" + QString::number(m_id) + "/";
    m_settings->session.file()->setValue(a + "type", "commentary");

    VerseUrl url = m_url;
    url.setModuleID(m_com->moduleID());

    UrlConverter urlConverter(UrlConverter::InterfaceUrl, UrlConverter::PersistentUrl, url);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleManager->m_moduleMap.data());
    VerseUrl newUrl = urlConverter.convert();
    m_settings->session.file()->setValue(a + "url", newUrl.toString());


    //TODO: Web m_settings->session.file()->setValue(a + "scrollPosition", m_view->page()->mainFrame()->scrollPosition());
    m_settings->session.file()->setValue(a + "zoom", m_view->zoomFactor());

    m_settings->session.file()->setValue(a + "hist1", m_browserHistory.data1());
    m_settings->session.file()->setValue(a + "hist2", m_browserHistory.data2());
    m_settings->session.file()->setValue(a + "hist3", m_browserHistory.data3());

}

void CommentaryForm::restore(const QString &key)
{
    const QString a = m_settings->session.id() + "/windows/" + key + "/";
    const qreal zoom = m_settings->session.file()->value(a + "zoom").toReal();
    const QPoint scroll = m_settings->session.file()->value(a + "scrollPosition").toPoint();

    const QVariant v = m_settings->session.file()->value(a + "hist1");
    const QVariant v2 = m_settings->session.file()->value(a + "hist2");
    if(!v.toStringList().isEmpty() || !v2.toStringList().isEmpty()) {
        m_browserHistory.setData1(v);
        m_browserHistory.setData2(v2);
        m_browserHistory.setData3(m_settings->session.file()->value(a + "hist3"));
        setButtons();
    }
    const QString url = m_settings->session.file()->value(a + "url").toString();
    UrlConverter2 urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
    urlConverter.setSM(m_settings, m_moduleManager->m_moduleMap.data());
    urlConverter.convert();

    if(urlConverter.moduleID() != -1) {
        parseUrl(urlConverter.url());//these urls are handeld by this Form
    }


    //TODO: Webm_view->page()->mainFrame()->setScrollPosition(scroll);
    m_view->setZoomFactor(zoom);

   /* QUrl url = m_settings->session.file()->value(a + "url").toUrl();
    if(url.isValid())
        m_view->load(url);
    */
}
