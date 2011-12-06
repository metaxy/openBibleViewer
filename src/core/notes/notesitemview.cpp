#include "notesitemview.h"

NotesItemView::NotesItemView(Notes *notes, QTreeView *treeView) : m_notes(notes), m_treeView(treeView)
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
    m_idC = m_notes->getIDList();
    create("-1", 0);
    m_idC.clear();
}

void NotesItemView::iterate(QStandardItem *item = 0)
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
QStandardItem * NotesItemView::addFolder(const QString &id, const QString &title, const QStandardItem* parentItem)
{
    QStandardItem *newItem = new QStandardItem;
    newItem->setIcon(m_folderIcon);
    newItem->setText(title);
    newItem->setData(id);

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
QStandardItem * NotesItemView::addNote(const QString &id, const QString &title, const QStandardItem* parentItem)
{
    QStandardItem *newItem = new QStandardItem;
    newItem->setText(title);
    newItem->setData(id);

    parentItem->appendRow(newItem);
    return newItem;
}

void NotesItemView::removeNote(const QString &noteID)
{
    const QModelIndexList list = m_proxyModel->match(m_itemModel->invisibleRootItem()->index(), Qt::UserRole + 1, noteID, -1, Qt::MatchExactly);
    if(list.size() != 1) {
        myWarning() << "invalid noteID = " << m_noteID;
        return;
    }
    const QModelIndex index = list.first();
    m_itemModel->removeRow(index.row(), index.parent());
}
QStandardItem* NotesItemView::find(const QString &noteID)
{
    return NULL;
}
void NotesItemView::setTitle(const QString &noteID, const QString &title)
{
    const QModelIndexList list = m_proxyModel->match(m_itemModel->invisibleRootItem()->index(), Qt::UserRole + 1, noteID, -1, Qt::MatchExactly);
    if(list.size() != 1) {
        myWarning() << "invalid noteID = " << id;
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
