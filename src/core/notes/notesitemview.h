#ifndef NOTESITEMVIEW_H
#define NOTESITEMVIEW_H
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtGui/QTreeView>
#include <QtGui/QStandardItemModel>
#include "src/ui/recursivproxymodel.h"
class NotesItemView : public QObject
{
        Q_OBJECT
public:
    NotesItemView(QTreeView *treeView);
public slots:
    void addNote(const int id, const QString &title, const int parentID);
    void addFolder(const int id, const QString &title, const int oarentID);
    void removeNote(const QPoint p);
    void removeNote(const int noteID);


signals:
    void showNote(const in noteID);
private:
    QTreeView m_treeView;
    QStandardItemModel *m_itemModel;
    RecursivProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
    QIcon m_folderIcon;
};

#endif // NOTESITEMVIEW_H
