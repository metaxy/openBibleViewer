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
    StandardItem * addNote(const QString &id, const QString &title, const QStandardItem *parentItem);
    QStandardItem * addFolder(const QString &id, const QString &title, const QString &parentID);
    QStandardItem * addFolder(const QString &id, const QString &title, const QStandardItem *parent);
    void removeNote(const QPoint p);
    void removeNote(const QString &noteID);

    void setTitle(const QString &noteID, const QString &title);
    QList<int> selectedNotes();
    void create(const QString &id, QStandardItem *parentItem);

    /**
      * Save the tree structure of all notes.
      */
    void iterate(QStandardItem *item);

    QStandardItem *find(const QString &noteID);


signals:
    void showNote(const in noteID);
private:
    QTreeView m_treeView;
    QStandardItemModel *m_itemModel;
    RecursivProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
    QIcon m_folderIcon;
    Notes *m_notes;
};

#endif // NOTESITEMVIEW_H
