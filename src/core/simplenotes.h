#ifndef SIMPLENOTES_H
#define SIMPLENOTES_H

#include <QObject>
#include <QtGui/QLineEdit>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeView>
#include <QtGui/QPushButton>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QLabel>
#include <QItemSelectionModel>
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
    void setDataWidget(QTextEdit *data);
    void setViewWidget(QTreeView *treeView);
    void setLinkWidget(QLabel* link);
    void setLinkButtonWidget(QPushButton* button);
    void setSettings(Settings *settings);
    void setNotes(Notes *notes);
    void setModuleManager(ModuleManager *moduleManager);
    void init();

signals:
    void reloadChapter();
public slots:
     void newNote();
     void showNote(const QString &noteID);
     void saveNote();
     void newNoteWithLink(VerseSelection selection);
     void fastSave();
private slots:
    void copyNote();
    void changeData(QString id, QString data);
    void changeTitle(QString id, QString title);
    void changeRef(QString id, QMap<QString, QString> ref);
    void showNote(QModelIndex index);
    void notesContextMenu();
    void removeNote();
    void editNoteLink();
    void updateNote(QString pos);
private:
    QLineEdit *m_lineEdit_title;
    QTextEdit *m_textEdit_note;
    QLabel *m_label_link;
    QPushButton *m_pushButton_link;


    Settings *m_settings;
    Notes *m_notes;
    ModuleManager *m_moduleManager;

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

};

#endif // SIMPLENOTES_H
