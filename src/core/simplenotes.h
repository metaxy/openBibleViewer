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
#include "src/module/modulemanager.h"
#include "src/core/verseselection.h"
#include "src/core/basicclass.h"
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
private:
    QLineEdit *m_lineEdit_title;
    QTextBrowser *m_textEdit_note;
    QWebFrame *m_frame;
    QLabel *m_label_link;
    QPushButton *m_pushButton_link;
    QPoint currentPoint;

    QStandardItemModel *m_itemModel;
    QTreeView *m_treeView;
    QSortFilterProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;

    QString m_noteID;
    QMap<QString, QString> currentNoteRef;
    void setTitle(QString title);
    void setData(QString data);
    void setRef(QMap<QString, QString> ref);
    void aktNote();
    void select(QString noteID);
    bool loadTextBrowser;

};

#endif // SIMPLENOTES_H
