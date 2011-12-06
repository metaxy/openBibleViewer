#ifndef NOTESITEMVIEW_H
#define NOTESITEMVIEW_H
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtGui/QTreeView>
#include <QtGui/QStandardItemModel>
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
