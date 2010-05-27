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
#include "simplenotes.h"
#include <QSortFilterProxyModel>
#include "src/core/dbghelper.h"
#include <QtGui/QClipboard>
#include <QtGui/QMenu>
#include <QtGui/QColorDialog>
#include <QApplication>
#include "src/ui/dialog/biblepassagedialog.h"
#include "src/core/urlconverter.h"
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

void SimpleNotes::init()
{
    m_itemModel = new QStandardItemModel(m_treeView);
    connect(m_treeView, SIGNAL(activated(QModelIndex)), this, SLOT(showNote(QModelIndex)));
    connect(m_treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(notesContextMenu(QPoint)));
    if (loadTextBrowser) {
        connect(m_textEdit_note, SIGNAL(undoAvailable(bool)), this, SLOT(fastSave()));
        connect(m_textEdit_note, SIGNAL(redoAvailable(bool)), this, SLOT(fastSave()));
    }


    connect(m_pushButton_link, SIGNAL(clicked()), this, SLOT(editNoteLink()));
    connect(m_label_link, SIGNAL(linkActivated(QString)), m_bibleDisplay, SIGNAL(get(QString)));

    if (!m_notes->isLoaded()) {
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
    QStringList id = m_notes->getIDList();

    QStandardItem *parentItem = m_itemModel->invisibleRootItem();
    myDebug() << " id = " << id;
    for (int i = 0; i < id.size(); i++) {
        if (m_notes->getType(id.at(i)) == "text") {
            QStandardItem *noteItem = new QStandardItem;
            noteItem->setText(m_notes->getTitle(id.at(i)));
            noteItem->setData(id.at(i));
            parentItem->appendRow(noteItem);
        }
    }
    m_noteID = "";

    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_itemModel);
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Note Title"));
    m_selectionModel = new QItemSelectionModel(m_proxyModel);

    m_treeView->setSortingEnabled(true);
    m_treeView->setModel(m_proxyModel);
    m_treeView->setSelectionModel(m_selectionModel);
}
void SimpleNotes::showNote(QModelIndex index)
{
    showNote(index.data(Qt::UserRole + 1).toString());
}
void SimpleNotes::showNote(const QString &noteID, bool selectNote)
{
    //DEBUG_FUNC_NAME
    //save current notes
    if (selectNote)
        select(noteID);

    fastSave();
    aktNote();
    //load new notes
    m_noteID = noteID;
    currentNoteRef = m_notes->getRef(m_noteID);

    setTitle(m_notes->getTitle(m_noteID));
    setData(m_notes->getData(m_noteID));
    setRef(currentNoteRef);
}
void SimpleNotes::setTitle(QString title)
{
    m_lineEdit_title->setText(title);
}
void SimpleNotes::setData(QString data)
{
    if (loadTextBrowser)
        m_textEdit_note->setHtml(data);
    else
        m_frame->setHtml(data);
}
void SimpleNotes::setRef(QMap<QString, QString> ref)
{
    if (!ref["link"].isEmpty()) {
        m_label_link->setText(m_moduleManager->notePos2Link(ref["link"]));
        m_pushButton_link->setEnabled(true);
    } else {
        m_label_link->setText("");
        m_pushButton_link->setEnabled(false);
    }
}
void SimpleNotes::editNoteLink()
{
    DEBUG_FUNC_NAME
    QString link = currentNoteRef["link"];

    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::None, link);
    urlConverter.m_biblesRootPath = m_moduleManager->bible()->biblesRootPath();
    urlConverter.pharse();

    BiblePassageDialog *passageDialog = new  BiblePassageDialog();
    connect(passageDialog, SIGNAL(updated(QString)), this, SLOT(updateNote(QString)));
    passageDialog->setSettings(m_settings);
    passageDialog->setCurrent(urlConverter.m_bibleID.toInt(), urlConverter.m_path, urlConverter.m_bookID, urlConverter.m_chapterID + 1, urlConverter.m_verseID + 1);
    passageDialog->show();
    passageDialog->exec();

}
void SimpleNotes::updateNote(QString link)
{
    DEBUG_FUNC_NAME
    currentNoteRef["link"] = link;
    m_notes->setRef(m_noteID, currentNoteRef);
    setRef(currentNoteRef);
    return;
}
void SimpleNotes::changeData(QString id, QString data)
{
    DEBUG_FUNC_NAME
    if (m_noteID == id) {
        setData(data);
    }
}
void SimpleNotes::changeTitle(QString id, QString title)
{
    DEBUG_FUNC_NAME
    if (m_noteID == id) {
        setTitle(title);
    }
    QModelIndexList list = m_treeView->model()->match(m_treeView->model()->index(0, 0), Qt::UserRole + 1, id);
    if (list.size() != 1) {
        myDebug() << "invalid noteID = " << m_noteID;
        return;
    }
    QModelIndex index = list.at(0);
    m_treeView->model()->setData(index, title, Qt::DisplayRole);
}
void SimpleNotes::changeRef(QString id, QMap<QString, QString> ref)
{
    DEBUG_FUNC_NAME
    if (m_noteID == id) {
        setRef(ref);
    }
}

void SimpleNotes::copyNote(void)
{
    QModelIndexList list = m_selectionModel->selectedRows(0);
    QClipboard *clipboard = QApplication::clipboard();
    QString text;
    for (int i = 0; i < list.size(); i++) {
        QTextDocument doc;
        doc.setHtml(m_notes->getData(list.at(i).data(Qt::UserRole + 1).toString()));
        if (i != 0)
            text += "\n";
        text += doc.toPlainText();
    }
    if (!text.isEmpty()) {
        clipboard->setText(text);
    }
}
void SimpleNotes::saveNote(void)
{
    //DEBUG_FUNC_NAME
    aktNote();
    fastSave();
    m_notes->saveNotes();
}
void SimpleNotes::fastSave(void)
{
    disconnect(m_notes, SIGNAL(titleChanged(QString, QString)), this, SLOT(changeTitle(QString, QString)));
    disconnect(m_notes, SIGNAL(dataChanged(QString, QString)), this, SLOT(changeData(QString, QString)));
    disconnect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));

    if (loadTextBrowser)
        m_notes->setData(m_noteID, m_textEdit_note->toHtml());
    else
        m_notes->setData(m_noteID, m_frame->toHtml());

    m_notes->setTitle(m_noteID, m_lineEdit_title->text());
    m_notes->setRef(m_noteID, currentNoteRef);

    connect(m_notes, SIGNAL(titleChanged(QString, QString)), this, SLOT(changeTitle(QString, QString)));
    connect(m_notes, SIGNAL(dataChanged(QString, QString)), this, SLOT(changeData(QString, QString)));
    connect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));
}
void SimpleNotes::aktNote()
{
    if (m_noteID == "")
        return;
    m_notes->setTitle(m_noteID, m_lineEdit_title->text());
    QModelIndexList list = m_treeView->model()->match(m_treeView->model()->index(0, 0), Qt::UserRole + 1, m_noteID);
    if (list.size() != 1) {
        myDebug() << "invalid noteID = " << m_noteID;
        return;
    }
    QModelIndex index = list.at(0);
    if (index.data(Qt::DisplayRole) != m_notes->getTitle(m_noteID)) {
        m_treeView->model()->setData(index, m_notes->getTitle(m_noteID), Qt::DisplayRole);


    }
}
void SimpleNotes::select(QString noteID)
{
    QModelIndexList list = m_treeView->model()->match(m_treeView->model()->index(0, 0), Qt::UserRole + 1, noteID);
    if (list.size() != 1) {
        return;
    }
    QModelIndex index = list.at(0);
    m_selectionModel->clearSelection();
    m_selectionModel->setCurrentIndex(index, QItemSelectionModel::Select);
}

void SimpleNotes::newNote(void)
{
    disconnect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    aktNote();
    fastSave();
    m_selectionModel->clearSelection();

    QString newID = m_notes->generateNewID();
    m_notes->setData(newID, "");
    m_notes->setTitle(newID, tr("(unnamed)"));
    m_notes->setType(newID, "text");

    currentNoteRef = QMap<QString, QString>();
    m_notes->setRef(newID, currentNoteRef);
    m_noteID = newID;
    m_notes->insertID(newID);


    QStandardItem *parentItem = m_itemModel->invisibleRootItem();
    QStandardItem *newItem = new QStandardItem;
    newItem->setText(m_notes->getTitle(m_noteID));
    newItem->setData(m_noteID);
    parentItem->appendRow(newItem);

    select(m_noteID);

    connect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));

    setTitle(tr("(unnamed)"));
    setData("");
    setRef(currentNoteRef);


}
void SimpleNotes::addNote(QString id)
{
    if (m_noteID != id && m_notes->getType(id) == "text") {
        //todo: if there is already this note(but something like that should never happen)
        QStandardItem *parentItem = m_itemModel->invisibleRootItem();
        QStandardItem *newItem = new QStandardItem;
        newItem->setText(m_notes->getTitle(id));
        newItem->setData(id);
        parentItem->appendRow(newItem);
    }
}

void SimpleNotes::newNoteWithLink(VerseSelection selection)
{
    //DEBUG_FUNC_NAME
    disconnect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    aktNote();
    fastSave();

    QString link;
    UrlConverter urlConverter(UrlConverter::None, UrlConverter::PersistentUrl, "");
    urlConverter.m_biblesRootPath = m_moduleManager->bible()->biblesRootPath();
    urlConverter.m_bibleID =  QString::number(m_moduleManager->bible()->bibleID());
    urlConverter.m_bookID = m_moduleManager->bible()->bookID();
    urlConverter.m_chapterID = m_moduleManager->bible()->chapterID() - m_moduleManager->bible()->chapterAdd();
    urlConverter.m_verseID = selection.startVerse - 1;
    urlConverter.m_bookName = m_moduleManager->bible()->bookFullName().at(m_moduleManager->bible()->bookID());
    link = urlConverter.convert();

    QString newID = m_notes->generateNewID();
    m_notes->setData(newID, "");
    m_notes->setTitle(newID, tr("(unnamed)"));
    m_notes->setType(newID, "text");

    QMap<QString, QString> ref;
    ref["link"] = link;
    currentNoteRef = ref;

    m_notes->setRef(newID, currentNoteRef);

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
    setRef(currentNoteRef);

    emit reloadChapter();
}
void SimpleNotes::notesContextMenu(QPoint point)
{
    QMenu *contextMenu = new QMenu(m_treeView);
    currentPoint = point;
    contextMenu->setObjectName("contextMenu");

    QAction *actionCopy = new QAction(this);
    actionCopy->setObjectName("actionCopy");
    actionCopy->setText(tr("Copy"));
    QIcon iconCopy;
    iconCopy.addPixmap(QPixmap(":/icons/16x16/edit-copy.png"), QIcon::Normal, QIcon::Off);
    actionCopy->setIcon(iconCopy);
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copyNote()));

    QAction *actionNew = new QAction(this);
    actionNew->setObjectName(QString::fromUtf8("actionNew"));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(newNote()));
    QIcon iconNew;
    iconNew.addPixmap(QPixmap(":/icons/16x16/document-new.png"), QIcon::Normal, QIcon::Off);
    actionNew->setIcon(iconNew);
    actionNew->setText(tr("New"));

    QAction *actionRemove = new QAction(this);
    actionRemove->setObjectName("actionRemove");
    actionRemove->setText(tr("Delete"));
    QIcon iconRemove;
    iconRemove.addPixmap(QPixmap(":/icons/16x16/edit-delete.png"), QIcon::Normal, QIcon::Off);
    actionRemove->setIcon(iconRemove);
    connect(actionRemove, SIGNAL(triggered()), this, SLOT(removeNote()));

    contextMenu->addAction(actionCopy);
    contextMenu->addAction(actionNew);
    contextMenu->addAction(actionRemove);
    contextMenu->exec(QCursor::pos());
}
void SimpleNotes::removeNote()
{
    DEBUG_FUNC_NAME
    QModelIndexList list = m_selectionModel->selectedRows(0);
    qDebug() << "list.size() = " << list.size() << list;
    disconnect(m_notes, SIGNAL(noteRemoved(QString)), this, SLOT(removeNote(QString)));
    //todo: if note has link, check if the page where the link shows is currently displayed, if yes reloadChapter
    if (list.size() == 0) {
        QModelIndex index = m_treeView->indexAt(currentPoint);
        if (index.isValid()) {
            QString id = index.data(Qt::UserRole + 1).toString();
            if (id == m_noteID) {
                setTitle("");
                setData("");
                setRef(QMap<QString, QString>());
            }
            m_notes->removeNote(id);
            m_treeView->model()->removeRow(index.row());
        }

    } else {
        while (list.size() != 0) {
            QString id = list.at(0).data(Qt::UserRole + 1).toString();
            if (id == m_noteID) {
                setTitle("");
                setData("");
                setRef(QMap<QString, QString>());
            }
            m_notes->removeNote(id);
            m_treeView->model()->removeRow(list.at(0).row());
            list = m_selectionModel->selectedRows(0);
        }
    }

    connect(m_notes, SIGNAL(noteRemoved(QString)), this, SLOT(removeNote(QString)));
}
void SimpleNotes::removeNote(QString id)
{
    DEBUG_FUNC_NAME
    if (id == m_noteID) {
        setTitle("");
        setData("");
        setRef(QMap<QString, QString>());
    }
    QModelIndexList list = m_treeView->model()->match(m_treeView->model()->index(0, 0), Qt::UserRole + 1, id);
    if (list.size() != 1) {
        myDebug() << "invalid noteID = " << m_noteID;
        return;
    }
    QModelIndex index = list.at(0);
    m_treeView->model()->removeRow(index.row());
}
