#include "notesitemview.h"

NotesItemView::NotesItemView(QTreeView *treeView) : m_treeView(treeView)
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
