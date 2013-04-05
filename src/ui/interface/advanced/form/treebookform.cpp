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
#include "treebookform.h"
#include "ui_treebookform.h"
#include "src/core/module/response/urlresponse.h"
#include "src/core/module/response/stringresponse.h"
#include "src/core/module/response/htmlresponse.h"
#include <QtWebKit/QWebElement>
TreeBookForm::TreeBookForm(QWidget *parent) :
    WebViewForm(parent),
    ui(new Ui::TreeBookForm),
    m_book(NULL)
{
    ui->setupUi(this);
    ui->splitter->addWidget(m_view);
    m_view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui->treeView, SIGNAL(activated(QModelIndex)), this, SLOT(loadChapter(QModelIndex)));

    connect(ui->toolButton_backward, SIGNAL(clicked()), this, SLOT(backward()));
    connect(ui->toolButton_forward, SIGNAL(clicked()), this, SLOT(forward()));

    ui->toolButton_backward->setShortcut(QKeySequence::Back);
    ui->toolButton_forward->setShortcut(QKeySequence::Forward);
    setButtons();
}

TreeBookForm::~TreeBookForm()
{
    if(m_book != NULL) {
        delete m_book;
        m_book = NULL;
    }
    delete ui;
}

void TreeBookForm::init()
{
    connect(m_view->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(get(QUrl)));
    connect(m_view, SIGNAL(linkMiddleOrCtrlClicked(QUrl)), this, SLOT(newGet(QUrl)));
    connect(m_view, SIGNAL(contextMenuRequested(QContextMenuEvent*)), this, SLOT(showContextMenu(QContextMenuEvent*)));
}

void TreeBookForm::restore(const QString &key)
{
    const QString a = m_settings->session.id() + "/windows/" + key + "/";
    const qreal zoom = m_settings->session.file()->value(a + "zoom").toReal();
    const QPoint scroll = m_settings->session.file()->value(a + "scrollPosition").toPoint();
    const QString uid = m_settings->session.file()->value(a + "uid").toString();
    const int chapter = m_settings->session.file()->value(a + "chapter").toInt();
    const QVariant v = m_settings->session.file()->value(a + "hist1");
    const QVariant v2 = m_settings->session.file()->value(a + "hist2");
    if(!v.toStringList().isEmpty() || !v2.toStringList().isEmpty()) {
        m_browserHistory.setData1(v);
        m_browserHistory.setData2(v2);
        m_browserHistory.setData3(m_settings->session.file()->value(a + "hist3"));
        setButtons();
    }
    int moduleID = m_moduleManager->m_moduleMap->UIDtoID(uid);
    if(moduleID != -1) {
        loadModule(moduleID);
        showChapter(moduleID, chapter);
    }
    ui->splitter->restoreState(m_settings->session.file()->value(a + "splitterSizes").toByteArray());
    m_view->page()->mainFrame()->setScrollPosition(scroll);
    m_view->setZoomFactor(zoom);
}

void TreeBookForm::save()
{
    const QString a = m_settings->session.id() + "/windows/" + QString::number(m_id) + "/";
    m_settings->session.file()->setValue(a + "type", "tbook");
    m_settings->session.file()->setValue(a + "scrollPosition", m_view->page()->mainFrame()->scrollPosition());
    m_settings->session.file()->setValue(a + "zoom",  m_view->zoomFactor());
    if(m_book != NULL) {
        m_settings->session.file()->setValue(a + "uid", m_moduleManager->getModule(m_book->moduleID())->moduleUID());
        m_settings->session.file()->setValue(a + "chapter", m_chapterID);
    }

    m_settings->session.file()->setValue(a + "hist1", m_browserHistory.data1());
    m_settings->session.file()->setValue(a + "hist2", m_browserHistory.data2());
    m_settings->session.file()->setValue(a + "hist3", m_browserHistory.data3());
    m_settings->session.file()->setValue(a + "splitterSizes", ui->splitter->saveState());

}

Form::FormType TreeBookForm::type() const
{
    return Form::TreeBookForm;
}

void TreeBookForm::activated()
{
    actTitle();
}
void TreeBookForm::actTitle()
{
    if(m_book) {
        m_actions->setTitle(m_book->moduleTitle());
        m_actions->setCurrentModule(m_book->moduleID());
    }
}

void TreeBookForm::loadModule(const int moduleID)
{
    if(m_book == NULL) {
        m_book = new TreeBook();
        m_moduleManager->initSimpleModule(m_book);
    } else {
        m_book->clearData();
    }
    ModuleTools::ModuleType type = m_moduleManager->getModule(moduleID)->moduleType();
    m_book->setModuleType(type);
    m_book->setModuleID(moduleID);
    m_book->loadModuleData(moduleID);

    m_actions->setTitle(m_book->moduleTitle());
    m_actions->setCurrentModule(m_book->moduleID());

    m_treeModel = new QStandardItemModel(ui->treeView);

    QStandardItem *parentItem = m_treeModel->invisibleRootItem();
    BookTree *tree = m_book->bookTree();
    createTree(parentItem, tree);


    m_proxyModel = new RecursivProxyModel(this);
    m_proxyModel->setSourceModel(m_treeModel);
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Chapter"));
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_selectionModel = new QItemSelectionModel(m_proxyModel);
    ui->treeView->setModel(m_proxyModel);
    ui->treeView->setSelectionModel(m_selectionModel);


}
void TreeBookForm::createTree(QStandardItem *parentItem, BookTree *parentTree)
{
    const QList<BookTree*> children = parentTree->children();
    for (int i = 0; i < children.size(); ++i) {
        BookTree *child = children.at(i);
        QStandardItem *item = new QStandardItem(child->title());
        item->setData(child->id());
        parentItem->appendRow(item);
        if(!child->children().isEmpty()) {
            createTree(item, child);
        }
    }
}

void TreeBookForm::showChapter(const int moduleID, const int chapterID)
{
    if(!m_book) {
        myWarning() << "m_book is null";
        return;
    }
    m_book->setModuleID(moduleID);
    Response *res = m_book->readChapter(chapterID);
    showResponse(res);

    selectChapter(chapterID);
    m_chapterID = chapterID;
    historySetUrl(ModuleTools::treeBookScheme + QString::number(moduleID)+","+QString::number(chapterID));

}
void TreeBookForm::loadChapter(QModelIndex index)
{
    const int chapterID = index.data(Qt::UserRole + 1).toInt();
    showChapter(m_book->moduleID(), chapterID);
}

void TreeBookForm::historySetUrl(QString url)
{
    m_browserHistory.setCurrent(url);
    setButtons();
}
void TreeBookForm::backward()
{
    m_actions->get(m_browserHistory.backward());
    setButtons();
}
void TreeBookForm::forward()
{
    m_actions->get(m_browserHistory.forward());
    setButtons();
}
void TreeBookForm::setButtons()
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
}
void TreeBookForm::showContextMenu(QContextMenuEvent* ev)
{
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

}
QString TreeBookForm::transformUrl(const QString &url)
{
    myDebug() << url;
    if(url.startsWith(ModuleTools::theWordScheme)) {
        VerseUrl vurl;
        vurl.fromTheWord(url);
        return vurl.toString();
    }
    return url;
}
void TreeBookForm::selectChapter(const int chapterID)
{
    if(chapterID == -1) {
        m_selectionModel->clearSelection();
        return;
    }

    const QModelIndexList list = m_proxyModel->match(m_treeModel->invisibleRootItem()->index(), Qt::UserRole + 1, QString::number(chapterID));

    if(list.size() == 1) {
        m_selectionModel->clearSelection();
        m_selectionModel->setCurrentIndex(m_proxyModel->mapFromSource(list.first()), QItemSelectionModel::Select);
    }
}
void TreeBookForm::get(QUrl url)
{
    if(m_book->moduleType() == ModuleTools::TheWordTopicModule) {
        m_actions->get(transformUrl(QString(url.toEncoded())));
    } else {
        m_actions->get(transformUrl(url.toString()));
    }
}

void TreeBookForm::newGet(QUrl url)
{
    if(m_book->moduleType() == ModuleTools::TheWordTopicModule) {
        m_actions->newGet(transformUrl(QString(url.toEncoded())));
    } else {
        m_actions->newGet(transformUrl(url.toString()));
    }

}
