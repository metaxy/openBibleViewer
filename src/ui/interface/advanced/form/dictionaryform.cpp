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
#include "dictionaryform.h"
#include "ui_dictionaryform.h"

#include <QtGui/QCompleter>
#include <QtCore/QScopedPointer>
#include <QtGui/QFileDialog>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtCore/QPointer>
#include <QtGui/QPrintPreviewDialog>
#include <QtGui/QTextDocumentWriter>
#include "src/module/dictionary/webdictionary.h"
#include "src/module/response/urlresponse.h"
#include "src/module/response/stringresponse.h"
#include <QtGui/QClipboard>

DictionaryForm::DictionaryForm(QWidget *parent) :
    WebViewForm(parent),
    ui(new Ui::DictionaryForm)
{
    ui->setupUi(this);
    connect(ui->toolButton_enter, SIGNAL(clicked()), this, SLOT(showEntry()));
    connect(ui->lineEdit_input, SIGNAL(returnPressed()), this, SLOT(showEntry()));
    connect(ui->toolButton_forward, SIGNAL(clicked()), this, SLOT(forward()));
    connect(ui->toolButton_back, SIGNAL(clicked()), this, SLOT(backward()));
    ui->toolButton_back->setShortcut(QKeySequence::Back);
    ui->toolButton_forward->setShortcut(QKeySequence::Forward);

    m_dictionary = NULL;

    ui->verticalLayout->addWidget(m_view);
    ui->verticalLayout->setMargin(0);

    setButtons();

}

DictionaryForm::~DictionaryForm()
{
    delete ui;
    if(m_dictionary != NULL) {
        delete m_dictionary;
        m_dictionary = NULL;
    }
}
void DictionaryForm::init()
{
    m_view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(m_view->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(get(QUrl)));
    connect(m_view, SIGNAL(linkMiddleOrCtrlClicked(QUrl)), SLOT(newGet(QUrl)));
    connect(m_view, SIGNAL(contextMenuRequested(QContextMenuEvent*)), this, SLOT(showContextMenu(QContextMenuEvent*)));


    m_view->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    m_view->settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
#if QT_VERSION >= 0x040700
    m_view->settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    m_view->settings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    m_view->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    m_view->settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
    m_view->settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
#endif
    createDefaultMenu();
}
void DictionaryForm::get(const QUrl &url)
{
    DEBUG_FUNC_NAME
    if(m_dictionary->moduleType() == ModuleTools::WebDictionaryModule) {
        WebDictionary *d = (WebDictionary*)(m_dictionary->module());
        m_actions->get(d->pharseUrl(url));
    } else {
        m_actions->get(url);
    }
}
void DictionaryForm::newGet(const QUrl &url)
{
    DEBUG_FUNC_NAME
    if(m_dictionary->moduleType() == ModuleTools::WebDictionaryModule) {
        WebDictionary *d = (WebDictionary*)(m_dictionary->module());
        m_actions->get(d->pharseUrl(url), Actions::OpenInNewWindow);
    } else {
        m_actions->get(url, Actions::OpenInNewWindow);
    }
}
Form::FormType DictionaryForm::type() const
{
    return Form::DictionaryForm;
}
Dictionary* DictionaryForm::dictionary() const
{
    return m_dictionary;
}
void DictionaryForm::restore(const QString &key)
{
    const QString a = m_settings->session.id() + "/windows/" + key + "/";
    const qreal zoom = m_settings->session.file()->value(a + "zoom").toReal();
    const QPoint scroll = m_settings->session.file()->value(a + "scrool").toPoint();

    const QString k = m_settings->session.file()->value(a + "key").toString();
    const QString uid = m_settings->session.file()->value(a + "uid").toString();



    int moduleID = -1;
    foreach(Module * m, m_moduleManager->m_moduleMap->data) {
        if(m->moduleUID() == uid) {
            moduleID = m->moduleID();
            break;
        }
    }
    if(moduleID != -1) {
        showEntry(k, moduleID);
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

void DictionaryForm::save()
{
    const QString a = m_settings->session.id() + "/windows/" + QString::number(m_id) + "/";
    m_settings->session.file()->setValue(a + "type", "dictionary");
    m_settings->session.file()->setValue(a + "key", m_key);
    m_settings->session.file()->setValue(a + "scrollPosition", m_view->page()->mainFrame()->scrollPosition());
    m_settings->session.file()->setValue(a + "zoom",  m_view->zoomFactor());
    m_settings->session.file()->setValue(a + "hist1", m_browserHistory.data1());
    m_settings->session.file()->setValue(a + "hist2", m_browserHistory.data2());
    m_settings->session.file()->setValue(a + "hist3", m_browserHistory.data3());


    if(m_dictionary != NULL) {
        m_settings->session.file()->setValue(a + "uid", m_moduleManager->getModule(m_dictionary->moduleID())->moduleUID());
    }
}


void DictionaryForm::activated()
{
    if(m_dictionary == NULL)
        return;
    m_actions->setTitle(m_dictionary->moduleTitle());
    m_actions->setCurrentModule(m_dictionary->moduleID());
}
void DictionaryForm::showEntry()
{
    const QString key = ui->lineEdit_input->text();
    showEntry(key,-1);
}
/**
  * moduleID == -1 means current Dictionary
  */
void DictionaryForm::showEntry(const QString &key, int moduleID)
{
    myDebug() << key;

    if(moduleID == -1) {
        historySetUrl(ModuleTools::dictScheme+QString::number(m_dictionary->moduleID())+"/"+key);
    } else {
        historySetUrl(ModuleTools::dictScheme+QString::number(moduleID)+"/"+key);
    }

    ui->lineEdit_input->setText(key);

    if(moduleID != -1)
        testDictionary(moduleID);

    m_key = "";

    if(key.isEmpty()) {
        showHtml(m_dictionary->moduleTitle());
    } else {
        m_key = key;
        Response *r = m_dictionary->getEntry(key);

        if(r->type() == Response::StringReponse) {
            StringResponse *st = (StringResponse*) r;
            QString data = st->data();
            if(!data.contains("<html>")) {
                data.prepend("<html><head><script type='text/javascript' src='qrc:/data/js/tools.js'></script></head><body><div class='dictionary'>");
                //data.append("</div></body></html>");
            }
            showHtml(data);

        } else if(r->type() == Response::UrlReponse) {
            UrlResponse *ut = (UrlResponse*) r;
            m_view->load(QUrl(ut->url()));
        } else {
            showHtml(tr("Unkown Responsetype"));
        }
        delete r;
    }

    m_actions->setTitle(m_dictionary->moduleTitle());

}
void DictionaryForm::loadDictionary(int moduleID)
{
    DEBUG_FUNC_NAME
    if(moduleID >= 0) {
        myDebug() << "moduleID = " << moduleID;
        Module *m = m_moduleManager->getModule(moduleID);
        ModuleTools::ModuleType type = m->moduleType();
        m_dictionary->setModuleType(type);
        int ret = m_dictionary->loadModuleData(moduleID);
        if(ret == 1) {
            showHtml("Could not load module " + QString::number(moduleID));
            return;
        }

        //todo: do this when closing
        m_settings->session.setData("lastDictModuleInWindow", m_settings->savableUrl(m->path()));


        QCompleter *completer = new QCompleter(m_dictionary->getAllKeys(), this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        if(ui->lineEdit_input->completer()) {
            delete ui->lineEdit_input->completer();
        }
        ui->lineEdit_input->setCompleter(completer);

        m_actions->setTitle(m_dictionary->moduleTitle());
        m_actions->setCurrentModule(m_dictionary->moduleID());
    }
}
void DictionaryForm::testDictionary(const int module)
{
    if(m_dictionary == NULL) {
        m_dictionary = new Dictionary();
        m_moduleManager->initSimpleModule(m_dictionary);
    }
    int newModuleID = newDictionary(module);
    if(newModuleID != -1)
        loadDictionary(newModuleID);
}
int DictionaryForm::newDictionary(const int module)
{
    if(!m_moduleManager->metaModuleLoaded(m_dictionary)) {
        if(module == -1) {
            const QString last = m_settings->session.getData("lastDictModuleInWindow").toString();
            int moduleID = -1;

            const QString lastUrl = m_settings->recoverUrl(last);
            QMapIterator<int, Module *> i(m_moduleManager->m_moduleMap->data);
            while(i.hasNext()) {
                i.next();
                if(i.value()->moduleClass() == ModuleTools::DictionaryModuleClass) {
                    moduleID = i.key();
                    if(i.value()->path() == lastUrl) {
                        break;
                    }
                }
            }

            if(moduleID == -1) {
                myDebug() << "no Dictionaries found";
                return -1;
            }
            return moduleID;
        } else {
            return module;
        }
    } else if(m_dictionary->moduleID() != module && module != -1) {
        return module;
    }
    return -1;
}

void DictionaryForm::historySetUrl(const QString &url)
{
    myDebug() << url;
    m_browserHistory.setCurrent(url);
    setButtons();
}
void DictionaryForm::backward()
{
    m_actions->get(m_browserHistory.backward());
    setButtons();
}
void DictionaryForm::forward()
{
    m_actions->get(m_browserHistory.forward());
    setButtons();
}
void DictionaryForm::setButtons()
{
    if(m_browserHistory.backwardAvailable()) {
        ui->toolButton_back->setDisabled(false);
    } else {
        ui->toolButton_back->setDisabled(true);
    }
    if(m_browserHistory.forwardAvailable()) {
        ui->toolButton_forward->setDisabled(false);
    } else {
        ui->toolButton_forward->setDisabled(true);
    }
}

void DictionaryForm::showHtml(QString html)
{
    if(m_dictionary->moduleType() != ModuleTools::WebDictionaryModule) {
        QString cssFile;
        if(m_dictionary)
            cssFile = m_settings->getModuleSettings(m_dictionary->moduleID())->styleSheet;
        if(cssFile.isEmpty())
            cssFile = ":/data/css/default.css";
        m_view->settings()->setUserStyleSheetUrl(QUrl::fromLocalFile(cssFile));
    }
    m_view->setHtml(html);
}

QString DictionaryForm::key() const
{
    return m_key;
}


void DictionaryForm::createDefaultMenu()
{
    m_actionSelect = new QAction(QIcon::fromTheme("edit-select-all", QIcon(":/icons/16x16/edit-select-all.png")), tr("Select All"), this);
    connect(m_actionSelect, SIGNAL(triggered()), this , SLOT(selectAll()));
}
void DictionaryForm::deleteDefaultMenu()
{
    delete m_actionSelect;
    m_actionSelect = 0;
}

void DictionaryForm::showContextMenu(QContextMenuEvent* ev)
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
    QUrl url = hitTest.linkUrl();
    if(url.isEmpty()) {


        QAction *dbg = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Debugger"), contextMenu.data());
        connect(dbg, SIGNAL(triggered()), this, SLOT(debugger()));

        contextMenu->addAction(actionCopy);
        contextMenu->addAction(m_actionSelect);
        contextMenu->addSeparator();
        contextMenu->addAction(dbg);
        contextMenu->exec(ev->globalPos());
    } else {
        myDebug() << "another menu";

        if(m_dictionary->moduleType() == ModuleTools::WebDictionaryModule) {
            WebDictionary *d = (WebDictionary*)(m_dictionary->module());
            url = QUrl(d->pharseUrl(url));
        }

        m_contextMenuUrl = url;
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

        ModuleTools::ContentType type = ModuleTools::contentTypeFromUrl(url.toString());
        ModuleTools::ModuleClass cl = ModuleTools::moduleClassFromUrl(url.toString());
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
            contextMenu->addMenu(openIn);
            contextMenu->addMenu(openInNew);
        }

        contextMenu->exec(ev->globalPos());
    }
}
void DictionaryForm::openInNewTab()
{
    m_actions->get(m_contextMenuUrl.toString(), Actions::OpenInNewWindow);
}

void DictionaryForm::openHere()
{
    m_actions->get(m_contextMenuUrl.toString(), Actions::NoModifer);
}

void DictionaryForm::copyLinkText()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_contextMenuText);
}

void DictionaryForm::copyLinkUrl()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_contextMenuUrl.toString());
}
void DictionaryForm::openIn()
{
    QAction *s = (QAction*) sender();
    if(s != NULL) {
        int moduleID = s->data().toInt();
        QString url = m_contextMenuUrl.toString();
        if(url.startsWith(ModuleTools::strongScheme)) {
            url = url.remove(0, ModuleTools::strongScheme.size());
            m_actions->get(ModuleTools::dictScheme + QString::number(moduleID) + "/" + url);
        } else if(url.startsWith(ModuleTools::rmacScheme)) {
            url = url.remove(0, ModuleTools::rmacScheme.size());
            m_actions->get(ModuleTools::dictScheme + QString::number(moduleID) + "/" + url);
        } else if(url.startsWith(ModuleTools::verseScheme)) {
            VerseUrl vurl;
            vurl.fromStringUrl(url);
            vurl.setModuleID(moduleID);
            m_actions->get(vurl);
        } else {
            m_actions->get(url);
        }
        m_settings->getModuleSettings(moduleID)->stats_timesOpend++;
    }
}
void DictionaryForm::openInNew()
{
    QAction *s = (QAction*) sender();
    if(s != NULL) {
        int moduleID = s->data().toInt();
        QString url = m_contextMenuUrl.toString();
        if(url.startsWith(ModuleTools::strongScheme)) {
            url = url.remove(0, ModuleTools::strongScheme.size());
            m_actions->get(ModuleTools::dictScheme + QString::number(moduleID) + "/" + url, Actions::OpenInNewWindow);
        } else if(url.startsWith(ModuleTools::rmacScheme)) {
            url = url.remove(0, ModuleTools::rmacScheme.size());
            m_actions->get(ModuleTools::dictScheme + QString::number(moduleID) + "/" + url, Actions::OpenInNewWindow);
        } else if(url.startsWith(ModuleTools::verseScheme)) {
            VerseUrl vurl;
            vurl.fromStringUrl(url);
            vurl.setModuleID(moduleID);
            m_actions->get(vurl, Actions::OpenInNewWindow);
        } else {
            m_actions->get(url);
        }
        m_settings->getModuleSettings(moduleID)->stats_timesOpend++;
    }
}
void DictionaryForm::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        deleteDefaultMenu();
        createDefaultMenu();
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
