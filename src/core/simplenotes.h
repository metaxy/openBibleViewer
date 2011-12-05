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
#include "src/core/settings/settings.h"
#include "src/core/notes/notes.h"
#include "src/core/verseselection.h"
#include "src/core/basicclass.h"
#include "src/ui/recursivproxymodel.h"
#include "src/core/link/urlconverter2.h"
#include "src/ui/dialog/biblepassagedialog.h"
#include "src/core/obvcore.h"
#include "src/core/dbghelper.h"

#include <QtGui/QSortFilterProxyModel>
#include <QtCore/QDateTime>
#include <QtGui/QClipboard>
#include <QtGui/QMenu>
#include <QtGui/QColorDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
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
    void contentChanged();
public slots:
    /**
     * Creates a new text note, and insert an item in the ViewWidget
     */
    void newTextNote();
    /**
     * Show the note with given noteID in the frame.
     * if selectNote is true it select the note item in the ViewWidget
     */
    void showNote(const QString &noteID, bool selectNote = false);
    /**
     * Save current note, the note hiarchy and write them to hard disk.
     */
    void saveNote();

    /**
     * Similiar to saveNote() but without saveing the note hiarchy and writing to hard disk
     */
    void fastSave();
    /**
     * Same as newTextNote() but add also an verse link ref using the given selection
     */
    void newTextNoteWithLink(VerseSelection selection, QSharedPointer<Versification> v11n);
    /**
     * Creates a new mark note using the given selection and style
     */
    void newStyleMark(VerseSelection selection, const QString &style, QSharedPointer<Versification> v11n);
private slots:
    /**
     * Insert the text(not html) of the current select note in the clipboard
     */
    void copyNote();
    void changeData(const QString &id, const QString &data);
    void changeTitle(const QString &id, const QString &title);
    void changeRef(const QString &id, const QMap<QString, QString> &ref);
    void addNote(const QString &id);
    void removeNote(const QString &id);
    void showNote(const QModelIndex &index);
    void notesContextMenu(QPoint point);
    void removeNote();
    /**
      * Shows an BiblePassageDialog where you can edit the note link.
      */
    void editNoteLink();
    void updateNoteLink(const VerseUrl &url);
    /**
      * Adds a new Folder with the default title (unnamed)
      */
    void newFolder();
    void updateTitle();

    void open(const QString &link);

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
    bool m_loadTextBrowser;
    void create(const QString &id, QStandardItem *parentItem);
    QIcon m_folderIcon;
    QStringList m_idC;
    QPoint m_point;

    /**
      * Save the tree structure of all notes.
      */
    void iterate(QStandardItem *item);


};

#endif // SIMPLENOTES_H
