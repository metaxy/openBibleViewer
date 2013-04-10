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
#ifndef NOTESITEMVIEW_H
#define NOTESITEMVIEW_H
#include <QTreeView>
#include <QStandardItemModel>
#include "src/core/notes/notes.h"
#include "src/ui/recursivproxymodel.h"
class NotesItemView : public QObject
{
        Q_OBJECT
public:
    NotesItemView(Notes *notes, QTreeView *treeView);
public slots:
    QStandardItem * addNote(const QString &id, const QString &title, const QString &parentID);
    QStandardItem * addNote(const QString &id, const QString &title, QStandardItem *parentItem);
    QStandardItem * addFolder(const QString &id, const QString &title, const QString &parentID);
    QStandardItem * addFolder(const QString &id, const QString &title, QStandardItem *parent);
    void removeNote(const QString &noteID);
    void init();

    void setTitle(const QString &noteID, const QString &title);
    QStringList selectedNotes() const;
    void create(const QString &id, QStandardItem *parentItem);

    /**
      * Save the tree structure of all notes.
      */
    void iterate(QStandardItem *item);

    QStandardItem *find(const QString &noteID);

    void save();

    void select(const QString &noteID);

    void copyNote();
    QStringList removeSelectedNotesFromView();

    QString newTextNote(const QString &id, const QString title, bool fromSender);
    QString newFolderNote(const QString &id, const QString title, bool fromSender);

    void showNote(const QModelIndex &index);

    void notesContextMenu(QPoint point);
    void addNewTextNote();
    void addNewFolderNote();

    void aktNote(const QString &noteID, const QString &title);
    void pasteNote();

signals:
    void showNote(const QString &noteID);
    void copyNotes(const QStringList &ids);
    void pasteNote(const QString &parentID);


    /**
      * Only to be used by SimpleNotes
      */
    void innerAddNewTextNote(const QString &parentID);
    void innerAddNewFolderNote(const QString &parentID);
    void innerRemoveNotes();
private:
    QTreeView *m_treeView;
    QStandardItemModel *m_itemModel;
    RecursivProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
    QIcon m_folderIcon;
    Notes *m_notes;
    QStringList m_idC;
    QPoint m_currentPoint;

    QPoint m_point;

    QModelIndex getIndex(const QString &noteID);
};

#endif // NOTESITEMVIEW_H
