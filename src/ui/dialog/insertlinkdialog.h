#ifndef INSERTLINKDIALOG_H
#define INSERTLINKDIALOG_H

#include <QDialog>
#include "src/core/basicclass.h"
#include <QtGui/QTreeView>
#include <QtGui/QSortFilterProxyModel>

namespace Ui
{
class InsertLinkDialog;
}

class InsertLinkDialog : public QDialog, public BasicClass
{
    Q_OBJECT
private slots:
    void indexChanged(int index);
    void save();
signals:
    void newLink(QString link);
public:
    explicit InsertLinkDialog(QWidget *parent = 0);
    ~InsertLinkDialog();
    void init();
    void setCurrent(const int &bible, const QString &path, const int &book, const int &chapter, const int &verse);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::InsertLinkDialog *ui;
    QStringList m_bibles;
    int m_bibleID, m_bookID, m_chapterID, m_verseID;
    QString m_path;

    QStandardItemModel *m_itemModel;
    QTreeView *m_treeView;
    QSortFilterProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
};

#endif // INSERTLINKDIALOG_H
