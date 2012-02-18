#ifndef TREEBOOKFORM_H
#define TREEBOOKFORM_H
#include "webviewform.h"
#include "src/module/book/treebook.h"
#include "src/core/history.h"
#include <QWidget>

namespace Ui {
class TreeBookForm;
}

class TreeBookForm : public WebViewForm
{
    Q_OBJECT
    
public:
    explicit TreeBookForm(QWidget *parent = 0);
    ~TreeBookForm();

    void init();
    void restore(const QString &key);
    void save();

    Form::FormType type() const;


    void loadModule(const int moduleID);

    void showChapter(const int moduleID, const int chapterID);
public slots:
    void activated();
private slots:
    void loadChapter(QModelIndex index);
    void backward();
    void forward();
    void showContextMenu(QContextMenuEvent* ev);
    void get(QUrl url);
    void newGet(QUrl url);
private:
    Ui::TreeBookForm *ui;

    TreeBook *m_book;

    int m_chapterID;

    void setButtons();
    void historySetUrl(QString url);
    History m_browserHistory;

    void createTree(QStandardItem *parentItem, BookTree *parentTree);
    void selectChapter(const int chapterID);
    QString transformUrl(const QString &url);

    RecursivProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
    QStandardItemModel *m_treeModel;
};

#endif // TREEBOOKFORM_H
