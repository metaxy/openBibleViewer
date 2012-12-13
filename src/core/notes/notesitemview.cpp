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
#include "notesitemview.h"
#include <QtCore/QPointer>
#include "src/core/dbghelper.h"
#include <QtGui/QMenu>
#include <QtGui/QApplication>
NotesItemView::NotesItemView(Notes *notes, QTreeView *treeView) :  m_treeView(treeView), m_notes(notes)
{
    m_itemModel = new QStandardItemModel(m_treeView);
    m_itemModel->setHeaderData(0, Qt::Horizontal, tr("Note Title"));
    connect(m_treeView, SIGNAL(activated(QModelIndex)), this, SLOT(showNote(QModelIndex)));
    connect(m_treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(notesContextMenu(QPoint)));

    m_proxyModel = new RecursivProxyModel(this);
    m_proxyModel->setSourceModel(m_itemModel);
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Note Title"));

    m_selectionModel = new QItemSelectionModel(m_proxyModel);

    m_treeView->setSortingEnabled(true);
    m_treeView->setModel(m_proxyModel);
    m_treeView->setSelectionModel(m_selectionModel);

    m_itemModel->clear();

    QStyle *style = QApplication::style();

    m_folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    m_folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);

}
void NotesItemView::init()
{
    //DEBUG_FUNC_NAME
    m_idC = m_notes->getIDList();
    create("-1", 0);
    m_idC.clear();
}

void NotesItemView::iterate(QStandardItem *item = 0)
{
    //DEBUG_FUNC_NAME
    if(item == NULL)
        return;
    const QString parentID = item->data().toString();
    for(int i = 0; i < item->rowCount(); ++i) {
        QStandardItem *m = item->child(i);
        m_notes->setRef(m->data().toString(), "parent", parentID);
        if(m->hasChildren())
            iterate(m);
    }
}
void NotesItemView::create(const QString &id, QStandardItem *parentItem)
{
    //DEBUG_FUNC_NAME
    foreach(const QString & i, m_idC) {
        if(id == "-1") {
            parentItem = m_itemModel->invisibleRootItem();
        }
        if((id == "-1" && m_notes->getRef(i, "parent").isEmpty()) || m_notes->getRef(i, "parent") == id) {
            if(m_notes->getType(i) == "text") {
                QStandardItem *noteItem = addNote(i, m_notes->getTitle(i), parentItem);
                create(i, noteItem);
            } else if(m_notes->getType(i) == "folder") {
                QStandardItem *folderItem = addFolder(i, m_notes->getTitle(i), parentItem);
                create(i, folderItem);
            }
        }

    }
}

QStandardItem * NotesItemView::addFolder(const QString &id, const QString &title, const QString &parentID)
{
    QStandardItem *parentItem;
    if(parentID.isEmpty()) {
        parentItem = m_itemModel->invisibleRootItem();
    } else {
        parentItem = find(parentID);
    }
    return addFolder(id,title,parentItem);
}

QStandardItem * NotesItemView::addFolder(const QString &id, const QString &title, QStandardItem *parentItem)
{
    QStandardItem *newItem = new QStandardItem;
    newItem->setIcon(m_folderIcon);
    newItem->setText(title);
    newItem->setData(id, Qt::UserRole + 1);

    parentItem->appendRow(newItem);
    return newItem;
}

QStandardItem * NotesItemView::addNote(const QString &id, const QString &title, const QString &parentID)
{
    QStandardItem *parentItem;
    if(parentID.isEmpty()) {
        parentItem = m_itemModel->invisibleRootItem();
    } else {
        parentItem = find(parentID);
    }
    return addNote(id,title,parentItem);
}

QStandardItem * NotesItemView::addNote(const QString &id, const QString &title, QStandardItem *parentItem)
{
    QStandardItem *newItem = new QStandardItem;
    newItem->setText(title);
    newItem->setData(id, Qt::UserRole + 1);

    parentItem->appendRow(newItem);
    return newItem;
}

void NotesItemView::removeNote(const QString &noteID)
{
    const QModelIndexList list = m_proxyModel->match(m_itemModel->invisibleRootItem()->index(), Qt::UserRole + 1, noteID, -1, Qt::MatchExactly);
    Q_ASSERT(list.size() == 1);
    if(list.isEmpty()) {
        myWarning() << "invalid noteID = " << noteID;
        return;
    }
    const QModelIndex index = list.first();
    m_itemModel->removeRow(index.row(), index.parent());
}

QStandardItem* NotesItemView::find(const QString &noteID)
{
    const QModelIndexList list = m_proxyModel->match(m_itemModel->invisibleRootItem()->index(), Qt::UserRole + 1, noteID, -1, Qt::MatchExactly);
    Q_ASSERT(list.size() == 1);
    if(list.isEmpty()) {
        myWarning() << "invalid noteID = " << noteID;
        return NULL;
    }
    myDebug() << m_itemModel->itemFromIndex(list.first());
  //  m_itemModel->itemFromIndex(m_proxyModel->mapToSource(m_treeView->indexAt(m_point)));

    return m_itemModel->itemFromIndex(list.first());
}
void NotesItemView::setTitle(const QString &noteID, const QString &title)
{
    const QModelIndexList list = m_proxyModel->match(m_itemModel->invisibleRootItem()->index(), Qt::UserRole + 1, noteID, -1, Qt::MatchExactly);
    if(list.size() != 1) {
        myWarning() << "invalid noteID = " << noteID;
        return;
    }
    m_itemModel->setData(list.first(), title, Qt::DisplayRole);
}

QStringList NotesItemView::selectedNotes() const
{
    QStringList ret;
    const QModelIndexList list = m_selectionModel->selectedRows(0);
    foreach(const QModelIndex &index, list) {
        ret.append(index.data(Qt::UserRole + 1).toString());
    }
    return ret;
}
void NotesItemView::save()
{
    iterate(m_itemModel->invisibleRootItem());
}
void NotesItemView::select(const QString &noteID)
{
    const QModelIndexList list = m_proxyModel->match(m_itemModel->invisibleRootItem()->index(), Qt::UserRole + 1, noteID,-1, Qt::MatchExactly);
    if(list.size() != 1) {
        return;
    }
    const QModelIndex index = m_proxyModel->mapFromSource(list.first());
    m_selectionModel->clearSelection();
    m_selectionModel->setCurrentIndex(index, QItemSelectionModel::Select);
}
void NotesItemView::notesContextMenu(QPoint point)
{
    m_point = point;
    QPointer<QMenu> contextMenu = new QMenu(m_treeView);
    m_currentPoint = point;
    contextMenu->setObjectName("contextMenu");

    QAction *actionCopy = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Copy Text"), contextMenu);
    actionCopy->setObjectName("actionCopy");
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copyNote()));

    QAction *actionPaste = new QAction(QIcon::fromTheme("edit-paste", QIcon(":/icons/16x16/edit-paste.png")), tr("Paste Text"), contextMenu);
    actionPaste->setObjectName("actionPaste");
    connect(actionPaste, SIGNAL(triggered()), this, SLOT(pasteNote()));

    QAction *actionNew = new QAction(QIcon::fromTheme("document-new", QIcon(":/icons/16x16/document-new.png")), tr("New Note"), contextMenu);
    actionNew->setObjectName("actionNew");
    connect(actionNew, SIGNAL(triggered()), this, SLOT(addNewTextNote()));

    QAction *actionNewFolder = new QAction(QIcon::fromTheme("document-new", QIcon(":/icons/16x16/document-new.png")), tr("New Folder"), contextMenu);
    actionNewFolder->setObjectName("actionNewFolder");
    connect(actionNewFolder, SIGNAL(triggered()), this, SLOT(addNewFolderNote()));

    QAction *actionDelete = new QAction(QIcon::fromTheme("edit-delete", QIcon(":/icons/16x16/edit-delete.png")), tr("Delete"), contextMenu);
    actionDelete->setObjectName("actionDelete");
    connect(actionDelete, SIGNAL(triggered()), this, SIGNAL(innerRemoveNotes()));

    contextMenu->addAction(actionNew);
    contextMenu->addAction(actionNewFolder);

    contextMenu->addSeparator();
    contextMenu->addAction(actionPaste);
    contextMenu->addAction(actionCopy);
    contextMenu->addAction(actionDelete);

    contextMenu->exec(QCursor::pos());
    delete contextMenu;
}
void NotesItemView::copyNote()
{
    emit copyNotes(this->selectedNotes());
}
void NotesItemView::pasteNote()
{
    QString parentID;
    if(sender()->objectName() == "actionPaste")
        parentID = m_treeView->indexAt(m_point).data(Qt::UserRole + 1).toString();
    emit pasteNote(parentID);
}

QString NotesItemView::newTextNote(const QString &id, const QString title, bool fromSender)
{
    m_selectionModel->clearSelection();
    QStandardItem *parentItem = NULL;

    if(fromSender)
        parentItem = m_itemModel->itemFromIndex(m_proxyModel->mapToSource(m_treeView->indexAt(m_point)));

    if(parentItem == NULL)
        parentItem = m_itemModel->invisibleRootItem();

    addNote(id, title, parentItem);

    select(id);

    return parentItem->data().toString();
}

QString NotesItemView::newFolderNote(const QString &id, const QString title, bool fromSender)
{
    m_selectionModel->clearSelection();
    QStandardItem *parentItem = NULL;

    if(parentItem == NULL)
        parentItem = m_itemModel->invisibleRootItem();

    addFolder(id, title, parentItem);

    select(id);
    return parentItem->data().toString();
}

QStringList NotesItemView::removeSelectedNotesFromView()
{
    QModelIndexList list = m_selectionModel->selectedRows();
    QStringList removedIDs;
    if(list.isEmpty()) {
        const QModelIndex index = m_proxyModel->mapToSource(m_treeView->indexAt(m_currentPoint));
        if(index.isValid()) {
            const QString id = index.data(Qt::UserRole + 1).toString();
            removedIDs << id;
            m_itemModel->removeRow(index.row(), index.parent());
        }

    } else {
        while(list.size() != 0) {
            const QModelIndex index = m_proxyModel->mapToSource(list.at(0));
            const QString id = index.data(Qt::UserRole + 1).toString();
            removedIDs << id;
            m_itemModel->removeRow(index.row(), index.parent());
            list = m_selectionModel->selectedRows(0);
        }
    }
    return removedIDs;
}

void NotesItemView::showNote(const QModelIndex &index)
{
    emit showNote(index.data(Qt::UserRole + 1).toString());
}

void NotesItemView::addNewFolderNote()
{
    QString parentID;
    if(sender()->objectName() == "actionNewFolder")
        parentID = m_treeView->indexAt(m_point).data(Qt::UserRole + 1).toString();

    emit innerAddNewFolderNote(parentID);
}

void NotesItemView::addNewTextNote()
{
    QString parentID;
    if(sender()->objectName() == "actionNew")
        parentID = m_treeView->indexAt(m_point).data(Qt::UserRole + 1).toString();

    emit innerAddNewTextNote(parentID);
}
void NotesItemView::aktNote(const QString &noteID, const QString &title)
{
    const QModelIndex index = getIndex(noteID);
    if(index.data(Qt::DisplayRole) != title) {
        m_itemModel->setData(index, title, Qt::DisplayRole);
    }
}
QModelIndex NotesItemView::getIndex(const QString &noteID)
{
    const QModelIndexList list = m_proxyModel->match(m_itemModel->invisibleRootItem()->index(), Qt::UserRole + 1, noteID, -1, Qt::MatchExactly);
    if(list.size() != 1) {
        myWarning() << "invalid noteID = " << noteID << " size = " << list.size();
        return QModelIndex();
    }
    return list.first();
}
