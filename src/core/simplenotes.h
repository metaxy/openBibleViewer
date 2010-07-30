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
#ifndef SIMPLENOTES_H
#define SIMPLENOTES_H

#include <QObject>
#include <QtGui/QLineEdit>
#include <QtGui/QTextBrowser>
#include <QtGui/QTreeView>
#include <QtGui/QPushButton>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QLabel>
#include <QtGui/QItemSelectionModel>
#include <QtWebKit/QWebFrame>
#include "src/core/settings.h"
#include "src/core/notes.h"
#include "src/core/verseselection.h"
#include "src/core/basicclass.h"
#include "src/ui/recursivproxymodel.h"
class SimpleNotes : public QObject, public BasicClass
{
    Q_OBJECT

public:
    explicit SimpleNotes();
    void setTitleWidget(QLineEdit *title);
    void setDataWidget(QTextBrowser *data);
    void setFrameWidget(QWebFrame *frame);
    void setViewWidget(QTreeView *treeView);
    void setLinkWidget(QLabel* link);
    void setLinkButtonWidget(QPushButton* button);
    void init();

signals:
    void reloadChapter();
public slots:
    void newNote();
    void showNote(const QString &noteID, bool selectNote = false);
    void saveNote();
    void newNoteWithLink(VerseSelection selection);
    void fastSave();
private slots:
    void copyNote();
    void changeData(QString id, QString data);
    void changeTitle(QString id, QString title);
    void changeRef(QString id, QMap<QString, QString> ref);
    void addNote(QString id);
    void removeNote(QString id);
    void showNote(QModelIndex index);
    void notesContextMenu(QPoint point);
    void removeNote();
    void editNoteLink();
    void updateNote(QString pos);
    void newFolder();
    void updateTitle();
private:
    QLineEdit *m_lineEdit_title;
    QTextBrowser *m_textEdit_note;
    QWebFrame *m_frame;
    QLabel *m_label_link;
    QPushButton *m_pushButton_link;
    QPoint m_currentPoint;

    QStandardItemModel *m_itemModel;
    QTreeView *m_treeView;
    RecursivProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;

    QString m_noteID;
    QMap<QString, QString> m_noteRef;
    void setTitle(QString title);
    void setData(QString data);
    void setRef(QMap<QString, QString> ref);
    void aktNote();
    void select(const QString &noteID);
    bool loadTextBrowser;
    void create(const QString &id, QStandardItem *parentItem);
    QIcon folderIcon;
    QStringList m_idC;
    QPoint m_point;
    void iterate(QStandardItem *item);


};

#endif // SIMPLENOTES_H
