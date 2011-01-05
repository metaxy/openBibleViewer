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

BibleForm::BibleForm(QWidget *parent) : QWidget(parent), m_ui(new Ui::BibleForm)
{
    //DEBUG_FUNC_NAME
    m_id = -1;
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

    m_api = 0;
    setButtons();
}
void BibleForm::setID(const int &id)
{
    m_id = id;
}

void BibleForm::init()
{
    //DEBUG_FUNC_NAME
    m_moduleManager->m_bibleList = new BibleList();
    Bible *b = new Bible();
    m_moduleManager->initBible(b);
    m_moduleManager->bibleList()->addBible(b, QPoint(0, 0));
    m_bibleList = m_moduleManager->m_bibleList;
    attachApi();
    connect(m_view->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attachApi()));

    connect(m_actions, SIGNAL(_updateChapters(QStringList)), this, SLOT(forwardSetChapters(QStringList)));
    connect(m_actions, SIGNAL(_updateBooks(QHash<int, QString>, QList<int>)), this, SLOT(forwardSetBooks(QHash<int, QString>, QList<int>)));

    connect(m_actions, SIGNAL(_clearBooks()), this, SLOT(forwardClearBooks()));
    connect(m_actions, SIGNAL(_clearChapters()), this, SLOT(forwardClearChapters()));

    connect(m_actions, SIGNAL(_setCurrentBook(QSet<int>)), this, SLOT(forwardSetCurrentBook(QSet<int>)));
    connect(m_actions, SIGNAL(_setCurrentChapter(QSet<int>)), this, SLOT(forwardSetCurrentChapter(QSet<int>)));
    connect(m_actions, SIGNAL(_historySetUrl(QString)), this, SLOT(forwardHistorySetUrl(QString)));
    connect(m_bibleDisplay, SIGNAL(newHtml(QString)), this, SLOT(forwardShowText(QString)));
    connect(m_actions, SIGNAL(_showTextRanges(QString, TextRanges, BibleUrl)), this, SLOT(forwardShowTextRanges(QString, TextRanges, BibleUrl)));


    connect(m_view, SIGNAL(contextMenuRequested(QContextMenuEvent*)), this, SLOT(showContextMenu(QContextMenuEvent*)));
    createDefaultMenu();
}
void BibleForm::setApi(Api *api)
{
    m_api = api;
}
void BibleForm::setBibleManager(BibleManager *bibleManager)
{
    m_bibleManager = bibleManager;
}

void BibleForm::setNotesManager(NotesManager *notesManager)
{
    m_notesManager = notesManager;
}
void BibleForm::attachApi()
{
    //DEBUG_FUNC_NAME
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
    m_api->bibleApi()->setFrame(frame);
}

void BibleForm::historySetUrl(QString url)
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
    BibleUrlRange r;
    r.setBible(BibleUrlRange::LoadCurrentBible);
    r.setStartBook(BibleUrlRange::LoadCurrentBook);
    r.setStartChapter(id);
    r.setStartVerse(BibleUrlRange::LoadFirstVerse);
    r.setEndVerse(BibleUrlRange::LoadLastVerse);
    BibleUrl url;
    url.addRange(r);

    m_actions->get(url);
}

void BibleForm::readBook(int id)
{
    const int i = m_bookIDs.at(id);

    BibleUrlRange r;
    r.setBible(BibleUrlRange::LoadCurrentBible);
    r.setStartBook(i);
    r.setStartChapter(BibleUrlRange::LoadFirstChapter);
    r.setStartVerse(BibleUrlRange::LoadFirstVerse);
    r.setEndVerse(BibleUrlRange::LoadLastVerse);
    BibleUrl url;
    url.addRange(r);

    m_actions->get(url);
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
    //DEBUG_FUNC_NAME
    //myDebug() << " windowID = " << m_id;
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
    //DEBUG_FUNC_NAME
    m_ui->comboBox_chapters->clear();
}
void BibleForm::setCurrentChapter(const QSet<int> &chapterID)
{
    //DEBUG_FUNC_NAME
    //myDebug() << " windowID = " << m_id;
    //myDebug() << chapterID;
    disconnect(m_ui->comboBox_chapters, SIGNAL(activated(int)), this, SLOT(readChapter(int)));
    int min = -1;
    foreach(int c, chapterID) {
        if(c < min || min == -1)
            min = c;
    }
    //myDebug() << "min = " << min;
    m_ui->comboBox_chapters->setCurrentIndex(min);
    connect(m_ui->comboBox_chapters, SIGNAL(activated(int)), this, SLOT(readChapter(int)));
}

void BibleForm::setBooks(const QHash<int, QString> &books, QList<int> ids)
{
    //DEBUG_FUNC_NAME
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
    //DEBUG_FUNC_NAME
    m_ui->comboBox_books->clear();
}
void BibleForm::setCurrentBook(const QSet<int> &bookID)
{
    //DEBUG_FUNC_NAME
    //todo: is there a better way then disconnect and connect?
    disconnect(m_ui->comboBox_books, SIGNAL(activated(int)), this, SLOT(readBook(int)));
    int min = -1;
    foreach(int b, bookID) {
        if(b < min || min == -1)
            min = b;
    }
    m_ui->comboBox_books->setCurrentIndex(m_moduleManager->bible()->bookIDs().indexOf(min));
    connect(m_ui->comboBox_books, SIGNAL(activated(int)), this, SLOT(readBook(int)));
}
void BibleForm::activated()
{
    //DEBUG_FUNC_NAME
    //myDebug() << " windowID = " << m_id;
    m_api->bibleApi()->setFrame(m_view->page()->mainFrame());
    BibleList *list = m_bibleList;
    if(m_bibleList == NULL || m_bibleList->bible() == NULL) {
        myDebug() << "no biblelist or no bible";
        clearChapters();
        clearBooks();
        m_actions->setTitle("");
        m_moduleManager->m_bibleList = NULL;
        return;
    }
    if(m_bibleList->bible()->moduleID() < 0) {
        myDebug() << "invalid moduleID";
        clearChapters();
        clearBooks();
        m_actions->setTitle("");
        m_moduleManager->m_bibleList = list;
        return;
    }
    m_moduleManager->m_bibleList = m_bibleList;

    m_actions->setTitle(m_moduleManager->bible()->moduleTitle());
    m_actions->setCurrentModule(m_moduleManager->bible()->moduleID());

    m_actions->updateChapters(m_moduleManager->bible()->chapterNames());
    m_actions->updateBooks(m_moduleManager->bible()->bookNames(), m_moduleManager->bible()->bookIDs());
    if(m_lastTextRanges.verseCount() != 0) {
        m_actions->setCurrentChapter(m_lastTextRanges.chapterIDs());
        m_actions->setCurrentBook(m_lastTextRanges.bookIDs());
    }
    m_moduleManager->bibleList()->setLastTextRanges(&m_lastTextRanges);
    m_moduleManager->bibleList()->setLastUrl(&m_lastUrl);


}

void BibleForm::scrollToAnchor(const QString &anchor)
{
    //DEBUG_FUNC_NAME
#if QT_VERSION >= 0x040700
    m_view->page()->mainFrame()->scrollToAnchor(anchor);
#else
    m_view->page()->mainFrame()->evaluateJavaScript("window.location.href = '" + anchor + "';");
#endif

}
void BibleForm::showText(const QString &text)
{
    //DEBUG_FUNC_NAME
    //myDebug() << " windowID = " << m_id;
    QString cssFile = m_settings->getModuleSettings(m_moduleManager->bible()->moduleID()).styleSheet;
    if(cssFile.isEmpty())
        cssFile = ":/data/css/default.css";

    QFile file(cssFile);
    if(file.open(QFile::ReadOnly))
        m_view->settings()->setUserStyleSheetUrl(QUrl("data:text/css;charset=utf-8;base64," + file.readAll().toBase64()));

    m_view->setHtml(text);
    if(m_lastTextRanges.verseCount() > 1) {
        //myDebug() << "scrolling";
        scrollToAnchor("currentEntry");
        if(m_moduleManager->bibleList()->hasTopBar())
            m_view->page()->mainFrame()->scroll(0, -40); //due to the biblelist bar on top
        //todo: it could be that the top bar has a width more than 40px
        //because the user zoomed in.
    }

    if(m_moduleManager->bible()->bibleType() == Module::BibleQuoteModule) {
        QWebElementCollection collection = m_view->page()->mainFrame()->documentElement().findAll("img");
        const QStringList searchPaths = m_moduleManager->bible()->getSearchPaths();

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
void BibleForm::forwardShowTextRanges(const QString &html, const TextRanges &range, const BibleUrl &url)
{
    if(!active())
        return;
    showTextRanges(html, range, url);
}

void BibleForm::showTextRanges(const QString &html, const TextRanges &range, const BibleUrl &url)
{
    //DEBUG_FUNC_NAME;
    showText(html);
    m_lastTextRanges = range;
    m_lastUrl = url;
    m_moduleManager->bibleList()->setLastTextRanges(&m_lastTextRanges);
    m_moduleManager->bibleList()->setLastUrl(&m_lastUrl);
    historySetUrl(url.toString());
}
void BibleForm::evaluateJavaScript(const QString &js)
{
    //DEBUG_FUNC_NAME
    m_view->page()->mainFrame()->evaluateJavaScript(js);
}
void BibleForm::print()
{
    QPrinter printer;
    QScopedPointer<QPrintDialog> dialog(new QPrintDialog(&printer, this));
    dialog->setWindowTitle(tr("Print"));
    if(dialog->exec() != QDialog::Accepted)
        return;


}
void BibleForm::printPreview()
{
    QPrinter printer;
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter *)), m_view, SLOT(print(QPrinter *)));
    preview.exec();
}

void BibleForm::saveFile()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    const QString lastPlace = m_settings->session.getData("lastSaveFilePlace").toString();
    const QString fileName = dialog.getSaveFileName(this, tr("Save output"), lastPlace, tr("Open Document (*.odt);;PDF (*.pdf);;Html (*.html *.htm);;Plain (*.txt)"));
    QFileInfo fi(fileName);
    m_settings->session.setData("lastSaveFilePlace", fi.path());
    if(fi.suffix().compare("html", Qt::CaseInsensitive) == 0 ||
            fi.suffix().compare("htm", Qt::CaseInsensitive) == 0) {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        out << m_view->page()->mainFrame()->toHtml();
        file.close();
    } else if(fi.suffix().compare("pdf", Qt::CaseInsensitive) == 0) {
        QPrinter printer;
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        m_view->print(&printer);
    } else if(fi.suffix().compare("odt", Qt::CaseInsensitive) == 0) {
        QTextDocumentWriter writer;
        writer.setFormat("odf");
        writer.setFileName(fileName);
        QTextDocument doc;
        doc.setHtml(m_view->page()->mainFrame()->toHtml());
        writer.write(&doc);
    } else {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        out << m_view->page()->mainFrame()->toPlainText();
        file.close();
    }

}
QString BibleForm::selectedText()
{
    return m_view->selectedText();
}

void BibleForm::copy()
{
    m_view->page()->triggerAction(QWebPage::Copy);
}

void BibleForm::selectAll()
{
    m_view->page()->triggerAction(QWebPage::SelectAll);
}
void BibleForm::forwardSetChapters(const QStringList &chapters)
{
    //DEBUG_FUNC_NAME
    // myDebug() << "chapters = " << chapters;
    if(!active())
        return;
    setChapters(chapters);
}

void BibleForm::forwardSetBooks(const QHash<int, QString> &books, QList<int> ids)
{
    if(!active())
        return;
    setBooks(books, ids);
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

void BibleForm::forwardShowText(const QString &text)
{
    //DEBUG_FUNC_NAME
    if(!active())
        return;
    showText(text);
}
void BibleForm::forwardHistorySetUrl(const QString &url)
{
    if(!active())
        return;
    historySetUrl(url);
}
bool BibleForm::active()
{
    //DEBUG_FUNC_NAME;
    /* QMdiArea *area = (QMdiArea*) this->parentWidget()->parentWidget();
     if(area) {
         QMdiSubWindow *window = area->activeSubWindow();
         myDebug() << "area = "<< area;
         myDebug() << "window = " << window;
         myDebug() << "mdiForm = " << window->widget()->findChild<BibleForm *>("mdiForm");
         myDebug() << "this = " << this;
         if(window && window->widget()->findChild<BibleForm *>("mdiForm") == this)
             return true;
     }
    */
    //myDebug() << "current = " << *currentWindowID << " m_ID = " << m_id;
    if(*currentWindowID == m_id)
        return true;
    //myDebug() << "false";
    return false;
}
int BibleForm::id()
{
    return m_id;
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

    QAction *actionCopyWholeVerse = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Copy Verse"), contextMenu.data());
    VerseSelection selection = verseSelection();
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
            actionCopyWholeVerse->setText(tr("Copy Verse %1").arg(addText));
            actionCopyWholeVerse->setEnabled(true);

            connect(actionCopyWholeVerse, SIGNAL(triggered()), this , SLOT(copyWholeVerse()));
        }
    } else {
        actionCopyWholeVerse->setEnabled(false);
    }

    QAction *dbg = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Debugger"), contextMenu.data());
    connect(dbg, SIGNAL(triggered()), this, SLOT(debugger()));

    contextMenu->addAction(m_actionCopy);
    contextMenu->addAction(actionCopyWholeVerse);
    contextMenu->addAction(m_actionSelect);
    contextMenu->addSeparator();
    contextMenu->addMenu(m_menuMark);
    contextMenu->addAction(m_actionRemoveMark);
    contextMenu->addSeparator();
    contextMenu->addAction(m_actionBookmark);
    contextMenu->addAction(m_actionNote);
    contextMenu->addAction(dbg);
    contextMenu->exec(ev->globalPos());

}
void BibleForm::newNoteWithLink()
{
    if(!m_moduleManager->bibleLoaded()) {
        return;
    }
    m_notesManager->newNoteWithLink(verseSelection());
}
void BibleForm::newBookmark()
{

}

void BibleForm::copyWholeVerse(void)
{
    //DEBUG_FUNC_NAME
    VerseSelection selection = verseSelection();
    if(selection.startVerse != -1) {
        QString sverse;
        if(selection.startVerse == selection.endVerse) {
            sverse = "," + QString::number(selection.startVerse + 1);
        } else {
            sverse = "," + QString::number(selection.startVerse + 1) + "-" + QString::number(selection.endVerse + 1);
        }

        int add = 0;
        if(m_moduleManager->bible()->bibleType() == Module::BibleQuoteModule)
            add = 1; //because of the title
        myDebug() << "startVerse = " << selection.startVerse << " endVerse = " << selection.endVerse;
        Range r;
        r.setBook(selection.bookID);
        r.setChapter(selection.chapterID);
        r.setModule(selection.moduleID);
        r.setStartVerse(selection.startVerse);
        r.setEndVerse(selection.endVerse);
        QString stext = m_moduleManager->bible()->readRange(r).join(" ");
        myDebug() << stext;

        QTextDocument doc2;
        doc2.setHtml(stext);
        stext = doc2.toPlainText();

        const QString curChapter = QString::number(m_moduleManager->bible()->chapterID() + 1);

        const QString newText = m_moduleManager->bible()->bookName(selection.bookID) + " " + curChapter + sverse + "\n" + stext;
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(newText);
    }
}

void BibleForm::debugger()
{
    //DEBUG_FUNC_NAME
    m_view->page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    if(m_inspector == 0) {
        m_inspector = new QWebInspector;
        m_inspector->setPage(m_view->page());
    }
    m_inspector->showNormal();
}

void BibleForm::newColorMark()
{
    if(!m_moduleManager->bibleLoaded()) {
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

    VerseSelection selection = verseSelection();
    m_notesManager->newCustomColorMark(selection, color);
}

void BibleForm::newCustomColorMark()
{
    if(!m_moduleManager->bibleLoaded()) {
        return;
    }
    VerseSelection selection = verseSelection();
    const QColor color = QColorDialog::getColor(QColor(255, 255, 0), this);
    if(color.isValid()) {
        m_notesManager->newCustomColorMark(selection, color);
    }

}

void BibleForm::newBoldMark()
{
    if(!m_moduleManager->bibleLoaded()) {
        return;
    }
    VerseSelection selection = verseSelection();
    m_notesManager->newBoldMark(selection);

}

void BibleForm::newItalicMark()
{
    if(!m_moduleManager->bibleLoaded()) {
        return;
    }

    VerseSelection selection = verseSelection();
    m_notesManager->newItalicMark(selection);

}
void BibleForm::newUnderlineMark()
{
    if(!m_moduleManager->bibleLoaded()) {
        return;
    }

    VerseSelection selection = verseSelection();
    m_notesManager->newUnderlineMark(selection);
}

void BibleForm::removeMark()
{
    //DEBUG_FUNC_NAME;
    if(!m_moduleManager->bibleLoaded()) {
        return;
    }
    VerseSelection selection = verseSelection();
    //myDebug() << "selection = " << selection.moduleID << selection.bookID << selection.chapterID << selection.startVerse;
    m_notesManager->removeMark(selection);
}



VerseSelection BibleForm::verseSelection()
{
    QWebFrame *f = m_view->page()->mainFrame();
    VerseSelection s;
    if(!f)
        return s;
    f->evaluateJavaScript("var verseSelection = new VerseSelection();verseSelection.getSelection();");
    s.startVerse = f->evaluateJavaScript("verseSelection.startVerse;").toInt();
    s.endVerse = f->evaluateJavaScript("verseSelection.endVerse;").toInt();
    s.moduleID = f->evaluateJavaScript("verseSelection.moduleID;").toInt();
    s.bookID  = f->evaluateJavaScript("verseSelection.bookID;").toInt();
    s.chapterID = f->evaluateJavaScript("verseSelection.chapterID;").toInt();

    const QString startVerseText = m_lastTextRanges.getVerse(s.bookID, s.chapterID, s.startVerse).data();
    QString endVerseText;
    if(s.startVerse != s.endVerse)
        endVerseText = m_lastTextRanges.getVerse(s.bookID, s.chapterID, s.endVerse).data();
    else
        endVerseText = startVerseText;

    const QString selectedText = f->evaluateJavaScript("verseSelection.selectedText;").toString();

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
    //todo: 0.6
    /* if(s.canBeUsedForMarks() == false) {
         //now the ultimative alogrithm
         f->evaluateJavaScript("var adVerseSelection = new AdVerseSelection();adVerseSelection.getSelection();");
         const QString startVerseText2 = f->evaluateJavaScript("adVerseSelection.startVerseText;").toString();

         const QString uniqueString = "!-_OPENBIBLEVIEWER_INSERT_-!";
         int posOfInsert = startVerseText2.lastIndexOf(uniqueString);

         QString back = selectedText;
         QString longestString;
         for(int i = selectedText.size()-1; i > 0; i--) {
             const int pos = startVerseText2.indexOf(back);
             if(pos != -1) {
                 longestString = back;
                 break;
             }
             back.remove(i,selectedText.size());
         }

         int count = 0;
         int currentPos = 0;
         while(true) {
             currentPos = startVerseText2.indexOf(longestString,currentPos+1);
             if(currentPos > posOfInsert || currentPos == -1) {
                 break;
             }
             count++;
         }
         s.type = VerseSelection::RepeatOfLongestString;
         s.repeat = count;
         s.longestString = longestString;
         //s.setCanBeUsedForMarks(true);
         //todo: end
         //myDebug() << longestString << " count = " << count;

     }*/
    return s;
}
BibleForm::~BibleForm()
{
    delete m_ui;
    delete m_bibleList;
}


void BibleForm::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        deleteDefaultMenu();
        createDefaultMenu();
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
