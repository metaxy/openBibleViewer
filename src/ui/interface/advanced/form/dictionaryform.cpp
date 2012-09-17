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
#include <QtGui/QPrinter>
#include <QtCore/QPointer>
#include <QtGui/QTextDocumentWriter>
#include "src/module/dictionary/webdictionary.h"
#include "src/module/response/urlresponse.h"
#include "src/module/response/stringresponse.h"
#include "src/module/response/htmlresponse.h"
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

    ui->splitter->addWidget(m_view);
    ui->verticalLayout->setMargin(0);
    ui->horizontalLayout->setMargin(0);

    setButtons();

    m_model = new QStandardItemModel(ui->listView);

    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_selectionModel = new QItemSelectionModel(m_proxyModel);
    ui->listView->setModel(m_proxyModel);
    ui->listView->setSelectionModel(m_selectionModel);

    connect(ui->listView, SIGNAL(activated(QModelIndex)), this, SLOT(loadKey(QModelIndex)));


    m_lastFilledModuleID = -1;

    connect(ui->toolButton_openIn, SIGNAL(clicked()), this, SLOT(openInMenu()));
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
    connect(m_view, SIGNAL(linkMiddleOrCtrlClicked(QUrl)), this,  SLOT(newGet(QUrl)));

    connect(m_view, SIGNAL(contextMenuRequested(QContextMenuEvent*)), this, SLOT(showContextMenu(QContextMenuEvent*)));

    createDefaultMenu();
}
void DictionaryForm::get(const QUrl &url)
{
    DEBUG_FUNC_NAME
    if(m_dictionary->moduleType() == ModuleTools::WebDictionaryModule) {
        WebDictionary *d = (WebDictionary*)(m_dictionary->module());
        m_actions->get(d->parseUrl(url));
    } else {
        m_actions->get(url);
    }
}
void DictionaryForm::newGet(const QUrl &url)
{
    DEBUG_FUNC_NAME
    if(m_dictionary->moduleType() == ModuleTools::WebDictionaryModule) {
        WebDictionary *d = (WebDictionary*)(m_dictionary->module());
        m_actions->get(d->parseUrl(url), Actions::OpenInNewWindow);
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
    ui->splitter->restoreState(m_settings->session.file()->value(a + "splitterSizes").toByteArray());
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
    m_settings->session.file()->setValue(a + "splitterSizes", ui->splitter->saveState());


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
    myDebug() << key << moduleID;
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
        } else if(r->type() == Response::HtmlResponse) {
            HtmlResponse *ht = (HtmlResponse*) r;
            showHtml(ht->data());
        }  else {
            showHtml(tr("Unkown Responsetype"));
        }
        delete r;
    }
    selectKey(m_key);
    m_actions->setTitle(m_dictionary->moduleTitle());

}
void DictionaryForm::loadDictionary(int moduleID)
{
    DEBUG_FUNC_NAME
    if(moduleID >= 0) {
        myDebug() << "moduleID = " << moduleID;
        Module *m = m_moduleManager->getModule(moduleID);
        ModuleTools::ModuleType type = m->moduleType();
        ModuleTools::ContentType contentType = m_settings->getModuleSettings(moduleID)->contentType;
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
        if(ModuleTools::alsoOk(ModuleTools::StrongsContent, contentType)) {
            //hide splitter
            myDebug() << "hide splitter";
            QList<int> sizes;
            sizes << 0 << 1;
            ui->splitter->setSizes(sizes);
        }
        fillList();

    }
}
void DictionaryForm::fillList()
{
    if(m_lastFilledModuleID != m_dictionary->moduleID()) {
        m_model->clear();
        QStandardItem *parentItem = m_model->invisibleRootItem();
        QStringList keys = m_dictionary->getAllKeys();
        foreach(const QString &id, keys) {
            QStandardItem *item = new QStandardItem(id);
            parentItem->appendRow(item);
        }
    }
}
void DictionaryForm::loadKey(const QModelIndex &index)
{
    showEntry(index.data().toString(), m_dictionary->moduleID());
}
void DictionaryForm::selectKey(const QString &key)
{
    const QModelIndexList list = m_proxyModel->match(m_model->invisibleRootItem()->index(), Qt::UserRole , key);

    if(!list.isEmpty()) {
        m_selectionModel->clearSelection();
        m_selectionModel->setCurrentIndex(m_proxyModel->mapFromSource(list.first()), QItemSelectionModel::Select);
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
            url = QUrl(d->parseUrl(url));
        }

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
void DictionaryForm::openInMenu()
{
    QScopedPointer<QMenu> contextMenu(new QMenu(this));
    QList<int> usedModules;
    ModuleTools::ContentType type = m_settings->getModuleSettings(dictionary()->moduleID())->contentType;
    ModuleTools::ModuleClass cl = ModuleTools::typeToClass(dictionary()->moduleType());
    QList<ModuleSettings*> list = m_settings->m_moduleSettings.values();

    qSort(list.begin(), list.end(), ModuleManager::sortModuleByPop);

    bool addSep = true;
    int counter = 0;

    if(type != ModuleTools::UnkownContent) {
        //most 7 used with the right content type

        foreach(ModuleSettings* m, list) {
            if(counter > 8)
                break;
            if(ModuleTools::alsoOk(m->contentType, type)
                    && m->contentType != ModuleTools::UnkownContent
                    && !usedModules.contains(m->moduleID)) {

                usedModules.append(m->moduleID);
                counter++;

                QAction *n = new QAction(m->name(true), contextMenu.data());
                n->setData(m->moduleID);
                connect(n, SIGNAL(triggered()), this, SLOT(openIn()));
                contextMenu->addAction(n);
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

            usedModules.append(m->moduleID);
            counter++;
            if(addSep) {
                contextMenu->addSeparator();
                addSep = false;
            }

            QAction *n = new QAction(m->name(true), contextMenu.data());
            n->setData(m->moduleID);
            connect(n, SIGNAL(triggered()), this, SLOT(openIn()));
            contextMenu->addAction(n);
        }
    }
    m_contextMenuUrl = m_browserHistory.current();

    contextMenu->exec(ui->toolButton_openIn->parentWidget()->mapToGlobal(ui->toolButton_openIn->frameGeometry().bottomLeft()));

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
