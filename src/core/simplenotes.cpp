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
#include "simplenotes.h"


SimpleNotes::SimpleNotes()
{
}

void SimpleNotes::setDataWidget(QTextBrowser *data)
{
    m_textEdit_note = data;
    loadTextBrowser = true;
}
void SimpleNotes::setFrameWidget(QWebFrame *frame)
{
    m_frame = frame;
    loadTextBrowser = false;
}
void SimpleNotes::setTitleWidget(QLineEdit *title)
{
    m_lineEdit_title = title;
    connect(title, SIGNAL(editingFinished()), this, SLOT(updateTitle()));
}
void SimpleNotes::setViewWidget(QTreeView *treeView)
{
    m_treeView = treeView;
}
void SimpleNotes::setLinkWidget(QLabel *link)
{
    m_label_link = link;
}
void SimpleNotes::setLinkButtonWidget(QPushButton *button)
{
    m_pushButton_link = button;
}
void SimpleNotes::create(const QString &id, QStandardItem *parentItem)
{
    //DEBUG_FUNC_NAME
    foreach(const QString & i, m_idC) {
        if(id == "-1") {
            parentItem = m_itemModel->invisibleRootItem();
        }
        if((id == "-1" && m_notes->getRef(i, "parent").isEmpty()) || m_notes->getRef(i, "parent") == id) {
            if(m_notes->getType(i) == "text") {
                QStandardItem *noteItem = new QStandardItem;
                noteItem->setText(m_notes->getTitle(i));
                noteItem->setData(i);
                parentItem->appendRow(noteItem);
                create(i, noteItem);
            } else if(m_notes->getType(i) == "folder") {
                QStandardItem *folderItem = new QStandardItem;
                folderItem->setIcon(folderIcon);
                folderItem->setText(m_notes->getTitle(i));
                folderItem->setData(i);
                parentItem->appendRow(folderItem);
                create(i, folderItem);
            }
        }

    }
}

void SimpleNotes::init()
{
    m_itemModel = new QStandardItemModel(m_treeView);
    connect(m_treeView, SIGNAL(activated(QModelIndex)), this, SLOT(showNote(QModelIndex)));
    connect(m_treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(notesContextMenu(QPoint)));
    if(loadTextBrowser) {
        connect(m_textEdit_note, SIGNAL(undoAvailable(bool)), this, SLOT(fastSave()));
        connect(m_textEdit_note, SIGNAL(redoAvailable(bool)), this, SLOT(fastSave()));
    }


    connect(m_pushButton_link, SIGNAL(clicked()), this, SLOT(editNoteLink()));
    connect(m_label_link, SIGNAL(linkActivated(QString)), m_actions, SLOT(get(QString)));

    if(!m_notes->isLoaded()) {
        m_notes->init(m_settings->homePath + "notes.xml");
        m_notes->loadNotes();
        m_notes->readNotes();
    }

    m_notes->loadingNewInstance();
    connect(m_notes, SIGNAL(saveAll()), this, SLOT(saveNote()));
    connect(m_notes, SIGNAL(titleChanged(QString, QString)), this, SLOT(changeTitle(QString, QString)));
    connect(m_notes, SIGNAL(dataChanged(QString, QString)), this, SLOT(changeData(QString, QString)));
    connect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));
    connect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    connect(m_notes, SIGNAL(noteRemoved(QString, QMap<QString, QString>)), this, SLOT(removeNote(QString)));

    m_itemModel->clear();

    QStyle *style = QApplication::style();

    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
    m_idC = m_notes->getIDList();
    create("-1", 0);

    m_noteID = "";

    m_proxyModel = new RecursivProxyModel(this);
    m_proxyModel->setSourceModel(m_itemModel);
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Note Title"));
    m_selectionModel = new QItemSelectionModel(m_proxyModel);

    m_treeView->setSortingEnabled(true);
    m_treeView->setModel(m_proxyModel);
    m_treeView->setSelectionModel(m_selectionModel);
}
void SimpleNotes::showNote(const QModelIndex &index)
{
    showNote(index.data(Qt::UserRole + 1).toString());
}
void SimpleNotes::showNote(const QString &noteID, bool selectNote)
{
    //save current notes
    if(selectNote)
        select(noteID);
    fastSave();
    aktNote();
    //load new notes
    m_noteID = noteID;
    m_noteRef = m_notes->getRef(m_noteID);

    setTitle(m_notes->getTitle(m_noteID));
    setData(m_notes->getData(m_noteID));
    setRef(m_noteRef);
    emit contentChanged();
}
void SimpleNotes::setTitle(QString title)
{
    m_lineEdit_title->setText(title);
}
void SimpleNotes::setData(QString data)
{
    if(loadTextBrowser)
        m_textEdit_note->setHtml(data);
    else
        m_frame->setHtml(data);
}
void SimpleNotes::setRef(QMap<QString, QString> ref)
{
    if(!ref["link"].isEmpty()) {
        m_label_link->setText(m_moduleManager->notePos2Link(ref["link"]));
        m_pushButton_link->setEnabled(true);
    } else {
        m_label_link->setText("");
        m_pushButton_link->setEnabled(false);
    }
}
void SimpleNotes::editNoteLink()
{
    const QString link = m_noteRef.value("link");
    UrlConverter2 urlConverter(UrlConverter::InterfaceUrl, UrlConverter::PersistentUrl, link);
    urlConverter.setSM(m_settings, m_moduleManager->m_moduleMap);
    urlConverter.convert();
    if(urlConverter.url().isValid()) {
        BiblePassageDialog *passageDialog = new BiblePassageDialog();
        connect(passageDialog, SIGNAL(updated(QString)), this, SLOT(updateNote(QString)));
        passageDialog->setSettings(m_settings);
        passageDialog->setModuleManager(m_moduleManager);
        passageDialog->setCurrent(urlConverter.url().ranges().first().bibleID(),
                                  urlConverter.url().ranges().first().bibleUID(),
                                  urlConverter.url().ranges().first().bookID(),
                                  urlConverter.url().ranges().first().chapterID() + 1,
                                  urlConverter.url().ranges().first().startVerseID() + 1);
        passageDialog->exec();
    }

}
void SimpleNotes::updateNote(const QString &link)
{
    m_noteRef["link"] = link;
    m_notes->setRef(m_noteID, m_noteRef);
    setRef(m_noteRef);
    return;
}
void SimpleNotes::changeData(const QString &id, const QString &data)
{
    if(m_noteID == id) {
        setData(data);
    }
}
void SimpleNotes::changeTitle(const QString &id, const QString &title)
{
    //DEBUG_FUNC_NAME
    if(m_noteID == id) {
        setTitle(title);
    }
    const QModelIndexList list = m_proxyModel->match(m_itemModel->invisibleRootItem()->index(), Qt::UserRole + 1, id);
    if(list.size() != 1) {
        myWarning() << "invalid noteID = " << id;
        return;
    }
    m_itemModel->setData(list.at(0), title, Qt::DisplayRole);
}
void SimpleNotes::updateTitle()
{
    //DEBUG_FUNC_NAME
    disconnect(m_notes, SIGNAL(titleChanged(QString, QString)), this, SLOT(changeTitle(QString, QString)));
    m_notes->setTitle(m_noteID, m_lineEdit_title->text());
    const QModelIndexList list = m_proxyModel->match(m_itemModel->invisibleRootItem()->index(), Qt::UserRole + 1, m_noteID, -1);
    if(list.size() != 1) {
        myWarning() << "invalid noteID = " << m_noteID;
        return;
    }
    m_itemModel->setData(list.at(0), m_lineEdit_title->text(), Qt::DisplayRole);
    connect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));
}

void SimpleNotes::changeRef(const QString &id, const QMap<QString, QString> &ref)
{
    if(m_noteID == id) {
        setRef(ref);
    }
}

void SimpleNotes::copyNote(void)
{
    const QModelIndexList list = m_selectionModel->selectedRows(0);
    QClipboard *clipboard = QApplication::clipboard();
    QString text;
    for(int i = 0; i < list.size(); i++) {
        QTextDocument doc;
        doc.setHtml(m_notes->getData(list.at(i).data(Qt::UserRole + 1).toString()));
        if(i != 0)
            text += "\n";
        text += doc.toPlainText();
    }
    if(!text.isEmpty()) {
        clipboard->setText(text);
    }

}
void SimpleNotes::iterate(QStandardItem *item = 0)
{
    //DEBUG_FUNC_NAME
    const QString parentID = item->data().toString();
    for(int i = 0; i < item->rowCount(); ++i) {
        QStandardItem *m = item->child(i);
        m_notes->setRef(m->data().toString(), "parent", parentID);
        if(m->hasChildren())
            iterate(m);
    }
}

void SimpleNotes::saveNote(void)
{
    //DEBUG_FUNC_NAME
    aktNote();
    fastSave();

    //save structure
    disconnect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));
    iterate(m_itemModel->invisibleRootItem());
    connect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));

    m_notes->saveNotes();
}
void SimpleNotes::fastSave(void)
{
    //DEBUG_FUNC_NAME
    disconnect(m_notes, SIGNAL(titleChanged(QString, QString)), this, SLOT(changeTitle(QString, QString)));
    disconnect(m_notes, SIGNAL(dataChanged(QString, QString)), this, SLOT(changeData(QString, QString)));
    disconnect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));

    if(loadTextBrowser)
        m_notes->setData(m_noteID, m_textEdit_note->toHtml());
    else
        m_notes->setData(m_noteID, m_frame->toHtml());

    m_notes->setTitle(m_noteID, m_lineEdit_title->text());
    m_notes->setRef(m_noteID, m_noteRef);


    connect(m_notes, SIGNAL(titleChanged(QString, QString)), this, SLOT(changeTitle(QString, QString)));
    connect(m_notes, SIGNAL(dataChanged(QString, QString)), this, SLOT(changeData(QString, QString)));
    connect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));
}
void SimpleNotes::aktNote()
{
    //DEBUG_FUNC_NAME

    if(m_noteID.isEmpty())
        return;

    m_notes->setTitle(m_noteID, m_lineEdit_title->text());
    const QModelIndexList list = m_proxyModel->match(m_itemModel->invisibleRootItem()->index(), Qt::UserRole + 1, m_noteID, -1);
    if(list.size() != 1) {
        myWarning() << "invalid noteID = " << m_noteID << " size = " << list.size();
        return;
    }
    const QModelIndex index = list.at(0);

    if(index.data(Qt::DisplayRole) != m_notes->getTitle(m_noteID)) {
        m_itemModel->setData(index, m_notes->getTitle(m_noteID), Qt::DisplayRole);
    }
}
void SimpleNotes::select(const QString &noteID)
{
    //DEBUG_FUNC_NAME
    const QModelIndexList list = m_proxyModel->match(m_itemModel->invisibleRootItem()->index(), Qt::UserRole + 1, noteID);
    if(list.size() != 1) {
        return;
    }
    const QModelIndex index = m_proxyModel->mapFromSource(list.at(0));
    m_selectionModel->clearSelection();
    m_selectionModel->setCurrentIndex(index, QItemSelectionModel::Select);
}

void SimpleNotes::newTextNote(void)
{
    //DEBUG_FUNC_NAME
    disconnect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));

    QStandardItem *parentItem = 0;
    //myDebug() << sender()->objectName();
    if(sender()->objectName() == "actionNew") {
        //myDebug() << m_point <<  m_treeView->indexAt(m_point).data();
        parentItem = m_itemModel->itemFromIndex(m_proxyModel->mapToSource(m_treeView->indexAt(m_point)));
    }
    if(parentItem == 0)
        parentItem = m_itemModel->invisibleRootItem();

    aktNote();
    fastSave();
    m_selectionModel->clearSelection();

    const QString newID = m_notes->generateNewID();
    m_notes->setData(newID, "");
    m_notes->setTitle(newID, tr("(unnamed)"));
    m_notes->setType(newID, "text");

    m_noteRef = QMap<QString, QString>();
    const QDateTime t = QDateTime::currentDateTime();

    m_noteRef["created"] = t.toString(Qt::ISODate);
    m_noteRef["parent"] = parentItem->data().toString();
    m_notes->setRef(newID, m_noteRef);
    m_noteID = newID;
    m_notes->insertID(newID);


    QStandardItem *newItem = new QStandardItem;
    newItem->setText(m_notes->getTitle(m_noteID));
    newItem->setData(m_noteID);
    parentItem->appendRow(newItem);

    select(m_noteID);

    connect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));

    setTitle(tr("(unnamed)"));
    setData("");
    setRef(m_noteRef);
}
void SimpleNotes::newFolder()
{
    //DEBUG_FUNC_NAME
    //todo: it is not syncing folders
    disconnect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    QStandardItem *parentItem = 0;
    if(sender()->objectName() == "actionNewFolder")
        parentItem = m_itemModel->itemFromIndex(m_proxyModel->mapToSource(m_treeView->indexAt(m_point)));
    if(parentItem == 0)
        parentItem = m_itemModel->invisibleRootItem();

    aktNote();
    fastSave();
    m_selectionModel->clearSelection();

    const QString newID = m_notes->generateNewID();
    m_notes->setData(newID, "");
    m_notes->setTitle(newID, tr("Folder")); // todo: count folders and generate an id
    m_notes->setType(newID, "folder");

    m_noteRef = QMap<QString, QString>();
    const QDateTime t = QDateTime::currentDateTime();

    m_noteRef["created"] = t.toString(Qt::ISODate);
    m_noteRef["parent"] = parentItem->data().toString();
    m_notes->setRef(newID, m_noteRef);
    m_noteID = newID;
    m_notes->insertID(newID);

    QStandardItem *newItem = new QStandardItem;
    QStyle *style = QApplication::style();
    QIcon folderIcon;
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
    newItem->setIcon(folderIcon);
    newItem->setText(tr("Folder"));
    newItem->setData(newID);

    parentItem->appendRow(newItem);
    select(m_noteID);

    connect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));

    setTitle(tr("Folder"));
    setData("");
    setRef(m_noteRef);
}

void SimpleNotes::addNote(const QString &id)
{
    //DEBUG_FUNC_NAME
    if(m_noteID != id && m_notes->getType(id) == "text") {
        //todo: if there is already this note(but something like that should never happen)
        QStandardItem *parentItem = m_itemModel->invisibleRootItem();
        QStandardItem *newItem = new QStandardItem;
        newItem->setText(m_notes->getTitle(id));
        newItem->setData(id);
        parentItem->appendRow(newItem);
    }
}

void SimpleNotes::newTextNoteWithLink(VerseSelection selection)
{
    //DEBUG_FUNC_NAME
    disconnect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    aktNote();
    fastSave();

    VerseUrlRange range;
    range.setModule(selection.moduleID);
    range.setBook(selection.bookID);
    range.setChapter(selection.chapterID);
    range.setStartVerse(selection.startVerse);
    VerseUrl url(range);
    UrlConverter2 urlConverter(UrlConverter::InterfaceUrl, UrlConverter::PersistentUrl, url);
    urlConverter.setSM(m_settings, m_moduleManager->m_moduleMap);
    urlConverter.setV11n(m_moduleManager->verseModule()->versification());
    urlConverter.convert();
    const QString link = urlConverter.url().toString();
    myDebug() << "link = " << link;
    const QString newID = m_notes->generateNewID();
    m_notes->setData(newID, "");
    m_notes->setTitle(newID, tr("(unnamed)"));
    m_notes->setType(newID, "text");

    QMap<QString, QString> ref;
    QDateTime t = QDateTime::currentDateTime();
    ref["created"] = t.toString(Qt::ISODate);
    ref["link"] = link;
    m_noteRef = ref;
    m_notes->setRef(newID, m_noteRef);

    m_notes->insertID(newID);
    m_noteID = newID;

    QStandardItem *parentItem = m_itemModel->invisibleRootItem();
    QStandardItem *newItem = new QStandardItem;
    newItem->setText(m_notes->getTitle(m_noteID));
    newItem->setData(m_noteID);
    parentItem->appendRow(newItem);

    select(m_noteID);
    connect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    setTitle(tr("(unnamed)"));
    setData("");
    setRef(m_noteRef);

    m_actions->reloadChapter();
}
void SimpleNotes::newStyleMark(VerseSelection selection, const QString &style)
{
    //myDebug() << selection.shortestStringInEndVerse << selection.shortestStringInStartVerse;
    if(selection.shortestStringInEndVerse == "" || selection.shortestStringInStartVerse == "") {
        myWarning() << "cannot create mark";
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot create mark."));
        //todo: use full verse mark
        return;
    }
    disconnect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    aktNote();
    fastSave();

    VerseUrlRange range;
    range.setModule(selection.moduleID);
    range.setBook(selection.bookID);
    range.setChapter(selection.chapterID);
    range.setStartVerse(selection.startVerse);
    VerseUrl url(range);
    UrlConverter2 urlConverter(UrlConverter::InterfaceUrl, UrlConverter::PersistentUrl, url);
    urlConverter.setSM(m_settings, m_moduleManager->m_moduleMap);
    urlConverter.setV11n(m_moduleManager->verseModule()->versification());
    urlConverter.convert();
    const QString link = urlConverter.url().toString();
    myDebug() << link;

    //reloadNotes();
    const QString newID = m_notes->generateNewID();
    m_notes->setData(newID, "");
    m_notes->setTitle(newID, tr("(unnamed)"));
    m_notes->setType(newID, "mark");

    QMap<QString, QString> ref;
    ref["link"] = link;
    ref["start"] = QString::number(selection.startVerse);
    ref["end"] = QString::number(selection.endVerse);
    ref["selection_pos_type"] = selection.typeToString();

    if(selection.type == VerseSelection::RepeatOfLongestString) {
        ref["repeat"] = QString::number(selection.repeat);
        ref["longeststring"] = selection.longestString;
    } else if(selection.type == VerseSelection::ShortestString) {
        ref["startString"] = selection.shortestStringInStartVerse;
        ref["endString"] = selection.shortestStringInEndVerse;
    }

    ref["style"] = style;
    m_notes->setRef(newID, ref);
    m_notes->insertID(newID);
    connect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    m_actions->reloadChapter();
}
void SimpleNotes::notesContextMenu(QPoint point)
{
    //DEBUG_FUNC_NAME
    m_point = point;
    QMenu *contextMenu = new QMenu(m_treeView);
    m_currentPoint = point;
    contextMenu->setObjectName("contextMenu");

    QAction *actionCopy = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Copy"), contextMenu);
    actionCopy->setObjectName("actionCopy");
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copyNote()));

    QAction *actionNew = new QAction(QIcon::fromTheme("document-new", QIcon(":/icons/16x16/document-new.png")), tr("New"), contextMenu);
    actionNew->setObjectName("actionNew");
    connect(actionNew, SIGNAL(triggered()), this, SLOT(newTextNote()));

    QAction *actionNewFolder = new QAction(QIcon::fromTheme("document-new", QIcon(":/icons/16x16/document-new.png")), tr("New Folder"), contextMenu);
    actionNewFolder->setObjectName("actionNewFolder");
    connect(actionNewFolder, SIGNAL(triggered()), this, SLOT(newFolder()));

    QAction *actionDelete = new QAction(QIcon::fromTheme("edit-delete", QIcon(":/icons/16x16/edit-delete.png")), tr("Delete"), contextMenu);
    actionDelete->setObjectName("actionDelete");
    connect(actionDelete, SIGNAL(triggered()), this, SLOT(removeNote()));

    contextMenu->addAction(actionNew);
    contextMenu->addAction(actionDelete);
    contextMenu->addSeparator();
    contextMenu->addAction(actionNewFolder);
    contextMenu->addAction(actionCopy);

    contextMenu->exec(QCursor::pos());
}
void SimpleNotes::removeNote()
{
    //DEBUG_FUNC_NAME
    QModelIndexList list = m_selectionModel->selectedRows();
    disconnect(m_notes, SIGNAL(noteRemoved(QString)), this, SLOT(removeNote(QString)));
    //todo: if note has link, check if the page where the link shows is currently displayed, if yes reloadChapter
    if(list.isEmpty()) {
        const QModelIndex index = m_proxyModel->mapToSource(m_treeView->indexAt(m_currentPoint));
        if(index.isValid()) {
            const QString id = index.data(Qt::UserRole + 1).toString();
            if(id == m_noteID) {
                setTitle("");
                setData("");
                setRef(QMap<QString, QString>());
            }
            m_notes->removeNote(id);
            m_itemModel->removeRow(index.row(), index.parent());
        }

    } else {
        while(list.size() != 0) {
            const QModelIndex index = m_proxyModel->mapToSource(list.at(0));
            const QString id = index.data(Qt::UserRole + 1).toString();
            if(id == m_noteID) {
                setTitle("");
                setData("");
                setRef(QMap<QString, QString>());
            }
            m_notes->removeNote(id);
            m_itemModel->removeRow(index.row(), index.parent());
            list = m_selectionModel->selectedRows(0);
        }
    }

    connect(m_notes, SIGNAL(noteRemoved(QString)), this, SLOT(removeNote(QString)));
}
void SimpleNotes::removeNote(const QString &id)
{
    //DEBUG_FUNC_NAME
    if(id == m_noteID) {
        setTitle("");
        setData("");
        setRef(QMap<QString, QString>());
    }
    const QModelIndexList list = m_proxyModel->match(m_itemModel->invisibleRootItem()->index(), Qt::UserRole + 1, id);
    if(list.size() != 1) {
        myWarning() << "invalid noteID = " << m_noteID;
        return;
    }
    const QModelIndex index = list.at(0);
    m_itemModel->removeRow(index.row(), index.parent());
}
